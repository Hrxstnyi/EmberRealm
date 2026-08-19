// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DataSystem/ERFactionData.h"
#include "DataSystem/ERUnitData.h"
#include "EmberEnums.generated.h"

UENUM(BlueprintType)
enum class ETimePhase : uint8
{
	Peace		UMETA(DisplayName = "非战时(季度)"),
	Wartime		UMETA(DisplayName = "战时(周)"),
	Battle		UMETA(DisplayName = "战役(四小时)")
};

UENUM(BlueprintType)
enum class ESeason : uint8
{
	Spring		UMETA(DisplayName = "春季"),
	Summer		UMETA(DisplayName = "夏季"),
	Autumn		UMETA(DisplayName = "秋季"),
	Winter		UMETA(DisplayName = "冬季")
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Plain		UMETA(DisplayName = "普通阵地"),
	Residential	UMETA(DisplayName = "居民区"),
	IronMine	UMETA(DisplayName = "铁矿矿区"),
	OilField	UMETA(DisplayName = "石油矿区"),
	CottonField	UMETA(DisplayName = "棉花地"),
	Hub			UMETA(DisplayName = "交通枢纽"),
	Base		UMETA(DisplayName = "军事基地"),
	Mountain	UMETA(DisplayName = "山地")
};

UENUM(BlueprintType)
enum class EUnitCategory : uint8
{
	Land		UMETA(DisplayName = "陆地单位"),
	Air			UMETA(DisplayName = "空中单位"),
	Naval		UMETA(DisplayName = "海上单位")
};

UENUM(BlueprintType)
enum class ECardCost : uint8
{
	None = 0 UMETA(Hidden),
	One = 1, Two = 2, Three = 3, Four = 4, Five = 5
};

UENUM(BlueprintType)
enum class EVictoryType : uint8
{
	Hegemony,
	Military,
	Economic,
	Insider
};
