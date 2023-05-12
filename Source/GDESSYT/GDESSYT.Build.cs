// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GDESSYT : ModuleRules
{
	public GDESSYT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameplayCameras", "GameplayCameras" });
	}
}
