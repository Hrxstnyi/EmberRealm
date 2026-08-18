// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Containers/Map.h"
#include "Containers/Array.h"

class EMBERREALM_API FEventBus
{
public:
	static FEventBus& Get();

	DECLARE_DELEGATE_OneParam(FEventCallback, const FString&);

	template<typename UserClass, typename... VarTypes>
	void Subscribe(const FString& EventName, UserClass* Object, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type Callback)
	{
	}

	void Broadcast(const FString& EventName, const FString& Payload = TEXT(""));
	void Unsubscribe(const FString& EventName, void* Object);
	void Clear();

private:
	FEventBus() = default;
	~FEventBus() = default;
	FEventBus(const FEventBus&) = delete;
	FEventBus& operator=(const FEventBus&) = delete;

	TMap<FString, TArray<FEventCallback>> Subscribers;
	TArray<TPair<FString, FString>> EventLog;
};

namespace EREvents
{
	static const FString OnTurnStart = TEXT("OnTurnStart");
	static const FString OnTurnEnd = TEXT("OnTurnEnd");
	static const FString OnTimeScaleChanged = TEXT("OnTimeScaleChanged");
	static const FString OnCampaignStart = TEXT("OnCampaignStart");
	static const FString OnCampaignEnd = TEXT("OnCampaignEnd");
	static const FString OnBattleResult = TEXT("OnBattleResult");
	static const FString OnResourceChanged = TEXT("OnResourceChanged");
	static const FString OnTaxCollected = TEXT("OnTaxCollected");
	static const FString OnUnitMoved = TEXT("OnUnitMoved");
	static const FString OnUnitDestroyed = TEXT("OnUnitDestroyed");
	static const FString OnTerritoryChanged = TEXT("OnTerritoryChanged");
	static const FString OnFPSMissionStart = TEXT("OnFPSMissionStart");
	static const FString OnFPSMissionComplete = TEXT("OnFPSMissionComplete");
	static const FString OnFPSMissionFailed = TEXT("OnFPSMissionFailed");
	static const FString OnSaboteurDetected = TEXT("OnSaboteurDetected");
	static const FString OnSaboteurAction = TEXT("OnSaboteurAction");
	static const FString OnVictory = TEXT("OnVictory");
	static const FString OnDefeat = TEXT("OnDefeat");
	static const FString OnSaveGame = TEXT("OnSaveGame");
	static const FString OnLoadGame = TEXT("OnLoadGame");
}
