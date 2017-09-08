// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickupMoreBombs.h"
#include "Pawn/IKACharacter.h"


AIKAPickupMoreBombs::AIKAPickupMoreBombs(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAPickupMoreBombs::GivePickupTo()
{
	Super::GivePickupTo();

	if (PickedUpBy)
	{
		PickedUpBy->AddBombAmount(BombAmountBonusAddition);
		PickedUpBy->GetPowerupDisplay().Message = FString("Gained More Bombs");
		PickedUpBy->GetPowerupDisplay().DisplayTime = 10.f;
	}
}