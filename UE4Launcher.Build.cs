
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4Launcher : ModuleRules
{
	public UE4Launcher(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.Add("Runtime/Launch/Public");

		PrivateIncludePaths.Add("Runtime/Launch/Private");		// For LaunchEngineLoop.cpp include
		PrivateIncludePaths.Add("Programs/UE4Launcher/Source");
        PrivateIncludePaths.Add("Programs/UE4Launcher/Source/Public");
        PrivateIncludePaths.Add("Programs/UE4Launcher/Source/Private");

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "AppFramework",
                "Core",
                "ApplicationCore",
                "Projects",
                "DesktopPlatform",
                "Slate",
                "SlateCore",
                "InputCore",
                "SlateReflector",
                "StandaloneRenderer",
                "WebBrowser",
                "SourceCodeAccess"
            }
        );
        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "SlateReflector",
            }
        );
        PublicIncludePathModuleNames.AddRange(
            new string[] {
                "SlateReflector",
            }
        );
        //DynamicallyLoadedModuleNames.AddRange(
        //    new string[] {
        //        "SlateReflector",
        //    }
        //);
    }
}

