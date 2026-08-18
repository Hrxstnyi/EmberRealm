// Copyright EmberRealm. All Rights Reserved.
#include "Core/TimeSystem.h"

UTimeSystem::UTimeSystem()
{
	CurrentTime = FGameTime();
	CurrentTime.Year = 1;
	CurrentTime.Quarter = 1;
	CurrentTime.Week = 1;
	CurrentTime.Season = ESeason::Spring;
	CurrentTime.TimePhase = ETimePhase::Peace;
	CurrentTime.BattleHours = 0;
}

void UTimeSystem::Advance()
{
	switch (CurrentTime.TimePhase)
	{
	case ETimePhase::Peace:	AdvancePeace(); break;
	case ETimePhase::Wartime:	AdvanceWartime(); break;
	case ETimePhase::Battle:	AdvanceBattle(); break;
	}
	UpdateSeason();
}

void UTimeSystem::AdvancePeace()
{
	// 非战时以季度为单位
	CurrentTime.Quarter++;
	if (CurrentTime.Quarter > 4)
	{
		CurrentTime.Quarter = 1;
		CurrentTime.Year++;
	}
	CurrentTime.Week = 1;
	UE_LOG(LogTemp, Log, TEXT("[TimeSystem] 非战时推进 -> 第%d年 第%d季度"),
		CurrentTime.Year, CurrentTime.Quarter);
}

void UTimeSystem::AdvanceWartime()
{
	// 战时以周为单位
	CurrentTime.Week++;
	if (CurrentTime.Week > 13) // 每季度约13周
	{
		CurrentTime.Week = 1;
		CurrentTime.Quarter++;
		if (CurrentTime.Quarter > 4)
		{
			CurrentTime.Quarter = 1;
			CurrentTime.Year++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("[TimeSystem] 战时推进 -> 第%d年 第%d季度 第%d周"),
		CurrentTime.Year, CurrentTime.Quarter, CurrentTime.Week);
}

void UTimeSystem::AdvanceBattle()
{
	// 战役以四小时为单位
	CurrentTime.BattleHours++;
	if (CurrentTime.BattleHours >= 6) // 一天6个四小时时段
	{
		CurrentTime.BattleHours = 0;
		CurrentTime.Week++;
	}
	UE_LOG(LogTemp, Log, TEXT("[TimeSystem] 战役推进 -> 四小时时段 %d"), CurrentTime.BattleHours);
}

void UTimeSystem::UpdatePhase()
{
	// 规则2.2：全域无交火连续两日 → 退回战时无战役
	if (CurrentTime.TimePhase == ETimePhase::Battle)
	{
		DaysWithoutCombat++;
		if (DaysWithoutCombat >= 2)
		{
			EnterWartimePhase();
		}
	}
}

void UTimeSystem::EnterBattlePhase()
{
	CurrentTime.TimePhase = ETimePhase::Battle;
	CurrentTime.BattleHours = 0;
	DaysWithoutCombat = 0;
	UE_LOG(LogTemp, Log, TEXT("[TimeSystem] 进入战役阶段（四小时流速）"));
}

void UTimeSystem::EnterPeacePhase()
{
	CurrentTime.TimePhase = ETimePhase::Peace;
	DaysWithoutCombat = 0;
	UE_LOG(LogTemp, Log, TEXT("[TimeSystem] 签署停火，回归非战时（季度流速）"));
}

void UTimeSystem::EnterWartimePhase()
{
	CurrentTime.TimePhase = ETimePhase::Wartime;
	UE_LOG(LogTemp, Log, TEXT("[TimeSystem] 退回战时无战役（周流速）"));
}

void UTimeSystem::UpdateSeason()
{
	switch (CurrentTime.Quarter)
	{
	case 1: CurrentTime.Season = ESeason::Spring; break;
	case 2: CurrentTime.Season = ESeason::Summer; break;
	case 3: CurrentTime.Season = ESeason::Autumn; break;
	case 4: CurrentTime.Season = ESeason::Winter; break;
	}
}

int32 UTimeSystem::GetActionPointRecovery() const
{
	// 规则第四章：每季6点、每周3点
	switch (CurrentTime.TimePhase)
	{
	case ETimePhase::Peace:	return 6;
	case ETimePhase::Wartime:	return 3;
	case ETimePhase::Battle:	return 2; // 战役时段回复少
	default: return 3;
	}
}
