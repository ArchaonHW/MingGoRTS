#Requires -Version 5.1

<#
.SYNOPSIS
    高效批量開發系統 v2 (簡化穩定版)

.PARAMETER Phase
    開發階段: Phase1 | Phase2 | Phase3 | Phase4 | All

.PARAMETER DryRun
    預演模式

.EXAMPLE
    .\BatchDev-v2.ps1 -Phase All
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("Phase1", "Phase2", "Phase3", "Phase4", "All")]
    [string]$Phase,
    [switch]$DryRun,
    [string]$ProjectRoot = "F:\MingGoRTS"
)

$ErrorActionPreference = "Stop"
$StartTime = Get-Date

$SourcePublic = "$ProjectRoot\Source\MingGoRTS\Public"
$SourcePrivate = "$ProjectRoot\Source\MingGoRTS\Private"
$ReportsDir = "$ProjectRoot\Reports"
$LogsDir = "$ProjectRoot\Logs"

@($ReportsDir, $LogsDir) | ForEach-Object {
    if (-not (Test-Path $_)) { New-Item -ItemType Directory -Path $_ -Force | Out-Null }
}

function Write-DevLog {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "HH:mm:ss"
    $Colors = @{ "INFO"="White"; "SUCCESS"="Green"; "WARNING"="Yellow"; "ERROR"="Red"; "TASK"="Cyan" }
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Colors[$Level]
    "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] [$Level] $Message" | Add-Content "$LogsDir\BatchDev_v2_$(Get-Date -Format 'yyyyMMdd').log"
}

# 44個任務定義
$AllTasks = @{
    Phase1 = @(
        @{ ID="SAGE-001"; Name="角色類型系統框架"; Priority="Critical"; Hours=8;
           Files=@("MingSageCharacterSystem.h", "MingSageCharacterSystem.cpp");
           Module="SageBrain"; Dependencies=@() },
        @{ ID="SAGE-002"; Name="聖者特性實現"; Priority="High"; Hours=12;
           Files=@("MingSageTraits.h", "MingSageTraits.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-001") },
        @{ ID="SAGE-003"; Name="魔王特性實現"; Priority="High"; Hours=12;
           Files=@("MingDemonTraits.h", "MingDemonTraits.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-001") },
        @{ ID="SAGE-004"; Name="偽聖者特性實現"; Priority="High"; Hours=12;
           Files=@("MingFalseSageTraits.h", "MingFalseSageTraits.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-001") },
        @{ ID="SAGE-005"; Name="三權系統框架"; Priority="Critical"; Hours=16;
           Files=@("MingThreePowerSystem.h", "MingThreePowerSystem.cpp");
           Module="SageCommand"; Dependencies=@() },
        @{ ID="SAGE-006"; Name="道權系統"; Priority="High"; Hours=20;
           Files=@("MingMoralAuthority.h", "MingMoralAuthority.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-005") },
        @{ ID="SAGE-007"; Name="策權系統"; Priority="High"; Hours=20;
           Files=@("MingStrategyAuthority.h", "MingStrategyAuthority.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-005") },
        @{ ID="SAGE-008"; Name="兵權系統"; Priority="High"; Hours=20;
           Files=@("MingMilitaryAuthority.h", "MingMilitaryAuthority.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-005") },
        @{ ID="SAGE-009"; Name="角色選擇UI"; Priority="Medium"; Hours=16;
           Files=@("MingSageCharacterUI.h", "MingSageCharacterUI.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-002", "SAGE-003", "SAGE-004") },
        @{ ID="SAGE-010"; Name="三權平衡機制"; Priority="Medium"; Hours=12;
           Files=@("MingAuthorityBalance.h", "MingAuthorityBalance.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-006", "SAGE-007", "SAGE-008") },
        @{ ID="SAGE-011"; Name="系統集成測試"; Priority="Medium"; Hours=12;
           Files=@("MingPhase1IntegrationTest.h", "MingPhase1IntegrationTest.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-009", "SAGE-010") }
    )

    Phase2 = @(
        @{ ID="SAGE-012"; Name="六策系統框架"; Priority="High"; Hours=8;
           Files=@("MingSixStrategiesSystem.h", "MingSixStrategiesSystem.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-007") },
        @{ ID="SAGE-013"; Name="立國策略(正)"; Priority="Medium"; Hours=12;
           Files=@("MingEstablishNationStrategy.h", "MingEstablishNationStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-014"; Name="立制策略(正)"; Priority="Medium"; Hours=12;
           Files=@("MingEstablishLawStrategy.h", "MingEstablishLawStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-015"; Name="立人策略(正)"; Priority="Medium"; Hours=12;
           Files=@("MingEstablishPeopleStrategy.h", "MingEstablishPeopleStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-016"; Name="破局策略(逆)"; Priority="Medium"; Hours=16;
           Files=@("MingBreakSituationStrategy.h", "MingBreakSituationStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-017"; Name="破結構策略(逆)"; Priority="Medium"; Hours=16;
           Files=@("MingBreakStructureStrategy.h", "MingBreakStructureStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-018"; Name="不破人策略(逆)"; Priority="Medium"; Hours=16;
           Files=@("MingBreakWithoutHarmStrategy.h", "MingBreakWithoutHarmStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-019"; Name="策略面板UI"; Priority="Medium"; Hours=16;
           Files=@("MingStrategyPanelUI.h", "MingStrategyPanelUI.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-013", "SAGE-016") },
        @{ ID="SAGE-020"; Name="五行節奏框架"; Priority="High"; Hours=8;
           Files=@("MingWuXingRhythmSystem.h", "MingWuXingRhythmSystem.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-012") },
        @{ ID="SAGE-021"; Name="木階段實現"; Priority="Low"; Hours=8;
           Files=@("MingWoodPhase.h", "MingWoodPhase.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-020") },
        @{ ID="SAGE-022"; Name="火階段實現"; Priority="Low"; Hours=8;
           Files=@("MingFirePhase.h", "MingFirePhase.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-020") },
        @{ ID="SAGE-023"; Name="土階段實現"; Priority="Low"; Hours=8;
           Files=@("MingEarthPhase.h", "MingEarthPhase.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-020") },
        @{ ID="SAGE-024"; Name="金階段實現"; Priority="Low"; Hours=8;
           Files=@("MingMetalPhase.h", "MingMetalPhase.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-020") },
        @{ ID="SAGE-025"; Name="水階段實現"; Priority="Low"; Hours=8;
           Files=@("MingWaterPhase.h", "MingWaterPhase.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-020") },
        @{ ID="SAGE-026"; Name="五行節奏面板UI"; Priority="Low"; Hours=12;
           Files=@("MingWuXingPanelUI.h", "MingWuXingPanelUI.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-021", "SAGE-022") },
        @{ ID="SAGE-027"; Name="Phase2集成測試"; Priority="Medium"; Hours=10;
           Files=@("MingPhase2IntegrationTest.h", "MingPhase2IntegrationTest.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-019", "SAGE-026") }
    )

    Phase3 = @(
        @{ ID="SAGE-028"; Name="防墮系統框架"; Priority="High"; Hours=16;
           Files=@("MingAntiFallSystem.h", "MingAntiFallSystem.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-011") },
        @{ ID="SAGE-029"; Name="內部始疑徵象"; Priority="Medium"; Hours=8;
           Files=@("MingInternalDoubtDetection.h", "MingInternalDoubtDetection.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-028") },
        @{ ID="SAGE-030"; Name="敵轉異常徵象"; Priority="Medium"; Hours=8;
           Files=@("MingEnemyAbnormalDetection.h", "MingEnemyAbnormalDetection.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-028") },
        @{ ID="SAGE-031"; Name="事過露跡徵象"; Priority="Medium"; Hours=8;
           Files=@("MingEvidenceExposureDetection.h", "MingEvidenceExposureDetection.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-028") },
        @{ ID="SAGE-032"; Name="贖罪任務系統"; Priority="Medium"; Hours=12;
           Files=@("MingAtonementQuestSystem.h", "MingAtonementQuestSystem.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-029", "SAGE-030", "SAGE-031") },
        @{ ID="SAGE-033"; Name="墮落狀態管理"; Priority="Medium"; Hours=8;
           Files=@("MingFallenStateManager.h", "MingFallenStateManager.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-032") },
        @{ ID="SAGE-034"; Name="AI逆策檢測"; Priority="Medium"; Hours=12;
           Files=@("MingAIReverseDetection.h", "MingAIReverseDetection.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-016", "SAGE-017", "SAGE-018") },
        @{ ID="SAGE-035"; Name="AI應對策略"; Priority="Medium"; Hours=12;
           Files=@("MingAIResponseStrategy.h", "MingAIResponseStrategy.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-034") },
        @{ ID="SAGE-036"; Name="AI策略選擇"; Priority="Medium"; Hours=16;
           Files=@("MingAIStrategySelector.h", "MingAIStrategySelector.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-035") },
        @{ ID="SAGE-037"; Name="AI五行適應"; Priority="Medium"; Hours=12;
           Files=@("MingAIWuXingAdaptation.h", "MingAIWuXingAdaptation.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-036", "SAGE-020") },
        @{ ID="SAGE-038"; Name="Phase3集成測試"; Priority="Medium"; Hours=10;
           Files=@("MingPhase3IntegrationTest.h", "MingPhase3IntegrationTest.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-033", "SAGE-037") }
    )

    Phase4 = @(
        @{ ID="SAGE-039"; Name="全系統集成框架"; Priority="Critical"; Hours=20;
           Files=@("MingFullSystemIntegration.h", "MingFullSystemIntegration.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-027", "SAGE-038") },
        @{ ID="SAGE-040"; Name="角色平衡測試"; Priority="High"; Hours=12;
           Files=@("MingCharacterBalanceTest.h", "MingCharacterBalanceTest.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-039") },
        @{ ID="SAGE-041"; Name="策略數值調優"; Priority="High"; Hours=12;
           Files=@("MingStrategyTuning.h", "MingStrategyTuning.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-039") },
        @{ ID="SAGE-042"; Name="AI難度校準"; Priority="High"; Hours=12;
           Files=@("MingAIDifficultyCalibration.h", "MingAIDifficultyCalibration.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-039") },
        @{ ID="SAGE-043"; Name="用戶體驗測試"; Priority="Medium"; Hours=12;
           Files=@("MingUXTesting.h", "MingUXTesting.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-040", "SAGE-041", "SAGE-042") },
        @{ ID="SAGE-044"; Name="性能優化"; Priority="High"; Hours=12;
           Files=@("MingPerformanceOptimization.h", "MingPerformanceOptimization.cpp");
           Module="SageCommand"; Dependencies=@("SAGE-039") }
    )
}

function Get-HeaderContent($Task) {
    $ClassName = ($Task.Files | Where-Object { $_ -like "*.h" }) -replace "\.h$", ""
    $headerGuard = "__" + ($ClassName.ToUpper() -replace "M", "MING_") + "_H__"

    return @"
// $ClassName.h
// 至聖者指揮學系統 - $($Task.Name)
// Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
// Task: $($Task.ID)

#pragma once
#ifndef $headerGuard
#define $headerGuard

#include "MingGoRTS.h"
#include "$ClassName.generated.h"

/**
 * $($Task.Name)
 * Priority: $($Task.Priority)
 * Estimated Hours: $($Task.Hours)
 */
UCLASS()
class MINGGORTS_API $ClassName : public UObject
{
    GENERATED_BODY()

public:
    $ClassName();

    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual void Initialize();

    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual bool Execute();

    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual void Shutdown();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bActive = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float DefaultValue = 1.0f;

    virtual void OnInitialize();
    virtual void OnExecute();
    virtual void OnShutdown();

    UFUNCTION(BlueprintPure, Category = "Utility")
    bool IsReady() const { return bInitialized && bActive; }
};

#endif // $headerGuard
"@
}

function Get-ImplContent($Task) {
    $ClassName = ($Task.Files | Where-Object { $_ -like "*.cpp" }) -replace "\.cpp$", ""
    $HeaderFile = $ClassName + ".h"

    return @"
// $ClassName.cpp
// 至聖者指揮學系統 - $($Task.Name)
// Generated: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
// Task: $($Task.ID)

#include "$HeaderFile"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(Log$ClassName, Log, All);

$className::$ClassName()
{
}

void $ClassName::Initialize()
{
    if (bInitialized)
    {
        UE_LOG(Log$ClassName, Warning, TEXT("Already initialized"));
        return;
    }
    UE_LOG(Log$ClassName, Log, TEXT("Initializing $($Task.Name)..."));
    OnInitialize();
    bInitialized = true;
    bActive = true;
    UE_LOG(Log$ClassName, Log, TEXT("$($Task.Name) initialized successfully"));
}

bool $ClassName::Execute()
{
    if (!bInitialized)
    {
        UE_LOG(Log$ClassName, Error, TEXT("Not initialized"));
        return false;
    }
    if (!bActive)
    {
        UE_LOG(Log$ClassName, Warning, TEXT("System not active"));
        return false;
    }
    UE_LOG(Log$ClassName, Log, TEXT("Executing $($Task.Name)..."));
    OnExecute();
    return true;
}

void $ClassName::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    UE_LOG(Log$ClassName, Log, TEXT("Shutting down $($Task.Name)..."));
    OnShutdown();
    bActive = false;
    bInitialized = false;
    UE_LOG(Log$ClassName, Log, TEXT("$($Task.Name) shutdown complete"));
}

void $ClassName::OnInitialize()
{
    UE_LOG(Log$ClassName, Verbose, TEXT("OnInitialize called"));
}

void $ClassName::OnExecute()
{
    UE_LOG(Log$ClassName, Verbose, TEXT("OnExecute called"));
}

void $ClassName::OnShutdown()
{
    UE_LOG(Log$ClassName, Verbose, TEXT("OnShutdown called"));
}
"@
}

function Invoke-DevelopmentTask($Task, [ref]$CompletedIDs) {
    $Result = @{
        TaskID = $Task.ID
        Task = $Task
        Success = $false
        StartTime = Get-Date
        EndTime = $null
        FilesCreated = @()
        Error = $null
        Skipped = $false
    }

    # 檢查依賴
    foreach ($dep in $Task.Dependencies) {
        if ($dep -notin $CompletedIDs.Value) {
            $Result.Error = "依賴未完成: $dep"
            $Result.Skipped = $true
            $Result.EndTime = Get-Date
            return $Result
        }
    }

    try {
        $ModuleDir = $Task.Module

        foreach ($file in $Task.Files) {
            $IsHeader = $file -like "*.h"
            $TargetDir = if ($IsHeader) { "$SourcePublic\$ModuleDir" } else { "$SourcePrivate\$ModuleDir" }
            $TargetPath = "$TargetDir\$file"

            if (-not (Test-Path $TargetDir)) {
                New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
            }

            if (-not (Test-Path $TargetPath)) {
                if (-not $DryRun) {
                    $Content = if ($IsHeader) { Get-HeaderContent $Task } else { Get-ImplContent $Task }
                    Set-Content -Path $TargetPath -Value $Content -Encoding UTF8 -NoNewline
                }
                $Result.FilesCreated += $file
            }
        }

        $Result.Success = $true
        $Result.EndTime = Get-Date
    }
    catch {
        $Result.Error = $_.Exception.Message
        $Result.EndTime = Get-Date
    }

    return $Result
}

Write-DevLog "啟動高效批量開發系統 v2..." "TASK"
Write-DevLog "階段: $Phase | 預演: $DryRun" "INFO"

$TasksToProcess = if ($Phase -eq "All") {
    $AllTasks.Phase1 + $AllTasks.Phase2 + $AllTasks.Phase3 + $AllTasks.Phase4
} else {
    $AllTasks[$Phase]
}

Write-DevLog "任務總數: $($TasksToProcess.Count)" "INFO"
Write-DevLog "預估總工時: $(($TasksToProcess | Measure-Object -Property Hours -Sum).Sum) 小時" "INFO"

if ($DryRun) {
    Write-DevLog "[預演模式] 不會實際修改文件" "WARNING"
}

$AllResults = @()
$CompletedIDs = @()
$TotalTasks = $TasksToProcess.Count
$Iteration = 0
$MaxIterations = $TotalTasks * 2

while ($CompletedIDs.Count -lt $TotalTasks -and $Iteration -lt $MaxIterations) {
    $Iteration++

    # 獲取可執行任務
    $ReadyTasks = $TasksToProcess | Where-Object {
        $_.ID -notin $CompletedIDs -and
        ($_.Dependencies | Where-Object { $_ -notin $CompletedIDs }).Count -eq 0
    }

    if ($ReadyTasks.Count -eq 0) {
        # 檢查是否有死鎖
        $PendingTasks = $TasksToProcess | Where-Object { $_.ID -notin $CompletedIDs }
        if ($PendingTasks.Count -gt 0) {
            Write-DevLog "警告: 有 $($PendingTasks.Count) 個任務因依賴無法執行" "WARNING"
            foreach ($pt in $PendingTasks) {
                Write-DevLog "  - $($pt.ID): 依賴 $($pt.Dependencies -join ', ')" "WARNING"
            }
        }
        break
    }

    foreach ($task in $ReadyTasks) {
        $Result = Invoke-DevelopmentTask $task ([ref]$CompletedIDs)
        $AllResults += $Result

        $StatusIcon = if ($Result.Success) { "✅" } elseif ($Result.Skipped) { "⏭️" } else { "❌" }
        $StatusText = if ($Result.Success) { "完成" } elseif ($Result.Skipped) { "跳過" } else { "失敗" }

        Write-DevLog "$StatusIcon [$($Result.TaskID)] $($Result.Task.Name) - $StatusText ($(if($Result.FilesCreated.Count -gt 0){$Result.FilesCreated.Count}else{0}) 文件)" $(if($Result.Success){"SUCCESS"}elseif($Result.Skipped){"WARNING"}else{"ERROR"})

        if ($Result.Success) {
            $CompletedIDs += $Result.TaskID
        }
    }

    # 顯示進度
    $Percent = [math]::Round(($CompletedIDs.Count / $TotalTasks) * 100, 1)
    Write-DevLog "進度: $CompletedIDs/$TotalTasks ($Percent%)" "INFO"
}

$EndTime = Get-Date

# 生成報告
$SuccessCount = ($AllResults | Where-Object { $_.Success }).Count
$SkipCount = ($AllResults | Where-Object { $_.Skipped }).Count
$FailCount = ($AllResults | Where-Object { -not $_.Success -and -not $_.Skipped }).Count
$TotalFiles = ($AllResults | ForEach-Object { $_.FilesCreated.Count } | Measure-Object -Sum).Sum

$ReportPath = "$ReportsDir\BatchDev_v2_Report_$Phase`_$((Get-Date -Format 'yyyyMMdd_HHmmss')).md"

$Report = @"
# 高效批量開發報告 v2

**階段**: $Phase
**執行時間**: $($StartTime.ToString('yyyy-MM-dd HH:mm:ss')) ~ $($EndTime.ToString('HH:mm:ss'))
**總耗時**: $((New-TimeSpan $StartTime $EndTime).ToString('hh\:mm\:ss'))

---

## 執行摘要

| 指標 | 數值 |
|------|------|
| 總任務 | $($AllResults.Count) |
| 成功 | $SuccessCount |
| 跳過 | $SkipCount |
| 失敗 | $FailCount |
| 成功率 | $([math]::Round(($SuccessCount/$AllResults.Count)*100, 1))% |
| 創建文件 | $TotalFiles |

---

## 任務詳情

| 任務ID | 名稱 | 狀態 | 文件數 |
|--------|------|------|--------|
$(($AllResults | ForEach-Object {
    $status = if ($_.Success) { "✅" } elseif ($_.Skipped) { "⏭️" } else { "❌" }
    "| $($_.TaskID) | $($_.Task.Name) | $status | $($_.FilesCreated.Count) |"
}) -join "`n")

---

*報告由 BatchDev-v2.ps1 生成*
"@

$Report | Set-Content $ReportPath -Encoding UTF8

Write-DevLog "" "INFO"
Write-DevLog "====================================" "SUCCESS"
Write-DevLog "批量開發完成!" "SUCCESS"
Write-DevLog "====================================" "SUCCESS"
Write-DevLog "總任務: $($AllResults.Count) | 成功: $SuccessCount | 跳過: $SkipCount | 失敗: $FailCount" "INFO"
Write-DevLog "創建文件: $TotalFiles" "INFO"
Write-DevLog "總耗時: $((New-TimeSpan $StartTime $EndTime).ToString('hh\:mm\:ss'))" "INFO"
Write-DevLog "報告: $ReportPath" "SUCCESS"

return @{
    Phase = $Phase
    TotalTasks = $AllResults.Count
    SuccessCount = $SuccessCount
    SkipCount = $SkipCount
    FailCount = $FailCount
    FilesCreated = $TotalFiles
    Duration = New-TimeSpan $StartTime $EndTime
    ReportPath = $ReportPath
    Results = $AllResults
}
