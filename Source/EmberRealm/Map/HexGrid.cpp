// Copyright EmberRealm. All Rights Reserved.
#include "Map/HexGrid.h"

UHexGrid::UHexGrid()
{
}

void UHexGrid::InitializeGrid(int32 InWidth, int32 InHeight)
{
	Width = InWidth;
	Height = InHeight;
	Tiles.Empty();

	// 预设阵地类型分布（模拟卡拉比亚战区）
	ETileType TileLayout[10][12] = {
		{ETileType::Plain, ETileType::Plain, ETileType::Residential, ETileType::Plain, ETileType::IronMine, ETileType::Mountain, ETileType::Mountain, ETileType::Plain, ETileType::OilField, ETileType::Plain, ETileType::Hub, ETileType::Plain},
		{ETileType::Plain, ETileType::Residential, ETileType::Hub, ETileType::Plain, ETileType::Plain, ETileType::Mountain, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::CottonField, ETileType::Plain, ETileType::Plain},
		{ETileType::CottonField, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::IronMine, ETileType::Plain, ETileType::Plain, ETileType::Residential, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::OilField},
		{ETileType::Plain, ETileType::Plain, ETileType::Mountain, ETileType::Plain, ETileType::Plain, ETileType::Hub, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::IronMine, ETileType::Plain, ETileType::Plain},
		{ETileType::Plain, ETileType::OilField, ETileType::Mountain, ETileType::Mountain, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::CottonField, ETileType::Plain, ETileType::Plain, ETileType::Residential, ETileType::Plain},
		{ETileType::Residential, ETileType::Plain, ETileType::Plain, ETileType::Mountain, ETileType::Plain, ETileType::Plain, ETileType::Hub, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::CottonField},
		{ETileType::Plain, ETileType::Plain, ETileType::IronMine, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::OilField, ETileType::Plain, ETileType::Mountain, ETileType::Plain},
		{ETileType::Plain, ETileType::CottonField, ETileType::Plain, ETileType::Plain, ETileType::Residential, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::Hub, ETileType::Mountain, ETileType::Plain},
		{ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::OilField, ETileType::Plain, ETileType::IronMine, ETileType::Plain, ETileType::Residential, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::Plain},
		{ETileType::Hub, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::CottonField, ETileType::Plain, ETileType::Plain, ETileType::Plain, ETileType::IronMine, ETileType::Plain, ETileType::Plain, ETileType::Residential}
	};

	for (int32 r = 0; r < Height; r++)
	{
		for (int32 q = 0; q < Width; q++)
		{
			FTileData Tile;
			Tile.Coord = FIntPoint(q, r);
			Tile.TileType = TileLayout[r][q];
			Tile.Elevation = (Tile.TileType == ETileType::Mountain) ? FMath::RandRange(3, 5) : FMath::RandRange(1, 2);
			Tile.Controller = EFactionType::Neutral;
			Tile.bIsHub = (Tile.TileType == ETileType::Hub);
			Tile.bHasBase = false;

			// 资源产出
			switch (Tile.TileType)
			{
			case ETileType::Residential:	Tile.ResourceOutput = 35; break;
			case ETileType::IronMine:		Tile.ResourceOutput = 120; break;
			case ETileType::OilField:		Tile.ResourceOutput = 150; break;
			case ETileType::CottonField:	Tile.ResourceOutput = 90; break;
			default: Tile.ResourceOutput = 0;
			}

			Tiles.Add(FIntPoint(q, r), Tile);
		}
	}

	// 设置三方初始根据地
	SetTileController(FIntPoint(0, 0), EFactionType::Garrison);
	SetTileController(FIntPoint(11, 0), EFactionType::Contract);
	SetTileController(FIntPoint(5, 9), EFactionType::Autonomy);

	UE_LOG(LogTemp, Log, TEXT("[HexGrid] 战区地图初始化完成：%dx%d，共%d个阵地"),
		Width, Height, Tiles.Num());
}

FTileData* UHexGrid::GetTile(FIntPoint Coord)
{
	return Tiles.Find(Coord);
}

void UHexGrid::SetTileController(FIntPoint Coord, EFactionType Faction)
{
	if (FTileData* Tile = Tiles.Find(Coord))
	{
		Tile->Controller = Faction;
	}
}

int32 UHexGrid::GetHexDistance(FIntPoint A, FIntPoint B) const
{
	int32 ax, ay, az, bx, by, bz;
	ToCube(A, ax, ay, az);
	ToCube(B, bx, by, bz);
	return (FMath::Abs(ax - bx) + FMath::Abs(ay - by) + FMath::Abs(az - bz)) / 2;
}

TArray<FIntPoint> UHexGrid::GetNeighbors(FIntPoint Coord) const
{
	// 轴向坐标六方向（偶数行偏移）
	static const int32 Directions[2][6][2] = {
		{{+1, 0}, {+1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {0, +1}},  // 偶数行
		{{+1, 0}, {+1, +1}, {0, +1}, {-1, +1}, {-1, 0}, {0, -1}}   // 奇数行
	};

	TArray<FIntPoint> Neighbors;
	int32 parity = Coord.Y & 1;
	for (int32 i = 0; i < 6; i++)
	{
		FIntPoint N(Coord.X + Directions[parity][i][0], Coord.Y + Directions[parity][i][1]);
		if (N.X >= 0 && N.X < Width && N.Y >= 0 && N.Y < Height)
		{
			Neighbors.Add(N);
		}
	}
	return Neighbors;
}

bool UHexGrid::CanMoveTo(FIntPoint From, FIntPoint To, EUnitCategory Category, int32 MoveRange) const
{
	int32 Dist = GetHexDistance(From, To);
	if (Dist > MoveRange) return false;

	const FTileData* FromTile = Tiles.Find(From);
	const FTileData* ToTile = Tiles.Find(To);
	if (!FromTile || !ToTile) return false;

	// 规则3.2：陆地单位无法直接进入高2阶及以上阵地
	if (Category == EUnitCategory::Land && ToTile->Elevation >= 3)
	{
		return false; // 需迂回/空降
	}

	// 海拔差1阶：行军消耗翻倍（简化为移动力-1）
	if (Category == EUnitCategory::Land && FMath::Abs(ToTile->Elevation - FromTile->Elevation) >= 1)
	{
		if (Dist >= MoveRange) return false; // 翻倍消耗后不够
	}

	return true;
}

int32 UHexGrid::GetElevationRangeModifier(FIntPoint Attacker, FIntPoint Defender) const
{
	const FTileData* A = Tiles.Find(Attacker);
	const FTileData* D = Tiles.Find(Defender);
	if (!A || !D) return 0;
	// 规则3.2：高低海拔交战，攻击射程差值 = 海拔阶数差
	return A->Elevation - D->Elevation;
}

int32 UHexGrid::GetHubCount() const
{
	int32 Count = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.bIsHub) Count++;
	}
	return Count;
}

int32 UHexGrid::GetControlledHubCount(EFactionType Faction) const
{
	int32 Count = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.bIsHub && Pair.Value.Controller == Faction) Count++;
	}
	return Count;
}

int32 UHexGrid::GetResourceTileCount() const
{
	int32 Count = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.TileType == ETileType::Residential ||
			Pair.Value.TileType == ETileType::IronMine ||
			Pair.Value.TileType == ETileType::OilField ||
			Pair.Value.TileType == ETileType::CottonField)
			Count++;
	}
	return Count;
}

int32 UHexGrid::GetControlledResourceCount(EFactionType Faction) const
{
	int32 Count = 0;
	for (const auto& Pair : Tiles)
	{
		if (Pair.Value.Controller == Faction &&
			(Pair.Value.TileType == ETileType::Residential ||
			 Pair.Value.TileType == ETileType::IronMine ||
			 Pair.Value.TileType == ETileType::OilField ||
			 Pair.Value.TileType == ETileType::CottonField))
			Count++;
	}
	return Count;
}

void UHexGrid::ToCube(FIntPoint Hex, int32& OutX, int32& OutY, int32& OutZ) const
{
	OutX = Hex.X - (Hex.Y - (Hex.Y & 1)) / 2;
	OutZ = Hex.Y;
	OutY = -OutX - OutZ;
}
