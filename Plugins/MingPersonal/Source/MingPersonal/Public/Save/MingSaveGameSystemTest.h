#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveGameSystemTest.generated.h"

// Test result enum
UENUM(BlueprintType)
enum class ESaveGameTestResult : uint8
{
    NotRun            UMETA(DisplayName = "Not Run"),
    Passed            UMETA(DisplayName = "Passed"),
    Failed            UMETA(DisplayName = "Failed"),
    Skipped           UMETA(DisplayName = "Skipped"),
    Error             UMETA(DisplayName = "Error")
};

// Test case structure
USTRUCT(BlueprintType)
struct FSaveGameTestCase
{
    GENERATED_BODY()

    UPROPERTY()
    FString TestName;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    ESaveGameTestResult Result;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    float ExecutionTime;

    FSaveGameTestCase()
        : Result(ESaveGameTestResult::NotRun)
        , ExecutionTime(0.0f)
    {}
};

// Forward declarations
class UMingSaveGameManager;
class UMingSaveGame;

/**
 * ‰øùÂ?Á≥ªÁµ±Ê∏¨Ë©¶Â•ó‰ª∂
 * È©óË?‰øùÂ?Á≥ªÁµ±?ÑÊ†∏ÂøÉÂ??? */
UCLASS()
class MINGPERSONAL_API UMingSaveGameSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingSaveGameSystemTest(};

    // ?ãË??Ä?âÊ∏¨Ë©?    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool RunAllTests(};

    // Ê∏¨Ë©¶ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    float GetTestPassRate() const;

    UFUNCTION(BlueprintPure, Category = "Save Game Test")
    TArray<FSaveGameTestCase> GetTestCases() const { return TestCases; }

    // ?ãÂà•Ê∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestSaveGameManagerCreation(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestCreateSaveGame(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestSaveToSlot(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestLoadFromSlot(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestQuickSave(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestQuickLoad(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestDeleteSave(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestGetAllSlots(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestSaveExists(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestMostRecentSave(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestAutosave(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestExportSave(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestImportSave(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestBackupRestore(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestCompression(};

    UFUNCTION(BlueprintCallable, Category = "Save Game Test")
    bool TestVersionCompatibility(};

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
    TArray<FSaveGameTestCase> TestCases;

    // Ê∏¨Ë©¶Âπ´Âä©?ΩÊï∏
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")};
    void ResetTestCounts(};
    void AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error = TEXT("")};

    // ?µÂª∫Ê∏¨Ë©¶?®Á?ÁÆ°Á???    UMingSaveGameManager* CreateTestSaveGameManager(};

    // Ê∏ÖÁ?Ê∏¨Ë©¶?∏Ê?
    void CleanupTestData(};

    // ?≤Â?Ê∏¨Ë©¶?®Á?Â≠òÊ?Êß?    int32 GetTestSaveSlot() const { return 99; }
};

