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

	Health = 100.f;
	BlastRangeMultiplier = 1.f;
	BombAmount = 1;
	MoveSpeedMultiplier = 1.f;
	UseRemoteControlledBomb = false;
	RemoteControlledBombAbilityActiveDuration = 10.f;
}

void AIKACharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

bool AIKACharacter::IsAlive() const
{
	return Health > 0;
}

void AIKACharacter::Die()
{
	Health = -1.f;
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

void AIKACharacter::RestoreBombAmount()
{
	++BombAmount;
}

void AIKACharacter::EnableUsingRemoteControlledBomb(float Duration)
{
	UseRemoteControlledBomb = true;
	RemoteControlledBombAbilityActiveDuration = Duration;
}

void AIKACharacter::PlaceBomb()
{
	if (BombAmount > 0)
	{
		FVector SpawnLocation = GetActorLocation();
		AActor* Actor = GetWorld()->SpawnActor(AIKABomb::StaticClass(), &SpawnLocation);
		if (AIKABomb * Bomb = Cast<AIKABomb>(Actor))
		{
			--BombAmount;
			Bomb->Instigator = this;
		}
	}

}
