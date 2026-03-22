#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingRelationshipWidget.generated.h"

// Forward declarations
class UMingRelationshipManager;
class UTextBlock;
class UImage;
class UProgressBar;
class UVerticalBox;
class UHorizontalBox;
class UScrollBox;
class UButton;

UENUM(BlueprintType)
enum class EMingRelationshipViewMode : uint8
{
    Overview UMETA(DisplayName = "Overview"),
    CharacterDetails UMETA(DisplayName = "Character Details"),
    NetworkGraph UMETA(DisplayName = "Network Graph"),
    Comparison UMETA(DisplayName = "Comparison")
};

USTRUCT(BlueprintType)
struct FMingRelationshipDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString CharacterName;

    UPROPERTY(BlueprintReadWrite)
    FName CharacterID;

    UPROPERTY(BlueprintReadWrite)
    TEnumAsByte<EMingRelationshipType> RelationshipType;

    UPROPERTY(BlueprintReadWrite)
    float RelationshipValue;

    UPROPERTY(BlueprintReadWrite)
    float NormalizedValue;

    UPROPERTY(BlueprintReadWrite)
    FString RelationshipDescription;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor RelationshipColor;

    UPROPERTY(BlueprintReadWrite)
    UTexture2D* CharacterPortrait;

    UPROPERTY(BlueprintReadWrite)
    bool bHasActiveQuest;

    UPROPERTY(BlueprintReadWrite)
    bool bCanInteract;
};

/**
 * Relationship Panel Widget
 * Displays character relationships and network
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingRelationshipWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingRelationshipWidget(const FObjectInitializer& ObjectInitializer};

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void InitializeWidget(UMingRelationshipManager* InRelationshipManager};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void RefreshDisplay(};

    // View Modes
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SetViewMode(EMingRelationshipViewMode NewMode};

    UFUNCTION(BlueprintPure, Category = "Relationship UI")
    EMingRelationshipViewMode GetCurrentViewMode() const { return CurrentViewMode; }

    // Character Selection
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SelectCharacter(FName CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void ClearCharacterSelection(};

    UFUNCTION(BlueprintPure, Category = "Relationship UI")
    FName GetSelectedCharacter() const { return SelectedCharacterID; }

    // Display Data
    UFUNCTION(BlueprintPure, Category = "Relationship UI")
    TArray<FMingRelationshipDisplayData> GetRelationshipDisplayData() const;

    UFUNCTION(BlueprintPure, Category = "Relationship UI")
    FMingRelationshipDisplayData GetCharacterDisplayData(FName CharacterID) const;

    // Filters
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SetFilterByType(EMingRelationshipType TypeFilter};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SetFilterByValueRange(float MinValue, float MaxValue};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SetSearchFilter(const FString& SearchText};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void ClearAllFilters(};

    // Sorting
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SortByName(bool bAscending = true};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SortByRelationshipValue(bool bAscending = false};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void SortByType(};

    // Interaction
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void OpenInteractionMenu(FName CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void CloseInteractionMenu(};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void ShowRelationshipHistory(FName CharacterID};

    // Visual Feedback
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void HighlightRelationshipChange(FName CharacterID, float DeltaValue};

    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void PlayRelationshipUpdateAnimation(FName CharacterID};

    // Audio Integration
    UFUNCTION(BlueprintCallable, Category = "Relationship UI")
    void PlayRelationshipSound(EMingRelationshipType RelationshipType, float Value};

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, FName, CharacterID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipClicked, FName, CharacterID, float, Value};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewModeChanged};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRefreshRequested};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCharacterSelected OnCharacterSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRelationshipClicked OnRelationshipClicked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnViewModeChanged OnViewModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRefreshRequested OnRefreshRequested;

protected:
    // UI Components
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* CharacterListContainer;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* CharacterScrollBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TitleText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SelectedCharacterName;

    UPROPERTY(meta = (BindWidget))
    UImage* SelectedCharacterPortrait;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* RelationshipProgressBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* RelationshipValueText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* RelationshipDescription;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* FilterContainer;

    UPROPERTY(meta = (BindWidget))
    UButton* OverviewButton;

    UPROPERTY(meta = (BindWidget))
    UButton* DetailsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GraphButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseButton;

    // Data
    UPROPERTY()
    UMingRelationshipManager* RelationshipManager;

    UPROPERTY()
    EMingRelationshipViewMode CurrentViewMode;

    UPROPERTY()
    FName SelectedCharacterID;

    UPROPERTY()
    TArray<FMingRelationshipDisplayData> DisplayDataCache;

    UPROPERTY()
    EMingRelationshipType TypeFilter;

    UPROPERTY()
    float MinValueFilter;

    UPROPERTY()
    float MaxValueFilter;

    UPROPERTY()
    FString SearchFilter;

    // Bindable Events
    UFUNCTION()
    void OnOverviewButtonClicked(};

    UFUNCTION()
    void OnDetailsButtonClicked(};

    UFUNCTION()
    void OnGraphButtonClicked(};

    UFUNCTION()
    void OnCloseButtonClicked(};

    UFUNCTION()
    void OnCharacterButtonClicked(FName CharacterID};

    // Internal Functions
    void PopulateCharacterList(};
    void UpdateCharacterDetails(};
    void UpdateProgressBar(float Value};
    FLinearColor GetRelationshipColor(float Value) const;
    FString GetRelationshipDescription(float Value) const;
    void FilterDisplayData(};
    void SortDisplayData(};
    void CreateCharacterEntry(const FMingRelationshipDisplayData& Data};
    void ClearCharacterList(};

    // Network Graph Functions
    void DrawNetworkGraph(};
    void CalculateNodePositions(};
    void DrawConnectionLines(};

    // Animation Functions
    void AnimateProgressBar(float TargetValue, float Duration};
    void AnimateCharacterEntry(UWidget* EntryWidget};
    void PlayHighlightAnimation(UWidget* TargetWidget};

public:
    // Static utility functions
    UFUNCTION(BlueprintPure, Category = "Relationship UI", meta = (StaticFunction))
    static FLinearColor GetColorForRelationshipType(EMingRelationshipType Type};

    UFUNCTION(BlueprintPure, Category = "Relationship UI", meta = (StaticFunction))
    static FString GetRelationshipTypeDisplayName(EMingRelationshipType Type};

    UFUNCTION(BlueprintPure, Category = "Relationship UI", meta = (StaticFunction))
    static UTexture2D* GetDefaultPortrait(};
};

