// Fill out your copyright notice in the Description page of Project Settings.

#include "IKABTTask_FindPointNearEnemy.h"
#include "Pawn/IKAAIController.h"
#include "Pawn/IKAPlayerCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UIKABTTask_FindPointNearEnemy::UIKABTTask_FindPointNearEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

EBTNodeResult::Type UIKABTTask_FindPointNearEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIKAAIController* MyController = Cast<AIKAAIController>(OwnerComp.GetAIOwner());
	if (MyController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyBot = MyController->GetPawn();
	AIKAPlayerCharacter* Enemy = MyController->GetEnemy();
	if (Enemy && MyBot)
	{
		const float SearchRadius = 10.0f;
		FVector SearchDirection = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), 0.f);

		const FVector SearchOrigin = Enemy->GetActorLocation() + 150.f * SearchDirection;
		FVector Loc(0);
		UNavigationSystem::K2_GetRandomReachablePointInRadius(MyController, SearchOrigin, Loc, SearchRadius);
		if (Loc != FVector::ZeroVector)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}

