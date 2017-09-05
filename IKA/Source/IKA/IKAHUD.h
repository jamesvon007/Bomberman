// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IKAHUD.generated.h"

/**
 * 
 */
UCLASS()
class IKA_API AIKAHUD : public AHUD
{
	GENERATED_UCLASS_BODY()
	
	/** Normal font. */
	UPROPERTY()
	UFont* NormalFont;

	/** UI scaling factor for other resolutions than Full HD. */
	float ScaleUI;
	
public:
	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	void RegisterCharacter(class AIKACharacter* Character);

	/** Draw player property in game*/
	void DrawPlayerTimedPowerupRemainTime();

	/** Draw player owned bomb amount in HUD*/
	void DrawBombAmount();

private:
	TArray<AIKACharacter*> Characters;
};
