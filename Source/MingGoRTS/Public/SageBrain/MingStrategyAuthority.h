#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingStrategyAuthority.generated.h"

UENUM(BlueprintType)
enum class EStrategyType : uint8
{
    None            UMETA(DisplayName = "無"),
    Offensive       UMETA(DisplayName = "攻勢"),
    Defensive       UMETA(DisplayName = "守勢"),
    Diplomatic      UMETA(DisplayName = "外交"),
    Economic        UMETA(DisplayName = "經濟"),
    Intelligence    UMETA(DisplayName = "情報")
};

UENUM(BlueprintType)
enum class ETacticComplexity : uint8
{
    Simple          UMETA(DisplayName = "簡單"),
    Moderate        UMETA(DisplayName = "中等"),
    Complex         UMETA(DisplayName = "複雜"),
    Master          UMETA(DisplayName = "大師")
};

USTRUCT(BlueprintType)
struct FStrategyStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrategicThinking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlanningAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Adaptability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Foresight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceManagement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ActiveStrategies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EStrategyType> MasteredStrategies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> StrategyModifiers;

    FStrategyStatus()
    {
        StrategicThinking = 50.0f;
        PlanningAbility = 50.0f;
        Adaptability = 50.0f;
        Foresight = 50.0f;
        ResourceManagement = 50.0f;
        ActiveStrategies = 0;
    }
};

USTRUCT(BlueprintType)
struct FTacticPlan
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TacticName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStrategyType StrategyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETacticComplexity Complexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ExpectedOutcomes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsExecuting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionProgress;

    FTacticPlan()
    {
        TacticName = "";
        StrategyType = EStrategyType::None;
        Complexity = ETacticComplexity::Simple;
        SuccessChance = 0.5f;
        ExecutionTime = 0.0f;
        ResourceCost = 0.0f;
        bIsExecuting = false;
        ExecutionProgress = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyExecuted, const FString&, StrategyName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTacticCompleted, const FString&, TacticName, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategicLevelUp, int32, NewLevel);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingStrategyAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategyAuthority();

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void InitializeStrategyAuthority();

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void SetStrategicThinking(float ThinkingLevel);

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    FStrategyStatus GetStrategyStatus() const { return CurrentStrategyStatus; }

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    bool ExecuteStrategy(EStrategyType StrategyType, const FString& StrategyName);

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    bool PlanTactic(const FTacticPlan& Tactic);

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void UpdateTacticExecution(const FString& TacticName, float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    bool CanExecuteStrategy(EStrategyType StrategyType, float RequiredPower) const;

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    TArray<FString> GetAvailableStrategies(EStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    bool CancelTactic(const FString& TacticName);

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    TArray<FTacticPlan> GetActiveTactics() const;

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void UpgradeStrategySkill(EStrategyType StrategyType, float Amount);

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    float CalculateStrategySuccessChance(const FTacticPlan& Tactic) const;

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void ApplyStrategyModifier(const FString& ModifierName, float ModifierValue);

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void RemoveStrategyModifier(const FString& ModifierName);

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    bool HasStrategyType(EStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void MasterStrategyType(EStrategyType StrategyType);

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    ETacticComplexity GetMaxComplexityLevel() const;

    UFUNCTION(BlueprintPure, Category = "Strategy Authority")
    FString GetStrategyRecommendation() const;

    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority")
    FOnStrategyExecuted OnStrategyExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority")
    FOnTacticCompleted OnTacticCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority")
    FOnStrategicLevelUp OnStrategicLevelUp;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Strategy Authority")
    void OnStrategyAuthorityInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Strategy Authority")
    void OnStrategyStatusChanged(const FStrategyStatus& NewStatus);

    UFUNCTION(BlueprintImplementableEvent, Category = "Strategy Authority")
    void OnTacticStarted(const FString& TacticName);

    UFUNCTION(BlueprintImplementableEvent, Category = "Strategy Authority")
    void OnStrategyTypeMastered(EStrategyType StrategyType);

private:
    void InitializeDefaultStrategies();
    void UpdateStrategyStatus();
    bool ValidateTacticPlan(const FTacticPlan& Tactic) const;
    void CompleteTactic(const FString& TacticName, bool bSuccess);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    FStrategyStatus CurrentStrategyStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    TArray<FTacticPlan> ActiveTactics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    float MaxStrategicThinking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    float StrategyRestoreRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    int32 MaxActiveStrategies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    TMap<EStrategyType, TArray<FString>> StrategyTypeMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    TMap<FString, FTacticPlan> TacticLibrary;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    TArray<FString> ExecutedStrategies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy Authority")
    TMap<EStrategyType, int32> StrategyExperience;
};
