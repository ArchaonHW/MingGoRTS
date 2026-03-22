using UnrealBuildTool;
using System.Collections.Generic;

public class MingCoreTarget : TargetRules
{
    public MingCoreTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Runtime;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "MingCore" });
    }
}
