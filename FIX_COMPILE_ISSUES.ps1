# MingGoRTS 編譯問題自動修復腳本
# 專業級解決方案 - PowerShell 版本

param(
    [switch]$Clean,
    [switch]$Rebuild,
    [switch]$Minimal,
    [switch]$Verbose
)

Write-Host "🎯 MingGoRTS 編譯問題修復工具" -ForegroundColor Green
Write-Host "======================================" -ForegroundColor Green

# 設定變數
$ProjectRoot = "C:\HW\MingGoRTS"
$EnginePath = "C:\Program Files\Epic Games\UE_5.7"
$UBTPath = "$EnginePath\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$ProjectFile = "$ProjectRoot\MingGoRTS.uproject"

function Write-Status {
    param($Message, $Status = "INFO")
    $Color = switch ($Status) {
        "SUCCESS" { "Green" }
        "ERROR" { "Red" }
        "WARNING" { "Yellow" }
        "INFO" { "Cyan" }
        default { "White" }
    }
    Write-Host $Message -ForegroundColor $Color
}

function Test-Environment {
    Write-Status "🔍 檢查系統環境..." "INFO"
    
    # 檢查 UE5 安裝
    if (-not (Test-Path $UBTPath)) {
        Write-Status "❌ UnrealBuildTool 未找到" "ERROR"
        return $false
    }
    Write-Status "✅ UnrealBuildTool: $UBTPath" "SUCCESS"
    
    # 檢查專案檔案
    if (-not (Test-Path $ProjectFile)) {
        Write-Status "❌ 專案檔案未找到" "ERROR"
        return $false
    }
    Write-Status "✅ 專案檔案: $ProjectFile" "SUCCESS"
    
    # 檢查 Windows SDK
    $SDKVersion = Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\Windows Kits\Installed Roots" -Name "KitsRoot10" | Select-Object -ExpandProperty Name
    Write-Status "✅ Windows SDK: $SDKVersion" "SUCCESS"
    
    return $true
}

function Clean-Project {
    Write-Status "🧹 清理專案..." "INFO"
    
    $PathsToClean = @(
        "$ProjectRoot\Intermediate",
        "$ProjectRoot\Binaries",
        "$ProjectRoot\Saved",
        "$ProjectRoot\.vs",
        "$ProjectRoot\DerivedDataCache"
    )
    
    foreach ($Path in $PathsToClean) {
        if (Test-Path $Path) {
            Write-Status "  清理: $Path" "INFO"
            Remove-Item -Path $Path -Recurse -Force -ErrorAction SilentlyContinue
        }
    }
}

function Rebuild-Project {
    Write-Status "🔧 重新生成專案檔案..." "INFO"
    
    $Args = @(
        "-projectfiles",
        "-project=`"$ProjectFile`"",
        "-game",
        "-progress"
    )
    
    if ($Verbose) {
        $Args += "-verbose"
    }
    
    Write-Status "  執行: $UBTPath $Args" "INFO"
    
    $Process = Start-Process -FilePath $UBTPath -ArgumentList $Args -Wait -PassThru
    
    if ($Process.ExitCode -eq 0) {
        Write-Status "✅ 專案檔案生成成功" "SUCCESS"
    } else {
        Write-Status "❌ 專案檔案生成失敗 (退出代碼: $($Process.ExitCode))" "ERROR"
        return $false
    }
    
    return $true
}

function Test-MinimalProject {
    Write-Status "🧪 創建最小測試專案..." "INFO"
    
    $MinimalProjectPath = "$ProjectRoot\MinimalTest"
    
    if (Test-Path $MinimalProjectPath) {
        Write-Status "  清理現有的最小專案" "INFO"
        Remove-Item -Path $MinimalProjectPath -Recurse -Force -ErrorAction SilentlyContinue
    }
    
    # 創建最小專案結構
    New-Item -ItemType Directory -Path "$MinimalProjectPath\Source\MinimalTest" -Force | Out-Null
    New-Item -ItemType Directory -Path "$MinimalProjectPath\Source\MinimalTest\Public" -Force | Out-Null
    New-Item -ItemType Directory -Path "$MinimalProjectPath\Source\MinimalTest\Private" -Force | Out-Null
    
    # 創建基本檔案
    $MinimalUProject = @"
{
    "FileVersion": 3,
    "EngineAssociation": "5.7",
    "Category": "",
    "Description": "Minimal Test Project",
    "Modules": [
        {
            "Name": "MinimalTest",
            "Type": "Runtime",
            "LoadingPhase": "Default"
        }
    ]
}
"@
    
    $MinimalTarget = @"
using UnrealBuildTool;

public class MinimalTestTarget : TargetRules
{
    public MinimalTestTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.AddRange(new string[] { "MinimalTest" });
    }
}
"@
    
    $MinimalModule = @"
using UnrealBuildTool;

public class MinimalTest : ModuleRules
{
    public MinimalTest(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );
    }
}
"@
    
    $MinimalHeader = @"
#pragma once

#include "CoreMinimal.h"

UCLASS(BlueprintType)
class MINIMALTEST_API UMinimalTestGame : public UObject
{
    GENERATED_BODY()

public:
    UMinimalTestGame();
    
    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestFunction();
};
"@
    
    $MinimalCPP = @"
#include "../Public/MinimalTest.h"

UMinimalTestGame::UMinimalTestGame()
{
}

void UMinimalTestGame::TestFunction()
{
    // 測試函數
}
"@
    
    # 寫入檔案
    $MinimalUProject | Out-File -FilePath "$MinimalProjectPath\MinimalTest.uproject" -Encoding UTF8
    $MinimalTarget | Out-File -FilePath "$MinimalProjectPath\Source\MinimalTest\MinimalTest.Target.cs" -Encoding UTF8
    $MinimalModule | Out-File -FilePath "$MinimalProjectPath\Source\MinimalTest\MinimalTest.Build.cs" -Encoding UTF8
    $MinimalHeader | Out-File -FilePath "$MinimalProjectPath\Source\MinimalTest\Public\MinimalTest.h" -Encoding UTF8
    $MinimalCPP | Out-File -FilePath "$MinimalProjectPath\Source\MinimalTest\Private\MinimalTest.cpp" -Encoding UTF8
    
    Write-Status "✅ 最小專案創建完成: $MinimalProjectPath" "SUCCESS"
    
    # 編譯最小專案
    $Args = @(
        "MinimalTestEditor",
        "Win64",
        "Development",
        "-project=`"$MinimalProjectPath\MinimalTest.uproject`"",
        "-progress"
    )
    
    Write-Status "  編譯最小專案..." "INFO"
    $Process = Start-Process -FilePath $UBTPath -ArgumentList $Args -Wait -PassThru
    
    if ($Process.ExitCode -eq 0) {
        Write-Status "✅ 最小專案編譯成功" "SUCCESS"
        return $true
    } else {
        Write-Status "❌ 最小專案編譯失敗 (退出代碼: $($Process.ExitCode))" "ERROR"
        return $false
    }
}

function Test-OriginalProject {
    Write-Status "🎮 測試原始專案編譯..." "INFO"
    
    $Args = @(
        "MingGoRTSEditor",
        "Win64",
        "Development",
        "-project=`"$ProjectFile`"",
        "-progress"
    )
    
    if ($Verbose) {
        $Args += "-verbose"
    }
    
    Write-Status "  執行: $UBTPath $Args" "INFO"
    
    $Process = Start-Process -FilePath $UBTPath -ArgumentList $Args -Wait -PassThru
    
    if ($Process.ExitCode -eq 0) {
        Write-Status "✅ 原始專案編譯成功" "SUCCESS"
        return $true
    } else {
        Write-Status "❌ 原始專案編譯失敗 (退出代碼: $($Process.ExitCode))" "ERROR"
        return $false
    }
}

# 主要執行流程
try {
    Write-Status "🚀 開始執行修復流程..." "INFO"
    
    # 環境檢查
    if (-not (Test-Environment)) {
        Write-Status "❌ 環境檢查失敗，終止執行" "ERROR"
        exit 1
    }
    
    # 清理階段
    if ($Clean) {
        Clean-Project
    }
    
    # 重新生成階段
    if ($Rebuild) {
        if (-not (Rebuild-Project)) {
            Write-Status "❌ 專案重新生成失敗，終止執行" "ERROR"
            exit 1
        }
    }
    
    # 最小測試階段
    if ($Minimal) {
        if (-not (Test-MinimalProject)) {
            Write-Status "❌ 最小專案測試失敗" "ERROR"
            exit 1
        }
    }
    
    # 原始專案測試
    if (-not $Clean -and -not $Rebuild -and -not $Minimal) {
        if (-not (Test-OriginalProject)) {
            Write-Status "❌ 原始專案測試失敗" "ERROR"
            exit 1
        }
    }
    
    Write-Status "🎉 所有步驟執行完成！" "SUCCESS"
    
} catch {
    Write-Status "❌ 執行過程中發生錯誤: $($_.Exception.Message)" "ERROR"
    exit 1
}
