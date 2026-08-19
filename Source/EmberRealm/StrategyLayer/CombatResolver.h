// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DataSystem/ERUnitData.h"
#include "StrategyLayer/StrategyTile.h"

USTRUCT(BlueprintType)
struct FCombatUnit
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	int32 UnitID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	TObjectPtr<UERUnitData> UnitData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	EFactionType Faction = EFactionType::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	int32 CurrentStrength = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	int32 MaxStrength = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位", meta = (ClampMin = "0", ClampMax = "100"))
	float Morale = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	ERankLevel Rank = ERankLevel::SecondLieutenant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	int32 CurrentAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	bool bIsDefending = false;

	float GetStrengthRatio() const { return MaxStrength > 0 ? (float)CurrentStrength / MaxStrength : 0.f; }
	bool IsRouted() const { return Morale < 20.0f || CurrentStrength <= 0; }

	float GetRankBonus() const
	{
		return (float)((int32)Rank + 1) / 34.0f;
	}
};

USTRUCT(BlueprintType)
struct FCombatResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	int32 AttackerLosses = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	int32 DefenderLosses = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	bool bAttackerWon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	bool bTerritoryCaptured = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	int32 AttackerRouted = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	int32 DefenderRouted = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "结果")
	FString BattleLog;
};

namespace ECombatBalance
{
	static float GetCounterMultiplier(EUnitType Attacker, EUnitType Defender)
	{
		if (Attacker == EUnitType::Infantry && Defender == EUnitType::Guerrilla) return 1.3f;
		if (Attacker == EUnitType::Armor && Defender == EUnitType::Infantry) return 1.4f;
		if (Attacker == EUnitType::AirDefense && Defender == EUnitType::Pilot) return 1.5f;
		if (Attacker == EUnitType::Coastal && Defender == EUnitType::Warship) return 1.4f;
		if (Attacker == EUnitType::Sniper && Defender == EUnitType::Officer) return 1.6f;
		if (Attacker == EUnitType::Guerrilla && Defender == EUnitType::Artillery) return 1.3f;
		if (Attacker == EUnitType::Pilot && Defender == EUnitType::Armor) return 1.3f;
		if (Attacker == EUnitType::Submarine && Defender == EUnitType::Warship) return 1.5f;
		if (Attacker == EUnitType::Engineer && Defender == EUnitType::Coastal) return 1.2f;

		if (Defender == EUnitType::Infantry && Attacker == EUnitType::Armor) return 0.7f;
		if (Defender == EUnitType::Pilot && Attacker == EUnitType::AirDefense) return 0.6f;
		if (Defender == EUnitType::Warship && Attacker == EUnitType::Coastal) return 0.7f;

		return 1.0f;
	}

	static float GetTerrainDefenseBonus(ETerrainType Terrain)
	{
		switch (Terrain)
		{
		case ETerrainType::MilitaryBase:	return 0.30f;
		case ETerrainType::Residential:		return 0.20f;
		case ETerrainType::Capital:			return 0.25f;
		case ETerrainType::TransportHub:	return 0.10f;
		case ETerrainType::IronMine:		return 0.05f;
		case ETerrainType::OilField:		return 0.05f;
		default:						return 0.0f;
		}
	}

	static float GetTerrainAttackBonus(ETerrainType Terrain)
	{
		switch (Terrain)
		{
		case ETerrainType::MilitaryBase:	return 0.15f;
		case ETerrainType::TransportHub:	return 0.10f;
		default:						return 0.0f;
		}
	}
}

class EMBERREALM_API FCombatResolver
{
public:
	FCombatResolver();
	~FCombatResolver();

	FCombatResult ResolveBattle(
		TArray<FCombatUnit>& Attackers,
		TArray<FCombatUnit>& Defenders,
		const FStrategyTile& DefenderTile
	);

private:
	float CalculateTotalAttack(const TArray<FCombatUnit>& Units, ETerrainType Terrain, bool bIsAttacker) const;
	float CalculateTotalDefense(const TArray<FCombatUnit>& Units, ETerrainType Terrain) const;
	float CalculateUnitAttack(const FCombatUnit& Unit, ETerrainType Terrain, bool bIsAttacker) const;
	float CalculateUnitDefense(const FCombatUnit& Unit, ETerrainType Terrain) const;
	void ApplyLosses(TArray<FCombatUnit>& Units, float TotalDamage, int32& OutRoutedCount);
	void UpdateMorale(TArray<FCombatUnit>& Units, float DamageRatio, bool bWon);
};
