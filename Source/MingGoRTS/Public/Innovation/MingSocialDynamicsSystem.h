#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "MingSocialDynamicsSystem.generated.h"

// 社會關係類型枚舉
UENUM(BlueprintType)
enum class ESocialRelationType : uint8
{
    Family = 0,            // 家庭關係
    Friendship = 1,        // 友誼關係
    Romantic = 2,          // 戀愛關係
    Professional = 3,      // 職業關係
    Political = 4,         // 政治關係
    Economic = 5,          // 經濟關係
    Religious = 6,         // 宗教關係
    Educational = 7,       // 教育關係
    Community = 8,         // 社區關係
    Rivalry = 9            // 競爭關係
};

// 社會階層類型枚舉
UENUM(BlueprintType)
enum class ESocialClass : uint8
{
    Upper = 0,             // 上層階級
    UpperMiddle = 1,        // 中上層階級
    Middle = 2,             // 中層階級
    LowerMiddle = 3,        // 中下層階級
    Lower = 4,              // 下層階級
    Outcast = 5             // 被排斥者
};

// 社會事件類型枚舉
UENUM(BlueprintType)
enum class ESocialEventType : uint8
{
    Birth = 0,              // 出生
    Death = 1,              // 死亡
    Marriage = 2,            // 婚姻
    Divorce = 3,            // 離婚
    Graduation = 4,         // 畢業
    Employment = 5,          // 就業
    Unemployment = 6,        // 失業
    Promotion = 7,          // 晉升
    Demotion = 8,           // 降職
    Migration = 9,           // 遷移
    Conflict = 10,           // 衝突
    Cooperation = 11,        // 合作
    Scandal = 12,           // 醜聞
    Achievement = 13,        // 成就
    Disaster = 14            // 災難
};

// 文化傳播類型枚舉
UENUM(BlueprintType)
enum class ECulturalTransmissionType : uint8
{
    Vertical = 0,           // 垂直傳播（代際）
    Horizontal = 1,         // 水平傳播（同輩）
    Oblique = 2,            // 斜向傳播（跨代）
    MassMedia = 3,           // 大眾媒體
    Education = 4,           // 教育系統
    Religious = 5,           // 宗教機構
    Government = 6,          // 政府機構
    Internet = 7,           // 網絡傳播
    SocialNetwork = 8,        // 社交網絡
    Community = 9            // 社區傳播
};

// 群體行為類型枚舉
UENUM(BlueprintType)
enum class EGroupBehaviorType : uint8
{
    Cooperation = 0,         // 合作
    Competition = 1,         // 競爭
    Conflict = 2,            // 衝突
    Conformity = 3,          // 從眾
    Innovation = 4,          // 創新
    Migration = 5,           // 遷移
    Segregation = 6,         // 隔離
    Integration = 7,         // 融合
    Polarization = 8,        // 極化
    Mobilization = 9         // 動員
};

// 社會個體數據結構
USTRUCT(BlueprintType)
struct FSocialIndividual
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString IndividualID;

    UPROPERTY(BlueprintReadOnly)
    FString FirstName;

    UPROPERTY(BlueprintReadOnly)
    FString LastName;

    UPROPERTY(BlueprintReadOnly)
    int32 Age = 0;

    UPROPERTY(BlueprintReadOnly)
    FString Gender;

    UPROPERTY(BlueprintReadOnly)
    ESocialClass SocialClass = ESocialClass::Middle;

    UPROPERTY(BlueprintReadOnly)
    FString Occupation;

    UPROPERTY(BlueprintReadOnly)
    FString Education;

    UPROPERTY(BlueprintReadOnly)
    FString Location;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> FamilyMembers;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Friends;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Colleagues;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> Relationships;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Skills;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Beliefs;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Values;

    UPROPERTY(BlueprintReadOnly)
    float SocialInfluence = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float EconomicStatus = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float PoliticalPower = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime BirthDate;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdated;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive = true;
};

// 社會關係數據結構
USTRUCT(BlueprintType)
struct FSocialRelationship
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RelationshipID;

    UPROPERTY(BlueprintReadOnly)
    FString IndividualA_ID;

    UPROPERTY(BlueprintReadOnly)
    FString IndividualB_ID;

    UPROPERTY(BlueprintReadOnly)
    ESocialRelationType RelationType = ESocialRelationType::Friendship;

    UPROPERTY(BlueprintReadOnly)
    float Strength = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Trust = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Influence = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Duration = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SharedInterests;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SharedValues;

    UPROPERTY(BlueprintReadOnly)
    FString RelationshipStatus;

    UPROPERTY(BlueprintReadOnly)
    FDateTime FormationDate;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastInteraction;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive = true;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> RelationshipMetadata;
};

// 社會事件數據結構
USTRUCT(BlueprintType)
struct FSocialEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    ESocialEventType EventType = ESocialEventType::Birth;

    UPROPERTY(BlueprintReadOnly)
    FString EventName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Participants;

    UPROPERTY(BlueprintReadOnly)
    FString Location;

    UPROPERTY(BlueprintReadOnly)
    float Impact = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Duration = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRelationships;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SocialConsequences;

    UPROPERTY(BlueprintReadOnly)
    bool bIsPublic = true;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRecurring = false;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventParameters;
};

// 文化元素數據結構
USTRUCT(BlueprintType)
struct FCulturalElement
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ElementID;

    UPROPERTY(BlueprintReadOnly)
    FString ElementName;

    UPROPERTY(BlueprintReadOnly)
    FString ElementCategory;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AssociatedValues;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AssociatedBeliefs;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AssociatedPractices;

    UPROPERTY(BlueprintReadOnly)
    float AdoptionRate = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float TransmissionStrength = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Carriers;

    UPROPERTY(BlueprintReadOnly)
    FDateTime OriginTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<ECulturalTransmissionType> TransmissionModes;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> RegionalVariations;

    UPROPERTY(BlueprintReadOnly)
    bool bIsDominant = false;

    UPROPERTY(BlueprintReadOnly)
    bool bIsEvolving = true;
};

// 群體行為數據結構
USTRUCT(BlueprintType)
struct FGroupBehavior
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString BehaviorID;

    UPROPERTY(BlueprintReadOnly)
    FString GroupID;

    UPROPERTY(BlueprintReadOnly)
    EGroupBehaviorType BehaviorType = EGroupBehaviorType::Cooperation;

    UPROPERTY(BlueprintReadOnly)
    FString BehaviorName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Participants;

    UPROPERTY(BlueprintReadOnly)
    float Intensity = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Duration = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Triggers;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Outcomes;

    UPROPERTY(BlueprintReadOnly)
    float SocialImpact = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsOngoing = false;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> BehaviorParameters;
};

// 社會網絡數據結構
USTRUCT(BlueprintType)
struct FSocialNetwork
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString NetworkID;

    UPROPERTY(BlueprintReadOnly)
    FString NetworkName;

    UPROPERTY(BlueprintReadOnly)
    FString NetworkType;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Nodes;

    UPROPERTY(BlueprintReadOnly)
    TArray<FSocialRelationship> Edges;

    UPROPERTY(BlueprintReadOnly)
    float Density = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float ClusteringCoefficient = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float AveragePathLength = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InfluentialNodes;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Communities;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CreationTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdated;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive = true;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> NetworkProperties;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIndividualCreated, const FSocialIndividual&, Individual);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelationshipFormed, const FSocialRelationship&, Relationship);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSocialEventOccurred, const FSocialEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCulturalElementSpread, const FCulturalElement&, Element);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroupBehaviorStarted, const FGroupBehavior&, Behavior);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSocialClassChanged, const FString&, IndividualID, ESocialClass NewClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSocialInfluenceChanged, const FString&, IndividualID, float NewInfluence);

/**
 * 社會動態系統接口
 * 提供複雜的社會關係網絡、社會階層流動、文化傳播和群體行為模擬
 */
UINTERFACE(BlueprintType)
class USocialDynamicsSystemInterface : public UInterface
{
    GENERATED_BODY()
};

class ISocialDynamicsSystemInterface
{
    GENERATED_BODY()

public:
    // 初始化系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    bool InitializeSocialDynamicsSystem();

    // 創建社會個體
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    FString CreateIndividual(const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass);

    // 建立社會關係
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    FString EstablishRelationship(const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength);

    // 觸發社會事件
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    FString TriggerSocialEvent(ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact);

    // 傳播文化元素
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    bool SpreadCulturalElement(const FString& ElementID, const TArray<FString>& Targets, ECulturalTransmissionType TransmissionType);

    // 模擬群體行為
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    FString SimulateGroupBehavior(const FString& GroupID, EGroupBehaviorType BehaviorType, const TArray<FString>& Participants);

    // 處理社會階層流動
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    bool ProcessSocialMobility(const FString& IndividualID, ESocialClass TargetClass);

    // 計算社會影響力
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    float CalculateSocialInfluence(const FString& IndividualID);

    // 更新社會網絡
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    bool UpdateSocialNetwork(const FString& NetworkID, float DeltaTime);

    // 模擬文化演變
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    bool SimulateCulturalEvolution(float DeltaTime);

    // 獲取個體信息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    FSocialIndividual GetIndividual(const FString& IndividualID) const;

    // 獲取系統統計信息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Social Dynamics System")
    TMap<FString, float> GetSystemStatistics() const;
};

/**
 * 社會動態系統
 * 實現複雜的社會關係網絡、社會階層流動、文化傳播和群體行為模擬
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Innovation))
class MINGGORTS_API UMingSocialDynamicsSystem : public UObject, public ISocialDynamicsSystemInterface
{
    GENERATED_BODY()

public:
    UMingSocialDynamicsSystem();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool InitializeSocialDynamicsSystem() override;

    // 創建社會個體
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    FString CreateIndividual(const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass) override;

    // 建立社會關係
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    FString EstablishRelationship(const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength) override;

    // 觸發社會事件
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    FString TriggerSocialEvent(ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact) override;

    // 傳播文化元素
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool SpreadCulturalElement(const FString& ElementID, const TArray<FString>& Targets, ECulturalTransmissionType TransmissionType) override;

    // 模擬群體行為
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    FString SimulateGroupBehavior(const FString& GroupID, EGroupBehaviorType BehaviorType, const TArray<FString>& Participants) override;

    // 處理社會階層流動
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool ProcessSocialMobility(const FString& IndividualID, ESocialClass TargetClass) override;

    // 計算社會影響力
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    float CalculateSocialInfluence(const FString& IndividualID) override;

    // 更新社會網絡
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool UpdateSocialNetwork(const FString& NetworkID, float DeltaTime) override;

    // 模擬文化演變
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool SimulateCulturalEvolution(float DeltaTime) override;

    // 獲取個體信息
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    FSocialIndividual GetIndividual(const FString& IndividualID) const override;

    // 獲取系統統計信息
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TMap<FString, float> GetSystemStatistics() const override;

    // 高級功能
    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TArray<FSocialIndividual> GetAllIndividuals() const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TArray<FSocialRelationship> GetRelationships(const FString& IndividualID) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TArray<FSocialEvent> GetRecentEvents(int32 Count = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TArray<FCulturalElement> GetActiveCulturalElements() const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TArray<FGroupBehavior> GetActiveGroupBehaviors() const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    FSocialNetwork GetSocialNetwork(const FString& NetworkID) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool CreateSocialNetwork(const FString& NetworkID, const FString& NetworkName, const FString& NetworkType);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool RemoveIndividual(const FString& IndividualID);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool UpdateRelationship(const FString& RelationshipID, float NewStrength, float NewTrust);

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    float CalculateNetworkDensity(const FString& NetworkID) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    TArray<FString> FindInfluentialIndividuals(int32 Count = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Social Dynamics System")
    bool SimulateSocialConflict(const FString& GroupA_ID, const FString& GroupB_ID, float ConflictIntensity);

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnIndividualCreated OnIndividualCreated;

    UPROPERTY(BlueprintAssignable)
    FOnRelationshipFormed OnRelationshipFormed;

    UPROPERTY(BlueprintAssignable)
    FOnSocialEventOccurred OnSocialEventOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnCulturalElementSpread OnCulturalElementSpread;

    UPROPERTY(BlueprintAssignable)
    FOnGroupBehaviorStarted OnGroupBehaviorStarted;

    UPROPERTY(BlueprintAssignable)
    FOnSocialClassChanged OnSocialClassChanged;

    UPROPERTY(BlueprintAssignable)
    FOnSocialInfluenceChanged OnSocialInfluenceChanged;

protected:
    // 系統組件
    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    TMap<FString, FSocialIndividual> Individuals;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    TMap<FString, FSocialRelationship> Relationships;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    TArray<FSocialEvent> SocialEvents;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    TMap<FString, FCulturalElement> CulturalElements;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    TArray<FGroupBehavior> GroupBehaviors;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    TMap<FString, FSocialNetwork> SocialNetworks;

    // 配置參數
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    float SimulationSpeed = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    float RelationshipDecayRate = 0.01f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    float CulturalTransmissionRate = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    float SocialMobilityProbability = 0.05f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    float GroupBehaviorThreshold = 0.7f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    int32 MaxIndividuals = 1000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    int32 MaxRelationshipsPerIndividual = 50;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    bool bEnableCulturalEvolution = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    bool bEnableSocialMobility = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Social Dynamics System")
    bool bEnableGroupBehaviors = true;

    // 狀態變數
    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    bool bIsInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    int32 ActiveIndividualCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    int32 ActiveRelationshipCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    float AverageSocialInfluence = 1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Social Dynamics System")
    ESocialClass DominantSocialClass = ESocialClass::Middle;

private:
    // 內部方法
    FString GenerateUniqueIndividualID() const;
    FString GenerateUniqueRelationshipID() const;
    FString GenerateUniqueEventID() const;
    FString GenerateUniqueElementID() const;
    FString GenerateUniqueBehaviorID() const;
    FString GenerateUniqueNetworkID() const;

    FSocialIndividual CreateDefaultIndividual(const FString& IndividualID, const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass);
    FSocialRelationship CreateDefaultRelationship(const FString& RelationshipID, const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength);
    FSocialEvent CreateDefaultEvent(const FString& EventID, ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact);
    FCulturalElement CreateDefaultCulturalElement(const FString& ElementID, const FString& ElementName, const FString& ElementCategory);
    FGroupBehavior CreateDefaultGroupBehavior(const FString& BehaviorID, const FString& GroupID, EGroupBehaviorType BehaviorType, const TArray<FString>& Participants);
    FSocialNetwork CreateDefaultSocialNetwork(const FString& NetworkID, const FString& NetworkName, const FString& NetworkType);

    void UpdateRelationships(float DeltaTime);
    void ProcessSocialEvents(float DeltaTime);
    void UpdateCulturalElements(float DeltaTime);
    void UpdateGroupBehaviors(float DeltaTime);
    void ProcessSocialMobility(float DeltaTime);

    float CalculateRelationshipStrength(const FString& IndividualA_ID, const FString& IndividualB_ID) const;
    float CalculateCulturalAdoption(const FString& IndividualID, const FString& ElementID) const;
    TArray<FString> FindSocialConnections(const FString& IndividualID, int32 MaxDepth = 2) const;
    ESocialClass DetermineSocialClass(const FSocialIndividual& Individual) const;

    bool ValidateIndividualCreation(const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass) const;
    bool ValidateRelationshipEstablishment(const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength) const;
    bool ValidateSocialEvent(ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact) const;

    // 定時器
    FTimerHandle SimulationTimerHandle;
    FTimerHandle RelationshipUpdateTimerHandle;
    FTimerHandle EventProcessingTimerHandle;
    FTimerHandle CulturalEvolutionTimerHandle;
    FTimerHandle SocialMobilityTimerHandle;

    // 統計數據
    UPROPERTY()
    TMap<FString, float> SystemStats;

    UPROPERTY()
    float LastSimulationTime = 0.0f;

    UPROPERTY()
    int32 TotalEventsProcessed = 0;

    UPROPERTY()
    int32 TotalRelationshipsFormed = 0;

    UPROPERTY()
    int32 TotalCulturalTransmissions = 0;
};
