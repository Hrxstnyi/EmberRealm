// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/EmberEnums.h"
#include "EmberPlayerController.generated.h"

class UHexGrid;
class AEmberGameMode;

UCLASS()
class EMBERREALM_API AEmberPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEmberPlayerController();

	/** 设置玩家所属阵营 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Player")
	void SetOwningFaction(EFactionType InFaction) { OwningFaction = InFaction; }

	UFUNCTION(BlueprintPure, Category = "EmberRealm|Player")
	EFactionType GetOwningFaction() const { return OwningFaction; }

	/** 选中单位 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Input")
	void SelectUnitAtTile(FIntPoint TileCoord);

	/** 移动选中单位到目标格 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Input")
	void MoveSelectedUnitTo(FIntPoint TargetCoord);

	/** 选中单位攻击目标格 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Input")
	void AttackTargetTile(FIntPoint TargetCoord);

	/** 结束回合 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Input")
	void EndPlayerTurn();

	/** 购买行动牌（2点随机 / 3点指定） */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Cards")
	void BuyActionCard(bool bSpecified, ECardCost MaxCost);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "EmberRealm|Player")
	EFactionType OwningFaction = EFactionType::Garrison;

	UPROPERTY()
	FGuid SelectedUnitId;

	bool bHasSelectedUnit = false;

	AEmberGameMode* GetEmberGameMode() const;
};
