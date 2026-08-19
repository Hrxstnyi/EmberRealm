// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StrategyLayer/StrategyMapManager.h"
#include "StrategyLayer/EconomySystem.h"
#include "ERGameInstance.generated.h"

UCLASS()
class EMBERREALM_API UERGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UERGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	bool bAutoSaveEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	int32 AutoSaveIntervalMinutes = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	int32 MaxSaveSlots = 10;

	UPROPERTY(BlueprintReadOnly, Category = "阵营")
	int32 SelectedFactionIndex = -1;

	UFUNCTION(BlueprintCallable, Category = "阵营")
	void SetSelectedFaction(int32 FactionIndex);

	FStrategyMapManager& GetStrategyMapManager() { return StrategyMap; }
	const FStrategyMapManager& GetStrategyMapManager() const { return StrategyMap; }

	FEconomySystem* GetEconomySystem(EFactionType Faction);

	UFUNCTION(BlueprintCallable, Category = "游戏")
	void StartNewGame(int32 MapSizeX = 12, int32 MapSizeY = 10);

	UFUNCTION(BlueprintCallable, Category = "回合")
	void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "回合")
	int32 GetCurrentTurn() const { return CurrentTurn; }

private:
	FStrategyMapManager StrategyMap;
	TMap<EFactionType, TSharedPtr<FEconomySystem>> EconomySystems;
	int32 CurrentTurn = 1;

	void InitializeEconomySystems();
};
