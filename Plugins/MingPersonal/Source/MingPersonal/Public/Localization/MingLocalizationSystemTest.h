#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLocalizationSystemTest.generated.h"

// Test result enum
UENUM(BlueprintType)
enum class ELocalizationTestResult : uint8
{
    NotRun            UMETA(DisplayName = "Not Run"),
    Passed            UMETA(DisplayName = "Passed"),
    Failed            UMETA(DisplayName = "Failed"),
    Skipped           UMETA(DisplayName = "Skipped"),
    Error             UMETA(DisplayName = "Error")
};

// Test case structure
USTRUCT(BlueprintType)
struct FLocalizationTestCase
{
    GENERATED_BODY()

    UPROPERTY()
    FString TestName;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    ELocalizationTestResult Result;

    UPROPERTY()
    FString ErrorMessage;

    UPROPERTY()
    float ExecutionTime;

    FLocalizationTestCase()
        : Result(ELocalizationTestResult::NotRun)
        , ExecutionTime(0.0f)
    {}
};

// Forward declarations
class UMingLocalizationManager;
class UMingLanguageSystem;
class UMingCultureAdapter;
class UMingRegionalContentManager;

/**
 * 本地化系統測試套件
 * 驗證本地化系統的核心功能
 */
UCLASS()
class MINGPERSONAL_API UMingLocalizationSystemTest : public UObject
{
    GENERATED_BODY()

public:
    UMingLocalizationSystemTest();

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool RunAllTests();

    // 測試結果
    UFUNCTION(BlueprintPure, Category = "Localization Test")
    FString GetTestResults() const { return TestResults; }

    UFUNCTION(BlueprintPure, Category = "Localization Test")
    int32 GetPassedTests() const { return PassedTests; }

    UFUNCTION(BlueprintPure, Category = "Localization Test")
    int32 GetFailedTests() const { return FailedTests; }

    UFUNCTION(BlueprintPure, Category = "Localization Test")
    float GetTestPassRate() const;

    UFUNCTION(BlueprintPure, Category = "Localization Test")
    TArray<FLocalizationTestCase> GetTestCases() const { return TestCases; }

    // 個別測試
    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestLocalizationManagerCreation();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestLanguageSwitching();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestTextLocalization();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestParameterizedText();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestLanguagePackInfo();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestRegionSetting();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestNumberFormatting();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestDateFormatting();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestCurrencyFormatting();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestLanguageDetection();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestChineseConversion();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestCulturalAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestColorMeanings();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestNumberMeanings();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestRegionalContentAvailability();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestContentAdaptation();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestRightToLeftDetection();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestTranslationQuality();

    UFUNCTION(BlueprintCallable, Category = "Localization Test")
    bool TestExportImport();

private:
    // 測試結果日誌
    UPROPERTY()
    FString TestResults;

    UPROPERTY()
    int32 PassedTests;

    UPROPERTY()
    int32 FailedTests;

    UPROPERTY()
    TArray<FString> ErrorMessages;

    UPROPERTY()
    TArray<FLocalizationTestCase> TestCases;

    // 測試幫助函數
    void LogTestResult(const FString& TestName, bool bPassed, const FString& Message = TEXT(""));
    void ResetTestCounts();
    void AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error = TEXT(""));

    // 創建測試用的管理器
    UMingLocalizationManager* CreateTestLocalizationManager();
};
