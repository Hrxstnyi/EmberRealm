// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataSystem/ERDataLibrary.h"

TArray<UERUnitData*> UERDataLibrary::CreateDefaultUnitData(UObject* WorldContextObject)
{
	TArray<UERUnitData*> Result;
	if (!WorldContextObject) return Result;

	UObject* Outer = WorldContextObject->GetWorld() ? (UObject*)WorldContextObject->GetWorld() : (UObject*)GetTransientPackage();

	auto CreateUnit = [&](EUnitType Type, const TCHAR* Name, EUnitDomain Domain,
		int32 Cost, int32 TrainWeeks, float Health, float Attack, int32 Range,
		float Defense, int32 Move, int32 Ammo, int32 Fuel, int32 Init, int32 Recon,
		int32 Stealth, ERankLevel ReqRank, EUnitType ReqUnit) -> UERUnitData*
	{
		UERUnitData* Data = NewObject<UERUnitData>(Outer, UERUnitData::StaticClass(), Name);
		Data->UnitName = FText::FromString(Name);
		Data->UnitType = Type;
		Data->Domain = Domain;
		Data->TrainCost = Cost;
		Data->TrainTimeWeeks = TrainWeeks;
		Data->MaxHealth = Health;
		Data->AttackDamage = Attack;
		Data->AttackRange = Range;
		Data->Defense = Defense;
		Data->Movement = Move;
		Data->MaxAmmo = Ammo;
		Data->FuelConsumption = Fuel;
		Data->Initiative = Init;
		Data->ReconRange = Recon;
		Data->Stealth = Stealth;
		Data->RequiredRank = ReqRank;
		Data->RequiredUnit = ReqUnit;
		Result.Add(Data);
		return Data;
	};

	CreateUnit(EUnitType::Infantry, TEXT("步兵"), EUnitDomain::Land, 40, 2, 100, 20, 1, 15, 2, 30, 0, 50, 2, 0, ERankLevel::SecondLieutenant, EUnitType::Militia);
	CreateUnit(EUnitType::Recon, TEXT("侦察兵"), EUnitDomain::Land, 35, 1, 60, 12, 1, 8, 4, 20, 0, 70, 5, 30, ERankLevel::SecondLieutenant, EUnitType::Infantry);
	CreateUnit(EUnitType::Medic, TEXT("医疗兵"), EUnitDomain::Land, 50, 2, 70, 8, 1, 10, 2, 25, 0, 40, 2, 0, ERankLevel::FirstLieutenant, EUnitType::Infantry);
	CreateUnit(EUnitType::Guerrilla, TEXT("游击兵"), EUnitDomain::Land, 45, 2, 80, 22, 1, 12, 3, 25, 0, 65, 3, 50, ERankLevel::FirstLieutenant, EUnitType::Infantry);
	CreateUnit(EUnitType::Coastal, TEXT("岸防兵"), EUnitDomain::Land, 60, 3, 120, 35, 2, 25, 1, 40, 0, 30, 1, 0, ERankLevel::Captain, EUnitType::Artillery);
	CreateUnit(EUnitType::AirDefense, TEXT("防空兵"), EUnitDomain::Land, 70, 3, 90, 40, 3, 20, 2, 50, 5, 35, 2, 0, ERankLevel::Captain, EUnitType::Artillery);
	CreateUnit(EUnitType::Officer, TEXT("前线军官"), EUnitDomain::Land, 80, 3, 90, 18, 1, 15, 2, 20, 0, 60, 3, 0, ERankLevel::MajorII, EUnitType::Infantry);
	CreateUnit(EUnitType::Commissar, TEXT("督战兵"), EUnitDomain::Land, 55, 2, 80, 15, 1, 12, 2, 20, 0, 55, 2, 0, ERankLevel::FirstLieutenant, EUnitType::Infantry);
	CreateUnit(EUnitType::Paratrooper, TEXT("空降兵"), EUnitDomain::Air, 90, 4, 90, 28, 1, 12, 3, 30, 10, 75, 3, 20, ERankLevel::Captain, EUnitType::Infantry);
	CreateUnit(EUnitType::Pilot, TEXT("航空兵"), EUnitDomain::Air, 120, 5, 80, 45, 3, 10, 4, 60, 20, 80, 4, 10, ERankLevel::MajorI, EUnitType::Paratrooper);
	CreateUnit(EUnitType::Sniper, TEXT("狙击兵"), EUnitDomain::Land, 75, 3, 70, 50, 4, 8, 2, 15, 0, 85, 3, 40, ERankLevel::Captain, EUnitType::Infantry);
	CreateUnit(EUnitType::Artillery, TEXT("阵地炮兵"), EUnitDomain::Land, 100, 4, 80, 55, 4, 15, 1, 80, 5, 25, 1, 0, ERankLevel::Captain, EUnitType::Infantry);
	CreateUnit(EUnitType::Armor, TEXT("装甲兵"), EUnitDomain::Land, 150, 5, 200, 40, 1, 35, 3, 40, 15, 40, 2, 0, ERankLevel::MajorI, EUnitType::Infantry);
	CreateUnit(EUnitType::Engineer, TEXT("工程兵"), EUnitDomain::Land, 55, 2, 70, 10, 1, 12, 2, 20, 0, 45, 2, 0, ERankLevel::FirstLieutenant, EUnitType::Infantry);
	CreateUnit(EUnitType::Submarine, TEXT("潜艇"), EUnitDomain::Sea, 180, 6, 150, 60, 2, 20, 2, 50, 25, 70, 3, 60, ERankLevel::LieutenantColonelII, EUnitType::Warship);
	CreateUnit(EUnitType::Warship, TEXT("舰艇"), EUnitDomain::Sea, 200, 6, 250, 50, 3, 40, 2, 100, 30, 35, 2, 0, ERankLevel::LieutenantColonelII, EUnitType::Coastal);
	CreateUnit(EUnitType::Militia, TEXT("民兵"), EUnitDomain::Land, 20, 1, 60, 12, 1, 8, 2, 15, 0, 30, 1, 0, ERankLevel::SecondLieutenant, EUnitType::Militia);

	UE_LOG(LogTemp, Log, TEXT("[ERDataLibrary] 生成 %d 种默认兵种数据"), Result.Num());
	return Result;
}

TArray<UERFactionData*> UERDataLibrary::CreateDefaultFactionData(UObject* WorldContextObject)
{
	TArray<UERFactionData*> Result;
	if (!WorldContextObject) return Result;

	UObject* Outer = WorldContextObject->GetWorld() ? (UObject*)WorldContextObject->GetWorld() : (UObject*)GetTransientPackage();

	{
		UERFactionData* Data = NewObject<UERFactionData>(Outer, UERFactionData::StaticClass(), TEXT("戍卫军"));
		Data->FactionName = FText::FromString(TEXT("戍卫军"));
		Data->FactionType = EFactionType::GarrisonArmy;
		Data->Description = FText::FromString(TEXT("国家正规军，装备精良，补给充足，擅长阵地战和多兵种协同。"));
		Data->InitialCurrency = 800;
		Data->InitialReputation = 15;
		Data->PrimaryColor = FLinearColor(0.15f, 0.35f, 0.75f);
		Data->SecondaryColor = FLinearColor(0.8f, 0.85f, 0.95f);
		Data->FactionBonuses.Add(TEXT("DefenseBonus"), 0.10f);
		Data->FactionBonuses.Add(TEXT("IncomeBonus"), 0.10f);
		Result.Add(Data);
	}

	{
		UERFactionData* Data = NewObject<UERFactionData>(Outer, UERFactionData::StaticClass(), TEXT("合约团"));
		Data->FactionName = FText::FromString(TEXT("合约团"));
		Data->FactionType = EFactionType::ContractCorps;
		Data->Description = FText::FromString(TEXT("精英雇佣武装，战斗力强但维护昂贵，擅长突袭和特种作战。"));
		Data->InitialCurrency = 600;
		Data->InitialReputation = 5;
		Data->PrimaryColor = FLinearColor(0.85f, 0.4f, 0.1f);
		Data->SecondaryColor = FLinearColor(0.95f, 0.8f, 0.6f);
		Data->FactionBonuses.Add(TEXT("AttackBonus"), 0.15f);
		Data->FactionBonuses.Add(TEXT("MoveBonus"), 0.10f);
		Data->FactionBonuses.Add(TEXT("UpkeepPenalty"), 0.20f);
		Result.Add(Data);
	}

	{
		UERFactionData* Data = NewObject<UERFactionData>(Outer, UERFactionData::StaticClass(), TEXT("自治同盟"));
		Data->FactionName = FText::FromString(TEXT("自治同盟"));
		Data->FactionType = EFactionType::AutonomyLeague;
		Data->Description = FText::FromString(TEXT("地方自治武装，士气高昂，熟悉地形，擅长游击战和防御。"));
		Data->InitialCurrency = 500;
		Data->InitialReputation = 20;
		Data->PrimaryColor = FLinearColor(0.15f, 0.65f, 0.3f);
		Data->SecondaryColor = FLinearColor(0.7f, 0.9f, 0.7f);
		Data->FactionBonuses.Add(TEXT("MoraleBonus"), 0.20f);
		Data->FactionBonuses.Add(TEXT("GuerrillaBonus"), 0.15f);
		Data->FactionBonuses.Add(TEXT("TerrainBonus"), 0.10f);
		Result.Add(Data);
	}

	UE_LOG(LogTemp, Log, TEXT("[ERDataLibrary] 生成 %d 个默认阵营数据"), Result.Num());
	return Result;
}

FText UERDataLibrary::GetUnitTypeName(EUnitType Type)
{
	switch (Type)
	{
	case EUnitType::Infantry:		return FText::FromString(TEXT("步兵"));
	case EUnitType::Recon:			return FText::FromString(TEXT("侦察兵"));
	case EUnitType::Medic:			return FText::FromString(TEXT("医疗兵"));
	case EUnitType::Guerrilla:		return FText::FromString(TEXT("游击兵"));
	case EUnitType::Coastal:		return FText::FromString(TEXT("岸防兵"));
	case EUnitType::AirDefense:		return FText::FromString(TEXT("防空兵"));
	case EUnitType::Officer:		return FText::FromString(TEXT("前线军官"));
	case EUnitType::Commissar:		return FText::FromString(TEXT("督战兵"));
	case EUnitType::Paratrooper:	return FText::FromString(TEXT("空降兵"));
	case EUnitType::Pilot:			return FText::FromString(TEXT("航空兵"));
	case EUnitType::Sniper:			return FText::FromString(TEXT("狙击兵"));
	case EUnitType::Artillery:		return FText::FromString(TEXT("阵地炮兵"));
	case EUnitType::Armor:			return FText::FromString(TEXT("装甲兵"));
	case EUnitType::Engineer:		return FText::FromString(TEXT("工程兵"));
	case EUnitType::Submarine:		return FText::FromString(TEXT("潜艇"));
	case EUnitType::Warship:		return FText::FromString(TEXT("舰艇"));
	case EUnitType::Militia:		return FText::FromString(TEXT("民兵"));
	default:					return FText::FromString(TEXT("未知"));
	}
}

FText UERDataLibrary::GetFactionName(EFactionType Faction)
{
	switch (Faction)
	{
	case EFactionType::GarrisonArmy:	return FText::FromString(TEXT("戍卫军"));
	case EFactionType::ContractCorps:	return FText::FromString(TEXT("合约团"));
	case EFactionType::AutonomyLeague:	return FText::FromString(TEXT("自治同盟"));
	case EFactionType::Neutral:			return FText::FromString(TEXT("中立"));
	case EFactionType::Saboteur:		return FText::FromString(TEXT("内鬼"));
	default:						return FText::FromString(TEXT("未知"));
	}
}

FText UERDataLibrary::GetDomainName(EUnitDomain Domain)
{
	switch (Domain)
	{
	case EUnitDomain::Land:	return FText::FromString(TEXT("陆地"));
	case EUnitDomain::Sea:	return FText::FromString(TEXT("海上"));
	case EUnitDomain::Air:	return FText::FromString(TEXT("空中"));
	default:				return FText::FromString(TEXT("未知"));
	}
}
