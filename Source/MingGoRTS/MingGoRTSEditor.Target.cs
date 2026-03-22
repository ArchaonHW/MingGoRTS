using UnrealBuildTool;
using System.Collections.Generic;

public class MingGoRTSEditorTarget : TargetRules
{
    public MingGoRTSEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        CppStandard = CppStandardVersion.Cpp20;
        
        // 使用 UE 5.7 默認設置，但不使用 Unique Build Environment
        ExtraModuleNames.AddRange(new string[] { "MingGoRTS" });
    }
}
