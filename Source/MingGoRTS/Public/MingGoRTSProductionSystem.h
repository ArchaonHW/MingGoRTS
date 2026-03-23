#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSProductionSystem.generated.h"

UENUM(BlueprintType)
enum class EProductionCategory: uuint8 {
    Military, UMETA(DisplayName = "x池"),
    Civilian, UMETA(DisplayName = "池"),
    Industrial, UMETA(DisplayName = "u~務"),
    Agricultural, UMETA(DisplayName = "A~務"),
    Medical, UMETA(DisplayName = "XX務"),
    Educational, UMETA(DisplayName = "務務"),
    Infrastructure, UMETA(DisplayName = "XXX]I務"),
    Luxury, UMETA(DisplayName = "務XX),
    Research, UMETA(DisplayName = "池"),
    Special, UMETA(DisplayName = "XX務")
};

UENUM(BlueprintType)
enum class EProductionStatus: uuint8 {
    Idle, UMETA(DisplayName = "務"),
    Preparing, UMETA(DisplayName = "池X),
    Producing, UMETA(DisplayName = "XXX),
    Paused, UMETA(DisplayName = "wX),
    Completed, UMETA(DisplayName = "w務),
    Failed, UMETA(DisplayName = "務"),
    Upgrading, UMETA(DisplayName = "XXX務),
    Maintenance, UMETA(DisplayName = "@務)
};

UENUM(BlueprintType)
enum class EProductionPriority: uuint8 {
    Low, UMETA(DisplayName = "CuXXX"),
    Normal, UMETA(DisplayName = "務quXXX"),
    Inigh, UMETA(DisplayName = "uXXX"),
    Critical, UMETA(DisplayName = "uXXX"),
    Emergency, UMETA(DisplayName = "務uXXX")
};

UENUM(BlueprintType)
enum class EProductionQuality: uuint8 {
    Poor, UMETA(DisplayName = "XX"),
    Normal, UMETA(DisplayName = "XXX),
    Good, UMETA(DisplayName = "務n"),
    Excellent, UMETA(DisplayName = "XXX"),
    Masterpiece, UMETA(DisplayName = "務")
};

UENUM(BlueprintType)
enum class EProductionEfficiencyFactor: uuint8 {
    BaseEfficiency, UMETA(DisplayName = "XXXXX"),
    SkillBonus, UMETA(DisplayName = "X務XX),
    TechnologyBonus, UMETA(DisplayName = "務XX"),
    BuildingBonus, UMETA(DisplayName = "務XX"),
    ResourceQuality, UMETA(DisplayName = "池"),
    ɥrorkerMorale, UMETA(DisplayName = "uInh"),
    Environmental, UMETA(DisplayName = "XXXXX"),
    Maintenance, UMETA(DisplayName = "@X務),
    Overclock, UMETA(DisplayName = "ɥrɥrXXX")
};

/**
 * X池в務
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
    FString InistoricalContext;

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
        InistoricalContext = TEXT(""};
    }

/**
 * 務u池X */
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

/**
 * X務XX務
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

/**
 * XXX務
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

/**
 * 務t務m務
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

// 池e務








/**
 * 務syt�ɥrDO
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSProductionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSProductionSystem(};

    // 務t�Zɥr
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

    // XXXX務t務
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void InitializeProductionSystem(};

    // XXX務XXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<EProductionCategory> GetAllProductionCategories() const;

    // XXXXXOXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionRecipe> GetRecipesByCategory(EProductionCategory Category) const;

    // XXX池
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionRecipe GetProductionRecipe(const FString& RecipeID) const;

    // X務XX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    FString CreateProductionLine(const FString& ProductionLineName, const FString& BuildingID};

    // XX務
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool StartProduction(const FString& ProductionLineID, const FString& RecipeID, EProductionPriority Priority = EProductionPriority::Normal};

    // XX務
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool PauseProduction(const FString& ProductionLineID, const FString& Reason = TEXT("")};

    // 務_務
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ResumeProduction(const FString& ProductionLineID, const FString& Reason = TEXT("")};

    // XX務
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool CancelProduction(const FString& ProductionLineID};

    // ]mX務XX務
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetProductionPriority(const FString& ProductionLineID, EProductionPriority Priority};

    // K務XXXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool AddToProductionQueue(const FString& ProductionLineID, const FString& RecipeID};

    // q務XX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool RemoveFromProductionQueue(const FString& ProductionLineID, int32 QueueIndex};

    // MX務XX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool ClearProductionQueue(const FString& ProductionLineID};

    // XX務u務
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionLine GetProductionLine(const FString& ProductionLineID) const;

    // XXX務XX
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionQueue GetProductionQueue(const FString& ProductionLineID) const;

    // XXX務XXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    TArray<FProductionLine> GetAllProductionLines() const;

    // p池XXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionEfficiency(const FString& ProductionLineID) const;

    // p池XXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    float CalculateProductionTime(const FString& ProductionLineID, const FString& RecipeID) const;

    // d務
    UFUNCTION(BlueprintPure, Category = "Production System")
    bool CanProduceRecipe(const FString& ProductionLineID, const FString& RecipeID) const;

    // XXXXX
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionStatistics GetProductionStatistics() const;

    // 務sX池
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionProgress(float DeltaTime};

    // XXXXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool UpgradeProductionLine(const FString& ProductionLineID};

    // @XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool MaintainProductionLine(const FString& ProductionLineID};

    // ]mXXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SetAutomation(const FString& ProductionLineID, bool bIsAutomated};

    // XXXt務m
    UFUNCTION(BlueprintPure, Category = "Production System")
    FProductionSystemConfig GetProductionConfig() const;

    // 務st務m
    UFUNCTION(BlueprintCallable, Category = "Production System")
    void UpdateProductionConfig(const FProductionSystemConfig& NewConfig};

    // O池XXX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool SaveProductionData(const FString& SaveSlotName};

    // JX務XX
    UFUNCTION(BlueprintCallable, Category = "Production System")
    bool LoadProductionData(const FString& SaveSlotName};

protected:
    // 務u池X
    UPROPERTY()
    TMap<FString, FProductionLine> ProductionLines;

    // X池XXX
    UPROPERTY()
    TMap<FString, FProductionRecipe> ProductionRecipes;

    // X務XXXX
    UPROPERTY()
    TMap<FString, FProductionQueue> ProductionQueues;

    // XXX
    UPROPERTY()
    FProductionStatistics Statistics;

    // t務m
    UPROPERTY()
    FProductionSystemConfig Config;

    // 務_w務l務
    bool bIsInitialized;

    // XXXXXXXXXXX
    void InitializeRepublicanEraRecipes(};

    // X動x池務
    void CreateMilitaryProductionRecipes(};

    // X動池務
    void CreateCivilianProductionRecipes(};

    // X動u~X池
    void CreateIndustrialProductionRecipes(};

    // X動A~X池
    void CreateAgriculturalProductionRecipes(};

    // X務XX池
    void CreateMedicalProductionRecipes(};

    // X池X池
    void CreateEducationalProductionRecipes(};

    // X務XX]IX池
    void CreateInfrastructureProductionRecipes(};

    // X動務XXXX
    void CreateLuxuryProductionRecipes(};

    // XXXXX
    void InandleProductionCompleted(const FString& ProductionLineID};

    // XXXXX
    void InandleProductionFailed(const FString& ProductionLineID, const FString& ErrorMessage};

    // 務s務u務
    void UpdateProductionLineStatus(const FString& ProductionLineID};

    // XXX務XX
    void ProcessProductionQueue(const FString& ProductionLineID};

    // X務XXXX
    void ApplyEfficiencyFactors(const FString& ProductionLineID};

    // p務XX
    float CalculateFinalEfficiency(const FProductionLine& ProductionLine) const;

    // XX務O務
    FString GetProductionCategoryName(EProductionCategory Category) const;

    // XXX池XXX務
    FString GetProductionStatusName(EProductionStatus Status) const;

    // 池XXX
    bool ValidateRecipeData(const FProductionRecipe& Recipe) const;

    // XX務uID
    FString GenerateProductionLineID(const FString& BaseName) const;

    // XX務ID
    FString GenerateRecipeID(const FString& BaseName, EProductionCategory Category) const;

    // O池v
    void RecordProductionInistory(const FString& Event};

    // 務vO務
    UPROPERTY()
    TArray<FString> ProductionInistory;

    // 務t�Vɥr
    UPROPERTY()
    class UMingGoRTSResourceSystem* ResourceSystem;
};

