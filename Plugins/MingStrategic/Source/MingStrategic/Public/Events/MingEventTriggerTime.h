#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerTime.generated.h"

/**
 * 時間觸發器類型
 */
UENUM(BlueprintType)
enum class ETimeTriggerMode : uint8
{
    AbsoluteTime      UMETA(DisplayName = "Absolute Time"),
    RelativeTime      UMETA(DisplayName = "Relative Time"),
    Periodic          UMETA(DisplayName = "Periodic"),
    Daily             UMETA(DisplayName = "Daily"),
    Weekly            UMETA(DisplayName = "Weekly"),
    Monthly           UMETA(DisplayName = "Monthly")
};

/**
 * 遊戲內時間結構
 */
USTRUCT(BlueprintType)
struct FGameDateTime
{
    GENERATED_BODY()
    
    // 年 (1924開始)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Year;
    
    // 月 (1-12)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Month;
    
    // 日 (1-31)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Day;
    
    // 時 (0-23)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Hour;
    
    // 分 (0-59)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Minute;
    
    FGameDateTime()
        : Year(1924)
        , Month(1)
        , Day(1)
        , Hour(0)
        , Minute(0)
    {}
    
    // 轉換為總分鐘數 (用於比較)
    int64 ToTotalMinutes() const;
    
    // 從總分鐘數轉換
    static FGameDateTime FromTotalMinutes(int64 TotalMinutes);
};

/**
 * 時間觸發器
 * 基於遊戲時間觸發事件
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerTime : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingEventTriggerTime();

    // 設置絕對時間觸發 (1926年3月12日)
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetAbsoluteTime(int32 Year, int32 Month, int32 Day, int32 Hour = 0, int32 Minute = 0);

    // 設置相對時間觸發 (3個月後)
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetRelativeTime(int32 Months, int32 Days = 0, int32 Hours = 0);

    // 設置週期性觸發 (每30天)
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetPeriodic(float PeriodInDays);

    // 設置每日觸發 (每天12:00)
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetDaily(int32 Hour, int32 Minute = 0);

    // 更新當前遊戲時間
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void UpdateGameTime(const FGameDateTime& CurrentTime);

    // 獲取目標時間
    UFUNCTION(BlueprintPure, Category = "Time Trigger")
    FGameDateTime GetTargetTime() const { return TargetTime; }

    // 獲取剩餘時間 (分鐘)
    UFUNCTION(BlueprintPure, Category = "Time Trigger")
    int32 GetRemainingMinutes() const;

protected:
    // 觸發模式
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Trigger")
    ETimeTriggerMode TimeMode;

    // 目標時間
    UPROPERTY()
    FGameDateTime TargetTime;

    // 當前遊戲時間
    UPROPERTY()
    FGameDateTime CurrentGameTime;

    // 週期 (天)
    UPROPERTY()
    float PeriodDays;

    // 上次觸發時間
    UPROPERTY()
    FGameDateTime LastTriggerTime;

    // 每日觸發時間 (小時:分鐘)
    UPROPERTY()
    int32 DailyHour;

    UPROPERTY()
    int32 DailyMinute;

    // 是否已設置時間
    UPROPERTY()
    bool bTimeSet;

    // 初始化時記錄開始時間
    UPROPERTY()
    FGameDateTime StartTime;

    // 重寫基類方法
    virtual void Initialize() override;
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // 檢查各種模式的觸發條件
    bool CheckAbsoluteTime() const;
    bool CheckRelativeTime() const;
    bool CheckPeriodic() const;
    bool CheckDaily() const;

    // 計算目標時間 (相對模式)
    void CalculateTargetTime();
};
