#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEcosystemResourceSystem.generated.h"

UENUM(BlueprintType)
enum class EResourceNodeType: uint8 {
    Forest              UMETA(DisplayName = "森林"),
    Mine                UMETA(DisplayName = "矿场"),
    Quarry              UMETA(DisplayName = "采石场"),
    Farm                UMETA(DisplayName = "农场"),
    FishingGround       UMETA(DisplayName = "渔场"),
    HuntingGround       UMETA(DisplayName = "猎场"),
    WaterSource         UMETA(DisplayName = "水源"),
    OilField            UMETA(DisplayName = "油田"),
    GasDeposit          UMETA(DisplayName = "天然气"),
    CrystalFormation    UMETA(DisplayName = "水晶矿"),
    AncientRuins        UMETA(DisplayName = "古代遗迹"),
    MagicalSpring       UMETA(DisplayName = "魔法泉")
};

UENUM(BlueprintType)
enum class EResourceExtractionMethod: uint8 {
    Manual              UMETA(DisplayName = "手工"),
    Tool                UMETA(DisplayName = "工具"),
    Mechanized          UMETA(DisplayName = "机械化"),
    Automated           UMETA(DisplayName = "自动化"),
    Advanced            UMETA(DisplayName = "先进"),
    Magical             UMETA(DisplayName = "魔法"),
    Sustainable         UMETA(DisplayName = "可持续")
};

UENUM(BlueprintType)
enum class EResourceQuality: uint8 {
    Poor                UMETA(DisplayName = "劣质"),
    Common              UMETA(DisplayName = "普通"),
    Good                UMETA(DisplayName = "良好"),
    Excellent           UMETA(DisplayName = "优秀"),
    Legendary           UMETA(DisplayName = "传奇")
};

USTRUCT(BlueprintType)
struct FResourceNode
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString NodeID;
    
    UPROPERTY(BlueprintReadOnly)
    EResourceNodeType NodeType;
    
    UPROPERTY(BlueprintReadOnly)
    FVector Location;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalReserve;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentAmount;
    
    UPROPERTY(BlueprintReadOnly)
    float RegenerationRate;
    
    UPROPERTY(BlueprintReadOnly)
    float NaturalCapacity;
    
    UPROPERTY(BlueprintReadOnly)
    float ExtractionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float ExtractionEfficiency;
    
    UPROPERTY(BlueprintReadOnly)
    EResourceQuality Quality;
    
    UPROPERTY(BlueprintReadOnly)
    float QualityModifier;
    
    UPROPERTY(BlueprintReadOnly)
    float DepletionLevel;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsDepleted;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsRenewable;
    
    UPROPERTY(BlueprintReadOnly)
    float LastExtractionTime;
    
    UPROPERTY(BlueprintReadOnly)
    float RecoveryCooldown;
    
    UPROPERTY(BlueprintReadOnly)
    float EnvironmentalImpact;
    
    UPROPERTY(BlueprintReadOnly)
    float SustainabilityScore;
    
    FResourceNode()
        : Location(FVector::ZeroVector)
        , TotalReserve(1000.0f)
        , CurrentAmount(1000.0f)
        , RegenerationRate(1.0f)
        , NaturalCapacity(1000.0f)
        , ExtractionRate(0.0f)
        , ExtractionEfficiency(1.0f)
        , Quality(EResourceQuality::Common)
        , QualityModifier(1.0f)
        , DepletionLevel(0.0f)
        , bIsDepleted(false)
        , bIsRenewable(true)
        , LastExtractionTime(0.0f)
        , RecoveryCooldown(0.0f)
        , EnvironmentalImpact(0.1f)
        , SustainabilityScore(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FExtractionOperation
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString OperationID;
    
    UPROPERTY(BlueprintReadOnly)
    FString NodeID;
    
    UPROPERTY(BlueprintReadOnly)
    EResourceExtractionMethod Method;
    
    UPROPERTY(BlueprintReadOnly)
    float ExtractionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float Efficiency;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalExtracted;
    
    UPROPERTY(BlueprintReadOnly)
    float EnvironmentalImpact;
    
    UPROPERTY(BlueprintReadOnly)
    float LaborRequirement;
    
    UPROPERTY(BlueprintReadOnly)
    float EnergyRequirement;
    
    FExtractionOperation()
        : Method(EResourceExtractionMethod::Manual)
        , ExtractionRate(1.0f)
        , Efficiency(1.0f)
        , bIsActive(false)
        , TotalExtracted(0.0f)
        , EnvironmentalImpact(0.1f)
        , LaborRequirement(1.0f)
        , EnergyRequirement(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FResourceConsumption
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EResourceType Resource;
    
    UPROPERTY(BlueprintReadOnly)
    float ConsumptionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalConsumed;
    
    UPROPERTY(BlueprintReadOnly)
    float WasteFactor;
    
    UPROPERTY(BlueprintReadOnly)
    float RecyclingRate;
    
    UPROPERTY(BlueprintReadOnly)
    float UrgencyLevel;
    
    FResourceConsumption()
        : Resource(EResourceType::Water)
        , ConsumptionRate(0.0f)
        , TotalConsumed(0.0f)
        , WasteFactor(0.1f)
        , RecyclingRate(0.0f)
        , UrgencyLevel(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FResourceBalance
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EResourceType Resource;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalSupply;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalDemand;
    
    UPROPERTY(BlueprintReadOnly)
    float NetBalance;
    
    UPROPERTY(BlueprintReadOnly)
    float ExtractionCapacity;
    
    UPROPERTY(BlueprintReadOnly)
    float ConsumptionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float ReserveDays;
    
    UPROPERTY(BlueprintReadOnly)
    float SustainabilityIndex;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsDeficit;
    
    UPROPERTY(BlueprintReadOnly)
    float CriticalityLevel;
    
    FResourceBalance()
        : Resource(EResourceType::Water)
        , TotalSupply(0.0f)
        , TotalDemand(0.0f)
        , NetBalance(0.0f)
        , ExtractionCapacity(0.0f)
        , ConsumptionRate(0.0f)
        , ReserveDays(30.0f)
        , SustainabilityIndex(1.0f)
        , bIsDeficit(false)
        , CriticalityLevel(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FResourceZone
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString ZoneID;
    
    UPROPERTY(BlueprintReadOnly)
    FVector CenterLocation;
    
    UPROPERTY(BlueprintReadOnly)
    float Radius;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FResourceNode> ResourceNodes;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<EResourceType, FResourceBalance> ResourceBalances;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalEnvironmentalImpact;
    
    UPROPERTY(BlueprintReadOnly)
    float SustainabilityScore;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceDiversityIndex;
    
    FResourceZone()
        : CenterLocation(FVector::ZeroVector)
        , Radius(5000.0f)
        , TotalEnvironmentalImpact(0.0f)
        , SustainabilityScore(1.0f)
        , ResourceDiversityIndex(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FResourceTrade
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString TradeID;
    
    UPROPERTY(BlueprintReadOnly)
    EResourceType Resource;
    
    UPROPERTY(BlueprintReadOnly)
    float Amount;
    
    UPROPERTY(BlueprintReadOnly)
    FString SourceZoneID;
    
    UPROPERTY(BlueprintReadOnly)
    FString TargetZoneID;
    
    UPROPERTY(BlueprintReadOnly)
    float TradeValue;
    
    UPROPERTY(BlueprintReadOnly)
    float TransportationCost;
    
    UPROPERTY(BlueprintReadOnly)
    float Efficiency;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    FResourceTrade()
        : Resource(EResourceType::Water)
        , Amount(0.0f)
        , TradeValue(0.0f)
        , TransportationCost(0.0f)
        , Efficiency(1.0f)
        , bIsActive(false)
    {}
};

USTRUCT(BlueprintType)
struct FSustainabilityMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float OverallSustainability;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceDepletionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float RegenerationEffectiveness;
    
    UPROPERTY(BlueprintReadOnly)
    float EnvironmentalImpactScore;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceEfficiency;
    
    UPROPERTY(BlueprintReadOnly)
    float LongTermViability;
    
    UPROPERTY(BlueprintReadOnly)
    float CarbonFootprint;
    
    UPROPERTY(BlueprintReadOnly)
    float WaterFootprint;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SustainabilityWarnings;
    
    FSustainabilityMetrics()
        : OverallSustainability(1.0f)
        , ResourceDepletionRate(0.0f)
        , RegenerationEffectiveness(1.0f)
        , EnvironmentalImpactScore(0.1f)
        , ResourceEfficiency(1.0f)
        , LongTermViability(1.0f)
        , CarbonFootprint(0.0f)
        , WaterFootprint(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceNodeDepleted, FString, NodeID, EResourceNodeType, NodeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceNodeRegenerated, FString, NodeID, float, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceBalanceChanged, FString, ZoneID, EResourceType, Resource, float, Balance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSustainabilityAlert, FString, WarningMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceTradeCompleted, FString, TradeID, float, TotalValue);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingEcosystemResourceSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingEcosystemResourceSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Resource")
    void InitializeResourceSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Resource")
    void ShutdownResourceSystem();
    
    // Zone Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Zone")
    FString CreateResourceZone(const FVector& Center, float Radius);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Zone")
    void RemoveResourceZone(const FString& ZoneID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Zone")
    FResourceZone GetResourceZone(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Zone")
    TArray<FString> GetAllZoneIDs() const;
    
    // Resource Node Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Node")
    FString CreateResourceNode(const FString& ZoneID, EResourceNodeType Type, const FVector& Location, float InitialReserve);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Node")
    void RemoveResourceNode(const FString& NodeID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Node")
    FResourceNode GetResourceNode(const FString& NodeID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Node")
    void SetNodeQuality(const FString& NodeID, EResourceQuality Quality);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Node")
    void UpgradeExtractionMethod(const FString& NodeID, EResourceExtractionMethod Method);
    
    // Extraction Operations
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Extraction")
    FString StartExtraction(const FString& NodeID, EResourceExtractionMethod Method, float Rate);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Extraction")
    void StopExtraction(const FString& OperationID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Extraction")
    float ExtractResource(const FString& NodeID, float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Extraction")
    void ModifyExtractionRate(const FString& OperationID, float NewRate);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Extraction")
    FExtractionOperation GetExtractionOperation(const FString& OperationID) const;
    
    // Resource Consumption
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Consumption")
    void RegisterConsumption(const FString& ZoneID, EResourceType Resource, float Rate, float Urgency);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Consumption")
    void UpdateConsumptionRate(const FString& ZoneID, EResourceType Resource, float NewRate);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Consumption")
    void SetRecyclingRate(const FString& ZoneID, EResourceType Resource, float Rate);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Consumption")
    FResourceConsumption GetConsumptionData(const FString& ZoneID, EResourceType Resource) const;
    
    // Regeneration System
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Regeneration")
    void SimulateResourceRegeneration(const FString& ZoneID, float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Regeneration")
    void SetRegenerationRate(const FString& NodeID, float NewRate);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Regeneration")
    void BoostRegeneration(const FString& ZoneID, EResourceType Resource, float BoostFactor);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Regeneration")
    void RestoreDepletedNode(const FString& NodeID, float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Regeneration")
    float CalculateNaturalRegeneration(const FString& NodeID, float DeltaTime) const;
    
    // Balance Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    void CalculateResourceBalance(const FString& ZoneID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Balance")
    FResourceBalance GetResourceBalance(const FString& ZoneID, EResourceType Resource) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    bool IsResourceCritical(const FString& ZoneID, EResourceType Resource) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    TArray<EResourceType> GetDeficitResources(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    float GetResourceAvailability(const FString& ZoneID, EResourceType Resource) const;
    
    // Trade System
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Trade")
    FString CreateResourceTrade(const FString& SourceZoneID, const FString& TargetZoneID, EResourceType Resource, float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Trade")
    void ExecuteTrade(const FString& TradeID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Trade")
    void CancelTrade(const FString& TradeID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Trade")
    FResourceTrade GetTradeInfo(const FString& TradeID) const;
    
    // Sustainability
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Sustainability")
    FSustainabilityMetrics CalculateSustainability(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Sustainability")
    void ImplementSustainablePractices(const FString& ZoneID, EResourceType Resource);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Sustainability")
    float GetEnvironmentalImpact(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Sustainability")
    void ReduceEnvironmentalImpact(const FString& ZoneID, float ReductionFactor);
    
    // Statistics
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Statistics")
    float GetTotalResourceAmount(const FString& ZoneID, EResourceType Resource) const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Statistics")
    float GetTotalExtractionRate(const FString& ZoneID, EResourceType Resource) const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Statistics")
    float GetResourceDiversityIndex(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Statistics")
    TMap<EResourceType, float> GetResourceDistribution(const FString& ZoneID) const;
    
    // Tick
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void Tick(float DeltaTime);
    
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnResourceNodeDepleted OnResourceNodeDepleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnResourceNodeRegenerated OnResourceNodeRegenerated;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnResourceBalanceChanged OnResourceBalanceChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnSustainabilityAlert OnSustainabilityAlert;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnResourceTradeCompleted OnResourceTradeCompleted;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    float CurrentGameTime;
    
    UPROPERTY()
    TMap<FString, FResourceZone> ResourceZones;
    
    UPROPERTY()
    TMap<FString, FResourceNode> AllResourceNodes;
    
    UPROPERTY()
    TMap<FString, FExtractionOperation> ActiveExtractions;
    
    UPROPERTY()
    TMap<FString, FResourceTrade> ActiveTrades;
    
    UPROPERTY()
    TMap<FString, TMap<EResourceType, FResourceConsumption>> ZoneConsumptions;
    
    // Default configurations
    UPROPERTY()
    TMap<EResourceNodeType, float> DefaultRegenerationRates;
    
    UPROPERTY()
    TMap<EResourceExtractionMethod, float> ExtractionEfficiencyMultipliers;
    
    UPROPERTY()
    TMap<EResourceExtractionMethod, float> EnvironmentalImpactMultipliers;
    
    // Private methods
    void InitializeDefaultRates();
    void SimulateNodeRegeneration(FResourceNode& Node, float DeltaTime);
    void UpdateExtractionOperations(float DeltaTime);
    void ProcessConsumption(const FString& ZoneID, float DeltaTime);
    void UpdateResourceBalances(const FString& ZoneID);
    float CalculateNodeSustainability(const FResourceNode& Node) const;
    EResourceType MapNodeTypeToResource(EResourceNodeType NodeType) const;
    float GetExtractionEfficiency(EResourceExtractionMethod Method) const;
    float GetEnvironmentalImpactMultiplier(EResourceExtractionMethod Method) const;
    void CheckSustainabilityThresholds(const FString& ZoneID);
    void DepleteNode(FResourceNode& Node);
    void RegenerateDepletedNodes(float DeltaTime);
    float CalculateDiversityIndex(const FString& ZoneID) const;
    void ProcessTrade(FResourceTrade& Trade);
    float CalculateTradeValue(EResourceType Resource, float Amount) const;
    void UpdateZoneSustainabilityScore(const FString& ZoneID);
    bool ValidateResourceAvailability(const FString& ZoneID, EResourceType Resource, float Amount) const;
};
