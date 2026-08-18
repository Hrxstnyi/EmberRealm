using UnrealBuildTool;

public class EmberRealm : ModuleRules
{
	public EmberRealm(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"UMG", "Slate", "SlateCore",
			"EnhancedInput", "NavigationSystem", "AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] { "EmberRealm" });
	}
}
