// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/ERGameModeBase.h"
#include "Core/EventBus.h"

AERGameModeBase::AERGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	UE_LOG(LogTemp, Log, TEXT("[ERGameModeBase] 游戏模式初始化"));
}

void AERGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("[ERGameModeBase] 游戏开始，第 %d 回合"), CurrentTurn);

	FEventBus::Get().Broadcast(EREvents::OnTurnStart,
		FString::Printf(TEXT("{\"turn\":%d}"), CurrentTurn));

	OnTurnStart();
}

void AERGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AERGameModeBase::EndTurn()
{
	UE_LOG(LogTemp, Log, TEXT("[ERGameModeBase] 第 %d 回合结束"), CurrentTurn);

	OnTurnEnd();

	FEventBus::Get().Broadcast(EREvents::OnTurnEnd,
		FString::Printf(TEXT("{\"turn\":%d}"), CurrentTurn));

	GameClock.AdvanceTurn();
	CurrentTurn++;

	UE_LOG(LogTemp, Log, TEXT("[ERGameModeBase] 进入第 %d 回合"), CurrentTurn);

	OnTurnStart();

	FEventBus::Get().Broadcast(EREvents::OnTurnStart,
		FString::Printf(TEXT("{\"turn\":%d}"), CurrentTurn));
}

void AERGameModeBase::OnTurnStart()
{
	UE_LOG(LogTemp, Log, TEXT("[ERGameModeBase] 回合开始处理"));
}

void AERGameModeBase::OnTurnEnd()
{
	UE_LOG(LogTemp, Log, TEXT("[ERGameModeBase] 回合结束处理"));
}
