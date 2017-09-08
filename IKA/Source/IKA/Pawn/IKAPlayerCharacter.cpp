// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPlayerCharacter.h"
#include "IKAPlayerController.h"
#include "GameFramework/GameModeBase.h"

AIKAPlayerCharacter::AIKAPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	PlayerIndex = int32(AutoPossessPlayer.GetValue()) - 1;
	if (PlayerIndex == 0)
		return;

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		GameMode->SpawnPlayerController(ROLE_Authority, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

FString AIKAPlayerCharacter::GetHumanReadableName() const
{
	if (PlayerIndex == 0)
	{
		return FString("Player John");
	}
	else
	{
		return FString("Player Tom");
	}
}





