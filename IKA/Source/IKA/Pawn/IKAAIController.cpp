// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAAIController.h"
#include "IKABotCharacter.h"
#include "IKAGameState.h"
#include "IKAPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameObject//IKABomb.h"

AIKAAIController::AIKAAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));

	BrainComponent = BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}

void AIKAAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AIKABotCharacter* Bot = Cast<AIKABotCharacter>(InPawn);

	// start behavior
	if (Bot && Bot->BotBehavior)
	{
		if (Bot->BotBehavior->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		}

 		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		ShortestBombKeyID = BlackboardComp->GetKeyID("Bomb");

		BehaviorComp->StartTree(*(Bot->BotBehavior));
	}
}

void AIKAAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

void AIKAAIController::TryPlacingBomb()
{
	AIKABotCharacter* MyBot = Cast<AIKABotCharacter>(GetPawn());
	if (MyBot)
	{
		MyBot->OnPlaceBomb();
	}
}

void AIKAAIController::DetectClosestBomb()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	TArray<AActor*> BombActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIKABomb::StaticClass(), BombActors);
	float ShortestDistSq = MAX_FLT;
	AIKABomb* ClosestBomb = NULL;
	const FVector MyLoc = MyBot->GetActorLocation();

	for (AActor* Actor : BombActors)
	{
		if (AIKABomb* Bomb = Cast<AIKABomb>(Actor))
		{
			float DistSq = (Bomb->GetActorLocation() - MyLoc).SizeSquared();
			if (DistSq < ShortestDistSq)
			{
				ShortestDistSq = DistSq;
				ClosestBomb = Bomb;
			}
		}
	}

	if (ClosestBomb && ShortestDistSq < 900.f)
	{
		SetClosestBomb(ClosestBomb);
	}
}

void AIKAAIController::FindClosestEnemy()
{
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	AIKAPlayerCharacter* BestPawn = NULL;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		AIKAPlayerCharacter* Pawn = Cast<AIKAPlayerCharacter>(*It);
		if (Pawn && Pawn->IsAlive())
		{
			const float DistSq = (Pawn->GetActorLocation() - MyLoc).SizeSquared();
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = Pawn;
			}
		}
	}

	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}

class AIKAPlayerCharacter* AIKAAIController::GetEnemy() const
{
	if (BlackboardComp)
	{
		return Cast<AIKAPlayerCharacter>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return NULL;
}

void AIKAAIController::SetEnemy(class APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		SetFocus(InPawn);
	}
}

void AIKAAIController::SetClosestBomb(class AIKABomb* InBomb)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(ShortestBombKeyID, InBomb);
	}
}

class AIKABomb* AIKAAIController::GetClosestBomb() const
{
	if (BlackboardComp)
	{
		return Cast<AIKABomb>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(ShortestBombKeyID));
	}

	return NULL;
}


