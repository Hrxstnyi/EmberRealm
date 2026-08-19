// Copyright Epic Games, Inc. All Rights Reserved.

#include "StrategyLayer/StrategyMapManager.h"
#include "Core/EventBus.h"

FStrategyMapManager::FStrategyMapManager()
{
	UE_LOG(LogTemp, Log, TEXT("[StrategyMapManager] 地图管理器创建"));
}

FStrategyMapManager::~FStrategyMapManager() = default;

void FStrategyMapManager::InitializeMap(int32 InMapSizeX, int32 InMapSizeY)
{
	MapSizeX = InMapSizeX;
	MapSizeY = InMapSizeY;
	Tiles.Empty();
	bInitialized = true;
	UE_LOG(LogTemp, Log, TEXT("[StrategyMapManager] 地图初始化: %dx%d"), MapSizeX, MapSizeY);
}

bool FStrategyMapManager::LoadMapFromData(const TArray<FStrategyTile>& InTiles)
{
	Tiles.Empty();
	for (const FStrategyTile& Tile : InTiles)
	{
		Tiles.Add(Tile.TileID, Tile);
	}
	bInitialized = Tiles.Num() > 0;
	UE_LOG(LogTemp, Log, TEXT("[StrategyMapManager] 加载 %d 个阵地"), Tiles.Num());
	return bInitialized;
}

FStrategyTile* FStrategyMapManager::GetTile(int32 TileID)
{
	return Tiles.Find(TileID);
}

const FStrategyTile* FStrategyMapManager::GetTile(int32 TileID) const
{
	return Tiles.Find(TileID);
}

TArray<FStrategyTile*> FStrategyMapManager::GetTilesByFaction(EFactionType Faction)
{
	TArray<FStrategyTile*> Result;
	for (auto& Pair : Tiles)
	{
		if (Pair.Value.Controller == Faction)
		{
			Result.Add(&Pair.Value);
		}
	}
	return Result;
}

TArray<FStrategyTile*> FStrategyMapManager::GetAdjacentTiles(int32 TileID)
{
	TArray<FStrategyTile*> Result;
	if (FStrategyTile* Tile = GetTile(TileID))
	{
		for (int32 AdjID : Tile->AdjacentTiles)
		{
			if (FStrategyTile* AdjTile = GetTile(AdjID))
			{
				Result.Add(AdjTile);
			}
		}
	}
	return Result;
}

bool FStrategyMapManager::ChangeTileController(int32 TileID, EFactionType NewController)
{
	FStrategyTile* Tile = GetTile(TileID);
	if (!Tile) return false;

	const EFactionType OldController = Tile->Controller;
	Tile->Controller = NewController;

	UE_LOG(LogTemp, Log, TEXT("[StrategyMapManager] 阵地 %d 控制权变更: %d -> %d"),
		TileID, (int32)OldController, (int32)NewController);

	FEventBus::Get().Broadcast(EREvents::OnTerritoryChanged,
		FString::Printf(TEXT("{\"tile\":%d,\"old\":%d,\"new\":%d}"), TileID, (int32)OldController, (int32)NewController));

	return true;
}

int32 FStrategyMapManager::CountTransportHubs(EFactionType Faction) const
{
	int32 Count = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.Controller == Faction && Pair.Value.TerrainType == ETerrainType::TransportHub)
		{
			Count++;
		}
	}
	return Count;
}

int32 FStrategyMapManager::CountResourceTiles(EFactionType Faction) const
{
	int32 Count = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.Controller == Faction && IsResourceTile(Pair.Value.TerrainType))
		{
			Count++;
		}
	}
	return Count;
}

int32 FStrategyMapManager::CalculateTotalIncome(EFactionType Faction, bool bIsWarTime) const
{
	int32 TotalIncome = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.Controller != Faction) continue;

		TotalIncome += ApplyWarTimeDiscount(Pair.Value.TaxIncome, 0.8f, bIsWarTime);
		TotalIncome += ApplyWarTimeDiscount(Pair.Value.ResourceOutput, 0.7f, bIsWarTime);
	}
	return TotalIncome;
}

bool FStrategyMapManager::CanMoveTo(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const
{
	const FStrategyTile* From = GetTile(FromTileID);
	const FStrategyTile* To = GetTile(ToTileID);
	if (!From || !To) return false;
	if (!From->AdjacentTiles.Contains(ToTileID)) return false;

	return bHasAirborne || GetElevationDiff(FromTileID, ToTileID) < 2;
}

int32 FStrategyMapManager::CalculateMoveCost(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const
{
	const FStrategyTile* From = GetTile(FromTileID);
	const FStrategyTile* To = GetTile(ToTileID);
	if (!From || !To) return 999;

	const int32 ElevationDiff = GetElevationDiff(FromTileID, ToTileID);
	if (bHasAirborne && ElevationDiff >= 2) return 1;

	return To->Elevation > From->Elevation ? 1 + ElevationDiff : 1;
}

// ============================================================
// 私有辅助函数
// ============================================================

bool FStrategyMapManager::IsResourceTile(ETerrainType Type)
{
	return Type == ETerrainType::IronMine
		|| Type == ETerrainType::OilField
		|| Type == ETerrainType::CottonField
		|| Type == ETerrainType::Residential;
}

int32 FStrategyMapManager::ApplyWarTimeDiscount(int32 Value, float Ratio, bool bIsWarTime)
{
	return bIsWarTime ? FMath::RoundToInt(Value * Ratio) : Value;
}

int32 FStrategyMapManager::GetElevationDiff(int32 FromTileID, int32 ToTileID) const
{
	const FStrategyTile* From = GetTile(FromTileID);
	const FStrategyTile* To = GetTile(ToTileID);
	if (!From || !To) return 0;
	return FMath::Abs(To->Elevation - From->Elevation);
}
