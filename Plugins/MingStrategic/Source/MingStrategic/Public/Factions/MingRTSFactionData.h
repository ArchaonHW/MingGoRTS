#pragma once

#include "CoreMinimal.h"
#include "Factions/MingRTSFactionTypes.h"
#include "MingRTSFactionData.generated.h"

/**
 * 勢力獨特單位數據
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FUniqueUnitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUniqueUnitType UnitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UTexture2D> UnitIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BaseDefense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class AMingRTSUnitBase>> UnitClassVariations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EFactionSpecialMechanic, float> SpecialAbilities; // 特殊能力: 效果強度

    FUniqueUnitData()
        : UnitType(EUniqueUnitType::Count)
        , BaseHealth(100)
        , BaseAttack(10)
        , BaseDefense(5)
        , MovementSpeed(300.0f)
        , AttackRange(100.0f)
    {}
};

/**
 * 勢力特色機制數據
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FFactionMechanicData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionSpecialMechanic MechanicType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MechanicName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectMultiplier; // 效果倍數

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnlockLevel; // 解鎖等級

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RelatedTechnologies; // 相關科技

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPassive; // 是否被動生效

    FFactionMechanicData()
        : MechanicType(EFactionSpecialMechanic::Count)
        , EffectMultiplier(1.0f)
        , UnlockLevel(1)
        , bIsPassive(true)
    {}
};

/**
 * 勢力起始配置
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FFactionStartingConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EStartingRegion PrimaryRegion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EStartingRegion> SecondaryRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartingYear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartingGold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartingPopulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartingUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartingBuildings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<class AMingRTSBuildingBase>> StartingBuildingTypes;

    FFactionStartingConfig()
        : PrimaryRegion(EStartingRegion::Beijing)
        , StartingYear(1912)
        , StartingGold(1000)
        , StartingPopulation(10000)
        , StartingUnits(5)
        , StartingBuildings(3)
    {}
};

/**
 * 勢力關係數據
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FFactionRelationData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType TargetFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionRelationType RelationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RelationValue; // -100 to 100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TradeAgreementValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryAllianceStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasNonAggressionPact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NonAggressionPactTurns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LastWarTurn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 WarCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ActiveTreaties;

    FFactionRelationData()
        : TargetFaction(EFactionType::Invalid)
        , RelationType(EFactionRelationType::Neutral)
        , RelationValue(0.0f)
        , TradeAgreementValue(0.0f)
        , MilitaryAllianceStrength(0.0f)
        , bHasNonAggressionPact(false)
        , NonAggressionPactTurns(0)
        , LastWarTurn(-1)
        , WarCount(0)
    {}
};

/**
 * 勢力AI配置
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FFactionAIConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionAIStrategy PrimaryStrategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EFactionAIStrategy> SecondaryStrategies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Aggressiveness; // 0-1

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DiplomacyWeight; // 0-1

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EconomicWeight; // 0-1

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryWeight; // 0-1

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExpansionSpeed; // 0-1

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAdaptiveBehavior; // 是否適應性調整

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EFactionType, EFactionRelationType> PreferredRelations; // 偏好關係

    FFactionAIConfig()
        : PrimaryStrategy(EFactionAIStrategy::BalancedApproach)
        , Aggressiveness(0.5f)
        , DiplomacyWeight(0.25f)
        , EconomicWeight(0.25f)
        , MilitaryWeight(0.25f)
        , ExpansionSpeed(0.5f)
        , bAdaptiveBehavior(true)
    {}
};

/**
 * 勢力完整數據
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FFactionData
{
    GENERATED_BODY()

    // 基本信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionType FactionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FactionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FactionDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionState CurrentState;

    // 起始配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FFactionStartingConfig StartingConfig;

    // 特色機制
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FFactionMechanicData> SpecialMechanics;

    // 專屬單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FUniqueUnitData> UniqueUnits;

    // 勢力關係
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EFactionType, FFactionRelationData> Relations;

    // AI配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FFactionAIConfig AIConfig;

    // 視覺資源
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UTexture2D> FactionIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<class UTexture2D> FactionBanner;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor FactionColor;

    // 遊戲數據
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 VictoryPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledTerritories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalMilitaryStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEconomicOutput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPlayerControlled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAlive;

    // 優勢劣勢分析
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Advantages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Disadvantages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PlaystyleRecommendations;

    FFactionData()
        : FactionType(EFactionType::Invalid)
        , Difficulty(EFactionDifficulty::Normal)
        , CurrentState(EFactionState::Active)
        , FactionColor(FLinearColor::White)
        , VictoryPoints(0)
        , ControlledTerritories(1)
        , TotalMilitaryStrength(100)
        , TotalEconomicOutput(100)
        , bIsPlayerControlled(false)
        , bIsAlive(true)
    {}
};
