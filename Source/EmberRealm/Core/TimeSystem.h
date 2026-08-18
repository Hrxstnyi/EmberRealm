// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/EmberStructs.h"
#include "TimeSystem.generated.h"

/**
 * 三阶动态时间流速系统（规则全书第二章）
 * 非战时：季度为单位
 * 战时无战役：周为单位
 * 战役阶段：四小时时段为单位
 */
UCLASS(BlueprintType)
class EMBERREALM_API UTimeSystem : public UObject
{
	GENERATED_BODY()

public:
	UTimeSystem();

	/** 推进一个时间单位（根据当前相位） */
	void Advance();

	/** 根据战局自动判定/更新时间相位 */
	void UpdatePhase();

	/** 强制进入战役阶段（宣战/交火触发） */
	void EnterBattlePhase();

	/** 强制进入非战时（停火协议） */
	void EnterPeacePhase();

	/** 退回战时无战役（连续两日无交火） */
	void EnterWartimePhase();

	UFUNCTION(BlueprintPure, Category = "EmberRealm|Time")
	FGameTime GetCurrentTime() const { return CurrentTime; }

	UFUNCTION(BlueprintPure, Category = "EmberRealm|Time")
	ETimePhase GetPhase() const { return CurrentTime.TimePhase; }

	/** 获取当前相位下每回合行动点回复量 */
	int32 GetActionPointRecovery() const;

private:
	FGameTime CurrentTime;
	int32 DaysWithoutCombat = 0; // 连续无交火天数

	void AdvancePeace();
	void AdvanceWartime();
	void AdvanceBattle();
	void UpdateSeason();
};
