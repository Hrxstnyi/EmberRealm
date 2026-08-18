// Copyright EmberRealm. All Rights Reserved.
#include "Economy/EconomyManager.h"
#include "Factions/FactionManager.h"
#include "Map/HexGrid.h"
#include "Core/TimeSystem.h"

UEconomyManager::UEconomyManager()
{
}

void UEconomyManager::SettleGlobalResources(UFactionManager* FactionMgr, UHexGrid* Grid, UTimeSystem* TimeSys)
{
	if (!FactionMgr || !Grid || !TimeSys) return;

	ETimePhase Phase = TimeSys->GetPhase();
	TArray<EFactionType> AllFactions = { EFactionType::Garrison, EFactionType::Contract, EFactionType::Autonomy };

	for (EFactionType Faction : AllFactions)
	{
		FFactionState State = FactionMgr->GetFactionState(Faction);
		if (State.bIsUnderSanction) continue; // 禁闭期间无资源变动

		int32 Income = CalculateIncome(Faction, FactionMgr, Grid, Phase);
		int32 Upkeep = CalculateUpkeep(Faction, FactionMgr);
		int32 Net = Income - Upkeep;

		// 直接修改阵营状态（通过公共接口）
		// 注意：这里简化处理，实际应通过FactionManager的方法
		State.KayaCoins += Net;
		FactionMgr->SpendActionPoints(Faction, 0); // 触发状态同步占位

		UE_LOG(LogTemp, Log, TEXT("[Economy] 阵营 %d 结算：收入+%d 维护-%d 净%+d 余额=%d"),
			(int32)Faction, Income, Upkeep, Net, State.KayaCoins);
	}
}

int32 UEconomyManager::CalculateIncome(EFactionType Faction, UFactionManager* FactionMgr, UHexGrid* Grid, ETimePhase Phase) const
{
	if (!Grid) return 0;
	int32 Income = 0;
	bool bPeace = (Phase == ETimePhase::Peace);

	for (const auto& Pair : Grid->GetAllTiles())
	{
		const FTileData& Tile = Pair.Value;
		if (Tile.Controller != Faction) continue;

		switch (Tile.TileType)
		{
		case ETileType::Residential:
			Income += bPeace ? ResidentialTaxPeace : ResidentialTaxWartime;
			break;
		case ETileType::IronMine:
			Income += IronMineOutput;
			break;
		case ETileType::OilField:
			Income += OilFieldOutput;
			break;
		case ETileType::CottonField:
			Income += bPeace ? CottonOutputPeace : CottonOutputWartime;
			break;
		default:
			break;
		}
	}
	return Income;
}

int32 UEconomyManager::CalculateUpkeep(EFactionType Faction, UFactionManager* FactionMgr) const
{
	if (!FactionMgr) return 0;
	FFactionState State = FactionMgr->GetFactionState(Faction);
	int32 Upkeep = 0;

	// 每个单位基础维护费（油耗+弹药）
	for (const FUnitInstance& Unit : State.Units)
	{
		Upkeep += 2; // 简化：每单位每回合2卡亚币维护
	}
	return Upkeep;
}

bool UEconomyManager::StartMining(EFactionType Faction, FIntPoint TileCoord, UFactionManager* FactionMgr, UHexGrid* Grid)
{
	if (!FactionMgr || !Grid) return false;
	FTileData* Tile = Grid->GetTile(TileCoord);
	if (!Tile || Tile->Controller != Faction) return false;

	int32 StartupCost = 0;
	switch (Tile->TileType)
	{
	case ETileType::IronMine:	StartupCost = IronMineStartup; break;
	case ETileType::OilField:	StartupCost = OilFieldStartup; break;
	case ETileType::CottonField:StartupCost = CottonStartup; break;
	default: return false;
	}

	// 检查并扣除费用（简化）
	FFactionState State = FactionMgr->GetFactionState(Faction);
	if (State.KayaCoins >= StartupCost)
	{
		UE_LOG(LogTemp, Log, TEXT("[Economy] 阵营 %d 启动开采 (%d,%d)，花费 %d"),
			(int32)Faction, TileCoord.X, TileCoord.Y, StartupCost);
		return true;
	}
	return false;
}

void UEconomyManager::Trade(EFactionType From, EFactionType To, int32 Amount, UFactionManager* FactionMgr)
{
	UE_LOG(LogTemp, Log, TEXT("[Economy] 阵营 %d 向阵营 %d 贸易 %d 卡亚币"), (int32)From, (int32)To, Amount);
}
