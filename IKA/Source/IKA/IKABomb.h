// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Bomb)
	float BlastRange;
	
public:	
	// Sets default values for this actor's properties
	AIKABomb();

	// Trigger bomb blast
	void Trigger();

	bool IsRemoteTrigger() { return RemoteTrigger; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Blast start */
	virtual void OnBlastStart();

	/** blueprint event: blast end */
	UFUNCTION(BlueprintImplementableEvent)
	void OnBlastEnd();

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

	/** sound played on respawn */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* BlastSound;

	UPROPERTY(EditDefaultsOnly, Category = Bomb)
	uint8 RemoteTrigger : 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	uint8 Exploding : 1;

	uint8 BlastFxSpawned : 1;

	float TickTimer;

	void SpawnBlastEffect(const TArray<FVector>& EndPoints);
	
	bool BlastTrace(TArray<struct FHitResult>& OutHits, const FVector& StartTrace, const FVector& EndTrace) const;

	/** The BoxComponent being used for collision. */
	UPROPERTY(Category = Bomb, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
};
