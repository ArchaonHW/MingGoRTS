#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingLocalizedWidget.generated.h"

// Forward declarations
class UMingLocalizationManager;

// Text case options
UENUM(BlueprintType)
enum class ELocalizedTextCase : uint8
{
    Default            UMETA(DisplayName = "Default"),
    Uppercase          UMETA(DisplayName = "Uppercase"),
    Lowercase          UMETA(DisplayName = "Lowercase"),
    TitleCase          UMETA(DisplayName = "Title Case"),
    SentenceCase       UMETA(DisplayName = "Sentence Case")
};

// Text truncation options
UENUM(BlueprintType)
enum class ETextTruncation : uint8
{
    None               UMETA(DisplayName = "None"),
    Character          UMETA(DisplayName = "Character"),
    Word               UMETA(DisplayName = "Word"),
    Ellipsis           UMETA(DisplayName = "Ellipsis")
};

// Text update mode
UENUM(BlueprintType)
enum class ELocalizationUpdateMode : uint8
{
    Auto               UMETA(DisplayName = "Auto"),
    Manual             UMETA(DisplayName = "Manual"),
    OnLanguageChanged  UMETA(DisplayName = "On Language Changed")
};

/**
 * ?¬åœ°?–æ??¬ç?ä»? * ?¯æ??ªå?èªè??‡æ??„æ??¬é¡¯ç¤ºç?ä»? */
UCLASS(ClassGroup = (Localization), Blueprintable, meta = (DisableNativeTick))
class MINGPERSONAL_API UMingLocalizedWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLocalizedWidget(const FObjectInitializer& ObjectInitializer};

    // ?å???    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // === ?¬åœ°?–éµè¨­ç½® ===

    // è¨­ç½®?¬åœ°?–éµ
    UFUNCTION(BlueprintCallable, Category = "Localized Widget")
    void SetLocalizationKey(const FString& Key, const FString& Namespace = TEXT("Default")};

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetLocalizationKey() const { return LocalizationKey; }

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetNamespace() const { return Namespace; }

    // === ?ƒæ•¸?¿æ? ===

    // è¨­ç½®?ƒæ•¸
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameter(const FString& ParamName, const FString& Value};

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameters(const TMap<FString, FString>& Params};

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void ClearParameters(};

    // === ?‡æœ¬?´æ–° ===

    // ?‹å??´æ–°?‡æœ¬
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void UpdateText(};

    // å¼·åˆ¶?·æ–°
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void ForceRefresh(};

    // === ?‡æœ¬?•ç? ===

    // è¨­ç½®?‡æœ¬å¤§å?å¯?    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTextCase(ELocalizedTextCase NewCase};

    UFUNCTION(BlueprintPure, Category = "Localized Widget|Formatting")
    ELocalizedTextCase GetTextCase() const { return TextCase; }

    // è¨­ç½®?ªæ–·?¸é?
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTruncation(ETextTruncation TruncationType, int32 MaxLength = 100};

    // === å±¬æ€?===

    // ?¬åœ°?–éµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString LocalizationKey;

    // ?½å?ç©ºé?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString Namespace;

    // é»˜è??‡æœ¬ (?¾ä??°æœ¬?°å??‚é¡¯ç¤?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    FText DefaultText;

    // ?´æ–°æ¨¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    ELocalizationUpdateMode UpdateMode;

    // ?‡æœ¬å¤§å?å¯?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ELocalizedTextCase TextCase;

    // ?ªæ–·è¨­ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ETextTruncation Truncation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting", meta = (EditCondition = "Truncation != ETextTruncation::None"))
    int32 MaxTextLength;

    // ä½¿ç”¨å¯Œæ???    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bUseRichText;

    // ?ªå??›è?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bAutoWrapText;

    // === äº‹ä»¶?•ç? ===

    // èªè??¹è??‚ç??èª¿
    UFUNCTION()
    void OnLanguageChanged(EMingLanguage NewLanguage};

    // ?²å??¶å?é¡¯ç¤º?„æ???    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetCurrentText() const;

    // ?²å??Ÿå??¬åœ°?–æ???(?ªè???
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetRawLocalizedText() const;

protected:
    // ?ƒæ•¸å­˜å„²
    UPROPERTY()
    TMap<FString, FString> Parameters;

    // ?¶å?é¡¯ç¤º?„æ???    UPROPERTY()
    FText CurrentText;

    // ç¶å??°æœ¬?°å?ç³»çµ±
    void BindToLocalizationSystem(};

    // è§?™¤ç¶å?
    void UnbindFromLocalizationSystem(};

    // ?‰ç”¨?‡æœ¬?•ç? (å¤§å?å¯«ã€æˆª?·ç?)
    FText ProcessText(const FText& RawText) const;

    // ?‰ç”¨å¤§å?å¯«è???    FText ApplyTextCase(const FText& Text) const;

    // ?‰ç”¨?ªæ–·
    FText ApplyTruncation(const FText& Text) const;

    // ?ƒæ•¸?¿æ?
    FText ReplaceParameters(const FText& Text) const;

    // ?²å??¬åœ°?–ç®¡?†å™¨
    UMingLocalizationManager* GetLocalizationManager() const;
};

