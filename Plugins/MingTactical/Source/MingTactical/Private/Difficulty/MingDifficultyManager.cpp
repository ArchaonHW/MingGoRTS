#include "Difficulty/MingDifficultyManager.h"
#include "Difficulty/MingPlayerPerformanceTracker.h"
#include "MingCoreEventBus.h"
#include "Misc/ConfigCacheIni.h"

TMap<EDifficultyLevel, FMingPresetDifficultyConfig> UMingDifficultyManager::CachedPresetConfigs;

UMingDifficultyManager::UMingDifficultyManager()
    : bIsInitialized(false)
    , bIsLocked(false)
    , bIsTransitioning(false)
    , TransitionProgress(0.0f)
    , TimeSinceLastEvaluation(0.0f)
    , MaxHistorySize(10)
{
    // 初始化默认设置
    CurrentSettings.InitializeFromPreset(EDifficultyLevel::Normal);
    TargetSettings = CurrentSettings;
    TransitionStartSettings = CurrentSettings;
}

void UMingDifficultyManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 尝试加载保存的设置
    LoadDifficultySettings();
    
    SetupEventSubscriptions();
    
    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("DifficultyManager initialized - Level: %s"), 
        *GetCurrentDifficultyDisplayName());
}

void UMingDifficultyManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 保存当前设置
    SaveDifficultySettings();
    
    CleanupEventSubscriptions();
    
    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("DifficultyManager shutdown"));
}

void UMingDifficultyManager::Tick(float DeltaTime)
{
    if (!bIsInitialized || bIsLocked)
    {
        return;
    }

    // 更新过渡
    if (bIsTransitioning)
    {
        UpdateTransition(DeltaTime);
    }

    // 评估是否需要调整难度
    if (CurrentSettings.bEnableDynamicAdjustment && !bIsTransitioning)
    {
        TimeSinceLastEvaluation += DeltaTime;
        
        if (TimeSinceLastEvaluation >= CurrentSettings.EvaluationIntervalSeconds)
        {
            PerformDifficultyEvaluation();
            TimeSinceLastEvaluation = 0.0f;
        }
    }
}

void UMingDifficultyManager::SetDifficultyLevel(EDifficultyLevel NewLevel)
{
    if (CurrentSettings.CurrentLevel == NewLevel)
    {
        return;
    }

    EDifficultyLevel OldLevel = CurrentSettings.CurrentLevel;
    
    // 如果是自定义难度，保留当前参数
    if (NewLevel == EDifficultyLevel::Custom)
    {
        CurrentSettings.CurrentLevel = NewLevel;
    }
    else
    {
        // 从预设配置初始化
        CurrentSettings.InitializeFromPreset(NewLevel);
    }
    
    TargetSettings = CurrentSettings;
    
    // 触发事件
    FDifficultyChangedEvent Event;
    Event.OldLevel = OldLevel;
    Event.NewLevel = NewLevel;
    Event.Reason = TEXT("手动难度切换");
    OnDifficultyChanged.Broadcast(Event);
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty changed from %s to %s"),
        *UEnum::GetValueAsString(OldLevel), *UEnum::GetValueAsString(NewLevel));
}

EDifficultyLevel UMingDifficultyManager::GetCurrentDifficultyLevel() const
{
    return CurrentSettings.CurrentLevel;
}

FString UMingDifficultyManager::GetCurrentDifficultyDisplayName() const
{
    FMingPresetDifficultyConfig Config = GetPresetConfig(CurrentSettings.CurrentLevel);
    return Config.DisplayName;
}

TArray<EDifficultyLevel> UMingDifficultyManager::GetAvailableDifficultyLevels()
{
    TArray<EDifficultyLevel> Levels;
    Levels.Add(EDifficultyLevel::Easy);
    Levels.Add(EDifficultyLevel::Normal);
    Levels.Add(EDifficultyLevel::Hard);
    Levels.Add(EDifficultyLevel::Expert);
    Levels.Add(EDifficultyLevel::Custom);
    return Levels;
}

FMingPresetDifficultyConfig UMingDifficultyManager::GetPresetConfig(EDifficultyLevel Level) const
{
    return FMingDifficultySettings::GetPresetConfig(Level);
}

void UMingDifficultyManager::SetDynamicAdjustmentEnabled(bool bEnabled)
{
    CurrentSettings.bEnableDynamicAdjustment = bEnabled;
    
    if (!bEnabled)
    {
        UE_LOG(LogTemp, Log, TEXT("Dynamic difficulty adjustment disabled"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Dynamic difficulty adjustment enabled"));
    }
}

bool UMingDifficultyManager::IsDynamicAdjustmentEnabled() const
{
    return CurrentSettings.bEnableDynamicAdjustment;
}

void UMingDifficultyManager::ForceDifficultyEvaluation()
{
    PerformDifficultyEvaluation();
}

void UMingDifficultyManager::LockDifficulty(bool bLocked)
{
    bIsLocked = bLocked;
    
    if (bLocked)
    {
        UE_LOG(LogTemp, Log, TEXT("Difficulty locked - auto adjustment disabled"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Difficulty unlocked"));
    }
}

bool UMingDifficultyManager::IsDifficultyLocked() const
{
    return bIsLocked;
}

float UMingDifficultyManager::GetParameterValue(EDifficultyParameter Parameter) const
{
    return CurrentSettings.GetParameterValue(Parameter);
}

void UMingDifficultyManager::SetParameterValue(EDifficultyParameter Parameter, float Value)
{
    CurrentSettings.SetParameterValue(Parameter, Value);
    CurrentSettings.CurrentLevel = EDifficultyLevel::Custom;
}

bool UMingDifficultyManager::AdjustParameter(EDifficultyParameter Parameter, float Delta)
{
    bool bHitBoundary = CurrentSettings.AdjustParameter(Parameter, Delta);
    
    if (!bHitBoundary)
    {
        CurrentSettings.CurrentLevel = EDifficultyLevel::Custom;
    }
    
    return !bHitBoundary;
}

FMingDifficultySettings UMingDifficultyManager::GetDifficultySettings() const
{
    return CurrentSettings;
}

void UMingDifficultyManager::ApplyDifficultySettings(const FMingDifficultySettings& Settings)
{
    // 启动平滑过渡
    if (CurrentSettings.AdjustmentSmoothingFactor > 0.0f && !bIsTransitioning)
    {
        TransitionStartSettings = CurrentSettings;
        TargetSettings = Settings;
        bIsTransitioning = true;
        TransitionProgress = 0.0f;
        
        UE_LOG(LogTemp, Log, TEXT("Starting difficulty transition..."));
    }
    else
    {
        CurrentSettings = Settings;
        UE_LOG(LogTemp, Log, TEXT("Difficulty settings applied immediately"));
    }
}

void UMingDifficultyManager::AdjustAIDifficulty(float NewReactionSpeed, float NewIntelligence)
{
    SetParameterValue(EDifficultyParameter::AIReactionSpeed, NewReactionSpeed);
    SetParameterValue(EDifficultyParameter::AIIntelligence, NewIntelligence);
}

float UMingDifficultyManager::GetAIDifficultyMultiplier() const
{
    float SpeedMult = GetParameterValue(EDifficultyParameter::AIReactionSpeed);
    float IntelMult = GetParameterValue(EDifficultyParameter::AIIntelligence);
    float StrengthMult = GetParameterValue(EDifficultyParameter::AIUnitStrength);
    
    return (SpeedMult + IntelMult + StrengthMult) / 3.0f;
}

void UMingDifficultyManager::AdjustResourceGeneration(float NewCollectionRate, float NewConsumptionRate)
{
    SetParameterValue(EDifficultyParameter::ResourceCollection, NewCollectionRate);
    SetParameterValue(EDifficultyParameter::ResourceConsumption, NewConsumptionRate);
}

float UMingDifficultyManager::GetResourceMultiplier() const
{
    float CollectionMult = GetParameterValue(EDifficultyParameter::ResourceCollection);
    float ConsumptionMult = GetParameterValue(EDifficultyParameter::ResourceConsumption);
    
    return CollectionMult / FMath::Max(0.1f, ConsumptionMult);
}

void UMingDifficultyManager::AdjustMissionDifficulty(float NewTimeLimit, float NewObjectiveScale)
{
    SetParameterValue(EDifficultyParameter::MissionTimeLimit, NewTimeLimit);
    SetParameterValue(EDifficultyParameter::MissionObjective, NewObjectiveScale);
}

float UMingDifficultyManager::GetMissionDifficultyMultiplier() const
{
    float TimeMult = GetParameterValue(EDifficultyParameter::MissionTimeLimit);
    float ObjectiveMult = GetParameterValue(EDifficultyParameter::MissionObjective);
    
    // 时间越短、目标越难，整体难度越高
    return ObjectiveMult / FMath::Max(0.1f, TimeMult);
}

bool UMingDifficultyManager::IsTransitioning() const
{
    return bIsTransitioning;
}

float UMingDifficultyManager::GetTransitionProgress() const
{
    return TransitionProgress;
}

void UMingDifficultyManager::AbortTransition()
{
    if (bIsTransitioning)
    {
        bIsTransitioning = false;
        TransitionProgress = 0.0f;
        TargetSettings = CurrentSettings;
        
        UE_LOG(LogTemp, Warning, TEXT("Difficulty transition aborted"));
    }
}

void UMingDifficultyManager::SetPlayerNotificationEnabled(bool bEnabled)
{
    CurrentSettings.bNotifyPlayerOnChange = bEnabled;
}

void UMingDifficultyManager::SendDifficultyNotification(const FString& Message, float Duration)
{
    if (!CurrentSettings.bNotifyPlayerOnChange)
    {
        return;
    }

    // 这里应该调用UI系统显示通知
    // 暂时使用日志输出
    UE_LOG(LogTemp, Log, TEXT("[Difficulty Notification] %s"), *Message);
    
    // 发布难度通知事件
    // IMingCoreEventBus::PublishEvent<FDifficultyNotificationEvent>(Message, Duration);
}

void UMingDifficultyManager::SaveDifficultySettings()
{
    // 保存到配置文件
    FString ConfigPath = FPaths::ProjectSavedDir() + TEXT("DifficultySettings.ini");
    
    GConfig->SetInt(TEXT("Difficulty"), TEXT("CurrentLevel"), 
        static_cast<int32>(CurrentSettings.CurrentLevel), ConfigPath);
    GConfig->SetBool(TEXT("Difficulty"), TEXT("DynamicAdjustment"), 
        CurrentSettings.bEnableDynamicAdjustment, ConfigPath);
    GConfig->SetFloat(TEXT("Difficulty"), TEXT("EvaluationInterval"), 
        CurrentSettings.EvaluationIntervalSeconds, ConfigPath);
    GConfig->SetBool(TEXT("Difficulty"), TEXT("NotifyPlayer"), 
        CurrentSettings.bNotifyPlayerOnChange, ConfigPath);
    
    // 保存各个参数值
    for (const auto& Param : CurrentSettings.DynamicParameters)
    {
        FString KeyName = FString::Printf(TEXT("Param_%d"), 
            static_cast<int32>(Param.ParameterType));
        GConfig->SetFloat(TEXT("Difficulty"), *KeyName, 
            Param.CurrentValue, ConfigPath);
    }
    
    GConfig->Flush(false, ConfigPath);
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty settings saved to %s"), *ConfigPath);
}

void UMingDifficultyManager::LoadDifficultySettings()
{
    FString ConfigPath = FPaths::ProjectSavedDir() + TEXT("DifficultySettings.ini");
    
    if (!FPaths::FileExists(ConfigPath))
    {
        UE_LOG(LogTemp, Log, TEXT("No saved difficulty settings found, using defaults"));
        return;
    }
    
    int32 SavedLevel = 0;
    GConfig->GetInt(TEXT("Difficulty"), TEXT("CurrentLevel"), SavedLevel, ConfigPath);
    
    bool bDynamic = true;
    GConfig->GetBool(TEXT("Difficulty"), TEXT("DynamicAdjustment"), bDynamic, ConfigPath);
    
    float Interval = 60.0f;
    GConfig->GetFloat(TEXT("Difficulty"), TEXT("EvaluationInterval"), Interval, ConfigPath);
    
    bool bNotify = true;
    GConfig->GetBool(TEXT("Difficulty"), TEXT("NotifyPlayer"), bNotify, ConfigPath);
    
    // 应用加载的设置
    CurrentSettings.CurrentLevel = static_cast<EDifficultyLevel>(SavedLevel);
    CurrentSettings.bEnableDynamicAdjustment = bDynamic;
    CurrentSettings.EvaluationIntervalSeconds = Interval;
    CurrentSettings.bNotifyPlayerOnChange = bNotify;
    
    // 如果是自定义难度，加载保存的参数值
    if (CurrentSettings.CurrentLevel == EDifficultyLevel::Custom)
    {
        for (auto& Param : CurrentSettings.DynamicParameters)
        {
            FString KeyName = FString::Printf(TEXT("Param_%d"), 
                static_cast<int32>(Param.ParameterType));
            float SavedValue = 1.0f;
            GConfig->GetFloat(TEXT("Difficulty"), *KeyName, SavedValue, ConfigPath);
            Param.CurrentValue = SavedValue;
        }
    }
    else
    {
        // 非自定义难度，重新初始化预设
        CurrentSettings.InitializeFromPreset(CurrentSettings.CurrentLevel);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty settings loaded from %s"), *ConfigPath);
}

void UMingDifficultyManager::ResetToDefault()
{
    CurrentSettings = FMingDifficultySettings();
    TargetSettings = CurrentSettings;
    TransitionStartSettings = CurrentSettings;
    
    bIsTransitioning = false;
    TransitionProgress = 0.0f;
    
    AdjustmentHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty settings reset to default"));
}

void UMingDifficultyManager::PrintDebugInfo()
{
    UE_LOG(LogTemp, Log, TEXT("=== Difficulty Manager Debug Info ==="));
    UE_LOG(LogTemp, Log, TEXT("Current Level: %s"), *GetCurrentDifficultyDisplayName());
    UE_LOG(LogTemp, Log, TEXT("Dynamic Adjustment: %s"), 
        CurrentSettings.bEnableDynamicAdjustment ? TEXT("Enabled") : TEXT("Disabled"));
    UE_LOG(LogTemp, Log, TEXT("Locked: %s"), bIsLocked ? TEXT("Yes") : TEXT("No"));
    UE_LOG(LogTemp, Log, TEXT("Transitioning: %s"), bIsTransitioning ? TEXT("Yes") : TEXT("No"));
    UE_LOG(LogTemp, Log, TEXT("AI Multiplier: %.2f"), GetAIDifficultyMultiplier());
    UE_LOG(LogTemp, Log, TEXT("Resource Multiplier: %.2f"), GetResourceMultiplier());
    UE_LOG(LogTemp, Log, TEXT("Mission Multiplier: %.2f"), GetMissionDifficultyMultiplier());
    UE_LOG(LogTemp, Log, TEXT("Adjustment History: %d entries"), AdjustmentHistory.Num());
    UE_LOG(LogTemp, Log, TEXT("===================================="));
}

FString UMingDifficultyManager::GetDebugString() const
{
    return FString::Printf(TEXT("Level: %s | AI: %.2f | Res: %.2f | Mission: %.2f"),
        *GetCurrentDifficultyDisplayName(),
        GetAIDifficultyMultiplier(),
        GetResourceMultiplier(),
        GetMissionDifficultyMultiplier());
}

void UMingDifficultyManager::PerformDifficultyEvaluation()
{
    if (!PerformanceTracker.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot evaluate difficulty - PerformanceTracker not available"));
        return;
    }

    // 检查是否有足够数据
    if (!PerformanceTracker->HasSufficientData(CurrentSettings.MinEvaluationSamples))
    {
        UE_LOG(LogTemp, Log, TEXT("Insufficient performance data for difficulty evaluation"));
        return;
    }

    // 检查是否应该防止振荡
    if (ShouldPreventOscillation())
    {
        UE_LOG(LogTemp, Log, TEXT("Preventing difficulty oscillation - skipping adjustment"));
        return;
    }

    // 计算推荐的难度变化
    EDifficultyChangeDirection Recommendation = CalculateRecommendedChange();
    
    if (Recommendation != EDifficultyChangeDirection::Maintain)
    {
        ExecuteDifficultyAdjustment(Recommendation);
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("Difficulty maintained - no adjustment needed"));
    }
}

EDifficultyChangeDirection UMingDifficultyManager::CalculateRecommendedChange() const
{
    if (!PerformanceTracker.IsValid())
    {
        return EDifficultyChangeDirection::Maintain;
    }

    return PerformanceTracker->GetDifficultyRecommendation();
}

void UMingDifficultyManager::ExecuteDifficultyAdjustment(EDifficultyChangeDirection Direction)
{
    if (Direction == EDifficultyChangeDirection::Maintain)
    {
        return;
    }

    FString Reason;
    EDifficultyLevel OldLevel = CurrentSettings.CurrentLevel;
    
    // 准备新的目标设置
    FMingDifficultySettings NewSettings = CurrentSettings;
    
    if (Direction == EDifficultyChangeDirection::Increase)
    {
        // 增加难度 - 调整各项参数
        Reason = TEXT("玩家表现优秀 - 增加挑战");
        
        // AI 参数增加
        NewSettings.AdjustParameter(EDifficultyParameter::AIIntelligence, 0.1f);
        NewSettings.AdjustParameter(EDifficultyParameter::AIReactionSpeed, 0.1f);
        
        // 资源参数调整
        NewSettings.AdjustParameter(EDifficultyParameter::ResourceCollection, -0.05f);
        NewSettings.AdjustParameter(EDifficultyParameter::ResourceConsumption, 0.05f);
        
        // 战斗参数调整
        NewSettings.AdjustParameter(EDifficultyParameter::PlayerDamageTaken, 0.05f);
        NewSettings.AdjustParameter(EDifficultyParameter::PlayerDamageDealt, -0.05f);
        
        // 尝试提升到更高难度等级
        switch (OldLevel)
        {
        case EDifficultyLevel::Easy:
            NewSettings.InitializeFromPreset(EDifficultyLevel::Normal);
            break;
        case EDifficultyLevel::Normal:
            NewSettings.InitializeFromPreset(EDifficultyLevel::Hard);
            break;
        default:
            // 已经是Hard或Expert，保持在动态调整模式
            NewSettings.CurrentLevel = EDifficultyLevel::Custom;
            break;
        }
    }
    else // Decrease
    {
        // 降低难度
        Reason = TEXT("玩家遇到困难 - 降低难度");
        
        // AI 参数降低
        NewSettings.AdjustParameter(EDifficultyParameter::AIIntelligence, -0.1f);
        NewSettings.AdjustParameter(EDifficultyParameter::AIReactionSpeed, -0.1f);
        
        // 资源参数调整
        NewSettings.AdjustParameter(EDifficultyParameter::ResourceCollection, 0.05f);
        NewSettings.AdjustParameter(EDifficultyParameter::ResourceConsumption, -0.05f);
        
        // 战斗参数调整
        NewSettings.AdjustParameter(EDifficultyParameter::PlayerDamageTaken, -0.05f);
        NewSettings.AdjustParameter(EDifficultyParameter::PlayerDamageDealt, 0.05f);
        
        // 尝试降低到更低难度等级
        switch (OldLevel)
        {
        case EDifficultyLevel::Expert:
            NewSettings.InitializeFromPreset(EDifficultyLevel::Hard);
            break;
        case EDifficultyLevel::Hard:
            NewSettings.InitializeFromPreset(EDifficultyLevel::Normal);
            break;
        case EDifficultyLevel::Normal:
            NewSettings.InitializeFromPreset(EDifficultyLevel::Easy);
            break;
        default:
            // 已经是Easy，保持在动态调整模式
            NewSettings.CurrentLevel = EDifficultyLevel::Custom;
            break;
        }
    }

    // 应用新设置（带平滑过渡）
    ApplyDifficultySettings(NewSettings);
    
    // 记录历史
    FDifficultyChangedEvent Event;
    Event.OldLevel = OldLevel;
    Event.NewLevel = NewSettings.CurrentLevel;
    Event.Reason = Reason;
    AdjustmentHistory.Add(Event);
    
    // 限制历史大小
    if (AdjustmentHistory.Num() > MaxHistorySize)
    {
        AdjustmentHistory.RemoveAt(0);
    }
    
    // 发送通知
    if (CurrentSettings.bNotifyPlayerOnChange)
    {
        SendDifficultyNotification(FString::Printf(TEXT("难度已调整: %s"), *Reason), 5.0f);
    }
    
    // 广播事件
    OnDifficultyChanged.Broadcast(Event);
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty adjusted - %s"), *Reason);
}

void UMingDifficultyManager::UpdateTransition(float DeltaTime)
{
    if (!bIsTransitioning)
    {
        return;
    }

    // 基于平滑因子计算进度增量
    float ProgressIncrement = DeltaTime * (1.0f / FMath::Max(0.1f, CurrentSettings.AdjustmentSmoothingFactor));
    TransitionProgress += ProgressIncrement * DeltaTime;

    if (TransitionProgress >= 1.0f)
    {
        CompleteTransition();
        return;
    }

    // 使用缓动函数计算当前插值
    float EasedProgress = FMath::InterpEaseInOut(0.0f, 1.0f, TransitionProgress, 2.0f);

    // 对每个参数进行插值
    for (auto& CurrentParam : CurrentSettings.DynamicParameters)
    {
        const FMingDifficultyParameter* TargetParam = TargetSettings.GetParameter(CurrentParam.ParameterType);
        const FMingDifficultyParameter* StartParam = TransitionStartSettings.GetParameter(CurrentParam.ParameterType);
        
        if (TargetParam && StartParam)
        {
            CurrentParam.CurrentValue = FMath::Lerp(
                StartParam->CurrentValue,
                TargetParam->CurrentValue,
                EasedProgress);
        }
    }
}

void UMingDifficultyManager::CompleteTransition()
{
    bIsTransitioning = false;
    TransitionProgress = 1.0f;
    
    // 应用最终目标值
    CurrentSettings = TargetSettings;
    
    UE_LOG(LogTemp, Log, TEXT("Difficulty transition completed"));
    
    if (CurrentSettings.bNotifyPlayerOnChange)
    {
        SendDifficultyNotification(TEXT("难度调整完成"), 3.0f);
    }
}

void UMingDifficultyManager::BroadcastDifficultyChanged(const FString& Reason)
{
    FDifficultyChangedEvent Event;
    Event.Reason = Reason;
    OnDifficultyChanged.Broadcast(Event);
}

bool UMingDifficultyManager::ShouldPreventOscillation() const
{
    if (AdjustmentHistory.Num() < 3)
    {
        return false;
    }

    // 检查最近几次调整是否出现振荡模式
    int32 RecentChanges = 0;
    int32 Increases = 0;
    int32 Decreases = 0;

    for (int32 i = AdjustmentHistory.Num() - 3; i < AdjustmentHistory.Num(); ++i)
    {
        if (i >= 0)
        {
            RecentChanges++;
            
            // 分析难度变化方向
            int32 LevelDiff = static_cast<int32>(AdjustmentHistory[i].NewLevel) - 
                static_cast<int32>(AdjustmentHistory[i].OldLevel);
            
            if (LevelDiff > 0)
            {
                Increases++;
            }
            else if (LevelDiff < 0)
            {
                Decreases++;
            }
        }
    }

    // 如果最近几次调整中有增加也有减少，可能存在振荡
    if (Increases > 0 && Decreases > 0)
    {
        float OscillationRatio = static_cast<float>(FMath::Min(Increases, Decreases)) / RecentChanges;
        
        if (OscillationRatio >= CurrentSettings.OscillationPreventionThreshold)
        {
            UE_LOG(LogTemp, Warning, TEXT("Detected oscillation pattern - Increases: %d, Decreases: %d"),
                Increases, Decreases);
            return true;
        }
    }

    return false;
}

void UMingDifficultyManager::SetupEventSubscriptions()
{
    UE_LOG(LogTemp, Log, TEXT("DifficultyManager event subscriptions setup"));
}

void UMingDifficultyManager::CleanupEventSubscriptions()
{
    UE_LOG(LogTemp, Log, TEXT("DifficultyManager event subscriptions cleaned up"));
}
