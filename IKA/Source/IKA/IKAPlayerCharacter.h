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


};
