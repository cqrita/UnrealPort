// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealP : ModuleRules
{
	public UnrealP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities" , "GameplayTags" });
        PrivateDependencyModuleNames.AddRange(new string[] {"GameplayTasks", "NavigationSystem", "Niagara", "AIModule" , "AnimGraphRuntime", "FieldSystemEngine" });

    }
}
