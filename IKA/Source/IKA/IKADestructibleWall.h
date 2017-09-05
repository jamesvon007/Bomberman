// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKAWall.h"
#include "IKADestructibleWall.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKADestructibleWall : public AIKAWall
{
	GENERATED_UCLASS_BODY()
	
	/** FX of being destroyed */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* DestroyedFX;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float SpawningChanceWhenDestroyed;

	/** blueprint event: spawn pickup */
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnPickup();
	
public:
	void Destroy();

private:
	void OnDestroy();
};
