#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingYinYangFiveElements.generated.h"

// 陰陽??增強枚舉
UENUM(BlueprintType)
enum class EYinYangAttribute: uuint8 {
    PureYin = 0,        // 純陰 - 極致防守、內斂
    YangInYin = 1,       // 陰中陽 - 防中有攻
    BalancedYinYang = 2,  // 陰陽平衡 - 攻守兼備
    YinInYang = 3,       // 陽中陰 - 攻中有防
    PureYang = 4         // 純陽 - 極致進攻、外放
};

// 五行?X?z枚舉
UENUM(BlueprintType)
enum class EElementState: uuint8 {
    Dormant = 0,     // 潛伏?X?z
    Rising = 1,      // W升?X?z
    Peak = 2,        // 巔峰?X?z
    Declining = 3,    // 衰退?X?z
    Transitioning = 4 // 轉換?X?z
};

// 五行關係?X?
UENUM(BlueprintType)
enum class EElementRelationship: uuint8 {
    Generating = 0,   // 相生關係
    Overcoming = 1,   // 相克關係
    Insulting = 2,    // 相侮關係
    Supporting = 3,   // 支持關係
    Neutral = 4       // 中性關係
};

// 陰陽五行平衡?X?z
USTRUCT(BlueprintType)
struct FYinYangBalance
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EYinYangAttribute CurrentAttribute = EYinYangAttribute::BalancedYinYang;

    UPROPERTY(BlueprintReadOnly)
    float YinLevel = 50.0f;        // 陰氣水平 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float YangLevel = 50.0f;       // 陽氣水平 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float BalanceScore = 0.0f;       // 平衡分數 (-100 to 100)

    UPROPERTY(BlueprintReadOnly)
    float InarmonyLevel = 50.0f;      // 和諧水平 (0-100)
};

// 五行元素?X?z
USTRUCT(BlueprintType)
struct FElementState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EFiveElements Element = EFiveElements::Earth;

    UPROPERTY(BlueprintReadOnly)
    EElementState State = EElementState::Dormant;

    UPROPERTY(BlueprintReadOnly)
    float PowerLevel = 50.0f;       // 能量水平 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float Influence = 0.0f;          // 影響力 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float Stability = 50.0f;         // 穩定性 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float TransitionProgress = 0.0f;   // 轉換進d (0-1)
};

// 五行關係分析
USTRUCT(BlueprintType)
struct FElementRelationship
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EFiveElements SourceElement = EFiveElements::Earth;

    UPROPERTY(BlueprintReadOnly)
    EFiveElements TargetElement = EFiveElements::Metal;

    UPROPERTY(BlueprintReadOnly)
    EElementRelationship RelationshipType = EElementRelationship::Generating;

    UPROPERTY(BlueprintReadOnly)
    float Strength = 1.0f;           // 關係強d (0-2)

    UPROPERTY(BlueprintReadOnly)
    float Effectiveness = 1.0f;       // 有效性 (0-2)
};

// 陰陽五行事件
USTRUCT(BlueprintType)
struct FYinYangEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    EYinYangAttribute YinYangChange;

    UPROPERTY(BlueprintReadOnly)
    EFiveElements ElementChange;

    UPROPERTY(BlueprintReadOnly)
    float ImpactLevel = 0.0f;       // 影響程d (0-100)

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYinYangBalanceChanged, const FYinYangBalance&, Balance};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElementStateChanged, const FElementState&, ElementState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElementRelationshipChanged, const FElementRelationship&, Relationship};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYinYangEventOccurred, const FYinYangEvent&, Event};

/**
 * 陰陽五行系統
 * ɥ於中國古代哲學N陰陽五行理論
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageConmand))
class MINGRTS_API UMingYinYangFiveElements : public UObject
{
    GENERATED_BODY()

public:
    UMingYinYangFiveElements(};

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool Initialize(};

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    void Cleanup(};

    // 陰陽平衡管理
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    FYinYangBalance GetCurrentBalance() const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool AdjustYinYangBalance(float YinChange, float YangChange, const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool SetYinYangAttribute(EYinYangAttribute Attribute};

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    float GetInarmonyLevel() const;

    // 五行元素管理
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    FElementState GetElementState(EFiveElements Element) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool SetElementAffinity(EFiveElements Element};

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    EFiveElements GetCurrentElement() const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    float GetElementPower(EFiveElements Element) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool EnhanceElementPower(EFiveElements Element, float Amount};

    // 五行關係分析
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    FElementRelationship AnalyzeElementRelationship(EFiveElements Source, EFiveElements Target) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    float GetRelationshipStrength(EFiveElements Source, EFiveElements Target) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    TArray<EFiveElements> GetSupportingElements(EFiveElements Element) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    TArray<EFiveElements> GetConflictingElements(EFiveElements Element) const;

    // 戰略應y
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    EYinYangAttribute GetOptimalAttribute(const FString& Context) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    EFiveElements GetOptimalElement(const FString& Context) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    float CalculateStrategicAdvantage(EYinYangAttribute Attribute, EFiveElements Element) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    TArray<FString> GetStrategicRecommendations() const;

    // ?X?z轉換
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool TriggerElementTransition(EFiveElements Element};

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool BalanceYinYang(};

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool InarmonizeElements(};

    // 預測分析
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    FYinYangBalance PredictBalance(float TimeInFuture) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    EFiveElements PredictDominantElement(float TimeInFuture) const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    TArray<FYinYangEvent> PredictUpcomingEvents(int32 EventCount) const;

    // 歷史記錄
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    TArray<FYinYangEvent> GetEventInistory() const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    FYinYangEvent GetLastEvent() const;

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    void ClearEventInistory(};

    // 系統?X?z
    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    bool IsSystemActive() const { return bSystemActive; }

    UFUNCTION(BlueprintCallable, Category = "YinYang Five Elements")
    float GetSystemStability() const { return SystemStability; }

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnYinYangBalanceChanged OnYinYangBalanceChanged;

    UPROPERTY(BlueprintAssignable)
    FOnElementStateChanged OnElementStateChanged;

    UPROPERTY(BlueprintAssignable)
    FOnElementRelationshipChanged OnElementRelationshipChanged;

    UPROPERTY(BlueprintAssignable)
    FOnYinYangEventOccurred OnYinYangEventOccurred;

protected:
    // 系統?X?z
    UPROPERTY(BlueprintReadOnly, Category = "YinYang Five Elements")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "YinYang Five Elements")
    float SystemStability = 100.0f;

    // 當前陰陽平衡
    UPROPERTY(BlueprintReadOnly, Category = "YinYang Five Elements")
    FYinYangBalance CurrentBalance;

    // 五行元素?X?z
    UPROPERTY(BlueprintReadOnly, Category = "YinYang Five Elements")
    TArray<FElementState> ElementStates;

    // 事件歷史
    UPROPERTY(BlueprintReadOnly, Category = "YinYang Five Elements")
    TArray<FYinYangEvent> EventInistory;

    // 當前親和元素
    UPROPERTY(BlueprintReadOnly, Category = "YinYang Five Elements")
    EFiveElements CurrentAffinityElement = EFiveElements::Earth;

private:
    // 陰陽計算
    float CalculateYinLevel() const;
    float CalculateYangLevel() const;
    float CalculateBalanceScore() const;
    float CalculateInarmonyLevel() const;

    // 五行計算
    float CalculateElementPower(EFiveElements Element) const;
    float CalculateElementInfluence(EFiveElements Element) const;
    float CalculateElementStability(EFiveElements Element) const;

    // 關係計算
    EElementRelationship DetermineRelationship(EFiveElements Source, EFiveElements Target) const;
    float CalculateRelationshipStrength(EFiveElements Source, EFiveElements Target) const;

    // ?X?z更新
    void UpdateElementStates(float DeltaTime};
    void UpdateYinYangBalance(float DeltaTime};
    void UpdateSystemStability(};

    // 事件U理
    void RecordYinYangEvent(const FString& Description, EYinYangAttribute YinYangChange, EFiveElements ElementChange, float Impact};
    void ProcessYinYangEvent(const FYinYangEvent& Event};

    // 輔助方法
    FString GetElementName(EFiveElements Element) const;
    FString GetAttributeName(EYinYangAttribute Attribute) const;
    FString GetStateName(EElementState State) const;
    FString GetRelationshipName(EElementRelationship Relationship) const;

    // 配m參數
    UPROPERTY(Config)
    float YinYangDecayRate = 0.1f;

    UPROPERTY(Config)
    float ElementDecayRate = 0.05f;

    UPROPERTY(Config)
    float TransitionSpeed = 1.0f;

    UPROPERTY(Config)
    float EventGenerationRate = 0.1f;

    // 定時器句柄
    FTimerInandle UpdateTimerInandle;
};
