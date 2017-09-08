// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "IKABTTask_FindPointNearEnemy.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API UIKABTTask_FindPointNearEnemy : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
