#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingPersonalManager.h"
#include "MingMainDashboard.generated.h"

UENUM(BlueprintType)
enum class EDashboardTab : uint8
{
    Overview,       // 羆凝
    Relationships,  // ????
    Reputation,     // ????
    Quests,         // ヴ??
    Audio,          // ??繵
    Settings        // 砞竚
};

USTRUCT(BlueprintType)
struct FDashboardQuickAction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Action")
    FString ActionName;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Action")
    FString ActionDescription;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Action")
    FString ActionIcon;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Action")
    bool bIsAvailable;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Action")
    FLinearColor ActionColor;

    FDashboardQuickAction()
    {
        ActionName = TEXT("");
        ActionDescription = TEXT("");
        ActionIcon = TEXT("");
        bIsAvailable = true;
        ActionColor = FLinearColor::White;
    }
};

USTRUCT(BlueprintType)
struct FDashboardSummary
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    int32 TotalRelationships;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    int32 FriendCount;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    int32 EnemyCount;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    int32 KnownRegions;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    int32 ActiveQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    int32 CompletedQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    float AverageReputation;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    EReputationLevel HighestReputation;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    FString CurrentRegion;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard Summary")
    FString CurrentTheme;

    FDashboardSummary()
    {
        TotalRelationships = 0;
        FriendCount = 0;
        EnemyCount = 0;
        KnownRegions = 0;
        ActiveQuests = 0;
        CompletedQuests = 0;
        AverageReputation = 0.0f;
        HighestReputation = EReputationLevel::Unknown;
        CurrentRegion = TEXT("");
        CurrentTheme = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashboardTabChanged, EDashboardTab, NewTab};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickActionExecuted, const FString&, ActionName};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashboardRefreshed, const FDashboardSummary&, Summary);

/**
 * ╰参??狾Widget
 * 参??恨?X???╰参??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingMainDashboard : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingMainDashboard(const FObjectInitializer& ObjectInitializer};

    // ???X    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void InitializeDashboard(UMingPersonalManager* InPersonalManager);

    // 夹乓??恨X    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void SwitchToTab(EDashboardTab Tab);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowOverview();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowRelationships();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowReputation();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowQuests();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowAudio();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowSettings();

    // ???X?穝
    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void RefreshDashboard();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void UpdateSummary();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void UpdateQuickActions();

    // е硉????    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ExecuteQuickAction(const FString& ActionName);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void QuickAddRelationship(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void QuickStartQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void QuickPlayAudio(const FString& AudioType);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void QuickOpenDialogue(const FString& CharacterID);

    // ??╰参
    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowNotification(const FString& Message, const FString& Type = TEXT("Info")};

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowWarning(const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowError(const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ClearNotifications();

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void ShowAchievementUnlocked(const FString& AchievementName, const FString& Description);

    // ????琩??    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    EDashboardTab GetCurrentTab() const { return CurrentTab; }

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    FDashboardSummary GetSummary() const { return DashboardSummary; }

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    TArray<FDashboardQuickAction> GetQuickActions() const { return QuickActions; }

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    bool IsTabVisible(EDashboardTab Tab) const;

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    int32 GetNotificationCount() const;

    // 参??獺
    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    float GetRelationshipProgress() const;

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    float GetReputationProgress() const;

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    float GetQuestProgress() const;

    UFUNCTION(BlueprintPure, Category = "Main Dashboard")
    float GetOverallProgress() const;

    // が?X???
    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void OnTabButtonClicked(EDashboardTab Tab);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void OnQuickActionButtonClicked(const FString& ActionName);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void OnNotificationClicked(int32 NotificationIndex);

    UFUNCTION(BlueprintCallable, Category = "Main Dashboard")
    void OnRefreshButtonClicked();

    // ㄆン
    UPROPERTY(BlueprintAssignable, Category = "Dashboard Events")
    FOnDashboardTabChanged OnDashboardTabChanged;

    UPROPERTY(BlueprintAssignable, Category = "Dashboard Events")
    FOnQuickActionExecuted OnQuickActionExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Dashboard Events")
    FOnDashboardRefreshed OnDashboardRefreshed;

protected:
    // ╰参まノ
    UPROPERTY()
    TObjectPtr<UMingPersonalManager> PersonalManager;

    // ???X?X    UPROPERTY(BlueprintReadOnly, Category = "Dashboard State")
    EDashboardTab CurrentTab;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard State")
    FDashboardSummary DashboardSummary;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard State")
    TArray<FDashboardQuickAction> QuickActions;

    UPROPERTY(BlueprintReadOnly, Category = "Dashboard State")
    TArray<FString> Notifications;

    // 砞竚
    UPROPERTY(BlueprintReadWrite, Category = "Dashboard Settings")
    bool bAutoRefresh = true;

    UPROPERTY(BlueprintReadWrite, Category = "Dashboard Settings")
    float RefreshInterval = 5.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Dashboard Settings")
    int32 MaxNotifications = 10;

    // UI舱ンまノ??璶Blueprintい??﹚??
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* TabContainer;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* TabButtonContainer;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* QuickActionContainer;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* NotificationContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SummaryText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* RelationshipCountText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestCountText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ReputationText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* OverallProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UButton* RefreshButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SettingsButton;

    // 夹乓??Widgetまノ
    UPROPERTY()
    TObjectPtr<UUserWidget> OverviewWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> RelationshipWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> ReputationWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> QuestWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> AudioWidget;

    UPROPERTY()
    TObjectPtr<UUserWidget> SettingsWidget;

    // ??览??计
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ??场??计
    void InitializeTabs();
    void InitializeQuickActions();
    void UpdateTabUI();
    void UpdateNotificationUI();
    void UpdateProgressBar();
    FDashboardSummary CalculateSummary() const;
    TArray<FDashboardQuickAction> GenerateQuickActions() const;
    FString GetTabDisplayName(EDashboardTab Tab) const;
    FLinearColor GetTabColor(EDashboardTab Tab) const;
    void CreateTabButtons();
    void CreateQuickActionButtons();

    // ㄆン竕??
    void SetupEventBindings();
    void OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason};
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason};
    void OnQuestCompleted(const FString& QuestID, const FString& RegionID, const FString& QuestGiverID};

private:
    // 徊??计
    void ShowTabContent(EDashboardTab Tab};
    void HideAllTabContent();
    void AddNotification(const FString& Message, const FString& Type};
    void RemoveNotification(int32 Index};
    void RefreshTimer();
    void UpdateProgressBars();

    // ﹚?X    FTimerHandle RefreshTimerHandle;
);

