// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "IKASaveGame.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API UIKASaveGame : public USaveGame
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY()
	uint8 PlayerFirstScore;

	UPROPERTY()
	uint8 PlayerSecondScore;
	
	/** Loads user persistence data if it exists, creates an empty record otherwise. */
	static UIKASaveGame* LoadPersistentGame(FString SlotName, const int32 UserIndex);

	void UIKASaveGame::SavePersistentGame();

private:
	/** The string identifier used to save/load this persistent user. */
	FString SlotName;
	int32 UserIndex;
};
