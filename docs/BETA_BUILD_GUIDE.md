# MingGoRTS Beta版本發布構建指南

## 📋 構建概述

本文檔詳細介紹了MingGoRTS Beta版本的發布構建流程，包括準備步驟、構建配置、測試驗證和發布準備。

---

## 🎯 構建目標

### 發布版本信息
- **版本號**: v1.0.0 Beta
- **構建類型**: Release
- **目標平台**: Windows 10/11 (x64)
- **發布日期**: 2026-04-15
- **構建ID**: BETA-1.0.0-20260415

### 質量標準
- **功能完整性**: 100%
- **性能基準**: 全部達標
- **穩定性**: 24小時無崩潰
- **兼容性**: 支持目標平台

---

## 🔧 構建環境準備

### 1. 開發環境要求

#### 系統要求
- **操作系統**: Windows 11 64位
- **Visual Studio**: 2022 (17.0+)
- **Windows SDK**: 10.0.22000.0+
- **.NET Framework**: 4.8+
- **Git**: 2.30+

#### UE5環境
- **Unreal Engine**: 5.3.2
- **插件版本**: 與引擎版本匹配
- **平台SDK**: Windows SDK 10.0
- **編譯器**: MSVC 2022

#### 硬件要求
- **CPU**: Intel i7-8700K 或 AMD Ryzen 7 3700X
- **內存**: 16GB RAM
- **存儲**: 500GB SSD
- **顯卡**: NVIDIA RTX 2070 或 AMD RX 5700 XT

### 2. 工具和依賴

#### 必需工具
- **Visual Studio 2022** - 主要開發環境
- **Unreal Engine 5.3.2** - 遊戲引擎
- **Git** - 版本控制
- **7-Zip** - 文件壓縮
- **Inno Setup** - 安裝程序製作

#### 可選工具
- **Intel VTune Profiler** - 性能分析
- **NVIDIA Nsight** - GPU調試
- **Wireshark** - 網絡分析
- **Process Monitor** - 系統監控

---

## 📦 構建配置

### 1. 項目配置

#### Build.cs配置
```csharp
// MingGoRTS.Build.cs
public class MingGoRTS : ModuleRules
{
    public MingGoRTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore",
            "NetCore",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "MassSignal",
            "MassEntity",
            "MassGameplay"
        });
        
        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "Slate",
            "SlateCore",
            "ToolMenus",
            "EditorStyleSet",
            "EditorWidgets",
            "UnrealEd",
            "LevelEditor",
            "SettingsEditor",
            "PropertyEditor",
            "DetailCustomizations",
            "ClassViewer",
            "OutputLog",
            "SourceControl",
            "SourceControlWindows",
            "Documentation",
            "GameProjectGeneration"
        });
        
        // 優化設置
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;
        bUseRTTI = true;
        bEnableExceptions = true;
        
        // 發布配置
        if (Target.bBuildEditor)
        {
            Definitions.Add("WITH_EDITOR=1");
        }
        else
        {
            Definitions.Add("WITH_EDITOR=0");
        }
    }
}
```

#### Target.cs配置
```csharp
// MingGoRTSTarget.cs
public class MingGoRTSTarget : TargetRules
{
    public MingGoRTSTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        
        // 發布配置
            bUseInliningInShipping = true;
            bOmitFramePointersInShipping = true;
            bCreateChangelog = false;
            bGenerateProjectFiles = true;
            bGenerateManifest = false;
            
            // 優化設置
            bUsePCHInShipping = true;
            bUseSharedPCHInShipping = true;
            bUseOpaqueCompileTimeClassTypeInfo = true;
            bUsePrecompiled = true;
            
            // 調試設置
            bUseDebugInfoInShipping = false;
            bDebugBuildsActuallyUseDebugCRT = false;
            bOverrideBuildEnvironment = false;
            bUseFastPDBLinking = true;
            
            // 運行時設置
            bEnableRuntimeMemoryDebugging = false;
            bEnableLoggingInShipping = false;
            bUseCheckedBuilds = false;
            
            // 平台特定設置
            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                bForceEnableExceptions = true;
                bEnableCPPExceptions = true;
                bUseAVX = true;
                bUseAVX2 = true;
            }
    }
}
```

### 2. 編譯配置

#### 編譯器設置
```xml
<!-- MingGoRTS.uproject -->
<?xml version="1.0" encoding="utf-8"?>
<ProjectFile>
    <ProjectFileVersion>1</ProjectFileVersion>
    <Configurations>
        <Configuration Name="Debug" Default="true">
            <CompilerSettings>
                <Optimization>Disabled</Optimization>
                <PreprocessorDefinitions>DEBUG;_DEBUG</PreprocessorDefinitions>
                <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>
            </CompilerSettings>
        </Configuration>
        <Configuration Name="Development">
            <CompilerSettings>
                <Optimization>Full</Optimization>
                <PreprocessorDefinitions>NDEBUG</PreprocessorDefinitions>
                <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary>
            </CompilerSettings>
        </Configuration>
        <Configuration Name="Shipping">
            <CompilerSettings>
                <Optimization>Full</Optimization>
                <PreprocessorDefinitions>NDEBUG;SHIPPING</PreprocessorDefinitions>
                <RuntimeLibrary>MultiThreaded</RuntimeLibrary>
                <WholeProgramOptimization>true</WholeProgramOptimization>
                <LinkTimeCodeGeneration>true</LinkTimeCodeGeneration>
            </CompilerSettings>
        </Configuration>
    </Configurations>
</ProjectFile>
```

---

## 🚀 構建流程

### 1. 準備階段

#### 代碼準備
```bash
# 1. 檢出最新代碼
git checkout main
git pull origin main

# 2. 檢查代碼狀態
git status
git log --oneline -10

# 3. 創建發布分支
git checkout -b release/beta-v1.0.0

# 4. 更新版本號
# 編輯 Config/DefaultEngine.ini
# 編輯 Source/MingGoRTS/MingGoRTS.Build.cs
# 編輯 Source/MingGoRTS/MingGoRTS.Target.cs
```

#### 清理環境
```bash
# 清理中間文件
if exist "Intermediate" rmdir /s /q "Intermediate"
if exist "Saved" rmdir /s /q "Saved"
if exist "DerivedDataCache" rmdir /s /q "DerivedDataCache"

# 清理編譯緩存
del /f /q "*.sln"
del /f /q "*.vcxproj*"
```

### 2. 編譯階段

#### 編譯命令
```bash
# 1. 生成項目文件
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%CD%\MingGoRTS.uproject" -game -rocket -progress

# 2. 編譯Editor版本
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTSEditor Win64 Development -project="%CD%\MingGoRTS.uproject" -game -rocket -progress

# 3. 編譯Shipping版本
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTS Win64 Shipping -project="%CD%\MingGoRTS.uproject" -game -rocket -progress

# 4. 編譯客戶端版本
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTSClient Win64 Shipping -project="%CD%\MingGoRTS.uproject" -game -rocket -progress
```

#### 編譯腳本
```powershell
# Build-MingGoRTS.ps1
param(
    [string]$Configuration = "Shipping",
    [string]$Platform = "Win64",
    [string]$OutputDir = "BuildOutput"
)

Write-Host "Starting MingGoRTS build..." -ForegroundColor Green
Write-Host "Configuration: $Configuration" -ForegroundColor Yellow
Write-Host "Platform: $Platform" -ForegroundColor Yellow

# 設置環境變量
$env:UE_ROOT = "C:\Program Files\Epic Games\UE_5.7"
$env:PROJECT_PATH = $PWD.Path
$env:OUTPUT_PATH = Join-Path $PWD.Path $OutputDir

# 創建輸出目錄
if (!(Test-Path $env:OUTPUT_PATH)) {
    New-Item -ItemType Directory -Path $env:OUTPUT_PATH -Force
}

# 編譯項目
$BuildTool = Join-Path $env:UE_ROOT "Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$ProjectFile = Join-Path $env:PROJECT_PATH "MingGoRTS.uproject"

$Arguments = @(
    "MingGoRTS",
    $Platform,
    $Configuration,
    "-project=`"$ProjectFile`"",
    "-game",
    "-rocket",
    "-progress",
    "-stdout"
)

Write-Host "Running: $BuildTool $Arguments" -ForegroundColor Cyan
& $BuildTool $Arguments

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build completed successfully!" -ForegroundColor Green
} else {
    Write-Host "Build failed with exit code: $LASTEXITCODE" -ForegroundColor Red
    exit 1
}
```

### 3. 打包階段

#### 打包配置
```ini
# DefaultEngine.ini - 打包設置
[/Script/UnrealEd.ProjectPackagingSettings]
BuildConfiguration=Shipping
BuildProduct=Shipping
BuildVersion=1.0.0
IncludeDebugFiles=false
IncludeEditorContent=false
bBuildAllPlatform=false
bBuildShippingOnly=true
bGenerateCompressed=true
bUseIoStore=true
bUseZenStore=true
bCookAll=false
bCookOnTheFly=false
bMonolithic=true
bSkipShaderCompile=false
NumberOfWorkersToSpawnForShaderCompiling=4
NumberOfWorkersToSpawnForCooking=8
```

#### 打包命令
```bash
# 1. 執行打包
"C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%CD%\MingGoRTS.uproject" -noP4 -platform=Win64 -clientconfig=Shipping -serverconfig=Shipping -cook -allmaps -build -stage -archive -pak -package -distribution -stagingdirectory="%CD%\BuildStaging" -archivedirectory="%CD%\BuildArchive"

# 2. 創建安裝包
"C:\Program Files\Inno Setup 6\ISCC.exe" "Setup\MingGoRTS.iss"

# 3. 創建壓縮包
7z a -tzip "MingGoRTS-Beta-v1.0.0.zip" "BuildArchive\MingGoRTS\WindowsNoEditor\*"
```

---

## 🧪 測試驗證

### 1. 自動化測試

#### 單元測試
```cpp
// 自動化測試腳本
class FBuildTestSuite
{
public:
    // 運行所有測試
    bool RunAllTests()
    {
        bool bAllPassed = true;
        
        // 功能測試
        bAllPassed &= RunFunctionalTests();
        
        // 性能測試
        bAllPassed &= RunPerformanceTests();
        
        // 集成測試
        bAllPassed &= RunIntegrationTests();
        
        // 兼容性測試
        bAllPassed &= RunCompatibilityTests();
        
        return bAllPassed;
    }
    
private:
    bool RunFunctionalTests()
    {
        // 實現功能測試
        return true;
    }
    
    bool RunPerformanceTests()
    {
        // 實現性能測試
        return true;
    }
    
    bool RunIntegrationTests()
    {
        // 實現集成測試
        return true;
    }
    
    bool RunCompatibilityTests()
    {
        // 實現兼容性測試
        return true;
    }
};
```

#### 測試腳本
```powershell
# Test-Build.ps1
param(
    [string]$BuildPath = "BuildArchive\MingGoRTS\WindowsNoEditor"
)

Write-Host "Starting build verification..." -ForegroundColor Green

# 1. 檢查文件完整性
$RequiredFiles = @(
    "MingGoRTS.exe",
    "MingGoRTS.pak",
    "Engine\Binaries\Win64\UE5Editor-Engine.dll",
    "Content\Paks\global.pak"
)

foreach ($File in $RequiredFiles) {
    $FilePath = Join-Path $BuildPath $File
    if (Test-Path $FilePath) {
        Write-Host "✓ $File exists" -ForegroundColor Green
    } else {
        Write-Host "✗ $File missing" -ForegroundColor Red
        exit 1
    }
}

# 2. 運行基本功能測試
Write-Host "Running basic functionality tests..." -ForegroundColor Yellow
& "$BuildPath\MingGoRTS.exe" -log -nosound -windowed -exec "RunBasicTests"

# 3. 檢查性能基準
Write-Host "Checking performance benchmarks..." -ForegroundColor Yellow
& "$BuildPath\MingGoRTS.exe" -log -nosound -windowed -exec "RunPerformanceTests"

# 4. 驗證本地化
Write-Host "Verifying localization..." -ForegroundColor Yellow
& "$BuildPath\MingGoRTS.exe" -log -nosound -windowed -exec "VerifyLocalization"

Write-Host "Build verification completed!" -ForegroundColor Green
```

### 2. 手動測試

#### 測試清單
```markdown
## 測試清單

### 基本功能測試
- [ ] 遊戲啟動正常
- [ ] 主界面顯示正常
- [ ] 單位選擇和移動
- [ ] 建築建設功能
- [ ] 資源管理系統
- [ ] 戰鬥系統功能

### 勢力系統測試
- [ ] 12個勢力可選
- [ ] 勢力特色機制
- [ ] 專屬單位功能
- [ ] 勢力平衡性

### 性能測試
- [ ] 幀率穩定 (≥60 FPS)
- [ ] 內存使用正常 (≤4GB)
- [ ] 加載時間合理 (≤30秒)
- [ ] 長時間運行穩定

### 兼容性測試
- [ ] Windows 10兼容性
- [ ] Windows 11兼容性
- [ ] 不同硬件配置
- [ ] 不同分辨率

### 本地化測試
- [ ] 中文界面正常
- [ ] 英文界面正常
- [ ] 其他語言界面
- [ ] 文本顯示正確
```

---

## 📦 發布準備

### 1. 版本信息

#### 版本號規則
```
格式: MAJOR.MINOR.PATCH[-BUILD]
示例: 1.0.0-BETA

MAJOR: 主版本號 (重大更新)
MINOR: 次版本號 (功能更新)
PATCH: 修復版本號 (Bug修復)
BUILD: 構建標識 (BETA, RC, RELEASE)
```

#### 版本文件
```ini
# DefaultEngine.ini - 版本信息
[/Script/EngineSettings.GameMapsSettings]
GameVersion=1.0.0-BETA
BuildVersion=20260415-001
CompatibleVersion=1.0.0

[/Script/Engine.GeneralProjectSettings]
ProjectID=12345678-1234-1234-1234-123456789012
ProjectName=MingGoRTS
ProjectVersion=1.0.0
Company=MingGoRTS Team
Copyright=Copyright (c) 2026 MingGoRTS. All rights reserved.
```

### 2. 發布文件

#### 文件結構
```
MingGoRTS-Beta-v1.0.0/
├── MingGoRTS.exe                 # 主程序
├── MingGoRTS.pak                 # 遊戲資源包
├── Engine/                       # 引擎文件
│   ├── Binaries/
│   ├── Content/
│   └── ...
├── Content/                      # 遊戲內容
│   ├── Paks/
│   └── ...
├── Documentation/                # 文檔
│   ├── User_Manual.pdf
│   ├── Readme.txt
│   └── ...
├── Redistributables/             # 運行庫
│   ├── DirectX/
│   ├── VisualC++/
│   └── ...
├── Setup.exe                     # 安裝程序
├── Setup.ini                     # 安裝配置
├── License.txt                   # 許可證
└── Version.txt                   # 版本信息
```

#### 安裝程序配置
```ini
; MingGoRTS.iss - Inno Setup配置
[Setup]
AppName=MingGoRTS
AppVersion=1.0.0-BETA
AppPublisher=MingGoRTS Team
AppPublisherURL=https://minggorts.com
AppSupportURL=https://minggorts.com/support
AppUpdatesURL=https://minggorts.com/updates
DefaultDirName={pf}\MingGoRTS
DefaultGroupName=MingGoRTS
OutputDir=Output
OutputBaseFilename=MingGoRTS-Beta-Setup
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin

[Files]
Source: "BuildArchive\MingGoRTS\WindowsNoEditor\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "Documentation\*"; DestDir: "{app}\Documentation"; Flags: ignoreversion
Source: "Redistributables\*"; DestDir: "{app}\Redistributables"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "License.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "Version.txt"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\MingGoRTS"; Filename: "{app}\MingGoRTS.exe"
Name: "{group}\User Manual"; Filename: "{app}\Documentation\User_Manual.pdf"
Name: "{group}\Uninstall MingGoRTS"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\Redistributables\DirectX\DXSETUP.exe"; Parameters: "/silent"; Flags: runhidden
Filename: "{app}\Redistributables\VisualC++\vcredist_x64.exe"; Parameters: "/quiet"; Flags: runhidden
```

### 3. 發布檢查清單

#### 技術檢查
- [ ] 編譯無錯誤
- [ ] 打包完整性
- [ ] 功能測試通過
- [ ] 性能基準達標
- [ ] 兼容性測試通過
- [ ] 本地化測試通過

#### 文檔檢查
- [ ] 用戶手冊完整
- [ ] 安裝說明清晰
- [ ] 發布說明準確
- [ ] 版本信息正確
- [ ] 許可證文件完整

#### 發布檢查
- [ ] 安裝程序正常
- [ ] 文件大小合理
- [ ] 下載鏈接有效
- [ ] 發布公告準備
- [ ] 支持渠道就緒

---

## 🚀 發布流程

### 1. 最終檢查

#### 質量檢查
```powershell
# Final-Check.ps1
Write-Host "Running final quality checks..." -ForegroundColor Green

# 1. 檢查文件大小
$MaxSize = 2GB # 2GB限制
$BuildSize = (Get-ChildItem -Path "BuildArchive\MingGoRTS\WindowsNoEditor" -Recurse | Measure-Object -Property Length -Sum).Sum

if ($BuildSize -gt $MaxSize) {
    Write-Host "Warning: Build size exceeds limit ($($BuildSize/1GB)GB > $($MaxSize/1GB)GB)" -ForegroundColor Yellow
} else {
    Write-Host "Build size: $($BuildSize/1MB)MB" -ForegroundColor Green
}

# 2. 檢查病毒掃描
Write-Host "Running virus scan..." -ForegroundColor Yellow
# 實際病毒掃描邏輯

# 3. 檢查數字簽名
Write-Host "Verifying digital signatures..." -ForegroundColor Yellow
# 實際簽名驗證邏輯

Write-Host "Final checks completed!" -ForegroundColor Green
```

### 2. 發布執行

#### 發布腳本
```powershell
# Publish-Build.ps1
param(
    [string]$Version = "1.0.0-BETA",
    [string]$ReleaseNotes = "Initial Beta Release"
)

Write-Host "Publishing MingGoRTS $Version..." -ForegroundColor Green

# 1. 創建發布目錄
$ReleaseDir = "Releases\MingGoRTS-$Version"
if (!(Test-Path $ReleaseDir)) {
    New-Item -ItemType Directory -Path $ReleaseDir -Force
}

# 2. 複製構建文件
Copy-Item -Path "BuildArchive\MingGoRTS\WindowsNoEditor\*" -Destination "$ReleaseDir\" -Recurse -Force

# 3. 複製文檔
Copy-Item -Path "Documentation\*" -Destination "$ReleaseDir\Documentation\" -Recurse -Force

# 4. 創建發布信息
$ReleaseInfo = @{
    Version = $Version
    ReleaseDate = Get-Date -Format "yyyy-MM-dd"
    ReleaseNotes = $ReleaseNotes
    BuildID = "BETA-$Version-$(Get-Date -Format 'yyyyMMdd-HHmm')"
    Requirements = @{
        OS = "Windows 10/11 (x64)"
        CPU = "Intel i3-3220 or AMD FX-6350"
        RAM = "4GB"
        GPU = "NVIDIA GTX 660 or AMD HD 7850"
        Storage = "10GB"
    }
}

$ReleaseInfo | ConvertTo-Json | Out-File -FilePath "$ReleaseDir\ReleaseInfo.json"

# 5. 創建壓縮包
Compress-Archive -Path "$ReleaseDir\*" -DestinationPath "Releases\MingGoRTS-$Version.zip" -Force

Write-Host "Publish completed!" -ForegroundColor Green
```

### 3. 發布後驗證

#### 驗證清單
- [ ] 下載鏈接有效
- [ ] 安裝程序正常
- [ ] 遊戲運行正常
- [ ] 版本信息正確
- [ ] 用戶反饋收集

---

## 📊 發布統計

### 構建統計
- **構建時間**: 記錄總構建時間
- **文件大小**: 各組件大小統計
- **測試覆蓋**: 測試用例覆蓋率
- **質量指標**: Bug數量、性能指標

### 發布統計
- **下載次數**: 發布後下載統計
- **安裝成功率**: 安裝成功統計
- **用戶反饋**: 反饋收集統計
- **問題報告**: 問題追蹤統計

---

## 🔄 維護和更新

### 版本維護
- **熱修復**: 緊急Bug修復
- **補丁更新**: 定期補丁發布
- **功能更新**: 新功能添加
- **性能優化**: 持續性能改進

### 支持流程
- **問題收集**: 用戶問題收集
- **問題分析**: 問題分析和分類
- **解決方案**: 問題解決和修復
- **發布更新**: 修復版本發布

---

**文檔版本**: v1.0  
**更新日期**: 2026-03-23  
**適用版本**: MingGoRTS Beta v1.0  
**文檔語言**: 簡體中文

通過遵循此構建指南，可以確保MingGoRTS Beta版本的高質量發布，為用戶提供優秀的遊戲體驗。
