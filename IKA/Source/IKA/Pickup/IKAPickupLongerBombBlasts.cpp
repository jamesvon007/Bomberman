// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickupLongerBombBlasts.h"
#include "Pawn/IKACharacter.h"


AIKAPickupLongerBombBlasts::AIKAPickupLongerBombBlasts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAPickupLongerBombBlasts::GivePickupTo()
{
	Super::GivePickupTo();

	if(PickedUpBy)
	{
		PickedUpBy->AddBlastRangeMultiplier(BlastRangeBonusAddition);
		PickedUpBy->GetPowerupDisplay().Message = FString("Gained Longer Bomb Blasts");
		PickedUpBy->GetPowerupDisplay().DisplayTime = 10.f;
	}
}

