// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ERSaveGame.generated.h"

UCLASS()
class EMBERREALM_API UERSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "存档")
	FString SaveName;

	UPROPERTY(VisibleAnywhere, Category = "存档")
	FDateTime SaveTimestamp;

	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	int32 TotalDaysElapsed = 0;

	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	int32 CurrentTurn = 1;

	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	int32 PlayerFactionIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "游戏进度")
	FString MapName;

	UPROPERTY(VisibleAnywhere, Category = "战略层")
	TArray<uint8> SerializedTiles;

	UPROPERTY(VisibleAnywhere, Category = "战略层")
	TArray<uint8> SerializedFactions;

	UPROPERTY(VisibleAnywhere, Category = "战略层")
	TArray<uint8> SerializedUnits;

	UPROPERTY(VisibleAnywhere, Category = "经济")
	int32 PlayerCurrency = 500;

	UPROPERTY(VisibleAnywhere, Category = "名誉")
	int32 PlayerReputation = 10;

	UPROPERTY(VisibleAnywhere, Category = "版本")
	FString GameVersion = "0.1.0";
};
