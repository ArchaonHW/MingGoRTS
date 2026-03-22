#include "EOSImplementationManager.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"

UEOSImplementationManager::UEOSImplementationManager()
    : CurrentPhase(EOSImplementationPhase::Planning)
    , OverallProgress(0.0f)
{
}

void UEOSImplementationManager::InitializeImplementationManager()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing EOS Implementation Manager"));
    
    // 初始化階段信息
    InitializePhaseInfos();
    
    // 記錄實施開始時間
    ImplementationStartTime = FDateTime::Now();
    
    // 計算初始進度
    UpdateOverallProgress();
    
    UE_LOG(LogTemp, Log, TEXT("EOS Implementation Manager initialized successfully"));
}

void UEOSImplementationManager::InitializePhaseInfos()
{
    PhaseInfos.Empty();
    
    // 階段一：基礎內核開發 (4週)
    FEOSPhaseInfo KernelPhase;
    KernelPhase.Phase = EOSImplementationPhase::Kernel;
    KernelPhase.PhaseName = TEXT("基礎內核開發");
    KernelPhase.Description = TEXT("開發作業系統核心組件，包括內核、進程管理、內存管理和文件系統");
    KernelPhase.DurationWeeks = 4;
    KernelPhase.Status = EOSImplementationStatus::NotStarted;
    KernelPhase.OverallProgress = 0.0f;
    
    // 內核開發任務
    FEOSPhaseTask KernelTask1;
    KernelTask1.TaskName = TEXT("內核框架設計");
    KernelTask1.Description = TEXT("設計作業系統內核架構和核心接口");
    KernelTask1.EstimatedDays = 5;
    KernelTask1.Status = EOSImplementationStatus::NotStarted;
    KernelTask1.AssignedDeveloper = TEXT("核心開發團隊");
    KernelTask1.ProgressPercentage = 0.0f;
    
    FEOSPhaseTask KernelTask2;
    KernelTask2.TaskName = TEXT("進程管理器實現");
    KernelTask2.Description = TEXT("實現進程創建、調度和終止功能");
    KernelTask2.EstimatedDays = 8;
    KernelTask2.Status = EOSImplementationStatus::NotStarted;
    KernelTask2.AssignedDeveloper = TEXT("系統開發團隊");
    KernelTask2.ProgressPercentage = 0.0f;
    KernelTask2.Dependencies.Add(TEXT("內核框架設計"));
    
    FEOSPhaseTask KernelTask3;
    KernelTask3.TaskName = TEXT("內存管理器實現");
    KernelTask3.Description = TEXT("實現內存分配、回收和優化功能");
    KernelTask3.EstimatedDays = 7;
    KernelTask3.Status = EOSImplementationStatus::NotStarted;
    KernelTask3.AssignedDeveloper = TEXT("系統開發團隊");
    KernelTask3.ProgressPercentage = 0.0f;
    KernelTask3.Dependencies.Add(TEXT("內核框架設計"));
    
    KernelPhase.Tasks.Add(KernelTask1);
    KernelPhase.Tasks.Add(KernelTask2);
    KernelPhase.Tasks.Add(KernelTask3);
    
    // 階段二：系統服務框架 (3週)
    FEOSPhaseInfo ServicesPhase;
    ServicesPhase.Phase = EOSImplementationPhase::Services;
    ServicesPhase.PhaseName = TEXT("系統服務框架");
    ServicesPhase.Description = TEXT("將現有管理器轉換為系統服務，建立服務管理機制");
    ServicesPhase.DurationWeeks = 3;
    ServicesPhase.Status = EOSImplementationStatus::NotStarted;
    ServicesPhase.OverallProgress = 0.0f;
    
    // 系統服務任務
    FEOSPhaseTask ServiceTask1;
    ServiceTask1.TaskName = TEXT("服務管理器開發");
    ServiceTask1.Description = TEXT("開發系統服務註冊、發現和管理機制");
    ServiceTask1.EstimatedDays = 6;
    ServiceTask1.Status = EOSImplementationStatus::NotStarted;
    ServiceTask1.AssignedDeveloper = TEXT("架構團隊");
    ServiceTask1.ProgressPercentage = 0.0f;
    
    FEOSPhaseTask ServiceTask2;
    ServiceTask2.TaskName = TEXT("現有系統服務化");
    ServiceTask2.Description = TEXT("將50+現有管理器轉換為系統服務");
    ServiceTask2.EstimatedDays = 9;
    ServiceTask2.Status = EOSImplementationStatus::NotStarted;
    ServiceTask2.AssignedDeveloper = TEXT("系統團隊");
    ServiceTask2.ProgressPercentage = 0.0f;
    ServiceTask2.Dependencies.Add(TEXT("服務管理器開發"));
    
    ServicesPhase.Tasks.Add(ServiceTask1);
    ServicesPhase.Tasks.Add(ServiceTask2);
    
    // 階段三：高級功能開發 (3週)
    FEOSPhaseInfo AdvancedPhase;
    AdvancedPhase.Phase = EOSImplementationPhase::Advanced;
    AdvancedPhase.PhaseName = TEXT("高級功能開發");
    AdvancedPhase.Description = TEXT("實現安全系統、網絡系統和用戶界面系統");
    AdvancedPhase.DurationWeeks = 3;
    AdvancedPhase.Status = EOSImplementationStatus::NotStarted;
    AdvancedPhase.OverallProgress = 0.0f;
    
    // 高級功能任務
    FEOSPhaseTask AdvancedTask1;
    AdvancedTask1.TaskName = TEXT("安全系統實現");
    AdvancedTask1.Description = TEXT("實現進程隔離、權限管理和安全策略");
    AdvancedTask1.EstimatedDays = 8;
    AdvancedTask1.Status = EOSImplementationStatus::NotStarted;
    AdvancedTask1.AssignedDeveloper = TEXT("安全團隊");
    AdvancedTask1.ProgressPercentage = 0.0f;
    
    FEOSPhaseTask AdvancedTask2;
    AdvancedTask2.TaskName = TEXT("網絡系統開發");
    AdvancedTask2.Description = TEXT("實現進程間通信和網絡協議支持");
    AdvancedTask2.EstimatedDays = 7;
    AdvancedTask2.Status = EOSImplementationStatus::NotStarted;
    AdvancedTask2.AssignedDeveloper = TEXT("網絡團隊");
    AdvancedTask2.ProgressPercentage = 0.0f;
    
    AdvancedPhase.Tasks.Add(AdvancedTask1);
    AdvancedPhase.Tasks.Add(AdvancedTask2);
    
    // 階段四：優化和集成 (2週)
    FEOSPhaseInfo IntegrationPhase;
    IntegrationPhase.Phase = EOSImplementationPhase::Integration;
    IntegrationPhase.PhaseName = TEXT("優化和集成");
    IntegrationPhase.Description = TEXT("性能優化、集成測試和文檔編寫");
    IntegrationPhase.DurationWeeks = 2;
    IntegrationPhase.Status = EOSImplementationStatus::NotStarted;
    IntegrationPhase.OverallProgress = 0.0f;
    
    // 集成任務
    FEOSPhaseTask IntegrationTask1;
    IntegrationTask1.TaskName = TEXT("性能優化");
    IntegrationTask1.Description = TEXT("優化系統性能，確保開銷<5%");
    IntegrationTask1.EstimatedDays = 5;
    IntegrationTask1.Status = EOSImplementationStatus::NotStarted;
    IntegrationTask1.AssignedDeveloper = TEXT("性能團隊");
    IntegrationTask1.ProgressPercentage = 0.0f;
    
    FEOSPhaseTask IntegrationTask2;
    IntegrationTask2.TaskName = TEXT("集成測試");
    IntegrationTask2.Description = TEXT("全面測試系統集成和兼容性");
    IntegrationTask2.EstimatedDays = 5;
    IntegrationTask2.Status = EOSImplementationStatus::NotStarted;
    IntegrationTask2.AssignedDeveloper = TEXT("測試團隊");
    IntegrationTask2.ProgressPercentage = 0.0f;
    
    IntegrationPhase.Tasks.Add(IntegrationTask1);
    IntegrationPhase.Tasks.Add(IntegrationTask2);
    
    // 添加所有階段
    PhaseInfos.Add(EOSImplementationPhase::Planning, FEOSPhaseInfo{EOSImplementationPhase::Planning, TEXT("Planning"), TEXT("規劃和準備階段"), 0, TArray<FEOSPhaseTask>(), 0.0f, EOSImplementationStatus::Completed});
    PhaseInfos.Add(EOSImplementationPhase::Kernel, KernelPhase);
    PhaseInfos.Add(EOSImplementationPhase::Services, ServicesPhase);
    PhaseInfos.Add(EOSImplementationPhase::Advanced, AdvancedPhase);
    PhaseInfos.Add(EOSImplementationPhase::Integration, IntegrationPhase);
    PhaseInfos.Add(EOSImplementationPhase::Complete, FEOSPhaseInfo{EOSImplementationPhase::Complete, TEXT("Complete"), TEXT("實施完成"), 0, TArray<FEOSPhaseTask>(), 100.0f, EOSImplementationStatus::NotStarted});
}

void UEOSImplementationManager::SetCurrentPhase(EOSImplementationPhase NewPhase)
{
    if (CurrentPhase != NewPhase && CanStartPhase(NewPhase))
    {
        EOSImplementationPhase OldPhase = CurrentPhase;
        CurrentPhase = NewPhase;
        
        // 更新階段狀態
        if (PhaseInfos.Contains(NewPhase))
        {
            FEOSPhaseInfo& PhaseInfo = PhaseInfos[NewPhase];
            PhaseInfo.Status = EOSImplementationStatus::InProgress;
            PhaseInfo.StartDate = FDateTime::Now();
        }
        
        UE_LOG(LogTemp, Log, TEXT("EOS Implementation phase changed from %d to %d"), (int32)OldPhase, (int32)NewPhase);
        
        // 觸發事件
        OnPhaseStatusChanged.Broadcast(NewPhase);
    }
}

FEOSPhaseInfo UEOSImplementationManager::GetPhaseInfo(EOSImplementationPhase Phase) const
{
    if (PhaseInfos.Contains(Phase))
    {
        return PhaseInfos[Phase];
    }
    return FEOSPhaseInfo();
}

TArray<FEOSPhaseInfo> UEOSImplementationManager::GetAllPhasesInfo() const
{
    TArray<FEOSPhaseInfo> AllPhases;
    
    for (const auto& PhasePair : PhaseInfos)
    {
        AllPhases.Add(PhasePair.Value);
    }
    
    return AllPhases;
}

void UEOSImplementationManager::UpdateTaskStatus(EOSImplementationPhase Phase, const FString& TaskName, EOSImplementationStatus Status)
{
    if (PhaseInfos.Contains(Phase))
    {
        FEOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
        
        for (FEOSPhaseTask& Task : PhaseInfo.Tasks)
        {
            if (Task.TaskName == TaskName)
            {
                Task.Status = Status;
                
                if (Status == EOSImplementationStatus::InProgress && Task.StartDate == FDateTime::MinValue())
                {
                    Task.StartDate = FDateTime::Now();
                }
                else if (Status == EOSImplementationStatus::Completed)
                {
                    Task.EndDate = FDateTime::Now();
                    Task.ProgressPercentage = 100.0f;
                }
                
                // 更新階段進度
                PhaseInfo.OverallProgress = CalculatePhaseProgress(PhaseInfo);
                UpdateOverallProgress();
                
                // 觸發事件
                OnTaskUpdated.Broadcast(Phase, TaskName);
                OnPhaseProgressUpdated.Broadcast(Phase);
                
                break;
            }
        }
    }
}

void UEOSImplementationManager::UpdateTaskProgress(EOSImplementationPhase Phase, const FString& TaskName, float Progress)
{
    if (PhaseInfos.Contains(Phase))
    {
        FEOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
        
        for (FEOSPhaseTask& Task : PhaseInfo.Tasks)
        {
            if (Task.TaskName == TaskName)
            {
                Task.ProgressPercentage = FMath::Clamp(Progress, 0.0f, 100.0f);
                
                // 更新階段進度
                PhaseInfo.OverallProgress = CalculatePhaseProgress(PhaseInfo);
                UpdateOverallProgress();
                
                // 觸發事件
                OnTaskUpdated.Broadcast(Phase, TaskName);
                OnPhaseProgressUpdated.Broadcast(Phase);
                
                break;
            }
        }
    }
}

float UEOSImplementationManager::GetOverallProgress() const
{
    return OverallProgress;
}

float UEOSImplementationManager::GetPhaseProgress(EOSImplementationPhase Phase) const
{
    if (PhaseInfos.Contains(Phase))
    {
        return PhaseInfos[Phase].OverallProgress;
    }
    return 0.0f;
}

bool UEOSImplementationManager::CanStartPhase(EOSImplementationPhase Phase) const
{
    if (Phase == EOSImplementationPhase::Planning)
    {
        return true; // 規劃階段總是可以開始的
    }
    
    if (!PhaseInfos.Contains(Phase))
    {
        return false;
    }
    
    const FEOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
    
    // 檢查所有依賴的任務是否完成
    for (const FEOSPhaseTask& Task : PhaseInfo.Tasks)
    {
        if (!ValidateTaskDependencies(Task))
        {
            return false;
        }
    }
    
    return true;
}

bool UEOSImplementationManager::StartPhase(EOSImplementationPhase Phase)
{
    if (CanStartPhase(Phase))
    {
        SetCurrentPhase(Phase);
        return true;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Cannot start phase %d - dependencies not met"), (int32)Phase);
    return false;
}

void UEOSImplementationManager::CompletePhase(EOSImplementationPhase Phase)
{
    if (PhaseInfos.Contains(Phase))
    {
        FEOSPhaseInfo& PhaseInfo = PhaseInfos[Phase];
        PhaseInfo.Status = EOSImplementationStatus::Completed;
        PhaseInfo.OverallProgress = 100.0f;
        PhaseInfo.EndDate = FDateTime::Now();
        
        UpdateOverallProgress();
        
        UE_LOG(LogTemp, Log, TEXT("Phase %d completed successfully"), (int32)Phase);
        OnPhaseStatusChanged.Broadcast(Phase);
    }
}

TArray<FString> UEOSImplementationManager::GetRiskAssessment() const
{
    TArray<FString> Risks;
    
    Risks.Add(TEXT("技術複雜度風險：作業系統開發涉及底層系統編程，技術門檻高"));
    Risks.Add(TEXT("性能影響風險：虛擬化可能影響遊戲性能，需要仔細優化"));
    Risks.Add(TEXT("兼容性風險：與現有UE5系統的集成可能遇到兼容性問題"));
    Risks.Add(TEXT("時間壓力風險：12週時間緊張，可能影響開發質量"));
    Risks.Add(TEXT("團隊協作風險：8人團隊需要高效協作，溝通成本高"));
    
    return Risks;
}

TArray<FString> UEOSImplementationManager::GetRecommendations() const
{
    TArray<FString> Recommendations;
    
    Recommendations.Add(TEXT("採用模組化設計，降低系統複雜度"));
    Recommendations.Add(TEXT("建立完善的測試框架，確保系統穩定性"));
    Recommendations.Add(TEXT("實施持續集成，及早發現和解決問題"));
    Recommendations.Add(TEXT("優先開發核心功能，確保基礎功能穩定"));
    Recommendations.Add(TEXT("建立詳細的文檔，便於團隊協作和知識傳承"));
    Recommendations.Add(TEXT("定期進行性能測試，監控系統開銷"));
    Recommendations.Add(TEXT("建立風險監控機制，及時識別和應對風險"));
    
    return Recommendations;
}

FString UEOSImplementationManager::GenerateImplementationReport() const
{
    FString Report;
    Report += TEXT("=== MingGoRTS 內建作業系統實施報告 ===\n\n");
    
    // 基本信息
    FTimespan ElapsedTime = FDateTime::Now() - ImplementationStartTime;
    Report += FString::Printf(TEXT("實施開始時間: %s\n"), *ImplementationStartTime.ToString());
    Report += FString::Printf(TEXT("已用時間: %s\n"), *ElapsedTime.ToString());
    Report += FString::Printf(TEXT("當前階段: %s\n"), *GetPhaseInfo(CurrentPhase).PhaseName);
    Report += FString::Printf(TEXT("整體進度: %.1f%%\n\n"), OverallProgress);
    
    // 階段詳情
    Report += TEXT("=== 階段詳情 ===\n");
    for (const auto& PhasePair : PhaseInfos)
    {
        const FEOSPhaseInfo& PhaseInfo = PhasePair.Value;
        Report += FString::Printf(TEXT("\n%s (%d週)\n"), *PhaseInfo.PhaseName, PhaseInfo.DurationWeeks);
        Report += FString::Printf(TEXT("狀態: %s\n"), *UEnum::GetDisplayValueAsText(PhaseInfo.Status).ToString());
        Report += FString::Printf(TEXT("進度: %.1f%%\n"), PhaseInfo.OverallProgress);
        
        if (PhaseInfo.Tasks.Num() > 0)
        {
            Report += TEXT("任務:\n");
            for (const FEOSPhaseTask& Task : PhaseInfo.Tasks)
            {
                Report += FString::Printf(TEXT("  - %s (%s) - %.1f%%\n"), 
                    *Task.TaskName, 
                    *Task.AssignedDeveloper, 
                    Task.ProgressPercentage);
            }
        }
    }
    
    // 風險評估
    Report += TEXT("\n=== 風險評估 ===\n");
    TArray<FString> Risks = GetRiskAssessment();
    for (int32 i = 0; i < Risks.Num(); ++i)
    {
        Report += FString::Printf(TEXT("%d. %s\n"), i + 1, *Risks[i]);
    }
    
    // 建議
    Report += TEXT("\n=== 建議 ===\n");
    TArray<FString> Recommendations = GetRecommendations();
    for (int32 i = 0; i < Recommendations.Num(); ++i)
    {
        Report += FString::Printf(TEXT("%d. %s\n"), i + 1, *Recommendations[i]);
    }
    
    return Report;
}

float UEOSImplementationManager::CalculatePhaseProgress(const FEOSPhaseInfo& PhaseInfo) const
{
    if (PhaseInfo.Tasks.Num() == 0)
    {
        return PhaseInfo.OverallProgress;
    }
    
    float TotalProgress = 0.0f;
    for (const FEOSPhaseTask& Task : PhaseInfo.Tasks)
    {
        TotalProgress += Task.ProgressPercentage;
    }
    
    return TotalProgress / PhaseInfo.Tasks.Num();
}

bool UEOSImplementationManager::ValidateTaskDependencies(const FEOSPhaseTask& Task) const
{
    for (const FString& Dependency : Task.Dependencies)
    {
        bool DependencyFound = false;
        bool DependencyCompleted = false;
        
        // 在所有階段中查找依賴任務
        for (const auto& PhasePair : PhaseInfos)
        {
            for (const FEOSPhaseTask& OtherTask : PhasePair.Value.Tasks)
            {
                if (OtherTask.TaskName == Dependency)
                {
                    DependencyFound = true;
                    if (OtherTask.Status == EOSImplementationStatus::Completed)
                    {
                        DependencyCompleted = true;
                    }
                    break;
                }
            }
            if (DependencyFound) break;
        }
        
        if (!DependencyFound || !DependencyCompleted)
        {
            return false;
        }
    }
    
    return true;
}

void UEOSImplementationManager::UpdateOverallProgress()
{
    float TotalProgress = 0.0f;
    int32 TotalDuration = 0;
    
    for (const auto& PhasePair : PhaseInfos)
    {
        const FEOSPhaseInfo& PhaseInfo = PhasePair.Value;
        if (PhaseInfo.DurationWeeks > 0)
        {
            TotalProgress += PhaseInfo.OverallProgress * PhaseInfo.DurationWeeks;
            TotalDuration += PhaseInfo.DurationWeeks;
        }
    }
    
    OverallProgress = TotalDuration > 0 ? (TotalProgress / TotalDuration) : 0.0f;
}
