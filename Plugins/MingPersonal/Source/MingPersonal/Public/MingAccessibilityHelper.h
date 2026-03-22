#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingAIUIManager.h"
#include "MingBehaviorAnalytics.h"
#include "MingAccessibilityHelper.generated.h"

UENUM(BlueprintType)
enum class EAccessibilityFeature : uint8
{
    HighContrast,          // 高對比度
    LargeText,            // 大字體
    ScreenReader,         // 屏幕閱讀器
    ColorBlindMode,       // 色盲模式
    ReducedMotion,        // 減少動畫
    KeyboardNavigation,   // 鍵盤導航
    VoiceControl,         // 語音控制
    VisualCues,           // 視覺提示
    AudioDescriptions,    // 音頻描述
    SimplifiedUI          // 簡化界面
};

UENUM(BlueprintType)
enum class EDisabilityType : uint8
{
    VisualImpairment,     // 視覺障礙
    HearingImpairment,    // 聽覺障礙
    MotorImpairment,      // 運動障礙
    CognitiveImpairment,   // 認知障礙
    ColorBlindness,       // 色盲
    None                  // 無障礙
};

USTRUCT(BlueprintType)
struct FAccessibilityProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    EDisabilityType DisabilityType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    TArray<EAccessibilityFeature> EnabledFeatures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    float TextScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    float ContrastLevel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    bool bHighContrastMode = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    bool bReduceMotion = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    bool bScreenReaderEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Profile")
    FLinearColor CustomColorScheme;

    FAccessibilityProfile()
    {
        DisabilityType = EDisabilityType::None;
        TextScale = 1.0f;
        ContrastLevel = 1.0f;
        bHighContrastMode = false;
        bReduceMotion = false;
        bScreenReaderEnabled = false;
        CustomColorScheme = FLinearColor::White;
    }
};

USTRUCT(BlueprintType)
struct FAccessibilitySettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Settings")
    FAccessibilityProfile Profile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Settings")
    TMap<EAccessibilityFeature, bool> FeatureStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Settings")
    TMap<FString, FString> CustomKeyBindings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Settings")
    float VoiceCommandSensitivity = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accessibility Settings")
    bool bAutoDetectNeeds = true;

    FAccessibilitySettings()
    {
        VoiceCommandSensitivity = 0.7f;
        bAutoDetectNeeds = true;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAccessibilityProfileChanged, const FAccessibilityProfile&, Profile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAccessibilityFeatureToggled, EAccessibilityFeature, Feature, bool, bEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAccessibilityNeedDetected, EDisabilityType, DisabilityType);

/**
 * AI驅動的輔助功能助手
 * 智能檢測和適應用戶的輔助功能需求
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAccessibilityHelper : public UObject
{
    GENERATED_BODY()

public:
    UMingAccessibilityHelper();

    // 初始化輔助功能系統
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void InitializeAccessibility(UMingAIUIManager* InAIManager, UMingBehaviorAnalytics* InAnalytics);

    // 需求檢測
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void DetectAccessibilityNeeds();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    EDisabilityType AnalyzeUserBehaviorForAccessibility();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void MonitorInteractionPatterns();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    bool ShouldEnableFeature(EAccessibilityFeature Feature);

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetAccessibilityProfile(const FAccessibilityProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    FAccessibilityProfile GetAccessibilityProfile() const { return CurrentProfile; }

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void EnableFeature(EAccessibilityFeature Feature);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void DisableFeature(EAccessibilityFeature Feature);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    bool IsFeatureEnabled(EAccessibilityFeature Feature) const;

    // UI適應
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ApplyAccessibilityToUI();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void AdjustTextSize(float Scale);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetHighContrastMode(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetColorBlindMode(EDisabilityType ColorBlindType);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ReduceMotion(bool bReduce);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void EnableKeyboardNavigation();

    // 語音控制
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void StartVoiceControl();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void StopVoiceControl();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ProcessVoiceCommand(const FString& Command);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    TArray<FString> GetAvailableVoiceCommands() const;

    // 屏幕閱讀器
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void EnableScreenReader();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void DisableScreenReader();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ReadElement(const FString& ElementText);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ReadUIPanel(EPersonalUIType PanelType);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetReadingSpeed(float Speed);

    // 鍵盤導航
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetupKeyboardNavigation();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void NavigateToElement(const FString& ElementID);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ActivateCurrentElement();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetCustomKeyBinding(const FString& Action, const FString& Key);

    // 視覺輔助
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ShowVisualCues();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void HighlightInteractiveElements();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void AddFocusIndicators();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ShowElementDescriptions();

    // 音頻輔助
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void EnableAudioDescriptions();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void PlayAudioDescription(const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetAudioDescriptionVolume(float Volume);

    // 智能適應
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void AutoOptimizeForUser();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void LearnFromUserFeedback();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SuggestImprovements();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    TArray<EAccessibilityFeature> GetRecommendedFeatures() const;

    // 測試和驗證
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void RunAccessibilityTest();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    bool ValidateUIAccessibility();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    FString GenerateAccessibilityReport() const;

    // 狀態查詢
    UFUNCTION(BlueprintPure, Category = "Accessibility")
    bool IsAccessibilityEnabled() const { return bAccessibilityEnabled; }

    UFUNCTION(BlueprintPure, Category = "Accessibility")
    int32 GetEnabledFeatureCount() const;

    UFUNCTION(BlueprintPure, Category = "Accessibility")
    TArray<EAccessibilityFeature> GetEnabledFeatures() const;

    UFUNCTION(BlueprintPure, Category = "Accessibility")
    float GetAccessibilityScore() const;

    // 設置
    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetAccessibilityEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SetAutoDetectionEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void SaveAccessibilitySettings();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void LoadAccessibilitySettings();

    UFUNCTION(BlueprintCallable, Category = "Accessibility")
    void ResetToDefaults();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Accessibility Events")
    FOnAccessibilityProfileChanged OnAccessibilityProfileChanged;

    UPROPERTY(BlueprintAssignable, Category = "Accessibility Events")
    FOnAccessibilityFeatureToggled OnAccessibilityFeatureToggled;

    UPROPERTY(BlueprintAssignable, Category = "Accessibility Events")
    FOnAccessibilityNeedDetected OnAccessibilityNeedDetected;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingAIUIManager> AIManager;

    UPROPERTY()
    TObjectPtr<UMingBehaviorAnalytics> BehaviorAnalytics;

    // 配置
    UPROPERTY()
    bool bAccessibilityEnabled = true;

    UPROPERTY()
    bool bAutoDetectionEnabled = true;

    UPROPERTY()
    FAccessibilityProfile CurrentProfile;

    UPROPERTY()
    FAccessibilitySettings Settings;

    // 檢測數據
    UPROPERTY()
    TMap<EDisabilityType, float> DisabilityScores;

    UPROPERTY()
    TArray<float> InteractionSpeedHistory;

    UPROPERTY()
    TArray<float> ClickAccuracyHistory;

    UPROPERTY()
    TArray<float> KeyboardUsageHistory;

    // 語音控制
    UPROPERTY()
    bool bVoiceControlEnabled = false;

    UPROPERTY()
    TMap<FString, FString> VoiceCommandMap;

    // 屏幕閱讀器
    UPROPERTY()
    bool bScreenReaderEnabled = false;

    UPROPERTY()
    float ReadingSpeed = 1.0f;

    // 內部函數
    void AnalyzeInteractionSpeed();
    void AnalyzeClickAccuracy();
    void AnalyzeKeyboardUsage();
    void DetectVisualImpairment();
    void DetectHearingImpairment();
    void DetectMotorImpairment();
    void DetectCognitiveImpairment();
    void ApplyAccessibilityChanges();
    void UpdateUIForAccessibility();
    float CalculateDisabilityScore(EDisabilityType DisabilityType) const;

    // AI輔助
    void TrainAccessibilityModel();
    EDisabilityType PredictDisabilityType() const;
    TArray<EAccessibilityFeature> RecommendFeaturesForDisability(EDisabilityType DisabilityType) const;

private:
    // 輔助函數
    void InitializeDefaultProfiles();
    void SetupEventListeners();
    void SaveAccessibilityData();
    void LoadAccessibilityData();
    FString GetAccessibilityDataPath() const;
};
