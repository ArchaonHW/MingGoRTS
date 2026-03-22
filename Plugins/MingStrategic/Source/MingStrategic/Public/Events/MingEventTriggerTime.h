#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEventTriggerTime.generated.h"

// 觸發時間類型
UENUM(BlueprintType)
enum class ETriggerTimeType : uint8
{
    Absolute,       // 絕對時間
    Relative,       // 相對時間
    Periodic,       // 週期性
    Daily,          // 每日
    Weekly,         // 每週
    Monthly,        // 每月
    Conditional     // 條件觸發
};

// 時間觸發條件
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FTimeTriggerCondition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ETriggerTimeType TriggerType;

    UPROPERTY(BlueprintReadOnly)
    FDateTime TargetTime;

    UPROPERTY(BlueprintReadOnly)
    int32 IntervalHours;

    UPROPERTY(BlueprintReadOnly)
    int32 IntervalDays;

    UPROPERTY(BlueprintReadOnly)
    int32 HourOfDay;

    UPROPERTY(BlueprintReadOnly)
    int32 DayOfWeek;

    UPROPERTY(BlueprintReadOnly)
    FString ConditionExpression;
};

/**
 * 事件觸發時間管理器
 * 負責管理各種時間觸發條件
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingEventTriggerTime : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingEventTriggerTime();

    // 初始化觸發時間系統
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void InitializeTriggerSystem();

    // 添加時間觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void AddTimeTrigger(const FString& EventID, const FTimeTriggerCondition& Condition);

    // 移除時間觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void RemoveTimeTrigger(const FString& EventID);

    // 更新觸發檢查
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void UpdateTriggerChecks(float DeltaTime);

    // 檢查特定觸發是否應該觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    bool ShouldTrigger(const FString& EventID) const;

    // 獲取下次觸發時間
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    FDateTime GetNextTriggerTime(const FString& EventID) const;

    // 設置絕對時間觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void SetAbsoluteTimeTrigger(const FString& EventID, const FDateTime& TargetTime);

    // 設置相對時間觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void SetRelativeTimeTrigger(const FString& EventID, int32 HoursFromNow);

    // 設置週期性觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void SetPeriodicTrigger(const FString& EventID, int32 IntervalHours);

    // 設置每日觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void SetDailyTrigger(const FString& EventID, int32 HourOfDay);

    // 暫停觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void PauseTrigger(const FString& EventID);

    // 恢復觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void ResumeTrigger(const FString& EventID);

    // 獲取所有活動觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    TArray<FString> GetActiveTriggers() const;

    // 清除所有觸發
    UFUNCTION(BlueprintCallable, Category = "Event|Time")
    void ClearAllTriggers();

protected:
    // 時間觸發映射
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FTimeTriggerCondition> TimeTriggers;

    // 上次檢查時間
    UPROPERTY(BlueprintReadOnly)
    FDateTime LastCheckTime;

    // 更新間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time|Settings")
    float UpdateInterval;

    // 時間縮放因子
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time|Settings")
    float TimeScale;

    // 檢查各種模式的觸發條件
    bool CheckAbsoluteTime(const FTimeTriggerCondition& Condition) const;
    bool CheckRelativeTime(const FTimeTriggerCondition& Condition) const;
    bool CheckPeriodic(const FTimeTriggerCondition& Condition) const;
    bool CheckDaily(const FTimeTriggerCondition& Condition) const;

    // 計算目標時間（對於各種模式）
    void CalculateTargetTime(FTimeTriggerCondition& Condition);

    // 觸發事件回調
    UFUNCTION(BlueprintImplementableEvent, Category = "Event|Time")
    void OnTimeTriggered(const FString& EventID);

private:
    // 累積時間
    float AccumulatedTime;

    // 已觸發的事件記錄
    TSet<FString> TriggeredEvents;

    // 重置每日觸發
    void ResetDailyTriggers();
};
