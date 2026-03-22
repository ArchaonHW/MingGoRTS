#pragma once

#include "CoreMinimal.h"
#include "MingRTSFactionManager.h"
#include "MingRTSFactionSystemTest.generated.h"

USTRUCT(BlueprintType)
struct FFactionSystemTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    bool bPassed;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    FFactionSystemTestResult()
        : bPassed(false)
        , ExecutionTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FFactionSystemTestSuiteResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalTests;

    UPROPERTY(BlueprintReadOnly)
    int32 PassedTests;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedTests;

    UPROPERTY(BlueprintReadOnly)
    float TotalExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FFactionSystemTestResult> TestResults;

    FFactionSystemTestSuiteResult()
        : TotalTests(0)
        , PassedTests(0)
        , FailedTests(0)
        , TotalExecutionTime(0.0f)
    {}

    float GetPassRate() const
    {
        return TotalTests > 0 ? (float)PassedTests / TotalTests * 100.0f : 0.0f;
    }
};

/**
 * 12勢力系統測試套件
 */
UCLASS(ClassGroup = (Testing), BlueprintType)
class MINGSTRATEGIC_API UMingRTSFactionSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSFactionSystemTest();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    void InitializeTestSuite();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestSuiteResult RunAllTests();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    void RunSpecificTest(const FString& TestName);

    // 各項測試
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestFactionManagerCreation();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestAll12FactionsInitialized();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestFactionDataIntegrity();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestFactionRelations();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestPlayerFactionSetting();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestVictoryPointsSystem();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestTerritoryControl();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestWarDeclaration();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestAllianceFormation();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestFactionDefeatAndRevival();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestUniqueUnits();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestSpecialMechanics();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestAIConfiguration();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestTurnSystem();

    // 特定勢力測試
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestBeiyangGovernment();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestChineseCommunistParty();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Faction|Testing")
    FFactionSystemTestResult TestXinjiangFaction();

private:
    UPROPERTY()
    TObjectPtr<UMingRTSFactionManager> TestFactionManager;

    FFactionSystemTestResult CreateTestResult(const FString& TestName, bool bPassed, const FString& ErrorMessage = TEXT(""));
    void LogTestResult(const FFactionSystemTestResult& Result);
};
