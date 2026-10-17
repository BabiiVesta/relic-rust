#include "RelicObjective.h"

#include "RelicGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ARelicObjective::ARelicObjective()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MeshAsset.Succeeded()) Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.15f));
}

void ARelicObjective::BeginPlay() { Super::BeginPlay(); }

void ARelicObjective::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const ARelicGameMode* GM = GetWorld() ? Cast<ARelicGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
	const bool bActive = GM && ((bRelay && GM->GetObjectiveStage() == 2) || (!bRelay && GM->GetObjectiveStage() == 0));
	const FColor Color = bActive ? FColor(80, 255, 225) : FColor(100, 110, 115);
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 145.f),
		bRelay ? TEXT("RELIC RELAY") : TEXT("FIELD RECORD"), nullptr, Color, 0.f, true);
}

void ARelicObjective::Interact()
{
	if (ARelicGameMode* GM = GetWorld() ? Cast<ARelicGameMode>(GetWorld()->GetAuthGameMode()) : nullptr)
	{
		GM->InteractWithTerminal(bRelay);
	}
}
