// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "IKAGameMode.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class IKA_API AIKAGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	virtual void PreInitializeComponents() override;
	
	/** update remaining time */
	virtual void DefaultTimer();
	
	/** starts new match */
	virtual void HandleMatchHasStarted() override;

	void RestartLevel();

	uint8 GetPlayerFirstScore() { return PlayerFirstScore;	}

	uint8 GetPlayerSecondScore() { return PlayerSecondScore;}

protected:
	/** match duration */
	UPROPERTY(EditAnywhere, Category = GameMode)
	int32 RoundTime;

	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

	void DetermineMatchWinner();

	void FinishMatch();

public:
	enum EMatchResult
	{
		WIN,
		DRAW,
		LOSE
	};

	EMatchResult GetMatchResult() const {	return MatchResult;	}
	class AIKACharacter* GetLastWinCharacter() { return LastWinCharacter; }

	APlayerController* GetMainPlayerController() { return MainPlayerController;	};
private:
	EMatchResult MatchResult;
	AIKACharacter* LastWinCharacter;
	APlayerController* MainPlayerController;
	uint8 PlayerFirstScore;
	uint8 PlayerSecondScore;
};
