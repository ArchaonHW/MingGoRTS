// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Ê∞ëÂ??≤Ë©©?¢Â?Á≥ªÁµ± - ?¢Â??∏Ê?ÁµêÊ?ÂÆöÁæ©

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "RepublicEraFactions.generated.h"

/**
 * ?¢Â???∫¶?öË?
 */
UENUM(BlueprintType)
enum class EFactionDifficulty : uint8
{
    VeryEasy	UMETA(DisplayName = "Very Easy"),
    Easy		UMETA(DisplayName = "Easy"),
    Medium		UMETA(DisplayName = "Medium"),
    Hard		UMETA(DisplayName = "Hard"),
    VeryHard	UMETA(DisplayName = "Very Hard")
};

/**
 * ?¢Â?È°ûÂ??öË?
 */
UENUM(BlueprintType)
enum class EFactionType : uint8
{
    Central		UMETA(DisplayName = "Central Government"),
    Warlord		UMETA(DisplayName = "Warlord"),
    Revolutionary	UMETA(DisplayName = "Revolutionary"),
    Regional	UMETA(DisplayName = "Regional Power"),
    Minority	UMETA(DisplayName = "Minority Group")
};

/**
 * ?¢Â??Æ‰?È°ûÂ??öË?
 */
UENUM(BlueprintType)
enum class EFactionUnitType : uint8
{
    Infantry	UMETA(DisplayName = "Infantry"),
    Cavalry		UMETA(DisplayName = "Cavalry"),
    Artillery	UMETA(DisplayName = "Artillery"),
    Elite		UMETA(DisplayName = "Elite Unit"),
    Guard		UMETA(DisplayName = "Guard Unit"),
    Militia		UMETA(DisplayName = "Militia"),
    Political	UMETA(DisplayName = "Political Unit"),
    Air		UMETA(DisplayName = "Air Unit")
};

/**
 * ?¢Â??πËâ≤Ê©üÂà∂ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionSpecialMechanic
{
    GENERATED_BODY()

    /** Ê©üÂà∂?çÁ®± */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicName;

    /** Ê©üÂà∂?èËø∞ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString Description;

    /** Ê©üÂà∂È°ûÂ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicType;

    /** ?àÊ??∏ÂÄ?*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    float EffectValue;

    /** ?ØÂê¶?üÁî® */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    bool bEnabled;

    FFactionSpecialMechanic()
        : MechanicName(TEXT(""))
        , Description(TEXT(""))
        , MechanicType(TEXT(""))
        , EffectValue(0.0f)
        , bEnabled(true)
    {
    }
};

/**
 * ?¢Â??Æ‰??∏Ê?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionUnit
{
    GENERATED_BODY()

    /** ?Æ‰??çÁ®± */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString UnitName;

    /** ?Æ‰?È°ûÂ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    EFactionUnitType UnitType;

    /** ?Æ‰??èËø∞ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString Description;

    /** ?ªÊ???*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Attack;

    /** ?≤Á¶¶??*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Defense;

    /** ÁßªÂ???*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Movement;

    /** Âª∫ÈÄ†Ê???*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Cost;

    /** Á∂≠Ë≠∑Ë≤ªÁî® */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Upkeep;

    /** ?πÊ??ΩÂ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString SpecialAbility;

    /** Ëß??Ê¢ù‰ª∂ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString UnlockCondition;

    FFactionUnit()
        : UnitName(TEXT(""))
        , UnitType(EFactionUnitType::Infantry)
        , Description(TEXT(""))
        , Attack(5)
        , Defense(5)
        , Movement(4)
        , Cost(100)
        , Upkeep(10)
        , SpecialAbility(TEXT(""))
        , UnlockCondition(TEXT(""))
    {
    }
};

/**
 * ?¢Â??∏Ê?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionData
{
    GENERATED_BODY()

    /** ?¢Â?ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 FactionID;

    /** ?¢Â??çÁ®± */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString FactionName;

    /** ?¢Â?È°ûÂ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionType FactionType;

    /** ??∫¶Á≠âÁ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionDifficulty Difficulty;

    /** Ëµ∑Â??∞È? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString StartingLocation;

    /** Ëµ∑Â?Âπ¥‰ªΩ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 StartYear;

    /** ?òË??çÁ®± */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString LeaderName;

    /** ?¢Â??èËø∞ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString Description;

    /** ?πËâ≤Ê©üÂà∂ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionSpecialMechanic> SpecialMechanics;

    /** Â∞àÂ±¨?Æ‰? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionUnit> UniqueUnits;

    /** ?™Âã¢?óË°® */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Advantages;

    /** ??ã¢?óË°® */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Disadvantages;

    /** ?®Ëñ¶?©Ê? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> RecommendedStrategies;

    /** ?ùÂ?Ë≥áÊ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialResources;

    /** ?ùÂ?‰∫∫Âè£ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialPopulation;

    /** ?ùÂ?ËªçÈ??∏È? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialMilitary;

    /** Â§ñ‰∫§?ú‰?‰øÆÊ≠£ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float DiplomaticModifier;

    /** Á∂ìÊ??ºÂ?‰øÆÊ≠£ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float EconomicModifier;

    /** Ëªç‰?ÂØ¶Â?‰øÆÊ≠£ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float MilitaryModifier;

    /** ÁßëÊ??ºÂ?‰øÆÊ≠£ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float TechnologyModifier;

    /** ?áÂ?ÂΩ±Èüø?õ‰øÆÊ≠?*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float CultureModifier;

    FFactionData()
        : FactionID(0)
        , FactionName(TEXT(""))
        , FactionType(EFactionType::Warlord)
        , Difficulty(EFactionDifficulty::Medium)
        , StartingLocation(TEXT(""))
        , StartYear(1912)
        , LeaderName(TEXT(""))
        , Description(TEXT(""))
        , InitialResources(1000)
        , InitialPopulation(100000)
        , InitialMilitary(100)
        , DiplomaticModifier(0.0f)
        , EconomicModifier(0.0f)
        , MilitaryModifier(0.0f)
        , TechnologyModifier(0.0f)
        , CultureModifier(0.0f)
    {
    }
};

/**
 * ?¢Â??∏Ê?Ë°®Ë?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionDataRow : public FTableRowBase
{
    GENERATED_BODY()

    /** ?¢Â??∏Ê? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FFactionData FactionData;
};

/**
 * ?¢Â?Á≥ªÁµ±ÁÆ°Á??? */
UCLASS(BlueprintType, Blueprintable)
class FFACTIONMECHANICS_API URepublicEraFactionSystem : public UObject
{
    GENERATED_BODY()

public:
    URepublicEraFactionSystem();

    /** ?ùÂ??ñÂã¢?õÁ≥ªÁµ?*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void InitializeFactionSystem();

    /** ?≤Â??Ä?âÂã¢?õÊï∏??*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionData> GetAllFactions() const;

    /** ?≤Â??áÂ??¢Â??∏Ê? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionData(int32 FactionID) const;

    /** ?≤Â??áÂ??¢Â??∏Ê?ÔºàÊ??çÁ®±Ôº?*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionDataByName(const FString& FactionName) const;

    /** ?≤Â??¢Â??Æ‰? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionUnit> GetFactionUnits(int32 FactionID) const;

    /** ?≤Â??¢Â??πËâ≤Ê©üÂà∂ */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionSpecialMechanic> GetFactionMechanics(int32 FactionID) const;

    /** Ê™¢Êü•?¢Â??ØÂê¶?ØÁî® */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool IsFactionAvailable(int32 FactionID) const;

    /** ?≤Â??¢Â??∏È? */
    UFUNCTION(BlueprintPure, Category = "Faction System")
    int32 GetFactionCount() const;

    /** ?µÂª∫?¢Â?ÂØ¶‰? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData CreateFactionInstance(int32 FactionID);

    /** ?âÁî®?¢Â?‰øÆÊ≠£ */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void ApplyFactionModifiers(FFactionData& FactionData);

    /** È©óË??¢Â??∏Ê? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool ValidateFactionData(const FFactionData& FactionData) const;

protected:
    /** ?†Ë??¢Â??∏Ê? */
    void LoadFactionData();

    /** ?ùÂ??ñÈ?Ë™çÂã¢??*/
    void InitializeDefaultFactions();

    /** Ë®ªÂ??¢Â? */
    void RegisterFaction(const FFactionData& FactionData);

private:
    /** ?Ä?âÂã¢?õÊï∏??*/
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    TArray<FFactionData> AllFactions;

    /** ?¢Â??∏Ê?Ë°?*/
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    UDataTable* FactionDataTable;

    /** Á≥ªÁµ±?ØÂê¶Â∑≤Â?ÂßãÂ? */
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;
};
