// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StrategyLayer/StrategyTile.h"

class EMBERREALM_API FStrategyMapManager
{
public:
	FStrategyMapManager();
	~FStrategyMapManager();

	void InitializeMap(int32 MapSizeX, int32 MapSizeY);
	bool LoadMapFromData(const TArray<FStrategyTile>& Tiles);

	FStrategyTile* GetTile(int32 TileID);
	const FStrategyTile* GetTile(int32 TileID) const;
	const TMap<int32, FStrategyTile>& GetAllTiles() const { return Tiles; }

	TArray<FStrategyTile*> GetTilesByFaction(EFactionType Faction);
	TArray<FStrategyTile*> GetAdjacentTiles(int32 TileID);

	bool ChangeTileController(int32 TileID, EFactionType NewController);

	int32 CountTransportHubs(EFactionType Faction) const;
	int32 CountResourceTiles(EFactionType Faction) const;
	int32 CalculateTotalIncome(EFactionType Faction, bool bIsWarTime) const;

	bool CanMoveTo(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const;
	int32 CalculateMoveCost(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const;

private:
	TMap<int32, FStrategyTile> Tiles;
	int32 MapSizeX = 0;
	int32 MapSizeY = 0;
	bool bInitialized = false;
};
