// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EmberEnums.h"
#include "EmberStructs.generated.h"

/** 兵种静态数据表（DataTable行结构） */
USTRUCT(BlueprintType)
struct FUnitStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) EUnitType UnitType = EUnitType::Militia;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EUnitCategory Category = EUnitCategory::Land;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AttackDamage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AttackRange = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MoveRange = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxAmmo = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TrainCost = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 TrainTurns = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 FuelCostPerTurn = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Initiative = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ERankLevel RequiredRank = ERankLevel::SecondLieutenant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ActiveSkillDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText PassiveSkillDesc;
};

/** 运行时单位实例数据 */
USTRUCT(BlueprintType)
struct FUnitInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) FGuid InstanceId;
	UPROPERTY(BlueprintReadWrite) EUnitType UnitType = EUnitType::Militia;
	UPROPERTY(BlueprintReadWrite) EFactionType OwnerFaction = EFactionType::Neutral;
	UPROPERTY(BlueprintReadWrite) int32 CurrentHealth = 100;
	UPROPERTY(BlueprintReadWrite) int32 CurrentAmmo = 10;
	UPROPERTY(BlueprintReadWrite) int32 MeritLevel = 0; // 0-3 勋阶
	UPROPERTY(BlueprintReadWrite) FIntPoint TileCoord;
	UPROPERTY(BlueprintReadWrite) bool bHasMoved = false;
	UPROPERTY(BlueprintReadWrite) bool bHasAttacked = false;
	UPROPERTY(BlueprintReadWrite) bool bIsHidden = false;
};

/** 阵地运行时数据 */
USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) FIntPoint Coord;
	UPROPERTY(BlueprintReadWrite) ETileType TileType = ETileType::Plain;
	UPROPERTY(BlueprintReadWrite) int32 Elevation = 1; // 1-5
	UPROPERTY(BlueprintReadWrite) EFactionType Controller = EFactionType::Neutral;
	UPROPERTY(BlueprintReadWrite) int32 ResourceOutput = 0;
	UPROPERTY(BlueprintReadWrite) bool bIsHub = false;
	UPROPERTY(BlueprintReadWrite) bool bHasBase = false;
};

/** 阵营运行时数据 */
USTRUCT(BlueprintType)
struct FFactionState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) EFactionType Faction = EFactionType::Neutral;
	UPROPERTY(BlueprintReadWrite) FText FactionName;
	UPROPERTY(BlueprintReadWrite) int32 KayaCoins = 200; // 卡亚币
	UPROPERTY(BlueprintReadWrite) int32 Reputation = 10; // 名誉值
	UPROPERTY(BlueprintReadWrite) ERankLevel Rank = ERankLevel::SecondLieutenant;
	UPROPERTY(BlueprintReadWrite) int32 ActionPoints = 6;
	UPROPERTY(BlueprintReadWrite) int32 MaxActionPoints = 11;
	UPROPERTY(BlueprintReadWrite) TArray<FUnitInstance> Units;
	UPROPERTY(BlueprintReadWrite) TArray<FIntPoint> ControlledTiles;
	UPROPERTY(BlueprintReadWrite) TArray<FName> HandCards;
	UPROPERTY(BlueprintReadWrite) bool bIsUnderSanction = false; // 禁闭
	UPROPERTY(BlueprintReadWrite) int32 SanctionTurnsLeft = 0;
	UPROPERTY(BlueprintReadWrite) TArray<EFactionType> Allies;
};

/** 战役结算结果 */
USTRUCT(BlueprintType)
struct FBattleResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) EFactionType Attacker = EFactionType::Neutral;
	UPROPERTY(BlueprintReadWrite) EFactionType Defender = EFactionType::Neutral;
	UPROPERTY(BlueprintReadWrite) bool bAttackerWon = false;
	UPROPERTY(BlueprintReadWrite) bool bSurpriseAttack = false;
	UPROPERTY(BlueprintReadWrite) int32 AttackerLosses = 0;
	UPROPERTY(BlueprintReadWrite) int32 DefenderLosses = 0;
	UPROPERTY(BlueprintReadWrite) FIntPoint TargetTile;
	UPROPERTY(BlueprintReadWrite) FText BattleLog;
};

/** 全局时间状态 */
USTRUCT(BlueprintType)
struct FGameTime
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite) int32 Year = 1;
	UPROPERTY(BlueprintReadWrite) int32 Quarter = 1; // 1-4
	UPROPERTY(BlueprintReadWrite) int32 Week = 1;
	UPROPERTY(BlueprintReadWrite) ESeason Season = ESeason::Spring;
	UPROPERTY(BlueprintReadWrite) ETimePhase TimePhase = ETimePhase::Peace;
	UPROPERTY(BlueprintReadWrite) int32 BattleHours = 0; // 战役内四小时时段计数
};
