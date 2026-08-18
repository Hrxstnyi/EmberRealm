// Copyright EmberRealm. All Rights Reserved.
#include "EmberRealm.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FEmberRealmModule, EmberRealm, "EmberRealm");

void FEmberRealmModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 烬域：卡拉比亚战区 模块已启动"));
}

void FEmberRealmModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("[EmberRealm] 烬域模块已关闭"));
}
