// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataSystem/ERFactionData.h"
#include "ERUnitData.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	Infantry	UMETA(DisplayName = "步兵"),
	Recon		UMETA(DisplayName = "侦察兵"),
	Medic		UMETA(DisplayName = "医疗兵"),
	Guerrilla	UMETA(DisplayName = "游击兵"),
	Coastal		UMETA(DisplayName = "岸防兵"),
	AirDefense	UMETA(DisplayName = "防空兵"),
	Officer		UMETA(DisplayName = "前线军官"),
	Commissar	UMETA(DisplayName = "督战兵"),
	Paratrooper	UMETA(DisplayName = "空降兵"),
	Pilot		UMETA(DisplayName = "航空兵"),
	Sniper		UMETA(DisplayName = "狙击兵"),
	Artillery	UMETA(DisplayName = "阵地炮兵"),
	Armor		UMETA(DisplayName = "装甲兵"),
	Engineer	UMETA(DisplayName = "工程兵"),
	Submarine	UMETA(DisplayName = "潜艇"),
	Warship		UMETA(DisplayName = "舰艇"),
	Militia		UMETA(DisplayName = "民兵")
};

UENUM(BlueprintType)
enum class EUnitDomain : uint8
{
	Land	UMETA(DisplayName = "陆地"),
	Sea		UMETA(DisplayName = "海上"),
	Air		UMETA(DisplayName = "空中")
};

UENUM(BlueprintType)
enum class ERankLevel : uint8
{
	SecondLieutenant	UMETA(DisplayName = "少尉"),
	FirstLieutenant		UMETA(DisplayName = "中尉"),
	Captain				UMETA(DisplayName = "上尉"),
	MajorII				UMETA(DisplayName = "少校II"),
	MajorI				UMETA(DisplayName = "少校I"),
	LieutenantColonelII	UMETA(DisplayName = "中校II"),
	LieutenantColonelI	UMETA(DisplayName = "中校I"),
	ColonelII			UMETA(DisplayName = "上校II"),
	ColonelI			UMETA(DisplayName = "上校I"),
	MajorGeneralIII		UMETA(DisplayName = "少将III"),
	MajorGeneralII		UMETA(DisplayName = "少将II"),
	MajorGeneralI		UMETA(DisplayName = "少将I"),
	LieutenantGeneralIII UMETA(DisplayName = "中将III"),
	LieutenantGeneralII	UMETA(DisplayName = "中将II"),
	LieutenantGeneralI	UMETA(DisplayName = "中将I"),
	General				UMETA(DisplayName = "上将"),
	CommanderInChief	UMETA(DisplayName = "统帅")
};

UCLASS(BlueprintType)
class EMBERREALM_API UERUnitData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	FText UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	EUnitType UnitType = EUnitType::Infantry;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础")
	EUnitDomain Domain = EUnitDomain::Land;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "基础", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "训练")
	int32 TrainCost = 40;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "训练")
	int32 TrainTimeWeeks = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "训练", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float UpkeepRatio = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "战斗")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "战斗")
	float AttackDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "战斗")
	int32 AttackRange = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "战斗")
	float Defense = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "移动")
	int32 Movement = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "补给")
	int32 MaxAmmo = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "补给")
	int32 FuelConsumption = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "战斗")
	int32 Initiative = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "侦察")
	int32 ReconRange = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "侦察", meta = (ClampMin = "0", ClampMax = "100"))
	int32 Stealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "解锁")
	ERankLevel RequiredRank = ERankLevel::SecondLieutenant;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "解锁")
	EUnitType RequiredUnit = EUnitType::Militia;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "解锁")
	FName RequiredTech = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "勋级")
	FText Skill1_Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "勋级")
	FText Skill2_Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "勋级")
	FText Skill3_Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "勋级", meta = (MultiLine = true))
	FText PassiveSkill;
};
