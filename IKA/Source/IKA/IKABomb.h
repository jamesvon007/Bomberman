// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IKABomb.generated.h"

UCLASS()
class IKA_API AIKABomb : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	/** Time to explode after laying */
	UPROPERTY(EditDefaultsOnly, Category = Bomb)
	float BlastTime;

	/** Blast duration */
	UPROPERTY(EditDefaultsOnly, Category = Bomb)
	float BlastDuration;

	/** Blast range */
	UPROPERTY(EditDefaultsOnly, Category = Bomb)
	float BlastRange;
	
public:	
	// Sets default values for this actor's properties
	AIKABomb();

	// Trigger bomb blast
	void Trigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Blast start */
	virtual void OnBlastStart();

	/** Blast end */
	virtual void OnBlastEnd();

	/** FX of blast */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* BlastFX;

	/** param name for beam target in blast trail */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName TrailTargetParam;

	/** param name for beam lifetime in blast trail */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName TrailLifetimeParam;

	/** set direction of this beam blast trail */
	UPROPERTY(EditAnywhere, Category = Effects)
	TArray<FVector> BlastDirection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool Exploding;

	bool BlastFxSpawned;

	float TickTimer;

	void SpawnBlastEffect(const TArray<FVector>& EndPoints);
	
	bool BlastTrace(TArray<struct FHitResult>& OutHits, const FVector& StartTrace, const FVector& EndTrace) const;
};
