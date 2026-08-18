// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * 烬域：卡拉比亚战区 - 主游戏模块
 * 
 * 核心系统入口，负责初始化全局游戏框架。
 * 模块划分：
 *  - Core:         游戏时钟、回合管理、事件系统
 *  - StrategyLayer: 战略层4X玩法
 *  - FPSLayer:     第一人称战术层
 *  - DataSystem:   数据驱动配置
 *  - SaveSystem:   存档系统
 *  - AISystem:     AI与内鬼系统
 *  - Network:      多人联机
 */
class FEmberRealmModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
