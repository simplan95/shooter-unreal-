// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Shooter2 : ModuleRules
{
	public Shooter2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
															"UMG","AIModule","NavigationSystem","GameplayTasks" });//UMG, AIModule, NavigationSystem GameplayTasks

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

	}
}
