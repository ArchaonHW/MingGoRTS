#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAudioSystemTest.generated.h"

/**
 * ?≥È†ªÁ≥ªÁµ±Ê∏¨Ë©¶?ãË??? * Ê∏¨Ë©¶?≥È†ª?áÈ?‰øÇÁ≥ªÁµ±Á??¥Â??üËÉΩ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioSystemTest : public UObject
{
    GENERATED_BODY()

public:
    /**
     * ?ãË??Ä?âÈü≥?ªÁ≥ªÁµ±Ê∏¨Ë©?     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void RunAllAudioTests(};

    /**
     * Ê∏¨Ë©¶?≥È†ª?ú‰?ÁÆ°Á??®Â?ÂßãÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioRelationshipManagerInitialization(};

    /**
     * Ê∏¨Ë©¶?ú‰?ËÆäÂ??≥È†ª?çÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRelationshipAudioFeedback(};

    /**
     * Ê∏¨Ë©¶?≤Ê?ËÆäÂ??≥È†ª?çÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestReputationAudioFeedback(};

    /**
     * Ê∏¨Ë©¶Â∞çË©±?≥È†ªÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestDialogueAudioSystem(};

    /**
     * Ê∏¨Ë©¶Ê∞ëÂ??ÇÊ??≥È†ª‰∏ªÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRepublicEraThemes(};

    /**
     * Ê∏¨Ë©¶?∞Â??≥È†ªÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestRegionalAudioSystem(};

    /**
     * Ê∏¨Ë©¶?≥È†ª?≥È??ßÂà∂
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioVolumeControl(};

    /**
     * Ê∏¨Ë©¶?≥È†ª‰∫ã‰ª∂?ûË™ø
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioEventCallbacks(};

    /**
     * Â£ìÂ?Ê∏¨Ë©¶ - Â§ßÈ??≥È†ª‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioSystemStress(};

    /**
     * ?ÜÊ?Ê∏¨Ë©¶ - ÂÆåÊï¥?ÑÈü≥?ªÈ?‰øÇÊ?Á®?     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static bool TestAudioRelationshipIntegration(};

    /**
     * ?µÂª∫Ê∏¨Ë©¶?≥È†ª?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void CreateTestAudioData(};

    /**
     * Ê∏ÖÁ?Ê∏¨Ë©¶?≥È†ª?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Audio Test")
    static void CleanupTestAudioData(};

    /**
     * È©óË??≥È†ªÁ≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio Test")
    static bool VerifyAudioSystemHealth(};

    /**
     * ?≤Â?Ê∏¨Ë©¶ÁµêÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Audio Test")
    static FString GetAudioTestReport(};

private:
    static bool bTestPassed;
    static FString TestReport;
    static int32 TestsRun;
    static int32 TestsPassed;

    static void AddTestResult(const FString& TestName, bool bPassed, const FString& Details = TEXT("")};
    static void ResetTestResults(};
};

