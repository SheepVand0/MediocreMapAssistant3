// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMA3 : ModuleRules
{
	public MMA3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "VaRest", 
			"RuntimeAudioImporter", "ProceduralMeshComponent", "Slate", "SlateCore", "UMG", "VorbisAudioDecoder" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Kiss_FFT" });
        PublicDependencyModuleNames.Add("Kiss_FFT");

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
