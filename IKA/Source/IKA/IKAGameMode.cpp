// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAGameMode.h"
#include "IKAPlayerController.h"
#include "IKAHUD.h"
#include "IKAGameState.h"


AIKAGameMode::AIKAGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = AIKAPlayerController::StaticClass();
	HUDClass = AIKAHUD::StaticClass();
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
			}
		}
	}
}

void AIKAGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if (AIKAGameState* const IKAGameState = Cast<AIKAGameState>(GameState))
	{
		IKAGameState->RemainingTime = RoundTime;
	}

}

void AIKAGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AIKAGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}
