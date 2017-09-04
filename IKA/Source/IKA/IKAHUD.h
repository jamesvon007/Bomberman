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
	
	
public:
	/** Main HUD update loop. */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	/** Draw player property in game*/
	void DrawPlayerTimedPowerupRemainTime();

private:
};
