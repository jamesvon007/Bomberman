// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IKAPlayerController.generated.h"

UCLASS()
class AIKAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AIKAPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	// End PlayerController interface

	/** Handles moving forward/backward */
	void MoveForwardOne(float Val);

	/** Handles stafing movement, left and right */
	void MoveRightOne(float Val);

	/** Handles place bomb */
	void PlaceBombOne();

	/** Remote Controll bomb blast */
	void RemoteTriggerOne();

	/** Handles moving forward/backward */
	void MoveForwardTwo(float Val);

	/** Handles stafing movement, left and right */
	void MoveRightTwo(float Val);

	/** Handles place bomb */
	void PlaceBombTwo();

	/** Remote Controll bomb blast */
	void RemoteTriggerTwo();

	/** Restart game after ending*/
	void RestartLevel();
};


