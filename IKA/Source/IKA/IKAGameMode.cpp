// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IKAGameMode.h"
#include "IKAPlayerController.h"
#include "IKACharacter.h"
#include "IKAHUD.h"
#include "UObject/ConstructorHelpers.h"

AIKAGameMode::AIKAGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AIKAPlayerController::StaticClass();
	HUDClass = AIKAHUD::StaticClass();

	// set default pawn class to our Blueprinted character
// 	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
// 	if (PlayerPawnBPClass.Class != NULL)
// 	{
// 		DefaultPawnClass = PlayerPawnBPClass.Class;
// 	}
}