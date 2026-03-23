#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSixStrategies.h"
#include "MingPositiveStrategies.generated.h"

// 立國策略結構
USTRUCT(BlueprintType)
struct FEstablishNationParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NationName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CapitalCity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> InitialTerritories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartingResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InitialPopulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GovernmentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> NationalPolicies;

    FEstablishNationParams()
    {
        NationName = TEXT("新國家");
        CapitalCity = TEXT("首都");
        StartingResources = 1000.0f;
        InitialPopulation = 10000;
        GovernmentType = TEXT("君主制");
    }
};

// 立國結果
USTRUCT(BlueprintType)
struct FEstablishNationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EstablishedNationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StabilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EconomicPotential;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryPotential;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EstablishedInstitutions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FEstablishNationResult()
    {
        bSuccess = false;
        StabilityScore = 0.0f;
        EconomicPotential = 0.0f;
        MilitaryPotential = 0.0f;
    }
};

// 立制策略結構
USTRUCT(BlueprintType)
struct FEstablishSystemParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SystemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetNationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SystemComponents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ImplementationCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredOfficials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SupportingLaws;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExpectedEfficiency;

    FEstablishSystemParams()
    {
        SystemType = TEXT("行政系統");
        ImplementationCost = 500.0f;
        RequiredOfficials = 10;
        ExpectedEfficiency = 0.7f;
    }
};

// 立制結果
USTRUCT(BlueprintType)
struct FEstablishSystemResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EstablishedSystemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ActualEfficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PublicAcceptance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdministrativeCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ImplementedLaws;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FEstablishSystemResult()
    {
        bSuccess = false;
        ActualEfficiency = 0.0f;
        PublicAcceptance = 0.5f;
        AdministrativeCost = 0.0f;
    }
};

// 立人策略結構
USTRUCT(BlueprintType)
struct FEstablishPeopleParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TargetPeopleIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EducationFocus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TrainingInvestment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SkillPriorities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LoyaltyBuilding;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RecruitmentQuota;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> MoralTeachings;

    FEstablishPeopleParams()
    {
        EducationFocus = TEXT("文武並重");
        TrainingInvestment = 300.0f;
        LoyaltyBuilding = 0.5f;
        RecruitmentQuota = 100;
    }
};

// 立人結果
USTRUCT(BlueprintType)
struct FEstablishPeopleResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TrainedPeople;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageSkillLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageLoyalty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> DevelopedTalents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SocialStabilityImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FEstablishPeopleResult()
    {
        bSuccess = false;
        TrainedPeople = 0;
        AverageSkillLevel = 0.0f;
        AverageLoyalty = 0.0f;
        SocialStabilityImpact = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNationEstablished, const FEstablishNationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemEstablished, const FEstablishSystemResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeopleEstablished, const FEstablishPeopleResult&, Result);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingPositiveStrategies : public UObject
{
    GENERATED_BODY()

public:
    UMingPositiveStrategies();

    // 立國策 - 建立國家基礎
    UFUNCTION(BlueprintCallable, Category = "Positive Strategies|Establish Nation")
    FEstablishNationResult ExecuteEstablishNation(const FEstablishNationParams& Params);

    // 立制策 - 建立制度規範
    UFUNCTION(BlueprintCallable, Category = "Positive Strategies|Establish System")
    FEstablishSystemResult ExecuteEstablishSystem(const FEstablishSystemParams& Params);

    // 立人策 - 培養人才體系
    UFUNCTION(BlueprintCallable, Category = "Positive Strategies|Establish People")
    FEstablishPeopleResult ExecuteEstablishPeople(const FEstablishPeopleParams& Params);

    // 輔助功能
    UFUNCTION(BlueprintPure, Category = "Positive Strategies")
    float CalculateNationStability(const FEstablishNationParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Positive Strategies")
    float CalculateSystemEfficiency(const FEstablishSystemParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Positive Strategies")
    float CalculatePeopleDevelopment(const FEstablishPeopleParams& Params) const;

    // 驗證功能
    UFUNCTION(BlueprintPure, Category = "Positive Strategies")
    bool CanEstablishNation(const FEstablishNationParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Positive Strategies")
    bool CanEstablishSystem(const FEstablishSystemParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Positive Strategies")
    bool CanEstablishPeople(const FEstablishPeopleParams& Params) const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Positive Strategies")
    FOnNationEstablished OnNationEstablished;

    UPROPERTY(BlueprintAssignable, Category = "Positive Strategies")
    FOnSystemEstablished OnSystemEstablished;

    UPROPERTY(BlueprintAssignable, Category = "Positive Strategies")
    FOnPeopleEstablished OnPeopleEstablished;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Positive Strategies")
    void OnEstablishNationStarted(const FEstablishNationParams& Params);

    UFUNCTION(BlueprintImplementableEvent, Category = "Positive Strategies")
    void OnEstablishSystemStarted(const FEstablishSystemParams& Params);

    UFUNCTION(BlueprintImplementableEvent, Category = "Positive Strategies")
    void OnEstablishPeopleStarted(const FEstablishPeopleParams& Params);

private:
    void InitializeDefaultValues();
    
    float CalculateTerritoryValue(const TArray<FString>& Territories) const;
    float CalculateResourceEfficiency(float Resources, int32 Population) const;
    float CalculateSystemCompatibility(const FString& GovernmentType, const FString& SystemType) const;
    float CalculateTeachingEffectiveness(const TArray<FString>& MoralTeachings) const;
    
    TArray<FString> GenerateInstitutions(const FEstablishNationParams& Params) const;
    TArray<FString> GenerateLaws(const FEstablishSystemParams& Params) const;
    TArray<FString> IdentifyTalents(const TArray<FString>& PeopleIds) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positive Strategies")
    TMap<FString, FEstablishNationResult> NationHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positive Strategies")
    TMap<FString, FEstablishSystemResult> SystemHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positive Strategies")
    TMap<FString, FEstablishPeopleResult> PeopleHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Positive Strategies")
    float BaseSuccessRate;
};
