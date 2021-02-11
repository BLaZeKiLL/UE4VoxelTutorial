// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;

using UnrealBuildTool;

public class VloxyEngine : ModuleRules
{
	public VloxyEngine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new[] {
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new[] {
			"ProceduralMeshComponent", 
			"FastNoise", 
			"FastNoiseGenerator"
		});

		// PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
