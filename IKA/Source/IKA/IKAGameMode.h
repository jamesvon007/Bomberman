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

protected:
	/** match duration */
	UPROPERTY(config)
	int32 RoundTime;

	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;
};
