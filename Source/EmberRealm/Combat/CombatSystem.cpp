// Copyright EmberRealm. All Rights Reserved.
#include "Combat/CombatSystem.h"
#include "Factions/FactionManager.h"
#include "Map/HexGrid.h"

UCombatSystem::UCombatSystem()
{
}

FBattleResult UCombatSystem::ResolveAttack(UFactionManager* FactionMgr, EFactionType Attacker,
	FGuid AttackerUnitId, FIntPoint TargetCoord)
{
	FBattleResult Result;
	Result.Attacker = Attacker;
	Result.TargetTile = TargetCoord;

	if (!FactionMgr) return Result;

	// 查找攻击单位
	FUnitInstance* AttackUnit = nullptr;
	FFactionState AttackerState = FactionMgr->GetFactionState(Attacker);
	for (FUnitInstance& Unit : const_cast<TArray<FUnitInstance>&>(AttackerState.Units))
	{
		if (Unit.InstanceId == AttackerUnitId)
		{
			AttackUnit = &Unit;
			break;
		}
	}

	if (!AttackUnit || AttackUnit->bHasAttacked || AttackUnit->bHasMoved)
	{
		Result.BattleLog = FText::FromString(TEXT("该单位本回合已行动，无法攻击"));
		return Result;
	}

	// 查找防御单位（目标格上的敌方单位）
	FUnitInstance* DefendUnit = nullptr;
	EFactionType DefenderFaction = EFactionType::Neutral;
	TArray<EFactionType> AllFactions = { EFactionType::Garrison, EFactionType::Contract, EFactionType::Autonomy };
	for (EFactionType F : AllFactions)
	{
		if (F == Attacker) continue;
		FFactionState State = FactionMgr->GetFactionState(F);
		for (FUnitInstance& Unit : const_cast<TArray<FUnitInstance>&>(State.Units))
		{
			if (Unit.TileCoord == TargetCoord)
			{
				DefendUnit = &Unit;
				DefenderFaction = F;
				break;
			}
		}
		if (DefendUnit) break;
	}

	Result.Defender = DefenderFaction;

	if (!DefendUnit)
	{
		// 无防御单位，直接占领
		Result.bAttackerWon = true;
		Result.BattleLog = FText::FromString(TEXT("目标阵地无防御，直接占领"));
		AttackUnit->bHasAttacked = true;
		return Result;
	}

	// 突袭判定
	bool bSurprise = CanSurpriseAttack(*AttackUnit, *DefendUnit);
	Result.bSurpriseAttack = bSurprise;

	// 计算伤害
	int32 Damage = CalculateDamage(*AttackUnit, *DefendUnit, nullptr, bSurprise);
	DefendUnit->CurrentHealth -= Damage;
	Result.DefenderLosses = Damage;

	// 规则10.2：突袭成功，敌方本回合完全无法开火反击
	if (!bSurprise)
	{
		// 反击伤害（简化为50%）
		int32 CounterDamage = FMath::RoundToInt(Damage * 0.5f);
		AttackUnit->CurrentHealth -= CounterDamage;
		Result.AttackerLosses = CounterDamage;
	}

	// 判定胜负
	Result.bAttackerWon = (DefendUnit->CurrentHealth <= 0);
	AttackUnit->bHasAttacked = true;

	if (Result.bAttackerWon)
	{
		Result.BattleLog = FText::FromString(FString::Printf(
			TEXT("攻击成功！造成%d伤害，击溃守军"), Damage));
		// 升勋检查（规则第六章：胜利2场升勋）
		AttackUnit->MeritLevel = FMath::Min(AttackUnit->MeritLevel + 1, 3);
	}
	else
	{
		Result.BattleLog = FText::FromString(FString::Printf(
			TEXT("攻击造成%d伤害，守军剩余%d生命"), Damage, DefendUnit->CurrentHealth));
	}

	UE_LOG(LogTemp, Log, TEXT("[Combat] 战斗结算：%s"), *Result.BattleLog.ToString());
	return Result;
}

int32 UCombatSystem::CalculateDamage(const FUnitInstance& Attacker, const FUnitInstance& Defender,
	UHexGrid* Grid, bool bSurprise) const
{
	int32 BaseDamage = 15; // 基础伤害（简化）

	// 升勋加成（规则第六章）
	int32 Damage = BaseDamage;
	ApplyMeritBonus(Attacker, Damage);

	// 海拔修正
	if (Grid)
	{
		int32 ElevMod = Grid->GetElevationRangeModifier(Attacker.TileCoord, Defender.TileCoord);
		Damage += ElevMod * 3; // 每阶海拔差+3伤害
	}

	// 突袭加成（规则10.2）
	if (bSurprise)
	{
		Damage = FMath::RoundToInt(Damage * 1.5f);
	}

	return FMath::Max(1, Damage);
}

bool UCombatSystem::CanSurpriseAttack(const FUnitInstance& Attacker, const FUnitInstance& Defender) const
{
	// 规则10.1：未被侦察的敌方兵棋全程倒置隐藏
	// 侦察单位接触后完全公开兵力
	// 未侦察遇敌：丧失先手权，无法突袭
	return Defender.bIsHidden; // 防守方未被侦察则可突袭
}

FBattleResult UCombatSystem::ResolveBattle(UFactionManager* FactionMgr, UHexGrid* Grid,
	EFactionType Attacker, EFactionType Defender, FIntPoint TargetTile)
{
	FBattleResult Result;
	Result.Attacker = Attacker;
	Result.Defender = Defender;
	Result.TargetTile = TargetTile;
	Result.bAttackerWon = false;

	UE_LOG(LogTemp, Log, TEXT("[Combat] 战役爆发于 (%d,%d)：阵营%d vs 阵营%d"),
		TargetTile.X, TargetTile.Y, (int32)Attacker, (int32)Defender);

	// 简化战役结算：比较双方总战力
	int32 AttackerPower = 0;
	int32 DefenderPower = 0;

	FFactionState AtkState = FactionMgr->GetFactionState(Attacker);
	FFactionState DefState = FactionMgr->GetFactionState(Defender);

	for (const FUnitInstance& U : AtkState.Units)
	{
		if (Grid && Grid->GetHexDistance(U.TileCoord, TargetTile) <= 2)
			AttackerPower += U.CurrentHealth;
	}
	for (const FUnitInstance& U : DefState.Units)
	{
		if (U.TileCoord == TargetTile || (Grid && Grid->GetHexDistance(U.TileCoord, TargetTile) <= 1))
			DefenderPower += U.CurrentHealth;
	}

	Result.bAttackerWon = AttackerPower > DefenderPower;
	Result.BattleLog = FText::FromString(FString::Printf(
		TEXT("战役结算：攻方战力%d vs 守方战力%d，%s"),
		AttackerPower, DefenderPower,
		Result.bAttackerWon ? TEXT("攻方胜利") : TEXT("守方胜利")));

	return Result;
}

void UCombatSystem::ApplyMeritBonus(const FUnitInstance& Unit, int32& Damage) const
{
	// 规则第六章：一勋+10%，二勋+25%，三勋+50%
	switch (Unit.MeritLevel)
	{
	case 1: Damage = FMath::RoundToInt(Damage * 1.1f); break;
	case 2: Damage = FMath::RoundToInt(Damage * 1.25f); break;
	case 3: Damage = FMath::RoundToInt(Damage * 1.5f); break;
	default: break;
	}
}
