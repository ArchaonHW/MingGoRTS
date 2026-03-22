#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFourLayerIntegrationManager.generated.h"

// 層級類型
UENUM(BlueprintType)
enum class EMingLayer : uint8
{
    Strategic,      // 戰略層
    Tactical,       // 戰術層
    Personal,       // 個人層
    Building        // 建設層
};

// 資源類型
UENUM(BlueprintType)
enum class EMingBuildingResourceType : uint8
{
    Gold,           // 黃金
    Food,           // 糧食
    Wood,           // 木材
    Stone,          // 石頭
    Iron,           // 鐵礦
    Coal,           // 煤炭
    Silk,           // 絲綢
    Tea,            // 茶葉
    Porcelain,      // 瓷器
    Spices          // 香料
};

// 歷史時期
UENUM(BlueprintType)
enum class EMingHistoricalPeriod : uint8
{
    EarlyRepublic,  // 民國早期 (1912-1928)
    NanjingDecade,  // 南京十年 (1928-1937)
    WarOfResistance, // 抗戰時期 (1937-1945)
    CivilWar        // 內戰時期 (1945-1949)
};

// 全局資源狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingGlobalResourceState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> ResourceAmounts;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> ResourceProduction;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> ResourceConsumption;

    UPROPERTY(BlueprintReadOnly)
    float TotalResourceValue;
};

// 戰略狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CurrentStrategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> StrategicObjectives;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> DiplomaticRelations;

    UPROPERTY(BlueprintReadOnly)
    float PoliticalStability;

    UPROPERTY(BlueprintReadOnly)
    float NationalMorale;
};

// 戰術狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingTacticalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> MilitaryUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FVector> ControlledTerritories;

    UPROPERTY(BlueprintReadOnly)
    TArray<FVector> Battlefronts;

    UPROPERTY(BlueprintReadOnly)
    float MilitaryStrength;

    UPROPERTY(BlueprintReadOnly)
    float CombatReadiness;
};

// 個人狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* MainCharacter;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> CharacterRelationships;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PersonalSkills;

    UPROPERTY(BlueprintReadOnly)
    float CharacterReputation;

    UPROPERTY(BlueprintReadOnly)
    float PersonalInfluence;
};

// 建設狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingBuildingState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> ConstructedBuildings;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> ConstructionQueue;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> BuildingLevels;

    UPROPERTY(BlueprintReadOnly)
    float InfrastructureLevel;

    UPROPERTY(BlueprintReadOnly)
    float EconomicOutput;
};

// 層級協調狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingLayerCoordinationState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingLayer, TMap<EMingLayer, float>> LayerRelations;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingLayer, TMap<EMingResourceType, float>> ResourceFlows;

    UPROPERTY(BlueprintReadOnly)
    TArray<struct FMingLayerConflict> ActiveConflicts;

    UPROPERTY(BlueprintReadOnly)
    float OverallCoordinationEfficiency;
};

// 層級衝突
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingLayerConflict
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMingLayer ConflictLayer1;

    UPROPERTY(BlueprintReadOnly)
    EMingLayer ConflictLayer2;

    UPROPERTY(BlueprintReadOnly)
    FString ConflictType;

    UPROPERTY(BlueprintReadOnly)
    float ConflictSeverity;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ResolutionOptions;
};

// 遊戲事件
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingFourLayerGameEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    EMingLayer SourceLayer;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingLayer> TargetLayers;

    UPROPERTY(BlueprintReadOnly)
    float EventPriority;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

// 遊戲決策
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingGameDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    EMingLayer SourceLayer;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingLayer> AffectedLayers;

    UPROPERTY(BlueprintReadOnly)
    float DecisionPriority;
};

// 全局狀態
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingGlobalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FDateTime CurrentGameTime;

    UPROPERTY(BlueprintReadOnly)
    EMingHistoricalPeriod CurrentPeriod;

    UPROPERTY(BlueprintReadOnly)
    FMingGlobalResourceState ResourceState;

    UPROPERTY(BlueprintReadOnly)
    FMingStrategicState StrategicState;

    UPROPERTY(BlueprintReadOnly)
    FMingTacticalState TacticalState;

    UPROPERTY(BlueprintReadOnly)
    FMingPersonalState PersonalState;

    UPROPERTY(BlueprintReadOnly)
    FMingBuildingState BuildingState;

    UPROPERTY(BlueprintReadOnly)
    FMingLayerCoordinationState CoordinationState;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingGameEvent> GlobalEvents;
};

/**
 * 四層策略整合管理器
 * 負責協調戰略、戰術、個人、建設四層系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingFourLayerIntegrationManager : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingFourLayerIntegrationManager();

    // 初始化四層系統
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void InitializeFourLayerSystem();

    // 更新四層系統
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void UpdateFourLayerSystem(float DeltaTime);

    // 層級間協調
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void CoordinateLayerInteractions();

    // 全局狀態同步
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void SynchronizeGlobalState();

    // 層級權重管理
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void ManageLayerPriorities();

    // 衝突解決
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void ResolveLayerConflicts();

    // 獲取全局狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    FMingGlobalState GetGlobalState() const { return GlobalState; }

    // 獲取特定層級狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    bool GetLayerState(EMingLayer Layer, FMingStrategicState& OutStrategicState, 
                     FMingTacticalState& OutTacticalState, FMingPersonalState& OutPersonalState, 
                     FMingBuildingState& OutBuildingState);

    // 處理戰略層級事件
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void ProcessGameEvent(const FMingStrategicLayerEvent& Event);

    // 執行建築戰略決策
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void ExecuteGameDecision(const FMingBuildingStrategicDecision& Decision);

    // 性能監控
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void MonitorLayerPerformance();

    // 獲取協調效率
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    float GetCoordinationEfficiency() const;

    // 重置系統
    UFUNCTION(BlueprintCallable, Category = "Ming|FourLayer")
    void ResetSystem();

protected:
    // 全局狀態
    UPROPERTY(BlueprintReadOnly)
    FMingGlobalState GlobalState;

    // 層級權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FourLayer|Settings")
    TMap<EMingLayer, float> LayerWeights;

    // 協調間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FourLayer|Settings")
    float CoordinationInterval;

    // 是否已初始化
    UPROPERTY(BlueprintReadOnly)
    bool bInitialized;

    // 累計時間
    float AccumulatedTime;

    // 初始化層級系統
    void InitializeLayerSystems();

    // 更新層級狀態
    void UpdateLayerStates(float DeltaTime);

    // 協調資源分配
    void CoordinateResourceAllocation();

    // 協調事件傳播
    void CoordinateEventPropagation();

    // 協調決策執行
    void CoordinateDecisionExecution();

    // 檢測層級衝突
    void DetectLayerConflicts();

    // 解決資源衝突
    void ResolveResourceConflicts();

    // 解決決策衝突
    void ResolveDecisionConflicts();

    // 更新協調效率
    void UpdateCoordinationEfficiency();

private:
    // 層級系統引用
    TArray<TSharedPtr<class IMingGameLayer>> LayerSystems;

    // 事件隊列
    TArray<FMingGameEvent> EventQueue;

    // 決策隊列
    TArray<FMingGameDecision> DecisionQueue;

    // 性能統計
    TMap<EMingLayer, float> LayerPerformance;
};
