#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RelicEnemy.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class RELICRUST_API ARelicEnemy : public AActor
{
	GENERATED_BODY()

public:
	ARelicEnemy();
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void SetRanged(bool bInRanged) { bRanged = bInRanged; }
	void SetDisplayName(const FString& Name) { DisplayName = Name; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CollisionCapsule;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> HeadMesh;
	UPROPERTY(EditAnywhere, Category="Enemy")
	float Health = 80.f;
	UPROPERTY(EditAnywhere, Category="Enemy")
	float MaxHealth = 80.f;
	UPROPERTY(EditAnywhere, Category="Enemy")
	float MoveSpeed = 210.f;
	UPROPERTY(EditAnywhere, Category="Enemy")
	float AttackRange = 155.f;
	UPROPERTY(EditAnywhere, Category="Enemy")
	float AttackCooldown = 1.6f;
	UPROPERTY(EditAnywhere, Category="Enemy")
	bool bRanged = false;
	FString DisplayName = TEXT("Meridian Enforcer");
	float AttackTimer = 0.f;
	float WindupRemaining = 0.f;
	float StaggerRemaining = 0.f;
	bool bAttackWindupActive = false;
	void AttackPlayer(class ARelicPlayerCharacter* Player);
};
