// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

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

USTRUCT(BlueprintType)
struct EMBERREALM_API FStrategyTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "阵地")
	int32 TileID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "阵地")
	ETerrainType TerrainType = ETerrainType::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "地形", meta = (ClampMin = "1", ClampMax = "5"))
	int32 Elevation = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "控制")
	EFactionType Controller = EFactionType::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "经济")
	int32 TaxIncome = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "经济")
	int32 ResourceOutput = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "阵地")
	FText TileName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "连接")
	TArray<int32> AdjacentTiles;
};
