#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSaveSystemTest.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingSaveGameManager;
class UMingRelationshipManager;

/**
 * �O??�t�δ��ծM��
 * ��??�O?X??X?�t��?X??X?��
 */
UCLASS()
class MINGPERSONAL_API UMingSaveSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingSaveSystemTest();

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Save System Test")
    bool RunAllTests();

    // ���յ�??
    UFUNCTION(BlueprintPure, Category = "Save System Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Save System Test")
    float GetTestPassRate() const;

    // ??�O����
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
    // ���յ�?X???
    UPROPERTY()
    FString TestResults;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 FailedTests;

    UPROPERTY()
    TArray<FString> ErrorMessages;

    // �������U??��
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT("")) {};
    void ResetTestCounts() {};

    // �M�z���զs��
    void CleanupTestSaves() {};

    // �Ыش��խӤH�޲z��
    UMingPersonalManager* CreateTestPersonalManager() {};

    // �Ыش��զs�ɺ޲z��
    UMingSaveGameManager* CreateTestSaveManager() {};
};


#endif // MINGSAVESYSTEMTEST_H
