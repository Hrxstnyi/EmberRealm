// Copyright Epic Games, Inc. All Rights Reserved.

#include "EmberRealm.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FEmberRealmModule, EmberRealm, "EmberRealm");

void FEmberRealmModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 烬域：卡拉比亚战区 模块启动"));
}

void FEmberRealmModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 烬域：卡拉比亚战区 模块关闭"));
}
