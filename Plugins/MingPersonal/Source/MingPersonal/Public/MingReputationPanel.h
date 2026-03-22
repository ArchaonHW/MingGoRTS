#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingRelationshipManager.h"
#include "MingReputationPanel.generated.h"

USTRUCT(BlueprintType)
struct FReputationDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    FString RegionID;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    FString RegionName;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    EReputationLevel ReputationLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    float ReputationScore;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    FString ReputationDescription;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    FLinearColor ReputationColor;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    int32 AvailableQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    int32 CompletedQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Display")
    float QuestSuccessRate;

    FReputationDisplayData()
    {
        RegionID = TEXT("");
        RegionName = TEXT("");
        ReputationLevel = EReputationLevel::Unknown;
        ReputationScore = 0.0f;
        ReputationDescription = TEXT("");
        ReputationColor = FLinearColor::White;
        AvailableQuests = 0;
        CompletedQuests = 0;
        QuestSuccessRate = 0.0f;
    }
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationFilterChanged, EReputationLevel, FilterLevel, bool, bActive);

/**
 * ???X?�OWidget
 * ���??��??�a摧毀??�H��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingReputationPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingReputationPanel(const FObjectInitializer& ObjectInitializer);

    // ???X
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void InitializeReputationPanel(UMingRelationshipManager* InRelationshipManager);

    // ???X?�s
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void UpdateReputationData();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void RefreshReputationList();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void UpdateSelectedReputation(const FString& RegionID);

    // ??�o摧毀??
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void FilterByReputationLevel(EReputationLevel ReputationLevel, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void SortByScore(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void SortByName(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ClearFilters();

    // ??��
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void SearchReputations(const FString& SearchText);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ClearSearch();

    // ��??�H��
    UFUNCTION(BlueprintPure, Category = "Reputation Panel")
    int32 GetTotalRegionCount() const;

    UFUNCTION(BlueprintPure, Category = "Reputation Panel")
    int32 GetGoodReputationCount() const;

    UFUNCTION(BlueprintPure, Category = "Reputation Panel")
    int32 GetBadReputationCount() const;

    UFUNCTION(BlueprintPure, Category = "Reputation Panel")
    float GetAverageReputationScore() const;

    UFUNCTION(BlueprintPure, Category = "Reputation Panel")
    EReputationLevel GetHighestReputationLevel() const;

    // �ԲӫH��
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ShowReputationDetails(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void HideReputationDetails();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ShowQuestList(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void HideQuestList();

    // ��?X???
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void OnReputationClicked(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void OnReputationDoubleClicked(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void OnReputationRightClicked(const FString& RegionID);

    // ��?X???
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    bool CanAcceptQuestInRegion(const FString& RegionID) const;

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    TArray<FString> GetAvailableQuestsInRegion(const FString& RegionID) const;

    // �ɥX???X
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ExportReputationData();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ImportReputationData();

    // �ƥ�
    UPROPERTY(BlueprintAssignable, Category = "Reputation Events")
    FOnReputationSelected OnReputationSelected;

    UPROPERTY(BlueprintAssignable, Category = "Reputation Events")
    FOnReputationFilterChanged OnReputationFilterChanged;

protected:
    // �t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // 摧毀�s�x
    UPROPERTY(BlueprintReadOnly, Category = "Reputation Data")
    TArray<FReputationDisplayData> DisplayedReputations;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Data")
    TArray<FReputationDisplayData> AllReputations;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Data")
    FReputationDisplayData SelectedReputation;

    // ??�o??X
    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<EReputationLevel, bool> ActiveFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    FString CurrentSearchText;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    bool bIsFiltered = false;

    // ???X?X
    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByScore = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortAscending = true;

    // UI�ե�ޥΡ]??�n�bBlueprint��??�w??
    UPROPERTY(meta = (BindWidget))
    class UListView* ReputationListView;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalRegionText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoodReputationText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BadReputationText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AverageScoreText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HighestLevelText;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* SearchBox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* DetailsScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* QuestScrollBox;

    // ??��??��
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ??��??��
    void ProcessReputationData();
    void ApplyFilters();
    void ApplySorting();
    void UpdateStatistics();
    void UpdateUI();
    FReputationDisplayData CreateDisplayData(const FString& RegionID, const FReputationData& ReputationData);
    FLinearColor GetReputationColor(EReputationLevel ReputationLevel) const;
    FString GetReputationDescription(EReputationLevel ReputationLevel, float Score) const;
    FString GetRegionDisplayName(const FString& RegionID) const;

    // �ƥ�j??
    void SetupEventBindings();
    void OnReputationDataChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // ���U??��
    bool PassesFilter(const FReputationDisplayData& ReputationData) const;
    bool PassesSearch(const FReputationDisplayData& ReputationData) const;
    void RefreshDisplayList();
    EReputationLevel CalculateReputationLevel(float Score) const;
};
};

