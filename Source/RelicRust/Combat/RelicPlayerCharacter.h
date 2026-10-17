#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RelicPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class URelicCombatComponent;

UCLASS()
class RELICRUST_API ARelicPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARelicPlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }
	URelicCombatComponent* GetCombatComponent() const { return Combat; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> ThirdPersonCamera;
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, Category="Appearance")
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	UPROPERTY(VisibleAnywhere, Category="Appearance")
	TObjectPtr<UStaticMeshComponent> HeadMesh;
	UPROPERTY(VisibleAnywhere, Category="Appearance")
	TObjectPtr<UStaticMeshComponent> PackMesh;
	UPROPERTY(VisibleAnywhere, Category="Appearance")
	TObjectPtr<UStaticMeshComponent> BladeMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> SwordGuardMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> SwordGripMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> SwordPommelMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> PistolBodyMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> PistolGripMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> PistolBarrelMesh;
	UPROPERTY(VisibleAnywhere, Category="Weapons")
	TObjectPtr<UStaticMeshComponent> PistolSightMesh;
	UPROPERTY(VisibleAnywhere, Category="Combat")
	TObjectPtr<URelicCombatComponent> Combat;
	UPROPERTY(EditAnywhere, Category="Player", meta=(ClampMin="1"))
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Player")
	float Health = 100.f;
	bool bFirstPerson = false;
	bool bFirstPersonShowsSword = false;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LightAttack();
	void HeavyAttack();
	void FirePistol();
	void UpdateWeaponPresentation();
	void ToggleCamera();
	void TryOverload();
	void TryInteract();
	void ChooseMeridian();
	void ChooseMarches();
	void ChooseContinuum();
};
