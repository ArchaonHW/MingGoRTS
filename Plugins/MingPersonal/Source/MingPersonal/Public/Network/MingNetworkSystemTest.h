#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingNetworkSystemTest.generated.h"

// Forward declarations
class UMingNetworkManager;
class UMingLobbySystem;
class UMingRelationshipReplication;
class UMingReputationReplication;
class UMingPersonalManager;

// Test result enum
UENUM(BlueprintType)
enum class ENetworkTestResult : uint8
{
    NotRun            UMETA(DisplayName = "Not Run"),
    Passed            UMETA(DisplayName = "Passed"),
    Failed            UMETA(DisplayName = "Failed"),
    Skipped           UMETA(DisplayName = "Skipped"),
    Error             UMETA(DisplayName = "Error")
};

// Test case structure
USTRUCT(BlueprintType)
struct FNetworkTestCase
{
    GENERATED_BODY()

    UPROPERTY()
    FString TestName;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    ENetworkTestResult Result;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    float ExecutionTime;

    FNetworkTestCase()
        : Result(ENetworkTestResult::NotRun)
        , ExecutionTime(0.0f)
    {}
};

/**
 * Á∂≤Áµ°Á≥ªÁµ±Ê∏¨Ë©¶Â•ó‰ª∂
 * È©óË?Â§ö‰∫∫?äÊà≤Á∂≤Áµ°Á≥ªÁµ±?ÑÊ†∏ÂøÉÂ??? */
UCLASS()
class MINGPERSONAL_API UMingNetworkSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingNetworkSystemTest(};

    // ?ãË??Ä?âÊ∏¨Ë©?    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool RunAllTests(};

    // Ê∏¨Ë©¶ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Network Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Network Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Network Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Network Test")
    float GetTestPassRate() const;

    UFUNCTION(BlueprintPure, Category = "Network Test")
    TArray<FNetworkTestCase> GetTestCases() const { return TestCases; }

    // ?ãÂà•Ê∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestNetworkManagerCreation(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestServerCreation(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestClientConnection(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestDisconnection(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestReconnection(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestPlayerManagement(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestLobbyCreation(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestLobbyJoinLeave(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestReadySystem(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestGameStart(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestRelationshipReplication(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestReputationReplication(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestChatSystem(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestHostMigration(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestNetworkStats(};

    UFUNCTION(BlueprintCallable, Category = "Network Test")
    bool TestErrorHandling(};

private:
    // Ê∏¨Ë©¶ÁµêÊ??•Ë?
    UPROPERTY()
    FString TestResults;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 FailedTests;

    UPROPERTY()
    TArray<FString> ErrorMessages;

    UPROPERTY()
    TArray<FNetworkTestCase> TestCases;

    // Ê∏¨Ë©¶Âπ´Âä©?ΩÊï∏
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};
    void ResetTestCounts(};
    void AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error = TEXT("")};

    // ?µÂª∫Ê∏¨Ë©¶?®Á?ÁÆ°Á???    UMingPersonalManager* CreateTestPersonalManager(};
    UMingNetworkManager* CreateTestNetworkManager(};
    UMingLobbySystem* CreateTestLobbySystem(};
};

