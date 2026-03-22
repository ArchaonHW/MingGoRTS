#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "MingStrategicLayerController.generated.h"

UENUM(BlueprintType)
enum class EMingMapRegionType : uint8
{
    Territory UMETA(DisplayName = "Territory"),
    City UMETA(DisplayName = "City"),
    Fortress UMETA(DisplayName = "Fortress"),
    ResourcePoint UMETA(DisplayName = "Resource Point"),
    TradeRoute UMETA(DisplayName = "Trade Route"),
    StrategicPass UMETA(DisplayName = "Strategic Pass")
};

UENUM(BlueprintType)
enum class EMingFactionRelation : uint8
{
    Allied UMETA(DisplayName = "Allied"),
    Friendly UMETA(DisplayName = "Friendly"),
    Neutral UMETA(DisplayName = "Neutral"),
    Hostile UMETA(DisplayName = "Hostile"),
    AtWar UMETA(DisplayName = "At War")
};

USTRUCT(BlueprintType)
struct FMingMapRegion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    FString RegionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    FString RegionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    EMingMapRegionType RegionType = EMingMapRegionType::Territory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    FString ControllingFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    FVector2D MapPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    float Radius = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    int32 ResourceValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    int32 StrategicValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    int32 GarrisonStrength = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    TArray<FString> ConnectedRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    bool bIsContested = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Region")
    bool bIsCapital = false;

    FMingMapRegion()
        : RegionType(EMingMapRegionType::Territory)
        , MapPosition(FVector2D::ZeroVector)
        , Radius(50.0f)
        , ResourceValue(0)
        , StrategicValue(0)
        , GarrisonStrength(0)
        , bIsContested(false)
        , bIsCapital(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingFactionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FString FactionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FString FactionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FLinearColor FactionColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    TArray<FString> ControlledRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 MilitaryStrength = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 EconomicStrength = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    int32 DiplomaticInfluence = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FString LeaderCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    bool bIsPlayer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    bool bIsDefeated = false;

    FMingFactionInfo()
        : FactionColor(FLinearColor::White)
        , MilitaryStrength(0)
        , EconomicStrength(0)
        , DiplomaticInfluence(0)
        , bIsPlayer(false)
        , bIsDefeated(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingDiplomaticRelation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    FString FactionA;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    FString FactionB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    EMingFactionRelation RelationType = EMingFactionRelation::Neutral;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    int32 RelationValue = 0; // -100 to 100

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    bool bHasTradeAgreement = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    bool bHasAlliance = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    bool bIsAtWar = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    int32 WarDurationDays = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Diplomacy")
    TArray<FString> ActiveTreaties;

    FMingDiplomaticRelation()
        : RelationType(EMingFactionRelation::Neutral)
        , RelationValue(0)
        , bHasTradeAgreement(false)
        , bHasAlliance(false)
        , bIsAtWar(false)
        , WarDurationDays(0)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionSelected, const FMingMapRegion&, SelectedRegion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFactionSelected, const FMingFactionInfo&, SelectedFaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiplomaticAction, const FString&, TargetFaction, EMingStrategicAction, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategicOrderIssued, const FMingStrategicDecision&, Order);

/**
 * Strategic Layer Controller
 * Manages the strategic map, factions, and high-level decision making
 */
UCLASS(ClassGroup = (Strategic), Blueprintable)
class MINGSTRATEGIC_API UMingStrategicLayerController : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategicLayerController();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void InitializeStrategicLayer();

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void ShutdownStrategicLayer();

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void ActivateStrategicLayer();

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void DeactivateStrategicLayer();

    // Map Management
    UFUNCTION(BlueprintCallable, Category = "Map Management")
    void LoadStrategicMap(const FString& MapID);

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    void ClearStrategicMap();

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    void AddRegion(const FMingMapRegion& Region);

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    void RemoveRegion(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    bool GetRegion(const FString& RegionID, FMingMapRegion& OutRegion) const;

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    TArray<FMingMapRegion> GetAllRegions() const;

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    TArray<FMingMapRegion> GetRegionsByFaction(const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Map Management")
    TArray<FMingMapRegion> GetContestedRegions() const;

    // Faction Management
    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    void RegisterFaction(const FMingFactionInfo& Faction);

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    void UnregisterFaction(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    bool GetFaction(const FString& FactionID, FMingFactionInfo& OutFaction) const;

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    FMingFactionInfo GetPlayerFaction() const;

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    TArray<FMingFactionInfo> GetAllFactions() const;

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    void UpdateFactionStrength(const FString& FactionID, int32 MilitaryDelta, int32 EconomicDelta);

    // Diplomacy Management
    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    void SetDiplomaticRelation(const FString& FactionA, const FString& FactionB, EMingFactionRelation Relation);

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    FMingDiplomaticRelation GetDiplomaticRelation(const FString& FactionA, const FString& FactionB) const;

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    void DeclareWar(const FString& AttackerFaction, const FString& DefenderFaction);

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    void NegotiatePeace(const FString& FactionA, const FString& FactionB);

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    void FormAlliance(const FString& FactionA, const FString& FactionB, int32 DurationDays);

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    void EstablishTradeAgreement(const FString& FactionA, const FString& FactionB);

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    void BreakTreaty(const FString& FactionA, const FString& FactionB, const FString& TreatyType);

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    TArray<FMingDiplomaticRelation> GetFactionRelations(const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    TArray<FString> GetAllies(const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Diplomacy")
    TArray<FString> GetEnemies(const FString& FactionID) const;

    // Strategic Actions
    UFUNCTION(BlueprintCallable, Category = "Strategic Actions")
    void ExecuteStrategicDecision(const FMingStrategicDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Strategic Actions")
    void MoveTroops(const FString& FromRegion, const FString& ToRegion, int32 TroopCount, const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Strategic Actions")
    void FortifyRegion(const FString& RegionID, int32 GarrisonIncrease);

    UFUNCTION(BlueprintCallable, Category = "Strategic Actions")
    void DevelopRegion(const FString& RegionID, const FString& DevelopmentType);

    UFUNCTION(BlueprintCallable, Category = "Strategic Actions")
    void LaunchCampaign(const FString& FromRegion, const FString& TargetRegion, int32 TroopCount);

    // Intelligence & Information
    UFUNCTION(BlueprintCallable, Category = "Intelligence")
    int32 GetRegionIntelligence(const FString& RegionID, const FString& FactionID) const;

    UFUNCTION(BlueprintCallable, Category = "Intelligence")
    void GatherIntelligence(const FString& TargetRegion, const FString& FactionID, int32 IntelLevel);

    UFUNCTION(BlueprintCallable, Category = "Intelligence")
    void RevealRegion(const FString& RegionID, const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Intelligence")
    void ConcealRegion(const FString& RegionID, const FString& FactionID);

    // Economy & Resources
    UFUNCTION(BlueprintCallable, Category = "Economy")
    int32 GetFactionResource(const FString& FactionID, const FString& ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddFactionResource(const FString& FactionID, const FString& ResourceType, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void ConsumeFactionResource(const FString& FactionID, const FString& ResourceType, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void TradeResources(const FString& FromFaction, const FString& ToFaction, TMap<FString, int32> Resources);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    int32 CalculateFactionIncome(const FString& FactionID) const;

    // UI Interface
    UFUNCTION(BlueprintCallable, Category = "UI Interface")
    void SelectRegion(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "UI Interface")
    void SelectFaction(const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "UI Interface")
    void ClearSelection();

    UFUNCTION(BlueprintPure, Category = "UI Interface")
    FString GetSelectedRegionID() const { return SelectedRegionID; }

    UFUNCTION(BlueprintPure, Category = "UI Interface")
    FString GetSelectedFactionID() const { return SelectedFactionID; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRegionSelected OnRegionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnFactionSelected OnFactionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDiplomaticAction OnDiplomaticAction;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStrategicOrderIssued OnStrategicOrderIssued;

protected:
    UPROPERTY()
    TMap<FString, FMingMapRegion> Regions;

    UPROPERTY()
    TMap<FString, FMingFactionInfo> Factions;

    UPROPERTY()
    TMap<FString, FMingDiplomaticRelation> DiplomaticRelations;

    UPROPERTY()
    FString SelectedRegionID;

    UPROPERTY()
    FString SelectedFactionID;

    UPROPERTY()
    FString PlayerFactionID;

    UPROPERTY()
    bool bIsActive = false;

    UPROPERTY()
    FString CurrentMapID;

    // Internal functions
    void InitializeDefaultFactions();
    void InitializeDefaultRegions();
    void SetupDiplomaticDefaults();
    
    void UpdateRegionControl(const FString& RegionID, const FString& NewFactionID);
    void CheckVictoryConditions();
    void ProcessEconomicTick();
    
    FString GenerateRelationKey(const FString& FactionA, const FString& FactionB) const;
    void NotifyRegionChanged(const FString& RegionID);
    void NotifyFactionChanged(const FString& FactionID);
};
