// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "IKAAIController.generated.h"


/**
 * 
 */
UCLASS()
class IKA_API AIKAAIController : public AAIController
{
	GENERATED_UCLASS_BODY()
	
	
private:
	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComp;

	/* Cached BT component */
	UPROPERTY(transient)
	UBehaviorTreeComponent* BehaviorComp;

public:
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;

	void SetEnemy(class APawn* InPawn);
	class AIKAPlayerCharacter* GetEnemy() const;

	void SetClosestBomb(class AIKABomb* InBomb);
	class AIKABomb* GetClosestBomb() const;

	/* Finds the closest enemy and sets them as current target */
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindClosestEnemy();

	/* Try to place bomb to kill players */
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void TryPlacingBomb();

	/* Finds the closest enemy and sets them as current target */
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void DetectClosestBomb();

	UBehaviorTreeComponent* GetBehaviorComp() { return BehaviorComp;	};

protected:
	
	int32 EnemyKeyID;
	int32 ShortestBombKeyID;
};
