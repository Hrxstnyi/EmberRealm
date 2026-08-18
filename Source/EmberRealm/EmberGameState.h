// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/EmberStructs.h"
#include "EmberGameState.generated.h"

UCLASS()
class EMBERREALM_API AEmberGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AEmberGameState();

	/** 全局时间（同步给所有客户端） */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "EmberRealm|Time")
	FGameTime GameTime;

	/** 当前行动阵营 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "EmberRealm|Turn")
	EFactionType CurrentFaction = EFactionType::Garrison;

	/** 全局回合数 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "EmberRealm|Turn")
	int32 GlobalTurn = 1;

	/** 游戏是否结束 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "EmberRealm|Game")
	bool bGameOver = false;

	/** 胜利阵营 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "EmberRealm|Game")
	EFactionType Winner = EFactionType::Neutral;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
