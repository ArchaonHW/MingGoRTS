#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveSystemTest.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingSaveGameManager;
class UMingRelationshipManager;

/**
 * ‰øùÂ?Á≥ªÁµ±Ê∏¨Ë©¶Â•ó‰ª∂
 * È©óË?‰øùÂ??åË??•Á≥ªÁµ±Á??∏Â??üËÉΩ
 */
UCLASS()
class MINGPERSONAL_API UMingSaveSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingSaveSystemTest();

    // ?ãË??Ä?âÊ∏¨Ë©?    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool RunAllTests();

    // Ê∏¨Ë©¶ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Save System Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    float GetTestPassRate() const;

    // ?ãÂà•Ê∏¨Ë©¶
    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveGameCreation();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveAndLoad();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestMultipleSaveSlots();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestQuickSave();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestDeleteSave();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveSlotInfo();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestAutoSaveSettings();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestRelationshipDataPersistence();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestReputationDataPersistence();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestExportImport();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestSaveValidation();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestVersionCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool TestCompressionEncryption();

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

    // Ê∏¨Ë©¶Âπ´Âä©?ΩÊï∏
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));
    void ResetTestCounts();

    // Ê∏ÖÁ?Ê∏¨Ë©¶Â≠òÊ?
    void CleanupTestSaves();

    // ?µÂª∫Ê∏¨Ë©¶?®Á??ã‰∫∫ÁÆ°Á???    UMingPersonalManager* CreateTestPersonalManager();

    // ?µÂª∫Ê∏¨Ë©¶?®Á?‰øùÂ?ÁÆ°Á???    UMingSaveGameManager* CreateTestSaveManager();
};
