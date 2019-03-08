
using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.All)]
public class UE4LauncherTarget : TargetRules
{
	public UE4LauncherTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Program;
		LinkType = TargetLinkType.Monolithic;
		LaunchModuleName = "UE4Launcher";
        ExtraModuleNames.Add("EditorStyle");
    }

    public override void SetupGlobalEnvironment(
        TargetInfo Target,
        ref LinkEnvironmentConfiguration OutLinkEnvironmentConfiguration,
        ref CPPEnvironmentConfiguration OutCPPEnvironmentConfiguration
        )
    {
        // Lean and mean
        bCompileLeanAndMeanUE = true;

        // No editor or editor-only data is needed
        bBuildEditor = false;
        //bBuildWithEditorOnlyData = false;

        // Compile out references from Core to the rest of the engine
        bCompileAgainstEngine = false;
        bCompileAgainstCoreUObject = true;

        // If ture the program entrance is WinMain,otherwise entrance is main
        bIsBuildingConsoleApplication = false;
    }
}

