// Copyright EmberRealm. All Rights Reserved.
#include "UI/EmberHUD.h"
#include "Engine/Font.h"

AEmberHUD::AEmberHUD()
{
	HudScale = 1.0f;
}

void AEmberHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawTopBar();
	DrawBattleLog();
}

void AEmberHUD::DrawTopBar()
{
	if (!HudFont) return;

	float X = 20, Y = 15;
	float Scale = HudScale;

	// 背景条
	DrawRect(FLinearColor(0.05f, 0.05f, 0.08f, 0.85f), 0, 0, Canvas->SizeX, 50 * Scale);

	// 游戏标题
	DrawText(TEXT("烬域：卡拉比亚战区"), FLinearColor(0.9f, 0.7f, 0.3f), X, Y, HudFont, Scale);

	// 可在蓝图中扩展：时间、阵营、资源等信息
}

void AEmberHUD::DrawUnitInfo(const FUnitInstance& Unit)
{
	// 单位信息面板绘制（蓝图扩展）
}

void AEmberHUD::DrawMiniMap()
{
	// 小地图绘制（蓝图扩展）
}

void AEmberHUD::DrawBattleLog()
{
	if (!HudFont) return;
	float Y = Canvas->SizeY - 200;
	float X = 20;
	int32 MaxLogs = 8;

	for (int32 i = 0; i < FMath::Min(BattleLogs.Num(), MaxLogs); i++)
	{
		DrawText(BattleLogs[i], FLinearColor(0.8f, 0.8f, 0.8f), X, Y + i * 20, HudFont, 0.8f);
	}
}

void AEmberHUD::AddBattleLog(const FString& Log)
{
	BattleLogs.Add(Log);
	if (BattleLogs.Num() > 50)
	{
		BattleLogs.RemoveAt(0);
	}
}
