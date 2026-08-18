// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/GameClock.h"
#include "ERGameModeBase.generated.h"

/**
 * 游戏主模式
 * 管理回合流程、时间流速、全局事件
 */
UCLASS()
class EMBERREALM_API AERGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AERGameModeBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** 结束当前回合 */
	UFUNCTION(BlueprintCallable, Category = "回合")
	void EndTurn();

	/** 获取当前回合数 */
	UFUNCTION(BlueprintPure, Category = "回合")
	int32 GetCurrentTurn() const { return CurrentTurn; }

	/** 获取游戏时钟 */
	FGameClock& GetGameClock() { return GameClock; }

protected:
	/** 当前回合数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "回合")
	int32 CurrentTurn = 1;

	/** 游戏时钟 */
	FGameClock GameClock;

	/** 是否启用内鬼系统 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "内鬼")
	bool bEnableInternalSaboteur = true;

	/** 内鬼刷新概率（每回合） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "内鬼", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SaboteurSpawnChance = 0.15f;

private:
	/** 处理回合开始逻辑 */
	void OnTurnStart();

	/** 处理回合结束逻辑 */
	void OnTurnEnd();
};
