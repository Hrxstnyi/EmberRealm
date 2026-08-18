// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 阵营类型枚举
 */
UENUM(BlueprintType)
enum class EFactionType : uint8
{
	GarrisonArmy	UMETA(DisplayName = "戍卫军（正规军）"),
	ContractCorps	UMETA(DisplayName = "合约团（雇佣武装）"),
	AutonomyLeague	UMETA(DisplayName = "自治同盟（地方武装）"),
	Neutral			UMETA(DisplayName = "中立"),
	Saboteur		UMETA(DisplayName = "内鬼（隐藏）")
};

/**
 * 阵营数据资产
 * 存储阵营的基础属性、特色、初始资源
 */
UCLASS(BlueprintType)
class EMBERREALM_API UERFactionData : public UDataAsset
{
	GENERATED_BODY()

public:
	/** 阵营名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	FText FactionName;

	/** 阵营类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	EFactionType FactionType = EFactionType::Neutral;

	/** 阵营描述 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础", meta = (MultiLine = true))
	FText Description;

	/** 初始卡亚币 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "经济")
	int32 InitialCurrency = 500;

	/** 初始名誉值 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "名誉")
	int32 InitialReputation = 10;

	/** 阵营特色增益（数据驱动） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "特色")
	TMap<FName, float> FactionBonuses;

	/** 阵营主题色 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "视觉")
	FLinearColor PrimaryColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "视觉")
	FLinearColor SecondaryColor = FLinearColor::Gray;
};
