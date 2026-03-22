#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EOSImplementationManager.generated.h"

UENUM(BlueprintType)
enum class EOSImplementationPhase : uint8
{
    Planning     UMETA(DisplayName = "Planning"),
    Kernel      UMETA(DisplayName = "Kernel Development"),
    Services    UMETA(DisplayName = "System Services"),
    Advanced    UMETA(DisplayName = "Advanced Features"),
    Integration UMETA(DisplayName = "Integration & Testing"),
    Complete    UMETA(DisplayName = "Complete")
};

UENUM(BlueprintType)
enum class EOSImplementationStatus : uint8
{
    NotStarted     UMETA(DisplayName = "Not Started"),
    InProgress     UMETA(DisplayName = "In Progress"),
    Testing        UMETA(DisplayName = "Testing"),
    Completed      UMETA(DisplayName = "Completed"),
    Failed         UMETA(DisplayName = "Failed"),
    Blocked        UMETA(DisplayName = "Blocked")
};

USTRUCT(BlueprintType)
struct FEOSPhaseTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FString TaskName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    int32 EstimatedDays;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    EOSImplementationStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FString AssignedDeveloper;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    float ProgressPercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FDateTime StartDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
    FDateTime EndDate;
};

USTRUCT(BlueprintType)
struct FEOSPhaseInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    EOSImplementationPhase Phase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    FString PhaseName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    int32 DurationWeeks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    TArray<FEOSPhaseTask> Tasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    float OverallProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phase")
    EOSImplementationStatus Status;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseProgressUpdated, EOSImplementationPhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseStatusChanged, EOSImplementationPhase, Phase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTaskUpdated, EOSImplementationPhase, Phase, const FString&, TaskName);

/**
 * 內建作業系統實施管理器
 * 負責管理整個12週的實施計劃，分為4個主要階段
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UEOSImplementationManager : public UObject
{
    GENERATED_BODY()

public:
    UEOSImplementationManager();

    // 初始化實施管理器
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    void InitializeImplementationManager();

    // 獲取當前階段
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    EOSImplementationPhase GetCurrentPhase() const { return CurrentPhase; }

    // 設置當前階段
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    void SetCurrentPhase(EOSImplementationPhase NewPhase);

    // 獲取階段信息
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    FEOSPhaseInfo GetPhaseInfo(EOSImplementationPhase Phase) const;

    // 獲取所有階段信息
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    TArray<FEOSPhaseInfo> GetAllPhasesInfo() const;

    // 更新任務狀態
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    void UpdateTaskStatus(EOSImplementationPhase Phase, const FString& TaskName, EOSImplementationStatus Status);

    // 更新任務進度
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    void UpdateTaskProgress(EOSImplementationPhase Phase, const FString& TaskName, float Progress);

    // 獲取整體進度
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    float GetOverallProgress() const;

    // 獲取階段進度
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    float GetPhaseProgress(EOSImplementationPhase Phase) const;

    // 檢查階段是否可以開始
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    bool CanStartPhase(EOSImplementationPhase Phase) const;

    // 開始階段
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    bool StartPhase(EOSImplementationPhase Phase);

    // 完成階段
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    void CompletePhase(EOSImplementationPhase Phase);

    // 獲取風險評估
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    TArray<FString> GetRiskAssessment() const;

    // 獲取建議
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    TArray<FString> GetRecommendations() const;

    // 生成實施報告
    UFUNCTION(BlueprintCallable, Category = "EOS Implementation")
    FString GenerateImplementationReport() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "EOS Implementation")
    FOnPhaseProgressUpdated OnPhaseProgressUpdated;

    UPROPERTY(BlueprintAssignable, Category = "EOS Implementation")
    FOnPhaseStatusChanged OnPhaseStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "EOS Implementation")
    FOnTaskUpdated OnTaskUpdated;

protected:
    // 當前階段
    UPROPERTY(BlueprintReadOnly, Category = "EOS Implementation")
    EOSImplementationPhase CurrentPhase;

    // 階段信息映射
    UPROPERTY(BlueprintReadOnly, Category = "EOS Implementation")
    TMap<EOSImplementationPhase, FEOSPhaseInfo> PhaseInfos;

    // 初始化階段信息
    void InitializePhaseInfos();

    // 計算階段進度
    float CalculatePhaseProgress(const FEOSPhaseInfo& PhaseInfo) const;

    // 驗證任務依賴
    bool ValidateTaskDependencies(const FEOSPhaseTask& Task) const;

    // 更新整體進度
    void UpdateOverallProgress();

private:
    // 整體進度
    float OverallProgress;

    // 實施開始時間
    FDateTime ImplementationStartTime;
};
