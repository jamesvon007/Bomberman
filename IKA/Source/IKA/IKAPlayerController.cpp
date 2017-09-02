// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IKAPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "IKACharacter.h"

AIKAPlayerController::AIKAPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AIKAPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AIKAPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindAxis("MoveForwardOne", this, &AIKAPlayerController::MoveForwardOne);
	InputComponent->BindAxis("MoveRightOne", this, &AIKAPlayerController::MoveRightOne);
	InputComponent->BindAxis("MoveForwardTwo", this, &AIKAPlayerController::MoveForwardTwo);
	InputComponent->BindAxis("MoveRightTwo", this, &AIKAPlayerController::MoveRightTwo);
}

void AIKAPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
	
}

void AIKAPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AIKAPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AIKAPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AIKAPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AIKAPlayerController::MoveForwardOne(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		APawn* pawn = GetPawn();
		if (pawn)
		{
			pawn->AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
		}
	}
}

void AIKAPlayerController::MoveRightOne(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		APawn* pawn = GetPawn();
		if (pawn)
		{
			pawn->AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
		}
	}
}

void AIKAPlayerController::MoveForwardTwo(float Value)
{
	if (Value != 0.0f)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC && PC != this)
			{
				// add movement in that direction
				APawn* pawn = PC->GetPawn();
				if (pawn)
				{
					pawn->AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
				}
				return;
			}
		}
	}
}

void AIKAPlayerController::MoveRightTwo(float Value)
{
	if (Value != 0.0f)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC && PC != this)
			{
				// add movement in that direction
				APawn* pawn = PC->GetPawn();
				if (pawn)
				{
					pawn->AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
				}
				return;
			}
		}
	}
}