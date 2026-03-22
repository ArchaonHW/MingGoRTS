#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSocialDynamics.generated.h"

UENUM(BlueprintType)
enum class ESocialClass: uint8 {
    Peasant UMETA(DisplayName = "農民"),
    Artisan UMETA(DisplayName = "工匠"),
    Merchant UMETA(DisplayName = "商人"),
    Scholar UMETA(DisplayName = "士人"),
    Official UMETA(DisplayName = "官吏"),
    Noble UMETA(DisplayName = "貴族"),
    Military UMETA(DisplayName = "軍人"),
    Religious UMETA(DisplayName = "宗教人士"),
    Outlaw UMETA(DisplayName = "流民/盜賊"),
    Count UMETA(DisplayName = "階層數量")
};

UENUM(BlueprintType)
enum class ESocialRelationType: uint8 {
    Family UMETA(DisplayName = "家族"),
    Friend UMETA(DisplayName = "朋友"),
    Rival UMETA(DisplayName = "競爭對手"),
    Mentor UMETA(DisplayName = "師徒"),
    Business UMETA(DisplayName = "商業"),
    Political UMETA(DisplayName = "政治"),
    Religious UMETA(DisplayName = "宗教"),
    Enemy UMETA(DisplayName = "仇敵"),
    Neutral UMETA(DisplayName = "中立"),
    Count UMETA(DisplayName = "關係類型數量")
};

USTRUCT(BlueprintType)
struct FSocialAgent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 AgentId;

    UPROPERTY(BlueprintReadWrite)
    FString AgentName;

    UPROPERTY(BlueprintReadWrite)
    ESocialClass SocialClass;

    UPROPERTY(BlueprintReadWrite)
    float Wealth;

    UPROPERTY(BlueprintReadWrite)
    float Influence;

    UPROPERTY(BlueprintReadWrite)
    float Reputation;

    UPROPERTY(BlueprintReadWrite)
    TMap<int32, ESocialRelationType> SocialRelations;

    UPROPERTY(BlueprintReadWrite)
    TMap<int32, float> RelationStrength;

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> FactionAffiliations;

    FSocialAgent()
        : AgentId(0)
        , SocialClass(ESocialClass::Peasant)
        , Wealth(50.0f)
        , Influence(10.0f)
        , Reputation(50.0f)
    {}
};

USTRUCT(BlueprintType)
struct FSocietyMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float SocialStability;

    UPROPERTY(BlueprintReadOnly)
    float EconomicInequality;

    UPROPERTY(BlueprintReadOnly)
    float SocialMobility;

    UPROPERTY(BlueprintReadOnly)
    float CulturalCohesion;

    UPROPERTY(BlueprintReadOnly)
    float AverageHappiness;

    UPROPERTY(BlueprintReadOnly)
    TMap<ESocialClass, float> ClassDistribution;

    FSocietyMetrics()
        : SocialStability(0.5f)
        , EconomicInequality(0.5f)
        , SocialMobility(0.5f)
        , CulturalCohesion(0.5f)
        , AverageHappiness(0.5f)
    {}
};

UCLASS(ClassGroup = (MingStrategic), BlueprintType)
class MINGSTRATEGIC_API UMingRTSSocialDynamics : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSocialDynamics();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Social|System")
    void InitializeSocialSystem();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Social|Agents")
    int32 CreateSocialAgent(const FString& Name, ESocialClass SocialClass);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Social|Relations")
    void EstablishRelation(int32 AgentA, int32 AgentB, ESocialRelationType RelationType, float Strength);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Social|Simulation")
    void SimulateSocialTick(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Social|Analytics")
    FSocietyMetrics GetSocietyMetrics() const;

private:
    UPROPERTY()
    TMap<int32, FSocialAgent> SocialAgents;

    UPROPERTY()
    int32 NextAgentId;

    void UpdateSocialMobility();
    void SpreadReputation();
    void ResolveSocialConflicts();
};
