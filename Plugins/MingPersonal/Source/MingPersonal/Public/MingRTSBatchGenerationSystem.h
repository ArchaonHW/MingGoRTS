#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"
#include "MingAIGeneratedContentSystem.h"
#include "MingAIContentRenderer.h"
#include "MingAIContentManager.h"
#include "MingRTSBatchGenerationSystem.generated.h"

UENUM(BlueprintType)
enum class EBatchType : uint8
{
    SceneBatch         UMETA(DisplayName = "Scene Batch"),
    CharacterBatch     UMETA(DisplayName = "Character Batch"),
    BuildingBatch      UMETA(DisplayName = "Building Batch"),
    UIBatch            UMETA(DisplayName = "UI Batch"),
    AudioBatch         UMETA(DisplayName = "Audio Batch"),
    ProjectBatch       UMETA(DisplayName = "Project Batch"),
    Custom             UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EVariationType : uint8
{
    TimeOfDay          UMETA(DisplayName = "Time of Day"),
    Weather            UMETA(DisplayName = "Weather"),
    Camera             UMETA(DisplayName = "Camera"),
    Lighting           UMETA(DisplayName = "Lighting"),
    Pose               UMETA(DisplayName = "Pose"),
    Equipment          UMETA(DisplayName = "Equipment"),
    Expression         UMETA(DisplayName = "Expression"),
    Action             UMETA(DisplayName = "Action"),
    Style              UMETA(DisplayName = "Style"),
    Mood               UMETA(DisplayName = "Mood"),
    Custom             UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EConditionType : uint8
{
    GameContext         UMETA(DisplayName = "Game Context"),
    TimeTrigger         UMETA(DisplayName = "Time Trigger"),
    EventTrigger        UMETA(DisplayName = "Event Trigger"),
    UserAction          UMETA(DisplayName = "User Action"),
    ResourceAvailable   UMETA(DisplayName = "Resource Available"),
    Custom              UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EOptimizationCriteria : uint8
{
    VisualQuality       UMETA(DisplayName = "Visual Quality"),
    Performance         UMETA(DisplayName = "Performance"),
    FileSize            UMETA(DisplayName = "File Size"),
    LoadingTime         UMETA(DisplayName = "Loading Time"),
    MemoryUsage         UMETA(DisplayName = "Memory Usage"),
    Custom              UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EBatchStatus : uint8
{
    Pending             UMETA(DisplayName = "Pending"),
    Running             UMETA(DisplayName = "Running"),
    Paused              UMETA(DisplayName = "Paused"),
    Completed           UMETA(DisplayName = "Completed"),
    Failed              UMETA(DisplayName = "Failed"),
    Cancelled           UMETA(DisplayName = "Cancelled"),
    Custom              UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FAIBatchConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    EBatchType BatchType;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    FString BatchName;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    int32 ItemCount;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    EQualityLevel Quality;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    EArtStyle Style;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    FString Theme;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    FString OutputFormat;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    FString Resolution;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    TArray<EVariationType> VariationRules;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    TMap<EAssetType, EAssetType> Dependencies;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    bool bEnableIterativeOptimization;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    int32 MaxIterations;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    TArray<EOptimizationCriteria> OptimizationCriteria;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Configuration")
    TMap<FString, FString> CustomParameters;

    FAIBatchConfiguration()
    {
        BatchType = EBatchType::SceneBatch;
        BatchName = TEXT("");
        ItemCount = 5;
        Quality = EQualityLevel::Standard;
        Style = EArtStyle::Realistic;
        Theme = TEXT("");
        OutputFormat = TEXT("PNG");
        Resolution = TEXT("1920x1080");
        bEnableIterativeOptimization = false;
        MaxIterations = 1;
    }
};

USTRUCT(BlueprintType)
struct FAIBatchCondition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Batch Condition")
    EConditionType ConditionType;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Condition")
    FString Context;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Condition")
    TArray<EAssetType> RequiredAssets;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Condition")
    TMap<FString, FString> Parameters;

    UPROPERTY(BlueprintReadWrite, Category = "Batch Condition")
    bool bPersistent;

    FAIBatchCondition()
    {
        ConditionType = EConditionType::GameContext;
        Context = TEXT("");
        bPersistent = false;
    }
};

USTRUCT(BlueprintType)
struct FAIBatchResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    FString BatchID;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    FString BatchName;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    EBatchStatus Status;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    int32 TotalItems;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    int32 SuccessCount;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    int32 ErrorCount;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    int32 SkippedCount;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    float StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    float EndTime;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    float TotalDuration;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    TArray<FAIGeneratedAsset> GeneratedAssets;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    TArray<FString> ErrorMessages;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Result")
    TArray<FString> Warnings;

    FAIBatchResult()
    {
        BatchID = TEXT("");
        BatchName = TEXT("");
        Status = EBatchStatus::Pending;
        TotalItems = 0;
        SuccessCount = 0;
        ErrorCount = 0;
        SkippedCount = 0;
        StartTime = 0.0f;
        EndTime = 0.0f;
        TotalDuration = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FAIBatchTemplate
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Batch Template")
    FString TemplateName;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Template")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Template")
    EBatchType BatchType;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Template")
    FAIBatchConfiguration DefaultConfiguration;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Template")
    TArray<FString> RecommendedVariations;

    UPROPERTY(BlueprintReadOnly, Category = "Batch Template")
    TArray<FString> UseCases;

    FAIBatchTemplate()
    {
        TemplateName = TEXT("");
        Description = TEXT("");
        BatchType = EBatchType::SceneBatch;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchProgress, const FString&, BatchID, int32, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchStarted, const FString&, BatchID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchCompleted, const FAIBatchResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchItemCompleted, const FString&, BatchID, const FAIGeneratedAsset&, Asset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatchError, const FString&, BatchID, const FString&, ErrorMessage);

/**
 * MingGoRTS 批次生成系统
 * 负责批量生成游戏画面和素材
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSBatchGenerationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSBatchGenerationSystem();

    // 初始化批次生成系统
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void InitializeBatchGenerationSystem(UWorld* World);

    // 启动批次生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FString StartBatchGeneration(const FAIBatchConfiguration& Configuration);

    // 暂停批次生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool PauseBatchGeneration(const FString& BatchID);

    // 恢复批次生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool ResumeBatchGeneration(const FString& BatchID);

    // 取消批次生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool CancelBatchGeneration(const FString& BatchID);

    // 获取批次状态
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    EBatchStatus GetBatchStatus(const FString& BatchID) const;

    // 获取批次进度
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    int32 GetBatchProgress(const FString& BatchID) const;

    // 获取批次结果
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FAIBatchResult GetBatchResult(const FString& BatchID) const;

    // 获取所有批次
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    TArray<FAIBatchResult> GetAllBatches() const;

    // 导出批次
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool ExportBatch(const FString& BatchID, const FString& OutputPath);

    // 导出批次为特定格式
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool ExportBatchAs(const FString& BatchID, EExportFormat Format);

    // 创建批次分享包
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FString CreateBatchSharePackage(const FString& BatchID);

    // 分享批次给团队
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool ShareBatchWithTeam(const FString& ShareID, const TArray<FString>& TeamMemberIDs);

    // 获取批次模板
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FAIBatchTemplate GetBatchTemplate(const FString& TemplateName) const;

    // 获取所有模板
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    TArray<FAIBatchTemplate> GetAllBatchTemplates() const;

    // 从模板创建配置
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FAIBatchConfiguration CreateConfigFromTemplate(const FAIBatchTemplate& Template) const;

    // 注册条件批次
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool RegisterConditionalBatch(const FAIBatchCondition& Condition, const FAIBatchConfiguration& Configuration);

    // 取消条件批次
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool UnregisterConditionalBatch(const FString& ConditionID);

    // 提供批次反馈
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void ProvideBatchFeedback(const FString& BatchID, const FString& Feedback);

    // 重新生成批次项目
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool RegenerateBatchItem(const FString& BatchID, const FString& AssetID);

    // 设置批次优先级
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool SetBatchPriority(const FString& BatchID, int32 Priority);

    // 获取批次统计
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    TMap<FString, float> GetBatchStatistics() const;

public:
    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Batch Generation System Events")
    FOnBatchProgress OnBatchProgress;

    UPROPERTY(BlueprintAssignable, Category = "Batch Generation System Events")
    FOnBatchStarted OnBatchStarted;

    UPROPERTY(BlueprintAssignable, Category = "Batch Generation System Events")
    FOnBatchCompleted OnBatchCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Batch Generation System Events")
    FOnBatchItemCompleted OnBatchItemCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Batch Generation System Events")
    FOnBatchError OnBatchError;

protected:
    // 处理批次生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void ProcessBatchGeneration(const FString& BatchID);

    // 生成批次项目
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FAIGeneratedAsset GenerateBatchItem(const FAIBatchConfiguration& Config, int32 ItemIndex);

    // 应用变化规则
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FString ApplyVariationRules(const FString& BasePrompt, const FAIBatchConfiguration& Config, int32 ItemIndex);

    // 验证批次配置
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool ValidateBatchConfiguration(const FAIBatchConfiguration& Config) const;

    // 处理条件触发
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void HandleConditionTrigger(const FAIBatchCondition& Condition);

    // 优化批次结果
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void OptimizeBatchResults(const FString& BatchID);

    // 处理批次错误
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void HandleBatchError(const FString& BatchID, const FString& ErrorMessage);

    // 更新批次进度
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void UpdateBatchProgress(const FString& BatchID, int32 Completed);

    // 完成批次生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void CompleteBatchGeneration(const FString& BatchID);

    // 初始化模板
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void InitializeBatchTemplates();

    // 生成唯一批次ID
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    FString GenerateUniqueBatchID() const;

    // 计算预估时间
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    float CalculateEstimatedTime(const FAIBatchConfiguration& Config) const;

    // 检查依赖关系
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    bool CheckDependencies(const FAIBatchConfiguration& Config) const;

    // 处理依赖生成
    UFUNCTION(BlueprintCallable, Category = "Batch Generation System")
    void ProcessDependencies(const FAIBatchConfiguration& Config, TArray<FAIGeneratedAsset>& Assets);

protected:
    // 当前世界引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // AI内容系统引用
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    UPROPERTY()
    TObjectPtr<UMingAIContentRenderer> ContentRenderer;

    UPROPERTY()
    TObjectPtr<UMingAIContentManager> ContentManager;

    // 批次数据
    UPROPERTY()
    TMap<FString, FAIBatchConfiguration> BatchConfigurations;

    UPROPERTY()
    TMap<FString, FAIBatchResult> BatchResults;

    UPROPERTY()
    TMap<FString, FAIBatchCondition> ConditionalBatches;

    // 批次模板
    UPROPERTY()
    TMap<FString, FAIBatchTemplate> BatchTemplates;

    // 运行状态
    UPROPERTY()
    TMap<FString, EBatchStatus> BatchStatuses;

    UPROPERTY()
    TMap<FString, int32> BatchProgress;

    UPROPERTY()
    TMap<FString, int32> BatchPriorities;

    // 系统配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Generation System")
    int32 MaxConcurrentBatches;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Generation System")
    int32 MaxItemsPerBatch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Generation System")
    float BatchUpdateInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Generation System")
    bool bEnableAutoOptimization;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Generation System")
    bool bEnableCaching;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Batch Generation System")
    bool bEnableParallelProcessing;

private:
    // 获取批次类型名称
    FString GetBatchTypeName(EBatchType BatchType) const;

    // 获取变化类型名称
    FString GetVariationTypeName(EVariationType VariationType) const;

    // 获取条件类型名称
    FString GetConditionTypeName(EConditionType ConditionType) const;

    // 验证批次ID
    bool IsValidBatchID(const FString& BatchID) const;

    // 清理完成的批次
    void CleanupCompletedBatches();

    // 记录批次日志
    void LogBatchMessage(const FString& BatchID, const FString& Message, bool bIsError = false);

    // 更新批次统计
    void UpdateBatchStatistics();

    // 处理批次队列
    void ProcessBatchQueue();

    // 检查批次限制
    bool CheckBatchLimits(const FAIBatchConfiguration& Config) const;

    // 获取系统状态
    TMap<FString, FString> GetSystemStatus() const;
};
