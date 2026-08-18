// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETimeScale : uint8
{
	Quarterly	UMETA(DisplayName = "季度（非战时）"),
	Weekly		UMETA(DisplayName = "周（战时无战役）"),
	FourHour	UMETA(DisplayName = "四小时（战役阶段）"),
	Paused		UMETA(DisplayName = "暂停")
};

class EMBERREALM_API FGameClock
{
public:
	FGameClock();
	~FGameClock();

	void AdvanceTurn();
	void SetTimeScale(ETimeScale NewScale);
	ETimeScale GetTimeScale() const { return CurrentTimeScale; }
	int32 GetCurrentDay() const { return TotalDaysElapsed; }
	int32 GetCurrentYear() const { return TotalDaysElapsed / 360 + 1; }
	int32 GetCurrentQuarter() const { return (TotalDaysElapsed % 360) / 90 + 1; }
	void EvaluateTimeScaleChange(bool bIsAnyCombatActive, bool bIsCampaignActive);

private:
	ETimeScale CurrentTimeScale = ETimeScale::Quarterly;
	int32 TotalDaysElapsed = 0;
	int32 ConsecutivePeaceTurns = 0;

	static constexpr int32 DaysPerQuarter = 90;
	static constexpr int32 DaysPerWeek = 7;
	static constexpr float DaysPerFourHour = 4.0f / 24.0f;
};
