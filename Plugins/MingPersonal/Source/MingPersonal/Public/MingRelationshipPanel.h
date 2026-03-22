#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingRelationshipManager.h"
#include "MingRelationshipPanel.generated.h"

USTRUCT(BlueprintType)
struct FRelationshipDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    ERelationshipType RelationshipType;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    float RelationshipValue;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    FString RelationshipDescription;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    FLinearColor RelationshipColor;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display")
    TArray<FString> SharedMemories;

    FRelationshipDisplayData()
    {
        CharacterID = TEXT(""};
        CharacterName = TEXT(""};
        RelationshipType = ERelationshipType::Neutral;
        RelationshipValue = 0.0f;
        RelationshipDescription = TEXT(""};
        RelationshipColor = FLinearColor::White;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelationshipSelected, const FString&, CharacterID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationshipFilterChanged, ERelationshipType, FilterType, bool, bActive};

/**
 * ?��X�板Widget
 * 顯示?�管?��X��?係信X */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingRelationshipPanel(const FObjectInitializer& ObjectInitializer};

    // ?��X    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void InitializeRelationshipPanel(UMingRelationshipManager* InRelationshipManager};

    // ?��X�新
    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void UpdateRelationshipData(};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void RefreshRelationshipList(};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void UpdateSelectedRelationship(const FString& CharacterID};

    // ?�濾?��?�?    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void FilterByRelationshipType(ERelationshipType RelationshipType, bool bActive};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void SortByValue(bool bAscending = true};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void SortByName(bool bAscending = true};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void ClearFilters(};

    // ?�索
    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void SearchRelationships(const FString& SearchText};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void ClearSearch(};

    // 統�?信息
    UFUNCTION(BlueprintPure, Category = "Relationship Panel")
    int32 GetTotalRelationshipCount() const;

    UFUNCTION(BlueprintPure, Category = "Relationship Panel")
    int32 GetFriendCount() const;

    UFUNCTION(BlueprintPure, Category = "Relationship Panel")
    int32 GetEnemyCount() const;

    UFUNCTION(BlueprintPure, Category = "Relationship Panel")
    int32 GetNeutralCount() const;

    UFUNCTION(BlueprintPure, Category = "Relationship Panel")
    float GetAverageRelationshipValue() const;

    // 詳細信息
    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void ShowRelationshipDetails(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void HideRelationshipDetails(};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void AddSharedMemory(const FString& CharacterID, const FString& Memory};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void RemoveSharedMemory(const FString& CharacterID, const FString& Memory};

    // 互�X��?
    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void OnRelationshipClicked(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void OnRelationshipDoubleClicked(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void OnRelationshipRightClicked(const FString& CharacterID};

    // 導出?��X    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void ExportRelationshipData(};

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void ImportRelationshipData(};

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Relationship Events")
    FOnRelationshipSelected OnRelationshipSelected;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Events")
    FOnRelationshipFilterChanged OnRelationshipFilterChanged;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // ?��?存儲
    UPROPERTY(BlueprintReadOnly, Category = "Relationship Data")
    TArray<FRelationshipDisplayData> DisplayedRelationships;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Data")
    TArray<FRelationshipDisplayData> AllRelationships;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Data")
    FRelationshipDisplayData SelectedRelationship;

    // ?�濾?�X    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<ERelationshipType, bool> ActiveFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    FString CurrentSearchText;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    bool bIsFiltered = false;

    // ?��X�X    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByValue = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortAscending = true;

    // UI組件引用（�?要在Blueprint中�?定�?
    UPROPERTY(meta = (BindWidget))
    class UListView* RelationshipListView;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalCountText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FriendCountText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EnemyCountText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AverageValueText;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* SearchBox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* DetailsScrollBox;

    // ?�擬?�數
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ?�部?�數
    void ProcessRelationshipData(};
    void ApplyFilters(};
    void ApplySorting(};
    void UpdateStatistics(};
    void UpdateUI(};
    FRelationshipDisplayData CreateDisplayData(const FString& CharacterID, const FRelationshipData& RelationshipData};
    FLinearColor GetRelationshipColor(ERelationshipType RelationshipType, float Value) const;
    FString GetRelationshipDescription(ERelationshipType RelationshipType, float Value) const;

    // 事件綁�?
    void SetupEventBindings(};
    void OnRelationshipDataChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason};

private:
    // 輔助?�數
    bool PassesFilter(const FRelationshipDisplayData& RelationshipData) const;
    bool PassesSearch(const FRelationshipDisplayData& RelationshipData) const;
    void RefreshDisplayList(};
};

