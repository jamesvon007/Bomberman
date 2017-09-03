// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKAPickup.h"
#include "IKAPickupFasterRunSpeed.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKAPickupFasterRunSpeed : public AIKAPickup
{
	GENERATED_UCLASS_BODY()
	
	/** Move speed */
	UPROPERTY(EditDefaultsOnly, Category = PickupBonus)
	float MoveSpeedBonusAddition;
	
protected:
	/** upgrade move speed*/
	virtual void GivePickupTo() override;
};
