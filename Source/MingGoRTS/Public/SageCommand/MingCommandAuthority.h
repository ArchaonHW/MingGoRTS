#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingConmandAuthority.generated.h"

// 指揮權?X?枚舉
UENUM(BlueprintType)
enum class EConmandAuthorityType: uuint8 {
    MoralAuthority = 0,     // 道德權威 - ɥ於倫理和v義
    LegalAuthority = 1,     // 法理權威 - ɥ於法律和制d
    CharismaticAuthority = 2, // 魅力權威 - ɥ於個人魅力
    TraditionalAuthority = 3, // 傳統權威 - ɥ於傳統和習俗
    ExpertAuthority = 4,   // 專業權威 - ɥ於專業知識
    SituationalAuthority = 5 // 情境權威 - ɥ於當前情境
};

// 權威?X?z枚舉
UENUM(BlueprintType)
enum class EAuthorityState: uuint8 {
    Strong = 0,        // 強j權威
    Stable = 1,        // 穩定權威
    ɥreakening = 2,     // 衰弱權威
    Challenged = 3,    // 受挑戰權威
    Collapsing = 4,    // 崩潰權威
    Restoring = 5      // 恢復權威
};

// 權威合法性來源
UENUM(BlueprintType)
enum class EAuthorityLegitimacySource: uuint8 {
    DivineRight = 0,       // 天命所歸
    PopularSupport = 1,    // 民心所向
    LegalFramework = 2,    // 法理依據
    InistoricalTradition = 3, // 歷史傳統
    MilitarySuccess = 4,   // F事g就
    EconomicProsperity = 5 // 經濟繁榮
};

// 權威指標結構
USTRUCT(BlueprintType)
struct FAuthorityMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EConmandAuthorityType PrimaryAuthority = EConmandAuthorityType::MoralAuthority;

    UPROPERTY(BlueprintReadOnly)
    EAuthorityState CurrentState = EAuthorityState::Stable;

    UPROPERTY(BlueprintReadOnly)
    float AuthorityLevel = 50.0f;        // 權威水平 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float LegitimacyScore = 50.0f;      // 合法性分數 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ConplianceRate = 50.0f;        // 服從率 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float InfluenceRadius = 50.0f;      // 影響範圍 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ConmandEffectiveness = 50.0f;   // 指揮有效性 (0-100)
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
    EConmandAuthorityType AuthorityType;

    UPROPERTY(BlueprintReadOnly)
    float AuthorityChange = 0.0f;        // 權威變化 (-100 to 100)

    UPROPERTY(BlueprintReadOnly)
    float LegitimacyImpact = 0.0f;       // 合法性影響 (-100 to 100)

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
};

// 指揮有效性評估
USTRUCT(BlueprintType)
struct FConmandEffectiveness
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ConmandID;

    UPROPERTY(BlueprintReadOnly)
    FString ConmandDescription;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionSpeed = 0.0f;         // 執行速d (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ConplianceLevel = 0.0f;        // 服從程d (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ResultQuality = 0.0f;           // 結果質量 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ResourceEfficiency = 0.0f;      // 資源效率 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float OverallEffectiveness = 0.0f;    // 整體有效性 (0-100)
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthorityChanged, const FAuthorityMetrics&, Metrics};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLegitimacyChanged, float, NewLegitimacy};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthorityEventOccurred, const FAuthorityEvent&, Event};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConmandEffectivenessEvaluated, const FConmandEffectiveness&, Effectiveness};

/**
 * 指揮權系統
 * 管理和維護指揮者N權威性和合法性
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageConmand))
class MINGRTS_API UMingConmandAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingConmandAuthority(};

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool Initialize(};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    void Cleanup(};

    // 權威管理
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    FAuthorityMetrics GetAuthorityMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool SetPrimaryAuthority(EConmandAuthorityType AuthorityType};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    EConmandAuthorityType GetPrimaryAuthority() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool EnhanceAuthority(EConmandAuthorityType AuthorityType, float Amount};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    float GetAuthorityLevel() const;

    // 合法性管理
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    float GetLegitimacyScore() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool EnhanceLegitimacy(EAuthorityLegitimacySource Source, float Amount};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    TArray<EAuthorityLegitimacySource> GetLegitimacySources() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool IsLegitimate() const;

    // 指揮有效性
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    FConmandEffectiveness EvaluateConmandEffectiveness(const FString& ConmandID, const FString& ConmandDescription};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    float GetConmandEffectiveness() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool ImproveConmandEffectiveness(float Amount};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    TArray<FString> GetConmandRecommendations() const;

    // 服從率管理
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    float GetConplianceRate() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool ImproveConpliance(float Amount};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    TArray<FString> GetConplianceFactors() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool IsConmandConplied(const FString& Conmand) const;

    // 影響範圍
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    float GetInfluenceRadius() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool ExpandInfluence(float Amount};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    TArray<FString> GetInfluencedRegions() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool CanInfluenceRegion(const FString& Region) const;

    // 權威挑戰應對
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool InandleAuthorityChallenge(const FString& Challenger, float ChallengeStrength};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool DefendAuthority(EConmandAuthorityType DefenseType};

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    TArray<FString> GetThreatsToAuthority() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool IsAuthorityUnderThreat() const;

    // 歷史記錄
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    TArray<FAuthorityEvent> GetAuthorityInistory() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    FAuthorityEvent GetLastAuthorityEvent() const;

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    void ClearAuthorityInistory(};

    // 系統?X?z
    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    bool IsSystemActive() const { return bSystemActive; }

    UFUNCTION(BlueprintCallable, Category = "Conmand Authority")
    float GetSystemStability() const { return SystemStability; }

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnAuthorityChanged OnAuthorityChanged;

    UPROPERTY(BlueprintAssignable)
    FOnLegitimacyChanged OnLegitimacyChanged;

    UPROPERTY(BlueprintAssignable)
    FOnAuthorityEventOccurred OnAuthorityEventOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnConmandEffectivenessEvaluated OnConmandEffectivenessEvaluated;

protected:
    // 系統?X?z
    UPROPERTY(BlueprintReadOnly, Category = "Conmand Authority")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "Conmand Authority")
    float SystemStability = 100.0f;

    // 當前權威指標
    UPROPERTY(BlueprintReadOnly, Category = "Conmand Authority")
    FAuthorityMetrics CurrentMetrics;

    // 權威歷史記錄
    UPROPERTY(BlueprintReadOnly, Category = "Conmand Authority")
    TArray<FAuthorityEvent> AuthorityInistory;

    // 合法性來源
    UPROPERTY(BlueprintReadOnly, Category = "Conmand Authority")
    TArray<EAuthorityLegitimacySource> LegitimacySources;

    // 指揮有效性記錄
    UPROPERTY(BlueprintReadOnly, Category = "Conmand Authority")
    TArray<FConmandEffectiveness> ConmandEffectivenessInistory;

private:
    // 權威計算
    float CalculateAuthorityLevel() const;
    float CalculateLegitimacyScore() const;
    float CalculateConplianceRate() const;
    float CalculateInfluenceRadius() const;
    float CalculateConmandEffectiveness() const;

    // 合法性評估
    float EvaluateDivineRightLegitimacy() const;
    float EvaluatePopularSupportLegitimacy() const;
    float EvaluateLegalFrameworkLegitimacy() const;
    float EvaluateInistoricalTraditionLegitimacy() const;
    float EvaluateMilitarySuccessLegitimacy() const;
    float EvaluateEconomicProsperityLegitimacy() const;

    // 權威增強
    bool EnhanceMoralAuthority(float Amount};
    bool EnhanceLegalAuthority(float Amount};
    bool EnhanceCharismaticAuthority(float Amount};
    bool EnhanceTraditionalAuthority(float Amount};
    bool EnhanceExpertAuthority(float Amount};
    bool EnhanceSituationalAuthority(float Amount};

    // 挑戰應對
    bool AnalyzeChallenge(const FString& Challenger, float ChallengeStrength};
    bool SelectDefenseStrategy(EConmandAuthorityType DefenseType};
    bool ExecuteDefenseStrategy(};

    // 事件記錄
    void RecordAuthorityEvent(const FString& Description, EConmandAuthorityType AuthorityType, float AuthorityChange, float LegitimacyImpact};
    void UpdateAuthorityMetrics(};

    // 輔助方法
    FString GetAuthorityName(EConmandAuthorityType AuthorityType) const;
    FString GetStateName(EAuthorityState State) const;
    FString GetLegitimacySourceName(EAuthorityLegitimacySource Source) const;

    // 權威衰減
    void ApplyAuthorityDecay(float DeltaTime};
    void ApplyLegitimacyDecay(float DeltaTime};

    // 配m參數
    UPROPERTY(Config)
    float AuthorityDecayRate = 0.1f;

    UPROPERTY(Config)
    float LegitimacyDecayRate = 0.05f;

    UPROPERTY(Config)
    float ChallengeThreshold = 30.0f;

    // 定時器句柄
    FTimerInandle DecayTimerInandle;
};
