// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/EmberStructs.h"
#include "EmberGameMode.generated.h"

class UHexGrid;
class UEconomyManager;
class UCombatSystem;
class UTimeSystem;
class UFactionManager;

UCLASS()
class EMBERREALM_API AEmberGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEmberGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** 结束当前玩家回合，推进到下一阵营 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Turn")
	void EndTurn();

	/** 推进全局时间（根据当前时间相位） */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Time")
	void AdvanceTime();

	/** 宣战：切入战时/战役状态 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Diplomacy")
	void DeclareWar(EFactionType Attacker, EFactionType Defender);

	/** 签署停火协议 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Diplomacy")
	void SignCeasefire(EFactionType A, EFactionType B);

	/** 获取当前行动阵营 */
	UFUNCTION(BlueprintPure, Category = "EmberRealm|Turn")
	EFactionType GetCurrentFaction() const { return CurrentFaction; }

	/** 获取全局时间 */
	UFUNCTION(BlueprintPure, Category = "EmberRealm|Time")
	FGameTime GetGameTime() const;

	/** 子系统引用 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Systems")
	UHexGrid* HexGrid = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Systems")
	UEconomyManager* EconomyManager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Systems")
	UCombatSystem* CombatSystem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Systems")
	UTimeSystem* TimeSystem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Systems")
	UFactionManager* FactionManager = nullptr;

	/** 回合数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Turn")
	int32 TurnNumber = 1;

protected:
	/** 初始化所有子系统 */
	void InitializeSystems();

	/** 执行标准化全局回合流程（规则全书第十四章） */
	void ExecuteTurnPipeline();

	/** 胜利条件检测 */
	void CheckVictoryConditions();

private:
	EFactionType CurrentFaction = EFactionType::Garrison;
	int32 CurrentFactionIndex = 0;
	TArray<EFactionType> FactionOrder;
	bool bGameOver = false;
};
