// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniDrivingSimulator : ModuleRules
{
	public MiniDrivingSimulator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"ChaosVehicles",
			"PhysicsCore",
			"UMG",
			"Slate",
			"Sockets",
			"Networking"
		});

		PublicIncludePaths.AddRange(new string[] {
			"MiniDrivingSimulator",
			"MiniDrivingSimulator/SportsCar",
			"MiniDrivingSimulator/OffroadCar",
			"MiniDrivingSimulator/Variant_Offroad",
			"MiniDrivingSimulator/Variant_TimeTrial",
			"MiniDrivingSimulator/Variant_TimeTrial/UI"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
