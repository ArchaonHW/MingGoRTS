#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingTacticalLayer.generated.h"

// ?∞Ë??Æ‰??Ä??UENUM(BlueprintType)
enum class EMingStrategicTacticalUnitState : uint8
{
    Idle,           // ÂæÖÂëΩ
    Moving,         // ÁßªÂ?‰∏?    Attacking,      // ?ªÊ?‰∏?    Defending,      // ?≤Á¶¶‰∏?    Retreating,     // ?§ÈÄÄ‰∏?    Regrouping      // ?çÁ?‰∏?};

// ?∞Ë??∞ÂΩ¢È°ûÂ?
UENUM(BlueprintType)
enum class EMingTacticalTerrain : uint8
{
    Plains,         // Âπ≥Â?
    Mountains,      // Â±±Âú∞
    Forest,         // Ê£ÆÊ?
    River,          // Ê≤≥Ê?
    Urban,          // ?éÂ?
    Desert          // Ê≤ôÊ?
};

// ?∞Ë?Â§©Ê∞£?ÄÊ≥?UENUM(BlueprintType)
enum class EMingTacticalWeather : uint8
{
    Clear,          // ?¥Ê?
    Cloudy,         // Â§öÈõ≤
    Rainy,          // ‰∏ãÈõ®
    Snowy,          // ‰∏ãÈõ™
    Foggy,          // ?âÈúß
    Stormy          // ?¥È¢®??};

// ?∞Ë??Ω‰ª§È°ûÂ?
UENUM(BlueprintType)
enum class EMingStratTacticalCommand : uint8
{
    Move,           // ÁßªÂ?
    Attack,         // ?ªÊ?
    Defend,         // ?≤Á¶¶
    Retreat,        // ?§ÈÄÄ
    Hold,           // ?ÖÂ?
    Patrol,         // Â∑°È?
    Ambush,         // ‰ºèÊ?
    Flank           // ?¥Áøº?ÖÊ?
};

// ?∞Ë??Æ‰?‰ø°ÊÅØ
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

// ?∞Ë??∞Â†¥‰ø°ÊÅØ
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

// ?∞Ë??Ω‰ª§
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

// ?∞Ë?‰∫ã‰ª∂
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
    FDateTime EventTime;
};

// ?∞Ë??Æ‰??Ä??UENUM(BlueprintType)
enum class EMingStrategicUnitState : uint8
{
    Idle,           // ÂæÖÂëΩ
    Moving,         // ÁßªÂ?‰∏?    Attacking,      // ?ªÊ?‰∏?    Defending,      // ?≤Á¶¶‰∏?    Retreating,     // ?§ÈÄÄ‰∏?    Regrouping      // ?çÁ?‰∏?};

// ?∞Ë??Æ‰?‰ø°ÊÅØ
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratUnit
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* UnitActor;

    UPROPERTY(BlueprintReadOnly)
    FString UnitID;

    UPROPERTY(BlueprintReadOnly)
    FString UnitType;

    UPROPERTY(BlueprintReadOnly)
    EMingTacticalUnitState CurrentState;

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
};

// ?∞Ë??Ω‰ª§
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratOrder
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
    float Priority;
};

// ?∞Ë??Ä??USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CurrentTactic;

    UPROPERTY(BlueprintReadOnly)
    float OverallSituation;

    UPROPERTY(BlueprintReadOnly)
    float FriendlyStrength;

    UPROPERTY(BlueprintReadOnly)
    float EnemyStrength;

    UPROPERTY(BlueprintReadOnly)
    float TerrainAdvantage;

    UPROPERTY(BlueprintReadOnly)
    float WeatherImpact;
};

// ?∞Ë?‰∫ã‰ª∂
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
    float EventImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

/**
 * ?∞Ë?Â±§Á≥ªÁµ? * Ë≤†Ë≤¨?∞Â†¥?áÊèÆ?ÅÈÉ®?äË™øÂ∫¶„ÄÅÊà∞Ë°ìÂü∑Ë°åÁ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingTacticalLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingTacticalLayer();

    // ÂØ¶Áèæ‰ªãÈù¢?πÊ?
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingStrategicIntegrationEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // ?∞Â†¥ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void CreateBattlefield(const FString& BattlefieldID, const FVector& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void UpdateBattlefield(const FString& BattlefieldID, EMingTacticalTerrain Terrain, EMingTacticalWeather Weather);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    FMingTacticalBattlefield GetBattlefield(const FString& BattlefieldID) const;

    // ?®È?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void AddTacticalUnit(class AActor* Unit, const FString& UnitID, const FString& UnitType);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void RemoveTacticalUnit(const FString& UnitID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void UpdateTacticalUnit(const FString& UnitID, const FVector& Position, EMingTacticalUnitState State);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FMingStratTacticalUnit> GetTacticalUnits() const;

    // ?∞Ë??Ω‰ª§
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void IssueTacticalOrder(const FMingStratTacticalOrder& Order);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void ExecuteTacticalOrders();

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    void CancelTacticalOrder(const FString& OrderID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FMingStratTacticalOrder> GetTacticalOrders() const;

    // ?∞Ë??ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    float AnalyzeBattlefieldControl(const FString& BattlefieldID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FVector> IdentifyStrategicPoints(const FString& BattlefieldID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> AssessTacticalThreats(const FString& BattlefieldID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> IdentifyTacticalOpportunities(const FString& BattlefieldID);

    // ?≤Â??∞Ë??Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    FMingTacticalState GetTacticalState() const;

    // ?≤Â??∞Â†¥?óË"
    UFUNCTION(BlueprintCallable, Category = "Ming|Tactical")
    TArray<FString> GetBattlefieldList() const;

protected:
    // ?∞Ë??Ä??    UPROPERTY(BlueprintReadOnly)
    FMingTacticalState TacticalState;

    // ?∞Â†¥?óË"
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingTacticalBattlefield> Battlefields;

    // ?∞Ë??Æ‰?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingStratTacticalUnit> TacticalUnits;

    // ?∞Ë??Ω‰ª§
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStratTacticalOrder> TacticalOrders;

    // ?∞Ë?‰∫ã‰ª∂Ê≠∑Âè≤
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingTacticalEvent> TacticalHistory;

    // ?∞Ë?È¢®Ê†º
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    FString TacticalStyle;

    // ?ªÊ??ßÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float TacticalAggressiveness;

    // Ë¨πÊ??ßÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float TacticalCautiousness;

    // ?îË™ø?ΩÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical|Settings")
    float CoordinationSkill;

    // ?ÜÊ??∞Â†¥Â±Ä??    void AnalyzeBattlefieldSituation(const FString& BattlefieldID);

    // Ë©ï‰º∞?®È??Ä??    void AssessUnitStatus();

    // Ë®àÁ??∞Ë??™Âã¢
    float CalculateTacticalAdvantage(const FString& BattlefieldID);

    // ?êÊ∏¨?∞Ë?ÁµêÊ?
    TArray<FString> PredictTacticalOutcome(const FString& BattlefieldID);

    // Ê∞ëÂ??πËâ≤?∞Ë?
    void ApplyRepublicanEraTactics();

    // ËªçÈñ•Ê∑∑Êà∞?∞Ë?
    void ExecuteWarlordConflictTactics();

    // ?ó‰??∞Ë?
    void ExecuteNorthernExpeditionTactics();

    // ?óÊó•?∞Ë?
    void ExecuteAntiJapaneseTactics();

    // Ê∏∏Ê??∞Ë?
    void ExecuteGuerrillaTactics();

    // Ê≠???∞Ë?
    void ExecuteConventionalTactics();

private:
    // ?∞Ë??¥Êñ∞?ìÈ?
    float TacticalUpdateInterval;

    // ‰∏äÊ¨°?¥Êñ∞?ÇÈ?
    float LastTacticalUpdate;

    // ?∞Â†¥?ßÂà∂?æÂÄ?    float BattlefieldControlThreshold;

    // ?Æ‰??îË™ø?äÂ?
    float UnitCoordinationRadius;

    // ?∞Ë?Â®ÅË?Ë©ï‰º∞
    TMap<FString, float> TacticalThreats;

    // ?∞Ë?Ê©üÊ?Ë©ï‰º∞
    TMap<FString, float> TacticalOpportunities;

    // ?∞Ë??Ω‰ª§Ê≠∑Âè≤
    TArray<FMingTacticalOrder> OrderHistory;
};