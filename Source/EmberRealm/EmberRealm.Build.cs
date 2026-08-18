// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmberRealm : ModuleRules
{
	public EmberRealm(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"GameplayTasks",
			"NavigationSystem",
			"AIModule",
			"Niagara",
			"PhysicsCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"RenderCore",
			"RHI"
		});

		// 游戏模块子目录
		PublicIncludePaths.AddRange(new string[]
		{
			"EmberRealm/Core",
			"EmberRealm/StrategyLayer",
			"EmberRealm/FPSLayer",
			"EmberRealm/DataSystem",
			"EmberRealm/SaveSystem",
			"EmberRealm/AISystem",
			"EmberRealm/Network"
		});

		// 启用UE5标准
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;

		// 警告处理
		bEnableExceptions = true;
	}
}
