// Fill out your copyright notice in the Description page of Project Settings.

#include "IKADestructibleWall.h"
#include "Kismet/GameplayStatics.h"

AIKADestructibleWall::AIKADestructibleWall(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SpawningChanceWhenDestroyed = 0.3f;
}

void AIKADestructibleWall::Destroy()
{
	OnDestroy();
}

void AIKADestructibleWall::OnDestroy()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DestroyedFX, GetActorLocation());

	if (FMath::FRandRange(0, 1) < SpawningChanceWhenDestroyed)
	{
		OnSpawnPickup();
	}

	SetLifeSpan(0.1f);
}
