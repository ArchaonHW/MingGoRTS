#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingTacticalLayer.generated.h"

// 戰術單位狀態
UENUM(BlueprintType)
enum class EMingStrategicTacticalUnitState : uint8
{
    Idle,           // 待命
    Moving,         // 移動中
    Attacking,      // 攻擊中
    Defending,      // 防禦中
    Retreating,     // 撤退中
    Regrouping      // 重組中
};

// 戰術地形類型
UENUM(BlueprintType)
enum class EMingTacticalTerrain : uint8
{
    Plains,         // 平原
    Mountains,      // 山地
    Forest,         // 森林
    River,          // 河流
    Urban,          // 城市
    Desert          // 沙漠
};

// 戰術天氣狀況
UENUM(BlueprintType)
enum class EMingTacticalWeather : uint8
{
    Clear,          // 晴朗
    Cloudy,         // 多雲
    Rainy,          // 下雨
    Snowy,          // 下雪
    Foggy,          // 有霧
    Stormy          // 暴風雨
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
    Flank           // 側翼包抄
};

// 戰術單位信息
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicTacticalUnit
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* UnitActor;

    UPROPERTY(BlueprintReadOnly)
    FString UnitID;

    UPROPERTY(BlueprintReadOnly)
    FString UnitType;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicTacticalUnitState CurrentState;

    UPROPERTY(BlueprintReadOnly)
    FVector CurrentPosition;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetPosition;

    UPROPERTY(BlueprintReadOnly)
    float Health;

    UPROPERTY(BlueprintReadOnly)
    float Morale;

    UPROPERTY(BlueprintReadOnly)
    float CombatEffectiveness;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingStratTacticalCommand> AvailableCommands;
};

// 戰術戰場信息
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingTacticalBattlefield
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString BattlefieldID;

    UPROPERTY(BlueprintReadOnly)
    FString BattlefieldName;

    UPROPERTY(BlueprintReadOnly)
    FVector BattlefieldCenter;

    UPROPERTY(BlueprintReadOnly)
    float BattlefieldRadius;

    UPROPERTY(BlueprintReadOnly)
    EMingTacticalTerrain TerrainType;

    UPROPERTY(BlueprintReadOnly)
    EMingTacticalWeather WeatherCondition;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicTacticalUnit> FriendlyUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicTacticalUnit> EnemyUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<FVector> StrategicPoints;

    UPROPERTY(BlueprintReadOnly)
    float BattlefieldControl;
};

// 戰術命令
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicTacticalOrder
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString OrderID;

    UPROPERTY(BlueprintReadOnly)
    EMingStratTacticalCommand CommandType;

    UPROPERTY(BlueprintReadOnly)
    FString TargetUnitID;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly)
    float OrderPriority;

    UPROPERTY(BlueprintReadOnly)
    FDateTime IssueTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Deadline;
};

// 戰術事件
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingTacticalEvent
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
 * 負責戰場指揮、部隊協調、戰術執行等
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
    void CreateBattlefield(const FString& BattlefieldID, const FVector& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void UpdateBattlefield(const FString& BattlefieldID, EMingTacticalTerrain Terrain, EMingTacticalWeather Weather);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    FMingTacticalBattlefield GetBattlefield(const FString& BattlefieldID) const;

    // 部隊管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void AddTacticalUnit(class AActor* Unit, const FString& UnitID, const FString& UnitType);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void RemoveTacticalUnit(const FString& UnitID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void UpdateTacticalUnit(const FString& UnitID, const FVector& Position, EMingTacticalUnitState State);

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
    float AnalyzeBattlefieldControl(const FString& BattlefieldID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FVector> IdentifyStrategicPoints(const FString& BattlefieldID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> AssessTacticalThreats(const FString& BattlefieldID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> IdentifyTacticalOpportunities(const FString& BattlefieldID);

    // 獲取戰術狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    FMingTacticalState GetTacticalState() const;

    // 獲取戰場列表
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> GetBattlefieldList() const;

protected:
    // 戰術狀態
    UPROPERTY(BlueprintReadOnly)
    FMingTacticalState TacticalState;

    // 戰場列表
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingTacticalBattlefield> Battlefields;

    // 戰術單位
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingStratTacticalUnit> TacticalUnits;

    // 戰術命令
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStratTacticalOrder> TacticalOrders;

    // 戰術事件歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingTacticalEvent> TacticalHistory;

    // 戰術風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    FString TacticalStyle;

    // 攻擊性等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float TacticalAggressiveness;

    // 謹慎性等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float TacticalCautiousness;

    // 協調能力
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float CoordinationSkill;

    // 分析戰場局勢
    void AnalyzeBattlefieldSituation(const FString& BattlefieldID);

    // 評估部隊狀態
    void AssessUnitStatus();

    // 計算戰術優勢
    float CalculateTacticalAdvantage(const FString& BattlefieldID);

    // 預測戰術結果
    TArray<FString> PredictTacticalOutcome(const FString& BattlefieldID);

    // 民國特色戰術
    void ApplyRepublicanEraTactics();

    // 軍閥混戰戰術
    void ExecuteWarlordConflictTactics();

    // 北伐戰術
    void ExecuteNorthernExpeditionTactics();

    // 抗日戰術
    void ExecuteAntiJapaneseTactics();

    // 游擊戰術
    void ExecuteGuerrillaTactics();

    // 正規戰術
    void ExecuteConventionalTactics();

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

    // 戰術命令歷史
    TArray<FMingTacticalOrder> OrderHistory;
};
