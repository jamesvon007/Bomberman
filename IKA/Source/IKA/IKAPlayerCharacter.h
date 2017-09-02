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
	
	// Current health of the Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float Health;
	
protected:

	virtual void PostInitializeComponents() override;

public:
	/** check if pawn is still alive */
	bool IsAlive() const;

};
