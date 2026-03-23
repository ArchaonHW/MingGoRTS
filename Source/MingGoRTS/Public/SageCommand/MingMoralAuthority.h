#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingMoralAuthority.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingThreePowerSystem;

// ============================================================================
// 墮落徵象枚舉定義
// ============================================================================

/** 墮落徵象類型 */
UENUM(BlueprintType)
enum class ECorruptionPhenomenon : uint8
{
    None                                    UMETA(DisplayName = "無"),
    RationalizationOfEvil                   UMETA(DisplayName = "將非常手段合理化"),
    FollowerDeification                     UMETA(DisplayName = "將追隨者神聖化"),
    OppositionDemonization                  UMETA(DisplayName = "將反對者妖魔化"),
    IAmJusticeSyndrome                     UMETA(DisplayName = "我即正義徵象"),
    PowerObsession                         UMETA(DisplayName = "權力沉迷"),
    MoralNumbness                          UMETA(DisplayName = "道德麻木"),
    CrueltyJustification                   UMETA(DisplayName = "殘忍正當化")
};

/** 失去資格判準 */
UENUM(BlueprintType)
enum class EDisqualificationCriteria : uint8
{
    None                                    UMETA(DisplayName = "無"),
    ExcessiveEvilUsage                     UMETA(DisplayName = "過度使用邪術"),
    LossOfMoralCompass                    UMETA(DisplayName = "失去道德羅盤"),
    AbuseOfPower                          UMETA(DisplayName = "權力濫用"),
    IrreversibleCorruption                  UMETA(DisplayName = "不可逆墮落"),
    ViolationOfCorePrinciples              UMETA(DisplayName = "違背核心原則")
};

/** 道德邊界級別 */
UENUM(BlueprintType)
enum class EMoralBoundaryLevel : uint8
{
    Safe                                   UMETA(DisplayName = "安全"),
    Warning                                UMETA(DisplayName = "警告"),
    Dangerous                              UMETA(DisplayName = "危險"),
    Critical                               UMETA(DisplayName = "嚴重"),
    Terminal                               UMETA(DisplayName = "終止")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 墮落徵象數據 */
USTRUCT(BlueprintType)
struct FCorruptionPhenomenonData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    ECorruptionPhenomenon PhenomenonType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    float Severity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    float Duration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    FString Description;
    
    UPROPERTY(BlueprintReadWrite, Category = "Corruption")
    FDateTime StartTime;
    
    FCorruptionPhenomenonData()
        : PhenomenonType(ECorruptionPhenomenon::None)
        , Severity(0.0f)
        , Duration(0.0f)
    {}
};

/** 道德邊界狀態 */
USTRUCT(BlueprintType)
struct FMoralBoundaryState
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    EMoralBoundaryLevel CurrentLevel;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float MoralIntegrity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    float CorruptionResistance;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    TArray<ECorruptionPhenomenon> ActivePhenomena;
    
    UPROPERTY(BlueprintReadWrite, Category = "Moral Boundary")
    bool bIsQualified;
    
    FMoralBoundaryState()
        : CurrentLevel(EMoralBoundaryLevel::Safe)
        , MoralIntegrity(100.0f)
        , CorruptionResistance(100.0f)
        , bIsQualified(true)
    {}
};

/** 防墮警告數據 */
USTRUCT(BlueprintType)
struct FCorruptionWarningData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    EMoralBoundaryLevel WarningLevel;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    FString WarningMessage;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    TArray<ECorruptionPhenomenon> DetectedPhenomena;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    float TimeUntilDisqualification;
    
    UPROPERTY(BlueprintReadWrite, Category = "Warning")
    TArray<FString> RecommendedActions;
    
    FCorruptionWarningData()
        : WarningLevel(EMoralBoundaryLevel::Safe)
        , TimeUntilDisqualification(-1.0f)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCorruptionPhenomenonDetected,
    ECorruptionPhenomenon, PhenomenonType,
    const FCorruptionPhenomenonData&, PhenomenonData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoralBoundaryChanged,
    EMoralBoundaryLevel, NewLevel,
    EMoralBoundaryLevel, PreviousLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisqualificationTriggered,
    EDisqualificationCriteria, Criteria);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCorruptionWarningIssued,
    EMoralBoundaryLevel, WarningLevel,
    const FCorruptionWarningData&, WarningData);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 道德權威系統
 * 負責監測墮落徵象、執行道德邊界檢查、管理防墮警告系統
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Moral Authority")
class MINGGORTS_API UMingMoralAuthority : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingMoralAuthority();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化道德權威系統 */
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    bool InitializeMoralAuthority();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Moral Authority")
    void ResetMoralAuthority();

    // ============================================================================
    // 墮落徵象監測
    // ============================================================================

    /** 檢測墮落徵象 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool DetectCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType, float Severity);

    /** 獲取當前活躍的墮落徵象 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    TArray<FCorruptionPhenomenonData> GetActiveCorruptionPhenomena() const;

    /** 計算總墮落程度 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    float CalculateTotalCorruptionLevel() const;

    /** 檢查特定墮落徵象是否存在 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool HasCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType) const;

    /** 清除墮落徵象 */
    UFUNCTION(BlueprintCallable, Category = "Corruption Detection")
    bool ClearCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType);

    // ============================================================================
    // 失去資格判準
    // ============================================================================

    /** 檢查是否失去資格 */
    UFUNCTION(BlueprintCallable, Category = "Qualification")
    bool CheckDisqualification();

    /** 獲取失去資格的原因 */
    UFUNCTION(BlueprintCallable, Category = "Qualification")
    TArray<EDisqualificationCriteria> GetDisqualificationReasons() const;

    /** 計算失去資格風險 */
    UFUNCTION(BlueprintCallable, Category = "Qualification")
    float CalculateDisqualificationRisk() const;

    /** 嘗試恢復資格 */
    UFUNCTION(BlueprintCallable, Category = "Qualification")
    bool AttemptQualificationRestoration();

    /** 設置失去資格判準參數 */
    UFUNCTION(BlueprintCallable, Category = "Qualification")
    void SetDisqualificationCriteria(const TArray<EDisqualificationCriteria>& Criteria);

    // ============================================================================
    // 道德邊界檢查
    // ============================================================================

    /** 檢查道德邊界 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    EMoralBoundaryLevel CheckMoralBoundary(const FString& Action, float MoralImpact);

    /** 獲取當前道德邊界狀態 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    FMoralBoundaryState GetCurrentMoralBoundaryState() const;

    /** 設置道德邊界參數 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    void SetMoralBoundaryParameters(float IntegrityThreshold, float ResistanceDecayRate);

    /** 恢復道德完整性 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    void RestoreMoralIntegrity(float Amount);

    /** 強化道德抵抗 */
    UFUNCTION(BlueprintCallable, Category = "Moral Boundary")
    void StrengthenMoralResistance(float Amount);

    // ============================================================================
    // 防墮警告系統
    // ============================================================================

    /** 發布防墮警告 */
    UFUNCTION(BlueprintCallable, Category = "Warning System")
    void IssueCorruptionWarning(EMoralBoundaryLevel WarningLevel, const FString& Message);

    /** 獲取當前警告狀態 */
    UFUNCTION(BlueprintCallable, Category = "Warning System")
    TArray<FCorruptionWarningData> GetActiveWarnings() const;

    /** 清除警告 */
    UFUNCTION(BlueprintCallable, Category = "Warning System")
    void ClearWarning(EMoralBoundaryLevel WarningLevel);

    /** 設置警告系統參數 */
    UFUNCTION(BlueprintCallable, Category = "Warning System")
    void SetWarningSystemParameters(float WarningThreshold, float CriticalThreshold);

    /** 檢查是否需要發布警告 */
    UFUNCTION(BlueprintCallable, Category = "Warning System")
    bool ShouldIssueWarning() const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 墮落徵象檢測事件 */
    UPROPERTY(BlueprintAssignable, Category = "Moral Authority Events")
    FOnCorruptionPhenomenonDetected OnCorruptionPhenomenonDetected;

    /** 道德邊界改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Moral Authority Events")
    FOnMoralBoundaryChanged OnMoralBoundaryChanged;

    /** 失去資格觸發事件 */
    UPROPERTY(BlueprintAssignable, Category = "Moral Authority Events")
    FOnDisqualificationTriggered OnDisqualificationTriggered;

    /** 防墮警告發布事件 */
    UPROPERTY(BlueprintAssignable, Category = "Moral Authority Events")
    FOnCorruptionWarningIssued OnCorruptionWarningIssued;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 當前道德邊界狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Moral Authority")
    FMoralBoundaryState CurrentBoundaryState;

    /** 活躍的墮落徵象 */
    UPROPERTY(BlueprintReadOnly, Category = "Moral Authority")
    TArray<FCorruptionPhenomenonData> ActiveCorruptionPhenomena;

    /** 當前警告 */
    UPROPERTY(BlueprintReadOnly, Category = "Moral Authority")
    TArray<FCorruptionWarningData> ActiveWarnings;

    /** 失去資格判準 */
    UPROPERTY(BlueprintReadWrite, Category = "Moral Authority")
    TArray<EDisqualificationCriteria> DisqualificationCriteria;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Moral Authority")
    float IntegrityThreshold;

    UPROPERTY(BlueprintReadWrite, Category = "Moral Authority")
    float ResistanceDecayRate;

    UPROPERTY(BlueprintReadWrite, Category = "Moral Authority")
    float WarningThreshold;

    UPROPERTY(BlueprintReadWrite, Category = "Moral Authority")
    float CriticalThreshold;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Moral Authority")
    bool bSystemInitialized;

    UPROPERTY(BlueprintReadOnly, Category = "Moral Authority")
    bool bIsDisqualified;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 更新墮落徵象狀態 */
    void UpdateCorruptionPhenomena(float DeltaTime);

    /** 更新道德邊界狀態 */
    void UpdateMoralBoundaryState(float DeltaTime);

    /** 檢查並觸發警告 */
    void CheckAndTriggerWarnings();

    /** 評估行動的道德影響 */
    float EvaluateMoralImpact(const FString& Action) const;

    /** 計算墮落徵象的影響 */
    float CalculatePhenomenonImpact(ECorruptionPhenomenon PhenomenonType, float Severity) const;

    /** 生成警告消息 */
    FString GenerateWarningMessage(EMoralBoundaryLevel Level) const;

    /** 獲取推薦的恢復行動 */
    TArray<FString> GetRecommendedRecoveryActions() const;

    /** 檢查系統一致性 */
    bool ValidateSystemConsistency() const;

private:
    /** 定時器句柄 */
    FTimerHandle CorruptionCheckTimer;

    /** 最大道德完整性 */
    static constexpr float MAX_MORAL_INTEGRITY = 100.0f;

    /** 最小道德完整性 */
    static constexpr float MIN_MORAL_INTEGRITY = 0.0f;

    /** 墮落檢測間隔 */
    static constexpr float CORRUPTION_CHECK_INTERVAL = 1.0f;
};
