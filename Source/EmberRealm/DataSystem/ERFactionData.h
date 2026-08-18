// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EFactionType : uint8
{
	GarrisonArmy	UMETA(DisplayName = "戍卫军（正规军）"),
	ContractCorps	UMETA(DisplayName = "合约团（雇佣武装）"),
	AutonomyLeague	UMETA(DisplayName = "自治同盟（地方武装）"),
	Neutral			UMETA(DisplayName = "中立"),
	Saboteur		UMETA(DisplayName = "内鬼（隐藏）")
};

UCLASS(BlueprintType)
class EMBERREALM_API UERFactionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	FText FactionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	EFactionType FactionType = EFactionType::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "经济")
	int32 InitialCurrency = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "名誉")
	int32 InitialReputation = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "特色")
	TMap<FName, float> FactionBonuses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "视觉")
	FLinearColor PrimaryColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "视觉")
	FLinearColor SecondaryColor = FLinearColor::Gray;
};
