#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingStrategicLayer.generated.h"

// 戰略目標類型
UENUM(BlueprintType)
enum class EMingStrategicObjective : uint8
{
    TerritoryExpansion,     // 領土擴張
    ResourceControl,        // 資源控制
    MilitarySupremacy,      // 軍事霸權
    EconomicDominance,      // 經濟主導
    PoliticalInfluence,     // 政治影響
    TechnologicalAdvancement, // 技術進步
    CulturalSupremacy,      // 文化霸權
    DiplomaticAlliance,     // 外交聯盟
    NationalUnity,          // 國家統一
    RevolutionaryGoals      // 革命目標
};

// 外交關係狀態
UENUM(BlueprintType)
enum class EMingStrategicDiplomacy : uint8
{
    Allied,         // 同盟
    Friendly,       // 友好
    Neutral,        // 中立
    Suspicious,     // 懷疑
    Hostile,        // 敵對
    AtWar,          // 戰爭
    Vassal,         // 附庸
    Protectorate,   // 保護國
    TradePartner,   // 貿易夥伴
    Rival           // 競爭對手
};

// 戰略決策類型
UENUM(BlueprintType)
enum class EMingStrategicDecision : uint8
{
    MilitaryCampaign,   // 軍事行動
    DiplomaticTreaty,    // 外交條約
    EconomicPolicy,     // 經濟政策
    SocialReform,       // 社會改革
    TechnologicalResearch, // 技術研究
    CulturalInitiative, // 文化倡議
    InfrastructureProject, // 基礎設施項目
    IntelligenceOperation, // 情報行動
    TradeAgreement,     // 貿易協議
    AllianceFormation   // 聯盟形成
};

// 戰略事件
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedFactions;

    UPROPERTY(BlueprintReadOnly)
    float StrategicImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

// 戰略目標
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicGoal
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString GoalID;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicObjective ObjectiveType;

    UPROPERTY(BlueprintReadOnly)
    FString GoalDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> StrategicPrerequisites;

    UPROPERTY(BlueprintReadOnly)
    float Priority;

    UPROPERTY(BlueprintReadOnly)
    float Progress;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Deadline;
};

// 戰略決策
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicDecision DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float LongTermImpact;

    UPROPERTY(BlueprintReadOnly)
    float ImmediateCost;
};

/**
 * 戰略層系統
 * 負責高層決策、資源分配、外交關係等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingStrategicLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // 建構子
    UMingStrategicLayer();

    // 實現介面方法
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingStrategicIntegrationEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // 戰略目標管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void SetStrategicGoal(const FMingStrategicGoal& Goal);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void UpdateGoalProgress(const FString& GoalID, float Progress);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void CompleteGoal(const FString& GoalID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FMingStrategicGoal> GetActiveGoals() const;

    // 戰略決策
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void MakeStrategicDecision(const FMingStrategicDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void ProcessStrategicDecisions();

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FMingStrategicDecision> GetStrategicDecisions() const;

    // 外交管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void EstablishDiplomaticRelation(const FString& FactionID, EMingStrategicDiplomacy Relation);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void UpdateDiplomaticRelation(const FString& FactionID, EMingStrategicDiplomacy Relation);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    EMingStrategicDiplomacy GetDiplomaticRelation(const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TMap<FString, EMingStrategicDiplomacy> GetAllDiplomaticRelations() const;

    // 資源分配
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void AllocateResources(const TMap<EMingResourceType, float>& Allocation);

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TMap<EMingResourceType, float> GetResourceAllocation() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    float GetResourceEfficiency() const;

    // 威脅評估
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FString> IdentifyStrategicThreats() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FString> IdentifyStrategicOpportunities() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    float CalculateStrategicPosition() const;

    // 獲取戰略狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    FMingStrategicState GetStrategicState() const;

protected:
    // 戰略狀態
    UPROPERTY(BlueprintReadOnly)
    FMingStrategicState StrategicState;

    // 戰略目標
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicGoal> StrategicGoals;

    // 戰略決策
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicDecision> StrategicDecisions;

    // 外交關係
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EMingStrategicDiplomacy> DiplomaticRelations;

    // 戰略事件歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicEvent> StrategicHistory;

    // 戰略風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    FString StrategicStyle;

    // 擴張偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    float ExpansionPreference;

    // 外交偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    float DiplomacyPreference;

    // 分析戰略狀況
    void AnalyzeStrategicSituation();

    // 評估威脅
    void AssessThreats();

    // 識別機會
    void IdentifyOpportunities();

    // 計算戰略優勢
    float CalculateStrategicAdvantage();

    // 預測決策後果
    TArray<FString> PredictDecisionConsequences(const FMingStrategicDecision& Decision);

    // 民國特色戰略
    void ApplyRepublicanEraStrategy();

    // 統一戰線策略
    void ExecuteUnitedFrontStrategy();

    // 北伐統一計劃
    void ExecuteNorthernExpeditionPlan();

    // 抗日戰爭總體戰
    void ExecuteAntiJapaneseWarStrategy();

    // 新生活運動
    void ExecuteNewLifeMovement();

    // 國家建設計劃
    void ExecuteNationalConstructionPlan();

    // 經濟現代化
    void ExecuteEconomicModernization();

    // 軍事現代化
    void ExecuteMilitaryModernization();

private:
    // 戰略更新間隔
    float StrategicUpdateInterval;

    // 上次更新時間
    float LastStrategicUpdate;

    // 戰略目標上限
    int32 MaxStrategicGoals;

    // 外交關係上限
    int32 MaxDiplomaticRelations;

    // 資源分配效率
    float ResourceAllocationEfficiency;

    // 戰略威脅評估
    TMap<FString, float> StrategicThreats;

    // 戰略機會評估
    TMap<FString, float> StrategicOpportunities;

    // 戰略決策歷史
    TArray<FMingStrategicDecision> DecisionHistory;
};
