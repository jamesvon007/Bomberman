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
	struct UTimedPowerupRemainTime
	{
		class UTextRenderComponent* TextRenderComponent;
		float RemainTime;
	};

public:
	AIKACharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void BeginPlay() override;

private:

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void InitPlayerUI();

public:
	/** check if pawn is still alive */
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	/** kill this pawn */
	void Die();

	void AddBlastRangeMultiplier(float Multiplier);

	void AddMoveSpeedMultiplier(float Multiplier);

	void AddBombAmount(uint8 Increment);

	void EnableUsingRemoteControlledBomb(float Duration);

	void DisableUsingRemoteControlledBomb();

	void TriggerRemoteBomb();

	UFUNCTION(BlueprintCallable)
	void RestoreBombAmount(class AIKABomb* Bomb);

	UFUNCTION(BlueprintCallable)
	void ConsumeBombAmount(class AIKABomb* Bomb);

	UFUNCTION(BlueprintCallable)
	bool CheckBombAmount();

	/** blueprint event: place bomb */
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlaceBomb();

	UTimedPowerupRemainTime GetTimedPowerupRemainTime();

	bool IsUsingRemoteControlledBomb() { return UseRemoteControlledBomb;}

	uint8 GetRemoteBombAmount() { return RemoteBombAmount;	}

	// Current health of the Pawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float Health;

	// Bomb amount
	UPROPERTY(BlueprintReadWrite)
	uint8 BombAmount;

	/** set remote-controlled bomb placed */
	UPROPERTY(BlueprintReadWrite)
	class AIKABomb* RemoteBomb;

	UPROPERTY(EditAnywhere, Category = Font)
	UFont* TextFont;

private:
	UPROPERTY(Category = Ability, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BlastRangeMultiplier;

	float MoveSpeedMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 UseRemoteControlledBomb : 1;

	float RemoteControlledBombAbilityActiveDuration;

	// how many remote-controlled bombs this pawn can place
	uint8 RemoteBombAmount;

	UTimedPowerupRemainTime TimedPowerupRemainTime;
};

