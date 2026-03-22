#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingBuildingLayer.generated.h"

// 建築類型
UENUM(BlueprintType)
enum class EMingStratBuildingType : uint8
{
    Residential,     // 住宅
    Commercial,      // 商業
    Industrial,      // 工業
    Military,        // 軍事
    Educational,     // 教育
    Medical,         // 醫療
    Cultural,        // 文化
    Agricultural,    // 農業
    Infrastructure,   // 基礎設施
    Government       // 政府
};

// 建築狀態
UENUM(BlueprintType)
enum class EMingBuildingState : uint8
{
    Planning,        // 規劃中
    UnderConstruction, // 建設中
    Operational,     // 運營中
    Upgrading,       // 升級中
    Damaged,         // 損壞
    Destroyed,       // 摧毀
    Abandoned        // 廢棄
};

// 建築等級
UENUM(BlueprintType)
enum class EMingBuildingLevel : uint8
{
    Level1,          // 等級1
    Level2,          // 等級2
    Level3,          // 等級3
    Level4,          // 等級4
    Level5           // 等級5
};

// 建築信息
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

// 建設項目
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

// 建設決策
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

// 建設事件
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
 * 建設層系統
 * 負責基地建設、資源管理、經濟運營等
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingBuildingLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // 建構子
    UMingBuildingLayer();

    // 實現介面方法
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingGameEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // 建築管理
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

    // 建設項目管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void StartConstructionProject(const FMingConstructionProject& Project);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void CancelConstructionProject(const FString& ProjectID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void PrioritizeProject(const FString& ProjectID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FMingConstructionProject> GetConstructionQueue() const;

    // 資源管理
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void AllocateResources(const TMap<EMingResourceType, float>& Allocation);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TMap<EMingResourceType, float> GetResourceProduction() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TMap<EMingResourceType, float> GetResourceConsumption() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    float GetResourceEfficiency() const;

    // 建設決策
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void MakeBuildingDecision(const FMingBuildingDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    void ProcessBuildingDecisions();

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FMingBuildingDecision> GetBuildingDecisions() const;

    // 經濟分析
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    float CalculateEconomicOutput() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    float CalculateInfrastructureLevel() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FString> IdentifyEconomicOpportunities() const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    TArray<FString> AssessEconomicThreats() const;

    // 獲取建設狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|Building")
    FMingBuildingState GetBuildingState() const;

protected:
    // 建設狀態
    UPROPERTY(BlueprintReadOnly)
    FMingBuildingState BuildingState;

    // 建築列表
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingBuildingInfo> Buildings;

    // 建設項目
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingConstructionProject> ConstructionQueue;

    // 建設決策
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingBuildingDecision> BuildingDecisions;

    // 建設事件歷史
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingBuildingEvent> BuildingHistory;

    // 建設風格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    FString BuildingStyle;

    // 經濟發展策略
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    FString EconomicDevelopmentStrategy;

    // 基礎設施投資偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    float InfrastructureInvestmentPreference;

    // 工業化程度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Settings")
    float IndustrializationLevel;

    // 分析建設狀況
    void AnalyzeConstructionStatus();

    // 評估經濟狀況
    void AssessEconomicConditions();

    // 計算建設效率
    float CalculateConstructionEfficiency();

    // 預測建設結果
    TArray<FString> PredictConstructionOutcome(const FMingConstructionProject& Project);

    // 民國特色建設
    void ApplyRepublicanEraConstruction();

    // 近代化工業建設
    void ExecuteModernIndustrialConstruction();

    // 城市現代化建設
    void ExecuteUrbanModernization();

    // 交通基礎設施建設
    void ExecuteTransportInfrastructureConstruction();

    // 教育設施建設
    void ExecuteEducationalFacilityConstruction();

    // 醫療設施建設
    void ExecuteMedicalFacilityConstruction();

    // 文化設施建設
    void ExecuteCulturalFacilityConstruction();

    // 農業現代化建設
    void ExecuteAgriculturalModernization();

private:
    // 建設更新間隔
    float BuildingUpdateInterval;

    // 上次更新時間
    float LastBuildingUpdate;

    // 最大建築數量
    int32 MaxBuildingCount;

    // 建設速度加成
    float ConstructionSpeedBonus;

    // 經濟增長率
    float EconomicGrowthRate;

    // 建設威脅評估
    TMap<FString, float> ConstructionThreats;

    // 建設機會評估
    TMap<FString, float> ConstructionOpportunities;

    // 建設決策歷史
    TArray<FMingBuildingDecision> DecisionHistory;
};
