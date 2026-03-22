#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSProductionSystem.generated.h"

UENUM(BlueprintType)
enum class EProductionCategory : uint8
{
    Military           UMETA(DisplayName = "軍事生產"),
    Civilian           UMETA(DisplayName = "民生生產"),
    Industrial         UMETA(DisplayName = "工業生產"),
    Agricultural       UMETA(DisplayName = "農業生產"),
    Medical           UMETA(DisplayName = "醫療生產"),
    Educational        UMETA(DisplayName = "教育生產"),
    Infrastructure     UMETA(DisplayName = "基礎設施生產"),
    Luxury            UMETA(DisplayName = "奢侈品生產"),
    Research          UMETA(DisplayName = "科研生產"),
    Special           UMETA(DisplayName = "特殊生產")
};

UENUM(BlueprintType)
enum class EProductionStatus : uint8
{
    Idle              UMETA(DisplayName = "空閒"),
    Preparing         UMETA(DisplayName = "準備中"),
    Producing         UMETA(DisplayName = "生產中"),
    Paused            UMETA(DisplayName = "已暫停"),
    Completed         UMETA(DisplayName = "已完成"),
    Failed            UMETA(DisplayName = "失敗"),
    Upgrading         UMETA(DisplayName = "升級中"),
    Maintenance        UMETA(DisplayName = "維護中")
};

UENUM(BlueprintType)
enum class EProductionPriority : uint8
{
    Low               UMETA(DisplayName = "低優先級"),
    Normal            UMETA(DisplayName = "普通優先級"),
    High              UMETA(DisplayName = "高優先級"),
    Critical          UMETA(DisplayName = "緊急優先級"),
    Emergency         UMETA(DisplayName = "應急優先級")
};

UENUM(BlueprintType)
enum class EProductionQuality : uint8
{
    Poor              UMETA(DisplayName = "劣質"),
    Normal            UMETA(DisplayName = "普通"),
    Good              UMETA(DisplayName = "良好"),
    Excellent         UMETA(DisplayName = "優秀"),
    Masterpiece        UMETA(DisplayName = "精品")
};

UENUM(BlueprintType)
enum class EProductionEfficiencyFactor : uint8
{
    BaseEfficiency    UMETA(DisplayName = "基礎效率"),
    SkillBonus        UMETA(DisplayName = "技能加成"),
    TechnologyBonus   UMETA(DisplayName = "科技加成"),
    BuildingBonus     UMETA(DisplayName = "建築加成"),
    ResourceQuality   UMETA(DisplayName = "資源品質"),
    WorkerMorale     UMETA(DisplayName = "工人士氣"),
    Environmental     UMETA(DisplayName = "環境因素"),
    Maintenance       UMETA(DisplayName = "維護狀態"),
    Overclock         UMETA(DisplayName = "超頻運行")
};

/**
 * 生產配方結構
 */
USTRUCT(BlueprintType)
struct FPRODUCTION_API FProductionRecipe
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    FString RecipeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    FString RecipeName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    EProductionCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    TArray<EResourceType> RequiredBuildings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    TMap<EResourceType, float> InputResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    TMap<EResourceType, float> OutputResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    float ProductionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    float BaseEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    int32 MinSkillLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    TArray<FString> RequiredTechnologies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    EProductionQuality DefaultQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    bool bIsRepeatable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Recipe")
    FString HistoricalContext;

    FProductionRecipe()
    {
        RecipeID = TEXT("");
        RecipeName = TEXT("");
        Description = TEXT("");
        Category = EProductionCategory::Industrial;
        ProductionTime = 0.0f;
        BaseEfficiency = 1.0f;
        MinSkillLevel = 0;
        DefaultQuality = EProductionQuality::Normal;
        bIsRepeatable = true;
        HistoricalContext = TEXT("");
    }
};

/**
 * 生產線結構
 */
USTRUCT(BlueprintType)
struct FPRODUCTION_API FProductionLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    FString ProductionLineID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    FString ProductionLineName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    FString BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    EProductionStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    FString CurrentRecipeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    float Progress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    float TimeRemaining;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    EProductionPriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    TMap<EProductionEfficiencyFactor, float> EfficiencyFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    float CurrentEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    int32 ProductionCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    FDateTime LastUpdateTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    bool bIsAutomated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Line")
    bool bIsPaused;

    FProductionLine()
    {
        ProductionLineID = TEXT("");
        ProductionLineName = TEXT("");
        BuildingID = TEXT("");
        Status = EProductionStatus::Idle;
        CurrentRecipeID = TEXT("");
        Progress = 0.0f;
        TimeRemaining = 0.0f;
        Priority = EProductionPriority::Normal;
        CurrentEfficiency = 1.0f;
        ProductionCount = 0;
        bIsAutomated = false;
        bIsPaused = false;
    }
};

/**
 * 生產隊列結構
 */
USTRUCT(BlueprintType)
struct FPRODUCTION_API FProductionQueue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Queue")
    FString QueueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Queue")
    FString ProductionLineID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Queue")
    TArray<FString> RecipeQueue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Queue")
    bool bIsLooping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Queue")
    int32 CurrentIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Queue")
    bool bAutoStart;

    FProductionQueue()
    {
        QueueID = TEXT("");
        ProductionLineID = TEXT("");
        bIsLooping = false;
        CurrentIndex = 0;
        bAutoStart = true;
    }
};

/**
 * 生產統計結構
 */
USTRUCT(BlueprintType)
struct FPRODUCTION_API FProductionStatistics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    TMap<EResourceType, float> TotalProduced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    TMap<EResourceType, float> TotalConsumed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    TMap<EProductionCategory, float> CategoryProduction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    float TotalProductionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    float AverageEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    int32 TotalProductionRuns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    int32 SuccessfulRuns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    int32 FailedRuns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Statistics")
    FDateTime LastProductionTime;

    FProductionStatistics()
    {
        TotalProductionTime = 0.0f;
        AverageEfficiency = 0.0f;
        TotalProductionRuns = 0;
        SuccessfulRuns = 0;
        FailedRuns = 0;
    }
};

/**
 * 生產系統配置結構
 */
USTRUCT(BlueprintType)
struct FPRODUCTION_API FProductionSystemConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    float GlobalProductionSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    TMap<EProductionCategory, float> CategoryBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    float MaxOverclockEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    float MaintenancePenalty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    bool bEnableAutoDistribution;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    bool bEnableQualityControl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production Config")
    int32 MaxQueueSize;

    FProductionSystemConfig()
    {
        GlobalProductionSpeed = 1.0f;
        MaxOverclockEfficiency = 1.5f;
        MaintenancePenalty = 0.3f;
        bEnableAutoDistribution = true;
        bEnableQualityControl = true;
        MaxQueueSize = 10;
    }
};

// 宣告事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionStarted, const FString&, ProductionLineID, const FString&, RecipeID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProductionProgress, const FString&, ProductionLineID, float, Progress, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionCompleted, const FString&, ProductionLineID, const TMap<EResourceType, float>&, OutputResources);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionFailed, const FString&, ProductionLineID, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionPaused, const FString&, ProductionLineID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionResumed, const FString&, ProductionLineID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionQueueChanged, const FString&, QueueID, const TArray<FString>&, RecipeQueue);

/**
 * 生產製造系統主類別
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionSystem();

    // 生產系統事件
    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionStarted OnProductionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionProgress OnProductionProgress;

    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionCompleted OnProductionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionFailed OnProductionFailed;

    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionPaused OnProductionPaused;

    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionResumed OnProductionResumed;

    UPROPERTY(BlueprintAssignable, Category = "Production System")
    FOnProductionQueueChanged OnProductionQueueChanged;

    // 初始化生產系統
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void InitializeProductionSystem();

    // 獲取所有生產類別
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<EProductionCategory> GetAllProductionCategories() const;

    // 獲取指定類別的配方
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionRecipe> GetRecipesByCategory(EProductionCategory Category) const;

    // 獲取生產配方
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionRecipe GetProductionRecipe(const FString& RecipeID) const;

    // 創建生產線
    UFUNCTION(BlueprintCallable, Category = "Production System")
    FString CreateProductionLine(const FString& ProductionLineName, const FString& BuildingID);

    // 開始生產
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool StartProduction(const FString& ProductionLineID, const FString& RecipeID, EProductionPriority Priority = EProductionPriority::Normal);

    // 暫停生產
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool PauseProduction(const FString& ProductionLineID, const FString& Reason = TEXT("手動暫停"));

    // 恢復生產
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ResumeProduction(const FString& ProductionLineID, const FString& Reason = TEXT("手動恢復"));

    // 取消生產
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool CancelProduction(const FString& ProductionLineID);

    // 設置生產優先級
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetProductionPriority(const FString& ProductionLineID, EProductionPriority Priority);

    // 添加到生產隊列
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool AddToProductionQueue(const FString& ProductionLineID, const FString& RecipeID);

    // 移除從生產隊列
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool RemoveFromProductionQueue(const FString& ProductionLineID, int32 QueueIndex);

    // 清空生產隊列
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ClearProductionQueue(const FString& ProductionLineID);

    // 獲取生產線狀態
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionLine GetProductionLine(const FString& ProductionLineID) const;

    // 獲取生產隊列
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionQueue GetProductionQueue(const FString& ProductionLineID) const;

    // 獲取所有生產線
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionLine> GetAllProductionLines() const;

    // 計算生產效率
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionEfficiency(const FString& ProductionLineID) const;

    // 計算生產時間
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionTime(const FString& ProductionLineID, const FString& RecipeID) const;

    // 檢查生產條件
    UFUNCTION(BlueprintPure, Category = "Production System")
    bool CanProduceRecipe(const FString& ProductionLineID, const FString& RecipeID) const;

    // 獲取生產統計
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionStatistics GetProductionStatistics() const;

    // 更新生產進度
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionProgress(float DeltaTime);

    // 升級生產線
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool UpgradeProductionLine(const FString& ProductionLineID);

    // 維護生產線
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool MaintainProductionLine(const FString& ProductionLineID);

    // 設置自動化
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetAutomation(const FString& ProductionLineID, bool bIsAutomated);

    // 獲取系統配置
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionSystemConfig GetProductionConfig() const;

    // 更新系統配置
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionConfig(const FProductionSystemConfig& NewConfig);

    // 保存生產數據
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SaveProductionData(const FString& SaveSlotName);

    // 載入生產數據
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool LoadProductionData(const FString& SaveSlotName);

protected:
    // 生產線映射
    UPROPERTY()
    TMap<FString, FProductionLine> ProductionLines;

    // 生產配方映射
    UPROPERTY()
    TMap<FString, FProductionRecipe> ProductionRecipes;

    // 生產隊列映射
    UPROPERTY()
    TMap<FString, FProductionQueue> ProductionQueues;

    // 生產統計
    UPROPERTY()
    FProductionStatistics Statistics;

    // 系統配置
    UPROPERTY()
    FProductionSystemConfig Config;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國時期生產配方
    void InitializeRepublicanEraRecipes();

    // 創建軍事生產配方
    void CreateMilitaryProductionRecipes();

    // 創建民生生產配方
    void CreateCivilianProductionRecipes();

    // 創建工業生產配方
    void CreateIndustrialProductionRecipes();

    // 創建農業生產配方
    void CreateAgriculturalProductionRecipes();

    // 創建醫療生產配方
    void CreateMedicalProductionRecipes();

    // 創建教育生產配方
    void CreateEducationalProductionRecipes();

    // 創建基礎設施生產配方
    void CreateInfrastructureProductionRecipes();

    // 創建奢侈品生產配方
    void CreateLuxuryProductionRecipes();

    // 處理生產完成
    void HandleProductionCompleted(const FString& ProductionLineID);

    // 處理生產失敗
    void HandleProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage);

    // 更新生產線狀態
    void UpdateProductionLineStatus(const FString& ProductionLineID);

    // 處理生產隊列
    void ProcessProductionQueue(const FString& ProductionLineID);

    // 應用效率因子
    void ApplyEfficiencyFactors(const FString& ProductionLineID);

    // 計算最終效率
    float CalculateFinalEfficiency(const FProductionLine& ProductionLine) const;

    // 獲取生產類別名稱
    FString GetProductionCategoryName(EProductionCategory Category) const;

    // 獲取生產狀態名稱
    FString GetProductionStatusName(EProductionStatus Status) const;

    // 驗證配方數據
    bool ValidateRecipeData(const FProductionRecipe& Recipe) const;

    // 生成生產線ID
    FString GenerateProductionLineID(const FString& BaseName) const;

    // 生成配方ID
    FString GenerateRecipeID(const FString& BaseName, EProductionCategory Category) const;

    // 記錄生產歷史
    void RecordProductionHistory(const FString& Event);

    // 生產歷史記錄
    UPROPERTY()
    TArray<FString> ProductionHistory;

    // 資源系統引用
    UPROPERTY()
    class UMingGoRTSResourceSystem* ResourceSystem;
};
