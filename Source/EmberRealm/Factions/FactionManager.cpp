// Copyright EmberRealm. All Rights Reserved.
#include "Factions/FactionManager.h"
#include "Map/HexGrid.h"

const ERankLevel UFactionManager::RankChain[16] = {
	ERankLevel::SecondLieutenant, ERankLevel::Lieutenant, ERankLevel::Captain,
	ERankLevel::MajorII, ERankLevel::MajorI, ERankLevel::LieutenantColonelII, ERankLevel::LieutenantColonelI,
	ERankLevel::ColonelII, ERankLevel::ColonelI,
	ERankLevel::MajorGeneralIII, ERankLevel::MajorGeneralII, ERankLevel::MajorGeneralI,
	ERankLevel::LieutenantGeneralIII, ERankLevel::LieutenantGeneralII, ERankLevel::LieutenantGeneralI,
	ERankLevel::Commander
};
const int32 UFactionManager::RankNamesCount = 16;

UFactionManager::UFactionManager()
{
}

void UFactionManager::InitializeFactions(const TArray<EFactionType>& FactionList)
{
	FactionStates.Empty();

	for (EFactionType Faction : FactionList)
	{
		FFactionState State;
		State.Faction = Faction;
		State.KayaCoins = 200;
		State.Reputation = 10;
		State.Rank = ERankLevel::SecondLieutenant;
		State.ActionPoints = 6;
		State.MaxActionPoints = 11;
		State.bIsUnderSanction = false;

		switch (Faction)
		{
		case EFactionType::Garrison:
			State.FactionName = FText::FromString(TEXT("戍卫军"));
			break;
		case EFactionType::Contract:
			State.FactionName = FText::FromString(TEXT("合约团"));
			break;
		case EFactionType::Autonomy:
			State.FactionName = FText::FromString(TEXT("自治同盟"));
			break;
		default:
			State.FactionName = FText::FromString(TEXT("未知"));
		}

		// 开局初始单位：3个民兵 + 1个根据地
		for (int32 i = 0; i < 3; i++)
		{
			FIntPoint StartCoord;
			switch (Faction)
			{
			case EFactionType::Garrison:	StartCoord = FIntPoint(i, 0); break;
			case EFactionType::Contract:	StartCoord = FIntPoint(11 - i, 0); break;
			case EFactionType::Autonomy:	StartCoord = FIntPoint(4 + i, 9); break;
			default: StartCoord = FIntPoint(0, 0);
			}
			State.Units.Add(CreateUnit(EUnitType::Militia, Faction, StartCoord));
			State.ControlledTiles.Add(StartCoord);
		}

		FactionStates.Add(Faction, State);
	}

	UE_LOG(LogTemp, Log, TEXT("[FactionManager] 初始化 %d 个阵营完成"), FactionList.Num());
}

FFactionState UFactionManager::GetFactionState(EFactionType Faction) const
{
	if (const FFactionState* State = FactionStates.Find(Faction))
	{
		return *State;
	}
	return FFactionState();
}

void UFactionManager::ResetFactionActions(EFactionType Faction)
{
	if (FFactionState* State = FactionStates.Find(Faction))
	{
		for (FUnitInstance& Unit : State->Units)
		{
			Unit.bHasMoved = false;
			Unit.bHasAttacked = false;
		}
	}
}

void UFactionManager::RestoreActionPoints(ETimePhase Phase)
{
	int32 Recovery = 3;
	switch (Phase)
	{
	case ETimePhase::Peace:		Recovery = 6; break;
	case ETimePhase::Wartime:	Recovery = 3; break;
	case ETimePhase::Battle:	Recovery = 2; break;
	}

	for (auto& Pair : FactionStates)
	{
		Pair.Value.ActionPoints = FMath::Min(Pair.Value.ActionPoints + Recovery, Pair.Value.MaxActionPoints);
	}
}

bool UFactionManager::SpendActionPoints(EFactionType Faction, int32 Amount)
{
	if (FFactionState* State = FactionStates.Find(Faction))
	{
		if (State->ActionPoints >= Amount)
		{
			State->ActionPoints -= Amount;
			return true;
		}
	}
	return false;
}

bool UFactionManager::MoveUnit(EFactionType Faction, FGuid UnitId, FIntPoint TargetCoord, UHexGrid* Grid)
{
	if (!Grid) return false;
	if (FFactionState* State = FactionStates.Find(Faction))
	{
		for (FUnitInstance& Unit : State->Units)
		{
			if (Unit.InstanceId == UnitId && !Unit.bHasMoved)
			{
				// 规则10.3：行军完毕本回合不能开火
				Unit.TileCoord = TargetCoord;
				Unit.bHasMoved = true;
				Unit.bHasAttacked = true; // 行军后不能开火

				// 占领阵地
				Grid->SetTileController(TargetCoord, Faction);
				if (!State->ControlledTiles.Contains(TargetCoord))
				{
					State->ControlledTiles.Add(TargetCoord);
				}

				UE_LOG(LogTemp, Log, TEXT("[FactionManager] 单位移动到 (%d,%d)"), TargetCoord.X, TargetCoord.Y);
				return true;
			}
		}
	}
	return false;
}

bool UFactionManager::TrainUnit(EFactionType Faction, EUnitType UnitType, FIntPoint BaseCoord)
{
	FFactionState* State = FactionStates.Find(Faction);
	if (!State || State->bIsUnderSanction) return false;

	// 简化练兵费用检查
	int32 Cost = 40;
	if (State->KayaCoins >= Cost)
	{
		State->KayaCoins -= Cost;
		State->Units.Add(CreateUnit(UnitType, Faction, BaseCoord));
		UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 训练新单位，花费 %d 卡亚币"), (int32)Faction, Cost);
		return true;
	}
	return false;
}

void UFactionManager::AddReputation(EFactionType Faction, int32 Delta)
{
	if (FFactionState* State = FactionStates.Find(Faction))
	{
		State->Reputation += Delta;
		UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 名誉值 %+d -> %d"),
			(int32)Faction, Delta, State->Reputation);

		CheckRankPromotion(*State);
		CheckJudgmentCard(*State);

		// 规则5.5：名誉负值触发禁闭
		if (State->Reputation < 0)
		{
			State->bIsUnderSanction = true;
			State->SanctionTurnsLeft = 1;
			UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 名誉负值，触发禁闭！"), (int32)Faction);
		}
	}
}

void UFactionManager::UpdateSanctions()
{
	for (auto& Pair : FactionStates)
	{
		FFactionState& State = Pair.Value;
		if (State.bIsUnderSanction)
		{
			State.SanctionTurnsLeft--;
			if (State.SanctionTurnsLeft <= 0)
			{
				State.bIsUnderSanction = false;
				UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 禁闭解除"), (int32)State.Faction);
			}
		}
	}
}

void UFactionManager::FormAlliance(EFactionType A, EFactionType B)
{
	FFactionState* StateA = FactionStates.Find(A);
	FFactionState* StateB = FactionStates.Find(B);
	if (StateA && StateB)
	{
		StateA->Allies.AddUnique(B);
		StateB->Allies.AddUnique(A);
		AddReputation(A, 5);
		AddReputation(B, 5);
		UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 与 %d 结盟"), (int32)A, (int32)B);
	}
}

void UFactionManager::Betray(EFactionType Betrayer, EFactionType Victim)
{
	FFactionState* State = FactionStates.Find(Betrayer);
	if (State)
	{
		State->Allies.Remove(Victim);
		AddReputation(Betrayer, -10); // 背刺减10名誉
		UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 背刺阵营 %d！"), (int32)Betrayer, (int32)Victim);
	}
}

void UFactionManager::CheckRankPromotion(FFactionState& State)
{
	// 规则5.2：每升10点名誉晋升一级
	int32 TargetRankIndex = FMath::Clamp(State.Reputation / 10, 0, RankNamesCount - 1);
	ERankLevel NewRank = RankChain[TargetRankIndex];
	if (NewRank != State.Rank)
	{
		State.Rank = NewRank;
		UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 军衔晋升！"), (int32)State.Faction);
	}
}

void UFactionManager::CheckJudgmentCard(FFactionState& State)
{
	// 规则5.4：名誉累计扣除20点 → 判决Debuff牌
	// 简化处理：名誉低于0时获得
	if (State.Reputation <= -20)
	{
		UE_LOG(LogTemp, Log, TEXT("[FactionManager] 阵营 %d 获得判决牌！"), (int32)State.Faction);
	}
}

FUnitInstance UFactionManager::CreateUnit(EUnitType Type, EFactionType Owner, FIntPoint Coord)
{
	FUnitInstance Unit;
	Unit.InstanceId = FGuid::NewGuid();
	Unit.UnitType = Type;
	Unit.OwnerFaction = Owner;
	Unit.TileCoord = Coord;
	Unit.CurrentHealth = 100;
	Unit.CurrentAmmo = 10;
	Unit.MeritLevel = 0;
	Unit.bHasMoved = false;
	Unit.bHasAttacked = false;
	Unit.bIsHidden = false;
	return Unit;
}
