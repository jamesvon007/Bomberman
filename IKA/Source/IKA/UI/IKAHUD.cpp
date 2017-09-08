// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAHUD.h"
#include "Pawn/IKACharacter.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"
#include "IKAGameState.h"
#include "IKAGameMode.h"

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

	AIKAGameMode* GameMode = Cast<AIKAGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		if (GameMode->GetMatchState() == MatchState::InProgress)
		{
			DrawPlayerTimedPowerupRemainTime();

			DrawPowerupDisplayComp();

			DrawPlayerNameBombAmount();

			DrawCountdownTimer();
		}
		else if (GameMode->GetMatchState() == MatchState::WaitingPostMatch)
		{
			DrawMatchInfo(GameMode);
		}
	}

}

void AIKAHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AIKAHUD::DrawMatchInfo(AIKAGameMode* GameMode)
{
	float TextScale = 2.f;
	FString MatchResultText = LOCTEXT("Match Result", "Match Result: DRAW").ToString();
	switch (GameMode->GetMatchResult())
	{
	case AIKAGameMode::EMatchResult::DRAW:
		break;
	case AIKAGameMode::EMatchResult::WIN:
		MatchResultText = LOCTEXT("Match Result", "Match Result: ").ToString() + GameMode->GetLastWinCharacter()->GetHumanReadableName() + FString(" WIN");
		break;
	case AIKAGameMode::EMatchResult::LOSE:
		MatchResultText = LOCTEXT("Match Result", "Match Result: LOSE").ToString();
		break;
	default:
		;
	}

	FVector2D TextSize(0.0f, 0.0f);
	Canvas->StrLen(NormalFont, MatchResultText, TextSize.X, TextSize.Y);
	float PosX = Canvas->SizeX / 2.f - TextSize.X*ScaleUI;
	float PosY = Canvas->SizeY / 2.f;
	FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), NormalFont, FColor::White);
	TextItem.Text = FText::FromString(MatchResultText);
	TextItem.Position = FVector2D(PosX, PosY);
	TextItem.Scale = FVector2D(TextScale*ScaleUI, TextScale*ScaleUI);
	Canvas->DrawItem(TextItem);

	FString ScoreText = LOCTEXT("Match Score", "Score ").ToString() + FString("PlayerOne: ") + FString::FromInt(GameMode->GetPlayerFirstScore())
		+ FString(", PlayerTwo: ") + FString::FromInt(GameMode->GetPlayerSecondScore());
	Canvas->StrLen(NormalFont, ScoreText, TextSize.X, TextSize.Y);
	PosX = Canvas->SizeX / 2.f - TextSize.X*ScaleUI;
	PosY = Canvas->SizeY / 2.f + TextSize.Y;
	TextScale = 1.f;
	TextItem.Text = FText::FromString(ScoreText);
	TextItem.Position = FVector2D(PosX, PosY);
	TextItem.Scale = FVector2D(TextScale*ScaleUI, TextScale*ScaleUI);
	Canvas->DrawItem(TextItem);

	FString ResetText = LOCTEXT("Match Result", "Please press R to restart Game").ToString();
	Canvas->StrLen(NormalFont, ResetText, TextSize.X, TextSize.Y);
	PosX = Canvas->SizeX / 2.f - TextSize.X*ScaleUI;
	PosY = Canvas->SizeY / 2.f + 2*TextSize.Y;
	TextScale = 1.f;
	TextItem.Text = FText::FromString(ResetText);
	TextItem.Position = FVector2D(PosX, PosY);
	TextItem.Scale = FVector2D(TextScale*ScaleUI, TextScale*ScaleUI);
	Canvas->DrawItem(TextItem);
}

void AIKAHUD::DrawCountdownTimer()
{
	AIKAGameState* const GameState = GetWorld()->GetGameState<AIKAGameState>();
	if (GameState && GameState->RemainingTime > 0)
	{
		float Offset = 10.0f;
		float TextScale = 0.8f;
		FString CountdownText = FString::Printf(TEXT("%02d : %02d"), GameState->RemainingTime / 60, GameState->RemainingTime % 60);
		FVector2D CountdownTextSize(0.0f, 0.0f);
		Canvas->StrLen(NormalFont, CountdownText, CountdownTextSize.X, CountdownTextSize.Y);
		const float PosX = Canvas->SizeX / 2.f - CountdownTextSize.X*ScaleUI;
		const float PosY = Offset;
		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), NormalFont, FColor::Green);
		TextItem.Text = FText::FromString(CountdownText);
		TextItem.Position = FVector2D(PosX, PosY);
		TextItem.Scale = FVector2D(TextScale*ScaleUI, TextScale*ScaleUI);
		Canvas->DrawItem(TextItem);
	}
	
}

void AIKAHUD::DrawPlayerNameBombAmount()
{
	float TextScale = 0.6f;
	float Offset = 30.0f;
	FColor HUDLight = FColor(255, 255, 255, 255);

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
					FString PlayerNameText = LOCTEXT("Player Name", "Player Name: ").ToString() +Character->GetHumanReadableName();
					float PosX = (Index == 0 ? Offset : 11 * Canvas->SizeX / 12.f - 4 * Offset);
					float PosY = Canvas->SizeY / 10.f;
					FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), NormalFont, FColor::White);
					TextItem.Text = FText::FromString(PlayerNameText);
					TextItem.Position = FVector2D(PosX, PosY);
					TextItem.Scale = FVector2D(TextScale*ScaleUI, TextScale*ScaleUI);
					Canvas->DrawItem(TextItem);

					FString BombAmountText = LOCTEXT("Normal Bomb Amount", "Normal Bomb Amount: ").ToString() 
						+ FString::FromInt(Character->BombAmount);
					FVector2D BombAmountTextSize(0.0f, 0.0f);
					Canvas->StrLen(NormalFont, BombAmountText, BombAmountTextSize.X, BombAmountTextSize.Y);
					TextItem.Text = FText::FromString(BombAmountText);
					TextItem.Position = FVector2D(PosX, PosY += BombAmountTextSize.Y / 2);
					Canvas->DrawItem(TextItem);

					BombAmountText = LOCTEXT("Remote-Controlled Bomb Amount", "Remote-Controlled Bomb Amount: ").ToString()
						+ FString::FromInt(Character->GetRemoteBombAmount());
					TextItem.Text = FText::FromString(BombAmountText);
					TextItem.Position = FVector2D(PosX, PosY += BombAmountTextSize.Y/2);
					Canvas->DrawItem(TextItem);

					FString Health = LOCTEXT("Health", "Health: ").ToString() + FString::FromInt(Character->Health);
					TextItem.Text = FText::FromString(Health);
					TextItem.Position = FVector2D(PosX, PosY += BombAmountTextSize.Y/2);
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

void AIKAHUD::DrawPowerupDisplayComp()
{
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
					AIKACharacter::UPowerupDisplay& PowerupDisplay = Character->GetPowerupDisplay();
					if (PowerupDisplay.DisplayTime > 0.f)
					{
						PowerupDisplay.TextRenderComponent->SetVisibility(true);

						FTransform Transform = Character->GetActorTransform();
						Transform.SetTranslation(
							Transform.GetTranslation() + 150.f * FVector(0.f, 0.f, 1.f));
						Transform.SetRotation(FQuat(FRotator(0.f, 180.f, 0.f)));
						PowerupDisplay.TextRenderComponent
							->SetWorldTransform(Transform);
						PowerupDisplay.TextRenderComponent
							->SetText(FText::FromString(PowerupDisplay.Message));
					}
					else
					{
						PowerupDisplay.TextRenderComponent->SetVisibility(false);
					}
				}
			}
		}
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


