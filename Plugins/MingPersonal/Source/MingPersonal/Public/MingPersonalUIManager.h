#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingPersonalUIManager.generated.h"

class UMingRelationshipManager;
class UMingAudioRelationshipManager;
class UMingRepublicEraAudioThemes;

UENUM(BlueprintType)
enum class EPersonalUIType : uint8
{
    RelationshipPanel,     // ?ú‰??¢Êùø
    ReputationPanel,       // ?≤Ê??¢Êùø
    DialoguePanel,         // Â∞çË©±?¢Êùø
    QuestPanel,            // ‰ªªÂ??¢Êùø
    AudioPanel,            // ?≥È†ª?¢Êùø
    MainDashboard,         // ‰∏ªÂ?Ë°®Êùø
    CharacterDetails,      // ËßíËâ≤Ë©≥Ê?
    RegionDetails,         // ?∞Â?Ë©≥Ê?
    SystemSettings         // Á≥ªÁµ±Ë®≠ÁΩÆ
};

UENUM(BlueprintType)
enum class EUIAnimationType : uint8
{
    None,                  // ?°Â???    Fade,                  // Ê∑°ÂÖ•Ê∑°Âá∫
    Slide,                 // ÊªëÂ?
    Scale,                 // Á∏ÆÊîæ
    Rotate,                // ?ãË?
    Bounce                 // ÂΩàË∑≥
};

USTRUCT(BlueprintType)
struct FPersonalUIConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    EPersonalUIType UIType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    FString WidgetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    FString WidgetPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    bool bIsVisible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    bool bIsInteractive = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    EUIAnimationType OpenAnimation = EUIAnimationType::Fade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    EUIAnimationType CloseAnimation = EUIAnimationType::Fade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Config")
    float AnimationDuration = 0.3f;

    FPersonalUIConfig()
    {
        UIType = EPersonalUIType::RelationshipPanel;
        WidgetName = TEXT(""};
        WidgetPath = TEXT(""};
        bIsVisible = false;
        bIsInteractive = true;
        OpenAnimation = EUIAnimationType::Fade;
        CloseAnimation = EUIAnimationType::Fade;
        AnimationDuration = 0.3f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIPanelOpened, EPersonalUIType, UIType, UUserWidget*, Widget};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIPanelClosed, EPersonalUIType, UIType, UUserWidget*, Widget};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIStateChanged, bool, bIsAnyPanelOpen};

/**
 * ?ã‰∫∫Á≥ªÁµ±UIÁÆ°Á??? * ÁÆ°Á??Ä?âÈ?‰øÇ„ÄÅËÅ≤?õ„ÄÅÂ?Ë©±Á?UI?åÈù¢
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingPersonalUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPersonalUIManager(};

    // ?ùÂ??ñUIÁ≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void InitializeUIManager(UMingRelationshipManager* RelationshipManager, 
                            UMingAudioRelationshipManager* AudioManager,
                            UMingRepublicEraAudioThemes* ThemeManager};

    // UI?¢ÊùøÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void OpenUIPanel(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void CloseUIPanel(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ToggleUIPanel(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void CloseAllPanels(};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowMainDashboard(};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void HideMainDashboard(};

    // WidgetÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    UUserWidget* GetWidget(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    bool IsWidgetVisible(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void SetWidgetVisibility(EPersonalUIType UIType, bool bVisible};

    // UI?çÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void AddUIConfig(const FPersonalUIConfig& Config};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void RemoveUIConfig(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal UI")
    FPersonalUIConfig GetUIConfig(EPersonalUIType UIType};

    // ?ïÁï´Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void PlayOpenAnimation(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void PlayCloseAnimation(EPersonalUIType UIType};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void SetAnimationSpeed(float Speed};

    // ?∏Ê??¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateRelationshipUI(};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateReputationUI(};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateDialogueUI(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateQuestUI(};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateAudioUI(};

    // ‰∏ªÂ?Ë°®Êùø
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void RefreshDashboard(};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowNotification(const FString& Message, float Duration = 3.0f};

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowConfirmationDialog(const FString& Title, const FString& Message, const FString& OnConfirm};

    // ?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "Personal UI")
    bool IsAnyPanelOpen() const;

    UFUNCTION(BlueprintPure, Category = "Personal UI")
    EPersonalUIType GetActivePanelType() const { return ActivePanelType; }

    UFUNCTION(BlueprintPure, Category = "Personal UI")
    TArray<EPersonalUIType> GetVisiblePanels() const;

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIPanelOpened OnUIPanelOpened;

    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIPanelClosed OnUIPanelClosed;

    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIStateChanged OnUIStateChanged;

protected:
    // Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    UPROPERTY()
    TObjectPtr<UMingAudioRelationshipManager> AudioManager;

    UPROPERTY()
    TObjectPtr<UMingRepublicEraAudioThemes> ThemeManager;

    // WidgetÂ≠òÂÑ≤
    UPROPERTY()
    TMap<EPersonalUIType, UUserWidget*> WidgetMap;

    // UI?çÁΩÆ
    UPROPERTY()
    TMap<EPersonalUIType, FPersonalUIConfig> UIConfigMap;

    // ?Ä??    UPROPERTY()
    EPersonalUIType ActivePanelType = EPersonalUIType::MainDashboard;

    UPROPERTY()
    bool bIsMainDashboardVisible = false;

    UPROPERTY()
    float AnimationSpeed = 1.0f;

    // ?ßÈÉ®?ΩÊï∏
    UUserWidget* CreateWidget(EPersonalUIType UIType};
    void InitializeDefaultConfigs(};
    void SetupEventBindings(};
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason};
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason};
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType};

private:
    // Widget?µÂª∫ËºîÂä©?ΩÊï∏
    UUserWidget* CreateRelationshipPanel(};
    UUserWidget* CreateReputationPanel(};
    UUserWidget* CreateDialoguePanel(};
    UUserWidget* CreateQuestPanel(};
    UUserWidget* CreateAudioPanel(};
    UUserWidget* CreateMainDashboard(};
    UUserWidget* CreateCharacterDetails(};
    UUserWidget* CreateRegionDetails(};
    UUserWidget* CreateSystemSettings(};

    // ?ïÁï´ËºîÂä©?ΩÊï∏
    void PlayAnimation(UUserWidget* Widget, EUIAnimationType AnimationType, float Duration};
    void StopAnimation(UUserWidget* Widget};
};

