#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSInnovationSystemTest.generated.h"

UCLASS(ClassGroup = (MingStrategic), BlueprintType)
class MINGSTRATEGIC_API UMingRTSInnovationSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSInnovationSystemTest();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Innovation|Test")
    void InitializeInnovationTestSuite();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Innovation|Test")
    bool RunCollaborativeAITests();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Innovation|Test")
    bool RunEcosystemSimulationTests();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Innovation|Test")
    bool RunSocialDynamicsTests();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Innovation|Test")
    bool RunBlockchainIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Innovation|Test")
    bool RunAllInnovationTests();

    UFUNCTION(BlueprintPure, Category = "MingRTS|Innovation|Test")
    int32 GetTotalTests() const { return TotalTests; }

    UFUNCTION(BlueprintPure, Category = "MingRTS|Innovation|Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "MingRTS|Innovation|Test")
    int32 GetFailedTests() const { return FailedTests; }

private:
    UPROPERTY()
    int32 TotalTests;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 FailedTests;

    UPROPERTY()
    TMap<FString, bool> TestResults;

    bool TestCollaborativeAINetworkCreation();
    bool TestAgentRegistration();
    bool TestCollaborativeDecisionMaking();
    bool TestEcosystemZoneCreation();
    bool TestResourceManagement();
    bool TestClimateSimulation();
    bool TestSpeciesPopulation();
    bool TestPollutionEffects();
    bool TestSocialAgentCreation();
    bool TestSocialRelationEstablishment();
    bool TestSocialMobility();
    bool TestSocietyMetrics();
    bool TestBlockchainConnection();
    bool TestAccountCreation();
    bool TestTransactionCreation();
    bool TestTokenMinting();

    void LogTestResult(const FString& TestName, bool bPassed);
};
