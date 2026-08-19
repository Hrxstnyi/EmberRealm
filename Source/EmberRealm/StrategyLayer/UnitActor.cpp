// Copyright Epic Games, Inc. All Rights Reserved.

#include "StrategyLayer/UnitActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "StrategyLayer/StrategyMapManager.h"
#include "StrategyLayer/CombatResolver.h"
#include "Core/EventBus.h"
#include "Core/ERGameInstance.h"

AUnitActor::AUnitActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	UnitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitMesh"));
	UnitMesh->SetupAttachment(RootComponent);
	UnitMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoWidget"));
	InfoWidget->SetupAttachment(RootComponent);
	InfoWidget->SetRelativeLocation(FVector(0, 0, 100));
	InfoWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InfoWidget->SetDrawSize(FVector2D(120, 40));
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateVisuals();
}

void AUnitActor::InitializeUnit(UERUnitData* UnitData, EFactionType Faction, int32 TileID)
{
	CombatData.UnitData = UnitData;
	CombatData.Faction = Faction;
	CombatData.CurrentStrength = UnitData ? 100 : 100;
	CombatData.MaxStrength = UnitData ? 100 : 100;
	CombatData.Morale = 80.0f;
	CombatData.Rank = UnitData ? UnitData->RequiredRank : ERankLevel::SecondLieutenant;
	CombatData.CurrentAmmo = UnitData ? UnitData->MaxAmmo : 30;

	CurrentTileID = TileID;
	MovementRemaining = UnitData ? UnitData->Movement : 2;
	bHasActed = false;

	UpdateVisuals();

	UE_LOG(LogTemp, Log, TEXT("[UnitActor] 单位初始化: %s, 阵营: %d, 阵地: %d"),
		UnitData ? *UnitData->UnitName.ToString() : TEXT("Unknown"), (int32)Faction, TileID);
}

bool AUnitActor::MoveToTile(int32 DestinationTileID)
{
	if (!CanMove())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UnitActor] 单位无法移动（已行动或无移动力）"));
		return false;
	}

	UERGameInstance* GI = Cast<UERGameInstance>(GetGameInstance());
	if (!GI) return false;

	const bool bHasAirborne = CombatData.UnitData && CombatData.UnitData->UnitType == EUnitType::Paratrooper;
	if (!GI->GetStrategyMapManager().CanMoveTo(CurrentTileID, DestinationTileID, bHasAirborne))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UnitActor] 无法移动到阵地 %d（不相邻或海拔差过大）"), DestinationTileID);
		return false;
	}

	const int32 MoveCost = GI->GetStrategyMapManager().CalculateMoveCost(CurrentTileID, DestinationTileID, bHasAirborne);
	if (MoveCost > MovementRemaining)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UnitActor] 移动力不足，需要 %d，剩余 %d"), MoveCost, MovementRemaining);
		return false;
	}

	TargetTileID = DestinationTileID;
	MovementRemaining -= MoveCost;

	FEventBus::Get().Broadcast(EREvents::OnUnitMoved,
		FString::Printf(TEXT("{\"unit\":%d,\"from\":%d,\"to\":%d}"), CombatData.UnitID, CurrentTileID, DestinationTileID));

	UE_LOG(LogTemp, Log, TEXT("[UnitActor] 单位从 %d 移动到 %d，消耗 %d 移动力"),
		CurrentTileID, DestinationTileID, MoveCost);

	CurrentTileID = DestinationTileID;
	TargetTileID = -1;
	return true;
}

bool AUnitActor::AttackTile(int32 TargetTile)
{
	if (!CanAttack())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UnitActor] 单位无法攻击"));
		return false;
	}

	UERGameInstance* GI = Cast<UERGameInstance>(GetGameInstance());
	if (!GI) return false;

	bHasActed = true;
	UE_LOG(LogTemp, Log, TEXT("[UnitActor] 单位攻击阵地 %d"), TargetTile);
	return true;
}

void AUnitActor::OnTurnStart()
{
	bHasActed = false;
	if (CombatData.UnitData)
	{
		MovementRemaining = CombatData.UnitData->Movement;
	}

	if (CombatData.UnitData)
	{
		CombatData.CurrentAmmo = FMath::Min(CombatData.UnitData->MaxAmmo,
			CombatData.CurrentAmmo + FMath::RoundToInt(CombatData.UnitData->MaxAmmo * 0.3f));
	}

	CombatData.Morale = FMath::Clamp(CombatData.Morale + 5.0f, 0.0f, 100.0f);
	UpdateVisuals();
}

FText AUnitActor::GetDisplayName() const
{
	if (CombatData.UnitData)
	{
		return CombatData.UnitData->UnitName;
	}
	return FText::FromString(TEXT("未知单位"));
}

void AUnitActor::UpdateVisuals()
{
	if (!UnitMesh) return;

	FLinearColor FactionColor = FLinearColor::Gray;
	switch (CombatData.Faction)
	{
	case EFactionType::GarrisonArmy:	FactionColor = FLinearColor(0.2f, 0.4f, 0.8f); break;
	case EFactionType::ContractCorps:	FactionColor = FLinearColor(0.8f, 0.4f, 0.1f); break;
	case EFactionType::AutonomyLeague:	FactionColor = FLinearColor(0.2f, 0.7f, 0.3f); break;
	default:						FactionColor = FLinearColor::Gray; break;
	}

	UnitMesh->SetVisibility(CombatData.CurrentStrength > 0);

	const float Scale = 0.5f + CombatData.GetStrengthRatio() * 0.5f;
	UnitMesh->SetRelativeScale3D(FVector(Scale));
}

void AUnitActor::PlayMoveAnimation(FVector StartLocation, FVector EndLocation, float Duration)
{
	SetActorLocation(EndLocation);
}
