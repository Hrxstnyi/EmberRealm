// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ERGameInstance.generated.h"

UCLASS()
class EMBERREALM_API UERGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UERGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	bool bAutoSaveEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	int32 AutoSaveIntervalMinutes = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "存档")
	int32 MaxSaveSlots = 10;

	UPROPERTY(BlueprintReadOnly, Category = "阵营")
	int32 SelectedFactionIndex = -1;

	UFUNCTION(BlueprintCallable, Category = "阵营")
	void SetSelectedFaction(int32 FactionIndex);
};
