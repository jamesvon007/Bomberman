// Fill out your copyright notice in the Description page of Project Settings.

#include "IKABotCharacter.h"
#include "IKAAIController.h"

AIKABotCharacter::AIKABotCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIControllerClass = AIKAAIController::StaticClass();
}


