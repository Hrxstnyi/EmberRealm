// Copyright Epic Games, Inc. All Rights Reserved.

#include "StrategyLayer/CombatResolver.h"
#include "Core/EventBus.h"

FCombatResolver::FCombatResolver()
{
}

FCombatResolver::~FCombatResolver() = default;

FCombatResult FCombatResolver::ResolveBattle(
	TArray<FCombatUnit>& Attackers,
	TArray<FCombatUnit>& Defenders,
	const FStrategyTile& DefenderTile)
{
	FCombatResult Result;

	if (Attackers.Num() == 0 || Defenders.Num() == 0)
	{
		Result.BattleLog = TEXT("无效战斗：一方无单位");
		return Result;
	}

	const ETerrainType Terrain = DefenderTile.TerrainType;

	const float AttackerPower = CalculateTotalAttack(Attackers, Terrain, true);
	const float DefenderPower = CalculateTotalDefense(Defenders, Terrain);

	UE_LOG(LogTemp, Log, TEXT("[CombatResolver] 进攻方战力: %.1f, 防守方战力: %.1f"),
		AttackerPower, DefenderPower);

	const float VictoryThreshold = DefenderPower * 1.1f;
	Result.bAttackerWon = AttackerPower > VictoryThreshold;

	const float AttackerDamageRatio = FMath::Clamp(DefenderPower / FMath::Max(AttackerPower, 1.0f) * 0.4f, 0.05f, 0.6f);
	const float DefenderDamageRatio = FMath::Clamp(AttackerPower / FMath::Max(DefenderPower, 1.0f) * 0.35f, 0.05f, 0.6f);

	const float RandFactor = 0.85f + FMath::FRand() * 0.3f;
	const float FinalAttackerRatio = AttackerDamageRatio * RandFactor;
	const float FinalDefenderRatio = DefenderDamageRatio * RandFactor;

	int32 TotalAttackerStrength = 0;
	for (const FCombatUnit& U : Attackers) TotalAttackerStrength += U.CurrentStrength;
	Result.AttackerLosses = FMath::RoundToInt(TotalAttackerStrength * FinalAttackerRatio);

	int32 TotalDefenderStrength = 0;
	for (const FCombatUnit& U : Defenders) TotalDefenderStrength += U.CurrentStrength;
	Result.DefenderLosses = FMath::RoundToInt(TotalDefenderStrength * FinalDefenderRatio);

	ApplyLosses(Attackers, Result.AttackerLosses, Result.AttackerRouted);
	ApplyLosses(Defenders, Result.DefenderLosses, Result.DefenderRouted);

	UpdateMorale(Attackers, FinalAttackerRatio, Result.bAttackerWon);
	UpdateMorale(Defenders, FinalDefenderRatio, !Result.bAttackerWon);

	Result.bTerritoryCaptured = Result.bAttackerWon && Defenders.FilterByPredicate([](const FCombatUnit& U) {
		return !U.IsRouted() && U.CurrentStrength > 0;
	}).Num() == 0;

	Result.BattleLog = FString::Printf(
		TEXT("战斗结束！进攻方损失 %d (溃败 %d), 防守方损失 %d (溃败 %d). %s"),
		Result.AttackerLosses, Result.AttackerRouted,
		Result.DefenderLosses, Result.DefenderRouted,
		Result.bAttackerWon ? TEXT("进攻方胜利") : TEXT("防守方胜利"));

	if (Result.bTerritoryCaptured)
	{
		Result.BattleLog += TEXT("，阵地被占领！");
	}

	UE_LOG(LogTemp, Log, TEXT("[CombatResolver] %s"), *Result.BattleLog);

	FEventBus::Get().Broadcast(EREvents::OnBattleResult,
		FString::Printf(TEXT("{\"attacker_loss\":%d,\"defender_loss\":%d,\"attacker_won\":%s,\"captured\":%s}"),
			Result.AttackerLosses, Result.DefenderLosses,
			Result.bAttackerWon ? TEXT("true") : TEXT("false"),
			Result.bTerritoryCaptured ? TEXT("true") : TEXT("false")));

	return Result;
}

float FCombatResolver::CalculateTotalAttack(const TArray<FCombatUnit>& Units, ETerrainType Terrain, bool bIsAttacker) const
{
	float Total = 0.0f;
	for (const FCombatUnit& Unit : Units)
	{
		if (Unit.CurrentStrength > 0 && !Unit.IsRouted())
		{
			Total += CalculateUnitAttack(Unit, Terrain, bIsAttacker);
		}
	}
	return Total;
}

float FCombatResolver::CalculateTotalDefense(const TArray<FCombatUnit>& Units, ETerrainType Terrain) const
{
	float Total = 0.0f;
	for (const FCombatUnit& Unit : Units)
	{
		if (Unit.CurrentStrength > 0 && !Unit.IsRouted())
		{
			Total += CalculateUnitDefense(Unit, Terrain);
		}
	}
	return Total;
}

float FCombatResolver::CalculateUnitAttack(const FCombatUnit& Unit, ETerrainType Terrain, bool bIsAttacker) const
{
	if (!Unit.UnitData) return 0.0f;

	float Attack = Unit.UnitData->AttackDamage;
	Attack *= Unit.GetStrengthRatio();
	Attack *= (Unit.Morale / 80.0f);
	Attack *= (1.0f + Unit.GetRankBonus());
	Attack *= (1.0f + ECombatBalance::GetTerrainAttackBonus(Terrain));

	if (Unit.CurrentAmmo < Unit.UnitData->MaxAmmo * 0.3f)
	{
		Attack *= 0.6f;
	}

	return Attack;
}

float FCombatResolver::CalculateUnitDefense(const FCombatUnit& Unit, ETerrainType Terrain) const
{
	if (!Unit.UnitData) return 0.0f;

	float Defense = Unit.UnitData->Defense;
	Defense *= Unit.GetStrengthRatio();
	Defense *= (Unit.Morale / 80.0f);
	Defense *= (1.0f + Unit.GetRankBonus());
	Defense *= (1.0f + ECombatBalance::GetTerrainDefenseBonus(Terrain));

	if (Unit.bIsDefending)
	{
		Defense *= 1.2f;
	}

	return Defense;
}

void FCombatResolver::ApplyLosses(TArray<FCombatUnit>& Units, float TotalDamage, int32& OutRoutedCount)
{
	OutRoutedCount = 0;
	if (Units.Num() == 0 || TotalDamage <= 0) return;

	float TotalDefense = 0.0f;
	for (const FCombatUnit& U : Units)
	{
		if (U.CurrentStrength > 0)
		{
			TotalDefense += U.UnitData ? U.UnitData->Defense * U.GetStrengthRatio() : 10.0f;
		}
	}

	for (FCombatUnit& Unit : Units)
	{
		if (Unit.CurrentStrength <= 0) continue;

		const float UnitDefense = Unit.UnitData ? Unit.UnitData->Defense * Unit.GetStrengthRatio() : 10.0f;
		const float ShareRatio = TotalDefense > 0 ? (1.0f - UnitDefense / TotalDefense) / Units.Num() : 1.0f / Units.Num();
		const int32 UnitLoss = FMath::RoundToInt(TotalDamage * FMath::Clamp(ShareRatio * Units.Num(), 0.2f, 2.0f) / Units.Num());

		Unit.CurrentStrength = FMath::Max(0, Unit.CurrentStrength - UnitLoss);

		if (Unit.UnitData)
		{
			Unit.CurrentAmmo = FMath::Max(0, Unit.CurrentAmmo - FMath::RoundToInt(Unit.UnitData->MaxAmmo * 0.2f));
		}

		if (Unit.IsRouted())
		{
			OutRoutedCount++;
		}
	}
}

void FCombatResolver::UpdateMorale(TArray<FCombatUnit>& Units, float DamageRatio, bool bWon)
{
	for (FCombatUnit& Unit : Units)
	{
		float MoraleChange = -DamageRatio * 30.0f;
		MoraleChange += bWon ? 10.0f : -15.0f;
		MoraleChange *= (1.0f - Unit.GetRankBonus() * 0.5f);
		Unit.Morale = FMath::Clamp(Unit.Morale + MoraleChange, 0.0f, 100.0f);
	}
}
