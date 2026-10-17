#include "RelicHUD.h"

#include "Combat/RelicPlayerCharacter.h"
#include "Combat/RelicCombatComponent.h"
#include "RelicGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void ARelicHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas || !PlayerOwner) return;
	const float CenterX = Canvas->ClipX * 0.5f;
	const float CenterY = Canvas->ClipY * 0.5f;
	Canvas->SetDrawColor(FColor(90, 235, 220, 230));
	Canvas->DrawText(GEngine ? GEngine->GetSmallFont() : nullptr, TEXT("+"), CenterX - 4.f, CenterY - 9.f, 1.2f, 1.2f);

	ARelicPlayerCharacter* Player = Cast<ARelicPlayerCharacter>(PlayerOwner->GetPawn());
	if (!Player || !GEngine) return;
	const URelicCombatComponent* Combat = Player->GetCombatComponent();
	const FString Health = FString::Printf(TEXT("VITALS  %03.0f / %03.0f"), Player->GetHealth(), Player->GetMaxHealth());
	const FString Heat = Combat ? FString::Printf(TEXT("NEURAL HEAT  %03.0f / 100   |   %s"), Combat->GetNeuralHeat(), *UEnum::GetValueAsString(Combat->GetCombatState())) : FString();
	const FString Instructions = TEXT("WASD move  |  SPACE jump  |  LMB light  |  R heavy  |  RMB pistol  |  Q parry  |  SHIFT dodge  |  E overload  |  F interact  |  V camera");
	const ARelicGameMode* GM = Cast<ARelicGameMode>(Player->GetWorld()->GetAuthGameMode());
	const FString Objective = GM ? GM->GetObjectiveText() : FString();
	const FString Message = GM ? GM->GetMissionMessage() : FString();
	Canvas->SetDrawColor(FColor::White);
	Canvas->DrawText(GEngine->GetSmallFont(), TEXT("RELIC & RUST  /  COMBAT SANDBOX"), 24.f, 20.f, 1.4f, 1.4f);
	Canvas->DrawText(GEngine->GetSmallFont(), Health, 24.f, 52.f, 1.2f, 1.2f);
	Canvas->DrawText(GEngine->GetSmallFont(), Heat, 24.f, 78.f, 1.1f, 1.1f);
	Canvas->SetDrawColor(FColor(90, 235, 220));
	Canvas->DrawText(GEngine->GetSmallFont(), Objective, 24.f, 112.f, 1.1f, 1.1f);
	Canvas->SetDrawColor(FColor(210, 215, 220));
	Canvas->DrawText(GEngine->GetSmallFont(), Message, 24.f, 140.f, 0.95f, 0.95f);
	Canvas->SetDrawColor(FColor(185, 200, 205));
	Canvas->DrawText(GEngine->GetSmallFont(), Instructions, 24.f, Canvas->ClipY - 42.f, 0.9f, 0.9f);
}
