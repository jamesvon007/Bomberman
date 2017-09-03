// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKAPickup.h"
#include "IKAPickupMoreBombs.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKAPickupMoreBombs : public AIKAPickup
{
	GENERATED_UCLASS_BODY()

	/** Bomb Slot */
	UPROPERTY(EditDefaultsOnly, Category = PickupBonus)
	uint8 BombAmountBonusAddition;
	
protected:
	/** upgrade number of bomb slot*/
	virtual void GivePickupTo() override;
	
	
};
