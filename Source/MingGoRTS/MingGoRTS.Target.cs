using UnrealBuildTool;
using System.Collections.Generic;

public class MingGoRTSTarget : TargetRules
{
    public MingGoRTSTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "MingGoRTS" });
    }
}
