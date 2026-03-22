#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGameBalanceManager.generated.h"

UENUM(BlueprintType)
enum class EMingBalanceCategory: uint8 {
    UnitStats UMETA(DisplayName = "Unit Stats"),
    ResourceEconomy UMETA(DisplayName = "Resource Economy"),
    BuildingCosts UMETA(DisplayName = "Building Costs"),
    Technology UMETA(DisplayName = "Technology"),
    Combat UMETA(DisplayName = "Combat"),
    Difficulty UMETA(DisplayName = "Difficulty"),
    AIBehavior UMETA(DisplayName = "AI Behavior"),
    CampaignProgression UMETA(DisplayName = "Campaign Progression")
};

USTRUCT(BlueprintType)
struct FMingUnitBalance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BuildTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Range;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Armor;

    FMingUnitBalance()
        : Health(100.0f)
        , Damage(10.0f)
        , AttackSpeed(1.0f)
        , MovementSpeed(1.0f)
        , Cost(100.0f)
        , BuildTime(5.0f)
        , Range(100.0f)
        , Armor(5.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingResourceBalance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartingResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IncomeRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceCap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GatheringSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceDepletionRate;

    FMingResourceBalance()
        : StartingResources(1000.0f)
        , IncomeRate(10.0f)
        , ResourceCap(10000.0f)
        , GatheringSpeed(1.0f)
        , ResourceDepletionRate(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingDifficultyBalance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AIResourceMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AIUnitHealthMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AIUnitDamageMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayerResourceMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayerUnitHealthMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayerUnitDamageMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AIReactionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AIAggressiveness;

    FMingDifficultyBalance()
        : AIResourceMultiplier(1.0f)
        , AIUnitHealthMultiplier(1.0f)
        , AIUnitDamageMultiplier(1.0f)
        , PlayerResourceMultiplier(1.0f)
        , PlayerUnitHealthMultiplier(1.0f)
        , PlayerUnitDamageMultiplier(1.0f)
        , AIReactionTime(1.0f)
        , AIAggressiveness(0.5f)
    {}
};

/**
 * Game Balance Manager for MingGoRTS
 * Manages and adjusts game balance across all systems
 */
UCLASS(ClassGroup = (Balance), Blueprintable)
class MINGBALANCE_API UMingGameBalanceManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGameBalanceManager();

    // Balance Management
    UFUNCTION(BlueprintCallable, Category = "Balance Management")
    void InitializeBalanceManager();

    UFUNCTION(BlueprintCallable, Category = "Balance Management")
    void ApplyAllBalanceSettings();

    UFUNCTION(BlueprintCallable, Category = "Balance Management")
    void ResetToDefaultBalance();

    UFUNCTION(BlueprintCallable, Category = "Balance Management")
    void SaveCurrentBalance(const FString& ProfileName) {};

    UFUNCTION(BlueprintCallable, Category = "Balance Management")
    void LoadBalanceProfile(const FString& ProfileName) {};

    // Unit Balance
    UFUNCTION(BlueprintCallable, Category = "Unit Balance")
    void SetUnitBalance(const FString& UnitType, const FMingUnitBalance& Balance) {};

    UFUNCTION(BlueprintPure, Category = "Unit Balance")
    FMingUnitBalance GetUnitBalance(const FString& UnitType) const;

    UFUNCTION(BlueprintCallable, Category = "Unit Balance")
    void AdjustUnitStat(const FString& UnitType, const FString& StatName, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Unit Balance")
    void ScaleAllUnits(float HealthScale, float DamageScale, float CostScale);

    // Resource Balance
    UFUNCTION(BlueprintCallable, Category = "Resource Balance")
    void SetResourceBalance(const FMingResourceBalance& Balance) {};

    UFUNCTION(BlueprintPure, Category = "Resource Balance")
    FMingResourceBalance GetResourceBalance() const;

    UFUNCTION(BlueprintCallable, Category = "Resource Balance")
    void AdjustResourceIncome(float Multiplier);

    UFUNCTION(BlueprintCallable, Category = "Resource Balance")
    void AdjustGatheringSpeed(float Multiplier);

    // Difficulty Balance
    UFUNCTION(BlueprintCallable, Category = "Difficulty Balance")
    void SetDifficultyBalance(const FString& DifficultyLevel, const FMingDifficultyBalance& Balance) {};

    UFUNCTION(BlueprintPure, Category = "Difficulty Balance")
    FMingDifficultyBalance GetDifficultyBalance(const FString& DifficultyLevel) const;

    UFUNCTION(BlueprintCallable, Category = "Difficulty Balance")
    void ApplyDifficultyLevel(const FString& DifficultyLevel) {};

    UFUNCTION(BlueprintCallable, Category = "Difficulty Balance")
    void AdjustAIDifficulty(float ResourceMultiplier, float HealthMultiplier, float DamageMultiplier);

    // Balance Analysis
    UFUNCTION(BlueprintCallable, Category = "Balance Analysis")
    void AnalyzeGameBalance();

    UFUNCTION(BlueprintPure, Category = "Balance Analysis")
    bool IsGameBalanced() const;

    UFUNCTION(BlueprintCallable, Category = "Balance Analysis")
    void GenerateBalanceReport();

    UFUNCTION(BlueprintCallable, Category = "Balance Analysis")
    void IdentifyBalanceIssues();

    // Validation
    UFUNCTION(BlueprintCallable, Category = "Validation")
    void ValidateBalanceSettings();

    UFUNCTION(BlueprintCallable, Category = "Validation")
    void CheckForExploits();

    UFUNCTION(BlueprintCallable, Category = "Validation")
    void VerifyUnitCounters();

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetBalanceCategoryName(EMingBalanceCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Utility")
    FString ExportBalanceData() const;

protected:
    UPROPERTY()
    TMap<FString, FMingUnitBalance> UnitBalances;

    UPROPERTY()
    FMingResourceBalance ResourceBalance;

    UPROPERTY()
    TMap<FString, FMingDifficultyBalance> DifficultyBalances;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeDefaultBalances();
    void SetupUnitBalances();
    void SetupResourceBalances();
    void SetupDifficultyBalances();

    // Balance Validation
    bool ValidateUnitBalance(const FMingUnitBalance& Balance) const;
    bool ValidateResourceBalance(const FMingResourceBalance& Balance) const;
    bool ValidateDifficultyBalance(const FMingDifficultyBalance& Balance) const;

    // Analysis Functions
    void AnalyzeUnitBalance();
    void AnalyzeResourceBalance();
    void AnalyzeCombatBalance();
    void AnalyzeEconomicBalance();
    void AnalyzeDifficultyCurve();

    // Adjustment Functions
    void AutoAdjustBalance();
    void SuggestBalanceChanges();
    void ApplyBalanceCorrections();
};

