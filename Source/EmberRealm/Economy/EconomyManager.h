// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/EmberStructs.h"
#include "EconomyManager.generated.h"

class UFactionManager;
class UHexGrid;
class UTimeSystem;

/**
 * 卡亚币经济闭环系统（规则全书第七章）
 * 税收、工业开采、练兵费用、油耗维护、贸易
 */
UCLASS(BlueprintType)
class EMBERREALM_API UEconomyManager : public UObject
{
	GENERATED_BODY()

public:
	UEconomyManager();

	/** 全局资源结算（每回合执行） */
	void SettleGlobalResources(UFactionManager* FactionMgr, UHexGrid* Grid, UTimeSystem* TimeSys);

	/** 计算单阵营本回合收入 */
	int32 CalculateIncome(EFactionType Faction, UFactionManager* FactionMgr, UHexGrid* Grid, ETimePhase Phase) const;

	/** 计算单阵营本回合维护支出（油耗、弹药、俘虏） */
	int32 CalculateUpkeep(EFactionType Faction, UFactionManager* FactionMgr) const;

	/** 开采资源矿区（支付启动费） */
	bool StartMining(EFactionType Faction, FIntPoint TileCoord, UFactionManager* FactionMgr, UHexGrid* Grid);

	/** 贸易：两阵营间资源交换 */
	void Trade(EFactionType From, EFactionType To, int32 Amount, UFactionManager* FactionMgr);

private:
	// 规则7.2 居民区税收
	static const int32 ResidentialTaxPeace = 35;
	static const int32 ResidentialTaxWartime = 28;

	// 规则7.3 工业开采
	static const int32 IronMineStartup = 60;
	static const int32 IronMineOutput = 120;
	static const int32 OilFieldStartup = 80;
	static const int32 OilFieldOutput = 150;
	static const int32 CottonStartup = 40;
	static const int32 CottonOutputPeace = 90;
	static const int32 CottonOutputWartime = 45;
};
