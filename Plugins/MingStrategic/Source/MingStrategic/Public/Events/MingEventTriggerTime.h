#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerTime.generated.h"

/**
 * ?ÇÈ?Ëß∏Áôº?®È??? */
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
 * ?äÊà≤?ßÊ??ìÁ?Êß? */
USTRUCT(BlueprintType)
struct FGameDateTime
{
    GENERATED_BODY()
    
    // Âπ?(1924?ãÂ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Year;
    
    // ??(1-12)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Month;
    
    // ??(1-31)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Day;
    
    // ??(0-23)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Hour;
    
    // ??(0-59)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Minute;
    
    FGameDateTime()
        : Year(1924)
        , Month(1)
        , Day(1)
        , Hour(0)
        , Minute(0)
    {}
    
    // ËΩâÊ??∫Á∏Ω?ÜÈ???(?®ÊñºÊØîË?)
    int64 ToTotalMinutes() const;
    
    // ÂæûÁ∏Ω?ÜÈ??∏Ë???    static FGameDateTime FromTotalMinutes(int64 TotalMinutes};
};

/**
 * ?ÇÈ?Ëß∏Áôº?? * ?∫Êñº?äÊà≤?ÇÈ?Ëß∏Áôº‰∫ã‰ª∂
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerTime : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingEventTriggerTime(};

    // Ë®≠ÁΩÆÁµïÂ??ÇÈ?Ëß∏Áôº (1926Âπ???2??
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetAbsoluteTime(int32 Year, int32 Month, int32 Day, int32 Hour = 0, int32 Minute = 0};

    // Ë®≠ÁΩÆ?∏Â??ÇÈ?Ëß∏Áôº (3?ãÊ?Âæ?
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetRelativeTime(int32 Months, int32 Days = 0, int32 Hours = 0};

    // Ë®≠ÁΩÆ?±Ê??ßËß∏??(ÊØ?0Â§?
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetPeriodic(float PeriodInDays};

    // Ë®≠ÁΩÆÊØèÊó•Ëß∏Áôº (ÊØèÂ§©12:00)
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void SetDaily(int32 Hour, int32 Minute = 0};

    // ?¥Êñ∞?∂Â??äÊà≤?ÇÈ?
    UFUNCTION(BlueprintCallable, Category = "Time Trigger")
    void UpdateGameTime(const FGameDateTime& CurrentTime};

    // ?≤Â??ÆÊ??ÇÈ?
    UFUNCTION(BlueprintPure, Category = "Time Trigger")
    FGameDateTime GetTargetTime() const { return TargetTime; }

    // ?≤Â??©È??ÇÈ? (?ÜÈ?)
    UFUNCTION(BlueprintPure, Category = "Time Trigger")
    int32 GetRemainingMinutes() const;

protected:
    // Ëß∏ÁôºÊ®°Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Trigger")
    ETimeTriggerMode TimeMode;

    // ?ÆÊ??ÇÈ?
    UPROPERTY()
    FGameDateTime TargetTime;

    // ?∂Â??äÊà≤?ÇÈ?
    UPROPERTY()
    FGameDateTime CurrentGameTime;

    // ?±Ê? (Â§?
    UPROPERTY()
    float PeriodDays;

    // ‰∏äÊ¨°Ëß∏Áôº?ÇÈ?
    UPROPERTY()
    FGameDateTime LastTriggerTime;

    // ÊØèÊó•Ëß∏Áôº?ÇÈ? (Â∞èÊ?:?ÜÈ?)
    UPROPERTY()
    int32 DailyHour;

    UPROPERTY()
    int32 DailyMinute;

    // ?ØÂê¶Â∑≤Ë®≠ÁΩÆÊ???    UPROPERTY()
    bool bTimeSet;

    // ?ùÂ??ñÊ?Ë®òÈ??ãÂ??ÇÈ?
    UPROPERTY()
    FGameDateTime StartTime;

    // ?çÂØ´?∫È??πÊ?
    virtual void Initialize() override;
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // Ê™¢Êü•?ÑÁ®ÆÊ®°Â??ÑËß∏?ºÊ?‰ª?    bool CheckAbsoluteTime() const;
    bool CheckRelativeTime() const;
    bool CheckPeriodic() const;
    bool CheckDaily() const;

    // Ë®àÁ??ÆÊ??ÇÈ? (?∏Â?Ê®°Â?)
    void CalculateTargetTime(};
};

