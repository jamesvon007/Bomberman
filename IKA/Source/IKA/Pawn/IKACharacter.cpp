// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IKACharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "IKA.h"
#include "GameObject//IKABomb.h"
#include "IKAGameState.h"
#include "IKAPlayerCharacter.h"
#include "IKAAIController.h"

AIKACharacter::AIKACharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_BLAST, ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_BOMB, ECR_Overlap);

	Health = 100.f;
	BlastRangeMultiplier = 1.f;
	BombAmount = 1;
	RemoteBombAmount = 0;
	MoveSpeedMultiplier = 1.f;
	UseRemoteControlledBomb = false;
	RemoteControlledBombAbilityActiveDuration = 10.f;

}

void AIKACharacter::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerUI();
}

void AIKACharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (UseRemoteControlledBomb)
	{
		TimedPowerupRemainTime.RemainTime -= DeltaSeconds;
	}

	if (PowerupDisplay.DisplayTime > 0.f)
	{
		PowerupDisplay.DisplayTime -= DeltaSeconds;
	}
}

bool AIKACharacter::IsAlive() const
{
	return Health > 0;
}

void AIKACharacter::Die(AIKABomb* Bomb)
{
	KilledBy = Bomb;
	Health = -1.f;

	if (AIKAPlayerCharacter* Player = Cast<AIKAPlayerCharacter>(this))
	{
		AIKAGameState* const GameState = GetWorld()->GetGameState<AIKAGameState>();
		{
			GameState->RemainingTime = 1.f;
		}
	}

	if (AIKAAIController* AIController = Cast<AIKAAIController>(GetController()))
	{
		if (UBrainComponent* BrainComp = AIController->BrainComponent)
		{
			AIController->GetBehaviorComp()->StopTree();
			TurnOff();
			SetLifeSpan(1.0f);
		}
	}
}

void AIKACharacter::AddBlastRangeMultiplier(float Multiplier)
{
	BlastRangeMultiplier += Multiplier;
}

void AIKACharacter::AddMoveSpeedMultiplier(float Multiplier)
{
	MoveSpeedMultiplier += Multiplier;
	GetCharacterMovement()->MaxWalkSpeed *= MoveSpeedMultiplier;
}

void AIKACharacter::AddBombAmount(uint8 Increment)
{
	BombAmount += Increment;
}

void AIKACharacter::RestoreBombAmount(AIKABomb* Bomb)
{
	if (Bomb)
	{
		if (Bomb->IsRemoteTrigger())
		{
			++RemoteBombAmount;
		}
		else
		{
			++BombAmount;
		}
	}
}

void AIKACharacter::ConsumeBombAmount(AIKABomb* Bomb)
{
	if (Bomb)
	{
		if (Bomb->IsRemoteTrigger() && UseRemoteControlledBomb)
		{
			--RemoteBombAmount;
		}
		else
		{
			--BombAmount;
		}
	}
}

bool AIKACharacter::CheckBombAmount()
{
	if (UseRemoteControlledBomb)
	{
		return RemoteBombAmount > 0;
	}
	else
	{
		return BombAmount > 0;
	}
}

void AIKACharacter::EnableUsingRemoteControlledBomb(float Duration)
{
	UseRemoteControlledBomb = true;
	RemoteControlledBombAbilityActiveDuration = Duration;
	RemoteBombAmount = 1;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AIKACharacter::DisableUsingRemoteControlledBomb, FMath::Max(3.f, RemoteControlledBombAbilityActiveDuration), false);

	TimedPowerupRemainTime.RemainTime = Duration;
}

void AIKACharacter::DisableUsingRemoteControlledBomb()
{
	UseRemoteControlledBomb = false;
	RemoteControlledBombAbilityActiveDuration = 0.f;
	RemoteBombAmount = 0;
}

void AIKACharacter::TriggerRemoteBomb()
{
	if (RemoteBomb)
	{
		RemoteBomb->Trigger();
	}
}

AIKACharacter::UTimedPowerupRemainTime AIKACharacter::GetTimedPowerupRemainTime()
{
	return TimedPowerupRemainTime;
}

void AIKACharacter::InitPlayerUI()
{
	UTextRenderComponent* TimedPowerupRemainTimeComp = NewObject<UTextRenderComponent>(this);
	InitTextRenderComponent(TimedPowerupRemainTimeComp);
	TimedPowerupRemainTime.TextRenderComponent = TimedPowerupRemainTimeComp;


	UTextRenderComponent* PowerupDisplayComp = NewObject<UTextRenderComponent>(this);
	InitTextRenderComponent(PowerupDisplayComp);
	PowerupDisplayComp->SetTextRenderColor(FColor::Yellow);
	PowerupDisplay.TextRenderComponent = PowerupDisplayComp;
}

void AIKACharacter::InitTextRenderComponent(UTextRenderComponent* Comp)
{
	AddOwnedComponent(Comp);
	Comp->RegisterComponent();
	Comp->SetMobility(EComponentMobility::Movable);
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Comp->bSelectable = false;

	Comp->bGenerateOverlapEvents = false;
	Comp->SetCanEverAffectNavigation(false);
	Comp->bCastDynamicShadow = false;
	Comp->bCastStaticShadow = false;
	Comp->bAffectDistanceFieldLighting = false;
	Comp->bAffectDynamicIndirectLighting = false;

	Comp->SetFont(TextFont);
	Comp->SetWorldSize(80.0f);
	Comp->SetHorizontalAlignment(EHTA_Center);
	Comp->SetVisibility(false);
}

