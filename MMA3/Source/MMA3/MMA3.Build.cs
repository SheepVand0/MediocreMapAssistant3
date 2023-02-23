// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMA3 : ModuleRules
{
	public MMA3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore", "RuntimeAudioImporter", "VaRest", "ProceduralMeshComponent" });

        PrivateDependencyModuleNames.AddRange(new string[] { "VaRest", "RuntimeAudioImporter" });

        //PublicIncludePaths.AddRange(new string[] { "VaRest/Public", "VaRest/Classes" });
        //PublicIncludePaths.AddRange(new string[] { "RuntimeAudioImporter/Public", "RuntimeAudioImporter/Classes" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
