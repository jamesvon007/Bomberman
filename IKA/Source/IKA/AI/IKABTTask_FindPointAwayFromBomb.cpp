// Fill out your copyright notice in the Description page of Project Settings.

#include "IKABTTask_FindPointAwayFromBomb.h"
#include "Pawn/IKAAIController.h"
#include "GameObject//IKABomb.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UIKABTTask_FindPointAwayFromBomb::UIKABTTask_FindPointAwayFromBomb(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

EBTNodeResult::Type UIKABTTask_FindPointAwayFromBomb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIKAAIController* MyController = Cast<AIKAAIController>(OwnerComp.GetAIOwner());
	if (MyController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	if (AIKABomb* Bomb = MyController->GetClosestBomb())
	{
		if (APawn* MyBot = MyController->GetPawn())
		{
			const float SearchRadius = 50.0f;
			const FVector SearchOrigin = Bomb->GetActorLocation() + 200.f * FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), 0.f);

			FVector Loc(0);
			UNavigationSystem::K2_GetRandomPointInNavigableRadius(MyController, SearchOrigin, Loc, SearchRadius);
			if (Loc != FVector::ZeroVector)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}

