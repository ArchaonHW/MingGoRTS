// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - Core Types and Enums

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSFactionTypes.generated.h"

/**
 * Faction difficulty level
 */
UENUM(BlueprintType)
enum class EFactionDifficulty : uint8
{
    VeryEasy    UMETA(DisplayName = "Very Easy"),
    Easy        UMETA(DisplayName = "Easy"),
    Normal      UMETA(DisplayName = "Normal"),
    Hard        UMETA(DisplayName = "Hard"),
    VeryHard    UMETA(DisplayName = "Very Hard")
};

/**
 * Faction type category
 */
UENUM(BlueprintType)
enum class EFactionCategory : uint8
{
    CentralGovernment   UMETA(DisplayName = "Central Government"),
    RegionalWarlord     UMETA(DisplayName = "Regional Warlord"),
    Revolutionary       UMETA(DisplayName = "Revolutionary Force"),
    MinorPower          UMETA(DisplayName = "Minor Power"),
    ForeignSupported    UMETA(DisplayName = "Foreign Supported")
};

/**
 * Unit type classification
 */
UENUM(BlueprintType)
enum class EFactionUnitType : uint8
{
    Infantry        UMETA(DisplayName = "Infantry"),
    Cavalry         UMETA(DisplayName = "Cavalry"),
    Artillery       UMETA(DisplayName = "Artillery"),
    Special         UMETA(DisplayName = "Special Unit"),
    Support         UMETA(DisplayName = "Support Unit"),
    Naval           UMETA(DisplayName = "Naval Unit"),
    Air             UMETA(DisplayName = "Air Unit")
};

/**
 * Unit specialization trait
 */
UENUM(BlueprintType)
enum class EUnitTrait : uint8
{
    None            UMETA(DisplayName = "None"),
    ShockTroop      UMETA(DisplayName = "Shock Troop"),
    Defensive       UMETA(DisplayName = "Defensive Specialist"),
    Guerrilla       UMETA(DisplayName = "Guerrilla Warfare"),
    Mountain        UMETA(DisplayName = "Mountain Warfare"),
    Urban           UMETA(DisplayName = "Urban Combat"),
    CavalryCharge   UMETA(DisplayName = "Cavalry Charge"),
    ArtillerySupport UMETA(DisplayName = "Artillery Support"),
    Engineer        UMETA(DisplayName = "Engineering"),
    Political       UMETA(DisplayName = "Political Commissar"),
    Religious       UMETA(DisplayName = "Religious Zeal"),
    Mercenary       UMETA(DisplayName = "Mercenary"),
    Foreign         UMETA(DisplayName = "Foreign Advisor")
};

/**
 * Faction unique mechanic type
 */
UENUM(BlueprintType)
enum class EFactionMechanicType : uint8
{
    Diplomatic      UMETA(DisplayName = "Diplomatic Advantage"),
    Economic        UMETA(DisplayName = "Economic Bonus"),
    Military        UMETA(DisplayName = "Military Specialization"),
    Political       UMETA(DisplayName = "Political Mechanic"),
    Cultural        UMETA(DisplayName = "Cultural Influence"),
    Technological   UMETA(DisplayName = "Technology Focus"),
    Geographical    UMETA(DisplayName = "Geographic Advantage")
};

/**
 * Base unit statistics
 */
USTRUCT(BlueprintType)
struct FFactionUnitStats
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    int32 AttackPower = 100;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    int32 DefensePower = 100;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    int32 MovementSpeed = 100;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    int32 HealthPoints = 100;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    int32 ProductionCost = 100;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    int32 UpkeepCost = 10;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Stats")
    float TrainingTime = 10.0f;
    
    FFactionUnitStats()
        : AttackPower(100)
        , DefensePower(100)
        , MovementSpeed(100)
        , HealthPoints(100)
        , ProductionCost(100)
        , UpkeepCost(10)
        , TrainingTime(10.0f)
    {}
};

/**
 * Faction unit definition
 */
USTRUCT(BlueprintType)
struct FFactionUnit
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    FName UnitID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    FText UnitName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    FText Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    EFactionUnitType UnitType = EFactionUnitType::Infantry;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    TArray<EUnitTrait> Traits;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    FFactionUnitStats BaseStats;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    TArray<FName> Prerequisites;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit Definition")
    TSoftObjectPtr<UTexture2D> UnitIcon;
    
    FFactionUnit()
        : UnitType(EFactionUnitType::Infantry)
    {}
};

/**
 * Unique faction mechanic
 */
USTRUCT(BlueprintType)
struct FFactionMechanic
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    FName MechanicID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    FText MechanicName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    FText Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    EFactionMechanicType MechanicType = EFactionMechanicType::Military;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    float EffectMultiplier = 1.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    TArray<FName> RequiredTechnologies;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanic")
    bool bPassive = true;
    
    FFactionMechanic()
        : MechanicType(EFactionMechanicType::Military)
        , EffectMultiplier(1.0f)
        , bPassive(true)
    {}
};

/**
 * Starting position information
 */
USTRUCT(BlueprintType)
struct FFactionStartingPosition
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Starting Position")
    FText CapitalCity;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Starting Position")
    TArray<FText> StartingTerritories;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Starting Position")
    int32 StartingYear = 1912;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Starting Position")
    int32 InitialGold = 1000;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Starting Position")
    int32 InitialManpower = 500;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Starting Position")
    int32 InitialReputation = 50;
    
    FFactionStartingPosition()
        : StartingYear(1912)
        , InitialGold(1000)
        , InitialManpower(500)
        , InitialReputation(50)
    {}
};

/**
 * Faction advantages and disadvantages
 */
USTRUCT(BlueprintType)
struct FFactionTraits
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Traits")
    TArray<FText> Advantages;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Traits")
    TArray<FText> Disadvantages;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Traits")
    TArray<FText> RecommendedStrategies;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Traits")
    TArray<FText> HistoricalNotes;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Traits")
    FText PlayerTypeRecommendation;
};

/**
 * Complete faction attributes
 */
USTRUCT(BlueprintType)
struct FFactionAttributes
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    FName FactionID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    FText FactionName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    FText FactionNameEnglish;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    EFactionDifficulty Difficulty = EFactionDifficulty::Normal;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    EFactionCategory Category = EFactionCategory::RegionalWarlord;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    FLinearColor FactionColor = FLinearColor::White;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    TSoftObjectPtr<UTexture2D> FactionFlag;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    TSoftObjectPtr<UTexture2D> FactionEmblem;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    FFactionStartingPosition StartingPosition;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    FFactionTraits Traits;
    
    FFactionAttributes()
        : Difficulty(EFactionDifficulty::Normal)
        , Category(EFactionCategory::RegionalWarlord)
        , FactionColor(FLinearColor::White)
    {}
};

/**
 * Gameplay guide for a faction
 */
USTRUCT(BlueprintType)
struct FGameplayGuide
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    FText EarlyGameStrategy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    FText MidGameStrategy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    FText LateGameStrategy;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    FText MilitaryTactics;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    FText DiplomaticAdvice;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    FText EconomicFocus;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    TArray<FText> KeyDecisions;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guide")
    TArray<FText> VictoryConditions;
};
