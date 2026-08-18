// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/EmberStructs.h"
#include "FactionManager.generated.h"

class UHexGrid;

UCLASS(BlueprintType)
class EMBERREALM_API UFactionManager : public UObject
{
	GENERATED_BODY()

public:
	UFactionManager();

	/** 初始化所有阵营 */
	void InitializeFactions(const TArray<EFactionType>& FactionList);

	/** 获取阵营状态 */
	FFactionState GetFactionState(EFactionType Faction) const;

	/** 重置阵营所有单位的行动状态（新回合开始） */
	void ResetFactionActions(EFactionType Faction);

	/** 恢复行动点 */
	void RestoreActionPoints(ETimePhase Phase);

	/** 消耗行动点 */
	bool SpendActionPoints(EFactionType Faction, int32 Amount);

	/** 移动单位 */
	bool MoveUnit(EFactionType Faction, FGuid UnitId, FIntPoint TargetCoord, UHexGrid* Grid);

	/** 训练新兵 */
	bool TrainUnit(EFactionType Faction, EUnitType UnitType, FIntPoint BaseCoord);

	/** 增加/减少名誉值，处理军衔晋升和判决牌 */
	void AddReputation(EFactionType Faction, int32 Delta);

	/** 检查并处理禁闭机制 */
	void UpdateSanctions();

	/** 结盟 */
	void FormAlliance(EFactionType A, EFactionType B);

	/** 背刺撕毁条约 */
	void Betray(EFactionType Betrayer, EFactionType Victim);

private:
	TMap<EFactionType, FFactionState> FactionStates;

	/** 军衔晋升链（规则5.6） */
	static const ERankLevel RankChain[16];
	static const int32 RankNamesCount;

	void CheckRankPromotion(FFactionState& State);
	void CheckJudgmentCard(FFactionState& State);
	FUnitInstance CreateUnit(EUnitType Type, EFactionType Owner, FIntPoint Coord);
};
