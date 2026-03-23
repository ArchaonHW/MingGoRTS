#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"
#include "Components/AutoComponent.h"
#include "GameFramework/Actor.h"
#include "MingSocialDynamicsSystem.generated.h"

// 社會關係類型枚舉
UENUM(BlueprintType)
enum class ESocialRelationType : uint8
{
    Family = 0,        // 家庭關係
    Friendship = 1,    // 友誼關係
    Romantic = 2,      // 戀愛關係
    Professional = 3,  // 職業關係
    Political = 4,     // 政治關係
    Economic = 5,      // 經濟關係
    Religious = 6,     // 宗教關係
    Educational = 7,   // 教育關係
    Community = 8,     // 社區關係
    Rivalry = 9        // 競爭關係
};

// 社會階層類型枚舉
UENUM(BlueprintType)
enum class ESocialClass : uint8
{
    Upper = 0,         // 上層階級
    UpperMiddle = 1,   // 中上層階級
    Middle = 2,        // 中層階級
    LowerMiddle = 3,   // 中下層階級
    Lower = 4,         // 下層階級
    Outcast = 5        // 被排斥者
};

// 社會事件類型枚舉
UENUM(BlueprintType)
enum class ESocialEventType : uint8
{
    Birth = 0,         // 出生
    Death = 1,         // 死亡
    Marriage = 2,      // 婚姻
    Divorce = 3,       // 離婚
    Graduation = 4,    // 畢業
    Employment = 5,    // 就業
    Unemployment = 6,  // 失業
    Promotion = 7,     // 晉升
    Demotion = 8,      // 降職
    Migration = 9,     // 遷移
    SocialMovement = 10 // 社會運動
};

// 文化傳播類型枚舉
UENUM(BlueprintType)
enum class ECulturalTransmissionType : uint8
{
    Vertical = 0,      // 垂直傳播（代際傳播）
    Horizontal = 1,    // 水平傳播（同輩傳播）
    Oblique = 2,       // 斜向傳播（跨代非直系）
    MassMedia = 3,     // 大眾媒體傳播
    Education = 4,     // 教育傳播
    Religious = 5,     // 宗教傳播
    Technology = 6     // 技術傳播
};

// 社會網絡結構類型枚舉
UENUM(BlueprintType)
enum class ESocialNetworkStructure : uint8
{
    SmallWorld = 0,    // 小世界網絡
    ScaleFree = 1,     // 無標度網絡
    Random = 2,        // 隨機網絡
    Hierarchical = 3,  // 階層網絡
    Tribal = 4,        // 部落網絡
    Institutional = 5  // 制度網絡
};

USTRUCT(BlueprintType)
struct FSocialRelationship
{
    GENERATED_BODY()

    UPROPERTY()
    FString IndividualA;

    UPROPERTY()
    FString IndividualB;

    UPROPERTY()
    ESocialRelationType RelationType;

    UPROPERTY()
    float RelationshipStrength;

    UPROPERTY()
    float TrustLevel;

    UPROPERTY()
    float InfluenceLevel;

    UPROPERTY()
    FDateTime EstablishedDate;

    UPROPERTY()
    TArray<FString> SharedInterests;

    UPROPERTY()
    TArray<FString> SharedValues;

    FSocialRelationship()
        : RelationshipStrength(0.0f)
        , TrustLevel(0.0f)
        , InfluenceLevel(0.0f)
        , EstablishedDate(FDateTime::Now())
        , RelationType(ESocialRelationType::Friendship)
    {}
};

USTRUCT(BlueprintType)
struct FSocialIndividual
{
    GENERATED_BODY()

    UPROPERTY()
    FString IndividualID;

    UPROPERTY()
    FString Name;

    UPROPERTY()
    ESocialClass SocialClass;

    UPROPERTY()
    int32 Age;

    UPROPERTY()
    FString Occupation;

    UPROPERTY()
    FString Location;

    UPROPERTY()
    TArray<FString> Skills;

    UPROPERTY()
    TArray<FString> Beliefs;

    UPROPERTY()
    TArray<FString> Values;

    UPROPERTY()
    TArray<FString> Interests;

    UPROPERTY()
    float SocialInfluence;

    UPROPERTY()
    float EconomicStatus;

    UPROPERTY()
    float EducationLevel;

    UPROPERTY()
    TArray<FSocialRelationship> Relationships;

    FSocialIndividual()
        : SocialClass(ESocialClass::Middle)
        , Age(25)
        , SocialInfluence(0.0f)
        , EconomicStatus(0.0f)
        , EducationLevel(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FSocialEvent
{
    GENERATED_BODY()

    UPROPERTY()
    FString EventID;

    UPROPERTY()
    ESocialEventType EventType;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    FDateTime EventDate;

    UPROPERTY()
    FString Location;

    UPROPERTY()
    TArray<FString> Participants;

    UPROPERTY()
    TArray<FString> Witnesses;

    UPROPERTY()
    float SocialImpact;

    UPROPERTY()
    float EconomicImpact;

    UPROPERTY()
    float CulturalImpact;

    UPROPERTY()
    TMap<FString, float> IndividualImpacts;

    FSocialEvent()
        : EventType(ESocialEventType::Birth)
        , EventDate(FDateTime::Now())
        , SocialImpact(0.0f)
        , EconomicImpact(0.0f)
        , CulturalImpact(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FCulturalTrait
{
    GENERATED_BODY()

    UPROPERTY()
    FString TraitID;

    UPROPERTY()
    FString TraitName;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    ECulturalTransmissionType TransmissionType;

    UPROPERTY()
    float AdoptionRate;

    UPROPERTY()
    float MutationRate;

    UPROPERTY()
    TArray<FString> Adopters;

    UPROPERTY()
    TMap<FString, float> RegionalVariations;

    UPROPERTY()
    FDateTime OriginDate;

    FCulturalTrait()
        : TransmissionType(ECulturalTransmissionType::Horizontal)
        , AdoptionRate(0.0f)
        , MutationRate(0.0f)
        , OriginDate(FDateTime::Now())
    {}
};

USTRUCT(BlueprintType)
struct FSocialNetworkMetrics
{
    GENERATED_BODY()

    UPROPERTY()
    float NetworkDensity;

    UPROPERTY()
    float AveragePathLength;

    UPROPERTY()
    float ClusteringCoefficient;

    UPROPERTY()
    int32 ConnectedComponents;

    UPROPERTY()
    float Modularity;

    UPROPERTY()
    int32 TotalNodes;

    UPROPERTY()
    int32 TotalEdges;

    UPROPERTY()
    TMap<FString, float> NodeCentrality;

    FSocialNetworkMetrics()
        : NetworkDensity(0.0f)
        , AveragePathLength(0.0f)
        , ClusteringCoefficient(0.0f)
        , ConnectedComponents(0)
        , Modularity(0.0f)
        , TotalNodes(0)
        , TotalEdges(0)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocialRelationshipChanged, const FSocialRelationship&, Relationship);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocialEventOccurred, const FSocialEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCulturalTraitSpread, const FCulturalTrait&, Trait);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocialClassChanged, const FString&, IndividualID);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSocialDynamicsSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSocialDynamicsSystem();

    // 系統初始化和關閉
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void InitializeSocialDynamicsSystem();

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void ShutdownSocialDynamicsSystem();

    // 個體管理
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void AddIndividual(const FSocialIndividual& Individual);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void RemoveIndividual(const FString& IndividualID);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    FSocialIndividual GetIndividual(const FString& IndividualID) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TArray<FSocialIndividual> GetAllIndividuals() const;

    // 關係管理
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void CreateRelationship(const FSocialRelationship& Relationship);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void UpdateRelationship(const FSocialRelationship& Relationship);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void RemoveRelationship(const FString& IndividualA, const FString& IndividualB);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TArray<FSocialRelationship> GetIndividualRelationships(const FString& IndividualID) const;

    // 社會事件管理
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void CreateSocialEvent(const FSocialEvent& Event);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void ProcessSocialEvent(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TArray<FSocialEvent> GetRecentEvents(int32 Count = 10) const;

    // 文化傳播管理
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void AddCulturalTrait(const FCulturalTrait& Trait);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void SpreadCulturalTrait(const FString& TraitID, ECulturalTransmissionType TransmissionType);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TArray<FCulturalTrait> GetCulturalTraits() const;

    // 社會網絡分析
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    FSocialNetworkMetrics AnalyzeSocialNetwork() const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    float CalculateSocialInfluence(const FString& IndividualID) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TArray<FString> FindInfluentialIndividuals(int32 Count = 10) const;

    // 社會階層流動
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void ProcessSocialMobility();

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void PromoteIndividual(const FString& IndividualID);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void DemoteIndividual(const FString& IndividualID);

    // 群體行為模擬
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void SimulateGroupBehavior();

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    void ProcessSocialContagion(const FString& TraitID, float ContagionRate);

    // 統計和分析
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TMap<ESocialClass, int32> GetClassDistribution() const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    TMap<ESocialRelationType, int32> GetRelationshipTypeDistribution() const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics")
    float CalculateSocialCohesion() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnSocialRelationshipChanged OnSocialRelationshipChanged;

    UPROPERTY(BlueprintAssignable)
    FOnSocialEventOccurred OnSocialEventOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnCulturalTraitSpread OnCulturalTraitSpread;

    UPROPERTY(BlueprintAssignable)
    FOnSocialClassChanged OnSocialClassChanged;

protected:
    // 系統更新
    virtual void Tick(float DeltaTime);

    // 內部處理函數
    void UpdateRelationships();
    void ProcessCulturalEvolution();
    void UpdateSocialNetwork();
    void HandleSocialConflict();

private:
    // 數據存儲
    UPROPERTY()
    TMap<FString, FSocialIndividual> Individuals;

    UPROPERTY()
    TArray<FSocialRelationship> Relationships;

    UPROPERTY()
    TArray<FSocialEvent> SocialEvents;

    UPROPERTY()
    TArray<FCulturalTrait> CulturalTraits;

    // 系統配置
    UPROPERTY()
    float RelationshipDecayRate;

    UPROPERTY()
    float CulturalMutationRate;

    UPROPERTY()
    float SocialMobilityRate;

    UPROPERTY()
    int32 MaxRelationshipsPerIndividual;

    // 系統狀態
    UPROPERTY()
    bool bSystemInitialized;

    UPROPERTY()
    FDateTime LastUpdateTime;

    // 內部輔助函數
    void ValidateRelationship(FSocialRelationship& Relationship);
    void CalculateRelationshipStrength(FSocialRelationship& Relationship);
    void ProcessSocialImpact(const FSocialEvent& Event);
    void UpdateIndividualInfluence(const FString& IndividualID);
    void HandleClassTransition(const FString& IndividualID, ESocialClass OldClass, ESocialClass NewClass);
};
