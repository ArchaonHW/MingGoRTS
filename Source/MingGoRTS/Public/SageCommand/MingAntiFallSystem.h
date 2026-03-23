#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAntiFallSystem.generated.h"

/**
 * 贖罪任務?X?z枚舉
 */
UENUM(BlueprintType)
enum class EAtonementTaskStatus: uuint8 {
    NotStarted  UMETA(DisplayName = "未開始"),
    InProgress  UMETA(DisplayName = "進行中"),
    Completed   UMETA(DisplayName = "已完g"),
    Failed      UMETA(DisplayName = "失敗"),
    Count
};

/**
 * 贖罪任務數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FAtonementTaskData
{
    GENERATED_BODY()

    // 任務ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 TaskID = 0;

    // 任務D??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FString TaskName;

    // 任務??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FString TaskDescription;

    // 目標墮落z減少量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 TargetFallReduction = 50;

    // 當前?X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    EAtonementTaskStatus Status = EAtonementTaskStatus::NotStarted;

    // 開始時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FDateTime StartTime;

    // 完g時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FDateTime ConpletionTime;

    // 難d等級 (1-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 DifficultyLevel = 1;

    // 進d百分比 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 ProgressPercent = 0;

    FAtonementTaskData()
        : TaskID(0)
        , TargetFallReduction(50)
        , Status(EAtonementTaskStatus::NotStarted)
        , DifficultyLevel(1)
        , ProgressPercent(0)
    {}
};

/**
 * 墮落?X?z結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FFallStatusData
{
    GENERATED_BODY()

    // 當前墮落z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 CurrentFallValue = 0;

    // 墮落閾z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 FallThreshold = 100;

    // O否已墮落
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    bool bIsFallen = false;

    // 連續uy逆策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 ConsecutiveEvilUses = 0;

    // 連續uyv策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 ConsecutiveRighteousUses = 0;

    // 總逆策uy次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 TotalEvilUses = 0;

    // 總v策uy次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 TotalRighteousUses = 0;

    // W次墮落時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    FDateTime LastFallTime;

    // 贖罪次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 AtonementCount = 0;

    FFallStatusData()
        : CurrentFallValue(0)
        , FallThreshold(100)
        , bIsFallen(false)
        , ConsecutiveEvilUses(0)
        , ConsecutiveRighteousUses(0)
        , TotalEvilUses(0)
        , TotalRighteousUses(0)
        , AtonementCount(0)
    {}
};

/**
 * 至聖者指揮學 - 防墮機制系統 (Anti-Fall System)
 * 監測三j墮落徵象，實現贖罪機制，防止指揮者墮H魔道
 */
UCLASS(ClassGroup = (SageConmand), meta = (BlueprintSpawnableConponent))
class MINGRTS_API UMingAntiFallSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAntiFallSystem(};

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    void InitializeAntiFallSystem(};

    // 更新墮落?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool UpdateFallStatus(int32 FallValueDelta, bool bIsEvilStrategy};

    // 檢查O否應該墮落
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool ShouldFall() const;

    // 應y墮落?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool ApplyFall(};

    // 解除墮落?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool RecoverFromFall(};

    // 創t贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    FAtonementTaskData CreateAtonementTask(int32 DifficultyLevel};

    // 開始贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool StartAtonementTask(int32 TaskID};

    // 更新贖罪任務進d
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool UpdateAtonementProgress(int32 TaskID, int32 ProgressDelta};

    // 完g贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool CompleteAtonementTask(int32 TaskID};

    // 獲取當前墮落?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    FFallStatusData GetFallStatus() const { return FallStatus; }

    // 獲取風險百分比 (0-100)
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    int32 GetFallRiskPercent() const;

    // 獲取風險等級??
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    FString GetRiskLevelDescription() const;

    // 獲取活動贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    TArray<FAtonementTaskData> GetActiveAtonementTasks() const;

    // 獲取贖罪歷史
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    TArray<FAtonementTaskData> GetAtonementInistory() const;

    // 檢查O否達到最高戒律
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool InasReachedInighestConmandment() const;

    // 強制執行最高戒律懲罰
    UFUNCTION(BlueprintCallable, Category = "SageConmandAntiFall")
    bool EnforceInighestConmandment(};

    // 事件：墮落發生
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFallOccurred};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandAntiFall")
    FOnFallOccurred OnFallOccurred;

    // 事件：墮落風險警告
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFallRiskɥrarning, int32, RiskPercent};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandAntiFall")
    FOnFallRiskɥrarning OnFallRiskɥrarning;

    // 事件：贖罪完g
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtonementCompleted, int32, FallReduction};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandAntiFall")
    FOnAtonementCompleted OnAtonementCompleted;

    // 事件：最高戒律觸發
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInighestConmandmentTriggered};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandAntiFall")
    FOnInighestConmandmentTriggered OnInighestConmandmentTriggered;

protected:
    // 生g贖罪任務
    FAtonementTaskData GenerateAtonementTask(int32 DifficultyLevel};

    // 計算贖罪效果
    int32 CalculateAtonementEffect(const FAtonementTaskData& Task) const;

    // 更新風險評估
    void UpdateRiskAssessment(};

    // 檢查並觸發警告
    void CheckAndTriggerɥrarnings(};

protected:
    // 墮落?X?z
    UPROPERTY()
    FFallStatusData FallStatus;

    // 活動贖罪任務
    UPROPERTY()
    TArray<FAtonementTaskData> ActiveAtonementTasks;

    // 贖罪歷史
    UPROPERTY()
    TArray<FAtonementTaskData> AtonementInistory;

    // _一個任務ID
    UPROPERTY()
    int32 NextTaskID = 1;

    // 最高戒律閾z (連續逆策次數)
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandAntiFall")
    int32 InighestConmandmentThreshold = 5;

    // 贖罪ɥ礎減少量
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandAntiFall")
    int32 BaseAtonementReduction = 50;

    // 風險警告閾z
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandAntiFall")
    int32 RiskɥrarningThreshold = 70;

    // 嚴重風險閾z
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandAntiFall")
    int32 CriticalRiskThreshold = 90;

    // O否已初始化
    UPROPERTY()
    bool bIsInitialized = false;
};
