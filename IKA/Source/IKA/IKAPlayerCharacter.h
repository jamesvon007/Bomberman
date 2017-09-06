// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKACharacter.h"
#include "IKAPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class IKA_API AIKAPlayerCharacter : public AIKACharacter
{
	GENERATED_UCLASS_BODY()

	
protected:
	virtual void PostInitializeComponents() override;

public:
	uint8 GetPlayerIndex() { return PlayerIndex;	}

	virtual FString GetHumanReadableName() const override;

private:
	uint8 PlayerIndex;
};
