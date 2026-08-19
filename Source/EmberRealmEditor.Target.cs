// Copyright Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.Collections.Generic;

public class EmberRealmEditorTarget : TargetRules
{
	public EmberRealmEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		BuildEnvironment = TargetBuildEnvironment.Unique;
		ExtraModuleNames.Add("EmberRealm");
	}
}
