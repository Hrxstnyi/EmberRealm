// Copyright EmberRealm. All Rights Reserved.
#include "EmberPlayerController.h"
#include "EmberGameMode.h"
#include "Map/HexGrid.h"
#include "Factions/FactionManager.h"
#include "Combat/CombatSystem.h"

AEmberPlayerController::AEmberPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AEmberPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AEmberPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 输入绑定可在蓝图或Enhanced Input中扩展
}

void AEmberPlayerController::SelectUnitAtTile(FIntPoint TileCoord)
{
	AEmberGameMode* GM = GetEmberGameMode();
	if (!GM || !GM->FactionManager) return;

	FFactionState State = GM->FactionManager->GetFactionState(OwningFaction);
	for (const FUnitInstance& Unit : State.Units)
	{
		if (Unit.TileCoord == TileCoord)
		{
			SelectedUnitId = Unit.InstanceId;
			bHasSelectedUnit = true;
			UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 选中单位 %s"), *Unit.InstanceId.ToString());
			return;
		}
	}
	bHasSelectedUnit = false;
}

void AEmberPlayerController::MoveSelectedUnitTo(FIntPoint TargetCoord)
{
	if (!bHasSelectedUnit) return;
	AEmberGameMode* GM = GetEmberGameMode();
	if (!GM || !GM->FactionManager || !GM->HexGrid) return;

	// 规则10.3：行军完毕本回合不能开火
	GM->FactionManager->MoveUnit(OwningFaction, SelectedUnitId, TargetCoord, GM->HexGrid);
}

void AEmberPlayerController::AttackTargetTile(FIntPoint TargetCoord)
{
	if (!bHasSelectedUnit) return;
	AEmberGameMode* GM = GetEmberGameMode();
	if (!GM || !GM->CombatSystem || !GM->FactionManager) return;

	GM->CombatSystem->ResolveAttack(GM->FactionManager, OwningFaction, SelectedUnitId, TargetCoord);
}

void AEmberPlayerController::EndPlayerTurn()
{
	AEmberGameMode* GM = GetEmberGameMode();
	if (GM) GM->EndTurn();
}

void AEmberPlayerController::BuyActionCard(bool bSpecified, ECardCost MaxCost)
{
	AEmberGameMode* GM = GetEmberGameMode();
	if (!GM || !GM->FactionManager) return;

	int32 Cost = bSpecified ? 3 : 2;
	GM->FactionManager->SpendActionPoints(OwningFaction, Cost);
	// 卡牌抽取逻辑待扩展
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 购买行动牌，花费 %d 行动点"), Cost);
}

AEmberGameMode* AEmberPlayerController::GetEmberGameMode() const
{
	return Cast<AEmberGameMode>(GetWorld()->GetAuthGameMode());
}
