using UnrealBuildTool;
using System.Collections.Generic;

public class MingGoRTSTarget : TargetRules
{
    public MingGoRTSTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        CppStandard = CppStandardVersion.Cpp20;

        // Use UE 5.7 default settings, but not using Unique Build Environment settings
        ExtraModuleNames.AddRange(new string[] { "MingGoRTS" });
    }
}