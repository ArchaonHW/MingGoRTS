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
    RelationshipPanel,     // 關係面板
    ReputationPanel,       // 聲望面板
    DialoguePanel,         // 對話面板
    QuestPanel,            // 任務面板
    AudioPanel,            // 音頻面板
    MainDashboard,         // 主儀表板
    CharacterDetails,      // 角色詳情
    RegionDetails,         // 地區詳情
    SystemSettings         // 系統設置
};

UENUM(BlueprintType)
enum class EUIAnimationType : uint8
{
    None,                  // 無動畫
    Fade,                  // 淡入淡出
    Slide,                 // 滑動
    Scale,                 // 縮放
    Rotate,                // 旋轉
    Bounce                 // 彈跳
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
        WidgetName = TEXT("");
        WidgetPath = TEXT("");
        bIsVisible = false;
        bIsInteractive = true;
        OpenAnimation = EUIAnimationType::Fade;
        CloseAnimation = EUIAnimationType::Fade;
        AnimationDuration = 0.3f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIPanelOpened, EPersonalUIType, UIType, UUserWidget*, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUIPanelClosed, EPersonalUIType, UIType, UUserWidget*, Widget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIStateChanged, bool, bIsAnyPanelOpen);

/**
 * 個人系統UI管理器
 * 管理所有關係、聲望、對話等UI界面
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingPersonalUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingPersonalUIManager();

    // 初始化UI系統
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void InitializeUIManager(UMingRelationshipManager* RelationshipManager, 
                            UMingAudioRelationshipManager* AudioManager,
                            UMingRepublicEraAudioThemes* ThemeManager);

    // UI面板管理
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void OpenUIPanel(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void CloseUIPanel(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ToggleUIPanel(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void CloseAllPanels();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowMainDashboard();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void HideMainDashboard();

    // Widget管理
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    UUserWidget* GetWidget(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    bool IsWidgetVisible(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void SetWidgetVisibility(EPersonalUIType UIType, bool bVisible);

    // UI配置
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void AddUIConfig(const FPersonalUIConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void RemoveUIConfig(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal UI")
    FPersonalUIConfig GetUIConfig(EPersonalUIType UIType);

    // 動畫系統
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void PlayOpenAnimation(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void PlayCloseAnimation(EPersonalUIType UIType);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void SetAnimationSpeed(float Speed);

    // 數據更新
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateRelationshipUI();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateReputationUI();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateDialogueUI(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateQuestUI();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void UpdateAudioUI();

    // 主儀表板
    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void RefreshDashboard();

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowNotification(const FString& Message, float Duration = 3.0f);

    UFUNCTION(BlueprintCallable, Category = "Personal UI")
    void ShowConfirmationDialog(const FString& Title, const FString& Message, const FString& OnConfirm);

    // 狀態查詢
    UFUNCTION(BlueprintPure, Category = "Personal UI")
    bool IsAnyPanelOpen() const;

    UFUNCTION(BlueprintPure, Category = "Personal UI")
    EPersonalUIType GetActivePanelType() const { return ActivePanelType; }

    UFUNCTION(BlueprintPure, Category = "Personal UI")
    TArray<EPersonalUIType> GetVisiblePanels() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIPanelOpened OnUIPanelOpened;

    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIPanelClosed OnUIPanelClosed;

    UPROPERTY(BlueprintAssignable, Category = "UI Events")
    FOnUIStateChanged OnUIStateChanged;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    UPROPERTY()
    TObjectPtr<UMingAudioRelationshipManager> AudioManager;

    UPROPERTY()
    TObjectPtr<UMingRepublicEraAudioThemes> ThemeManager;

    // Widget存儲
    UPROPERTY()
    TMap<EPersonalUIType, UUserWidget*> WidgetMap;

    // UI配置
    UPROPERTY()
    TMap<EPersonalUIType, FPersonalUIConfig> UIConfigMap;

    // 狀態
    UPROPERTY()
    EPersonalUIType ActivePanelType = EPersonalUIType::MainDashboard;

    UPROPERTY()
    bool bIsMainDashboardVisible = false;

    UPROPERTY()
    float AnimationSpeed = 1.0f;

    // 內部函數
    UUserWidget* CreateWidget(EPersonalUIType UIType);
    void InitializeDefaultConfigs();
    void SetupEventBindings();
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason);
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);
    void OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType);

private:
    // Widget創建輔助函數
    UUserWidget* CreateRelationshipPanel();
    UUserWidget* CreateReputationPanel();
    UUserWidget* CreateDialoguePanel();
    UUserWidget* CreateQuestPanel();
    UUserWidget* CreateAudioPanel();
    UUserWidget* CreateMainDashboard();
    UUserWidget* CreateCharacterDetails();
    UUserWidget* CreateRegionDetails();
    UUserWidget* CreateSystemSettings();

    // 動畫輔助函數
    void PlayAnimation(UUserWidget* Widget, EUIAnimationType AnimationType, float Duration);
    void StopAnimation(UUserWidget* Widget);
};
