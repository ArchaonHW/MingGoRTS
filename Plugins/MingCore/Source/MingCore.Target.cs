using UnrealBuildTool;
using System.Collections.Generic;

public class MingCoreTarget : TargetRules
{
    public MingCoreTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        Linker = TargetLinker.LLD;
        
        CppStandard = CppStandardVersion.Cpp20;
        
        // 啟用優化以支持1000+單位性能
                bUseRTTI = false;
                bEnableExceptions = false;
                bUseUnity = false;
                bEnableUndefinedIdentifierWarnings = true;
                bOmitFramePointers = false;
        }
        
        public override void SetupBinaries(
            TargetInfo Target,
            List<UEBuildBinary> Binaries,
            Dictionary<string, List<UEBuildBinary>> BinaryModules)
        {
            // 添加模組二進制文件
            Binaries.Add(new UEBuildBinary(UEBuildBinaryType.Executable, Target.Platform, Target.Configuration, Target.Architecture, "MingCoreEditor"));
        }

        public override bool ShouldUseUnityBuild(UEBuildBinary Binary)
        {
            return false; // MingCore需要精細控制編譯以確保性能
        }
}
