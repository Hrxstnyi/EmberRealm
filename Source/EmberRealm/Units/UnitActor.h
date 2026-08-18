// Copyright EmberRealm. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/EmberStructs.h"
#include "UnitActor.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

/**
 * 地图上的可视兵棋单位
 * 支持选中高亮、阵营颜色、生命值显示
 */
UCLASS()
class EMBERREALM_API AUnitActor : public AActor
{
	GENERATED_BODY()

public:
	AUnitActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Unit")
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EmberRealm|Unit")
	UWidgetComponent* HealthBarWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EmberRealm|Unit")
	FUnitInstance UnitData;

	/** 设置阵营颜色 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Unit")
	void SetFactionColor(EFactionType Faction);

	/** 更新生命值显示 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Unit")
	void UpdateHealthDisplay();

	/** 选中/取消选中 */
	UFUNCTION(BlueprintCallable, Category = "EmberRealm|Unit")
	void SetSelected(bool bSelected);

protected:
	virtual void BeginPlay() override;

private:
	bool bIsSelected = false;
};
