// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ERGameInstance.h"
#include "Core/EventBus.h"

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

	Super::Shutdown();
}

void UERGameInstance::SetSelectedFaction(int32 FactionIndex)
{
	SelectedFactionIndex = FactionIndex;
	UE_LOG(LogTemp, Log, TEXT("[ERGameInstance] 选择阵营: %d"), FactionIndex);
}
