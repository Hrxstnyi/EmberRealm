// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EmberEnums.generated.h"

/** 三大正统阵营 + 隐藏内鬼阵营 */
UENUM(BlueprintType)
enum class EFactionType : uint8
{
	Garrison	UMETA(DisplayName = "戍卫军"),
	Contract	UMETA(DisplayName = "合约团"),
	Autonomy	UMETA(DisplayName = "自治同盟"),
	Insider		UMETA(DisplayName = "内鬼"),
	Neutral		UMETA(DisplayName = "中立")
};

/** 三阶时间流速 */
UENUM(BlueprintType)
enum class ETimePhase : uint8
{
	Peace		UMETA(DisplayName = "非战时(季度)"),
	Wartime		UMETA(DisplayName = "战时(周)"),
	Battle		UMETA(DisplayName = "战役(四小时)")
};

/** 四季 */
UENUM(BlueprintType)
enum class ESeason : uint8
{
	Spring		UMETA(DisplayName = "春季"),
	Summer		UMETA(DisplayName = "夏季"),
	Autumn		UMETA(DisplayName = "秋季"),
	Winter		UMETA(DisplayName = "冬季")
};

/** 阵地类型 */
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

/** 单位大类 */
UENUM(BlueprintType)
enum class EUnitCategory : uint8
{
	Land		UMETA(DisplayName = "陆地单位"),
	Air			UMETA(DisplayName = "空中单位"),
	Naval		UMETA(DisplayName = "海上单位")
};

/** 兵种ID（对应规则全书练兵表） */
UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Militia, Infantry, Scout, Medic, Guerrilla,
	CoastalDefense, AirDefense, FrontlineOfficer,
	Commissar, Paratrooper, AirForce, Sniper,
	Artillery, Armor, Engineer, Submarine, Warship
};

/** 军衔等级 */
UENUM(BlueprintType)
enum class ERankLevel : uint8
{
	SecondLieutenant, Lieutenant, Captain,
	MajorII, MajorI, LieutenantColonelII, LieutenantColonelI,
	ColonelII, ColonelI,
	MajorGeneralIII, MajorGeneralII, MajorGeneralI,
	LieutenantGeneralIII, LieutenantGeneralII, LieutenantGeneralI,
	Commander
};

/** 行动牌费用梯度 */
UENUM(BlueprintType)
enum class ECardCost : uint8
{
	One = 1, Two = 2, Three = 3, Four = 4, Five = 5
};

/** 胜利条件类型 */
UENUM(BlueprintType)
enum class EVictoryType : uint8
{
	Hegemony,		// 战区霸权
	Military,		// 军事碾压
	Economic,		// 经济霸权
	Insider			// 内鬼独立胜利
};
