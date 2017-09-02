// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPlayerCharacter.h"
#include "IKAPlayerController.h"
#include "GameFramework/GameModeBase.h"

AIKAPlayerCharacter::AIKAPlayerCharacter(const FObjectInitializer& ObjectInitializer)
{
	Health = 100.f;
}

void AIKAPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	const int32 PlayerIndex = int32(AutoPossessPlayer.GetValue()) - 1;
	if (PlayerIndex == 0)
		return;

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		GameMode->SpawnPlayerController(ROLE_Authority, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

bool AIKAPlayerCharacter::IsAlive() const
{
	return Health > 0;
}



