#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RelicCombatComponent.generated.h"

UENUM(BlueprintType)
enum class ERelicCombatState : uint8
{
	Ready,
	Attacking,
	Dodging,
	Parrying,
	Staggered
};

/** Prototype combat actions. Add this component to an ACharacter-derived player. */
UCLASS(ClassGroup=(RelicRust), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class RELICRUST_API URelicCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URelicCombatComponent();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void LightAttack();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void HeavyAttack();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void Dodge();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void BeginParry();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void EndParry();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void FirePistol();

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void Overload(AActor* Target);

	UFUNCTION(BlueprintCallable, Category="Relic|Combat")
	void NotifyParrySucceeded(AActor* Attacker);

	UFUNCTION(BlueprintPure, Category="Relic|Combat")
	ERelicCombatState GetCombatState() const { return CombatState; }

	UFUNCTION(BlueprintPure, Category="Relic|Combat")
	float GetNeuralHeat() const { return NeuralHeat; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Melee")
	float MeleeDamage = 25.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Melee")
	float MeleeRange = 175.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Melee")
	float MeleeRadius = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Ranged")
	float PistolDamage = 18.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Ranged")
	float PistolRange = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Hack")
	float OverloadDamage = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Hack")
	float OverloadHeat = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat|Hack")
	float MaxNeuralHeat = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat")
	float ActionCommitmentSeconds = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relic|Combat")
	float DodgeImpulse = 650.f;

	UPROPERTY(BlueprintReadOnly, Category="Relic|Combat")
	ERelicCombatState CombatState = ERelicCombatState::Ready;

	UPROPERTY(BlueprintReadOnly, Category="Relic|Combat|Hack")
	float NeuralHeat = 0.f;

	UFUNCTION(BlueprintImplementableEvent, Category="Relic|Combat")
	void OnCombatAction(const FName ActionName);

	UFUNCTION(BlueprintImplementableEvent, Category="Relic|Combat")
	void OnOverloadApplied(AActor* Target, float NewHeat);

	UFUNCTION(BlueprintImplementableEvent, Category="Relic|Combat")
	void OnParryWindowOpened();

private:
	FTimerHandle StateTimer;
	FTimerHandle HeatTimer;
	float LastLightAttackTime = -100.f;
	int32 LightComboIndex = -1;
	void SetTemporaryState(ERelicCombatState NewState, float Duration);
	void ReturnToReady();
	void CoolHeat();
	ACharacter* GetOwnerCharacter() const;
};
