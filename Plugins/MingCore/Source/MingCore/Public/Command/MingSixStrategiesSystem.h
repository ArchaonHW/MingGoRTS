#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSixStrategiesSystem.generated.h"

UENUM(BlueprintType)
enum class ESixStrategyType: uint8 {
    // 正六策：立國、立制、立人
    EstablishNation,     // 立國：伐心之正道
    EstablishSystem,     // 立制：伐勢、伐命之正道
    EstablishPeople,     // 立人：伐氣、伐影、伐無之正道
    
    // 逆六策：破局、破結構、不破人
    BreakSituation,      // 破局：非常之手段
    BreakStructure,      // 破結構：隱蔽之破壞
    PreservePeople       // 不破人：道德底線
};

UENUM(BlueprintType)
enum class ESixStrategyNature: uint8 {
    Orthodox,            // 正道：順天應人、堂堂之陣、陽剛之用
    Unconventional,      // 逆術：違時背理、隱隱之行、陰柔之變
    Hybrid              // 混合：正中用逆，逆中藏正
};

UENUM(BlueprintType)
enum class ESixStrategyTarget: uint8 {
    Heart,              // 伐心：奪其志
    Energy,             // 伐氣：挫其銳
    Shadow,             // 伐影：亂其象
    Life,               // 伐命：斷其根
    Power,              // 伐勢：逆其時
    Nothing             // 伐無：藏其跡
};

USTRUCT(BlueprintType)
struct FSixStrategyProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESixStrategyType StrategyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESixStrategyNature StrategyNature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESixStrategyTarget TargetType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StrategyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeRequired;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReversible;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresPublicSupport;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    FSixStrategyProfile()
    {
        StrategyType = ESixStrategyType::EstablishNation;
        StrategyNature = ESixStrategyNature::Orthodox;
        TargetType = ESixStrategyTarget::Heart;
        StrategyName = TEXT("");
        Description = TEXT("");
        ExecutionCost = 0.0f;
        SuccessProbability = 50.0f;
        MoralCost = 0.0f;
        TimeRequired = 0.0f;
        bIsReversible = true;
        bRequiresPublicSupport = false;
    }
};

USTRUCT(BlueprintType)
struct FSixStrategyExecution
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSixStrategyProfile Profile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsExecuting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ElapsedTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> CurrentEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SideEffects;

    FSixStrategyExecution()
    {
        Profile = FSixStrategyProfile();
        ExecutionProgress = 0.0f;
        bIsExecuting = false;
        bIsCompleted = false;
        StartTime = 0.0f;
        ElapsedTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSixStrategyCombination
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ESixStrategyType> StrategySequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CombinationName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallSuccessProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallMoralCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsValidCombination;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SynergyEffects;

    FSixStrategyCombination()
    {
        CombinationName = TEXT("");
        Description = TEXT("");
        OverallSuccessProbability = 0.0f;
        OverallMoralCost = 0.0f;
        bIsValidCombination = true;
    }
};

/**
 * 六策系統
 * 
 * 核心理念：
 * 1. 正六策：立國、立制、立人
 * 2. 逆六策：破局、破結構、不破人
 * 3. 正逆並用，六伐皆兵
 * 4. 視局勢之需，變化無方
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingSixStrategiesSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSixStrategiesSystem();

    // 初始化六策系統
    UFUNCTION(BlueprintCallable)
    void InitializeSixStrategiesSystem();

    // 獲取策略檔案
    UFUNCTION(BlueprintCallable)
    FSixStrategyProfile GetStrategyProfile(ESixStrategyType StrategyType);

    // 執行策略
    UFUNCTION(BlueprintCallable)
    bool ExecuteStrategy(ESixStrategyType StrategyType);

    // 檢查策略可用性
    UFUNCTION(BlueprintCallable)
    bool IsStrategyAvailable(ESixStrategyType StrategyType);

    // 計算策略成功率
    UFUNCTION(BlueprintCallable)
    float CalculateStrategySuccess(ESixStrategyType StrategyType);

    // 計算策略道德成本
    UFUNCTION(BlueprintCallable)
    float CalculateStrategyMoralCost(ESixStrategyType StrategyType);

    // 獲取推薦策略
    UFUNCTION(BlueprintCallable)
    TArray<ESixStrategyType> GetRecommendedStrategies();

    // 檢查策略組合
    UFUNCTION(BlueprintCallable)
    FSixStrategyCombination AnalyzeStrategyCombination(const TArray<ESixStrategyType>& Strategies);

    // 執行策略組合
    UFUNCTION(BlueprintCallable)
    bool ExecuteStrategyCombination(const TArray<ESixStrategyType>& Strategies);

    // 檢查正逆兼容性
    UFUNCTION(BlueprintCallable)
    bool AreStrategiesCompatible(ESixStrategyType Strategy1, ESixStrategyType Strategy2);

    // 獲取當前執行狀態
    UFUNCTION(BlueprintCallable)
    TArray<FSixStrategyExecution> GetCurrentExecutions();

    // 停止策略執行
    UFUNCTION(BlueprintCallable)
    bool StopStrategyExecution(ESixStrategyType StrategyType);

    // 獲取策略名稱
    UFUNCTION(BlueprintCallable)
    FString GetStrategyName(ESixStrategyType StrategyType);

    // 獲取策略描述
    UFUNCTION(BlueprintCallable)
    FString GetStrategyDescription(ESixStrategyType StrategyType);

protected:
    // 策略檔案庫
    UPROPERTY(BlueprintReadOnly)
    TMap<ESixStrategyType, FSixStrategyProfile> StrategyProfiles;

    // 當前執行中的策略
    UPROPERTY(BlueprintReadOnly)
    TArray<FSixStrategyExecution> CurrentExecutions;

    // 策略執行歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FSixStrategyExecution> ExecutionHistory;

    // 策略組合庫
    UPROPERTY(BlueprintReadOnly)
    TArray<FSixStrategyCombination> StrategyCombinations;

    // 執行閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionThreshold;

    // 道德成本閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralCostThreshold;

    // 成功率閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRateThreshold;

private:
    // 內部輔助函數
    void InitializeStrategyProfiles();
    void InitializeStrategyCombinations();
    bool CanExecuteStrategy(const FSixStrategyProfile& Profile);
    void UpdateExecutionProgress();
    void RecordStrategyExecution(const FSixStrategyExecution& Execution);
    float CalculateSynergyBonus(const TArray<ESixStrategyType>& Strategies);
};
