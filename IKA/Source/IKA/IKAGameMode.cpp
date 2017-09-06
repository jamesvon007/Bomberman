// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAGameMode.h"
#include "IKAPlayerController.h"
#include "IKAHUD.h"
#include "IKAGameState.h"
#include "IKACharacter.h"
#include "IKAPlayerCharacter.h"


AIKAGameMode::AIKAGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = AIKAPlayerController::StaticClass();
	HUDClass = AIKAHUD::StaticClass();
	GameStateClass = AIKAGameState::StaticClass();
	RoundTime = 10;
	PlayerFirstScore = 0;
	PlayerSecondScore = 0;
}

void AIKAGameMode::DefaultTimer()
{
	if (AIKAGameState* const IKAGameState = Cast<AIKAGameState>(GameState))
	{
		if (IKAGameState->RemainingTime > 0)
		{
			IKAGameState->RemainingTime--;

			if (IKAGameState->RemainingTime <= 0)
			{
				FName MatchName = GetMatchState();
				if (MatchName == MatchState::InProgress)
				{
					FinishMatch();
				}
			}
		}
	}
}

void AIKAGameMode::RestartGame()
{
	if (GetMatchState() == MatchState::WaitingPostMatch)
	{
		Super::RestartGame();
	}
}

void AIKAGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if (AIKAGameState* const IKAGameState = Cast<AIKAGameState>(GameState))
	{
		IKAGameState->RemainingTime = RoundTime;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APlayerController* PC = Iterator->Get())
		{
			if (AIKAPlayerCharacter* Character = Cast<AIKAPlayerCharacter>(PC->GetPawn()))
			{
				MainPlayerController = PC;
			}
		}
	}

}

void AIKAGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AIKAGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

	
}

void AIKAGameMode::FinishMatch()
{
	if (IsMatchInProgress())
	{
		DetermineMatchWinner();
		EndMatch();

		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (APlayerController* PC = Iterator->Get())
			{
				// Disable inputs
				PC->SetIgnoreLookInput(true);
				PC->SetIgnoreMoveInput(true);
			}
		}

	}
	
}

void AIKAGameMode::DetermineMatchWinner()
{
	uint8 AliveCount = 0;
	bool DieFromSameBomb = false;
	class AIKABomb* LastPlayerDieFrom = nullptr;
	

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APlayerController* PC = Iterator->Get())
		{
			if (AIKACharacter* Character = Cast<AIKACharacter>(PC->GetPawn()))
			{
				if (Character)
				{
					if (Character->IsAlive())
					{
						if (Character->AutoPossessPlayer == EAutoReceiveInput::Type::Player0)
						{
							++PlayerFirstScore;
						}
						else
						{
							++PlayerSecondScore;
						}
						LastWinCharacter = Character;
						++AliveCount;
					}
					else
					{
						if (LastPlayerDieFrom != nullptr && LastPlayerDieFrom == Character->DieFrom())
						{
							DieFromSameBomb = true;
						}
						else
						{
							LastPlayerDieFrom = Character->DieFrom();
						}
					}
				}
			}
		}
	}

	switch (AliveCount)
	{
	case 0:
		if (DieFromSameBomb)
		{
			MatchResult = DRAW;
		}
		else
		{
			MatchResult = LOSE;
		}
		break;

	case 1:
		MatchResult = WIN;
		break;

	case 2:
		MatchResult = DRAW;
		--PlayerFirstScore;
		--PlayerSecondScore;
		break;

	default:
		MatchResult = WIN;
	}
}
