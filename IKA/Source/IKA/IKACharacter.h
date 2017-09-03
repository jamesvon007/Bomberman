// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IKACharacter.generated.h"

UCLASS(Blueprintable)
class AIKACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AIKACharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	/** check if pawn is still alive */
	bool IsAlive() const;

	/** kill this pawn */
	void Die();

	void AddBlastRangeMultiplier(float Multiplier);

	void AddMoveSpeedMultiplier(float Multiplier);

	void AddBombAmount(uint8 Increment);

	void EnableUsingRemoteControlledBomb(float Duration);

	void RestoreBombAmount();

	void PlaceBomb();

	// Current health of the Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float Health;

private:
	float BlastRangeMultiplier;

	uint8 BombAmount;

	float MoveSpeedMultiplier;

	uint8 UseRemoteControlledBomb : 1;

	float RemoteControlledBombAbilityActiveDuration;

};

