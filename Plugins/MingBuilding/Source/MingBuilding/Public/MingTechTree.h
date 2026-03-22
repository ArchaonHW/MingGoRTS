#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingActor.h"
#include "MingResourceSystem.h"
#include "MingTechTree.generated.h"

UENUM(BlueprintType)
enum class EMingTechCategory : uint8
{
    Military UMETA(DisplayName = "Military"),
    Economy UMETA(DisplayName = "Economy"),
    Infrastructure UMETA(DisplayName = "Infrastructure"),
    Defense UMETA(DisplayName = "Defense"),
    Special UMETA(DisplayName = "Special")
};

USTRUCT(BlueprintType)
struct FMingTechUnlock
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingBuildingType UnlockedBuilding;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class AMingTacticalUnit> UnlockedUnit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString UnlockedAbility;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float StatModifier; // e.g., 0.1 for 10% improvement

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString StatName; // "AttackDamage", "BuildSpeed", etc.

    FMingTechUnlock()
        : UnlockedBuilding(EMingBuildingType::CommandCenter)
        , StatModifier(0.0f)
    {}
};

// Wrapper struct to allow TArray in TMap (UHT limitation)
USTRUCT(BlueprintType)
struct FMingBuildingUpgradeList
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingBuildingUpgrade> Upgrades;
};

USTRUCT(BlueprintType)
struct FMingTechNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString TechID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString TechName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingTechCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingResourceCost> ResearchCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ResearchTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Prerequisites; // TechIDs that must be unlocked first

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FMingTechUnlock Unlock;

    UPROPERTY(BlueprintReadOnly)
    bool bIsResearched;

    UPROPERTY(BlueprintReadOnly)
    bool bIsResearching;

    UPROPERTY(BlueprintReadOnly)
    float ResearchProgress;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 TechLevel; // 1-3 for tier system

    FMingTechNode()
        : Category(EMingTechCategory::Military)
        , ResearchTime(30.0f)
        , bIsResearched(false)
        , bIsResearching(false)
        , ResearchProgress(0.0f)
        , TechLevel(1)
    {}
};

USTRUCT(BlueprintType)
struct FMingBuildingUpgrade
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString UpgradeID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString UpgradeName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingResourceCost> UpgradeCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float UpgradeTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxUpgradeLevel;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentLevel;

    // Stat modifiers per level
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float HealthMultiplier; // 1.2 = +20% per level

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ProductionMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DefenseMultiplier;

    FMingBuildingUpgrade()
        : UpgradeTime(15.0f)
        , MaxUpgradeLevel(3)
        , CurrentLevel(0)
        , HealthMultiplier(1.0f)
        , ProductionMultiplier(1.0f)
        , DefenseMultiplier(1.0f)
    {}
};

/**
 * Tech Tree system for MingGoRTS
 * Manages research, building unlocks, and upgrades
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API UMingTechTree : public UObject
{
    GENERATED_BODY()

public:
    UMingTechTree();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void InitializeDefaultTechTree();

    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void LoadTechTree(const TArray<FMingTechNode>& TechNodes);

    // Tech Node Management
    UFUNCTION(BlueprintCallable, Category = "Tech Nodes")
    void AddTechNode(const FMingTechNode& Node);

    UFUNCTION(BlueprintCallable, Category = "Tech Nodes")
    bool RemoveTechNode(const FString& TechID);

    UFUNCTION(BlueprintPure, Category = "Tech Nodes")
    FMingTechNode GetTechNode(const FString& TechID) const;

    UFUNCTION(BlueprintPure, Category = "Tech Nodes")
    TArray<FMingTechNode> GetAllTechNodes() const;

    UFUNCTION(BlueprintPure, Category = "Tech Nodes")
    TArray<FMingTechNode> GetTechNodesByCategory(EMingTechCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Tech Nodes")
    TArray<FMingTechNode> GetAvailableTechs() const;

    UFUNCTION(BlueprintPure, Category = "Tech Nodes")
    TArray<FMingTechNode> GetResearchedTechs() const;

    // Research
    UFUNCTION(BlueprintCallable, Category = "Research")
    bool CanResearchTech(const FString& TechID) const;

    UFUNCTION(BlueprintCallable, Category = "Research")
    bool StartResearch(const FString& TechID);

    UFUNCTION(BlueprintCallable, Category = "Research")
    void CancelResearch(const FString& TechID);

    UFUNCTION(BlueprintCallable, Category = "Research")
    void UpdateResearch(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Research")
    void CompleteResearch(const FString& TechID);

    UFUNCTION(BlueprintPure, Category = "Research")
    bool IsTechResearched(const FString& TechID) const;

    UFUNCTION(BlueprintPure, Category = "Research")
    bool IsTechResearching(const FString& TechID) const;

    UFUNCTION(BlueprintPure, Category = "Research")
    float GetResearchProgress(const FString& TechID) const;

    // Building Unlocks
    UFUNCTION(BlueprintPure, Category = "Unlocks")
    bool IsBuildingUnlocked(EMingBuildingType BuildingType) const;

    UFUNCTION(BlueprintPure, Category = "Unlocks")
    TArray<EMingBuildingType> GetUnlockedBuildings() const;

    UFUNCTION(BlueprintPure, Category = "Unlocks")
    TArray<EMingBuildingType> GetLockedBuildings() const;

    UFUNCTION(BlueprintPure, Category = "Unlocks")
    FString GetBuildingUnlockTech(EMingBuildingType BuildingType) const;

    // Building Upgrades
    UFUNCTION(BlueprintCallable, Category = "Upgrades")
    void RegisterBuildingUpgrade(EMingBuildingType BuildingType, const FMingBuildingUpgrade& Upgrade);

    UFUNCTION(BlueprintCallable, Category = "Upgrades")
    bool CanUpgradeBuilding(const FString& UpgradeID) const;

    UFUNCTION(BlueprintCallable, Category = "Upgrades")
    bool UpgradeBuilding(const FString& UpgradeID);

    UFUNCTION(BlueprintPure, Category = "Upgrades")
    FMingBuildingUpgrade GetBuildingUpgrade(const FString& UpgradeID) const;

    UFUNCTION(BlueprintPure, Category = "Upgrades")
    TArray<FMingBuildingUpgrade> GetAvailableUpgradesForBuilding(EMingBuildingType BuildingType) const;

    UFUNCTION(BlueprintPure, Category = "Upgrades")
    int32 GetUpgradeLevel(const FString& UpgradeID) const;

    // Prerequisites
    UFUNCTION(BlueprintPure, Category = "Prerequisites")
    bool ArePrerequisitesMet(const FString& TechID) const;

    UFUNCTION(BlueprintPure, Category = "Prerequisites")
    TArray<FString> GetMissingPrerequisites(const FString& TechID) const;

    // Cost Calculation
    UFUNCTION(BlueprintPure, Category = "Costs")
    TArray<FMingResourceCost> GetResearchCost(const FString& TechID) const;

    UFUNCTION(BlueprintPure, Category = "Costs")
    float GetResearchTime(const FString& TechID) const;

    // Effects & Modifiers
    UFUNCTION(BlueprintPure, Category = "Effects")
    float GetStatModifier(const FString& StatName) const;

    UFUNCTION(BlueprintPure, Category = "Effects")
    bool HasAbilityUnlocked(const FString& AbilityName) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechResearched, const FMingTechNode&, TechNode);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResearchProgress, const FMingTechNode&, TechNode);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingUnlocked, EMingBuildingType, BuildingType);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingUpgraded, EMingBuildingType, BuildingType, int32, NewLevel);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTechResearched OnTechResearched;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnResearchProgress OnResearchProgress;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUnlocked OnBuildingUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUpgraded OnBuildingUpgraded;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetTechCategoryName(EMingTechCategory Category);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FLinearColor GetTechCategoryColor(EMingTechCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SerializeTechTree() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void DeserializeTechTree(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingTechNode> TechNodes;

    UPROPERTY()
    TMap<EMingBuildingType, FMingBuildingUpgradeList> BuildingUpgrades;

    UPROPERTY()
    TSet<EMingBuildingType> UnlockedBuildings;

    UPROPERTY()
    TSet<FString> UnlockedAbilities;

    UPROPERTY()
    TWeakObjectPtr<class UMingResourceSystem> ResourceSystem;

    UPROPERTY()
    TArray<FString> CurrentlyResearching;

    // Default tech tree setup
    void SetupDefaultMilitaryTechs();
    void SetupDefaultEconomyTechs();
    void SetupDefaultInfrastructureTechs();
    void SetupDefaultDefenseTechs();

    // Unlock application
    void ApplyTechUnlock(const FMingTechUnlock& Unlock);
    void UnlockBuilding(EMingBuildingType BuildingType);
    void UnlockUnit(TSubclassOf<class AMingTacticalUnit> UnitClass);
    void UnlockAbility(const FString& AbilityName);

    // Helpers
    void RecalculateUnlockedBuildings();
    bool HasEnoughResources(const TArray<FMingResourceCost>& Costs) const;
    bool ConsumeResources(const TArray<FMingResourceCost>& Costs);
};
