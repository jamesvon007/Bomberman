// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "IKABTTask_FindPointAwayFromBomb.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API UIKABTTask_FindPointAwayFromBomb : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
