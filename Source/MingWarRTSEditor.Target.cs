using UnrealBuildTool;
using System.Collections.Generic;

public class MingWarRTSEditorTarget : TargetRules
{
    public MingWarRTSEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("MingWarRTS");
        
        // Override build environment to allow custom settings
        bOverrideBuildEnvironment = true;
    }
}