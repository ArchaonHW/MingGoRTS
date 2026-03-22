using UnrealBuildTool;
using System.Collections.Generic;

public class MingCoreEditorTarget : TargetRules
{
    public MingCoreEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "MingCore" });
    }
}
