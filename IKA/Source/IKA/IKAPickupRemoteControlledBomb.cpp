// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickupRemoteControlledBomb.h"
#include "IKACharacter.h"

AIKAPickupRemoteControlledBomb::AIKAPickupRemoteControlledBomb(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAPickupRemoteControlledBomb::GivePickupTo()
{
	Super::GivePickupTo();

	if (PickedUpBy)
	{
		PickedUpBy->EnableUsingRemoteControlledBomb(AbilityActiveDuration);
	}
}
