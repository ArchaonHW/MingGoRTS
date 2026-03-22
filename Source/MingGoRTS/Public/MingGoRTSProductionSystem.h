#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSProductionSystem.generated.h"

UENUM(BlueprintType)
enum class EProductionCategory : uint8
{
    Military, UMETA(DisplayName = "x?X?"),
    Civilian, UMETA(DisplayName = "?X?"),
    Industrial, UMETA(DisplayName = "u~X?"),
    Agricultural, UMETA(DisplayName = "A~X?"),
    Medical, UMETA(DisplayName = "X??X?"),
    Educational, UMETA(DisplayName = "X?|X?"),
    Infrastructure, UMETA(DisplayName = "X??X]IX?"),
    Luxury, UMETA(DisplayName = "?X??X),
    Research, UMETA(DisplayName = "?X?"),
    Special, UMETA(DisplayName = "X??X?")
};

UENUM(BlueprintType)
enum class EProductionStatus : uint8
{
    Idle, UMETA(DisplayName = "?X"),
    Preparing, UMETA(DisplayName = "?X?X),
    Producing, UMETA(DisplayName = "X??X),
    Paused, UMETA(DisplayName = "wX),
    Completed, UMETA(DisplayName = "w?X),
    Failed, UMETA(DisplayName = "?X"),
    Upgrading, UMETA(DisplayName = "X??X?X),
    Maintenance, UMETA(DisplayName = "@?X)
};

UENUM(BlueprintType)
enum class EProductionPriority : uint8
{
    Low, UMETA(DisplayName = "CuX??X"),
    Normal, UMETA(DisplayName = "X?quX??X"),
    High, UMETA(DisplayName = "uX??X"),
    Critical, UMETA(DisplayName = "uX??X"),
    Emergency, UMETA(DisplayName = "X?uX??X")
};

UENUM(BlueprintType)
enum class EProductionQuality : uint8
{
    Poor, UMETA(DisplayName = "X??"),
    Normal, UMETA(DisplayName = "X??X),
    Good, UMETA(DisplayName = "X?n"),
    Excellent, UMETA(DisplayName = "X??X"),
    Masterpiece, UMETA(DisplayName = "?X")
};

UENUM(BlueprintType)
enum class EProductionEfficiencyFactor : uint8
{
    BaseEfficiency, UMETA(DisplayName = "X??X??X"),
    SkillBonus, UMETA(DisplayName = "X?X??X),
    TechnologyBonus, UMETA(DisplayName = "?X??X"),
    BuildingBonus, UMETA(DisplayName = "?X??X"),
    ResourceQuality, UMETA(DisplayName = "?X?"),
    WorkerMorale, UMETA(DisplayName = "uHh"),
    Environmental, UMETA(DisplayName = "X??X??X"),
    Maintenance, UMETA(DisplayName = "@X?X),
    Overclock, UMETA(DisplayName = "WWX??X")
};

/**
 * X?X?赲?X
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
        RecipeID = TEXT(""};
        RecipeName = TEXT(""};
        Description = TEXT(""};
        Category = EProductionCategory::Industrial;
        ProductionTime = 0.0f;
        BaseEfficiency = 1.0f;
        MinSkillLevel = 0;
        DefaultQuality = EProductionQuality::Normal;
        bIsRepeatable = true;
        HistoricalContext = TEXT(""};
    }
};

/**
 * X?u?X?X */
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
        ProductionLineID = TEXT(""};
        ProductionLineName = TEXT(""};
        BuildingID = TEXT(""};
        Status = EProductionStatus::Idle;
        CurrentRecipeID = TEXT(""};
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
 * X?X??X?X
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
        QueueID = TEXT(""};
        ProductionLineID = TEXT(""};
        bIsLooping = false;
        CurrentIndex = 0;
        bAutoStart = true;
    }
};

/**
 * X??X?X
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
 * X?tX?m?X
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

// ?Xƥe?X
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionStarted, const FString&, ProductionLineID, const FString&, RecipeID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProductionProgress, const FString&, ProductionLineID, float, Progress, float, TimeRemaining};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionCompleted, const FString&, ProductionLineID, const TMap<EResourceType, float>&, OutputResources};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionFailed, const FString&, ProductionLineID, const FString&, ErrorMessage};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionPaused, const FString&, ProductionLineID, const FString&, Reason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionResumed, const FString&, ProductionLineID, const FString&, Reason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProductionQueueChanged, const FString&, QueueID, const TArray<FString>&, RecipeQueue};

/**
 * X?sytΥDO
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionSystem(};

    // X?tΨƥ
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

    // X??X??X?t?X    UFUNCTION(BlueprintCallable, Category = "Production System")
    void InitializeProductionSystem(};

    // X??X?X??X??X    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<EProductionCategory> GetAllProductionCategories() const;

    // X??X??XOX??X    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionRecipe> GetRecipesByCategory(EProductionCategory Category) const;

    // X??X?X?
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionRecipe GetProductionRecipe(const FString& RecipeID) const;

    // X?X??X    UFUNCTION(BlueprintCallable, Category = "Production System")
    FString CreateProductionLine(const FString& ProductionLineName, const FString& BuildingID};

    // X??X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool StartProduction(const FString& ProductionLineID, const FString& RecipeID, EProductionPriority Priority = EProductionPriority::Normal};

    // X??X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool PauseProduction(const FString& ProductionLineID, const FString& Reason = TEXT("X??X??X")};

    // X?_X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ResumeProduction(const FString& ProductionLineID, const FString& Reason = TEXT("X??X?_")};

    // X??X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool CancelProduction(const FString& ProductionLineID};

    // ]mX?X??X?X    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetProductionPriority(const FString& ProductionLineID, EProductionPriority Priority};

    // K?X??X??X    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool AddToProductionQueue(const FString& ProductionLineID, const FString& RecipeID};

    // q?X??X    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool RemoveFromProductionQueue(const FString& ProductionLineID, int32 QueueIndex};

    // MX?X??X
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ClearProductionQueue(const FString& ProductionLineID};

    // X??X?u?X    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionLine GetProductionLine(const FString& ProductionLineID) const;

    // X??X?X??X
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionQueue GetProductionQueue(const FString& ProductionLineID) const;

    // X??X?X??X??X
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionLine> GetAllProductionLines() const;

    // p?X?X??X
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionEfficiency(const FString& ProductionLineID) const;

    // p?X?X??X
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionTime(const FString& ProductionLineID, const FString& RecipeID) const;

    // ˬdX?
    UFUNCTION(BlueprintPure, Category = "Production System")
    bool CanProduceRecipe(const FString& ProductionLineID, const FString& RecipeID) const;

    // X??X??X
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionStatistics GetProductionStatistics() const;

    // X?sX?X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionProgress(float DeltaTime};

    // X??X??X    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool UpgradeProductionLine(const FString& ProductionLineID};

    // @X??X    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool MaintainProductionLine(const FString& ProductionLineID};

    // ]mX??X    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetAutomation(const FString& ProductionLineID, bool bIsAutomated};

    // X??XtX?m
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionSystemConfig GetProductionConfig() const;

    // X?stX?m
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionConfig(const FProductionSystemConfig& NewConfig};

    // O?X?X??X
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SaveProductionData(const FString& SaveSlotName};

    // JX?X??X
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool LoadProductionData(const FString& SaveSlotName};

protected:
    // X?u?X?X    UPROPERTY()
    TMap<FString, FProductionLine> ProductionLines;

    // X?X?X??X
    UPROPERTY()
    TMap<FString, FProductionRecipe> ProductionRecipes;

    // X?X??X??X
    UPROPERTY()
    TMap<FString, FProductionQueue> ProductionQueues;

    // X??X
    UPROPERTY()
    FProductionStatistics Statistics;

    // tX?m
    UPROPERTY()
    FProductionSystemConfig Config;

    // X?_w?Xl?X
    bool bIsInitialized;

    // X??X??X??X??X??X    void InitializeRepublicanEraRecipes(};

    // X?حx?X?X?
    void CreateMilitaryProductionRecipes(};

    // X?إ?X?X?
    void CreateCivilianProductionRecipes(};

    // X?ؤu~X?X?
    void CreateIndustrialProductionRecipes(};

    // X?عA~X?X?
    void CreateAgriculturalProductionRecipes(};

    // X?X??X?X?
    void CreateMedicalProductionRecipes(};

    // X?X?|X?X?
    void CreateEducationalProductionRecipes(};

    // X?X??X]IX?X?
    void CreateInfrastructureProductionRecipes(};

    // X?ذ?X??X??X    void CreateLuxuryProductionRecipes(};

    // X??X??X
    void HandleProductionCompleted(const FString& ProductionLineID};

    // X??X??X
    void HandleProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage};

    // X?sX?u?X    void UpdateProductionLineStatus(const FString& ProductionLineID};

    // X??X?X??X
    void ProcessProductionQueue(const FString& ProductionLineID};

    // X?X??X??X
    void ApplyEfficiencyFactors(const FString& ProductionLineID};

    // p?X??X    float CalculateFinalEfficiency(const FProductionLine& ProductionLine) const;

    // X??X?OX?
    FString GetProductionCategoryName(EProductionCategory Category) const;

    // X??X?X?X??X?X    FString GetProductionStatusName(EProductionStatus Status) const;

    // ?X?X??X
    bool ValidateRecipeData(const FProductionRecipe& Recipe) const;

    // X??X?uID
    FString GenerateProductionLineID(const FString& BaseName) const;

    // X??X?ID
    FString GenerateRecipeID(const FString& BaseName, EProductionCategory Category) const;

    // O?X?v
    void RecordProductionHistory(const FString& Event};

    // X?vO?X
    UPROPERTY()
    TArray<FString> ProductionHistory;

    // ?XtΤޥ
    UPROPERTY()
    class UMingGoRTSResourceSystem* ResourceSystem;
};

