#include "RelicCombatComponent.h"
#include "RelicDamageTypes.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"

URelicCombatComponent::URelicCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URelicCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(HeatTimer, this, &URelicCombatComponent::CoolHeat, 0.25f, true);
}

ACharacter* URelicCombatComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

void URelicCombatComponent::LightAttack()
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character || CombatState != ERelicCombatState::Ready) return;
	const float Now = GetWorld()->GetTimeSeconds();
	LightComboIndex = (Now - LastLightAttackTime <= 1.05f) ? (LightComboIndex + 1) % 3 : 0;
	LastLightAttackTime = Now;
	const float ComboDamageScale = LightComboIndex == 2 ? 1.35f : (LightComboIndex == 1 ? 1.1f : 1.f);
	SetTemporaryState(ERelicCombatState::Attacking, ActionCommitmentSeconds * (LightComboIndex == 2 ? 1.2f : 1.f));
	OnCombatAction(TEXT("LightAttack"));
	DrawDebugString(GetWorld(), Character->GetActorLocation() + FVector(0.f, 0.f, 135.f),
		FString::Printf(TEXT("ARC COMBO %d/3"), LightComboIndex + 1), nullptr, FColor(80, 235, 220), 0.7f, true);

	const FVector Start = Character->GetActorLocation() + FVector(0.f, 0.f, 55.f);
	const FVector End = Start + Character->GetActorForwardVector() * MeleeRange;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RelicMelee), false, Character);
	FHitResult Hit;
	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(MeleeRadius), Params) && Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), MeleeDamage * ComboDamageScale, Character->GetActorForwardVector(), Hit,
			Character->GetController(), Character, nullptr);
	}
}

void URelicCombatComponent::HeavyAttack()
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character || CombatState != ERelicCombatState::Ready) return;
	SetTemporaryState(ERelicCombatState::Attacking, ActionCommitmentSeconds * 2.2f);
	OnCombatAction(TEXT("HeavyAttack"));
	const FVector Start = Character->GetActorLocation() + FVector(0.f, 0.f, 55.f);
	const FVector End = Start + Character->GetActorForwardVector() * (MeleeRange * 1.3f);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RelicHeavyMelee), false, Character);
	FHitResult Hit;
	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(MeleeRadius * 1.2f), Params) && Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), MeleeDamage * 1.8f, Character->GetActorForwardVector(), Hit,
			Character->GetController(), Character, nullptr);
	}
}

void URelicCombatComponent::Dodge()
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character || CombatState != ERelicCombatState::Ready) return;
	SetTemporaryState(ERelicCombatState::Dodging, 0.35f);
	OnCombatAction(TEXT("Dodge"));
	FVector Direction = Character->GetLastMovementInputVector().GetSafeNormal();
	if (Direction.IsNearlyZero()) Direction = -Character->GetActorForwardVector();
	Character->LaunchCharacter(Direction * DodgeImpulse + FVector(0.f, 0.f, 80.f), true, true);
}

void URelicCombatComponent::BeginParry()
{
	if (CombatState != ERelicCombatState::Ready) return;
	SetTemporaryState(ERelicCombatState::Parrying, 0.55f);
	OnCombatAction(TEXT("Parry"));
	OnParryWindowOpened();
}

void URelicCombatComponent::EndParry()
{
	if (CombatState == ERelicCombatState::Parrying) ReturnToReady();
}

void URelicCombatComponent::NotifyParrySucceeded(AActor* Attacker)
{
	if (CombatState != ERelicCombatState::Parrying) return;
	if (IsValid(Attacker))
	{
		UGameplayStatics::ApplyDamage(Attacker, 32.f, GetOwner()->GetInstigatorController(), GetOwner(), nullptr);
	}
	ReturnToReady();
	OnCombatAction(TEXT("ParryCounter"));
	OnParryWindowOpened();
}

void URelicCombatComponent::FirePistol()
{
	ACharacter* Character = GetOwnerCharacter();
	if (!Character || CombatState == ERelicCombatState::Dodging || CombatState == ERelicCombatState::Staggered) return;
	OnCombatAction(TEXT("FirePistol"));
	const FVector Start = Character->GetPawnViewLocation();
	const FVector End = Start + Character->GetControlRotation().Vector() * PistolRange;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RelicPistol), true, Character);
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), PistolDamage, (End - Start).GetSafeNormal(), Hit,
			Character->GetController(), Character, nullptr);
	}
}

void URelicCombatComponent::Overload(AActor* Target)
{
	if (!IsValid(Target) || NeuralHeat + OverloadHeat > MaxNeuralHeat) return;
	NeuralHeat = FMath::Clamp(NeuralHeat + OverloadHeat, 0.f, MaxNeuralHeat);
	UGameplayStatics::ApplyDamage(Target, OverloadDamage, GetOwner()->GetInstigatorController(), GetOwner(), URelicOverloadDamageType::StaticClass());
	OnOverloadApplied(Target, NeuralHeat);
}

void URelicCombatComponent::SetTemporaryState(ERelicCombatState NewState, float Duration)
{
	CombatState = NewState;
	GetWorld()->GetTimerManager().ClearTimer(StateTimer);
	GetWorld()->GetTimerManager().SetTimer(StateTimer, this, &URelicCombatComponent::ReturnToReady, Duration, false);
}

void URelicCombatComponent::ReturnToReady()
{
	CombatState = ERelicCombatState::Ready;
}

void URelicCombatComponent::CoolHeat()
{
	NeuralHeat = FMath::Max(0.f, NeuralHeat - 3.f);
}
