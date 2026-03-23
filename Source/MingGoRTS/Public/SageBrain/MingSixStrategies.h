#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingSixStrategies.generated.h"

// 六策類型
UENUM(BlueprintType)
enum class ESixStrategyType : uint8
{
    None            UMETA(DisplayName = "無"),
    // 正三策
    EstablishNation UMETA(DisplayName = "立國策"),
    EstablishSystem UMETA(DisplayName = "立制策"),
    EstablishPeople UMETA(DisplayName = "立人策"),
    // 逆三策
    BreakSituation  UMETA(DisplayName = "破局策"),
    BreakStructure  UMETA(DisplayName = "破結構策"),
    PreservePeople  UMETA(DisplayName = "不破人策")
};

// 策略方向
UENUM(BlueprintType)
enum class EStrategyDirection : uint8
{
    None            UMETA(DisplayName = "無"),
    Positive        UMETA(DisplayName = "正策"),
    Negative        UMETA(DisplayName = "逆策")
};

// 策略效果類型
UENUM(BlueprintType)
enum class EStrategyEffectType : uint8
{
    None            UMETA(DisplayName = "無"),
    Economic        UMETA(DisplayName = "經濟效果"),
    Military        UMETA(DisplayName = "軍事效果"),
    Diplomatic      UMETA(DisplayName = "外交效果"),
    Cultural        UMETA(DisplayName = "文化效果"),
    Technological   UMETA(DisplayName = "科技效果")
};

// 策略狀態結構
USTRUCT(BlueprintType)
struct FStrategyStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESixStrategyType StrategyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStrategyDirection Direction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PowerLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Effectiveness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ExecutionCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EStrategyEffectType> ActiveEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> StrategyModifiers;

    FStrategyStatus()
    {
        StrategyType = ESixStrategyType::None;
        Direction = EStrategyDirection::None;
        PowerLevel = 0.0f;
        Effectiveness = 0.5f;
        ExecutionCount = 0;
        SuccessRate = 0.5f;
    }
};

// 策略執行結果
USTRUCT(BlueprintType)
struct FStrategyExecutionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESixStrategyType StrategyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ImpactScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedTargets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FStrategyExecutionResult()
    {
        StrategyType = ESixStrategyType::None;
        bSuccess = false;
        ImpactScore = 0.0f;
    }
};

// 六策組合結構
USTRUCT(BlueprintType)
struct FSixStrategyCombination
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ESixStrategyType> ActiveStrategies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SynergyBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsValidCombination;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombinationName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalPower;

    FSixStrategyCombination()
    {
        SynergyBonus = 0.0f;
        bIsValidCombination = false;
        TotalPower = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyExecuted, const FStrategyExecutionResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyCombinationActivated, const FSixStrategyCombination&, Combination);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyEffectTriggered, ESixStrategyType, StrategyType, EStrategyEffectType, EffectType);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSixStrategies : public UObject
{
    GENERATED_BODY()

public:
    UMingSixStrategies();

    // 初始化六策系統
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    void InitializeSixStrategies();

    // 執行單一策略
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    FStrategyExecutionResult ExecuteStrategy(ESixStrategyType StrategyType, const TArray<FString>& Targets);

    // 檢查策略組合有效性
    UFUNCTION(BlueprintPure, Category = "Six Strategies")
    bool IsValidStrategyCombination(const TArray<ESixStrategyType>& Strategies) const;

    // 激活策略組合
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    FSixStrategyCombination ActivateStrategyCombination(const TArray<ESixStrategyType>& Strategies);

    // 獲取策略狀態
    UFUNCTION(BlueprintPure, Category = "Six Strategies")
    FStrategyStatus GetStrategyStatus(ESixStrategyType StrategyType) const;

    // 更新策略威力
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    void UpdateStrategyPower(ESixStrategyType StrategyType, float NewPower);

    // 獲取策略推薦
    UFUNCTION(BlueprintPure, Category = "Six Strategies")
    TArray<ESixStrategyType> GetRecommendedStrategies(EStrategyDirection Direction) const;

    // 計算策略協同效果
    UFUNCTION(BlueprintPure, Category = "Six Strategies")
    float CalculateSynergyEffect(const TArray<ESixStrategyType>& Strategies) const;

    // 正策相關
    UFUNCTION(BlueprintCallable, Category = "Six Strategies|Positive")
    FStrategyExecutionResult ExecuteEstablishNation(const FString& NationName);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies|Positive")
    FStrategyExecutionResult ExecuteEstablishSystem(const FString& SystemType);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies|Positive")
    FStrategyExecutionResult ExecuteEstablishPeople(const TArray<FString>& PeopleList);

    // 逆策相關
    UFUNCTION(BlueprintCallable, Category = "Six Strategies|Negative")
    FStrategyExecutionResult ExecuteBreakSituation(const FString& TargetSituation);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies|Negative")
    FStrategyExecutionResult ExecuteBreakStructure(const FString& TargetStructure);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies|Negative")
    FStrategyExecutionResult ExecutePreservePeople(const FString& TargetNation);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Six Strategies")
    FOnStrategyExecuted OnStrategyExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Six Strategies")
    FOnStrategyCombinationActivated OnStrategyCombinationActivated;

    UPROPERTY(BlueprintAssignable, Category = "Six Strategies")
    FOnStrategyEffectTriggered OnStrategyEffectTriggered;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Six Strategies")
    void OnSixStrategiesInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Six Strategies")
    void OnStrategyExecutionCompleted(const FStrategyExecutionResult& Result);

private:
    void InitializeDefaultStrategies();
    float CalculateBaseEffectiveness(ESixStrategyType StrategyType) const;
    bool CanExecuteStrategy(ESixStrategyType StrategyType) const;
    void RecordStrategyExecution(ESixStrategyType StrategyType, bool bSuccess);
    float CalculateImpactScore(ESixStrategyType StrategyType, const TArray<FString>& Targets) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Six Strategies")
    TMap<ESixStrategyType, FStrategyStatus> StrategyStatusMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Six Strategies")
    TArray<FSixStrategyCombination> ActiveCombinations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Six Strategies")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Six Strategies")
    float MaxStrategyPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Six Strategies")
    TMap<ESixStrategyType, TArray<FString>> StrategyHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Six Strategies")
    int32 TotalStrategyExecutions;
};
