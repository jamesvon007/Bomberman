// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "IKAGameState.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKAGameState : public AGameState
{
	GENERATED_UCLASS_BODY()
	
public:
	/** time left for warmup / match */
	UPROPERTY(Transient)
	int32 RemainingTime;
	
	
};
