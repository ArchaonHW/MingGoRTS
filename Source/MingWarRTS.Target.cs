using UnrealBuildTool;
using System.Collections.Generic;

public class MingWarRTSTarget : TargetRules
{
    public MingWarRTSTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add("MingWarRTS");
        
        // Override build environment to allow custom settings
        bOverrideBuildEnvironment = true;
    }
}