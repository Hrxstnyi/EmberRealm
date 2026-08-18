// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ERSaveGame.generated.h"

/**
 * 游戏存档数据
 * 支持多存档位、自动存档
 */
UCLASS()
class EMBERREALM_API UERSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** 存档名称 */
	UPROPERTY(VisibleAnywhere, Category = "存档")
	FString SaveName;

	/** 存档时间戳 */
	UPROPERTY(VisibleAnywhere, Category = "存档")
	FDateTime SaveTimestamp;

	/** 游戏内天数 */
	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	int32 TotalDaysElapsed = 0;

	/** 当前回合数 */
	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	int32 CurrentTurn = 1;

	/** 玩家阵营 */
	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	int32 PlayerFactionIndex = 0;

	/** 地图名称 */
	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	FString MapName;

	/** 所有阵地数据（序列化） */
	UPROPERTY(VisibleAnywhere, Category = "战略层")
	TArray<uint8> SerializedTiles;

	/** 所有阵营数据（序列化） */
	UPROPERTY(VisibleAnywhere, Category = "战略层")
	TArray<uint8> SerializedFactions;

	/** 所有部队数据（序列化） */
	UPROPERTY(VisibleAnywhere, Category = "战略层")
	TArray<uint8> SerializedUnits;

	/** 玩家卡亚币 */
	UPROPERTY(VisibleAnywhere, Category = "经济")
	int32 PlayerCurrency = 500;

	/** 玩家名誉值 */
	UPROPERTY(VisibleAnywhere, Category = "名誉")
	int32 PlayerReputation = 10;

	/** 游戏版本号（用于存档兼容性检查） */
	UPROPERTY(VisibleAnywhere, Category = "版本")
	FString GameVersion = "0.1.0";
};
