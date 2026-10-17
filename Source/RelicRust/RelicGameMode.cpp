#include "RelicGameMode.h"

#include "Combat/RelicEnemy.h"
#include "Combat/RelicPlayerCharacter.h"
#include "RelicObjective.h"
#include "RelicHUD.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "UObject/ConstructorHelpers.h"

ARelicGameMode::ARelicGameMode()
{
	DefaultPawnClass = ARelicPlayerCharacter::StaticClass();
	HUDClass = ARelicHUD::StaticClass();
}

void ARelicGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnBlockout();
	ARelicObjective* Record = GetWorld()->SpawnActor<ARelicObjective>(FVector(220.f, 0.f, 130.f), FRotator::ZeroRotator);
	if (Record) Record->SetActorScale3D(FVector(1.f, 1.f, 1.f));
	ARelicObjective* Relay = GetWorld()->SpawnActor<ARelicObjective>(FVector(1120.f, 0.f, 130.f), FRotator::ZeroRotator);
	if (Relay) Relay->SetRelay(true);
}

FString ARelicGameMode::GetObjectiveText() const
{
	switch (ObjectiveStage)
	{
	case 0: return TEXT("OBJECTIVE: Inspect the field record  [F]");
	case 1: return FString::Printf(TEXT("OBJECTIVE: Defeat the defenders  (%d remaining)"), EnemiesRemaining);
	case 2: return TEXT("OBJECTIVE: Reach the relic relay  [F]");
	case 3: return TEXT("DECIDE: 1 Meridian  |  2 Free Marches  |  3 Continuum");
	default: return EndingText;
	}
}

void ARelicGameMode::InteractWithTerminal(bool bRelay)
{
	if (!bRelay && ObjectiveStage == 0)
	{
		ObjectiveStage = 1;
		MissionMessage = TEXT("FIELD RECORD: Meridian sent the defenders to erase the route to the vault.");
		if (EnemiesRemaining == 0) ObjectiveStage = 2;
		return;
	}
	if (bRelay && ObjectiveStage == 2)
	{
		ObjectiveStage = 3;
		MissionMessage = TEXT("The relay can expose the vault route, hand it over, or erase the record.");
	}
}

void ARelicGameMode::NotifyEnemyDefeated()
{
	EnemiesRemaining = FMath::Max(0, EnemiesRemaining - 1);
	if (ObjectiveStage == 1 && EnemiesRemaining == 0) ObjectiveStage = 2;
}

void ARelicGameMode::SelectEnding(int32 ChoiceIndex)
{
	if (ObjectiveStage != 3 || ChoiceIndex < 1 || ChoiceIndex > 3) return;
	ObjectiveStage = 4;
	SelectedEnding = ChoiceIndex;
	switch (ChoiceIndex)
	{
	case 1: EndingText = TEXT("ENDING: Meridian receives the route. The relay district goes under corporate quarantine."); break;
	case 2: EndingText = TEXT("ENDING: The Marches receive a copy. Local crews claim the district, with no single authority."); break;
	case 3: EndingText = TEXT("ENDING: You erase the route. The vault falls silent, and its secrets stay buried."); break;
	}
	MissionMessage = EndingText;
}

void ARelicGameMode::SpawnBlock(const FVector& Location, const FVector& Scale, const TCHAR* MeshPath)
{
	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, MeshPath);
	if (!Mesh) return;
	AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
	if (!Block) return;
	Block->SetMobility(EComponentMobility::Static);
	Block->GetStaticMeshComponent()->SetStaticMesh(Mesh);
	Block->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("BlockAll"));
	Block->SetActorScale3D(Scale);
}

void ARelicGameMode::SpawnBlockout()
{
	SpawnBlock(FVector(0.f, 0.f, -50.f), FVector(24.f, 20.f, 1.f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(600.f, 980.f, 130.f), FVector(18.f, 0.8f, 2.6f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(600.f, -980.f, 130.f), FVector(18.f, 0.8f, 2.6f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(1230.f, 0.f, 130.f), FVector(0.8f, 20.f, 2.6f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(280.f, 0.f, 80.f), FVector(1.3f, 3.3f, 1.6f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(830.f, -450.f, 65.f), FVector(2.2f, 0.7f, 1.3f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(820.f, 420.f, 65.f), FVector(2.2f, 0.7f, 1.3f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(1050.f, 650.f, 160.f), FVector(0.9f, 0.9f, 3.2f), TEXT("/Engine/BasicShapes/Cube.Cube"));
	SpawnBlock(FVector(1060.f, -660.f, 100.f), FVector(0.9f, 0.9f, 2.f), TEXT("/Engine/BasicShapes/Cube.Cube"));

	ADirectionalLight* Sun = GetWorld()->SpawnActor<ADirectionalLight>(FVector(0.f), FRotator(-38.f, -32.f, 0.f));
	if (Sun) Sun->GetLightComponent()->SetIntensity(8.f);

	ARelicEnemy* Melee = GetWorld()->SpawnActor<ARelicEnemy>(FVector(540.f, -300.f, 105.f), FRotator::ZeroRotator);
	if (Melee) Melee->SetDisplayName(TEXT("March Raider"));
	ARelicEnemy* Shooter = GetWorld()->SpawnActor<ARelicEnemy>(FVector(970.f, 320.f, 105.f), FRotator::ZeroRotator);
	if (Shooter)
	{
		Shooter->SetRanged(true);
		Shooter->SetDisplayName(TEXT("Combine Rifleman"));
	}
}
