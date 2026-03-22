// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingAntiFallSystem.h"

UMingAntiFallSystem::UMingAntiFallSystem()
    : NextTaskID(1)
    , HighestCommandmentThreshold(5)
    , BaseAtonementReduction(50)
    , RiskWarningThreshold(70)
    , CriticalRiskThreshold(90)
    , bIsInitialized(false)
{
}

void UMingAntiFallSystem::InitializeAntiFallSystem()
{
    if (bIsInitialized)
    {
        return;
    }

    FallStatus = FFallStatusData();
    ActiveAtonementTasks.Empty();
    AtonementHistory.Empty();
    NextTaskID = 1;

    bIsInitialized = true;
}

bool UMingAntiFallSystem::UpdateFallStatus(int32 FallValueDelta, bool bIsEvilStrategy)
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 更新墮落值
    FallStatus.CurrentFallValue = FMath::Max(0, FallStatus.CurrentFallValue + FallValueDelta);

    // 更新使用統計
    if (bIsEvilStrategy)
    {
        FallStatus.ConsecutiveEvilUses++;
        FallStatus.ConsecutiveRighteousUses = 0;
        FallStatus.TotalEvilUses++;
    }
    else
    {
        FallStatus.ConsecutiveRighteousUses++;
        FallStatus.ConsecutiveEvilUses = 0;
        FallStatus.TotalRighteousUses++;
    }

    // 檢查是否應該墮落
    if (ShouldFall())
    {
        ApplyFall();
        return true;
    }

    // 更新風險評估並檢查警告
    UpdateRiskAssessment();
    CheckAndTriggerWarnings();

    return true;
}

bool UMingAntiFallSystem::ShouldFall() const
{
    // 檢查是否達到墮落閾值
    if (FallStatus.CurrentFallValue >= FallStatus.FallThreshold)
    {
        return true;
    }

    // 檢查是否已經在墮落狀態
    if (FallStatus.bIsFallen)
    {
        return false;
    }

    return false;
}

bool UMingAntiFallSystem::ApplyFall()
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (FallStatus.bIsFallen)
    {
        return false;
    }

    FallStatus.bIsFallen = true;
    FallStatus.LastFallTime = FDateTime::Now();

    // 清除連擊
    FallStatus.ConsecutiveRighteousUses = 0;

    // 廣播墮落事件
    OnFallOccurred.Broadcast();

    return true;
}

bool UMingAntiFallSystem::RecoverFromFall()
{
    if (!FallStatus.bIsFallen)
    {
        return false;
    }

    // 檢查是否可以解除墮落
    if (FallStatus.CurrentFallValue >= (FallStatus.FallThreshold / 2))
    {
        // 墮落值仍然太高，無法解除
        return false;
    }

    FallStatus.bIsFallen = false;
    return true;
}

FAtonementTaskData UMingAntiFallSystem::CreateAtonementTask(int32 DifficultyLevel)
{
    if (!bIsInitialized)
    {
        return FAtonementTaskData();
    }

    FAtonementTaskData Task = GenerateAtonementTask(DifficultyLevel);
    ActiveAtonementTasks.Add(Task);

    return Task;
}

bool UMingAntiFallSystem::StartAtonementTask(int32 TaskID)
{
    for (FAtonementTaskData& Task : ActiveAtonementTasks)
    {
        if (Task.TaskID == TaskID)
        {
            if (Task.Status == EAtonementTaskStatus::NotStarted)
            {
                Task.Status = EAtonementTaskStatus::InProgress;
                Task.StartTime = FDateTime::Now();
                return true;
            }
            return false;
        }
    }
    return false;
}

bool UMingAntiFallSystem::UpdateAtonementProgress(int32 TaskID, int32 ProgressDelta)
{
    for (FAtonementTaskData& Task : ActiveAtonementTasks)
    {
        if (Task.TaskID == TaskID)
        {
            if (Task.Status == EAtonementTaskStatus::InProgress)
            {
                Task.ProgressPercent = FMath::Clamp(Task.ProgressPercent + ProgressDelta, 0, 100);
                return true;
            }
            return false;
        }
    }
    return false;
}

bool UMingAntiFallSystem::CompleteAtonementTask(int32 TaskID)
{
    int32 TaskIndex = -1;
    for (int32 i = 0; i < ActiveAtonementTasks.Num(); ++i)
    {
        if (ActiveAtonementTasks[i].TaskID == TaskID)
        {
            TaskIndex = i;
            break;
        }
    }

    if (TaskIndex < 0)
    {
        return false;
    }

    FAtonementTaskData& Task = ActiveAtonementTasks[TaskIndex];
    
    if (Task.Status != EAtonementTaskStatus::InProgress)
    {
        return false;
    }

    // 計算減少的墮落值
    int32 FallReduction = CalculateAtonementEffect(Task);

    // 更新任務狀態
    Task.Status = EAtonementTaskStatus::Completed;
    Task.CompletionTime = FDateTime::Now();
    Task.ProgressPercent = 100;

    // 減少墮落值
    FallStatus.CurrentFallValue = FMath::Max(0, FallStatus.CurrentFallValue - FallReduction);
    FallStatus.AtonementCount++;

    // 移動到歷史
    AtonementHistory.Add(Task);
    ActiveAtonementTasks.RemoveAt(TaskIndex);

    // 檢查是否可以解除墮落
    if (FallStatus.bIsFallen && FallStatus.CurrentFallValue < (FallStatus.FallThreshold / 2))
    {
        RecoverFromFall();
    }

    // 廣播贖罪完成事件
    OnAtonementCompleted.Broadcast(FallReduction);

    return true;
}

int32 UMingAntiFallSystem::GetFallRiskPercent() const
{
    if (FallStatus.FallThreshold <= 0)
    {
        return 0;
    }

    return FMath::Clamp((FallStatus.CurrentFallValue * 100) / FallStatus.FallThreshold, 0, 100);
}

FString UMingAntiFallSystem::GetRiskLevelDescription() const
{
    int32 RiskPercent = GetFallRiskPercent();

    if (FallStatus.bIsFallen)
    {
        return TEXT("已墮落：無法使用正策，必須完成贖罪任務才能恢復。");
    }

    if (RiskPercent >= CriticalRiskThreshold)
    {
        return FString::Printf(TEXT("極高危險 (%d%%)：即將墮落！立即停止所有逆策，執行正策或贖罪！"), RiskPercent);
    }
    else if (RiskPercent >= RiskWarningThreshold)
    {
        return FString::Printf(TEXT("高風險 (%d%%)：墮落風險較高，建議減少逆策使用，增加正策。"), RiskPercent);
    }
    else if (RiskPercent >= 50)
    {
        return FString::Printf(TEXT("中等風險 (%d%%)：需要注意正逆平衡。"), RiskPercent);
    }
    else if (RiskPercent >= 30)
    {
        return FString::Printf(TEXT("低風險 (%d%%)：處於安全範圍，但仍需警惕。"), RiskPercent);
    }
    else
    {
        return FString::Printf(TEXT("安全 (%d%%)：當前無墮落風險。"), RiskPercent);
    }
}

TArray<FAtonementTaskData> UMingAntiFallSystem::GetActiveAtonementTasks() const
{
    return ActiveAtonementTasks;
}

TArray<FAtonementTaskData> UMingAntiFallSystem::GetAtonementHistory() const
{
    return AtonementHistory;
}

bool UMingAntiFallSystem::HasReachedHighestCommandment() const
{
    return FallStatus.ConsecutiveEvilUses >= HighestCommandmentThreshold;
}

bool UMingAntiFallSystem::EnforceHighestCommandment()
{
    if (!HasReachedHighestCommandment())
    {
        return false;
    }

    // 強制墮落
    FallStatus.CurrentFallValue = FallStatus.FallThreshold;
    FallStatus.ConsecutiveEvilUses = 0;

    ApplyFall();

    // 廣播最高戒律觸發事件
    OnHighestCommandmentTriggered.Broadcast();

    return true;
}

FAtonementTaskData UMingAntiFallSystem::GenerateAtonementTask(int32 DifficultyLevel)
{
    FAtonementTaskData Task;
    Task.TaskID = NextTaskID++;
    Task.DifficultyLevel = FMath::Clamp(DifficultyLevel, 1, 5);

    // 根據難度生成任務
    switch (Task.DifficultyLevel)
    {
    case 1:
        Task.TaskName = TEXT("基礎贖罪");
        Task.TaskDescription = TEXT("執行3次正策，恢復部隊士氣。");
        Task.TargetFallReduction = BaseAtonementReduction / 2;
        break;
    case 2:
        Task.TaskName = TEXT("普通贖罪");
        Task.TaskDescription = TEXT("執行5次正策，並在戰鬥中保護無辜平民。");
        Task.TargetFallReduction = BaseAtonementReduction;
        break;
    case 3:
        Task.TaskName = TEXT("困難贖罪");
        Task.TaskDescription = TEXT("執行7次正策，幫助盟友獲得勝利，修復聲譽。");
        Task.TargetFallReduction = BaseAtonementReduction * 3 / 2;
        break;
    case 4:
        Task.TaskName = TEXT("專家贖罪");
        Task.TaskDescription = TEXT("執行10次正策，公開承認錯誤，大幅修復聲譽。");
        Task.TargetFallReduction = BaseAtonementReduction * 2;
        break;
    case 5:
        Task.TaskName = TEXT("傳奇贖罪");
        Task.TaskDescription = TEXT("執行15次正策，完成一次英雄的自我犧牲行為，徹底洗清罪孽。");
        Task.TargetFallReduction = BaseAtonementReduction * 3;
        break;
    }

    Task.Status = EAtonementTaskStatus::NotStarted;
    Task.ProgressPercent = 0;

    return Task;
}

int32 UMingAntiFallSystem::CalculateAtonementEffect(const FAtonementTaskData& Task) const
{
    // 基礎減少量
    int32 BaseReduction = Task.TargetFallReduction;

    // 根據完成質量調整 (進度百分比)
    float QualityMultiplier = Task.ProgressPercent / 100.0f;

    return FMath::RoundToInt(BaseReduction * QualityMultiplier);
}

void UMingAntiFallSystem::UpdateRiskAssessment()
{
    // 風險評估已經在GetFallRiskPercent中實現
    // 這裡可以添加額外的風險計算邏輯
}

void UMingAntiFallSystem::CheckAndTriggerWarnings()
{
    int32 RiskPercent = GetFallRiskPercent();

    // 檢查是否達到警告閾值
    if (RiskPercent >= RiskWarningThreshold)
    {
        OnFallRiskWarning.Broadcast(RiskPercent);
    }

    // 檢查是否達到最高戒律
    if (HasReachedHighestCommandment())
    {
        EnforceHighestCommandment();
    }
}
