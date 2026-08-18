// Copyright EmberRealm. All Rights Reserved.
#include "EmberGameMode.h"
#include "Map/HexGrid.h"
#include "Economy/EconomyManager.h"
#include "Combat/CombatSystem.h"
#include "Core/TimeSystem.h"
#include "Factions/FactionManager.h"
#include "EmberPlayerController.h"
#include "EmberGameState.h"

AEmberGameMode::AEmberGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerControllerClass = AEmberPlayerController::StaticClass();
	GameStateClass = AEmberGameState::StaticClass();
}

void AEmberGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitializeSystems();

	// 默认三方对战顺序
	FactionOrder = { EFactionType::Garrison, EFactionType::Contract, EFactionType::Autonomy };
	CurrentFaction = FactionOrder[0];
	CurrentFactionIndex = 0;

	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 游戏开始！当前阵营：%d，回合：%d"),
		(int32)CurrentFaction, TurnNumber);
}

void AEmberGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (AEmberPlayerController* EmberPC = Cast<AEmberPlayerController>(NewPlayer))
	{
		EmberPC->SetOwningFaction(CurrentFaction);
	}
}

void AEmberGameMode::InitializeSystems()
{
	HexGrid = NewObject<UHexGrid>(this, TEXT("HexGrid"));
	HexGrid->InitializeGrid(12, 10); // 12x10 六边形战区

	EconomyManager = NewObject<UEconomyManager>(this, TEXT("EconomyManager"));
	CombatSystem = NewObject<UCombatSystem>(this, TEXT("CombatSystem"));
	TimeSystem = NewObject<UTimeSystem>(this, TEXT("TimeSystem"));
	FactionManager = NewObject<UFactionManager>(this, TEXT("FactionManager"));

	if (FactionManager)
	{
		FactionManager->InitializeFactions(FactionOrder);
	}

	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 所有子系统初始化完成"));
}

void AEmberGameMode::EndTurn()
{
	if (bGameOver) return;

	// 重置当前阵营单位行动状态
	if (FactionManager)
	{
		FactionManager->ResetFactionActions(CurrentFaction);
	}

	// 推进到下一阵营
	CurrentFactionIndex = (CurrentFactionIndex + 1) % FactionOrder.Num();
	CurrentFaction = FactionOrder[CurrentFactionIndex];

	// 一轮结束（所有阵营都行动过），推进时间并执行全局结算
	if (CurrentFactionIndex == 0)
	{
		TurnNumber++;
		ExecuteTurnPipeline();
	}

	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 回合切换 -> 阵营：%d，全局回合：%d"),
		(int32)CurrentFaction, TurnNumber);

	CheckVictoryConditions();
}

void AEmberGameMode::AdvanceTime()
{
	if (TimeSystem)
	{
		TimeSystem->Advance();
	}
}

void AEmberGameMode::DeclareWar(EFactionType Attacker, EFactionType Defender)
{
	if (TimeSystem)
	{
		TimeSystem->EnterBattlePhase();
	}
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 阵营 %d 向阵营 %d 宣战！进入战役状态"),
		(int32)Attacker, (int32)Defender);
}

void AEmberGameMode::SignCeasefire(EFactionType A, EFactionType B)
{
	if (TimeSystem)
	{
		TimeSystem->EnterPeacePhase();
	}
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 阵营 %d 与阵营 %d 签署停火协议"), (int32)A, (int32)B);
}

FGameTime AEmberGameMode::GetGameTime() const
{
	if (TimeSystem) return TimeSystem->GetCurrentTime();
	return FGameTime();
}

void AEmberGameMode::ExecuteTurnPipeline()
{
	// 规则全书第十四章：标准化全局回合流程
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] === 执行全局回合流程 ==="));

	// 1. 时序状态判定更新
	if (TimeSystem) TimeSystem->UpdatePhase();

	// 2. 全局资源结算（税收、开采、油耗、维护扣费）
	if (EconomyManager && HexGrid)
	{
		EconomyManager->SettleGlobalResources(FactionManager, HexGrid, TimeSystem);
	}

	// 3. 行动点回复
	if (FactionManager && TimeSystem)
	{
		FactionManager->RestoreActionPoints(TimeSystem->GetCurrentTime().TimePhase);
	}

	// 4-9. 外交、建设、侦察、行军、战役、成长（由玩家在回合内操作）

	// 10. 事件牌触发（占位）
	// 11. 战报（占位）

	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] === 全局回合流程结束 ==="));
}

void AEmberGameMode::CheckVictoryConditions()
{
	if (!FactionManager || !HexGrid) return;

	for (EFactionType Faction : FactionOrder)
	{
		FFactionState State = FactionManager->GetFactionState(Faction);

		// A. 战区霸权胜利：掌控≥50%交通枢纽 + ≥70%资源阵地
		int32 TotalHubs = HexGrid->GetHubCount();
		int32 ControlledHubs = HexGrid->GetControlledHubCount(Faction);
		int32 TotalResourceTiles = HexGrid->GetResourceTileCount();
		int32 ControlledResources = HexGrid->GetControlledResourceCount(Faction);

		if (TotalHubs > 0 && TotalResourceTiles > 0 &&
			ControlledHubs >= TotalHubs * 0.5f &&
			ControlledResources >= TotalResourceTiles * 0.7f)
		{
			bGameOver = true;
			UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 阵营 %d 达成战区霸权胜利！"), (int32)Faction);
			return;
		}

		// C. 经济霸权胜利：卡亚币远超其他阵营
		bool bEconomicDominance = true;
		for (EFactionType Other : FactionOrder)
		{
			if (Other == Faction) continue;
			FFactionState OtherState = FactionManager->GetFactionState(Other);
			if (State.KayaCoins <= OtherState.KayaCoins * 2)
			{
				bEconomicDominance = false;
				break;
			}
		}
		if (bEconomicDominance && TurnNumber > 16) // 第四年开始
		{
			bGameOver = true;
			UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 阵营 %d 达成经济霸权胜利！"), (int32)Faction);
			return;
		}
	}
}
