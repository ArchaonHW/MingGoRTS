#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSTechTree.generated.h"

UENUM(BlueprintType)
enum class ETechCategory : uint8
{
    Military          UMETA(DisplayName = "Ëªç‰?ÁßëÊ?"),
    Economic          UMETA(DisplayName = "Á∂ìÊ?ÁßëÊ?"),
    Diplomatic        UMETA(DisplayName = "Â§ñ‰∫§ÁßëÊ?"),
    Social            UMETA(DisplayName = "Á§æÊ?ÁßëÊ?"),
    Industrial        UMETA(DisplayName = "Â∑•Ê•≠ÁßëÊ?"),
    Agricultural      UMETA(DisplayName = "Ëæ≤Ê•≠ÁßëÊ?"),
    Medical           UMETA(DisplayName = "?´Á?ÁßëÊ?"),
    Education         UMETA(DisplayName = "?ôËÇ≤ÁßëÊ?"),
    Infrastructure    UMETA(DisplayName = "?∫Á?Ë®≠ÊñΩ"),
    Communication     UMETA(DisplayName = "?öË?ÁßëÊ?")
};

UENUM(BlueprintType)
enum class ETechTier : uint8
{
    Basic             UMETA(DisplayName = "?∫Á?ÁßëÊ?"),
    Intermediate      UMETA(DisplayName = "‰∏≠Á?ÁßëÊ?"),
    Advanced          UMETA(DisplayName = "È´òÁ?ÁßëÊ?"),
    Elite             UMETA(DisplayName = "Á≤æËã±ÁßëÊ?"),
    Legendary         UMETA(DisplayName = "?≥Ë™™ÁßëÊ?")
};

UENUM(BlueprintType)
enum class ETechStatus : uint8
{
    Locked            UMETA(DisplayName = "?™Ëß£??),
    Available         UMETA(DisplayName = "?ØÁ???),
    Researching        UMETA(DisplayName = "?îÁôº‰∏?),
    Completed         UMETA(DisplayName = "Â∑≤Â???),
    Disabled          UMETA(DisplayName = "Â∑≤Á???)
};

UENUM(BlueprintType)
enum class ETechEffectType : uint8
{
    UnlockUnit        UMETA(DisplayName = "Ëß???Æ‰?"),
    UnlockBuilding    UMETA(DisplayName = "Ëß??Âª∫Á?"),
    UnlockResource    UMETA(DisplayName = "Ëß??Ë≥áÊ?"),
    StatBoost         UMETA(DisplayName = "Â±¨ÊÄßÊ???),
    CostReduction     UMETA(DisplayName = "?êÊú¨?ç‰?"),
    ProductionBonus   UMETA(DisplayName = "?üÁî¢?†Ê?"),
    ResearchBonus     UMETA(DisplayName = "?îÁôº?†Ê?"),
    DiplomaticBonus   UMETA(DisplayName = "Â§ñ‰∫§?†Ê?"),
    MilitaryBonus     UMETA(DisplayName = "Ëªç‰??†Ê?"),
    EconomicBonus     UMETA(DisplayName = "Á∂ìÊ??†Ê?"),
    SocialBonus       UMETA(DisplayName = "Á§æÊ??†Ê?"),
    SpecialAbility    UMETA(DisplayName = "?πÊ??ΩÂ?")
};

/**
 * ÁßëÊ??àÊ??∏Ê?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FTECHTREE_API FTechEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Effect")
    ETechEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Effect")
    FString TargetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Effect")
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Effect")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Effect")
    bool bIsPermanent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Effect")
    float Duration;

    FTechEffect()
    {
        EffectType = ETechEffectType::StatBoost;
        TargetID = TEXT("");
        Value = 0.0f;
        Description = TEXT("");
        bIsPermanent = true;
        Duration = 0.0f;
    }
};

/**
 * ÁßëÊ??îÁôº?êÊú¨ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FTECHTREE_API FTechCost
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Cost")
    TMap<EResourceType, float> ResourceCosts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Cost")
    float TimeCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Cost")
    int32 RequiredScientists;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Cost")
    TArray<FString> RequiredBuildings;

    FTechCost()
    {
        TimeCost = 0.0f;
        RequiredScientists = 0;
        RequiredBuildings.Empty();
    }
};

/**
 * ÁßëÊ?Ê®πÁ?ÈªûÊï∏?öÁ?Êß? */
USTRUCT(BlueprintType)
struct FTECHTREE_API FTechNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FString TechID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FString TechName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FString TechDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    ETechCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    ETechTier Tier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    ETechStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FTechCost ResearchCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    TArray<FTechEffect> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    TArray<FString> Prerequisites;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    TArray<FString> Unlocks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FVector2D Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    bool bIsKeyTechnology;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    bool bIsRepeatable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    int32 MaxResearchCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    FString HistoricalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Node")
    TArray<FString> FlavorText;

    FTechNode()
    {
        TechID = TEXT("");
        TechName = TEXT("");
        TechDescription = TEXT("");
        Category = ETechCategory::Military;
        Tier = ETechTier::Basic;
        Status = ETechStatus::Locked;
        IconPath = TEXT("");
        bIsKeyTechnology = false;
        bIsRepeatable = false;
        MaxResearchCount = 1;
        HistoricalContext = TEXT("");
        FlavorText.Empty();
        Position = FVector2D::ZeroVector;
    }
};

/**
 * ÁßëÊ??îÁôº?≤Â∫¶ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FTECHTREE_API FTechResearchProgress
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Research Progress")
    FString TechID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Research Progress")
    float Progress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Research Progress")
    float TimeRemaining;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Research Progress")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Research Progress")
    FString ResearchingBuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Research Progress")
    bool bIsPaused;

    FTechResearchProgress()
    {
        TechID = TEXT("");
        Progress = 0.0f;
        TimeRemaining = 0.0f;
        ResearchingBuildingID = TEXT("");
        bIsPaused = false;
    }
};

/**
 * ÁßëÊ?Ê®πÈ?ÁΩÆÁ?Êß? */
USTRUCT(BlueprintType)
struct FTECHTREE_API FTechTreeConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Tree Config")
    TMap<ETechCategory, float> CategoryResearchBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Tree Config")
    TMap<ETechTier, float> TierResearchBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Tree Config")
    float GlobalResearchSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Tree Config")
    int32 MaxConcurrentResearch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Tree Config")
    bool bEnableTechSharing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tech Tree Config")
    float TechSharingEfficiency;

    FTechTreeConfig()
    {
        GlobalResearchSpeed = 1.0f;
        MaxConcurrentResearch = 1;
        bEnableTechSharing = false;
        TechSharingEfficiency = 0.5f;
    }
};

// ÂÆ??‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTechResearchStarted, const FString&, TechID, const FString&, BuildingID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTechResearchCompleted, const FString&, TechID, const TArray<FTechEffect>&, Effects);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechResearchPaused, const FString&, TechID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechResearchCancelled, const FString&, TechID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTechUnlocked, const FString&, TechID, ETechCategory, Category);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechEffectApplied, const FTechEffect&, Effect);

/**
 * ÁßëÊ?Ê®πÁ≥ªÁµ±‰∏ªÈ°ûÂà•
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechTree : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTree();

    // ÁßëÊ?Ê®π‰?‰ª?    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
    FOnTechResearchStarted OnTechResearchStarted;

    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
    FOnTechResearchCompleted OnTechResearchCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
    FOnTechResearchPaused OnTechResearchPaused;

    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
    FOnTechResearchCancelled OnTechResearchCancelled;

    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
    FOnTechUnlocked OnTechUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
    FOnTechEffectApplied OnTechEffectApplied;

    // ?ùÂ??ñÁ??ÄÊ®πÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void InitializeTechTree();

    // ?≤Â??Ä?âÁ??ÄÈ°ûÂà•
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<ETechCategory> GetAllTechCategories() const;

    // ?≤Â??áÂ?È°ûÂà•?ÑÁ??Ä
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<FTechNode> GetTechsByCategory(ETechCategory Category) const;

    // ?≤Â?ÁßëÊ?ÁØÄÈªû‰ø°??    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechNode GetTechNode(const FString& TechID) const;

    // ?ãÂ?ÁßëÊ??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool StartResearch(const FString& TechID, const FString& BuildingID);

    // ?´Â?ÁßëÊ??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool PauseResearch(const FString& TechID);

    // ?ñÊ?ÁßëÊ??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool CancelResearch(const FString& TechID);

    // ÂÆåÊ?ÁßëÊ??îÁôº
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool CompleteResearch(const FString& TechID);

    // Ê™¢Êü•ÁßëÊ??ØÂê¶?ØÁ???    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    bool CanResearchTech(const FString& TechID) const;

    // Ê™¢Êü•ÁßëÊ??çÁΩÆÊ¢ù‰ª∂
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    bool CheckPrerequisites(const FString& TechID) const;

    // ?≤Â?ÁßëÊ??îÁôº?≤Â∫¶
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechResearchProgress GetResearchProgress(const FString& TechID) const;

    // ?≤Â??Ä?âÂ∑≤ÂÆåÊ??ÑÁ??Ä
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<FString> GetCompletedTechs() const;

    // ?≤Â??∂Â??îÁôº‰∏≠Á?ÁßëÊ?
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<FTechResearchProgress> GetActiveResearch() const;

    // ?âÁî®ÁßëÊ??àÊ?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void ApplyTechEffects(const FString& TechID);

    // ÁßªÈô§ÁßëÊ??àÊ?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void RemoveTechEffects(const FString& TechID);

    // Ë®àÁ??îÁôº?ÇÈ?
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    float CalculateResearchTime(const FString& TechID) const;

    // Ë®àÁ??îÁôº?êÊú¨
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechCost CalculateResearchCost(const FString& TechID) const;

    // ?≤Â?ÁßëÊ?Ê®πÈ?ÁΩ?    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechTreeConfig GetTechTreeConfig() const;

    // ?¥Êñ∞ÁßëÊ?Ê®πÈ?ÁΩ?    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void UpdateTechTreeConfig(const FTechTreeConfig& NewConfig);

    // ?¥Êñ∞ÁßëÊ??îÁôº?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void UpdateResearchProgress(float DeltaTime);

    // Ëß??ÁßëÊ?
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool UnlockTech(const FString& TechID);

    // ?çÁΩÆÁßëÊ?Ê®?    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void ResetTechTree();

    // ‰øùÂ?ÁßëÊ?Ê®πÊï∏??    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool SaveTechTreeData(const FString& SaveSlotName);

    // ËºâÂÖ•ÁßëÊ?Ê®πÊï∏??    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool LoadTechTreeData(const FString& SaveSlotName);

protected:
    // ÁßëÊ?ÁØÄÈªûÊ?Â∞?    UPROPERTY()
    TMap<FString, FTechNode> TechNodes;

    // Â∑≤Â??êÁ??Ä?ÜÂ?
    UPROPERTY()
    TSet<FString> CompletedTechs;

    // ?îÁôº?≤Â∫¶?†Â?
    UPROPERTY()
    TMap<FString, FTechResearchProgress> ResearchProgress;

    // ÁßëÊ?Ê®πÈ?ÁΩ?    UPROPERTY()
    FTechTreeConfig Config;

    // ?∂Â?Ê¥ªÂ??îÁôº
    UPROPERTY()
    TArray<FString> ActiveResearchProjects;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ??ãÊ??üÁ??ÄÊ®?    void InitializeRepublicanEraTechTree();

    // ?µÂª∫Ëªç‰?ÁßëÊ?
    void CreateMilitaryTechs();

    // ?µÂª∫Á∂ìÊ?ÁßëÊ?
    void CreateEconomicTechs();

    // ?µÂª∫Â§ñ‰∫§ÁßëÊ?
    void CreateDiplomaticTechs();

    // ?µÂª∫Á§æÊ?ÁßëÊ?
    void CreateSocialTechs();

    // ?µÂª∫Â∑•Ê•≠ÁßëÊ?
    void CreateIndustrialTechs();

    // ?µÂª∫Ëæ≤Ê•≠ÁßëÊ?
    void CreateAgriculturalTechs();

    // ?µÂª∫?´Á?ÁßëÊ?
    void CreateMedicalTechs();

    // ?µÂª∫?ôËÇ≤ÁßëÊ?
    void CreateEducationTechs();

    // ?µÂª∫?∫Á?Ë®≠ÊñΩÁßëÊ?
    void CreateInfrastructureTechs();

    // ?µÂª∫?öË?ÁßëÊ?
    void CreateCommunicationTechs();

    // ?¥Êñ∞ÁßëÊ??Ä??    void UpdateTechStatus(const FString& TechID);

    // Ê™¢Êü•‰∏¶Ëß£?ñÊñ∞ÁßëÊ?
    void CheckAndUnlockNewTechs();

    // ?âÁî®?ÆÂÄãÁ??Ä?àÊ?
    void ApplySingleTechEffect(const FTechEffect& Effect);

    // ÁßªÈô§?ÆÂÄãÁ??Ä?àÊ?
    void RemoveSingleTechEffect(const FTechEffect& Effect);

    // ?≤Â?ÁßëÊ?È°ûÂà•?çÁ®±
    FString GetTechCategoryName(ETechCategory Category) const;

    // ?≤Â?ÁßëÊ?Á≠âÁ??çÁ®±
    FString GetTechTierName(ETechTier Tier) const;

    // È©óË?ÁßëÊ??∏Ê?
    bool ValidateTechData(const FTechNode& TechNode) const;

    // ?üÊ?ÁßëÊ?ID
    FString GenerateTechID(const FString& BaseName, ETechCategory Category) const;

    // Ë®òÈ?ÁßëÊ?Ê≠∑Âè≤
    void RecordTechHistory(const FString& Event);

    // ÁßëÊ?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FString> TechHistory;
};
