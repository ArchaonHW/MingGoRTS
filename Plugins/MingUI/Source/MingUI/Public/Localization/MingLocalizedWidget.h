#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingLocalizedWidget.generated.h"

// Forward declarations
class UMingLocalizationManager;

// Text case options
UENUM(BlueprintType)
enum class ELocalizedTextCase : uint8 {
    Default            UMETA(DisplayName = "Default"),
    Uppercase          UMETA(DisplayName = "Uppercase"),
    Lowercase          UMETA(DisplayName = "Lowercase"),
    TitleCase          UMETA(DisplayName = "Title Case"),
    SentenceCase       UMETA(DisplayName = "Sentence Case")
};

// Text truncation options
UENUM(BlueprintType)
enum class ETextTruncation : uint8 {
    None               UMETA(DisplayName = "None"),
    Character          UMETA(DisplayName = "Character"),
    Word               UMETA(DisplayName = "Word"),
    Ellipsis           UMETA(DisplayName = "Ellipsis")
};

// Text update mode
UENUM(BlueprintType)
enum class ELocalizationUpdateMode : uint8 {
    Auto               UMETA(DisplayName = "Auto"),
    Manual             UMETA(DisplayName = "Manual"),
    OnLanguageChanged  UMETA(DisplayName = "On Language Changed")
};

/**
 * Localized Text Widget
 * Displays dynamically localized text with formatting options
 */
UCLASS(ClassGroup = (Localization), Blueprintable, meta = (DisableNativeTick))
class MINGPERSONAL_API UMingLocalizedWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLocalizedWidget(const FObjectInitializer& ObjectInitializer};

    // Initialize
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // === Localization Key Settings ===

    // Set localization key
    UFUNCTION(BlueprintCallable, Category = "Localized Widget")
    void SetLocalizationKey(const FString& Key, const FString& Namespace = TEXT("Default")};

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetLocalizationKey() const { return LocalizationKey; }

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetNamespace() const { return Namespace; }

    // === Parameters ===

    // Set parameters
    UFUNCTION(BlueprintCallable, Category = "Localized WidgetParameters")
    void SetParameter(const FString& ParamName, const FString& Value};

    UFUNCTION(BlueprintCallable, Category = "Localized WidgetParameters")
    void SetParameters(const TMap<FString, FString>& Params};

    UFUNCTION(BlueprintCallable, Category = "Localized WidgetParameters")
    void ClearParameters(};

    // === Text Updates ===

    // Refresh text
    UFUNCTION(BlueprintCallable, Category = "Localized WidgetUpdate")
    void UpdateText(};

    // Force refresh
    UFUNCTION(BlueprintCallable, Category = "Localized WidgetUpdate")
    void ForceRefresh(};

    // === Formatting ===

    // Set text case
    UFUNCTION(BlueprintCallable, Category = "Localized WidgetFormatting")
    void SetTextCase(ELocalizedTextCase NewCase};

    UFUNCTION(BlueprintPure, Category = "Localized WidgetFormatting")
    ELocalizedTextCase GetTextCase() const { return TextCase; }

    // Set truncation
    UFUNCTION(BlueprintCallable, Category = "Localized WidgetFormatting")
    void SetTruncation(ETextTruncation TruncationType, int32 MaxLength = 100};

    // === Properties ===

    // Localization key
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString LocalizationKey;

    // Namespace
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString Namespace;

    // Default text (fallback)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    FText DefaultText;

    // Update mode
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    ELocalizationUpdateMode UpdateMode;

    // Text case
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized WidgetFormatting")
    ELocalizedTextCase TextCase;

    // Truncation settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized WidgetFormatting")
    ETextTruncation Truncation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized WidgetFormatting", meta = (EditCondition = "Truncation != ETextTruncation::None"))
    int32 MaxTextLength;

    // Use rich text
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bUseRichText;

    // Auto wrap text
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bAutoWrapText;

    // === Events ===

    // Language changed
    UFUNCTION()
    void OnLanguageChanged(EMingLanguage NewLanguage};

    // Get current text
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetCurrentText() const;

    // Get raw localized text (no formatting)
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetRawLocalizedText() const;

protected:
    // Parameters map
    UPROPERTY()
    TMap<FString, FString> Parameters;

    // Cached text
    UPROPERTY()
    FText CurrentText;

    // Bind to system
    void BindToLocalizationSystem(};

    // Unbind from system
    void UnbindFromLocalizationSystem(};

    // Process text (format + truncate)
    FText ProcessText(const FText& RawText) const;

    // Apply text case
    FText ApplyTextCase(const FText& Text) const;

    // Apply truncation
    FText ApplyTruncation(const FText& Text) const;

    // Replace parameters
    FText ReplaceParameters(const FText& Text) const;

    // Get localization manager
    UMingLocalizationManager* GetLocalizationManager() const;
};
