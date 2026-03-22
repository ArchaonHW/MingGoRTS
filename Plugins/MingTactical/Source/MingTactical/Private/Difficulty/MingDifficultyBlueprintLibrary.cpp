#include "Difficulty/MingDifficultyBlueprintLibrary.h"
#include "Difficulty/MingDifficultyManager.h"
#include "Difficulty/MingPlayerPerformanceTracker.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

UMingDifficultyManager* UMingDifficultyBlueprintLibrary::GetDifficultyManager(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    // 尝试从World获取管理器
    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // 这里简化处理 - 实际实现中应该从GameInstance或Subsystem获取
    // 暂时返回一个静态实例
    static UMingDifficultyManager* StaticManager = nullptr;
    if (!StaticManager)
    {
        StaticManager = NewObject<UMingDifficultyManager>();
        StaticManager->Initialize();
    }

    return StaticManager;
}

UMingPlayerPerformanceTracker* UMingDifficultyBlueprintLibrary::GetPerformanceTracker(UObject* WorldContextObject)
{
    // 类似地，获取或创建性能追踪器
    static UMingPlayerPerformanceTracker* StaticTracker = nullptr;
    if (!StaticTracker)
    {
        StaticTracker = NewObject<UMingPlayerPerformanceTracker>();
        StaticTracker->Initialize();
    }

    return StaticTracker;
}

EDifficultyLevel UMingDifficultyBlueprintLibrary::GetCurrentDifficultyLevel(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->GetCurrentDifficultyLevel();
    }
    return EDifficultyLevel::Normal;
}

void UMingDifficultyBlueprintLibrary::SetDifficultyLevel(UObject* WorldContextObject, EDifficultyLevel NewLevel)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->SetDifficultyLevel(NewLevel);
    }
}

FString UMingDifficultyBlueprintLibrary::GetDifficultyDisplayName(EDifficultyLevel Level)
{
    FMingPresetDifficultyConfig Config = FMingDifficultySettings::GetPresetConfig(Level);
    return Config.DisplayName;
}

TArray<EDifficultyLevel> UMingDifficultyBlueprintLibrary::GetAllDifficultyLevels()
{
    TArray<EDifficultyLevel> Levels;
    Levels.Add(EDifficultyLevel::Easy);
    Levels.Add(EDifficultyLevel::Normal);
    Levels.Add(EDifficultyLevel::Hard);
    Levels.Add(EDifficultyLevel::Expert);
    Levels.Add(EDifficultyLevel::Custom);
    return Levels;
}

float UMingDifficultyBlueprintLibrary::GetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->GetParameterValue(Parameter);
    }
    return 1.0f;
}

void UMingDifficultyBlueprintLibrary::SetDifficultyParameter(UObject* WorldContextObject, EDifficultyParameter Parameter, float Value)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->SetParameterValue(Parameter, Value);
    }
}

void UMingDifficultyBlueprintLibrary::SetDynamicDifficultyEnabled(UObject* WorldContextObject, bool bEnabled)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->SetDynamicAdjustmentEnabled(bEnabled);
    }
}

bool UMingDifficultyBlueprintLibrary::IsDynamicDifficultyEnabled(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->IsDynamicAdjustmentEnabled();
    }
    return false;
}

void UMingDifficultyBlueprintLibrary::LockDifficulty(UObject* WorldContextObject, bool bLocked)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->LockDifficulty(bLocked);
    }
}

bool UMingDifficultyBlueprintLibrary::IsDifficultyLocked(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->IsDifficultyLocked();
    }
    return false;
}

void UMingDifficultyBlueprintLibrary::ForceDifficultyEvaluation(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->ForceDifficultyEvaluation();
    }
}

void UMingDifficultyBlueprintLibrary::IncreaseDifficulty(UObject* WorldContextObject)
{
    EDifficultyLevel CurrentLevel = GetCurrentDifficultyLevel(WorldContextObject);
    EDifficultyLevel NewLevel = CurrentLevel;

    switch (CurrentLevel)
    {
    case EDifficultyLevel::Easy:
        NewLevel = EDifficultyLevel::Normal;
        break;
    case EDifficultyLevel::Normal:
        NewLevel = EDifficultyLevel::Hard;
        break;
    case EDifficultyLevel::Hard:
        NewLevel = EDifficultyLevel::Expert;
        break;
    default:
        // Expert 或 Custom 保持在当前等级
        break;
    }

    SetDifficultyLevel(WorldContextObject, NewLevel);
}

void UMingDifficultyBlueprintLibrary::DecreaseDifficulty(UObject* WorldContextObject)
{
    EDifficultyLevel CurrentLevel = GetCurrentDifficultyLevel(WorldContextObject);
    EDifficultyLevel NewLevel = CurrentLevel;

    switch (CurrentLevel)
    {
    case EDifficultyLevel::Expert:
        NewLevel = EDifficultyLevel::Hard;
        break;
    case EDifficultyLevel::Hard:
        NewLevel = EDifficultyLevel::Normal;
        break;
    case EDifficultyLevel::Normal:
        NewLevel = EDifficultyLevel::Easy;
        break;
    default:
        // Easy 或 Custom 保持在当前等级
        break;
    }

    SetDifficultyLevel(WorldContextObject, NewLevel);
}

void UMingDifficultyBlueprintLibrary::ResetDifficulty(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->ResetToDefault();
    }
}

float UMingDifficultyBlueprintLibrary::GetAIDifficultyMultiplier(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->GetAIDifficultyMultiplier();
    }
    return 1.0f;
}

float UMingDifficultyBlueprintLibrary::GetResourceMultiplier(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->GetResourceMultiplier();
    }
    return 1.0f;
}

float UMingDifficultyBlueprintLibrary::GetMissionDifficultyMultiplier(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        return Manager->GetMissionDifficultyMultiplier();
    }
    return 1.0f;
}

void UMingDifficultyBlueprintLibrary::TrackUnitLoss(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitValue)
{
    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        Tracker->RecordUnitLost(UnitId, UnitType, UnitValue);
    }
}

void UMingDifficultyBlueprintLibrary::TrackUnitCreation(UObject* WorldContextObject, int32 UnitId, int32 UnitType, float UnitCost)
{
    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        Tracker->RecordUnitCreated(UnitId, UnitType, UnitCost);
    }
}

void UMingDifficultyBlueprintLibrary::TrackResourceCollection(UObject* WorldContextObject, float ResourceType, float Amount, float ExpectedRate)
{
    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        Tracker->RecordResourceCollected(ResourceType, Amount, ExpectedRate);
    }
}

void UMingDifficultyBlueprintLibrary::TrackMissionComplete(UObject* WorldContextObject, const FString& MissionId, bool bSuccess, float CompletionTime)
{
    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        Tracker->RecordMissionCompleted(MissionId, bSuccess, CompletionTime);
    }
}

void UMingDifficultyBlueprintLibrary::TrackCombatResult(UObject* WorldContextObject, int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken)
{
    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        Tracker->RecordCombatResult(EnemiesKilled, AlliesLost, DamageDealt, DamageTaken);
    }
}

float UMingDifficultyBlueprintLibrary::GetPlayerSkillIndex(UObject* WorldContextObject)
{
    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        return Tracker->GetSkillIndex();
    }
    return 50.0f;
}

void UMingDifficultyBlueprintLibrary::SendDifficultyNotification(UObject* WorldContextObject, const FString& Message, float Duration)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->SendDifficultyNotification(Message, Duration);
    }
}

void UMingDifficultyBlueprintLibrary::SetDifficultyNotificationsEnabled(UObject* WorldContextObject, bool bEnabled)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->SetPlayerNotificationEnabled(bEnabled);
    }
}

void UMingDifficultyBlueprintLibrary::PrintDifficultyDebugInfo(UObject* WorldContextObject)
{
    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Manager->PrintDebugInfo();
    }

    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        Tracker->PrintDebugInfo();
    }
}

FString UMingDifficultyBlueprintLibrary::GetDifficultyDebugString(UObject* WorldContextObject)
{
    FString Result;

    if (UMingDifficultyManager* Manager = GetDifficultyManager(WorldContextObject))
    {
        Result = Manager->GetDebugString();
    }

    if (UMingPlayerPerformanceTracker* Tracker = GetPerformanceTracker(WorldContextObject))
    {
        if (!Result.IsEmpty())
        {
            Result += TEXT(" | ");
        }
        Result += Tracker->GetDebugString();
    }

    return Result;
}
