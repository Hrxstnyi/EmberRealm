// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SaveSystem/ERSaveGame.h"

class EMBERREALM_API FSaveManager
{
public:
	FSaveManager();
	~FSaveManager();

	static FSaveManager& Get();

	bool SaveGame(int32 SlotIndex, const FString& SaveName);
	bool LoadGame(int32 SlotIndex);
	bool DeleteSave(int32 SlotIndex);

	TArray<FString> ListSaves() const;
	bool HasSave(int32 SlotIndex) const;

	FString GetSaveDirectory() const;

	void SetMaxSlots(int32 InMaxSlots) { MaxSlots = InMaxSlots; }
	int32 GetMaxSlots() const { return MaxSlots; }

private:
	int32 MaxSlots = 10;
	static const FString SaveSlotPrefix;

	FString GetSaveFileName(int32 SlotIndex) const;
};
