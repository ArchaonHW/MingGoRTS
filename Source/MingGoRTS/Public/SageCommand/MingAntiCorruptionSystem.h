#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingAntiCorruptionSystem.generated.h"

// ============================================================================
// 防墮機制系統枚舉定義
// ============================================================================

/** 墮落徵象嚴重程度 */
UENUM(BlueprintType)
enum class ECorruptionSeverity : uint8
{
    None            UMETA(DisplayName = "無"),
    Mild            UMETA(DisplayName = "輕微"),
    Moderate        UMETA(DisplayName = "中等"),
    Severe          UMETA(DisplayName = "嚴重"),
    Critical        UMETA(DisplayName = "危急")
};

/** 墮落檢測類型 */
UENUM(BlueprintType)
enum class ECorruptionDetectionType : uint8
{
    None                            UMETA(DisplayName = "無"),
    RationalizationOfEvil           UMETA(DisplayName = "合理化邪術"),
    FollowerDeification             UMETA(DisplayName = "追隨者神聖化"),
    OppositionDemonization          UMETA(DisplayName = "反對者妖魔化"),
    IAmJusticeSyndrome              UMETA(DisplayName = "我即正義徵象"),
    PowerObsession                  UMETA(DisplayName = "權力沉迷"),
    MoralNumbness                   UMETA(DisplayName = "道德麻木"),
    CrueltyJustification            UMETA(DisplayName = "殘忍正當化")
};

/** 防墮措施類型 */
UENUM(BlueprintType)
enum class EAntiCorruptionMeasure : uint8
{
    None                    UMETA(DisplayName = "無"),
    SelfReflection          UMETA(DisplayName = "自我反思"),
    ExternalAudit           UMETA(DisplayName = "外部審查"),
    MoralRestraint          UMETA(DisplayName = "道德約束"),
    PowerLimitation         UMETA(DisplayName = "權力限制"),
    Accountability          UMETA(DisplayName = "問責機制"),
    Transparency            UMETA(DisplayName = "透明化"),
    SeparationOfPowers      UMETA(DisplayName = "分權制衡")
};

/** 自我審計狀態 */
UENUM(BlueprintType)
enum class ESelfAuditStatus : uint8
{
    None            UMETA(DisplayName = "無"),
    Scheduled       UMETA(DisplayName = "已計劃"),
    InProgress      UMETA(DisplayName = "進行中"),
    Completed       UMETA(DisplayName = "已完成"),
    IssuesFound     UMETA(DisplayName = "發現問題"),
    Corrected       UMETA(DisplayName = "已糾正")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 墮落徵象數據 */
USTRUCT(BlueprintType)
struct FCorruptionPhenomenon
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    ECorruptionPhenomenon PhenomenonType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    ECorruptionSeverity Severity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    float SeverityValue;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    FString Description;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    FDateTime DetectionTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    float Duration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    TArray<FString> Evidence;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    bool bIsAcknowledged;
    
    FCorruptionPhenomenon()
        : PhenomenonType(ECorruptionPhenomenon::None)
        , Severity(ECorruptionSeverity::None)
        , SeverityValue(0.0f)
        , Duration(0.0f)
        , bIsAcknowledged(false)
    {}
};

/** 防墮警告數據 */
USTRUCT(BlueprintType)
struct FAntiCorruptionWarning
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    int32 WarningID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    ECorruptionPhenomenon TargetPhenomenon;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    ECorruptionSeverity WarningLevel;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    FString WarningMessage;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    TArray<EAntiCorruptionMeasure> RecommendedMeasures;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    FDateTime IssueTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    float TimeToCorrect;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    bool bIsUrgent;
    
    FAntiCorruptionWarning()
        : WarningID(0)
        , TargetPhenomenon(ECorruptionPhenomenon::None)
        , WarningLevel(ECorruptionSeverity::None)
        , TimeToCorrect(0.0f)
        , bIsUrgent(false)
    {}
};

/** 自我審計數據 */
USTRUCT(BlueprintType)
struct FSelfAuditData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    int32 AuditID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    ESelfAuditStatus Status;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    FDateTime StartTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    FDateTime EndTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    TArray<ECorruptionPhenomenon> CheckedPhenomena;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    TArray<FCorruptionPhenomenon> DetectedIssues;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    TArray<EAntiCorruptionMeasure> RecommendedActions;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    float OverallHealthScore;
    
    UPROPERTY(BlueprintReadWrite, Category = "Self Audit")
    FString AuditReport;
    
    FSelfAuditData()
        : AuditID(0)
        , Status(ESelfAuditStatus::None)
        , OverallHealthScore(100.0f)
    {}
};

/** 防墮措施效果 */
USTRUCT(BlueprintType)
struct FAntiCorruptionMeasureEffect
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Measure Effect")
    EAntiCorruptionMeasure MeasureType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Measure Effect")
    float Effectiveness;
    
    UPROPERTY(BlueprintReadWrite, Category = "Measure Effect")
    float Duration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Measure Effect")
    TArray<ECorruptionPhenomenon> TargetPhenomena;
    
    UPROPERTY(BlueprintReadWrite, Category = "Measure Effect")
    float Cost;
    
    UPROPERTY(BlueprintReadWrite, Category = "Measure Effect")
    bool bIsActive;
    
    FAntiCorruptionMeasureEffect()
        : MeasureType(EAntiCorruptionMeasure::None)
        , Effectiveness(0.0f)
        , Duration(0.0f)
        , Cost(0.0f)
        , bIsActive(false)
    {}
};

/** 道德邊界狀態 */
USTRUCT(BlueprintType)
struct FMoralBoundaryStatus
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float MoralIntegrity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float CorruptionResistance;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float WarningThreshold;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float CriticalThreshold;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    bool bIsWithinBoundary;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float RiskLevel;
    
    FMoralBoundaryStatus()
        : MoralIntegrity(100.0f)
        , CorruptionResistance(100.0f)
        , WarningThreshold(70.0f)
        , CriticalThreshold(30.0f)
        , bIsWithinBoundary(true)
        , RiskLevel(0.0f)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCorruptionPhenomenonDetected,
    const FCorruptionPhenomenon&, Phenomenon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAntiCorruptionWarningIssued,
    const FAntiCorruptionWarning&, Warning);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelfAuditCompleted,
    const FSelfAuditData&, AuditData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoralBoundaryCrossed,
    float, NewIntegrity,
    float, PreviousIntegrity);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAntiCorruptionMeasureApplied,
    EAntiCorruptionMeasure, Measure,
    float, Effectiveness);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 防墮機制系統
 * 負責墮落徵象檢測、防墮警告系統、自我審計機制、道德邊界管理
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Anti Corruption")
class MINGGORTS_API UMingAntiCorruptionSystem : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingAntiCorruptionSystem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化防墮機制系統 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption")
    bool InitializeAntiCorruptionSystem();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption")
    void ResetAntiCorruptionSystem();

    // ============================================================================
    // 墮落徵象檢測
    // ============================================================================

    /** 檢測墮落徵象 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool DetectCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType, float Severity, const FString& Evidence = TEXT(""));

    /** 獲取當前墮落徵象列表 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    TArray<FCorruptionPhenomenon> GetCurrentCorruptionPhenomena() const;

    /** 獲取特定徵象數據 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    FCorruptionPhenomenon GetCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType) const;

    /** 計算總墮落風險 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    float CalculateTotalCorruptionRisk() const;

    /** 檢查是否出現特定徵象 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool HasCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType) const;

    /** 清除墮落徵象 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool ClearCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType);

    /** 更新徵象嚴重程度 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool UpdatePhenomenonSeverity(ECorruptionPhenomenon PhenomenonType, float NewSeverity);

    // ============================================================================
    // 防墮警告系統
    // ============================================================================

    /** 發布防墮警告 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Warning")
    int32 IssueAntiCorruptionWarning(ECorruptionPhenomenon TargetPhenomenon, ECorruptionSeverity WarningLevel);

    /** 獲取當前警告列表 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Warning")
    TArray<FAntiCorruptionWarning> GetCurrentWarnings() const;

    /** 解除警告 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Warning")
    bool ResolveWarning(int32 WarningID);

    /** 獲取警告詳情 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Warning")
    FAntiCorruptionWarning GetWarningDetails(int32 WarningID) const;

    /** 設置警告系統參數 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Warning")
    void SetWarningParameters(float WarningThreshold, float CriticalThreshold);

    /** 自動生成警告建議 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Warning")
    TArray<EAntiCorruptionMeasure> GenerateWarningRecommendations(ECorruptionPhenomenon PhenomenonType) const;

    // ============================================================================
    // 自我審計機制
    // ============================================================================

    /** 啟動自我審計 */
    UFUNCTION(BlueprintCallable, Category = "Self Audit")
    int32 StartSelfAudit();

    /** 執行審計檢查 */
    UFUNCTION(BlueprintCallable, Category = "Self Audit")
    bool PerformAuditCheck(int32 AuditID, ECorruptionPhenomenon PhenomenonToCheck);

    /** 完成自我審計 */
    UFUNCTION(BlueprintCallable, Category = "Self Audit")
    FSelfAuditData CompleteSelfAudit(int32 AuditID);

    /** 獲取審計歷史 */
    UFUNCTION(BlueprintCallable, Category = "Self Audit")
    TArray<FSelfAuditData> GetAuditHistory() const;

    /** 獲取審計詳情 */
    UFUNCTION(BlueprintCallable, Category = "Self Audit")
    FSelfAuditData GetAuditDetails(int32 AuditID) const;

    /** 設置審計計劃 */
    UFUNCTION(BlueprintCallable, Category = "Self Audit")
    bool ScheduleRegularAudit(float Interval);

    // ============================================================================
    // 道德邊界管理
    // ============================================================================

    /** 檢查道德邊界狀態 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    FMoralBoundaryStatus CheckMoralBoundaryStatus() const;

    /** 更新道德完整性 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    bool UpdateMoralIntegrity(float Delta);

    /** 強化道德抵抗 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    bool StrengthenMoralResistance(float Amount);

    /** 設置道德邊界參數 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    void SetMoralBoundaryParameters(float WarningThreshold, float CriticalThreshold);

    /** 評估行動的道德風險 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    float EvaluateMoralRisk(const FString& Action, float Impact) const;

    /** 獲取道德建議 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    TArray<FString> GetMoralRecommendations() const;

    // ============================================================================
    // 防墮措施
    // ============================================================================

    /** 應用防墮措施 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Measures")
    bool ApplyAntiCorruptionMeasure(EAntiCorruptionMeasure Measure);

    /** 獲取措施效果 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Measures")
    FAntiCorruptionMeasureEffect GetMeasureEffect(EAntiCorruptionMeasure Measure) const;

    /** 獲取推薦措施 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Measures")
    TArray<EAntiCorruptionMeasure> GetRecommendedMeasures(ECorruptionPhenomenon TargetPhenomenon) const;

    /** 計算措施效果 */
    UFUNCTION(BlueprintCallable, Category = "Anti Corruption Measures")
    float CalculateMeasureEffectiveness(EAntiCorruptionMeasure Measure, ECorruptionPhenomenon Target) const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 墮落徵象檢測事件 */
    UPROPERTY(BlueprintAssignable, Category = "Anti Corruption Events")
    FOnCorruptionPhenomenonDetected OnCorruptionPhenomenonDetected;

    /** 防墮警告發布事件 */
    UPROPERTY(BlueprintAssignable, Category = "Anti Corruption Events")
    FOnAntiCorruptionWarningIssued OnAntiCorruptionWarningIssued;

    /** 自我審計完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "Anti Corruption Events")
    FOnSelfAuditCompleted OnSelfAuditCompleted;

    /** 道德邊界跨越事件 */
    UPROPERTY(BlueprintAssignable, Category = "Anti Corruption Events")
    FOnMoralBoundaryCrossed OnMoralBoundaryCrossed;

    /** 防墮措施應用事件 */
    UPROPERTY(BlueprintAssignable, Category = "Anti Corruption Events")
    FOnAntiCorruptionMeasureApplied OnAntiCorruptionMeasureApplied;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 當前墮落徵象 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    TMap<ECorruptionPhenomenon, FCorruptionPhenomenon> CurrentCorruptionPhenomena;

    /** 活動警告列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    TArray<FAntiCorruptionWarning> ActiveWarnings;

    /** 警告歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    TArray<FAntiCorruptionWarning> WarningHistory;

    /** 審計歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    TArray<FSelfAuditData> AuditHistory;

    /** 當前審計 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    TMap<int32, FSelfAuditData> CurrentAudits;

    /** 道德邊界狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    FMoralBoundaryStatus MoralBoundaryStatus;

    /** 活動防墮措施 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    TArray<FAntiCorruptionMeasureEffect> ActiveMeasures;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Anti Corruption")
    float AuditInterval;

    UPROPERTY(BlueprintReadWrite, Category = "Anti Corruption")
    int32 MaxWarnings;

    UPROPERTY(BlueprintReadWrite, Category = "Anti Corruption")
    float PhenomenonDecayRate;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    bool bSystemInitialized;

    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    int32 NextWarningID;

    UPROPERTY(BlueprintReadOnly, Category = "Anti Corruption")
    int32 NextAuditID;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 自動檢測墮落徵象 */
    void AutoDetectCorruptionPhenomena();

    /** 評估徵象嚴重程度 */
    ECorruptionSeverity AssessSeverity(float Value) const;

    /** 生成警告信息 */
    FString GenerateWarningMessage(ECorruptionPhenomenon PhenomenonType, ECorruptionSeverity Severity) const;

    /** 更新道德狀態 */
    void UpdateMoralStatus(float DeltaTime);

    /** 處理警告超時 */
    void HandleWarningTimeouts();

    /** 執行定期審計 */
    void PerformScheduledAudit();

    /** 驗證措施有效性 */
    bool ValidateMeasureEffectiveness(EAntiCorruptionMeasure Measure) const;

    /** 觸發徵象檢測事件 */
    void TriggerPhenomenonDetectedEvent(const FCorruptionPhenomenon& Phenomenon);

    /** 觸發警告事件 */
    void TriggerWarningIssuedEvent(const FAntiCorruptionWarning& Warning);

    /** 計算道德風險 */
    float CalculateMoralRisk() const;

private:
    /** 定時器句柄 */
    FTimerHandle AutoDetectionTimer;
    FTimerHandle AuditTimer;
    FTimerHandle StatusUpdateTimer;

    /** 檢測間隔 */
    static constexpr float AUTO_DETECTION_INTERVAL = 5.0f;

    /** 最大警告數量 */
    static constexpr int32 MAX_ACTIVE_WARNINGS = 10;

    /** 最大審計數量 */
    static constexpr int32 MAX_AUDIT_HISTORY = 50;
};
