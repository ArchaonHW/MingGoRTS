// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Relationship Panel UI Component

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingRelationshipPanel.generated.h"

class UMingRelationshipManager;

UENUM(BlueprintType)
enum class ERelationshipSortType: uint8 {
    Name UMETA(DisplayName = "Name"),
    Level UMETA(DisplayName = "Level"),
    Status UMETA(DisplayName = "Status"),
    LastInteraction UMETA(DisplayName = "Last Interaction")
};

UENUM(BlueprintType)
enum class ERelationshipFilterType: uint8 {
    All UMETA(DisplayName = "All"),
    Friends UMETA(DisplayName = "Friends"),
    Allies UMETA(DisplayName = "Allies"),
    Neutral UMETA(DisplayName = "Neutral"),
    Enemies UMETA(DisplayName = "Enemies")
};

USTRUCT(BlueprintType)
struct FRelationshipDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display Data")
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display Data")
    int32 RelationshipLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display Data")
    FString RelationshipStatus;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display Data")
    FString LastInteractionDate;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship Display Data")
    UTexture2D* CharacterPortrait;

    FRelationshipDisplayData()
        : CharacterName(TEXT(""))
        , RelationshipLevel(0)
        , RelationshipStatus(TEXT(""))
        , LastInteractionDate(TEXT(""))
        , CharacterPortrait(nullptr)
    {}
};

/**
 * MingGoRTS Relationship Panel
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipPanel : public UWidget
{
    GENERATED_BODY()

public:
    UMingRelationshipPanel();

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void InitializeRelationshipPanel(UMingRelationshipManager* RelationshipManager);

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void RefreshRelationships();

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void SetSortType(ERelationshipSortType SortType);

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void SetFilterType(ERelationshipFilterType FilterType);

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void SetSearchText(const FString& SearchText);

    UFUNCTION(BlueprintPure, Category = "Relationship Panel")
    TArray<FRelationshipDisplayData> GetDisplayedRelationships() const;

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void OnRelationshipSelected(const FString& CharacterName);

    UFUNCTION(BlueprintCallable, Category = "Relationship Panel")
    void ShowRelationshipDetails(const FString& CharacterName);

protected:
    UPROPERTY()
    UMingRelationshipManager* RelationshipManager;

    UPROPERTY()
    TArray<FRelationshipDisplayData> RelationshipData;

    UPROPERTY()
    TArray<FRelationshipDisplayData> FilteredData;

    UPROPERTY()
    ERelationshipSortType CurrentSortType;

    UPROPERTY()
    ERelationshipFilterType CurrentFilterType;

    UPROPERTY()
    FString CurrentSearchText;

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    void UpdateRelationshipData();
    void ApplySorting();
    void ApplyFiltering();
    void SortByName(TArray<FRelationshipDisplayData>& Data);
    void SortByLevel(TArray<FRelationshipDisplayData>& Data);
    void SortByStatus(TArray<FRelationshipDisplayData>& Data);
    void SortByLastInteraction(TArray<FRelationshipDisplayData>& Data);
    bool PassesFilter(const FRelationshipDisplayData& RelationshipData) const;
    bool PassesSearch(const FRelationshipDisplayData& RelationshipData) const;
    void RefreshDisplayList();
};
