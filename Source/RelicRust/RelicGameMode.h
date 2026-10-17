#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RelicGameMode.generated.h"

UCLASS()
class RELICRUST_API ARelicGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARelicGameMode();
	virtual void BeginPlay() override;
	void InteractWithTerminal(bool bRelay);
	void NotifyEnemyDefeated();
	void SelectEnding(int32 ChoiceIndex);
	FString GetObjectiveText() const;
	FString GetMissionMessage() const { return MissionMessage; }
	int32 GetObjectiveStage() const { return ObjectiveStage; }
	UFUNCTION(BlueprintPure, Category="Mission")
	int32 GetSelectedEnding() const { return SelectedEnding; }

private:
	void SpawnBlockout();
	void SpawnBlock(const FVector& Location, const FVector& Scale, const TCHAR* MeshPath);
	int32 ObjectiveStage = 0;
	int32 EnemiesRemaining = 2;
	int32 SelectedEnding = 0;
	FString EndingText;
	FString MissionMessage = TEXT("Extraction failed. The relic bonded to your nervous system.");
};
