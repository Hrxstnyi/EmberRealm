// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/EmberStructs.h"
#include "CombatSystem.generated.h"

class UFactionManager;
class UHexGrid;

/**
 * 战斗结算系统（规则全书第十章）
 * 侦察、突袭、海拔修正、行军开火铁规、阵地保护
 */
UCLASS(BlueprintType)
class EMBERREALM_API UCombatSystem : public UObject
{
	GENERATED_BODY()

public:
	UCombatSystem();

	/** 解析一次攻击行动 */
	FBattleResult ResolveAttack(UFactionManager* FactionMgr, EFactionType Attacker,
		FGuid AttackerUnitId, FIntPoint TargetCoord);

	/** 计算伤害（考虑海拔、升勋、突袭） */
	int32 CalculateDamage(const FUnitInstance& Attacker, const FUnitInstance& Defender,
		UHexGrid* Grid, bool bSurprise) const;

	/** 检查是否可以发起突袭（敌方未被侦察） */
	bool CanSurpriseAttack(const FUnitInstance& Attacker, const FUnitInstance& Defender) const;

	/** 战役结算：多单位混战 */
	FBattleResult ResolveBattle(UFactionManager* FactionMgr, UHexGrid* Grid,
		EFactionType Attacker, EFactionType Defender, FIntPoint TargetTile);

private:
	void ApplyMeritBonus(const FUnitInstance& Unit, int32& Damage) const;
};
