#include "Events/MingEventTriggerTime.h"

int64 FGameDateTime::ToTotalMinutes() const
{
    // 簡化計算：假設每月30天，每年360天
    int64 TotalMinutes = Year * 360 * 24 * 60;
    TotalMinutes += (Month - 1) * 30 * 24 * 60;
    TotalMinutes += (Day - 1) * 24 * 60;
    TotalMinutes += Hour * 60;
    TotalMinutes += Minute;
    return TotalMinutes;
}

FGameDateTime FGameDateTime::FromTotalMinutes(int64 TotalMinutes)
{
    FGameDateTime Result;
    
    int64 MinutesPerYear = 360 * 24 * 60;
    int64 MinutesPerMonth = 30 * 24 * 60;
    int64 MinutesPerDay = 24 * 60;
    
    Result.Year = static_cast<int32>(TotalMinutes / MinutesPerYear);
    TotalMinutes %= MinutesPerYear;
    
    Result.Month = static_cast<int32>(TotalMinutes / MinutesPerMonth) + 1;
    TotalMinutes %= MinutesPerMonth;
    
    Result.Day = static_cast<int32>(TotalMinutes / MinutesPerDay) + 1;
    TotalMinutes %= MinutesPerDay;
    
    Result.Hour = static_cast<int32>(TotalMinutes / 60);
    Result.Minute = static_cast<int32>(TotalMinutes % 60);
    
    return Result;
}

UMingEventTriggerTime::UMingEventTriggerTime()
    : TimeMode(ETimeTriggerMode::AbsoluteTime)
    , PeriodDays(0.0f)
    , DailyHour(0)
    , DailyMinute(0)
    , bTimeSet(false)
{
    TriggerType = EEventTriggerType::Time;
}

void UMingEventTriggerTime::Initialize()
{
    Super::Initialize();
    
    // 記錄開始時間
    StartTime = CurrentGameTime;
    
    // 如果是相對時間模式，計算目標時間
    if (TimeMode == ETimeTriggerMode::RelativeTime && bTimeSet)
    {
        CalculateTargetTime();
    }
    
    UE_LOG(LogTemp, Log, TEXT("TimeTrigger %s initialized (Mode: %s)"),
        *TriggerId, *UEnum::GetValueAsString(TimeMode));
}

void UMingEventTriggerTime::SetAbsoluteTime(int32 Year, int32 Month, int32 Day, int32 Hour, int32 Minute)
{
    TimeMode = ETimeTriggerMode::AbsoluteTime;
    
    TargetTime.Year = Year;
    TargetTime.Month = FMath::Clamp(Month, 1, 12);
    TargetTime.Day = FMath::Clamp(Day, 1, 30);
    TargetTime.Hour = FMath::Clamp(Hour, 0, 23);
    TargetTime.Minute = FMath::Clamp(Minute, 0, 59);
    
    bTimeSet = true;
    
    UE_LOG(LogTemp, Verbose, TEXT("TimeTrigger %s set absolute time: %04d-%02d-%02d %02d:%02d"),
        *TriggerId, Year, Month, Day, Hour, Minute);
}

void UMingEventTriggerTime::SetRelativeTime(int32 Months, int32 Days, int32 Hours)
{
    TimeMode = ETimeTriggerMode::RelativeTime;
    
    // 計算總分鐘數偏移
    int64 TotalMinutes = Months * 30 * 24 * 60;
    TotalMinutes += Days * 24 * 60;
    TotalMinutes += Hours * 60;
    
    // 保存為週期天數 (用於計算)
    PeriodDays = static_cast<float>(TotalMinutes) / (24.0f * 60.0f);
    
    bTimeSet = true;
    
    UE_LOG(LogTemp, Verbose, TEXT("TimeTrigger %s set relative time: %d months, %d days, %d hours"),
        *TriggerId, Months, Days, Hours);
}

void UMingEventTriggerTime::SetPeriodic(float PeriodInDays)
{
    TimeMode = ETimeTriggerMode::Periodic;
    PeriodDays = PeriodInDays;
    bTimeSet = true;
    
    UE_LOG(LogTemp, Verbose, TEXT("TimeTrigger %s set periodic: %.1f days"),
        *TriggerId, PeriodInDays);
}

void UMingEventTriggerTime::SetDaily(int32 Hour, int32 Minute)
{
    TimeMode = ETimeTriggerMode::Daily;
    DailyHour = FMath::Clamp(Hour, 0, 23);
    DailyMinute = FMath::Clamp(Minute, 0, 59);
    bTimeSet = true;
    
    UE_LOG(LogTemp, Verbose, TEXT("TimeTrigger %s set daily: %02d:%02d"),
        *TriggerId, DailyHour, DailyMinute);
}

void UMingEventTriggerTime::UpdateGameTime(const FGameDateTime& CurrentTime)
{
    CurrentGameTime = CurrentTime;
}

int32 UMingEventTriggerTime::GetRemainingMinutes() const
{
    if (!bTimeSet)
    {
        return -1;
    }
    
    int64 CurrentMinutes = CurrentGameTime.ToTotalMinutes();
    int64 TargetMinutes = TargetTime.ToTotalMinutes();
    
    return static_cast<int32>(TargetMinutes - CurrentMinutes);
}

bool UMingEventTriggerTime::PerformTrigger()
{
    // 時間觸發器的觸發邏輯在 CheckTriggerCondition 中處理
    return true;
}

bool UMingEventTriggerTime::CheckTriggerCondition() const
{
    if (!bTimeSet)
    {
        return false;
    }
    
    switch (TimeMode)
    {
    case ETimeTriggerMode::AbsoluteTime:
        return CheckAbsoluteTime();
        
    case ETimeTriggerMode::RelativeTime:
        return CheckRelativeTime();
        
    case ETimeTriggerMode::Periodic:
        return CheckPeriodic();
        
    case ETimeTriggerMode::Daily:
        return CheckDaily();
        
    default:
        return false;
    }
}

bool UMingEventTriggerTime::CheckAbsoluteTime() const
{
    // 檢查當前時間是否達到或超過目標時間
    int64 CurrentMinutes = CurrentGameTime.ToTotalMinutes();
    int64 TargetMinutes = TargetTime.ToTotalMinutes();
    
    return CurrentMinutes >= TargetMinutes;
}

bool UMingEventTriggerTime::CheckRelativeTime() const
{
    // 檢查是否超過相對時間偏移
    int64 StartMinutes = StartTime.ToTotalMinutes();
    int64 CurrentMinutes = CurrentGameTime.ToTotalMinutes();
    int64 TargetMinutes = StartMinutes + static_cast<int64>(PeriodDays * 24 * 60);
    
    return CurrentMinutes >= TargetMinutes;
}

bool UMingEventTriggerTime::CheckPeriodic() const
{
    // 檢查是否達到下一個週期
    int64 LastMinutes = LastTriggerTime.ToTotalMinutes();
    int64 CurrentMinutes = CurrentGameTime.ToTotalMinutes();
    int64 PeriodMinutes = static_cast<int64>(PeriodDays * 24 * 60);
    
    if (PeriodMinutes <= 0)
    {
        return false;
    }
    
    return (CurrentMinutes - LastMinutes) >= PeriodMinutes;
}

bool UMingEventTriggerTime::CheckDaily() const
{
    // 檢查是否到達每日指定時間
    if (CurrentGameTime.Hour == DailyHour && CurrentGameTime.Minute == DailyMinute)
    {
        // 檢查今天是否已經觸發過
        if (LastTriggerTime.Year != CurrentGameTime.Year ||
            LastTriggerTime.Month != CurrentGameTime.Month ||
            LastTriggerTime.Day != CurrentGameTime.Day)
        {
            return true;
        }
    }
    
    return false;
}

void UMingEventTriggerTime::CalculateTargetTime()
{
    // 計算相對時間的目標時間
    int64 StartMinutes = StartTime.ToTotalMinutes();
    int64 TargetMinutes = StartMinutes + static_cast<int64>(PeriodDays * 24 * 60);
    TargetTime = FGameDateTime::FromTotalMinutes(TargetMinutes);
}
