// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GravityGunTest : ModuleRules
{
	public GravityGunTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG", });
	}
}
