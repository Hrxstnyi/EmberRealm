// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataSystem/ERUnitData.h"
#include "DataSystem/ERFactionData.h"
#include "ERDataLibrary.generated.h"

UCLASS()
class EMBERREALM_API UERDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "烬域|数据")
	static TArray<UERUnitData*> CreateDefaultUnitData(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "烬域|数据")
	static TArray<UERFactionData*> CreateDefaultFactionData(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "烬域|数据")
	static FText GetUnitTypeName(EUnitType Type);

	UFUNCTION(BlueprintCallable, Category = "烬域|数据")
	static FText GetFactionName(EFactionType Faction);

	UFUNCTION(BlueprintCallable, Category = "烬域|数据")
	static FText GetDomainName(EUnitDomain Domain);
};
