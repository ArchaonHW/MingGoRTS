#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingBuildingLayer.generated.h"

// Âª∫Á?È°ûÂ?
UENUM(BlueprintType)
enum class EMingStratBuildingType : uint8
{
    Residential,     // ‰ΩèÂ?
    Commercial,      // ?ÜÊ•≠
    Industrial,      // Â∑•Ê•≠
    Military,        // Ëªç‰?
    Educational,     // ?ôËÇ≤
    Medical,         // ?´Á?
    Cultural,        // ?áÂ?
    Agricultural,    // Ëæ≤Ê•≠
    Infrastructure,   // ?∫Á?Ë®≠ÊñΩ
    Government       // ?øÂ?
};

// Âª∫Á??Ä??UENUM(BlueprintType)
enum class EMingBuildingState : uint8
{
    Planning,        // Ë¶èÂ?‰∏?    UnderConstruction, // Âª∫Ë®≠‰∏?    Operational,     // ?ãÁ?‰∏?    Upgrading,       // ?áÁ?‰∏?    Damaged,         // ?çÂ?
    Destroyed,       // ?ßÊ?
    Abandoned        // Âª¢Ê?
};

// Âª∫Á?Á≠âÁ?
UENUM(BlueprintType)
enum class EMingBuildingLevel : uint8
{
    Level1,          // Á≠âÁ?1
    Level2,          // Á≠âÁ?2
    Level3,          // Á≠âÁ?3
    Level4,          // Á≠âÁ?4
    Level5           // Á≠âÁ?5
};

// Âª∫Á?‰ø°ÊÅØ
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingBuildingInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* BuildingActor;

    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;

    UPROPERTY(BlueprintReadOnly)
    FString BuildingName;

    UPROPERTY(BlueprintReadOnly)
    EMingBuildingType BuildingType;

    UPROPERTY(BlueprintReadOnly)
    EMingBuildingState CurrentState;

    UPROPERTY(BlueprintReadOnly)
    EMingBuildingLevel BuildingLevel;

    UPROPERTY(BlueprintReadOnly)
    FVector Location;

    UPROPERTY(BlueprintReadOnly)
    float Health;

    UPROPERTY(BlueprintReadOnly)
    float ProductionRate;

    UPROPERTY(BlueprintReadOnly)
    float MaintenanceCost;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> ResourceProduction;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> ResourceConsumption;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AvailableUpgrades;
};

// Âª∫Ë®≠?ÖÁõÆ
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingConstructionProject
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProjectID;

    UPROPERTY(BlueprintReadOnly)
    FString ProjectName;

    UPROPERTY(BlueprintReadOnly)
    EMingBuildingType BuildingType;

    UPROPERTY(BlueprintReadOnly)
    FVector ConstructionLocation;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    float ConstructionTime;

    UPROPERTY(BlueprintReadOnly)
    float Progress;

    UPROPERTY(BlueprintReadOnly)
    bool bIsPriority;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EstimatedCompletion;
};

// Âª∫Ë®≠Ê±∫Á?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingBuildingDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float EconomicImpact;

    UPROPERTY(BlueprintReadOnly)
    float SocialImpact;

    UPROPERTY(BlueprintReadOnly)
    float MilitaryImpact;
};

// Âª∫Ë®≠‰∫ã‰ª∂
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingBuildingEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    FString BuildingID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedBuildings;

    UPROPERTY(BlueprintReadOnly)
    float EconomicImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

/**
 * Âª∫Ë®≠Â±§Á≥ªÁµ? * Ë≤†Ë≤¨?∫Âú∞Âª∫Ë®≠?ÅË?Ê∫êÁÆ°?Ü„ÄÅÁ?ÊøüÈ??üÁ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingBuildingLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingBuildingLayer();

    // ÂØ¶Áèæ‰ªãÈù¢?πÊ?
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingGameEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // Âª∫Á?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void ConstructBuilding(const FString& BuildingID, const FString& BuildingName, EMingBuildingType Type, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void UpgradeBuilding(const FString& BuildingID, EMingBuildingLevel TargetLevel);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void DestroyBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void RepairBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    FMingBuildingInfo GetBuilding(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FMingBuildingInfo> GetAllBuildings() const;

    // Âª∫Ë®≠?ÖÁõÆÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void StartConstructionProject(const FMingConstructionProject& Project);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void CancelConstructionProject(const FString& ProjectID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void PrioritizeProject(const FString& ProjectID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FMingConstructionProject> GetConstructionQueue() const;

    // Ë≥áÊ?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void AllocateResources(const TMap<EMingResourceType, float>& Allocation);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TMap<EMingResourceType, float> GetResourceProduction() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TMap<EMingResourceType, float> GetResourceConsumption() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    float GetResourceEfficiency() const;

    // Âª∫Ë®≠Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void MakeBuildingDecision(const FMingBuildingDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void ProcessBuildingDecisions();

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FMingBuildingDecision> GetBuildingDecisions() const;

    // Á∂ìÊ??ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    float CalculateEconomicOutput() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    float CalculateInfrastructureLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FString> IdentifyEconomicOpportunities() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FString> AssessEconomicThreats() const;

    // ?≤Â?Âª∫Ë®≠?Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    FMingBuildingState GetBuildingState() const;

protected:
    // Âª∫Ë®≠?Ä??    UPROPERTY(BlueprintReadOnly)
    FMingBuildingState BuildingState;

    // Âª∫Á??óË°®
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingBuildingInfo> Buildings;

    // Âª∫Ë®≠?ÖÁõÆ
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingConstructionProject> ConstructionQueue;

    // Âª∫Ë®≠Ê±∫Á?
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingBuildingDecision> BuildingDecisions;

    // Âª∫Ë®≠‰∫ã‰ª∂Ê≠∑Âè≤
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingBuildingEvent> BuildingHistory;

    // Âª∫Ë®≠È¢®Ê†º
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    FString BuildingStyle;

    // Á∂ìÊ??ºÂ?Á≠ñÁï•
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    FString EconomicDevelopmentStrategy;

    // ?∫Á?Ë®≠ÊñΩ?ïË??èÂ•Ω
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    float InfrastructureInvestmentPreference;

    // Â∑•Ê•≠?ñÁ?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    float IndustrializationLevel;

    // ?ÜÊ?Âª∫Ë®≠?ÄÊ≥?    void AnalyzeConstructionStatus();

    // Ë©ï‰º∞Á∂ìÊ??ÄÊ≥?    void AssessEconomicConditions();

    // Ë®àÁ?Âª∫Ë®≠?àÁ?
    float CalculateConstructionEfficiency();

    // ?êÊ∏¨Âª∫Ë®≠ÁµêÊ?
    TArray<FString> PredictConstructionOutcome(const FMingConstructionProject& Project);

    // Ê∞ëÂ??πËâ≤Âª∫Ë®≠
    void ApplyRepublicanEraConstruction();

    // Ëøë‰ª£?ñÂ∑•Ê•≠Âª∫Ë®?    void ExecuteModernIndustrialConstruction();

    // ?éÂ??æ‰ª£?ñÂª∫Ë®?    void ExecuteUrbanModernization();

    // ‰∫§ÈÄöÂü∫Á§éË®≠?ΩÂª∫Ë®?    void ExecuteTransportInfrastructureConstruction();

    // ?ôËÇ≤Ë®≠ÊñΩÂª∫Ë®≠
    void ExecuteEducationalFacilityConstruction();

    // ?´Á?Ë®≠ÊñΩÂª∫Ë®≠
    void ExecuteMedicalFacilityConstruction();

    // ?áÂ?Ë®≠ÊñΩÂª∫Ë®≠
    void ExecuteCulturalFacilityConstruction();

    // Ëæ≤Ê•≠?æ‰ª£?ñÂª∫Ë®?    void ExecuteAgriculturalModernization();

private:
    // Âª∫Ë®≠?¥Êñ∞?ìÈ?
    float BuildingUpdateInterval;

    // ‰∏äÊ¨°?¥Êñ∞?ÇÈ?
    float LastBuildingUpdate;

    // ?ÄÂ§ßÂª∫ÁØâÊï∏??    int32 MaxBuildingCount;

    // Âª∫Ë®≠?üÂ∫¶?†Ê?
    float ConstructionSpeedBonus;

    // Á∂ìÊ?Â¢ûÈï∑??    float EconomicGrowthRate;

    // Âª∫Ë®≠Â®ÅË?Ë©ï‰º∞
    TMap<FString, float> ConstructionThreats;

    // Âª∫Ë®≠Ê©üÊ?Ë©ï‰º∞
    TMap<FString, float> ConstructionOpportunities;

    // Âª∫Ë®≠Ê±∫Á?Ê≠∑Âè≤
    TArray<FMingBuildingDecision> DecisionHistory;
};
