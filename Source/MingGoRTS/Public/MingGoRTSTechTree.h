#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.h"
#include "MingGoRTSTechTree.generated.h"

UENUM(BlueprintType)
enum class ETechCategory : uint8
{
    Military          UMETA(DisplayName = "軍事科技"),
    Economic          UMETA(DisplayName = "經濟科技"),
    Diplomatic        UMETA(DisplayName = "外交科技"),
    Social            UMETA(DisplayName = "社會科技"),
    Industrial        UMETA(DisplayName = "工業科技"),
    Agricultural      UMETA(DisplayName = "農業科技"),
    Medical           UMETA(DisplayName = "醫療科技"),
    Education         UMETA(DisplayName = "教育科技"),
    Infrastructure    UMETA(DisplayName = "基礎設施"),
    Communication     UMETA(DisplayName = "通訊科技")
};

UENUM(BlueprintType)
enum class ETechTier : uint8
{
    Basic             UMETA(DisplayName = "基礎科技"),
    Intermediate      UMETA(DisplayName = "中級科技"),
    Advanced          UMETA(DisplayName = "高級科技"),
    Elite             UMETA(DisplayName = "精英科技"),
    Legendary         UMETA(DisplayName = "傳說科技")
};

UENUM(BlueprintType)
enum class ETechStatus : uint8
{
    Locked            UMETA(DisplayName = "未解鎖"),
    Available         UMETA(DisplayName = "可研發"),
    Researching        UMETA(DisplayName = "研發中"),
    Completed         UMETA(DisplayName = "已完成"),
    Disabled          UMETA(DisplayName = "已禁用")
};

UENUM(BlueprintType)
enum class ETechEffectType : uint8
{
    UnlockUnit        UMETA(DisplayName = "解鎖單位"),
    UnlockBuilding    UMETA(DisplayName = "解鎖建築"),
    UnlockResource    UMETA(DisplayName = "解鎖資源"),
    StatBoost         UMETA(DisplayName = "屬性提升"),
    CostReduction     UMETA(DisplayName = "成本降低"),
    ProductionBonus   UMETA(DisplayName = "生產加成"),
    ResearchBonus     UMETA(DisplayName = "研發加成"),
    DiplomaticBonus   UMETA(DisplayName = "外交加成"),
    MilitaryBonus     UMETA(DisplayName = "軍事加成"),
    EconomicBonus     UMETA(DisplayName = "經濟加成"),
    SocialBonus       UMETA(DisplayName = "社會加成"),
    SpecialAbility    UMETA(DisplayName = "特殊能力")
};

/**
 * 科技效果數據結構
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
 * 科技研發成本結構
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
 * 科技樹節點數據結構
 */
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
 * 科技研發進度結構
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
 * 科技樹配置結構
 */
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

// 宣告事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTechResearchStarted, const FString&, TechID, const FString&, BuildingID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTechResearchCompleted, const FString&, TechID, const TArray<FTechEffect>&, Effects);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechResearchPaused, const FString&, TechID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechResearchCancelled, const FString&, TechID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTechUnlocked, const FString&, TechID, ETechCategory, Category);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTechEffectApplied, const FTechEffect&, Effect);

/**
 * 科技樹系統主類別
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSTechTree : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSTechTree();

    // 科技樹事件
    UPROPERTY(BlueprintAssignable, Category = "Tech Tree")
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

    // 初始化科技樹系統
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void InitializeTechTree();

    // 獲取所有科技類別
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<ETechCategory> GetAllTechCategories() const;

    // 獲取指定類別的科技
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<FTechNode> GetTechsByCategory(ETechCategory Category) const;

    // 獲取科技節點信息
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechNode GetTechNode(const FString& TechID) const;

    // 開始科技研發
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool StartResearch(const FString& TechID, const FString& BuildingID);

    // 暫停科技研發
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool PauseResearch(const FString& TechID);

    // 取消科技研發
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool CancelResearch(const FString& TechID);

    // 完成科技研發
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool CompleteResearch(const FString& TechID);

    // 檢查科技是否可研發
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    bool CanResearchTech(const FString& TechID) const;

    // 檢查科技前置條件
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    bool CheckPrerequisites(const FString& TechID) const;

    // 獲取科技研發進度
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechResearchProgress GetResearchProgress(const FString& TechID) const;

    // 獲取所有已完成的科技
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<FString> GetCompletedTechs() const;

    // 獲取當前研發中的科技
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    TArray<FTechResearchProgress> GetActiveResearch() const;

    // 應用科技效果
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void ApplyTechEffects(const FString& TechID);

    // 移除科技效果
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void RemoveTechEffects(const FString& TechID);

    // 計算研發時間
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    float CalculateResearchTime(const FString& TechID) const;

    // 計算研發成本
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechCost CalculateResearchCost(const FString& TechID) const;

    // 獲取科技樹配置
    UFUNCTION(BlueprintPure, Category = "Tech Tree")
    FTechTreeConfig GetTechTreeConfig() const;

    // 更新科技樹配置
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void UpdateTechTreeConfig(const FTechTreeConfig& NewConfig);

    // 更新科技研發進度
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void UpdateResearchProgress(float DeltaTime);

    // 解鎖科技
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool UnlockTech(const FString& TechID);

    // 重置科技樹
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    void ResetTechTree();

    // 保存科技樹數據
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool SaveTechTreeData(const FString& SaveSlotName);

    // 載入科技樹數據
    UFUNCTION(BlueprintCallable, Category = "Tech Tree")
    bool LoadTechTreeData(const FString& SaveSlotName);

protected:
    // 科技節點映射
    UPROPERTY()
    TMap<FString, FTechNode> TechNodes;

    // 已完成科技集合
    UPROPERTY()
    TSet<FString> CompletedTechs;

    // 研發進度映射
    UPROPERTY()
    TMap<FString, FTechResearchProgress> ResearchProgress;

    // 科技樹配置
    UPROPERTY()
    FTechTreeConfig Config;

    // 當前活動研發
    UPROPERTY()
    TArray<FString> ActiveResearchProjects;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國時期科技樹
    void InitializeRepublicanEraTechTree();

    // 創建軍事科技
    void CreateMilitaryTechs();

    // 創建經濟科技
    void CreateEconomicTechs();

    // 創建外交科技
    void CreateDiplomaticTechs();

    // 創建社會科技
    void CreateSocialTechs();

    // 創建工業科技
    void CreateIndustrialTechs();

    // 創建農業科技
    void CreateAgriculturalTechs();

    // 創建醫療科技
    void CreateMedicalTechs();

    // 創建教育科技
    void CreateEducationTechs();

    // 創建基礎設施科技
    void CreateInfrastructureTechs();

    // 創建通訊科技
    void CreateCommunicationTechs();

    // 更新科技狀態
    void UpdateTechStatus(const FString& TechID);

    // 檢查並解鎖新科技
    void CheckAndUnlockNewTechs();

    // 應用單個科技效果
    void ApplySingleTechEffect(const FTechEffect& Effect);

    // 移除單個科技效果
    void RemoveSingleTechEffect(const FTechEffect& Effect);

    // 獲取科技類別名稱
    FString GetTechCategoryName(ETechCategory Category) const;

    // 獲取科技等級名稱
    FString GetTechTierName(ETechTier Tier) const;

    // 驗證科技數據
    bool ValidateTechData(const FTechNode& TechNode) const;

    // 生成科技ID
    FString GenerateTechID(const FString& BaseName, ETechCategory Category) const;

    // 記錄科技歷史
    void RecordTechHistory(const FString& Event);

    // 科技歷史記錄
    UPROPERTY()
    TArray<FString> TechHistory;
};
