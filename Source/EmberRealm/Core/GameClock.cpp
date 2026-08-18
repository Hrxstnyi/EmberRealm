// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/GameClock.h"

FGameClock::FGameClock()
{
	UE_LOG(LogTemp, Log, TEXT("[GameClock] 游戏时钟初始化，初始流速: 季度"));
}

FGameClock::~FGameClock()
{
}

void FGameClock::AdvanceTurn()
{
	int32 DaysToAdvance = 0;

	switch (CurrentTimeScale)
	{
	case ETimeScale::Quarterly:
		DaysToAdvance = DaysPerQuarter;
		break;
	case ETimeScale::Weekly:
		DaysToAdvance = DaysPerWeek;
		break;
	case ETimeScale::FourHour:
		DaysToAdvance = FMath::RoundToInt(DaysPerFourHour);
		break;
	case ETimeScale::Paused:
	default:
		break;
	}

	if (DaysToAdvance > 0)
	{
		TotalDaysElapsed += DaysToAdvance;
		UE_LOG(LogTemp, Log, TEXT("[GameClock] 回合推进，流逝 %d 天，总天数: %d (第%d年第%d季度)"),
			DaysToAdvance, TotalDaysElapsed, GetCurrentYear(), GetCurrentQuarter());
	}
}

void FGameClock::SetTimeScale(ETimeScale NewScale)
{
	if (CurrentTimeScale != NewScale)
	{
		ETimeScale OldScale = CurrentTimeScale;
		CurrentTimeScale = NewScale;

		const TCHAR* OldName = TEXT("");
		const TCHAR* NewName = TEXT("");
		switch (OldScale)
		{
		case ETimeScale::Quarterly: OldName = TEXT("季度"); break;
		case ETimeScale::Weekly: OldName = TEXT("周"); break;
		case ETimeScale::FourHour: OldName = TEXT("四小时"); break;
		case ETimeScale::Paused: OldName = TEXT("暂停"); break;
		}
		switch (NewScale)
		{
		case ETimeScale::Quarterly: NewName = TEXT("季度"); break;
		case ETimeScale::Weekly: NewName = TEXT("周"); break;
		case ETimeScale::FourHour: NewName = TEXT("四小时"); break;
		case ETimeScale::Paused: NewName = TEXT("暂停"); break;
		}

		UE_LOG(LogTemp, Log, TEXT("[GameClock] 时间流速切换: %s -> %s"), OldName, NewName);

		FEventBus::Get().Broadcast(EREvents::OnTimeScaleChanged,
			FString::Printf(TEXT("{\"old\":\"%s\",\"new\":\"%s\"}"), OldName, NewName));
	}
}

void FGameClock::EvaluateTimeScaleChange(bool bIsAnyCombatActive, bool bIsCampaignActive)
{
	if (bIsCampaignActive)
	{
		SetTimeScale(ETimeScale::FourHour);
		ConsecutivePeaceTurns = 0;
	}
	else if (bIsAnyCombatActive)
	{
		SetTimeScale(ETimeScale::Weekly);
		ConsecutivePeaceTurns = 0;
	}
	else
	{
		ConsecutivePeaceTurns++;
		if (ConsecutivePeaceTurns >= 2)
		{
			SetTimeScale(ETimeScale::Quarterly);
		}
		else
		{
			SetTimeScale(ETimeScale::Weekly);
		}
	}
}
