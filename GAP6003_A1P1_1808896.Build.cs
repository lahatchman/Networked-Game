// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAP6003_A1P1_1808896 : ModuleRules
{
	public GAP6003_A1P1_1808896(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
