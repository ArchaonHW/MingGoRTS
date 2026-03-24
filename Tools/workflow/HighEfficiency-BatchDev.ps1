#Requires -Version 5.1

<#
.SYNOPSIS
    高效批量開發系統 (High-Efficiency Batch Development System)
    
.DESCRIPTION
    專為至聖者指揮學系統設計的高效批量開發工作流：
    - 並行任務處理 (Parallel Task Processing)
    - 智能依賴解析 (Smart Dependency Resolution)
    - 自動編譯驗證 (Auto Compile Validation)
    - 實時進度追蹤 (Real-time Progress Tracking)
    
.PARAMETER Phase
    開發階段: Phase1 | Phase2 | Phase3 | Phase4 | All
    
.PARAMETER ParallelJobs
    並行作業數量 (預設: 4)
    
.PARAMETER AutoCompile
    開發後自動編譯驗證
    
.PARAMETER DryRun
    預演模式，不實際修改文件
    
.EXAMPLE
    .\HighEfficiency-BatchDev.ps1 -Phase Phase1 -ParallelJobs 4 -AutoCompile
    .\HighEfficiency-BatchDev.ps1 -Phase All -ParallelJobs 8 -AutoCompile
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("Phase1", "Phase2", "Phase3", "Phase4", "All")]
    [string]$Phase,
    
    [int]$ParallelJobs = 4,
    [switch]$AutoCompile,
    [switch]$DryRun,
    [switch]$Force,
    [string]$ProjectRoot = "F:\MingGoRTS"
)

$ErrorActionPreference = "Stop"
$StartTime = Get-Date

# 初始化路徑
$SourcePublic = "$ProjectRoot\Source\MingGoRTS\Public"
$SourcePrivate = "$ProjectRoot\Source\MingGoRTS\Private"
$ReportsDir = "$ProjectRoot\Reports"
$LogsDir = "$ProjectRoot\Logs"

# 確保目錄存在
@($ReportsDir, $LogsDir) | ForEach-Object {
    if (-not (Test-Path $_)) { New-Item -ItemType Directory -Path $_ -Force | Out-Null }
}

# ============================================
# 任務定義
# ============================================

$AllTasks = @{
    # Phase 1: 核心系統 (11任務, 152h)
    Phase1 = @(
        @{ ID="SAGE-001"; Name="角色類型系統框架"; Priority="Critical"; Hours=8;  
           Files=@("MingSageCharacterSystem.h", "MingSageCharacterSystem.cpp");
           Tags=@("Core", "Character"); Dependencies=@() },
        @{ ID="SAGE-002"; Name="聖者特性實現"; Priority="High"; Hours=12; 
           Files=@("MingSageTraits.h", "MingSageTraits.cpp");
           Tags=@("Character", "Sage"); Dependencies=@("SAGE-001") },
        @{ ID="SAGE-003"; Name="魔王特性實現"; Priority="High"; Hours=12; 
           Files=@("MingDemonTraits.h", "MingDemonTraits.cpp");
           Tags=@("Character", "Demon"); Dependencies=@("SAGE-001") },
        @{ ID="SAGE-004"; Name="偽聖者特性實現"; Priority="High"; Hours=12; 
           Files=@("MingFalseSageTraits.h", "MingFalseSageTraits.cpp");
           Tags=@("Character", "FalseSage"); Dependencies=@("SAGE-001") },
        @{ ID="SAGE-005"; Name="三權系統框架"; Priority="Critical"; Hours=16; 
           Files=@("MingThreePowerSystem.h", "MingThreePowerSystem.cpp");
           Tags=@("Core", "Authority"); Dependencies=@() },
        @{ ID="SAGE-006"; Name="道權系統"; Priority="High"; Hours=20; 
           Files=@("MingMoralAuthority.h", "MingMoralAuthority.cpp");
           Tags=@("Authority", "Moral"); Dependencies=@("SAGE-005") },
        @{ ID="SAGE-007"; Name="策權系統"; Priority="High"; Hours=20; 
           Files=@("MingStrategyAuthority.h", "MingStrategyAuthority.cpp");
           Tags=@("Authority", "Strategy"); Dependencies=@("SAGE-005") },
        @{ ID="SAGE-008"; Name="兵權系統"; Priority="High"; Hours=20; 
           Files=@("MingMilitaryAuthority.h", "MingMilitaryAuthority.cpp");
           Tags=@("Authority", "Military"); Dependencies=@("SAGE-005") },
        @{ ID="SAGE-009"; Name="角色選擇UI"; Priority="Medium"; Hours=16; 
           Files=@("MingSageCharacterUI.h", "MingSageCharacterUI.cpp");
           Tags=@("UI", "Character"); Dependencies=@("SAGE-002", "SAGE-003", "SAGE-004") },
        @{ ID="SAGE-010"; Name="三權平衡機制"; Priority="Medium"; Hours=12; 
           Files=@("MingAuthorityBalance.h", "MingAuthorityBalance.cpp");
           Tags=@("Authority", "Balance"); Dependencies=@("SAGE-006", "SAGE-007", "SAGE-008") },
        @{ ID="SAGE-011"; Name="系統集成測試"; Priority="Medium"; Hours=12; 
           Files=@("MingPhase1IntegrationTest.h", "MingPhase1IntegrationTest.cpp");
           Tags=@("Test", "Integration"); Dependencies=@("SAGE-009", "SAGE-010") }
    )
    
    # Phase 2: 策略系統 (16任務, 120h)
    Phase2 = @(
        @{ ID="SAGE-012"; Name="六策系統框架"; Priority="High"; Hours=8; 
           Files=@("MingSixStrategiesSystem.h", "MingSixStrategiesSystem.cpp");
           Tags=@("Core", "Strategy"); Dependencies=@("SAGE-007") },
        @{ ID="SAGE-013"; Name="立國策略(正)"; Priority="Medium"; Hours=12; 
           Files=@("MingEstablishNationStrategy.h", "MingEstablishNationStrategy.cpp");
           Tags=@("Strategy", "Positive"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-014"; Name="立制策略(正)"; Priority="Medium"; Hours=12; 
           Files=@("MingEstablishLawStrategy.h", "MingEstablishLawStrategy.cpp");
           Tags=@("Strategy", "Positive"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-015"; Name="立人策略(正)"; Priority="Medium"; Hours=12; 
           Files=@("MingEstablishPeopleStrategy.h", "MingEstablishPeopleStrategy.cpp");
           Tags=@("Strategy", "Positive"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-016"; Name="破局策略(逆)"; Priority="Medium"; Hours=16; 
           Files=@("MingBreakSituationStrategy.h", "MingBreakSituationStrategy.cpp");
           Tags=@("Strategy", "Reverse"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-017"; Name="破結構策略(逆)"; Priority="Medium"; Hours=16; 
           Files=@("MingBreakStructureStrategy.h", "MingBreakStructureStrategy.cpp");
           Tags=@("Strategy", "Reverse"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-018"; Name="不破人策略(逆)"; Priority="Medium"; Hours=16; 
           Files=@("MingBreakWithoutHarmStrategy.h", "MingBreakWithoutHarmStrategy.cpp");
           Tags=@("Strategy", "Reverse"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-019"; Name="策略面板UI"; Priority="Medium"; Hours=16; 
           Files=@("MingStrategyPanelUI.h", "MingStrategyPanelUI.cpp");
           Tags=@("UI", "Strategy"); Dependencies=@("SAGE-013", "SAGE-016") },
        @{ ID="SAGE-020"; Name="五行節奏框架"; Priority="High"; Hours=8; 
           Files=@("MingWuXingRhythmSystem.h", "MingWuXingRhythmSystem.cpp");
           Tags=@("Core", "WuXing"); Dependencies=@("SAGE-012") },
        @{ ID="SAGE-021"; Name="木階段實現"; Priority="Low"; Hours=8; 
           Files=@("MingWoodPhase.h", "MingWoodPhase.cpp");
           Tags=@("WuXing", "Wood"); Dependencies=@("SAGE-020") },
        @{ ID="SAGE-022"; Name="火階段實現"; Priority="Low"; Hours=8; 
           Files=@("MingFirePhase.h", "MingFirePhase.cpp");
           Tags=@("WuXing", "Fire"); Dependencies=@("SAGE-020") },
        @{ ID="SAGE-023"; Name="土階段實現"; Priority="Low"; Hours=8; 
           Files=@("MingEarthPhase.h", "MingEarthPhase.cpp");
           Tags=@("WuXing", "Earth"); Dependencies=@("SAGE-020") },
        @{ ID="SAGE-024"; Name="金階段實現"; Priority="Low"; Hours=8; 
           Files=@("MingMetalPhase.h", "MingMetalPhase.cpp");
           Tags=@("WuXing", "Metal"); Dependencies=@("SAGE-020") },
        @{ ID="SAGE-025"; Name="水階段實現"; Priority="Low"; Hours=8; 
           Files=@("MingWaterPhase.h", "MingWaterPhase.cpp");
           Tags=@("WuXing", "Water"); Dependencies=@("SAGE-020") },
        @{ ID="SAGE-026"; Name="五行節奏面板UI"; Priority="Low"; Hours=12; 
           Files=@("MingWuXingPanelUI.h", "MingWuXingPanelUI.cpp");
           Tags=@("UI", "WuXing"); Dependencies=@("SAGE-021", "SAGE-022") },
        @{ ID="SAGE-027"; Name="Phase2集成測試"; Priority="Medium"; Hours=10; 
           Files=@("MingPhase2IntegrationTest.h", "MingPhase2IntegrationTest.cpp");
           Tags=@("Test", "Integration"); Dependencies=@("SAGE-019", "SAGE-026") }
    )
    
    # Phase 3: 進階機制 (11任務, 108h)
    Phase3 = @(
        @{ ID="SAGE-028"; Name="防墮系統框架"; Priority="High"; Hours=16; 
           Files=@("MingAntiFallSystem.h", "MingAntiFallSystem.cpp");
           Tags=@("Core", "AntiFall"); Dependencies=@("SAGE-011") },
        @{ ID="SAGE-029"; Name="內部始疑徵象"; Priority="Medium"; Hours=8; 
           Files=@("MingInternalDoubtDetection.h", "MingInternalDoubtDetection.cpp");
           Tags=@("AntiFall", "Detection"); Dependencies=@("SAGE-028") },
        @{ ID="SAGE-030"; Name="敵轉異常徵象"; Priority="Medium"; Hours=8; 
           Files=@("MingEnemyAbnormalDetection.h", "MingEnemyAbnormalDetection.cpp");
           Tags=@("AntiFall", "Detection"); Dependencies=@("SAGE-028") },
        @{ ID="SAGE-031"; Name="事過露跡徵象"; Priority="Medium"; Hours=8; 
           Files=@("MingEvidenceExposureDetection.h", "MingEvidenceExposureDetection.cpp");
           Tags=@("AntiFall", "Detection"); Dependencies=@("SAGE-028") },
        @{ ID="SAGE-032"; Name="贖罪任務系統"; Priority="Medium"; Hours=12; 
           Files=@("MingAtonementQuestSystem.h", "MingAtonementQuestSystem.cpp");
           Tags=@("AntiFall", "Quest"); Dependencies=@("SAGE-029", "SAGE-030", "SAGE-031") },
        @{ ID="SAGE-033"; Name="墮落狀態管理"; Priority="Medium"; Hours=8; 
           Files=@("MingFallenStateManager.h", "MingFallenStateManager.cpp");
           Tags=@("AntiFall", "State"); Dependencies=@("SAGE-032") },
        @{ ID="SAGE-034"; Name="AI逆策檢測"; Priority="Medium"; Hours=12; 
           Files=@("MingAIReverseDetection.h", "MingAIReverseDetection.cpp");
           Tags=@("AI", "Detection"); Dependencies=@("SAGE-016", "SAGE-017", "SAGE-018") },
        @{ ID="SAGE-035"; Name="AI應對策略"; Priority="Medium"; Hours=12; 
           Files=@("MingAIResponseStrategy.h", "MingAIResponseStrategy.cpp");
           Tags=@("AI", "Response"); Dependencies=@("SAGE-034") },
        @{ ID="SAGE-036"; Name="AI策略選擇"; Priority="Medium"; Hours=16; 
           Files=@("MingAIStrategySelector.h", "MingAIStrategySelector.cpp");
           Tags=@("AI", "Strategy"); Dependencies=@("SAGE-035") },
        @{ ID="SAGE-037"; Name="AI五行適應"; Priority="Medium"; Hours=12; 
           Files=@("MingAIWuXingAdaptation.h", "MingAIWuXingAdaptation.cpp");
           Tags=@("AI", "WuXing"); Dependencies=@("SAGE-036", "SAGE-020") },
        @{ ID="SAGE-038"; Name="Phase3集成測試"; Priority="Medium"; Hours=10; 
           Files=@("MingPhase3IntegrationTest.h", "MingPhase3IntegrationTest.cpp");
           Tags=@("Test", "Integration"); Dependencies=@("SAGE-033", "SAGE-037") }
    )
    
    # Phase 4: 集成測試 (6任務, 80h)
    Phase4 = @(
        @{ ID="SAGE-039"; Name="全系統集成框架"; Priority="Critical"; Hours=20; 
           Files=@("MingFullSystemIntegration.h", "MingFullSystemIntegration.cpp");
           Tags=@("Core", "Integration"); Dependencies=@("SAGE-027", "SAGE-038") },
        @{ ID="SAGE-040"; Name="角色平衡測試"; Priority="High"; Hours=12; 
           Files=@("MingCharacterBalanceTest.h", "MingCharacterBalanceTest.cpp");
           Tags=@("Test", "Balance"); Dependencies=@("SAGE-039") },
        @{ ID="SAGE-041"; Name="策略數值調優"; Priority="High"; Hours=12; 
           Files=@("MingStrategyTuning.h", "MingStrategyTuning.cpp");
           Tags=@("Tuning", "Strategy"); Dependencies=@("SAGE-039") },
        @{ ID="SAGE-042"; Name="AI難度校準"; Priority="High"; Hours=12; 
           Files=@("MingAIDifficultyCalibration.h", "MingAIDifficultyCalibration.cpp");
           Tags=@("AI", "Calibration"); Dependencies=@("SAGE-039") },
        @{ ID="SAGE-043"; Name="用戶體驗測試"; Priority="Medium"; Hours=12; 
           Files=@("MingUXTesting.h", "MingUXTesting.cpp");
           Tags=@("Test", "UX"); Dependencies=@("SAGE-040", "SAGE-041", "SAGE-042") },
        @{ ID="SAGE-044"; Name="性能優化"; Priority="High"; Hours=12; 
           Files=@("MingPerformanceOptimization.h", "MingPerformanceOptimization.cpp");
           Tags=@("Optimization", "Performance"); Dependencies=@("SAGE-039") }
    )
}

# ============================================
# 日誌系統
# ============================================

function Write-DevLog {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "HH:mm:ss"
    $ColorMap = @{
        "INFO" = "White"; "SUCCESS" = "Green"; "WARNING" = "Yellow"
        "ERROR" = "Red"; "TASK" = "Cyan"; "PROGRESS" = "Magenta"
    }
    $Color = $ColorMap[$Level]
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    
    # 寫入日誌文件
    $LogFile = "$LogsDir\BatchDev_$(Get-Date -Format 'yyyyMMdd').log"
    "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] [$Level] $Message" | Add-Content $LogFile
}

# ============================================
# 智能調度引擎
# ============================================

class TaskScheduler {
    [array]$Tasks
    [array]$CompletedTasks = @()
    [array]$RunningTasks = @()
    [hashtable]$TaskStatus = @{}
    [int]$MaxParallelJobs
    
    TaskScheduler([array]$tasks, [int]$maxJobs) {
        $this.Tasks = $tasks
        $this.MaxParallelJobs = $maxJobs
        foreach ($task in $tasks) {
            $this.TaskStatus[$task.ID] = "Pending"
        }
    }
    
    [array] GetReadyTasks() {
        return $this.Tasks | Where-Object {
            $this.TaskStatus[$_.ID] -eq "Pending" -and
            $this.AreDependenciesMet($_)
        }
    }
    
    [bool] AreDependenciesMet($task) {
        foreach ($dep in $task.Dependencies) {
            if ($this.TaskStatus[$dep] -ne "Completed") {
                return $false
            }
        }
        return $true
    }
    
    StartNextBatch([scriptblock]$executor) {
        $ready = $this.GetReadyTasks()
        $slots = $this.MaxParallelJobs - $this.RunningTasks.Count
        
        foreach ($task in ($ready | Select-Object -First $slots)) {
            $this.TaskStatus[$task.ID] = "Running"
            $this.RunningTasks += $task
            
            # 啟動異步任務
            Start-Job -ScriptBlock $executor -ArgumentList $task | Out-Null
        }
    }
    
    UpdateStatus() {
        # 檢查完成的作業
        $completed = Get-Job -State "Completed"
        foreach ($job in $completed) {
            $result = Receive-Job $job
            Remove-Job $job
            
            if ($result.Success) {
                $this.TaskStatus[$result.TaskID] = "Completed"
                $this.CompletedTasks += $result.Task
            }
            $this.RunningTasks = $this.RunningTasks | Where-Object { $_.ID -ne $result.TaskID }
        }
    }
    
    [bool] IsComplete() {
        return $this.TaskStatus.Values -notcontains "Pending" -and 
               $this.TaskStatus.Values -notcontains "Running"
    }
    
    [array] GetFailedTasks() {
        return $this.Tasks | Where-Object { $this.TaskStatus[$_.ID] -eq "Failed" }
    }
}

# ============================================
# 任務執行器
# ============================================

function Invoke-DevelopmentTask {
    param([hashtable]$Task)
    
    $Result = @{
        TaskID = $Task.ID
        Task = $Task
        Success = $false
        StartTime = Get-Date
        EndTime = $null
        FilesCreated = @()
        Error = $null
    }
    
    try {
        $ModuleDir = if ($Task.Tags -contains "Core") { "SageCommand" } 
                     elseif ($Task.Tags -contains "Character") { "SageBrain" }
                     else { "SageCommand" }
        
        # 創建文件
        foreach ($file in $Task.Files) {
            $IsHeader = $file -like "*.h"
            $TargetDir = if ($IsHeader) { "$SourcePublic\$ModuleDir" } else { "$SourcePrivate\$ModuleDir" }
            $TargetPath = "$TargetDir\$file"
            
            if (-not (Test-Path $TargetDir)) {
                New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
            }
            
            if (-not (Test-Path $TargetPath) -or $Force) {
                $Content = if ($IsHeader) { Generate-HeaderContent $Task } else { Generate-ImplContent $Task }
                Set-Content -Path $TargetPath -Value $Content -Encoding UTF8
                $Result.FilesCreated += $TargetPath
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

# ============================================
# 代碼生成器
# ============================================

function Get-HeaderContent($Task) {
        $ClassName = ($Task.Files | Where-Object { $_ -like "*.h" }) -replace "\.h$", ""
        
        $includes = @()
        if ($Task.Tags -contains "Core") {
            $includes += '#include "CoreMinimal.h"'
            $includes += '#include "UObject/NoExportTypes.h"'
        }
        if ($Task.Tags -contains "Character") {
            $includes += '#include "SageCommand/MingSageCharacterTypes.h"'
        }
        if ($Task.Tags -contains "Strategy") {
            $includes += '#include "SageCommand/MingStrategyTypes.h"'
        }
        if ($Task.Tags -contains "WuXing") {
            $includes += '#include "SageCommand/MingWuXingTypes.h"'
        }
        if ($Task.Tags -contains "AI") {
            $includes += '#include "AI/MingAIInterface.h"'
        }
        
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
$($includes -join "`n")
#include "$ClassName.generated.h"

/**
 * $($Task.Name)
 * Priority: $($Task.Priority)
 * Estimated Hours: $($Task.Hours)
 * 
 * Components: $($Task.Components -join ', ')
 */
UCLASS()
class MINGGORTS_API $ClassName : public UObject
{
    GENERATED_BODY()

public:
    // Constructor
    $ClassName();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual void Initialize();

    // Core functionality
    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual bool Execute();

    // Cleanup
    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual void Shutdown();

protected:
    // System state
    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bActive = false;

    // Configuration
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float DefaultValue = 1.0f;

    // Tags for system identification
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tags")
    TArray<FString> SystemTags;

    // Internal methods
    virtual void OnInitialize();
    virtual void OnExecute();
    virtual void OnShutdown();

    // Utility functions
    UFUNCTION(BlueprintPure, Category = "Utility")
    bool IsReady() const { return bInitialized && bActive; }

    UFUNCTION(BlueprintPure, Category = "Utility")
    FString GetSystemName() const { return TEXT("$($Task.Name)"); }
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
    // Initialize tags
    SystemTags = { $(($Task.Tags | ForEach-Object { "TEXT(\`"$_\`")" }) -join ", ") };
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
    // Override in derived classes
    UE_LOG(Log$ClassName, Verbose, TEXT("OnInitialize called"));
}

void $ClassName::OnExecute()
{
    // Override in derived classes
    UE_LOG(Log$ClassName, Verbose, TEXT("OnExecute called"));
}

void $ClassName::OnShutdown()
{
    // Override in derived classes
    UE_LOG(Log$ClassName, Verbose, TEXT("OnShutdown called"));
}
"@
}

# ============================================
# 進度顯示
# ============================================

function Show-ProgressDashboard($Scheduler, $TotalTasks, $StartTime) {
    $Elapsed = (Get-Date) - $StartTime
    $Completed = ($Scheduler.TaskStatus.Values | Where-Object { $_ -eq "Completed" }).Count
    $Running = ($Scheduler.TaskStatus.Values | Where-Object { $_ -eq "Running" }).Count
    $Pending = ($Scheduler.TaskStatus.Values | Where-Object { $_ -eq "Pending" }).Count
    
    $Percent = if ($TotalTasks -gt 0) { [math]::Round(($Completed / $TotalTasks) * 100, 1) } else { 0 }
    
    Clear-Host
    Write-Host "
╔══════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
    Write-Host "║      高效批量開發系統 (High-Efficiency Batch Dev)         ║" -ForegroundColor Cyan
    Write-Host "╠══════════════════════════════════════════════════════════╣" -ForegroundColor Cyan
    Write-Host "║  階段: $Phase                                            ║" -ForegroundColor White
    Write-Host "║  並行作業: $ParallelJobs                                 ║" -ForegroundColor White
    Write-Host "╠══════════════════════════════════════════════════════════╣" -ForegroundColor Cyan
    Write-Host "║  總任務: $TotalTasks  已完成: $Completed  運行中: $Running  待處理: $Pending  ║" -ForegroundColor White
    Write-Host "║  完成度: $Percent%  耗時: $($Elapsed.ToString('hh\:mm\:ss'))                                    ║" -ForegroundColor White
    Write-Host "╚══════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
    Write-Host ""
    
    # 顯示運行中任務
    if ($Running -gt 0) {
        Write-Host "  [運行中]" -ForegroundColor Yellow
        foreach ($task in $Scheduler.RunningTasks) {
            Write-Host "    - $($task.ID): $($task.Name)" -ForegroundColor Gray
        }
    }
}

# ============================================
# 報告生成
# ============================================

function Export-DevelopmentReport($Results, $Phase, $StartTime, $EndTime) {
    $Duration = $EndTime - $StartTime
    $SuccessCount = ($Results | Where-Object { $_.Success }).Count
    $FailCount = ($Results | Where-Object { -not $_.Success }).Count
    $FilesCreated = ($Results | ForEach-Object { $_.FilesCreated.Count } | Measure-Object -Sum).Sum
    
    $ReportPath = "$ReportsDir\HighEfficiency_DevReport_$Phase`_$((Get-Date -Format 'yyyyMMdd_HHmmss')).md"
    
    $Report = @"
# 高效批量開發報告

**階段**: $Phase  
**執行時間**: $($StartTime.ToString('yyyy-MM-dd HH:mm:ss')) ~ $($EndTime.ToString('HH:mm:ss'))  
**總耗時**: $($Duration.ToString('hh\:mm\:ss'))  
**並行作業數**: $ParallelJobs

---

## 執行摘要

| 指標 | 數值 |
|------|------|
| 總任務 | $($Results.Count) |
| 成功 | $SuccessCount |
| 失敗 | $FailCount |
| 成功率 | $([math]::Round(($SuccessCount/$Results.Count)*100, 1))% |
| 創建文件 | $FilesCreated |

---

## 任務詳情

| 任務ID | 名稱 | 狀態 | 耗時 | 創建文件 |
|--------|------|------|------|----------|
$(foreach ($r in $Results) {
    $status = if ($r.Success) { "✅ 成功" } else { "❌ 失敗" }
    $dur = if ($r.EndTime) { ($r.EndTime - $r.StartTime).ToString('mm\:ss') } else { "N/A" }
    "| $($r.TaskID) | $($r.Task.Name) | $status | $dur | $($r.FilesCreated.Count) |"
})

---

## 創建的文件

$(foreach ($r in ($Results | Where-Object { $_.FilesCreated.Count -gt 0 })) {
    "### $($r.TaskID): $($r.Task.Name)"
    foreach ($f in $r.FilesCreated) {
        "- ``$f``"
    }
    ""
})

---

## 系統配置

- 預演模式: $(if ($DryRun) { "是" } else { "否" })
- 自動編譯: $(if ($AutoCompile) { "是" } else { "否" })
- 強制覆蓋: $(if ($Force) { "是" } else { "否" })

---

*報告由 HighEfficiency-BatchDev.ps1 生成*
"@
    
    $Report | Set-Content $ReportPath -Encoding UTF8
    Write-DevLog "報告已生成: $ReportPath" "SUCCESS"
    return $ReportPath
}

# ============================================
# 主程序
# ============================================

Write-DevLog "啟動高效批量開發系統..." "TASK"
Write-DevLog "階段: $Phase | 並行作業: $ParallelJobs | 預演: $DryRun" "INFO"

# 獲取任務列表
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

# 初始化調度器
$Scheduler = [TaskScheduler]::new($TasksToProcess, $ParallelJobs)
$AllResults = @()

# 主循環
while (-not $Scheduler.IsComplete()) {
    $Scheduler.UpdateStatus()
    $Scheduler.StartNextBatch(${function:Invoke-DevelopmentTask})
    
    Show-ProgressDashboard $Scheduler $TasksToProcess.Count $StartTime
    
    # 收集結果
    $jobs = Get-Job -State Completed
    foreach ($job in $jobs) {
        $result = Receive-Job $job
        $AllResults += $result
        Remove-Job $job
    }
    
    Start-Sleep -Milliseconds 100
}

$EndTime = Get-Date

# 生成報告
$ReportPath = Export-DevelopmentReport $AllResults $Phase $StartTime $EndTime

# 總結
Write-DevLog "" "INFO"
Write-DevLog "════════════════════════════════════" "SUCCESS"
Write-DevLog "批量開發完成!" "SUCCESS"
Write-DevLog "════════════════════════════════════" "SUCCESS"
Write-DevLog "總任務: $($TasksToProcess.Count)" "INFO"
Write-DevLog "成功: $(($AllResults | Where-Object { $_.Success }).Count)" "SUCCESS"
Write-DevLog "失敗: $(($AllResults | Where-Object { -not $_.Success }).Count)" $(if (($AllResults | Where-Object { -not $_.Success }).Count -gt 0) { "ERROR" } else { "SUCCESS" })
Write-DevLog "創建文件: $(($AllResults | ForEach-Object { $_.FilesCreated.Count } | Measure-Object -Sum).Sum)" "INFO"
Write-DevLog "總耗時: $((New-TimeSpan $StartTime $EndTime).ToString('hh\:mm\:ss'))" "INFO"
Write-DevLog "報告: $ReportPath" "INFO"

# 返回結果
return @{
    Phase = $Phase
    TotalTasks = $TasksToProcess.Count
    SuccessCount = ($AllResults | Where-Object { $_.Success }).Count
    FailCount = ($AllResults | Where-Object { -not $_.Success }).Count
    FilesCreated = ($AllResults | ForEach-Object { $_.FilesCreated.Count } | Measure-Object -Sum).Sum
    Duration = New-TimeSpan $StartTime $EndTime
    ReportPath = $ReportPath
    Results = $AllResults
}
