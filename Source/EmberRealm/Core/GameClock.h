// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 时间流速枚举 - 三阶时间系统
 * 和平时期：季度（90天/回合）
 * 战时无战役：周（7天/回合）
 * 战役阶段：四小时（4小时/回合）
 */
UENUM(BlueprintType)
enum class ETimeScale : uint8
{
	Quarterly	UMETA(DisplayName = "季度（非战时）"),
	Weekly		UMETA(DisplayName = "周（战时无战役）"),
	FourHour	UMETA(DisplayName = "四小时（战役阶段）"),
	Paused		UMETA(DisplayName = "暂停")
};

/**
 * 游戏时钟管理器
 * 负责三阶时间流速的切换、游戏内日期追踪、回合推进
 */
class EMBERREALM_API FGameClock
{
public:
	FGameClock();
	~FGameClock();

	/** 推进一个回合 */
	void AdvanceTurn();

	/** 设置时间流速 */
	void SetTimeScale(ETimeScale NewScale);

	/** 获取当前时间流速 */
	ETimeScale GetTimeScale() const { return CurrentTimeScale; }

	/** 获取当前游戏内日期（天） */
	int32 GetCurrentDay() const { return TotalDaysElapsed; }

	/** 获取当前游戏年份 */
	int32 GetCurrentYear() const { return TotalDaysElapsed / 360 + 1; }

	/** 获取当前季度 (1-4) */
	int32 GetCurrentQuarter() const { return (TotalDaysElapsed % 360) / 90 + 1; }

	/** 检测是否应切换时间流速 */
	void EvaluateTimeScaleChange(bool bIsAnyCombatActive, bool bIsCampaignActive);

private:
	ETimeScale CurrentTimeScale = ETimeScale::Quarterly;
	int32 TotalDaysElapsed = 0;
	int32 ConsecutivePeaceTurns = 0;

	/** 各流速对应的天数/回合 */
	static constexpr int32 DaysPerQuarter = 90;
	static constexpr int32 DaysPerWeek = 7;
	static constexpr float DaysPerFourHour = 4.0f / 24.0f;
};
