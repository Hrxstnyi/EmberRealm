// Copyright Epic Games, Inc. All Rights Reserved.

#include "StrategyLayer/StrategyMapManager.h"
#include "Core/EventBus.h"

FStrategyMapManager::FStrategyMapManager()
{
	UE_LOG(LogTemp, Log, TEXT("[StrategyMapManager] 地图管理器创建"));
}

FStrategyMapManager::~FStrategyMapManager()
{
}

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
	if (FStrategyTile* Tile = GetTile(TileID))
	{
		EFactionType OldController = Tile->Controller;
		Tile->Controller = NewController;

		UE_LOG(LogTemp, Log, TEXT("[StrategyMapManager] 阵地 %d 控制权变更: %d -> %d"),
			TileID, (int32)OldController, (int32)NewController);

		FEventBus::Get().Broadcast(EREvents::OnTerritoryChanged,
			FString::Printf(TEXT("{\"tile\":%d,\"old\":%d,\"new\":%d}"), TileID, (int32)OldController, (int32)NewController));

		return true;
	}
	return false;
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
		if (Pair.Value.Controller == Faction)
		{
			if (Pair.Value.TerrainType == ETerrainType::IronMine ||
				Pair.Value.TerrainType == ETerrainType::OilField ||
				Pair.Value.TerrainType == ETerrainType::CottonField ||
				Pair.Value.TerrainType == ETerrainType::Residential)
			{
				Count++;
			}
		}
	}
	return Count;
}

int32 FStrategyMapManager::CalculateTotalIncome(EFactionType Faction, bool bIsWarTime) const
{
	int32 TotalIncome = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.Controller == Faction)
		{
			int32 Tax = Pair.Value.TaxIncome;
			if (bIsWarTime)
			{
				Tax = FMath::RoundToInt(Tax * 0.8f);
			}
			TotalIncome += Tax;

			int32 Resource = Pair.Value.ResourceOutput;
			if (bIsWarTime)
			{
				Resource = FMath::RoundToInt(Resource * 0.7f);
			}
			TotalIncome += Resource;
		}
	}
	return TotalIncome;
}

bool FStrategyMapManager::CanMoveTo(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const
{
	const FStrategyTile* From = GetTile(FromTileID);
	const FStrategyTile* To = GetTile(ToTileID);
	if (!From || !To) return false;

	if (!From->AdjacentTiles.Contains(ToTileID)) return false;

	int32 ElevationDiff = FMath::Abs(To->Elevation - From->Elevation);
	if (ElevationDiff >= 2 && !bHasAirborne)
	{
		return false;
	}

	return true;
}

int32 FStrategyMapManager::CalculateMoveCost(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const
{
	const FStrategyTile* From = GetTile(FromTileID);
	const FStrategyTile* To = GetTile(ToTileID);
	if (!From || !To) return 999;

	int32 BaseCost = 1;
	int32 ElevationDiff = FMath::Abs(To->Elevation - From->Elevation);

	if (bHasAirborne && ElevationDiff >= 2)
	{
		return 1;
	}

	if (To->Elevation > From->Elevation)
	{
		BaseCost += ElevationDiff;
	}

	return BaseCost;
}
