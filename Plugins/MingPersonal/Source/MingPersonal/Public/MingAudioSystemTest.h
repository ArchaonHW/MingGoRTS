#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAudioSystemTest.generated.h"

/**
 * ??頻系統測試???X * 測試??頻摧毀係系統?X??X?能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 目標數量音??系統測??     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void RunAllAudioTests();

    /**
     * 測試??頻摧毀管?X???始??
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioRelationshipManagerInitialization();

    /**
     * 測試摧毀變?X?頻摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRelationshipAudioFeedback();

    /**
     * 測試摧毀變?X?頻摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestReputationAudioFeedback();

    /**
     * 測試對話??頻系統
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestDialogueAudioSystem();

    /**
     * 測試民?X??X?頻主??
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRepublicEraThemes();

    /**
     * 測試???X?頻系統
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRegionalAudioSystem();

    /**
     * 測試??頻時間限制
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioVolumeControl();

    /**
     * 測試??頻事件??調
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioEventCallbacks();

    /**
     * 壓??測試 - 大?X?頻事件
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioSystemStress();

    /**
     * 摧毀測試 - 完整??音摧毀係摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioRelationshipIntegration();

    /**
     * ??建測試??頻摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void CreateTestAudioData();

    /**
     * 清??測試??頻摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void CleanupTestAudioData();

    /**
     * 驗?X?頻系統??X     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio Test")
    static bool VerifyAudioSystemHealth();

    /**
     * 摧毀測試結?X???
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio Test")
    static FString GetAudioTestReport();

private:
    static bool bTestPassed;
    static FString TestReport;
    static int32 TestsRun;
    static int32 TestsPassed;

    static void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};
    static void ResetTestResults();
);

