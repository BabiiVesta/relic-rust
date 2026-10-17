#include "RelicEnemy.h"

#include "RelicPlayerCharacter.h"
#include "../RelicGameMode.h"
#include "RelicDamageTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ARelicEnemy::ARelicEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	SetRootComponent(CollisionCapsule);
	CollisionCapsule->InitCapsuleSize(42.f, 95.f);
	CollisionCapsule->SetCollisionProfileName(TEXT("Pawn"));
	CollisionCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CollisionCapsule);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BodyMesh->SetRelativeScale3D(FVector(0.65f, 0.65f, 1.6f));
	if (CubeMesh.Succeeded()) BodyMesh->SetStaticMesh(CubeMesh.Object);
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(CollisionCapsule);
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 73.f));
	HeadMesh->SetRelativeScale3D(FVector(0.38f));
	if (SphereMesh.Succeeded()) HeadMesh->SetStaticMesh(SphereMesh.Object);
}

void ARelicEnemy::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void ARelicEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AttackTimer = FMath::Max(0.f, AttackTimer - DeltaSeconds);
	if (StaggerRemaining > 0.f)
	{
		StaggerRemaining = FMath::Max(0.f, StaggerRemaining - DeltaSeconds);
		bAttackWindupActive = false;
		DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 145.f), TEXT("DISRUPTED"), nullptr, FColor::Cyan, 0.f, true);
		return;
	}
	ARelicPlayerCharacter* Player = Cast<ARelicPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player || Player->GetHealth() <= 0.f) return;
	FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.f;
	const float Distance = ToPlayer.Size();
	if (Distance < 1.f) return;
	const FVector Direction = ToPlayer / Distance;
	SetActorRotation(FRotator(0.f, Direction.Rotation().Yaw, 0.f));
	const float PreferredRange = bRanged ? 700.f : AttackRange;
	if (bRanged && Distance < PreferredRange - 100.f)
	{
		SetActorLocation(GetActorLocation() - Direction * MoveSpeed * DeltaSeconds, true);
	}
	else if (Distance > PreferredRange + (bRanged ? 70.f : 10.f))
	{
		SetActorLocation(GetActorLocation() + Direction * MoveSpeed * DeltaSeconds, true);
	}
	const float AttackDistance = bRanged ? 1150.f : AttackRange;
	if (Distance <= AttackDistance && AttackTimer <= 0.f)
	{
		if (!bAttackWindupActive)
		{
			bAttackWindupActive = true;
			WindupRemaining = bRanged ? 0.45f : 0.7f;
		}
		const TCHAR* Warning = bRanged ? TEXT("AIMING") : TEXT("WINDUP!");
		DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 165.f), Warning, nullptr, FColor(255, 180, 40), 0.f, true);
		WindupRemaining -= DeltaSeconds;
		if (WindupRemaining <= 0.f)
		{
			bAttackWindupActive = false;
			AttackPlayer(Player);
		}
	}
	else if (Distance > AttackDistance)
	{
		bAttackWindupActive = false;
	}
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 115.f),
		FString::Printf(TEXT("%s  %0.0f HP"), *DisplayName, Health), nullptr, FColor::White, 0.f, true);
}

void ARelicEnemy::AttackPlayer(ARelicPlayerCharacter* Player)
{
	AttackTimer = AttackCooldown;
	if (bRanged)
	{
		FHitResult Hit;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(EnemyShot), false, this);
		const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 55.f);
		const FVector End = Player->GetActorLocation() + FVector(0.f, 0.f, 45.f);
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.GetActor() == Player)
		{
			UGameplayStatics::ApplyDamage(Player, 9.f, GetInstigatorController(), this, nullptr);
		}
		DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 0.18f, 0, 2.f);
	}
	else
	{
		UGameplayStatics::ApplyDamage(Player, 13.f, GetInstigatorController(), this, nullptr);
		DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * 90.f, 45.f, 12, FColor::Red, false, 0.2f);
	}
}

float ARelicEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = FMath::Min(Health, DamageAmount);
	Health = FMath::Max(0.f, Health - Applied);
	if (DamageEvent.DamageTypeClass == URelicOverloadDamageType::StaticClass())
	{
		StaggerRemaining = 1.4f;
		AttackTimer = FMath::Max(AttackTimer, StaggerRemaining);
		bAttackWindupActive = false;
	}
	if (Health <= 0.f)
	{
		if (ARelicGameMode* GM = GetWorld() ? Cast<ARelicGameMode>(GetWorld()->GetAuthGameMode()) : nullptr) GM->NotifyEnemyDefeated();
		Destroy();
	}
	return Applied;
}
