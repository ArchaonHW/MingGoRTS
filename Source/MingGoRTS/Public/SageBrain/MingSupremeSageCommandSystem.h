#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.generated.h"

// 前向聲明
class UMingThreeAuthoritiesManager;
class UMingFiveElementsRotation;
class UMingCorruptionPrevention;
class UMingSixConquestExecutor;
class UMingTwelveStrategiesExecutor;

// ============================================================================
// 枚舉定義
// ============================================================================

/** 三權類型：道權、策權、兵權 */
UENUM(BlueprintType)
enum class ESupremeAuthorityType : uint8
{
    DaoAuthority        UMETA(DisplayName = "道權 - 防墮監督"),
    StrategyAuthority   UMETA(DisplayName = "策權 - 正逆切換"),
    MilitaryAuthority   UMETA(DisplayName = "兵權 - 執行載體"),
    None                UMETA(DisplayName = "無")
};

/** 五行階段：木火土金水 */
UENUM(BlueprintType)
enum class EFiveElementPhase : uint8
{
    Wood    UMETA(DisplayName = "木 - 立名正當性"),
    Fire    UMETA(DisplayName = "火 - 造勢加速"),
    Earth   UMETA(DisplayName = "土 - 收權止亂"),
    Metal   UMETA(DisplayName = "金 - 裁斷清除"),
    Water   UMETA(DisplayName = "水 - 留白不動"),
    None    UMETA(DisplayName = "無")
};

/** 正邪兵種類型 */
UENUM(BlueprintType)
enum class EForceType : uint8
{
    Righteous   UMETA(DisplayName = "正兵 - 公開可承"),
    Evil        UMETA(DisplayName = "邪兵 - 隱蔽短期"),
    Hybrid      UMETA(DisplayName = "混合 - 正邪並用")
};

/** 策略方法：正道或逆道 */
UENUM(BlueprintType)
enum class EStrategyApproach : uint8
{
    Righteous   UMETA(DisplayName = "正道"),
    Reverse     UMETA(DisplayName = "逆道"),
    Hybrid      UMETA(DisplayName = "混合")
};

/** 六伐策略：伐心、伐氣、伐勢、伐命、伐影、伐無 */
UENUM(BlueprintType)
enum class ESixConquestStrategy : uint8
{
    ConquerMind     UMETA(DisplayName = "伐心 - 奪其志"),
    ConquerSpirit   UMETA(DisplayName = "伐氣 - 挫其銳"),
    ConquerMomentum UMETA(DisplayName = "伐勢 - 逆其時"),
    ConquerLife     UMETA(DisplayName = "伐命 - 斷其根"),
    ConquerShadow   UMETA(DisplayName = "伐影 - 亂其象"),
    ConquerNothing  UMETA(DisplayName = "伐無 - 藏其跡")
};

/** 正逆十二策 */
UENUM(BlueprintType)
enum class ETwelveStrategies : uint8
{
    // 正六策
    EstablishNation     UMETA(DisplayName = "立國 - 伐心正道"),
    EstablishSystem     UMETA(DisplayName = "立制 - 伐勢伐命正道"),
    EstablishPeople     UMETA(DisplayName = "立人 - 伐氣伐影伐無正道"),
    
    // 逆六策
    BreakSituation      UMETA(DisplayName = "破局 - 伐勢伐氣逆道"),
    BreakStructure      UMETA(DisplayName = "破結構 - 伐命伐影逆道"),
    BreakNotPerson      UMETA(DisplayName = "不破人 - 伐心伐無逆道")
};

/** 墮落徵象類型 */
UENUM(BlueprintType)
enum class ECorruptionSign : uint8
{
    RationalizationOfEvil   UMETA(DisplayName = "將非常手段合理化"),
    FollowerDeification     UMETA(DisplayName = "將追隨者神聖化"),
    OppositionDemonization UMETA(DisplayName = "將反對者妖魔化"),
    IAmJusticeSyndrome     UMETA(DisplayName = "我即正義徵象")
};

/** 警告級別 */
UENUM(BlueprintType)
enum class EWarningLevel : uint8
{
    Notice      UMETA(DisplayName = "注意"),
    Warning     UMETA(DisplayName = "警告"),
    Critical    UMETA(DisplayName = "嚴重"),
    Terminal    UMETA(DisplayName = "終止")
};

/** 指揮狀態 */
UENUM(BlueprintType)
enum class ECommandStatus : uint8
{
    Active,
    Suspended,
    Transferring,
    Terminated
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 階段上下文 */
USTRUCT(BlueprintType)
struct FPhaseContext
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float DurationEstimate;
    
    UPROPERTY(BlueprintReadWrite)
    EForceType PrimaryForce;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Objectives;
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime StartTime;
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime ExpectedEndTime;
    
    FPhaseContext()
        : DurationEstimate(0.0f)
        , PrimaryForce(EForceType::Righteous)
    {}
};

/** 三權狀態 */
USTRUCT(BlueprintType)
struct FAuthorityStatus
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESupremeAuthorityType AuthorityType;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    UPROPERTY(BlueprintReadOnly)
    float HealthLevel;  // 0-1
    
    UPROPERTY(BlueprintReadOnly)
    float CorruptionRisk;  // 0-1
    
    UPROPERTY(BlueprintReadOnly)
    FString LastAction;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime LastCheckTime;
    
    FAuthorityStatus()
        : AuthorityType(ESupremeAuthorityType::None)
        , bIsActive(false)
        , HealthLevel(1.0f)
        , CorruptionRisk(0.0f)
    {}
};

/** 部署配置 */
USTRUCT(BlueprintType)
struct FDeploymentConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString DeploymentName;
    
    UPROPERTY(BlueprintReadWrite)
    float ResourceAllocation;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsCovert;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TargetIDs;
    
    FDeploymentConfig()
        : ResourceAllocation(0.0f)
        , Duration(0.0f)
        , bIsCovert(false)
    {}
};

/** 征服目標 */
USTRUCT(BlueprintType)
struct FConquestTarget
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString TargetID;
    
    UPROPERTY(BlueprintReadWrite)
    FString TargetName;
    
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> WeaknessMap;
    
    UPROPERTY(BlueprintReadWrite)
    float ResistanceLevel;
    
    UPROPERTY(BlueprintReadWrite)
    float StrategicValue;
    
    FConquestTarget()
        : ResistanceLevel(0.5f)
        , StrategicValue(0.5f)
    {}
};

/** 征服結果 */
USTRUCT(BlueprintType)
struct FConquestResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    
    UPROPERTY(BlueprintReadOnly)
    float Effectiveness;
    
    UPROPERTY(BlueprintReadOnly)
    float MoralCost;
    
    UPROPERTY(BlueprintReadOnly)
    float ReputationImpact;
    
    UPROPERTY(BlueprintReadOnly)
    FString OutcomeDescription;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SideEffects;
    
    FConquestResult()
        : bSuccess(false)
        , Effectiveness(0.0f)
        , MoralCost(0.0f)
        , ReputationImpact(0.0f)
    {}
};

/** 策略上下文 */
USTRUCT(BlueprintType)
struct FStrategyContext
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString StrategyName;
    
    UPROPERTY(BlueprintReadWrite)
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> SituationFactors;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> AvailableResources;
    
    UPROPERTY(BlueprintReadWrite)
    float UrgencyLevel;
    
    FStrategyContext()
        : CurrentPhase(EFiveElementPhase::None)
        , UrgencyLevel(0.5f)
    {}
};

/** 策略結果 */
USTRUCT(BlueprintType)
struct FStrategyResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    
    UPROPERTY(BlueprintReadOnly)
    ETwelveStrategies ExecutedStrategy;
    
    UPROPERTY(BlueprintReadOnly)
    float StrategicGain;
    
    UPROPERTY(BlueprintReadOnly)
    float RiskLevel;
    
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;
    
    FStrategyResult()
        : bSuccess(false)
        , ExecutedStrategy(ETwelveStrategies::EstablishNation)
        , StrategicGain(0.0f)
        , RiskLevel(0.0f)
    {}
};

/** 墮落檢測結果 */
USTRUCT(BlueprintType)
struct FCorruptionCheckResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bRationalizationOfEvil;
    
    UPROPERTY(BlueprintReadOnly)
    bool bFollowerDeification;
    
    UPROPERTY(BlueprintReadOnly)
    bool bOppositionDemonization;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIAmJusticeSyndrome;
    
    UPROPERTY(BlueprintReadOnly)
    float OverallCorruptionRisk;
    
    UPROPERTY(BlueprintReadOnly)
    FString WarningMessage;
    
    UPROPERTY(BlueprintReadOnly)
    bool bShouldTransferCommand;
    
    FCorruptionCheckResult()
        : bRationalizationOfEvil(false)
        , bFollowerDeification(false)
        , bOppositionDemonization(false)
        , bIAmJusticeSyndrome(false)
        , OverallCorruptionRisk(0.0f)
        , bShouldTransferCommand(false)
    {}
};

/** 警告內容 */
USTRUCT(BlueprintType)
struct FWarningContent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    EWarningLevel Level;
    
    UPROPERTY(BlueprintReadWrite)
    FString Title;
    
    UPROPERTY(BlueprintReadWrite)
    FString Message;
    
    UPROPERTY(BlueprintReadWrite)
    FString RecommendedAction;
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime IssueTime;
    
    FWarningContent()
        : Level(EWarningLevel::Notice)
    {}
};

/** 自我審核報告 */
USTRUCT(BlueprintType)
struct FSelfAuditReport
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime AuditTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 TotalCommandsIssued;
    
    UPROPERTY(BlueprintReadOnly)
    int32 EvilForceUsages;
    
    UPROPERTY(BlueprintReadOnly)
    int32 PhaseTransitions;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageDecisionQuality;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CriticalDecisions;
    
    UPROPERTY(BlueprintReadOnly)
    FCorruptionCheckResult CorruptionStatus;
    
    UPROPERTY(BlueprintReadOnly)
    FString Recommendations;
    
    FSelfAuditReport()
        : TotalCommandsIssued(0)
        , EvilForceUsages(0)
        , PhaseTransitions(0)
        , AverageDecisionQuality(0.0f)
    {}
};

/** 指揮決策建議 */
USTRUCT(BlueprintType)
struct FCommandRecommendation
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESupremeAuthorityType RecommendedAuthority;
    
    UPROPERTY(BlueprintReadOnly)
    EFiveElementPhase RecommendedPhase;
    
    UPROPERTY(BlueprintReadOnly)
    EForceType RecommendedForceType;
    
    UPROPERTY(BlueprintReadOnly)
    FString RecommendationDescription;
    
    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Reasoning;
    
    FCommandRecommendation()
        : RecommendedAuthority(ESupremeAuthorityType::None)
        , RecommendedPhase(EFiveElementPhase::None)
        , RecommendedForceType(EForceType::Righteous)
        , ConfidenceLevel(0.0f)
    {}
};

// ============================================================================
// 事件委派定義
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, EFiveElementPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAuthoritySwitched, ESupremeAuthorityType, From, ESupremeAuthorityType, To);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCorruptionSignDetected, ECorruptionSign, Sign);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWarningIssued, FWarningContent, Warning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyExecuted, FStrategyResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCommandTransferRequired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelfAuditCompleted, FSelfAuditReport, Report);

// ============================================================================
// 至聖者指揮系統核心類
// ============================================================================

/**
 * 至聖者指揮系統核心類
 * 實現《至聖者指揮學》的核心概念：
 * - 三權模型（道權、策權、兵權）
 * - 五行正邪輪轉法（木火土金水）
 * - 六伐策略與正逆十二策
 * - 防墮機制與自我審核
 */
UCLASS(Blueprintable, BlueprintType)
class MINGGORTS_API UMingSupremeSageCommandSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingSupremeSageCommandSystem();
    
    // ========== 系統生命周期 ==========
    
    /** 初始化至聖者指揮系統 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|Lifecycle")
    bool InitializeSystem();
    
    /** 關閉系統 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|Lifecycle")
    void ShutdownSystem();
    
    /** 獲取系統狀態 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|Lifecycle")
    ECommandStatus GetSystemStatus() const { return SystemStatus; }
    
    // ========== 三權模型管理 ==========
    
    /** 初始化三權體系 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|ThreeAuthorities")
    bool InitializeThreeAuthorities();
    
    /** 獲取指定權限的狀態 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|ThreeAuthorities")
    FAuthorityStatus GetAuthorityStatus(ESupremeAuthorityType AuthorityType) const;
    
    /** 切換到指定權限主導 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|ThreeAuthorities")
    bool SwitchToAuthority(ESupremeAuthorityType AuthorityType);
    
    /** 獲取當前主導權限 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|ThreeAuthorities")
    ESupremeAuthorityType GetCurrentAuthority() const { return CurrentAuthority; }
    
    /** 更新權限健康度 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|ThreeAuthorities")
    bool UpdateAuthorityHealth(ESupremeAuthorityType AuthorityType, float NewHealth);
    
    // ========== 五行輪轉管理 ==========
    
    /** 進入指定五行階段 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|FiveElements")
    bool EnterPhase(EFiveElementPhase Phase, const FPhaseContext& Context);
    
    /** 轉換到下一階段 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|FiveElements")
    bool TransitionToNextPhase();
    
    /** 獲取當前階段 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|FiveElements")
    EFiveElementPhase GetCurrentPhase() const { return CurrentPhase; }
    
    /** 在當前階段部署兵種 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|FiveElements")
    bool DeployForce(EForceType ForceType, const FDeploymentConfig& Config);
    
    /** 檢查是否適合階段轉換 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|FiveElements")
    bool CanTransitionToPhase(EFiveElementPhase TargetPhase) const;
    
    /** 獲取當前階段推薦的兵種 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|FiveElements")
    TArray<EForceType> GetRecommendedForcesForPhase(EFiveElementPhase Phase) const;
    
    // ========== 六伐策略 ==========
    
    /** 執行六伐策略 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|SixConquests")
    FConquestResult ExecuteSixConquest(
        ESixConquestStrategy Strategy,
        EStrategyApproach Approach,
        const FConquestTarget& Target
    );
    
    /** 獲取當前情境推薦的六伐策略 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|SixConquests")
    TArray<ESixConquestStrategy> GetRecommendedConquestStrategies() const;
    
    // ========== 正逆十二策 ==========
    
    /** 執行正逆十二策 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|TwelveStrategies")
    FStrategyResult ExecuteTwelveStrategy(
        ETwelveStrategies Strategy,
        const FStrategyContext& Context
    );
    
    /** 獲取當前情境推薦的策略 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|TwelveStrategies")
    FCommandRecommendation GetRecommendedStrategy() const;
    
    /** 評估策略適用性 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|TwelveStrategies")
    float EvaluateStrategyFitness(ETwelveStrategies Strategy, const FStrategyContext& Context) const;
    
    // ========== 防墮機制 ==========
    
    /** 執行墮落檢查 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|CorruptionPrevention")
    FCorruptionCheckResult PerformCorruptionCheck();
    
    /** 檢查特定墮落徵象 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|CorruptionPrevention")
    bool IsCorruptionSignPresent(ECorruptionSign Sign) const;
    
    /** 發布戒令 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|CorruptionPrevention")
    bool IssueWarningCommand(EWarningLevel Level, const FWarningContent& Content);
    
    /** 評估是否需要權力交接 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|CorruptionPrevention")
    bool ShouldTransferCommand() const;
    
    /** 執行權力交接 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|CorruptionPrevention")
    bool ExecuteCommandTransfer();
    
    // ========== 自我審核 ==========
    
    /** 生成自我審核報告 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|SelfAudit")
    FSelfAuditReport GenerateSelfAuditReport();
    
    /** 記錄關鍵決策 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|SelfAudit")
    bool RecordCriticalDecision(const FString& DecisionDescription, float QualityScore);
    
    /** 獲取歷史決策分析 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|SelfAudit")
    TMap<FString, float> GetHistoricalDecisionAnalysis() const;
    
    // ========== 智能建議 ==========
    
    /** 獲取指揮建議 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|AI")
    FCommandRecommendation GetCommandRecommendation() const;
    
    /** 評估當前指揮狀態 */
    UFUNCTION(BlueprintCallable, Category = "SupremeSage|AI")
    float EvaluateCommandState() const;
    
    // ========== 事件接口 ==========
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnPhaseChanged OnPhaseChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnAuthoritySwitched OnAuthoritySwitched;
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnCorruptionSignDetected OnCorruptionSignDetected;
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnWarningIssued OnWarningIssued;
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnStrategyExecuted OnStrategyExecuted;
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnCommandTransferRequired OnCommandTransferRequired;
    
    UPROPERTY(BlueprintAssignable, Category = "SupremeSage|Events")
    FOnSelfAuditCompleted OnSelfAuditCompleted;
    
protected:
    // ========== 內部方法 ==========
    
    /** 確定下一階段 */
    EFiveElementPhase DetermineNextPhase() const;
    
    /** 計算墮落風險 */
    float CalculateCorruptionRisk() const;
    
    /** 更新系統統計 */
    void UpdateSystemStatistics();
    
    /** 檢查階段轉換條件 */
    bool CheckPhaseTransitionConditions(EFiveElementPhase From, EFiveElementPhase To) const;
    
    /** 記錄系統日誌 */
    void LogSystemEvent(const FString& Event);
    
private:
    // ========== 成員變量 ==========
    
    UPROPERTY()
    ECommandStatus SystemStatus;
    
    UPROPERTY()
    ESupremeAuthorityType CurrentAuthority;
    
    UPROPERTY()
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY()
    FPhaseContext CurrentPhaseContext;
    
    UPROPERTY()
    TArray<FAuthorityStatus> AuthorityStatuses;
    
    UPROPERTY()
    int32 TotalCommandsIssued;
    
    UPROPERTY()
    int32 EvilForceUsageCount;
    
    UPROPERTY()
    int32 PhaseTransitionCount;
    
    UPROPERTY()
    TArray<FString> CriticalDecisionHistory;
    
    UPROPERTY()
    float TotalDecisionQualityScore;
    
    UPROPERTY()
    FDateTime LastAuditTime;
    
    // 子系統
    UPROPERTY()
    UMingThreeAuthoritiesManager* AuthoritiesManager;
    
    UPROPERTY()
    UMingFiveElementsRotation* FiveElementsRotation;
    
    UPROPERTY()
    UMingCorruptionPrevention* CorruptionPrevention;
    
    UPROPERTY()
    UMingSixConquestExecutor* SixConquestExecutor;
    
    UPROPERTY()
    UMingTwelveStrategiesExecutor* TwelveStrategiesExecutor;
};
