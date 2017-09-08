// Fill out your copyright notice in the Description page of Project Settings.

#include "IKASaveGame.h"
#include "Kismet/GameplayStatics.h"

UIKASaveGame::UIKASaveGame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UIKASaveGame::SavePersistentGame()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, UserIndex);
}

UIKASaveGame* UIKASaveGame::LoadPersistentGame(FString SlotName, const int32 UserIndex)
{
	UIKASaveGame* Result = nullptr;

	if(SlotName.Len() > 0)
	{
		Result = Cast<UIKASaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
		if (Result == NULL)
		{
			// if failed to load, create a new one
			Result = Cast<UIKASaveGame>(UGameplayStatics::CreateSaveGameObject(UIKASaveGame::StaticClass()));
		}
		check(Result != NULL);

		Result->SlotName = SlotName;
		Result->UserIndex = UserIndex;
	}

	return Result;
}
