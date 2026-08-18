// Copyright Epic Games, Inc. All Rights Reserved.

#include "SaveSystem/SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/EventBus.h"

const FString FSaveManager::SaveSlotPrefix = TEXT("EmberRealm_Save_");

FSaveManager::FSaveManager()
{
	UE_LOG(LogTemp, Log, TEXT("[SaveManager] 存档管理器创建"));
}

FSaveManager::~FSaveManager()
{
}

FSaveManager& FSaveManager::Get()
{
	static FSaveManager Instance;
	return Instance;
}

bool FSaveManager::SaveGame(int32 SlotIndex, const FString& SaveName)
{
	if (SlotIndex < 0 || SlotIndex >= MaxSlots)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveManager] 无效的存档槽位: %d"), SlotIndex);
		return false;
	}

	FString SlotName = GetSaveFileName(SlotIndex);

	UERSaveGame* SaveGameInstance = Cast<UERSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UERSaveGame::StaticClass()));

	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveManager] 创建存档对象失败"));
		return false;
	}

	SaveGameInstance->SaveName = SaveName;
	SaveGameInstance->SaveTimestamp = FDateTime::Now();

	bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("[SaveManager] 存档成功: 槽位%d, 名称:%s"), SlotIndex, *SaveName);
		FEventBus::Get().Broadcast(EREvents::OnSaveGame,
			FString::Printf(TEXT("{\"slot\":%d,\"name\":\"%s\"}"), SlotIndex, *SaveName));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveManager] 存档失败: 槽位%d"), SlotIndex);
	}

	return bSuccess;
}

bool FSaveManager::LoadGame(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= MaxSlots)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveManager] 无效的存档槽位: %d"), SlotIndex);
		return false;
	}

	FString SlotName = GetSaveFileName(SlotIndex);

	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("[SaveManager] 槽位%d无存档"), SlotIndex);
		return false;
	}

	UERSaveGame* LoadedGame = Cast<UERSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	if (!LoadedGame)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveManager] 加载存档失败: 槽位%d"), SlotIndex);
		return false;
	}

	if (LoadedGame->GameVersion != TEXT("0.1.0"))
	{
		UE_LOG(LogTemp, Warning, TEXT("[SaveManager] 存档版本不兼容: %s"), *LoadedGame->GameVersion);
	}

	UE_LOG(LogTemp, Log, TEXT("[SaveManager] 加载存档成功: 槽位%d, 名称:%s, 时间:%s"),
		SlotIndex, *LoadedGame->SaveName, *LoadedGame->SaveTimestamp.ToString());

	FEventBus::Get().Broadcast(EREvents::OnLoadGame,
		FString::Printf(TEXT("{\"slot\":%d,\"name\":\"%s\"}"), SlotIndex, *LoadedGame->SaveName));

	return true;
}

bool FSaveManager::DeleteSave(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= MaxSlots) return false;

	FString SlotName = GetSaveFileName(SlotIndex);
	bool bSuccess = UGameplayStatics::DeleteGameInSlot(SlotName, 0);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("[SaveManager] 删除存档: 槽位%d"), SlotIndex);
	}

	return bSuccess;
}

TArray<FString> FSaveManager::ListSaves() const
{
	TArray<FString> SaveList;
	for (int32 i = 0; i < MaxSlots; i++)
	{
		FString SlotName = GetSaveFileName(i);
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			SaveList.Add(FString::Printf(TEXT("槽位%d: %s"), i, *SlotName));
		}
	}
	return SaveList;
}

bool FSaveManager::HasSave(int32 SlotIndex) const
{
	if (SlotIndex < 0 || SlotIndex >= MaxSlots) return false;
	return UGameplayStatics::DoesSaveGameExist(GetSaveFileName(SlotIndex), 0);
}

FString FSaveManager::GetSaveDirectory() const
{
	return FPaths::ProjectSavedDir() / TEXT("SaveGames");
}

FString FSaveManager::GetSaveFileName(int32 SlotIndex) const
{
	return FString::Printf(TEXT("%s%d"), *SaveSlotPrefix, SlotIndex);
}
