// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StrategyLayer/CombatResolver.h"
#include "UnitActor.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class EMBERREALM_API AUnitActor : public AActor
{
	GENERATED_BODY()

public:
	AUnitActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "单位")
	FCombatUnit CombatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动")
	int32 CurrentTileID = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "移动")
	int32 TargetTileID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "移动")
	int32 MovementRemaining = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "状态")
	bool bHasActed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "组件")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "组件")
	TObjectPtr<UStaticMeshComponent> UnitMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "组件")
	TObjectPtr<UWidgetComponent> InfoWidget;

	UFUNCTION(BlueprintCallable, Category = "单位")
	void InitializeUnit(UERUnitData* UnitData, EFactionType Faction, int32 TileID);

	UFUNCTION(BlueprintCallable, Category = "移动")
	bool MoveToTile(int32 DestinationTileID);

	UFUNCTION(BlueprintCallable, Category = "战斗")
	bool AttackTile(int32 TargetTile);

	UFUNCTION(BlueprintCallable, Category = "回合")
	void OnTurnStart();

	UFUNCTION(BlueprintCallable, Category = "信息")
	FText GetDisplayName() const;

	UFUNCTION(BlueprintCallable, Category = "信息")
	float GetHealthPercent() const { return CombatData.GetStrengthRatio(); }

	UFUNCTION(BlueprintCallable, Category = "信息")
	float GetMoralePercent() const { return CombatData.Morale; }

	UFUNCTION(BlueprintCallable, Category = "移动")
	bool CanMove() const { return !bHasActed && MovementRemaining > 0 && CombatData.CurrentStrength > 0; }

	UFUNCTION(BlueprintCallable, Category = "战斗")
	bool CanAttack() const { return !bHasActed && CombatData.CurrentStrength > 0 && CombatData.CurrentAmmo > 0; }

	UFUNCTION(BlueprintCallable, Category = "状态")
	bool IsRouted() const { return CombatData.IsRouted(); }

protected:
	virtual void BeginPlay() override;

private:
	void UpdateVisuals();
	void PlayMoveAnimation(FVector StartLocation, FVector EndLocation, float Duration);
};
