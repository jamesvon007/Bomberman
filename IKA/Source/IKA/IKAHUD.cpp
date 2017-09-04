// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAHUD.h"
#include "IKACharacter.h"
#include "Components/TextRenderComponent.h"

AIKAHUD::AIKAHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AIKAHUD::DrawHUD()
{
	Super::DrawHUD();
	if (Canvas == nullptr)
	{
		return;
	}

	DrawPlayerTimedPowerupRemainTime();
}

void AIKAHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AIKAHUD::DrawPlayerTimedPowerupRemainTime()
{
	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumIntegralDigits = 1;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = 0;
	NumberFormat.MaximumFractionalDigits = 0;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC)
		{
			APawn* Pawn = PC->GetPawn();
			if (AIKACharacter* Character = Cast<AIKACharacter>(Pawn))
			{
				if (Character)
				{
					if (Character->IsUsingRemoteControlledBomb())
					{
						const float RemainTime = Character->GetTimedPowerupRemainTime().RemainTime;
						Character->GetTimedPowerupRemainTime().TextRenderComponent->SetVisibility(true);
						
						FTransform Transform = Character->GetActorTransform();
						Transform.SetTranslation(
							Transform.GetTranslation() + 100.f * FVector(0.f, 0.f, 1.f));
						Transform.SetRotation(FQuat(FRotator(0.f, 180.f, 0.f)));
						Character->GetTimedPowerupRemainTime().TextRenderComponent
							->SetWorldTransform(Transform);
						Character->GetTimedPowerupRemainTime().TextRenderComponent
							->SetText(FText::AsNumber(RemainTime, &NumberFormat));
					}
					else
					{
						Character->GetTimedPowerupRemainTime().TextRenderComponent->SetVisibility(false);
					}
				}
			}
		}
	}
}


