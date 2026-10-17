#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RelicObjective.generated.h"

class UStaticMeshComponent;

UCLASS()
class RELICRUST_API ARelicObjective : public AActor
{
	GENERATED_BODY()

public:
	ARelicObjective();
	virtual void Tick(float DeltaSeconds) override;
	void SetRelay(bool bInRelay) { bRelay = bInRelay; }
	void Interact();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	bool bRelay = false;
};
