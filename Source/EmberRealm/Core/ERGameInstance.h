// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ERGameInstance.generated.h"

/**
 * 游戏全局实例
 * 管理跨关卡数据、存档、全局设置
 */
UCLASS()
class EMBERREALM_API UERGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UERGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	/** 是否启用自动存档 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	bool bAutoSaveEnabled = true;

	/** 自动存档间隔（分钟） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	int32 AutoSaveIntervalMinutes = 5;

	/** 最大存档槽位数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	int32 MaxSaveSlots = 10;

	/** 当前选中的阵营 */
	UPROPERTY(BlueprintReadOnly, Category = "阵营")
	int32 SelectedFactionIndex = -1;

	/** 设置选中阵营 */
	UFUNCTION(BlueprintCallable, Category = "阵营")
	void SetSelectedFaction(int32 FactionIndex);
};
