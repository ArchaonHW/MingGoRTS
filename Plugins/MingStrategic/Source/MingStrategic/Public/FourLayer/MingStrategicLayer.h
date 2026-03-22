#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingStrategicLayer.generated.h"

// 戰略目標類型
UENUM(BlueprintType)
enum class EMingStrategicObjectiveType : uint8
{
    MilitaryConquest,    // 軍事征服
    PoliticalUnity,      // 政治統一
    EconomicDevelopment,  // 經濟發展
    CulturalInfluence,    // 文化影響
    TerritorialExpansion  // 領土擴張
};

// 外交關係狀態
UENUM(BlueprintType)
enum class EMingDiplomaticStatus : uint8
{
    Allied,         // 同盟
    Friendly,       // 友好
    Neutral,        // 中立
    Hostile,        // 敵對
    AtWar          // 戰爭
};

// 戰略決策類型
UENUM(BlueprintType)
enum class EMingStrategicDecisionType : uint8
{
    DeclareWar,         // 宣戰
    FormAlliance,       // 結盟
    EconomicReform,     // 經濟改革
    MilitaryExpansion,   // 軍事擴張
    CulturalPolicy      // 文化政策
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
struct MINGSTRATEGIC_API FMingStrategicObjective
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ObjectiveID;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicObjectiveType ObjectiveType;

    UPROPERTY(BlueprintReadOnly)
    FString ObjectiveDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TargetRegions;

    UPROPERTY(BlueprintReadOnly)
    float Priority;

    UPROPERTY(BlueprintReadOnly)
    float Progress;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Deadline;
};

// 戰略決策
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratLayerDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicDecisionType DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float ExpectedImpact;

    UPROPERTY(BlueprintReadOnly)
    float RiskLevel;
};

// 戰略狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CurrentStrategy;

    UPROPERTY(BlueprintReadOnly)
    float OverallStrength;

    UPROPERTY(BlueprintReadOnly)
    float EconomicPower;

    UPROPERTY(BlueprintReadOnly)
    float MilitaryPower;

    UPROPERTY(BlueprintReadOnly)
    float DiplomaticInfluence;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> TerritoryControl;
};

/**
 * 戰略層系統
 * 負責宏觀戰略決策、資源分配、外交關係等
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
    virtual void HandleLayerEvent_Implementation(const FMingGameEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // 戰略決策
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void MakeStrategicDecision(const FMingStrategicDecision& Decision);

    // 設置戰略目標
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void SetStrategicObjective(const FMingStrategicObjective& Objective);

    // 管理外交關係
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void ManageDiplomaticRelations(const FString& Faction, EMingDiplomaticStatus Status);

    // 分配戰略資源
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void AllocateStrategicResources(const TMap<EMingResourceType, float>& Allocation);

    // 評估戰略威脅
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FString> AssessStrategicThreats();

    // 識別戰略機會
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FString> IdentifyStrategicOpportunities();

    // 獲取當前戰略狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    FMingStrategicState GetCurrentStrategicState() const;

    // 獲取戰略目標進度
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FMingStrategicObjective> GetStrategicObjectives() const;

    // 獲取外交關係
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TMap<FString, EMingDiplomaticStatus> GetDiplomaticRelations() const;

protected:
    // 戰略狀態
    UPROPERTY(BlueprintReadOnly)
    FMingStrategicState StrategicState;

    // 戰略目標列表
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicObjective> StrategicObjectives;

    // 外交關係
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EMingDiplomaticStatus> DiplomaticRelations;

    // 戰略資源分配
    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> StrategicResourceAllocation;

    // 戰略事件歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicEvent> StrategicHistory;

    // 當前戰略
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    FString CurrentStrategy;

    // 戰略風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    FString StrategicStyle;

    // 攻擊性等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    float AggressivenessLevel;

    // 外交傾向
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    float DiplomaticTendency;

    // 分析戰略局勢
    void AnalyzeStrategicSituation();

    // 評估資源需求
    void AssessResourceRequirements();

    // 計算戰略風險
    float CalculateStrategicRisk(const FMingStrategicDecision& Decision);

    // 預測決策後果
    TArray<FString> PredictDecisionOutcome(const FMingStrategicDecision& Decision);

    // 民國特色戰略
    void ApplyRepublicanEraStrategy();

    // 北伐統一戰略
    void ExecuteNorthernExpeditionStrategy();

    // 抗日戰爭戰略
    void ExecuteAntiJapaneseStrategy();

    // 經濟建設戰略
    void ExecuteEconomicDevelopmentStrategy();

    // 文化影響戰略
    void ExecuteCulturalInfluenceStrategy();

private:
    // 戰略更新間隔
    float StrategicUpdateInterval;

    // 上次更新時間
    float LastStrategicUpdate;

    // 戰略威脅評估
    TMap<FString, float> StrategicThreats;

    // 戰略機會評估
    TMap<FString, float> StrategicOpportunities;

    // 戰略決策歷史
    TArray<FMingStrategicDecision> DecisionHistory;
};
