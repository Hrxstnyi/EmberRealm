// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ERGameInstance.h"
#include "Core/EventBus.h"
#include "Core/GameClock.h"

UERGameInstance::UERGameInstance()
{
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 游戏实例初始化"));
}

void UERGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 游戏实例启动"));
	FEventBus::Get().Clear();
}

void UERGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 游戏实例关闭"));
	FEventBus::Get().Clear();
	EconomySystems.Empty();
	Super::Shutdown();
}

void UERGameInstance::SetSelectedFaction(int32 FactionIndex)
{
	SelectedFactionIndex = FactionIndex;
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 选择阵营: %d"), FactionIndex);
}

FEconomySystem* UERGameInstance::GetEconomySystem(EFactionType Faction)
{
	if (TSharedPtr<FEconomySystem>* Found = EconomySystems.Find(Faction))
	{
		return Found->Get();
	}
	return nullptr;
}

void UERGameInstance::StartNewGame(int32 MapSizeX, int32 MapSizeY)
{
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 开始新游戏: %dx%d"), MapSizeX, MapSizeY);

	StrategyMap.InitializeMap(MapSizeX, MapSizeY);
	InitializeEconomySystems();
	CurrentTurn = 1;

	FEventBus::Get().Broadcast(EREvents::OnCampaignStart,
		FString::Printf(TEXT("{\"turn\":%d,\"map\":\"%dx%d\"}"), CurrentTurn, MapSizeX, MapSizeY));
}

void UERGameInstance::EndTurn()
{
	CurrentTurn++;
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 回合 %d 结束"), CurrentTurn);

	FEventBus::Get().Broadcast(EREvents::OnTurnEnd,
		FString::Printf(TEXT("{\"turn\":%d}"), CurrentTurn));

	for (auto& Pair : EconomySystems)
	{
		if (Pair.Value.IsValid())
		{
			const bool bIsWarTime = true;
			Pair.Value->ProcessTurn(CurrentTurn, bIsWarTime);
		}
	}

	FEventBus::Get().Broadcast(EREvents::OnTurnStart,
		FString::Printf(TEXT("{\"turn\":%d}"), CurrentTurn + 1));
}

void UERGameInstance::InitializeEconomySystems()
{
	EconomySystems.Empty();

	const TArray<TPair<EFactionType, int32>> FactionStartCurrency = {
		{ EFactionType::GarrisonArmy, 800 },
		{ EFactionType::ContractCorps, 600 },
		{ EFactionType::AutonomyLeague, 500 }
	};

	for (const auto& Pair : FactionStartCurrency)
	{
		TSharedPtr<FEconomySystem> Economy = MakeShared<FEconomySystem>();
		FResourcePack InitialResources;
		InitialResources.Currency = Pair.Value;
		InitialResources.Manpower = 200;
		InitialResources.Ammo = 100;
		Economy->Initialize(Pair.Key, InitialResources);
		EconomySystems.Add(Pair.Key, Economy);
	}

	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 初始化 %d 个阵营经济系统"), EconomySystems.Num());
}
