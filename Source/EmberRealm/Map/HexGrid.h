// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/EmberStructs.h"
#include "HexGrid.generated.h"

/**
 * 六边形战区网格系统
 * 支持轴向坐标(q, r)，海拔机制，阵地类型
 */
UCLASS(BlueprintType)
class EMBERREALM_API UHexGrid : public UObject
{
	GENERATED_BODY()

public:
	UHexGrid();

	/** 初始化网格 */
	void InitializeGrid(int32 InWidth, int32 InHeight);

	/** 获取指定坐标的阵地数据 */
	FTileData* GetTile(FIntPoint Coord);

	/** 设置阵地控制者 */
	void SetTileController(FIntPoint Coord, EFactionType Faction);

	/** 获取两格之间的六边形距离 */
	UFUNCTION(BlueprintPure, Category = "EmberRealm|Map")
	int32 GetHexDistance(FIntPoint A, FIntPoint B) const;

	/** 获取相邻格子（6方向） */
	TArray<FIntPoint> GetNeighbors(FIntPoint Coord) const;

	/** 检查单位是否可以从From移动到To（考虑海拔） */
	bool CanMoveTo(FIntPoint From, FIntPoint To, EUnitCategory Category, int32 MoveRange) const;

	/** 获取海拔攻击射程修正（规则3.2） */
	int32 GetElevationRangeModifier(FIntPoint Attacker, FIntPoint Defender) const;

	/** 统计 */
	int32 GetHubCount() const;
	int32 GetControlledHubCount(EFactionType Faction) const;
	int32 GetResourceTileCount() const;
	int32 GetControlledResourceCount(EFactionType Faction) const;

	/** 获取所有阵地 */
	const TMap<FIntPoint, FTileData>& GetAllTiles() const { return Tiles; }

	UFUNCTION(BlueprintPure, Category = "EmberRealm|Map")
	int32 GetWidth() const { return Width; }

	UFUNCTION(BlueprintPure, Category = "EmberRealm|Map")
	int32 GetHeight() const { return Height; }

private:
	int32 Width = 12;
	int32 Height = 10;
	TMap<FIntPoint, FTileData> Tiles;

	/** 轴向坐标转立方坐标用于距离计算 */
	void ToCube(FIntPoint Hex, int32& OutX, int32& OutY, int32& OutZ) const;
};
