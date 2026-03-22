#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingTacticalLayer.generated.h"

// 戰術單位狀態
UENUM(BlueprintType)
enum class EMingStratTacticalUnitState : uint8
{
    Idle,           // 待命
    Moving,         // 移動中
    Attacking,      // 攻擊中
    Defending,      // 防禦中
    Retreating,     // 撤退中
    Regrouping,     // 重組中
    Captured,       // 被俘
    Destroyed       // 摧毀
};

// 戰術地形類型
UENUM(BlueprintType)
enum class EMingStratTacticalTerrain : uint8
{
    Plains,         // 平原
    Mountains,      // 山地
    Forest,         // 森林
    Desert,         // 沙漠
    River,          // 河流
    Urban,          // 城市
    Coastal,        // 海岸
    Swamp,          // 沼澤
    Tundra,         // 凍土
    Jungle          // 叢林
};

// 戰術天氣狀況
UENUM(BlueprintType)
enum class EMingStratTacticalWeather : uint8
{
    Clear,          // 晴朗
    Cloudy,         // 多雲
    Rain,           // 下雨
    Snow,           // 下雪
    Fog,            // 霧
    Storm,          // 暴風雨
    Windy,          // 大風
    Extreme         // 極端天氣
};

// 戰術命令類型
UENUM(BlueprintType)
enum class EMingStratTacticalCommand : uint8
{
    Move,           // 移動
    Attack,         // 攻擊
    Defend,         // 防禦
    Retreat,        // 撤退
    Hold,           // 堅守
    Patrol,         // 巡邏
    Ambush,         // 伏擊
    Flank,          // 側翼攻擊
    Charge,         // 衝鋒
    Skirmish        // 騷擾
};

// 戰術單位信息
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratTacticalUnit
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* UnitActor;

    UPROPERTY(BlueprintReadOnly)
    FString UnitID;

    UPROPERTY(BlueprintReadOnly)
    FString UnitName;

    UPROPERTY(BlueprintReadOnly)
    EMingStratTacticalUnitState CurrentState;

    UPROPERTY(BlueprintReadOnly)
    FVector Position;

    UPROPERTY(BlueprintReadOnly)
    float Health;

    UPROPERTY(BlueprintReadOnly)
    float Morale;

    UPROPERTY(BlueprintReadOnly)
    float AttackPower;

    UPROPERTY(BlueprintReadOnly)
    float DefensePower;

    UPROPERTY(BlueprintReadOnly)
    float MovementSpeed;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CurrentOrders;
};

// 戰術戰場信息
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratTacticalBattlefield
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString BattlefieldID;

    UPROPERTY(BlueprintReadOnly)
    FString BattlefieldName;

    UPROPERTY(BlueprintReadOnly)
    FVector CenterLocation;

    UPROPERTY(BlueprintReadOnly)
    float BattlefieldRadius;

    UPROPERTY(BlueprintReadOnly)
    EMingStratTacticalTerrain TerrainType;

    UPROPERTY(BlueprintReadOnly)
    EMingStratTacticalWeather WeatherCondition;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStratTacticalUnit> AlliedUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStratTacticalUnit> EnemyUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> StrategicObjectives;
};

// 戰術命令
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratTacticalOrder
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString OrderID;

    UPROPERTY(BlueprintReadOnly)
    EMingStratTacticalCommand CommandType;

    UPROPERTY(BlueprintReadOnly)
    FString IssuingUnitID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TargetUnitIDs;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly)
    FString OrderDescription;

    UPROPERTY(BlueprintReadOnly)
    float Priority;

    UPROPERTY(BlueprintReadOnly)
    FDateTime IssueTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ExpectedCompletion;
};

// 戰術事件
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratTacticalEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    FString BattlefieldID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedUnits;

    UPROPERTY(BlueprintReadOnly)
    float TacticalImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

/**
 * 戰術層系統
 * 負責戰場管理、部隊指揮、戰術執行等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingTacticalLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // 建構子
    UMingTacticalLayer();

    // 實現介面方法
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingStrategicIntegrationEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // 戰場管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void CreateBattlefield(const FString& BattlefieldID, const FString& BattlefieldName, const FVector& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void UpdateBattlefieldConditions(const FString& BattlefieldID, EMingStratTacticalTerrain Terrain, EMingStratTacticalWeather Weather);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    FMingStratTacticalBattlefield GetBattlefield(const FString& BattlefieldID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FMingStratTacticalBattlefield> GetAllBattlefields() const;

    // 部隊管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void AddTacticalUnit(const FString& UnitID, const FString& UnitName, const FVector& Position);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void UpdateTacticalUnit(const FString& UnitID, const FVector& Position, EMingStratTacticalUnitState State);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FMingStratTacticalUnit> GetTacticalUnits() const;

    // 戰術命令
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void IssueTacticalOrder(const FMingStratTacticalOrder& Order);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void ExecuteTacticalOrders();

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void CancelTacticalOrder(const FString& OrderID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FMingStratTacticalOrder> GetTacticalOrders() const;

    // 戰術分析
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    float CalculateBattlefieldControl(const FString& BattlefieldID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> IdentifyTacticalOpportunities() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> AssessTacticalThreats() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    float CalculateUnitCoordination() const;

    // 獲取戰術狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    FMingTacticalState GetTacticalState() const;

protected:
    // 戰術狀態
    UPROPERTY(BlueprintReadOnly)
    FMingTacticalState TacticalState;

    // 戰場列表
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingStratTacticalBattlefield> Battlefields;

    // 戰術單位
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingStratTacticalUnit> TacticalUnits;

    // 戰術命令
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStratTacticalOrder> TacticalOrders;

    // 戰術事件歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStratTacticalEvent> TacticalHistory;

    // 戰術風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    FString TacticalStyle;

    // 戰鬥偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float CombatAggressiveness;

    // 部隊協調偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float UnitCoordinationPreference;

    // 分析戰場狀況
    void AnalyzeBattlefieldConditions();

    // 評估部隊狀態
    void AssessUnitStatus();

    // 計算戰術優勢
    float CalculateTacticalAdvantage();

    // 預測戰鬥結果
    TArray<FString> PredictBattleOutcome(const FString& BattlefieldID);

    // 民國特色戰術
    void ApplyRepublicanEraTactics();

    // 軍閥混戰協調
    void ExecuteWarlordConflictCoordination();

    // 現代化軍事改革
    void ExecuteModernMilitaryReform();

    // 游擊戰術
    void ExecuteGuerrillaWarfare();

    // 正規戰術
    void ExecuteConventionalWarfare();

    // 統一戰線戰術
    void ExecuteUnitedFrontTactics();

    // 抗日戰爭戰術
    void ExecuteAntiJapaneseWarTactics();

private:
    // 戰術更新間隔
    float TacticalUpdateInterval;

    // 上次更新時間
    float LastTacticalUpdate;

    // 戰場控制閾值
    float BattlefieldControlThreshold;

    // 單位協調半徑
    float UnitCoordinationRadius;

    // 戰術威脅評估
    TMap<FString, float> TacticalThreats;

    // 戰術機會評估
    TMap<FString, float> TacticalOpportunities;

    // 戰術決策歷史
    TArray<FMingStratTacticalOrder> OrderHistory;
};
