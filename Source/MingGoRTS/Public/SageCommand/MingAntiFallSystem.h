// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAntiFallSystem.generated.h"

/**
 * 贖罪任務狀態枚舉
 */
UENUM(BlueprintType)
enum class EAtonementTaskStatus: uint8 {
    NotStarted  UMETA(DisplayName = "未開始"),
    InProgress  UMETA(DisplayName = "進行中"),
    Completed   UMETA(DisplayName = "已完成"),
    Failed      UMETA(DisplayName = "失敗"),
    Count
};

/**
 * 贖罪任務數據結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FAtonementTaskData
{
    GENERATED_BODY()

    // 任務ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 TaskID = 0;

    // 任務名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FString TaskName;

    // 任務描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FString TaskDescription;

    // 目標墮落值減少量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 TargetFallReduction = 50;

    // 當前狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    EAtonementTaskStatus Status = EAtonementTaskStatus::NotStarted;

    // 開始時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FDateTime StartTime;

    // 完成時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    FDateTime CompletionTime;

    // 難度等級 (1-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atonement")
    int32 DifficultyLevel = 1;

    // 進度百分比 (0-100)
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
 * 墮落狀態結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FFallStatusData
{
    GENERATED_BODY()

    // 當前墮落值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 CurrentFallValue = 0;

    // 墮落閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 FallThreshold = 100;

    // 是否已墮落
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    bool bIsFallen = false;

    // 連續使用逆策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 ConsecutiveEvilUses = 0;

    // 連續使用正策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 ConsecutiveRighteousUses = 0;

    // 總逆策使用次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 TotalEvilUses = 0;

    // 總正策使用次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallStatus")
    int32 TotalRighteousUses = 0;

    // 上次墮落時間
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
 * 監測三大墮落徵象，實現贖罪機制，防止指揮者墮入魔道
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingAntiFallSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAntiFallSystem();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    void InitializeAntiFallSystem();

    // 更新墮落狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool UpdateFallStatus(int32 FallValueDelta, bool bIsEvilStrategy);

    // 檢查是否應該墮落
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool ShouldFall() const;

    // 應用墮落狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool ApplyFall();

    // 解除墮落狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool RecoverFromFall();

    // 創建贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    FAtonementTaskData CreateAtonementTask(int32 DifficultyLevel);

    // 開始贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool StartAtonementTask(int32 TaskID);

    // 更新贖罪任務進度
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool UpdateAtonementProgress(int32 TaskID, int32 ProgressDelta);

    // 完成贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool CompleteAtonementTask(int32 TaskID);

    // 獲取當前墮落狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    FFallStatusData GetFallStatus() const { return FallStatus; }

    // 獲取風險百分比 (0-100)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    int32 GetFallRiskPercent() const;

    // 獲取風險等級描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    FString GetRiskLevelDescription() const;

    // 獲取活動贖罪任務
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    TArray<FAtonementTaskData> GetActiveAtonementTasks() const;

    // 獲取贖罪歷史
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    TArray<FAtonementTaskData> GetAtonementHistory() const;

    // 檢查是否達到最高戒律
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool HasReachedHighestCommandment() const;

    // 強制執行最高戒律懲罰
    UFUNCTION(BlueprintCallable, Category = "SageCommand|AntiFall")
    bool EnforceHighestCommandment();

    // 事件：墮落發生
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFallOccurred);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|AntiFall")
    FOnFallOccurred OnFallOccurred;

    // 事件：墮落風險警告
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFallRiskWarning, int32, RiskPercent);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|AntiFall")
    FOnFallRiskWarning OnFallRiskWarning;

    // 事件：贖罪完成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtonementCompleted, int32, FallReduction);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|AntiFall")
    FOnAtonementCompleted OnAtonementCompleted;

    // 事件：最高戒律觸發
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHighestCommandmentTriggered);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|AntiFall")
    FOnHighestCommandmentTriggered OnHighestCommandmentTriggered;

protected:
    // 生成贖罪任務
    FAtonementTaskData GenerateAtonementTask(int32 DifficultyLevel);

    // 計算贖罪效果
    int32 CalculateAtonementEffect(const FAtonementTaskData& Task) const;

    // 更新風險評估
    void UpdateRiskAssessment();

    // 檢查並觸發警告
    void CheckAndTriggerWarnings();

protected:
    // 墮落狀態
    UPROPERTY()
    FFallStatusData FallStatus;

    // 活動贖罪任務
    UPROPERTY()
    TArray<FAtonementTaskData> ActiveAtonementTasks;

    // 贖罪歷史
    UPROPERTY()
    TArray<FAtonementTaskData> AtonementHistory;

    // 下一個任務ID
    UPROPERTY()
    int32 NextTaskID = 1;

    // 最高戒律閾值 (連續逆策次數)
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|AntiFall")
    int32 HighestCommandmentThreshold = 5;

    // 贖罪基礎減少量
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|AntiFall")
    int32 BaseAtonementReduction = 50;

    // 風險警告閾值
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|AntiFall")
    int32 RiskWarningThreshold = 70;

    // 嚴重風險閾值
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|AntiFall")
    int32 CriticalRiskThreshold = 90;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;
};
