// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAHUD.h"
#include "IKACharacter.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"

#define LOCTEXT_NAMESPACE "IKAGame.HUD.Menu"

AIKAHUD::AIKAHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	NormalFont = NormalFontOb.Object;
}

void AIKAHUD::DrawHUD()
{
	Super::DrawHUD();
	if (Canvas == nullptr)
	{
		return;
	}

	ScaleUI = Canvas->ClipY / 1080.0f;

	DrawPlayerTimedPowerupRemainTime();

	DrawBombAmount();
}

void AIKAHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AIKAHUD::DrawBombAmount()
{
	float TextScale = 0.6f;
	float Offset = 30.0f;
	FColor HUDDark = FColor(255, 255, 255, 255);

	uint8 Index = 0;
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
					FString BombAmountText = LOCTEXT("Normal Bomb Amount", "Normal Bomb Amount: ").ToString() 
						+ FString::FromInt(Character->BombAmount);
					FVector2D BombAmountTextSize(0.0f, 0.0f);
					Canvas->StrLen(NormalFont, BombAmountText, BombAmountTextSize.X, BombAmountTextSize.Y);
					const float PosX = (Index == 0 ? Offset : 11 * Canvas->SizeX / 12.f - 4*Offset);
					const float PosY = Canvas->SizeY / 10.f;
					FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), NormalFont, HUDDark);
					TextItem.Text = FText::FromString(BombAmountText);
					TextItem.Position = FVector2D(PosX, PosY);
					TextItem.Scale = FVector2D(TextScale*ScaleUI, TextScale*ScaleUI);
					Canvas->DrawItem(TextItem);

					BombAmountText = LOCTEXT("Remote-Controlled Bomb Amount", "Remote-Controlled Bomb Amount: ").ToString()
						+ FString::FromInt(Character->GetRemoteBombAmount());
					TextItem.Text = FText::FromString(BombAmountText);
					TextItem.Position = FVector2D(PosX, PosY + BombAmountTextSize.Y/2);
					Canvas->DrawItem(TextItem);

					FString Health = LOCTEXT("Health", "Health: ").ToString() + FString::FromInt(Character->Health);
					TextItem.Text = FText::FromString(Health);
					TextItem.Position = FVector2D(PosX, PosY + BombAmountTextSize.Y);
					Canvas->DrawItem(TextItem);
				}
			}
		}

		++Index;
	}
}

void AIKAHUD::RegisterCharacter(AIKACharacter* Character)
{
	if (Character != nullptr)
	{
		Characters.Add(Character);
	}
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


