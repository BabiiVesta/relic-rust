#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RelicHUD.generated.h"

UCLASS()
class RELICRUST_API ARelicHUD : public AHUD
{
	GENERATED_BODY()
	virtual void DrawHUD() override;
};
