// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKACharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "IKABotCharacter.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKABotCharacter : public AIKACharacter
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* BotBehavior;
	
	
};
