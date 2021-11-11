
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UE4Launcher : ModuleRules
{
	public UE4Launcher(ReadOnlyTargetRules Target) : base(Target)
	{
        PublicIncludePaths.AddRange(
            new string[]
            {
                "Runtime/Launch/Public",
                "Programs/UE4Launcher/Source/Public",
				"Runtime/Core/Public/Containers",
                "Runtime/Core/Public/Misc"
            }); 
        PrivateIncludePaths.AddRange(
            new string[]
            {
                "Runtime/Launch/Private",  // For LaunchEngineLoop.cpp include
                "Programs/UE4Launcher/Source/Private"
            });

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
                "SourceCodeAccess",
				"OpenGL"
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
        PublicDefinitions.Add("LAUNCHER_VERSION=\"v0.21\"");
    }
}

