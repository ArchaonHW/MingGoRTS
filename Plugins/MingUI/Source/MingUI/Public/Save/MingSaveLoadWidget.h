// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingSaveLoadWidget - UI for Save/Load functionality

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingSaveLoadWidget.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotStatus : uint8 {
    Empty               UMETA(DisplayName = "Empty"),
    Occupied            UMETA(DisplayName = "Occupied"),
    Corrupted           UMETA(DisplayName = "Corrupted"),
    CloudOnly           UMETA(DisplayName = "Cloud Only"),
    LocalOnly           UMETA(DisplayName = "Local Only")
};

USTRUCT(BlueprintType)
struct FSaveSlotInfo {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 SlotIndex;

    UPROPERTY(BlueprintReadOnly)
    ESaveSlotStatus Status;

    UPROPERTY(BlueprintReadOnly)
    FString SaveName;

    UPROPERTY(BlueprintReadOnly)
    FDateTime SaveTimestamp;

    UPROPERTY(BlueprintReadOnly)
    FString PlayTime;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerLevel;

    UPROPERTY(BlueprintReadOnly)
    FString Location;

    UPROPERTY(BlueprintReadOnly)
    UTexture2D* Screenshot;

    FSaveSlotInfo()
        : SlotIndex(-1)
        , Status(ESaveSlotStatus::Empty)
        , Screenshot(nullptr) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotSelected, int32, SlotIndex};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotHovered, int32, SlotIndex};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveGameConfirmed, int32, SlotIndex, FString, SaveName};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLoadGameConfirmed, int32, SlotIndex, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteSaveConfirmed, int32, SlotIndex};

UCLASS()
class MINGUI_API UMingSaveLoadWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UMingSaveLoadWidget(const FObjectInitializer& ObjectInitializer};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void InitializeSaveLoadWidget(bool bIsSaveMode};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void RefreshSaveSlots(};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void SelectSlot(int32 SlotIndex};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void ConfirmSave(const FString& SaveName};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void ConfirmLoad(};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void ConfirmDelete(};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void SetNewSaveName(const FString& NewName};

    UFUNCTION(BlueprintPure, Category = "SaveLoad Widget")
    FSaveSlotInfo GetSlotInfo(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "SaveLoad Widget")
    TArray<FSaveSlotInfo> GetAllSlotInfo() const;

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void ShowDeleteConfirmation(int32 SlotIndex};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void HideDeleteConfirmation(};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void ShowSaveDetails(int32 SlotIndex};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget")
    void HideSaveDetails(};

    UPROPERTY(BlueprintAssignable, Category = "SaveLoad Widget Events")
    FOnSaveSlotSelected OnSaveSlotSelected;

    UPROPERTY(BlueprintAssignable, Category = "SaveLoad Widget Events")
    FOnSaveSlotHovered OnSaveSlotHovered;

    UPROPERTY(BlueprintAssignable, Category = "SaveLoad Widget Events")
    FOnSaveGameConfirmed OnSaveGameConfirmed;

    UPROPERTY(BlueprintAssignable, Category = "SaveLoad Widget Events")
    FOnLoadGameConfirmed OnLoadGameConfirmed;

    UPROPERTY(BlueprintAssignable, Category = "SaveLoad Widget Events")
    FOnDeleteSaveConfirmed OnDeleteSaveConfirmed;

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UVerticalBox* SaveSlotContainer;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UEditableTextBox* SaveNameInput;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* SelectedSlotInfo;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* SelectedSlotScreenshot;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UWidget* DeleteConfirmationPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UWidget* SaveDetailsPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* CancelButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UButton* DeleteButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveLoad Widget")
    TSubclassOf<UUserWidget> SaveSlotWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveLoad Widget")
    int32 MaxSaveSlots;

    UPROPERTY(BlueprintReadOnly)
    bool bInSaveMode;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentlySelectedSlot;

    UPROPERTY(BlueprintReadOnly)
    TArray<FSaveSlotInfo> CachedSlotInfo;

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget Internal")
    void UpdateUIForSelectedSlot(};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget Internal")
    void PopulateSaveSlots(};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget Internal")
    void HandleSlotClicked(int32 SlotIndex};

    UFUNCTION(BlueprintCallable, Category = "SaveLoad Widget Internal")
    void HandleSlotHovered(int32 SlotIndex};

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    bool ValidateSaveName(const FString& Name) const;
    void UpdateConfirmButtonState(};
};
