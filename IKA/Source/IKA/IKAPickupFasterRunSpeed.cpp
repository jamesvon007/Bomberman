// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickupFasterRunSpeed.h"
#include "IKACharacter.h"

AIKAPickupFasterRunSpeed::AIKAPickupFasterRunSpeed(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAPickupFasterRunSpeed::GivePickupTo()
{
	Super::GivePickupTo();

	if (PickedUpBy)
	{
		PickedUpBy->AddMoveSpeedMultiplier(MoveSpeedBonusAddition);
	}
}

