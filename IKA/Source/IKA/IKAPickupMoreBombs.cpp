// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickupMoreBombs.h"
#include "IKACharacter.h"


AIKAPickupMoreBombs::AIKAPickupMoreBombs(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAPickupMoreBombs::GivePickupTo()
{
	Super::GivePickupTo();

	if (PickedUpBy)
	{
		PickedUpBy->AddBombAmount(BombAmountBonusAddition);
	}
}