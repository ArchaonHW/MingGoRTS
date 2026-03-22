#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSProductionSystem.generated.h"

UENUM(BlueprintType)
enum class EProductionCategory : uint8
{
    Military           UMETA(DisplayName = "Ëªç‰??üÁî¢"),
    Civilian           UMETA(DisplayName = "Ê∞ëÁ??üÁî¢"),
    Industrial         UMETA(DisplayName = "Â∑•Ê•≠?üÁî¢"),
    Agricultural       UMETA(DisplayName = "Ëæ≤Ê•≠?üÁî¢"),
    Medical           UMETA(DisplayName = "?´Á??üÁî¢"),
    Educational        UMETA(DisplayName = "?ôËÇ≤?üÁî¢"),
    Infrastructure     UMETA(DisplayName = "?∫Á?Ë®≠ÊñΩ?üÁî¢"),
    Luxury            UMETA(DisplayName = "Â•¢‰??ÅÁ???),
    Research          UMETA(DisplayName = "ÁßëÁ??üÁî¢"),
    Special           UMETA(DisplayName = "?πÊ??üÁî¢")
};

UENUM(BlueprintType)
enum class EProductionStatus : uint8
{
    Idle              UMETA(DisplayName = "Á©∫È?"),
    Preparing         UMETA(DisplayName = "Ê∫ñÂ?‰∏?),
    Producing         UMETA(DisplayName = "?üÁî¢‰∏?),
    Paused            UMETA(DisplayName = "Â∑≤Êö´??),
    Completed         UMETA(DisplayName = "Â∑≤Â???),
    Failed            UMETA(DisplayName = "Â§±Ê?"),
    Upgrading         UMETA(DisplayName = "?áÁ?‰∏?),
    Maintenance        UMETA(DisplayName = "Á∂≠Ë≠∑‰∏?)
};

UENUM(BlueprintType)
enum class EProductionPriority : uint8
{
    Low               UMETA(DisplayName = "‰ΩéÂÑ™?àÁ?"),
    Normal            UMETA(DisplayName = "?ÆÈÄöÂÑ™?àÁ?"),
    High              UMETA(DisplayName = "È´òÂÑ™?àÁ?"),
    Critical          UMETA(DisplayName = "Á∑äÊÄ•ÂÑ™?àÁ?"),
    Emergency         UMETA(DisplayName = "?âÊÄ•ÂÑ™?àÁ?")
};

UENUM(BlueprintType)
enum class EProductionQuality : uint8
{
    Poor              UMETA(DisplayName = "??≥™"),
    Normal            UMETA(DisplayName = "?ÆÈÄ?),
    Good              UMETA(DisplayName = "?ØÂ•Ω"),
    Excellent         UMETA(DisplayName = "?™Á?"),
    Masterpiece        UMETA(DisplayName = "Á≤æÂ?")
};

UENUM(BlueprintType)
enum class EProductionEfficiencyFactor : uint8
{
    BaseEfficiency    UMETA(DisplayName = "?∫Á??àÁ?"),
    SkillBonus        UMETA(DisplayName = "?Ä?ΩÂ???),
    TechnologyBonus   UMETA(DisplayName = "ÁßëÊ??†Ê?"),
    BuildingBonus     UMETA(DisplayName = "Âª∫Á??†Ê?"),
    ResourceQuality   UMETA(DisplayName = "Ë≥áÊ??ÅË≥™"),
    WorkerMorale     UMETA(DisplayName = "Â∑•‰∫∫Â£´Ê∞£"),
    Environmental     UMETA(DisplayName = "?∞Â??†Á?"),
    Maintenance       UMETA(DisplayName = "Á∂≠Ë≠∑?Ä??),
    Overclock         UMETA(DisplayName = "Ë∂ÖÈ†ª?ãË?")
};

/**
 * ?üÁî¢?çÊñπÁµêÊ?
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
 * ?üÁî¢Á∑öÁ?Êß? */
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
 * ?üÁî¢?äÂ?ÁµêÊ?
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
 * ?üÁî¢Áµ±Ë?ÁµêÊ?
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
 * ?üÁî¢Á≥ªÁµ±?çÁΩÆÁµêÊ?
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

// ÂÆ??‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionStarted, const FString&, ProductionLineID, const FString&, RecipeID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProductionProgress, const FString&, ProductionLineID, float, Progress, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionCompleted, const FString&, ProductionLineID, const TMap<EResourceType, float>&, OutputResources);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionFailed, const FString&, ProductionLineID, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionPaused, const FString&, ProductionLineID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionResumed, const FString&, ProductionLineID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionQueueChanged, const FString&, QueueID, const TArray<FString>&, RecipeQueue);

/**
 * ?üÁî¢Ë£ΩÈÄ†Á≥ªÁµ±‰∏ªÈ°ûÂà•
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionSystem();

    // ?üÁî¢Á≥ªÁµ±‰∫ã‰ª∂
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

    // ?ùÂ??ñÁ??¢Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Production System")
    void InitializeProductionSystem();

    // ?≤Â??Ä?âÁ??¢È???    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<EProductionCategory> GetAllProductionCategories() const;

    // ?≤Â??áÂ?È°ûÂà•?ÑÈ???    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionRecipe> GetRecipesByCategory(EProductionCategory Category) const;

    // ?≤Â??üÁî¢?çÊñπ
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionRecipe GetProductionRecipe(const FString& RecipeID) const;

    // ?µÂª∫?üÁî¢Á∑?    UFUNCTION(BlueprintCallable, Category = "Production System")
    FString CreateProductionLine(const FString& ProductionLineName, const FString& BuildingID);

    // ?ãÂ??üÁî¢
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool StartProduction(const FString& ProductionLineID, const FString& RecipeID, EProductionPriority Priority = EProductionPriority::Normal);

    // ?´Â??üÁî¢
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool PauseProduction(const FString& ProductionLineID, const FString& Reason = TEXT("?ãÂ??´Â?"));

    // ?¢Âæ©?üÁî¢
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ResumeProduction(const FString& ProductionLineID, const FString& Reason = TEXT("?ãÂ??¢Âæ©"));

    // ?ñÊ??üÁî¢
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool CancelProduction(const FString& ProductionLineID);

    // Ë®≠ÁΩÆ?üÁî¢?™Â?Á¥?    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetProductionPriority(const FString& ProductionLineID, EProductionPriority Priority);

    // Ê∑ªÂ??∞Á??¢È???    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool AddToProductionQueue(const FString& ProductionLineID, const FString& RecipeID);

    // ÁßªÈô§ÂæûÁ??¢È???    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool RemoveFromProductionQueue(const FString& ProductionLineID, int32 QueueIndex);

    // Ê∏ÖÁ©∫?üÁî¢?äÂ?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ClearProductionQueue(const FString& ProductionLineID);

    // ?≤Â??üÁî¢Á∑öÁ???    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionLine GetProductionLine(const FString& ProductionLineID) const;

    // ?≤Â??üÁî¢?äÂ?
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionQueue GetProductionQueue(const FString& ProductionLineID) const;

    // ?≤Â??Ä?âÁ??¢Á?
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionLine> GetAllProductionLines() const;

    // Ë®àÁ??üÁî¢?àÁ?
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionEfficiency(const FString& ProductionLineID) const;

    // Ë®àÁ??üÁî¢?ÇÈ?
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionTime(const FString& ProductionLineID, const FString& RecipeID) const;

    // Ê™¢Êü•?üÁî¢Ê¢ù‰ª∂
    UFUNCTION(BlueprintPure, Category = "Production System")
    bool CanProduceRecipe(const FString& ProductionLineID, const FString& RecipeID) const;

    // ?≤Â??üÁî¢Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionStatistics GetProductionStatistics() const;

    // ?¥Êñ∞?üÁî¢?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionProgress(float DeltaTime);

    // ?áÁ??üÁî¢Á∑?    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool UpgradeProductionLine(const FString& ProductionLineID);

    // Á∂≠Ë≠∑?üÁî¢Á∑?    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool MaintainProductionLine(const FString& ProductionLineID);

    // Ë®≠ÁΩÆ?™Â???    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetAutomation(const FString& ProductionLineID, bool bIsAutomated);

    // ?≤Â?Á≥ªÁµ±?çÁΩÆ
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionSystemConfig GetProductionConfig() const;

    // ?¥Êñ∞Á≥ªÁµ±?çÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionConfig(const FProductionSystemConfig& NewConfig);

    // ‰øùÂ??üÁî¢?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SaveProductionData(const FString& SaveSlotName);

    // ËºâÂÖ•?üÁî¢?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool LoadProductionData(const FString& SaveSlotName);

protected:
    // ?üÁî¢Á∑öÊ?Â∞?    UPROPERTY()
    TMap<FString, FProductionLine> ProductionLines;

    // ?üÁî¢?çÊñπ?†Â?
    UPROPERTY()
    TMap<FString, FProductionRecipe> ProductionRecipes;

    // ?üÁî¢?äÂ??†Â?
    UPROPERTY()
    TMap<FString, FProductionQueue> ProductionQueues;

    // ?üÁî¢Áµ±Ë?
    UPROPERTY()
    FProductionStatistics Statistics;

    // Á≥ªÁµ±?çÁΩÆ
    UPROPERTY()
    FProductionSystemConfig Config;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ??ãÊ??üÁ??¢È???    void InitializeRepublicanEraRecipes();

    // ?µÂª∫Ëªç‰??üÁî¢?çÊñπ
    void CreateMilitaryProductionRecipes();

    // ?µÂª∫Ê∞ëÁ??üÁî¢?çÊñπ
    void CreateCivilianProductionRecipes();

    // ?µÂª∫Â∑•Ê•≠?üÁî¢?çÊñπ
    void CreateIndustrialProductionRecipes();

    // ?µÂª∫Ëæ≤Ê•≠?üÁî¢?çÊñπ
    void CreateAgriculturalProductionRecipes();

    // ?µÂª∫?´Á??üÁî¢?çÊñπ
    void CreateMedicalProductionRecipes();

    // ?µÂª∫?ôËÇ≤?üÁî¢?çÊñπ
    void CreateEducationalProductionRecipes();

    // ?µÂª∫?∫Á?Ë®≠ÊñΩ?üÁî¢?çÊñπ
    void CreateInfrastructureProductionRecipes();

    // ?µÂª∫Â•¢‰??ÅÁ??¢È???    void CreateLuxuryProductionRecipes();

    // ?ïÁ??üÁî¢ÂÆåÊ?
    void HandleProductionCompleted(const FString& ProductionLineID);

    // ?ïÁ??üÁî¢Â§±Ê?
    void HandleProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage);

    // ?¥Êñ∞?üÁî¢Á∑öÁ???    void UpdateProductionLineStatus(const FString& ProductionLineID);

    // ?ïÁ??üÁî¢?äÂ?
    void ProcessProductionQueue(const FString& ProductionLineID);

    // ?âÁî®?àÁ??†Â?
    void ApplyEfficiencyFactors(const FString& ProductionLineID);

    // Ë®àÁ??ÄÁµÇÊ???    float CalculateFinalEfficiency(const FProductionLine& ProductionLine) const;

    // ?≤Â??üÁî¢È°ûÂà•?çÁ®±
    FString GetProductionCategoryName(EProductionCategory Category) const;

    // ?≤Â??üÁî¢?Ä?ãÂ?Á®?    FString GetProductionStatusName(EProductionStatus Status) const;

    // È©óË??çÊñπ?∏Ê?
    bool ValidateRecipeData(const FProductionRecipe& Recipe) const;

    // ?üÊ??üÁî¢Á∑öID
    FString GenerateProductionLineID(const FString& BaseName) const;

    // ?üÊ??çÊñπID
    FString GenerateRecipeID(const FString& BaseName, EProductionCategory Category) const;

    // Ë®òÈ??üÁî¢Ê≠∑Âè≤
    void RecordProductionHistory(const FString& Event);

    // ?üÁî¢Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FString> ProductionHistory;

    // Ë≥áÊ?Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    class UMingGoRTSResourceSystem* ResourceSystem;
};
