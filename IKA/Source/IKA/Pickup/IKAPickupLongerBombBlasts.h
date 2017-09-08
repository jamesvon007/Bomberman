// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKAPickup.h"
#include "IKAPickupLongerBombBlasts.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKAPickupLongerBombBlasts : public AIKAPickup
{
	GENERATED_UCLASS_BODY()
	
	/** Blast range */
	UPROPERTY(EditDefaultsOnly, Category = PickupBonus)
	float BlastRangeBonusAddition;
	
protected:
	/** upgrade longer bomb */
	virtual void GivePickupTo() override;
};
