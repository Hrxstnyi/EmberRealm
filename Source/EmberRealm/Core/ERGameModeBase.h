// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/GameClock.h"
#include "ERGameModeBase.generated.h"

UCLASS()
class EMBERREALM_API AERGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AERGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "回合")
	void EndTurn();

	UFUNCTION(BlueprintPure, Category = "回合")
	int32 GetCurrentTurn() const { return CurrentTurn; }

	FGameClock& GetGameClock() { return GameClock; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "回合")
	int32 CurrentTurn = 1;

	FGameClock GameClock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "内鬼")
	bool bEnableInternalSaboteur = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "内鬼", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SaboteurSpawnChance = 0.15f;

private:
	void OnTurnStart();
	void OnTurnEnd();
};
