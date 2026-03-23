#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingSageCharacterSystem.h"
#include "MingSageCharacterSelectionUI.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSelectionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESageCharacterType CharacterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* CharacterPortrait;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSageCharacterTraits CharacterTraits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AvailableAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Strengths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Weaknesses;

    FCharacterSelectionData()
    {
        CharacterType = ESageCharacterType::None;
        CharacterName = "";
        CharacterTitle = "";
        CharacterDescription = "";
        CharacterPortrait = nullptr;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, ESageCharacterType, SelectedType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterPreviewChanged, FCharacterSelectionData, PreviewData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectionConfirmed, ESageCharacterType, ConfirmedType, const FString&, CharacterName);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageCharacterSelectionUI : public UWidget
{
    GENERATED_BODY()

public:
    UMingSageCharacterSelectionUI();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void InitializeSelectionUI();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void ShowCharacterSelection();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void HideCharacterSelection();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void SelectCharacter(ESageCharacterType CharacterType);

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void PreviewCharacter(ESageCharacterType CharacterType);

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void ConfirmSelection();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void CancelSelection();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void SetCustomCharacterName(const FString& CharacterName);

    UFUNCTION(BlueprintPure, Category = "Character Selection")
    ESageCharacterType GetSelectedCharacter() const { return SelectedCharacterType; }

    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FCharacterSelectionData GetCharacterPreviewData() const { return CurrentPreviewData; }

    UFUNCTION(BlueprintPure, Category = "Character Selection")
    TArray<FCharacterSelectionData> GetAllCharacterOptions() const;

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void FilterCharacterOptions(const TArray<ESageCharacterType>& AllowedTypes);

    UFUNCTION(BlueprintPure, Category = "Character Selection")
    bool CanConfirmSelection() const;

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void RandomCharacterSelection();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void LoadCharacterPresets();

    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void SaveCurrentSelection();

    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FString GetCharacterRecommendation() const;

    UPROPERTY(BlueprintAssignable, Category = "Character Selection")
    FOnCharacterSelected OnCharacterSelected;

    UPROPERTY(BlueprintAssignable, Category = "Character Selection")
    FOnCharacterPreviewChanged OnCharacterPreviewChanged;

    UPROPERTY(BlueprintAssignable, Category = "Character Selection")
    FOnSelectionConfirmed OnSelectionConfirmed;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnSelectionUIInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnCharacterTypeSelected(ESageCharacterType CharacterType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnSelectionUIHidden();

    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnSelectionConfirmedEvent(ESageCharacterType CharacterType, const FString& CharacterName);

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void InitializeCharacterData();
    void UpdateCharacterPreview();
    void ValidateSelection();
    void ApplyCharacterFilters();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    ESageCharacterType SelectedCharacterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    FCharacterSelectionData CurrentPreviewData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    TArray<FCharacterSelectionData> CharacterOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    FString CustomCharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    bool bIsSelectionVisible;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    bool bAllowCustomNaming;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    bool bShowPreview;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    TArray<ESageCharacterType> AllowedCharacterTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    TMap<ESageCharacterType, UTexture2D*> CharacterPortraits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    float PreviewTransitionDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    bool bEnableRandomSelection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    bool bRequireCharacterName;
};
