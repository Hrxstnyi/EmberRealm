// Copyright Epic Games, Inc. All Rights Reserved.

#include "StrategyLayer/EconomySystem.h"
#include "Core/EventBus.h"

FEconomySystem::FEconomySystem()
{
}

FEconomySystem::~FEconomySystem() = default;

void FEconomySystem::Initialize(EFactionType InFaction, const FResourcePack& InitialResources)
{
	Faction = InFaction;
	Resources = InitialResources;
	BuildQueue.Empty();
	LastTurnIncome = FResourcePack();
	LastTurnExpense = FResourcePack();

	UE_LOG(LogTemp, Log, TEXT("[EconomySystem] 阵营 %d 经济系统初始化，初始卡亚币: %d"),
		(int32)Faction, Resources.Currency);
}

void FEconomySystem::ProcessTurn(int32 TurnNumber, bool bIsWarTime)
{
	FResourcePack TurnIncome = BaseIncome;
	if (bIsWarTime)
	{
		TurnIncome.Currency = FMath::RoundToInt(TurnIncome.Currency * 0.8f);
		TurnIncome.Steel = FMath::RoundToInt(TurnIncome.Steel * 0.7f);
		TurnIncome.Oil = FMath::RoundToInt(TurnIncome.Oil * 0.7f);
		TurnIncome.Cotton = FMath::RoundToInt(TurnIncome.Cotton * 0.7f);
	}

	FResourcePack TurnExpense = UpkeepCost;
	Resources = Resources + TurnIncome - TurnExpense;
	ProcessBuildQueue();

	LastTurnIncome = TurnIncome;
	LastTurnExpense = TurnExpense;

	UE_LOG(LogTemp, Log, TEXT("[EconomySystem] 回合 %d 结算: 收入 %d, 支出 %d, 余额 %d"),
		TurnNumber, TurnIncome.Currency, TurnExpense.Currency, Resources.Currency);

	BroadcastResourceChanged();
}

void FEconomySystem::AddResources(const FResourcePack& Amount)
{
	Resources = Resources + Amount;
	BroadcastResourceChanged();
}

bool FEconomySystem::SpendResources(const FResourcePack& Amount)
{
	if (!CanAfford(Amount))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EconomySystem] 资源不足，无法支付"));
		return false;
	}
	Resources.Deduct(Amount);
	BroadcastResourceChanged();
	return true;
}

bool FEconomySystem::CanAfford(const FResourcePack& Cost) const
{
	return Resources.CanAfford(Cost);
}

bool FEconomySystem::AddBuildItem(const FBuildQueueItem& Item)
{
	if (!CanAfford(Item.Cost))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EconomySystem] 资源不足，无法加入建造队列: %s"),
			*Item.DisplayName.ToString());
		return false;
	}

	Resources.Deduct(Item.Cost);
	BuildQueue.Add(Item);

	UE_LOG(LogTemp, Log, TEXT("[EconomySystem] 加入建造队列: %s (%d回合)"),
		*Item.DisplayName.ToString(), Item.TotalTurns);

	BroadcastResourceChanged();
	return true;
}

bool FEconomySystem::CancelBuildItem(int32 Index)
{
	if (!BuildQueue.IsValidIndex(Index)) return false;

	const FBuildQueueItem& Item = BuildQueue[Index];
	FResourcePack Refund;
	Refund.Currency = FMath::RoundToInt(Item.Cost.Currency * 0.5f);
	Refund.Steel = FMath::RoundToInt(Item.Cost.Steel * 0.5f);
	Refund.Oil = FMath::RoundToInt(Item.Cost.Oil * 0.5f);
	Refund.Cotton = FMath::RoundToInt(Item.Cost.Cotton * 0.5f);
	Refund.Ammo = FMath::RoundToInt(Item.Cost.Ammo * 0.5f);
	Refund.Manpower = FMath::RoundToInt(Item.Cost.Manpower * 0.5f);

	Resources = Resources + Refund;
	BuildQueue.RemoveAt(Index);

	UE_LOG(LogTemp, Log, TEXT("[EconomySystem] 取消建造项，返还50%%资源"));
	BroadcastResourceChanged();
	return true;
}

void FEconomySystem::ProcessBuildQueue()
{
	TArray<int32> CompletedIndices;

	for (int32 i = 0; i < BuildQueue.Num(); i++)
	{
		BuildQueue[i].TurnsCompleted++;

		if (BuildQueue[i].IsComplete())
		{
			CompletedIndices.Add(i);
			UE_LOG(LogTemp, Log, TEXT("[EconomySystem] 建造完成: %s"),
				*BuildQueue[i].DisplayName.ToString());

			FEventBus::Get().Broadcast(EREvents::OnUnitMoved,
				FString::Printf(TEXT("{\"build_complete\":\"%s\",\"tile\":%d}"),
					*BuildQueue[i].ItemID.ToString(), BuildQueue[i].TargetTileID));
		}
	}

	for (int32 i = CompletedIndices.Num() - 1; i >= 0; i--)
	{
		BuildQueue.RemoveAt(CompletedIndices[i]);
	}
}

void FEconomySystem::BroadcastResourceChanged() const
{
	FEventBus::Get().Broadcast(EREvents::OnResourceChanged,
		FString::Printf(TEXT("{\"faction\":%d,\"currency\":%d,\"steel\":%d,\"oil\":%d}"),
			(int32)Faction, Resources.Currency, Resources.Steel, Resources.Oil));
}
