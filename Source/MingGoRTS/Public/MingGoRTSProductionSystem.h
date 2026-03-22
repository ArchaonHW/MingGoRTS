#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSProductionSystem.generated.h"

UENUM(BlueprintType)
enum class EProductionCategory: uint8 {
    Military, UMETA(DisplayName = "x?X?"),
    Civilian, UMETA(DisplayName = "?X?"),
    Industrial, UMETA(DisplayName = "u~X?"),
    Agricultural, UMETA(DisplayName = "A~X?"),
    Medical, UMETA(DisplayName = "XXX?"),
    Educational, UMETA(DisplayName = "X?|X?"),
    Infrastructure, UMETA(DisplayName = "XXX]IX?"),
    Luxury, UMETA(DisplayName = "?XXX),
    Research, UMETA(DisplayName = "?X?"),
    Special, UMETA(DisplayName = "XXX?")
};

UENUM(BlueprintType)
enum class EProductionStatus: uint8 {
    Idle, UMETA(DisplayName = "?X"),
    Preparing, UMETA(DisplayName = "?X?X),
    Producing, UMETA(DisplayName = "XXX),
    Paused, UMETA(DisplayName = "wX),
    Completed, UMETA(DisplayName = "w?X),
    Failed, UMETA(DisplayName = "?X"),
    Upgrading, UMETA(DisplayName = "XXX?X),
    Maintenance, UMETA(DisplayName = "@?X)
};

UENUM(BlueprintType)
enum class EProductionPriority: uint8 {
    Low, UMETA(DisplayName = "CuXXX"),
    Normal, UMETA(DisplayName = "X?quXXX"),
    High, UMETA(DisplayName = "uXXX"),
    Critical, UMETA(DisplayName = "uXXX"),
    Emergency, UMETA(DisplayName = "X?uXXX")
};

UENUM(BlueprintType)
enum class EProductionQuality: uint8 {
    Poor, UMETA(DisplayName = "XX"),
    Normal, UMETA(DisplayName = "XXX),
    Good, UMETA(DisplayName = "X?n"),
    Excellent, UMETA(DisplayName = "XXX"),
    Masterpiece, UMETA(DisplayName = "?X")
};

UENUM(BlueprintType)
enum class EProductionEfficiencyFactor: uint8 {
    BaseEfficiency, UMETA(DisplayName = "XXXXX"),
    SkillBonus, UMETA(DisplayName = "X?XXX),
    TechnologyBonus, UMETA(DisplayName = "?XXX"),
    BuildingBonus, UMETA(DisplayName = "?XXX"),
    ResourceQuality, UMETA(DisplayName = "?X?"),
    WorkerMorale, UMETA(DisplayName = "uHh"),
    Environmental, UMETA(DisplayName = "XXXXX"),
    Maintenance, UMETA(DisplayName = "@X?X),
    Overclock, UMETA(DisplayName = "WWXXX")
};

/**
 * X?X?в?X
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
);

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
);

/**
 * X?XXX?X
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
);

/**
 * XXX?X
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

// ?X?e?X








/**
 * X?syt�WDO
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSProductionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionSystem();

    // X?t�Z?
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

    // XXXXX?t?X
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void InitializeProductionSystem();

    // XXX?XXXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<EProductionCategory> GetAllProductionCategories() const;

    // XXXXXOXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionRecipe> GetRecipesByCategory(EProductionCategory Category) const;

    // XXX?X?
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionRecipe GetProductionRecipe(const FString& RecipeID) const;

    // X?XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    FString CreateProductionLine(const FString& ProductionLineName, const FString& BuildingID);

    // XXX?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool StartProduction(const FString& ProductionLineID, const FString& RecipeID, EProductionPriority Priority = EProductionPriority::Normal);

    // XXX?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool PauseProduction(const FString& ProductionLineID, const FString& Reason = TEXT("XXXXX")};

    // X?_X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ResumeProduction(const FString& ProductionLineID, const FString& Reason = TEXT("XXX?_")};

    // XXX?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool CancelProduction(const FString& ProductionLineID);

    // ]mX?XXX?X
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetProductionPriority(const FString& ProductionLineID, EProductionPriority Priority);

    // K?XXXXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool AddToProductionQueue(const FString& ProductionLineID, const FString& RecipeID);

    // q?XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool RemoveFromProductionQueue(const FString& ProductionLineID, int32 QueueIndex);

    // MX?XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ClearProductionQueue(const FString& ProductionLineID);

    // XXX?u?X
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionLine GetProductionLine(const FString& ProductionLineID) const;

    // XXX?XXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionQueue GetProductionQueue(const FString& ProductionLineID) const;

    // XXX?XXXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionLine> GetAllProductionLines() const;

    // p?X?XXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionEfficiency(const FString& ProductionLineID) const;

    // p?X?XXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionTime(const FString& ProductionLineID, const FString& RecipeID) const;

    // ?dX?
    UFUNCTION(BlueprintPure, Category = "Production System")
    bool CanProduceRecipe(const FString& ProductionLineID, const FString& RecipeID) const;

    // XXXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionStatistics GetProductionStatistics() const;

    // X?sX?X?
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionProgress(float DeltaTime);

    // XXXXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool UpgradeProductionLine(const FString& ProductionLineID);

    // @XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool MaintainProductionLine(const FString& ProductionLineID);

    // ]mXXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetAutomation(const FString& ProductionLineID, bool bIsAutomated);

    // XXXtX?m
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionSystemConfig GetProductionConfig() const;

    // X?stX?m
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionConfig(const FProductionSystemConfig& NewConfig);

    // O?X?XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SaveProductionData(const FString& SaveSlotName);

    // JX?XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool LoadProductionData(const FString& SaveSlotName);

protected:
    // X?u?X?X
    UPROPERTY()
    TMap<FString, FProductionLine> ProductionLines;

    // X?X?XXX
    UPROPERTY()
    TMap<FString, FProductionRecipe> ProductionRecipes;

    // X?XXXXX
    UPROPERTY()
    TMap<FString, FProductionQueue> ProductionQueues;

    // XXX
    UPROPERTY()
    FProductionStatistics Statistics;

    // tX?m
    UPROPERTY()
    FProductionSystemConfig Config;

    // X?_w?Xl?X
    bool bIsInitialized;

    // XXXXXXXXXXX
    void InitializeRepublicanEraRecipes();

    // X??x?X?X?
    void CreateMilitaryProductionRecipes();

    // X???X?X?
    void CreateCivilianProductionRecipes();

    // X??u~X?X?
    void CreateIndustrialProductionRecipes();

    // X??A~X?X?
    void CreateAgriculturalProductionRecipes();

    // X?XXX?X?
    void CreateMedicalProductionRecipes();

    // X?X?|X?X?
    void CreateEducationalProductionRecipes();

    // X?XXX]IX?X?
    void CreateInfrastructureProductionRecipes();

    // X???XXXXX
    void CreateLuxuryProductionRecipes();

    // XXXXX
    void HandleProductionCompleted(const FString& ProductionLineID);

    // XXXXX
    void HandleProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage);

    // X?sX?u?X
    void UpdateProductionLineStatus(const FString& ProductionLineID);

    // XXX?XXX
    void ProcessProductionQueue(const FString& ProductionLineID);

    // X?XXXXX
    void ApplyEfficiencyFactors(const FString& ProductionLineID);

    // p?XXX
    float CalculateFinalEfficiency(const FProductionLine& ProductionLine) const;

    // XXX?OX?
    FString GetProductionCategoryName(EProductionCategory Category) const;

    // XXX?X?XXX?X
    FString GetProductionStatusName(EProductionStatus Status) const;

    // ?X?XXX
    bool ValidateRecipeData(const FProductionRecipe& Recipe) const;

    // XXX?uID
    FString GenerateProductionLineID(const FString& BaseName) const;

    // XXX?ID
    FString GenerateRecipeID(const FString& BaseName, EProductionCategory Category) const;

    // O?X?v
    void RecordProductionHistory(const FString& Event);

    // X?vO?X
    UPROPERTY()
    TArray<FString> ProductionHistory;

    // ?Xt�V?
    UPROPERTY()
    class UMingGoRTSResourceSystem* ResourceSystem;
};

