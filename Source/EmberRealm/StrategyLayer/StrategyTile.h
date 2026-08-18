// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 阵地类型枚举
 */
UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	Normal			UMETA(DisplayName = "普通阵地"),
	Residential		UMETA(DisplayName = "居民区"),
	IronMine		UMETA(DisplayName = "铁矿矿区"),
	OilField		UMETA(DisplayName = "石油矿区"),
	CottonField		UMETA(DisplayName = "棉花地"),
	MilitaryBase	UMETA(DisplayName = "军事基地"),
	TransportHub	UMETA(DisplayName = "交通枢纽"),
	Capital			UMETA(DisplayName = "首都/核心城市")
};

/**
 * 战略地图阵地数据
 */
USTRUCT(BlueprintType)
struct EMBERREALM_API FStrategyTile
{
	GENERATED_BODY()

	/** 阵地ID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "阵地")
	int32 TileID = -1;

	/** 阵地类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "阵地")
	ETerrainType TerrainType = ETerrainType::Normal;

	/** 海拔阶数 (1-5) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "地形", meta = (ClampMin = "1", ClampMax = "5"))
	int32 Elevation = 1;

	/** 控制阵营 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "控制")
	EFactionType Controller = EFactionType::Neutral;

	/** 每季度税收（卡亚币） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "经济")
	int32 TaxIncome = 0;

	/** 资源产出（每季度） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "经济")
	int32 ResourceOutput = 0;

	/** 阵地名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "阵地")
	FText TileName;

	/** 相邻阵地ID列表 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "连接")
	TArray<int32> AdjacentTiles;
};
