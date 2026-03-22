#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCommandAuthority.generated.h"

// 指揮權類型枚舉
UENUM(BlueprintType)
enum class ECommandAuthorityType: uint8 {
    MoralAuthority = 0,     // 道德權威 - 基於倫理和正義
    LegalAuthority = 1,     // 法理權威 - 基於法律和制度
    CharismaticAuthority = 2, // 魅力權威 - 基於個人魅力
    TraditionalAuthority = 3, // 傳統權威 - 基於傳統和習俗
    ExpertAuthority = 4,   // 專業權威 - 基於專業知識
    SituationalAuthority = 5 // 情境權威 - 基於當前情境
};

// 權威狀態枚舉
UENUM(BlueprintType)
enum class EAuthorityState: uint8 {
    Strong = 0,        // 強大權威
    Stable = 1,        // 穩定權威
    Weakening = 2,     // 衰弱權威
    Challenged = 3,    // 受挑戰權威
    Collapsing = 4,    // 崩潰權威
    Restoring = 5      // 恢復權威
};

// 權威合法性來源
UENUM(BlueprintType)
enum class EAuthorityLegitimacySource: uint8 {
    DivineRight = 0,       // 天命所歸
    PopularSupport = 1,    // 民心所向
    LegalFramework = 2,    // 法理依據
    HistoricalTradition = 3, // 歷史傳統
    MilitarySuccess = 4,   // 軍事成就
    EconomicProsperity = 5 // 經濟繁榮
};

// 權威指標結構
USTRUCT(BlueprintType)
struct FAuthorityMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ECommandAuthorityType PrimaryAuthority = ECommandAuthorityType::MoralAuthority;

    UPROPERTY(BlueprintReadOnly)
    EAuthorityState CurrentState = EAuthorityState::Stable;

    UPROPERTY(BlueprintReadOnly)
    float AuthorityLevel = 50.0f;        // 權威水平 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float LegitimacyScore = 50.0f;      // 合法性分數 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ComplianceRate = 50.0f;        // 服從率 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float InfluenceRadius = 50.0f;      // 影響範圍 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float CommandEffectiveness = 50.0f;   // 指揮有效性 (0-100)
};

// 權威事件結構
USTRUCT(BlueprintType)
struct FAuthorityEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    ECommandAuthorityType AuthorityType;

    UPROPERTY(BlueprintReadOnly)
    float AuthorityChange = 0.0f;        // 權威變化 (-100 to 100)

    UPROPERTY(BlueprintReadOnly)
    float LegitimacyImpact = 0.0f;       // 合法性影響 (-100 to 100)

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
};

// 指揮有效性評估
USTRUCT(BlueprintType)
struct FCommandEffectiveness
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CommandID;

    UPROPERTY(BlueprintReadOnly)
    FString CommandDescription;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionSpeed = 0.0f;         // 執行速度 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ComplianceLevel = 0.0f;        // 服從程度 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ResultQuality = 0.0f;           // 結果質量 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ResourceEfficiency = 0.0f;      // 資源效率 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float OverallEffectiveness = 0.0f;    // 整體有效性 (0-100)
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthorityChanged, const FAuthorityMetrics&, Metrics);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLegitimacyChanged, float, NewLegitimacy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthorityEventOccurred, const FAuthorityEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandEffectivenessEvaluated, const FCommandEffectiveness&, Effectiveness);

/**
 * 指揮權系統
 * 管理和維護指揮者的權威性和合法性
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageCommand))
class MINGGORTS_API UMingCommandAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingCommandAuthority();

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    void Cleanup();

    // 權威管理
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    FAuthorityMetrics GetAuthorityMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool SetPrimaryAuthority(ECommandAuthorityType AuthorityType);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    ECommandAuthorityType GetPrimaryAuthority() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool EnhanceAuthority(ECommandAuthorityType AuthorityType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    float GetAuthorityLevel() const;

    // 合法性管理
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    float GetLegitimacyScore() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool EnhanceLegitimacy(EAuthorityLegitimacySource Source, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    TArray<EAuthorityLegitimacySource> GetLegitimacySources() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool IsLegitimate() const;

    // 指揮有效性
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    FCommandEffectiveness EvaluateCommandEffectiveness(const FString& CommandID, const FString& CommandDescription);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    float GetCommandEffectiveness() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool ImproveCommandEffectiveness(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    TArray<FString> GetCommandRecommendations() const;

    // 服從率管理
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    float GetComplianceRate() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool ImproveCompliance(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    TArray<FString> GetComplianceFactors() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool IsCommandComplied(const FString& Command) const;

    // 影響範圍
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    float GetInfluenceRadius() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool ExpandInfluence(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    TArray<FString> GetInfluencedRegions() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool CanInfluenceRegion(const FString& Region) const;

    // 權威挑戰應對
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool HandleAuthorityChallenge(const FString& Challenger, float ChallengeStrength);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool DefendAuthority(ECommandAuthorityType DefenseType);

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    TArray<FString> GetThreatsToAuthority() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool IsAuthorityUnderThreat() const;

    // 歷史記錄
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    TArray<FAuthorityEvent> GetAuthorityHistory() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    FAuthorityEvent GetLastAuthorityEvent() const;

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    void ClearAuthorityHistory();

    // 系統狀態
    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    bool IsSystemActive() const { return bSystemActive; }

    UFUNCTION(BlueprintCallable, Category = "Command Authority")
    float GetSystemStability() const { return SystemStability; }

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnAuthorityChanged OnAuthorityChanged;

    UPROPERTY(BlueprintAssignable)
    FOnLegitimacyChanged OnLegitimacyChanged;

    UPROPERTY(BlueprintAssignable)
    FOnAuthorityEventOccurred OnAuthorityEventOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnCommandEffectivenessEvaluated OnCommandEffectivenessEvaluated;

protected:
    // 系統狀態
    UPROPERTY(BlueprintReadOnly, Category = "Command Authority")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "Command Authority")
    float SystemStability = 100.0f;

    // 當前權威指標
    UPROPERTY(BlueprintReadOnly, Category = "Command Authority")
    FAuthorityMetrics CurrentMetrics;

    // 權威歷史記錄
    UPROPERTY(BlueprintReadOnly, Category = "Command Authority")
    TArray<FAuthorityEvent> AuthorityHistory;

    // 合法性來源
    UPROPERTY(BlueprintReadOnly, Category = "Command Authority")
    TArray<EAuthorityLegitimacySource> LegitimacySources;

    // 指揮有效性記錄
    UPROPERTY(BlueprintReadOnly, Category = "Command Authority")
    TArray<FCommandEffectiveness> CommandEffectivenessHistory;

private:
    // 權威計算
    float CalculateAuthorityLevel() const;
    float CalculateLegitimacyScore() const;
    float CalculateComplianceRate() const;
    float CalculateInfluenceRadius() const;
    float CalculateCommandEffectiveness() const;

    // 合法性評估
    float EvaluateDivineRightLegitimacy() const;
    float EvaluatePopularSupportLegitimacy() const;
    float EvaluateLegalFrameworkLegitimacy() const;
    float EvaluateHistoricalTraditionLegitimacy() const;
    float EvaluateMilitarySuccessLegitimacy() const;
    float EvaluateEconomicProsperityLegitimacy() const;

    // 權威增強
    bool EnhanceMoralAuthority(float Amount);
    bool EnhanceLegalAuthority(float Amount);
    bool EnhanceCharismaticAuthority(float Amount);
    bool EnhanceTraditionalAuthority(float Amount);
    bool EnhanceExpertAuthority(float Amount);
    bool EnhanceSituationalAuthority(float Amount);

    // 挑戰應對
    bool AnalyzeChallenge(const FString& Challenger, float ChallengeStrength);
    bool SelectDefenseStrategy(ECommandAuthorityType DefenseType);
    bool ExecuteDefenseStrategy();

    // 事件記錄
    void RecordAuthorityEvent(const FString& Description, ECommandAuthorityType AuthorityType, float AuthorityChange, float LegitimacyImpact);
    void UpdateAuthorityMetrics();

    // 輔助方法
    FString GetAuthorityName(ECommandAuthorityType AuthorityType) const;
    FString GetStateName(EAuthorityState State) const;
    FString GetLegitimacySourceName(EAuthorityLegitimacySource Source) const;

    // 權威衰減
    void ApplyAuthorityDecay(float DeltaTime);
    void ApplyLegitimacyDecay(float DeltaTime);

    // 配置參數
    UPROPERTY(Config)
    float AuthorityDecayRate = 0.1f;

    UPROPERTY(Config)
    float LegitimacyDecayRate = 0.05f;

    UPROPERTY(Config)
    float ChallengeThreshold = 30.0f;

    // 定時器句柄
    FTimerHandle DecayTimerHandle;
};
