// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "StrategyLayer/StrategyTile.h"

/**
 * 战略地图管理器
 * 管理所有阵地、连接关系、阵营控制
 */
class EMBERREALM_API FStrategyMapManager
{
public:
	FStrategyMapManager();
	~FStrategyMapManager();

	/** 初始化地图 */
	void InitializeMap(int32 MapSizeX, int32 MapSizeY);

	/** 加载地图配置 */
	bool LoadMapFromData(const TArray<FStrategyTile>& Tiles);

	/** 获取阵地 */
	FStrategyTile* GetTile(int32 TileID);
	const FStrategyTile* GetTile(int32 TileID) const;

	/** 获取所有阵地 */
	const TMap<int32, FStrategyTile>& GetAllTiles() const { return Tiles; }

	/** 获取某阵营控制的所有阵地 */
	TArray<FStrategyTile*> GetTilesByFaction(EFactionType Faction);

	/** 获取相邻阵地 */
	TArray<FStrategyTile*> GetAdjacentTiles(int32 TileID);

	/** 变更阵地控制 */
	bool ChangeTileController(int32 TileID, EFactionType NewController);

	/** 获取交通枢纽数量 */
	int32 CountTransportHubs(EFactionType Faction) const;

	/** 获取资源阵地数量 */
	int32 CountResourceTiles(EFactionType Faction) const;

	/** 计算某阵营的总收入 */
	int32 CalculateTotalIncome(EFactionType Faction, bool bIsWarTime) const;

	/** 检查阵地是否可通行（考虑海拔） */
	bool CanMoveTo(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const;

	/** 计算行军消耗（回合数） */
	int32 CalculateMoveCost(int32 FromTileID, int32 ToTileID, bool bHasAirborne) const;

private:
	/** 判断是否为资源产出阵地 */
	static bool IsResourceTile(ETerrainType Type);

	/** 应用战时折扣 */
	static int32 ApplyWarTimeDiscount(int32 Value, float Ratio, bool bIsWarTime);

	/** 获取两阵地海拔差 */
	int32 GetElevationDiff(int32 FromTileID, int32 ToTileID) const;

	/** 所有阵地 */
	TMap<int32, FStrategyTile> Tiles;

	/** 地图尺寸 */
	int32 MapSizeX = 0;
	int32 MapSizeY = 0;

	/** 是否已初始化 */
	bool bInitialized = false;
};
