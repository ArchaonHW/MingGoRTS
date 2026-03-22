#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingReputationWidget.generated.h"

UENUM(BlueprintType)
enum class EMingReputationViewMode : uint8
{
    Overview UMETA(DisplayName = "Overview"),
    RegionDetails UMETA(DisplayName = "Region Details"),
    Comparison UMETA(DisplayName = "Comparison"),
    Timeline UMETA(DisplayName = "Timeline")
};

USTRUCT(BlueprintType)
struct FMingReputationDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName RegionID;

    UPROPERTY(BlueprintReadWrite)
    FString RegionName;

    UPROPERTY(BlueprintReadWrite)
    float ReputationValue;

    UPROPERTY(BlueprintReadWrite)
    int32 ReputationRank;

    UPROPERTY(BlueprintReadWrite)
    FString RankTitle;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor ReputationColor;

    UPROPERTY(BlueprintReadWrite)
    float NextRankProgress;

    UPROPERTY(BlueprintReadWrite)
    float ReputationToNextRank;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> ActiveEffects;

    UPROPERTY(BlueprintReadWrite)
    int32 AvailableQuests;

    UPROPERTY(BlueprintReadWrite)
    int32 CompletedQuests;
};

USTRUCT(BlueprintType)
struct FMingReputationHistoryPoint
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(BlueprintReadWrite)
    float ReputationValue;

    UPROPERTY(BlueprintReadWrite)
    FString EventDescription;
};

/**
 * Reputation Panel Widget
 * Displays faction/region reputation status
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingReputationWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingReputationWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void InitializeWidget(class UMingPersonalManager* InPersonalManager);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void RefreshDisplay();

    // View Modes
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void SetViewMode(EMingReputationViewMode NewMode);

    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    EMingReputationViewMode GetCurrentViewMode() const { return CurrentViewMode; }

    // Region Selection
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void SelectRegion(FName RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void ClearRegionSelection();

    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    FName GetSelectedRegion() const { return SelectedRegionID; }

    // Display Data
    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    TArray<FMingReputationDisplayData> GetAllReputationData() const;

    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    FMingReputationDisplayData GetRegionData(FName RegionID) const;

    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    float GetTotalReputation() const;

    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    int32 GetAverageRank() const;

    // History
    UFUNCTION(BlueprintPure, Category = "Reputation UI")
    TArray<FMingReputationHistoryPoint> GetReputationHistory(FName RegionID, int32 MaxPoints = 50) const;

    // Filters and Sorting
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void SortByReputation(bool bDescending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void SortByRegionName(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void SortByRank(bool bDescending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void FilterByMinimumRank(int32 MinRank);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void ShowOnlyRegionsWithQuests();

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void ClearFilters();

    // Visual Feedback
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void HighlightReputationChange(FName RegionID, float DeltaValue);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void PlayRankUpAnimation(FName RegionID, int32 NewRank);

    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void ShowReputationEffectNotification(FName RegionID, const FString& EffectDescription);

    // Audio Integration
    UFUNCTION(BlueprintCallable, Category = "Reputation UI")
    void PlayReputationSound(float Value, float Delta);

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionSelected, FName, RegionID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationClicked, FName, RegionID, float, ReputationValue);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewModeChanged);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRankChanged, int32, NewRank);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRegionSelected OnRegionSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnReputationClicked OnReputationClicked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnViewModeChanged OnViewModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRankChanged OnRankChanged;

protected:
    // UI Components
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* RegionListContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TitleText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SelectedRegionName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* RankTitleText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ReputationProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ReputationValueText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NextRankText;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* EffectsContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestsAvailableText;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* StatsContainer;

    // Data
    UPROPERTY()
    class UMingPersonalManager* PersonalManager;

    UPROPERTY()
    EMingReputationViewMode CurrentViewMode;

    UPROPERTY()
    FName SelectedRegionID;

    UPROPERTY()
    TArray<FMingReputationDisplayData> DisplayDataCache;

    UPROPERTY()
    int32 MinimumRankFilter;

    UPROPERTY()
    bool bFilterQuestsOnly;

    // Button Handlers
    UFUNCTION()
    void OnOverviewButtonClicked();

    UFUNCTION()
    void OnDetailsButtonClicked();

    UFUNCTION()
    void OnTimelineButtonClicked();

    UFUNCTION()
    void OnCloseButtonClicked();

    UFUNCTION()
    void OnRegionButtonClicked(FName RegionID);

    // Internal Functions
    void PopulateRegionList();
    void UpdateRegionDetails();
    void UpdateStatsOverview();
    void DrawReputationGraph();
    void DrawTimeline();
    void CreateRegionEntry(const FMingReputationDisplayData& Data);
    void CreateEffectEntry(const FString& EffectDescription);
    void ClearRegionList();
    void ClearEffectsList();
    void FilterAndSortData();
    FLinearColor GetReputationColor(float Value) const;
    FString GetRankTitle(int32 Rank) const;
    float CalculateNextRankProgress(float Value, int32 CurrentRank) const;

    // Animation
    void AnimateProgressBar(float TargetValue, float Duration);
    void AnimateRankUp(UWidget* TargetWidget);
    void PlayHighlightEffect(FName RegionID);

public:
    // Static utilities
    UFUNCTION(BlueprintPure, Category = "Reputation UI", meta = (StaticFunction))
    static FLinearColor GetColorForReputationValue(float Value);

    UFUNCTION(BlueprintPure, Category = "Reputation UI", meta = (StaticFunction))
    static FString GetRankTitleForLevel(int32 Rank);

    UFUNCTION(BlueprintPure, Category = "Reputation UI", meta = (StaticFunction))
    static int32 GetMaxRank();
};
