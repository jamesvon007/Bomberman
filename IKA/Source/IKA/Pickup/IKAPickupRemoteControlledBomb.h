// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKAPickup.h"
#include "IKAPickupRemoteControlledBomb.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKAPickupRemoteControlledBomb : public AIKAPickup
{
	GENERATED_UCLASS_BODY()
	
	/** Remote controlled bomb ability active duration */
	UPROPERTY(EditDefaultsOnly, Category = PickupBonus)
	float AbilityActiveDuration;

protected:
	/** upgrade move speed*/
	virtual void GivePickupTo() override;
	

};
