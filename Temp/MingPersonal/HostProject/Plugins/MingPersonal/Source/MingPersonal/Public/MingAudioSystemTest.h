#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAudioSystemTest.generated.h"

/**
 * 音頻系統測試運行器
 * 測試音頻與關係系統的整合功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 運行所有音頻系統測試
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void RunAllAudioTests();

    /**
     * 測試音頻關係管理器初始化
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioRelationshipManagerInitialization();

    /**
     * 測試關係變化音頻反饋
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRelationshipAudioFeedback();

    /**
     * 測試聲望變化音頻反饋
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestReputationAudioFeedback();

    /**
     * 測試對話音頻系統
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestDialogueAudioSystem();

    /**
     * 測試民國時期音頻主題
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRepublicEraThemes();

    /**
     * 測試地區音頻系統
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRegionalAudioSystem();

    /**
     * 測試音頻音量控制
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioVolumeControl();

    /**
     * 測試音頻事件回調
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioEventCallbacks();

    /**
     * 壓力測試 - 大量音頻事件
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioSystemStress();

    /**
     * 集成測試 - 完整的音頻關係流程
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioRelationshipIntegration();

    /**
     * 創建測試音頻數據
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void CreateTestAudioData();

    /**
     * 清理測試音頻數據
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void CleanupTestAudioData();

    /**
     * 驗證音頻系統狀態
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio Test")
    static bool VerifyAudioSystemHealth();

    /**
     * 獲取測試結果報告
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio Test")
    static FString GetAudioTestReport();

private:
    static bool bTestPassed;
    static FString TestReport;
    static int32 TestsRun;
    static int32 TestsPassed;

    static void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT(""));
    static void ResetTestResults();
};
