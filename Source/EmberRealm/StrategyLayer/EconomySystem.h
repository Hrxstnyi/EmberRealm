// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "DataSystem/ERFactionData.h"
#include "EconomySystem.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Currency	UMETA(DisplayName = "卡亚币"),
	Steel		UMETA(DisplayName = "钢铁"),
	Oil			UMETA(DisplayName = "石油"),
	Cotton		UMETA(DisplayName = "棉花"),
	Ammo		UMETA(DisplayName = "弹药"),
	Manpower	UMETA(DisplayName = "人力")
};

USTRUCT(BlueprintType)
struct FResourcePack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资源")
	int32 Currency = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资源")
	int32 Steel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资源")
	int32 Oil = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资源")
	int32 Cotton = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资源")
	int32 Ammo = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "资源")
	int32 Manpower = 0;

	FResourcePack operator+(const FResourcePack& Other) const
	{
		FResourcePack Result;
		Result.Currency = Currency + Other.Currency;
		Result.Steel = Steel + Other.Steel;
		Result.Oil = Oil + Other.Oil;
		Result.Cotton = Cotton + Other.Cotton;
		Result.Ammo = Ammo + Other.Ammo;
		Result.Manpower = Manpower + Other.Manpower;
		return Result;
	}

	FResourcePack operator-(const FResourcePack& Other) const
	{
		FResourcePack Result;
		Result.Currency = Currency - Other.Currency;
		Result.Steel = Steel - Other.Steel;
		Result.Oil = Oil - Other.Oil;
		Result.Cotton = Cotton - Other.Cotton;
		Result.Ammo = Ammo - Other.Ammo;
		Result.Manpower = Manpower - Other.Manpower;
		return Result;
	}

	bool CanAfford(const FResourcePack& Cost) const
	{
		return Currency >= Cost.Currency && Steel >= Cost.Steel && Oil >= Cost.Oil
			&& Cotton >= Cost.Cotton && Ammo >= Cost.Ammo && Manpower >= Cost.Manpower;
	}

	void Deduct(const FResourcePack& Cost)
	{
		Currency -= Cost.Currency;
		Steel -= Cost.Steel;
		Oil -= Cost.Oil;
		Cotton -= Cost.Cotton;
		Ammo -= Cost.Ammo;
		Manpower -= Cost.Manpower;
	}

	int32 GetByType(EResourceType Type) const
	{
		switch (Type)
		{
		case EResourceType::Currency: return Currency;
		case EResourceType::Steel: return Steel;
		case EResourceType::Oil: return Oil;
		case EResourceType::Cotton: return Cotton;
		case EResourceType::Ammo: return Ammo;
		case EResourceType::Manpower: return Manpower;
		default: return 0;
		}
	}
};

UENUM(BlueprintType)
enum class EBuildItemType : uint8
{
	TrainUnit	UMETA(DisplayName = "训练单位"),
	BuildBuilding UMETA(DisplayName = "建造建筑"),
	Research	UMETA(DisplayName = "研发科技")
};

USTRUCT(BlueprintType)
struct FBuildQueueItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	EBuildItemType ItemType = EBuildItemType::TrainUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	int32 TotalTurns = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	int32 TurnsCompleted = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	FResourcePack Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "建造")
	int32 TargetTileID = -1;

	bool IsComplete() const { return TurnsCompleted >= TotalTurns; }
	float GetProgress() const { return TotalTurns > 0 ? (float)TurnsCompleted / TotalTurns : 0.f; }
};

class EMBERREALM_API FEconomySystem
{
public:
	FEconomySystem();
	~FEconomySystem();

	void Initialize(EFactionType InFaction, const FResourcePack& InitialResources);
	void ProcessTurn(int32 TurnNumber, bool bIsWarTime);
	void AddResources(const FResourcePack& Amount);
	bool SpendResources(const FResourcePack& Amount);
	bool CanAfford(const FResourcePack& Cost) const;
	bool AddBuildItem(const FBuildQueueItem& Item);
	bool CancelBuildItem(int32 Index);

	const FResourcePack& GetResources() const { return Resources; }
	const TArray<FBuildQueueItem>& GetBuildQueue() const { return BuildQueue; }
	const FResourcePack& GetLastTurnIncome() const { return LastTurnIncome; }
	const FResourcePack& GetLastTurnExpense() const { return LastTurnExpense; }
	void SetBaseIncome(const FResourcePack& Income) { BaseIncome = Income; }
	void SetUpkeepCost(const FResourcePack& Upkeep) { UpkeepCost = Upkeep; }
	EFactionType GetFaction() const { return Faction; }

private:
	EFactionType Faction = EFactionType::Neutral;
	FResourcePack Resources;
	FResourcePack BaseIncome;
	FResourcePack UpkeepCost;
	FResourcePack LastTurnIncome;
	FResourcePack LastTurnExpense;
	TArray<FBuildQueueItem> BuildQueue;

	void ProcessBuildQueue();
	void BroadcastResourceChanged() const;
};
