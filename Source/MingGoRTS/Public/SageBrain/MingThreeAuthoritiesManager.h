#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingThreeAuthoritiesManager.generated.h"

UENUM(BlueprintType)
enum class EDaoAuthorityDecisionType : uint8
{
    Allow,          // 允許執行
    Warn,           // 發出警告
    Suspend,        // 暫停行動
    StopAndSeal,    // 停止並封存
    TransferPower   // 權力交接
};

USTRUCT(BlueprintType)
struct FDaoAuthorityDecision
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EDaoAuthorityDecisionType Decision;
    
    UPROPERTY(BlueprintReadOnly)
    FString Reasoning;
    
    UPROPERTY(BlueprintReadOnly)
    FString RecommendedAction;
    
    UPROPERTY(BlueprintReadOnly)
    float MoralRiskAssessment;
    
    FDaoAuthorityDecision()
        : Decision(EDaoAuthorityDecisionType::Allow)
        , MoralRiskAssessment(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FSituationAssessment
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    float UrgencyLevel;         // 緊急程度
    
    UPROPERTY(BlueprintReadWrite)
    float ChaosLevel;           // 混亂程度
    
    UPROPERTY(BlueprintReadWrite)
    float EnemyStrength;        // 敵方實力
    
    UPROPERTY(BlueprintReadWrite)
    float MoralSupport;         // 道德支持度
    
    UPROPERTY(BlueprintReadWrite)
    float ResourceAvailability; // 資源可用性
    
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> FactorScores;
    
    FSituationAssessment()
        : UrgencyLevel(0.5f)
        , ChaosLevel(0.5f)
        , EnemyStrength(0.5f)
        , MoralSupport(0.5f)
        , ResourceAvailability(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FStrategySwitchDecision
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bShouldSwitch;
    
    UPROPERTY(BlueprintReadOnly)
    bool bToRighteous;  // true=轉向正道, false=轉向逆道
    
    UPROPERTY(BlueprintReadOnly)
    FString Reasoning;
    
    UPROPERTY(BlueprintReadOnly)
    float SwitchUrgency;  // 切換緊迫性
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Preconditions;
    
    FStrategySwitchDecision()
        : bShouldSwitch(false)
        , bToRighteous(true)
        , SwitchUrgency(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FExecutionOrder
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString OrderID;
    
    UPROPERTY(BlueprintReadWrite)
    FString OrderType;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Targets;
    
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, FString> Parameters;
    
    UPROPERTY(BlueprintReadWrite)
    float Priority;
    
    UPROPERTY(BlueprintReadWrite)
    bool bRequiresConfirmation;
    
    FExecutionOrder()
        : Priority(1.0f)
        , bRequiresConfirmation(false)
    {}
};

/**
 * 三權管理器
 * 實現道權、策權、兵權的分工與協作
 */
UCLASS()
class MINGGORTS_API UMingThreeAuthoritiesManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingThreeAuthoritiesManager();
    
    /** 初始化三權 */
    UFUNCTION(BlueprintCallable)
    bool Initialize();
    
    // ========== 道權 (防墮監督) ==========
    
    /** 道權檢查：評估行動是否符合天理大道 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Dao")
    FDaoAuthorityResult EvaluateAction(const FCommandAction& Action);
    
    /** 道權決策：決定是否允許執行 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Dao")
    FDaoAuthorityDecision MakeDaoDecision(const FCommandSituation& Situation);
    
    /** 檢查是否觸及墮落紅線 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Dao")
    bool CheckMoralRedLine(const FActionHistory& History) const;
    
    /** 評估長期道德影響 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Dao")
    float AssessLongTermMoralImpact(const FStrategyPlan& Plan) const;
    
    // ========== 策權 (正逆切換) ==========
    
    /** 策權決策：決定正逆切換 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Strategy")
    FStrategySwitchDecision DecideStrategySwitch(
        EFiveElementPhase CurrentPhase,
        const FSituationAssessment& Assessment
    );
    
    /** 評估當前策略適用性 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Strategy")
    float EvaluateCurrentStrategyFitness() const;
    
    /** 推薦下一階段策略 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Strategy")
    ETwelveStrategies RecommendNextStrategy(
        const FStrategyContext& Context,
        const FSituationAssessment& Assessment
    ) const;
    
    /** 檢查策略切換時機 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Strategy")
    bool IsSwitchTimingAppropriate() const;
    
    // ========== 兵權 (執行載體) ==========
    
    /** 兵權執行：執行命令 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Military")
    bool ExecuteOrder(const FExecutionOrder& Order);
    
    /** 評估執行能力 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Military")
    float AssessExecutionCapability(const FExecutionOrder& Order) const;
    
    /** 獲取執行狀態 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Military")
    FExecutionStatus GetExecutionStatus(const FString& OrderID) const;
    
    /** 取消執行 */
    UFUNCTION(BlueprintCallable, Category = "ThreeAuthorities|Military")
    bool CancelExecution(const FString& OrderID);
    
    // ========== 三權協調 ==========
    
    /** 檢查三權是否協調一致 */
    UFUNCTION(BlueprintCallable)
    bool AreAuthoritiesAligned() const;
    
    /** 解決三權衝突 */
    UFUNCTION(BlueprintCallable)
    bool ResolveAuthorityConflict();
    
    /** 獲取三權狀態報告 */
    UFUNCTION(BlueprintCallable)
    FThreeAuthoritiesReport GetAuthoritiesReport() const;
    
private:
    UPROPERTY()
    float DaoAuthorityHealth;
    
    UPROPERTY()
    float StrategyAuthorityHealth;
    
    UPROPERTY()
    float MilitaryAuthorityHealth;
    
    UPROPERTY()
    TArray<FExecutionOrder> PendingOrders;
    
    UPROPERTY()
    TArray<FExecutionOrder> ExecutingOrders;
    
    UPROPERTY()
    TMap<FString, FExecutionStatus> ExecutionHistory;
};
