using UnrealBuildTool;
using System.Collections.Generic;

public class MingCoreTarget : TargetRules
{
    public MingCoreTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Runtime;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        CppStandard = CppStandardVersion.Cpp20;
        
        // 使用 UE 5.7 默認設置，但不使用 Unique Build Environment
        ExtraModuleNames.AddRange(new string[] { "MingCore" });
    }
}
