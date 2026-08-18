// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Core/EmberStructs.h"
#include "EmberHUD.generated.h"

class UFont;

UCLASS()
class EMBERREALM_API AEmberHUD : public AHUD
{
	GENERATED_BODY()

public:
	AEmberHUD();

	virtual void DrawHUD() override;

	/** 绘制顶部状态栏（时间、阵营、资源） */
	void DrawTopBar();

	/** 绘制选中单位信息面板 */
	void DrawUnitInfo(const FUnitInstance& Unit);

	/** 绘制小地图 */
	void DrawMiniMap();

	/** 绘制战报日志 */
	void DrawBattleLog();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EmberRealm|HUD")
	float HudScale = 1.0f;

protected:
	UPROPERTY()
	UFont* HudFont = nullptr;

	TArray<FString> BattleLogs;

public:
	void AddBattleLog(const FString& Log);
};
