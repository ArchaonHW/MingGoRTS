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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReputationSelected, const FString&, RegionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationFilterChanged, EReputationLevel, FilterLevel, bool, bActive);

/**
 * 聲望面板Widget
 * 顯示和管理地區聲望信息
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingReputationPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingReputationPanel(const FObjectInitializer& ObjectInitializer);

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void InitializeReputationPanel(UMingRelationshipManager* InRelationshipManager);

    // 數據更新
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void UpdateReputationData();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void RefreshReputationList();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void UpdateSelectedReputation(const FString& RegionID);

    // 過濾和排序
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void FilterByReputationLevel(EReputationLevel ReputationLevel, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void SortByScore(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void SortByName(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ClearFilters();

    // 搜索
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void SearchReputations(const FString& SearchText);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ClearSearch();

    // 統計信息
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

    // 詳細信息
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ShowReputationDetails(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void HideReputationDetails();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ShowQuestList(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void HideQuestList();

    // 互動操作
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void OnReputationClicked(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void OnReputationDoubleClicked(const FString& RegionID);

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void OnReputationRightClicked(const FString& RegionID);

    // 任務相關
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    bool CanAcceptQuestInRegion(const FString& RegionID) const;

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    TArray<FString> GetAvailableQuestsInRegion(const FString& RegionID) const;

    // 導出和導入
    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ExportReputationData();

    UFUNCTION(BlueprintCallable, Category = "Reputation Panel")
    void ImportReputationData();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Reputation Events")
    FOnReputationSelected OnReputationSelected;

    UPROPERTY(BlueprintAssignable, Category = "Reputation Events")
    FOnReputationFilterChanged OnReputationFilterChanged;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // 數據存儲
    UPROPERTY(BlueprintReadOnly, Category = "Reputation Data")
    TArray<FReputationDisplayData> DisplayedReputations;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Data")
    TArray<FReputationDisplayData> AllReputations;

    UPROPERTY(BlueprintReadOnly, Category = "Reputation Data")
    FReputationDisplayData SelectedReputation;

    // 過濾狀態
    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<EReputationLevel, bool> ActiveFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    FString CurrentSearchText;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    bool bIsFiltered = false;

    // 排序狀態
    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByScore = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortAscending = true;

    // UI組件引用（需要在Blueprint中綁定）
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

    // 虛擬函數
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // 內部函數
    void ProcessReputationData();
    void ApplyFilters();
    void ApplySorting();
    void UpdateStatistics();
    void UpdateUI();
    FReputationDisplayData CreateDisplayData(const FString& RegionID, const FReputationData& ReputationData);
    FLinearColor GetReputationColor(EReputationLevel ReputationLevel) const;
    FString GetReputationDescription(EReputationLevel ReputationLevel, float Score) const;
    FString GetRegionDisplayName(const FString& RegionID) const;

    // 事件綁定
    void SetupEventBindings();
    void OnReputationDataChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // 輔助函數
    bool PassesFilter(const FReputationDisplayData& ReputationData) const;
    bool PassesSearch(const FReputationDisplayData& ReputationData) const;
    void RefreshDisplayList();
    EReputationLevel CalculateReputationLevel(float Score) const;
};
