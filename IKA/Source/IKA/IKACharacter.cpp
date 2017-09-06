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
#include "IKABomb.h"
#include "IKAGameState.h"

AIKACharacter::AIKACharacter()
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

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

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
}

bool AIKACharacter::IsAlive() const
{
	return Health > 0;
}

void AIKACharacter::Die(AIKABomb* Bomb)
{
	KilledBy = Bomb;
	Health = -1.f;

	AIKAGameState* const GameState = GetWorld()->GetGameState<AIKAGameState>();
	{
		GameState->RemainingTime = 1.f;
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
		if (Bomb->IsRemoteTrigger() && UseRemoteControlledBomb)
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
	AddOwnedComponent(TimedPowerupRemainTimeComp);
	//TimedPowerupRemainTimeComp->SetupAttachment(GetCapsuleComponent());
	TimedPowerupRemainTimeComp->RegisterComponent();
	TimedPowerupRemainTimeComp->SetMobility(EComponentMobility::Movable);
	TimedPowerupRemainTimeComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TimedPowerupRemainTimeComp->bSelectable = false;

	TimedPowerupRemainTimeComp->bGenerateOverlapEvents = false;
	TimedPowerupRemainTimeComp->SetCanEverAffectNavigation(false);
	TimedPowerupRemainTimeComp->bCastDynamicShadow = false;
	TimedPowerupRemainTimeComp->bCastStaticShadow = false;
	TimedPowerupRemainTimeComp->bAffectDistanceFieldLighting = false;
	TimedPowerupRemainTimeComp->bAffectDynamicIndirectLighting = false;

	TimedPowerupRemainTimeComp->SetFont(TextFont);
	TimedPowerupRemainTimeComp->SetWorldSize(80.0f);
	//TimedPowerupRemainTime->SetTextMaterial(UserScaleIndicatorMaterial);
	TimedPowerupRemainTimeComp->SetHorizontalAlignment(EHTA_Center);
	TimedPowerupRemainTimeComp->SetVisibility(false);

	TimedPowerupRemainTime.TextRenderComponent = TimedPowerupRemainTimeComp;
}

