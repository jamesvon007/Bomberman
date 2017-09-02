// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "IKAPickup.generated.h"

UCLASS()
class IKA_API AIKAPickup : public AActor
{
	GENERATED_UCLASS_BODY()

	/** pickup on touch */
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;
	
public:	
	// Sets default values for this actor's properties
	AIKAPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** show and enable pickup */
	virtual void SpawnPickup();

	/** show effects when pickup appears */
	virtual void OnSpawned();

	/** show effects when pickup disappears */
	virtual void OnPickedUp();

	/** check if pawn can use this pickup */
	virtual bool CanBePickedUp(class AIKAPlayerCharacter* Pawn) const;

	/** give pickup */
	virtual void GivePickupTo(class AIKAPlayerCharacter* Pawn);

	/** FX of active pickup */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UParticleSystem* ActiveFX;

	/** sound played on respawn */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* SpawnSound;

	/** sound played when player picks it up */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* PickupSound;

	/** is it ready for interactions? */
	UPROPERTY(Transient)
	uint32 bIsActive : 1;

	/* The character who has picked up this pickup */
	AIKAPlayerCharacter* PickedUpBy;

protected:
	/** handle touches */
	void PickupOnTouch(class AIKAPlayerCharacter* Pawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** FX component */
	UPROPERTY(VisibleDefaultsOnly, Category = Effects)
	UParticleSystemComponent* PickupPSC;
	
};
