// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IKAPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "IKAPlayerCharacter.h"
#include "UI/IKAHUD.h"
#include "IKAGameMode.h"

AIKAPlayerController::AIKAPlayerController()
{
}

void AIKAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AIKAHUD* HUD = Cast<AIKAHUD>(GetHUD());
	if (HUD)
	{
		if (AIKACharacter* Character = Cast<AIKACharacter>(GetPawn()))
		{
			HUD->RegisterCharacter(Character);
		}
	}
}

void AIKAPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AIKAPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	check(InputComponent);
	InputComponent->BindAxis("MoveForwardOne", this, &AIKAPlayerController::MoveForwardOne);
	InputComponent->BindAxis("MoveRightOne", this, &AIKAPlayerController::MoveRightOne);
	InputComponent->BindAction("PlaceBombOne", IE_Released, this, &AIKAPlayerController::PlaceBombOne);
	InputComponent->BindAction("RemoteTriggerOne", IE_Released, this, &AIKAPlayerController::RemoteTriggerOne);
	InputComponent->BindAxis("MoveForwardTwo", this, &AIKAPlayerController::MoveForwardTwo);
	InputComponent->BindAxis("MoveRightTwo", this, &AIKAPlayerController::MoveRightTwo);
	InputComponent->BindAction("PlaceBombTwo", IE_Released, this, &AIKAPlayerController::PlaceBombTwo);
	InputComponent->BindAction("RemoteTriggerTwo", IE_Released, this, &AIKAPlayerController::RemoteTriggerTwo);
	InputComponent->BindAction("ResetGame", IE_Released, this, &AIKAPlayerController::RestartLevel);
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

void AIKAPlayerController::PlaceBombOne()
{
	AIKAPlayerCharacter* Player = Cast<AIKAPlayerCharacter>(GetPawn());
	if (Player)
	{
		AIKAGameMode* GameMode = Cast<AIKAGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			if (GameMode->GetMatchState() == MatchState::InProgress)
			{
				// place bomb
				Player->OnPlaceBomb();
			}
		}
	}
}

void AIKAPlayerController::RemoteTriggerOne()
{
	AIKAPlayerCharacter* Player = Cast<AIKAPlayerCharacter>(GetPawn());
	if (Player)
	{
		Player->TriggerRemoteBomb();
	}
}

void AIKAPlayerController::RestartLevel()
{
	AIKAGameMode* GameMode = Cast<AIKAGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->RestartLevel();
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

void AIKAPlayerController::RemoteTriggerTwo()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC != this)
		{
			// trigger bomb
			AIKAPlayerCharacter* Player = Cast<AIKAPlayerCharacter>(PC->GetPawn());
			if (Player)
			{
				Player->TriggerRemoteBomb();
			}
			return;
		}
	}
}

void AIKAPlayerController::PlaceBombTwo()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC != this)
		{
			// place bomb
			AIKAPlayerCharacter* Player = Cast<AIKAPlayerCharacter>(PC->GetPawn());
			if (Player)
			{
				AIKAGameMode* GameMode = Cast<AIKAGameMode>(GetWorld()->GetAuthGameMode());
				if (GameMode)
				{
					if (GameMode->GetMatchState() == MatchState::InProgress)
					{
						Player->OnPlaceBomb();
					}
				}
			}
			return;
		}
	}
}
