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
 * ?�地?��X��?�? * ?��X��?語�X��X��X�顯示�?�? */
UCLASS(ClassGroup = (Localization), Blueprintable, meta = (DisableNativeTick))
class MINGPERSONAL_API UMingLocalizedWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLocalizedWidget(const FObjectInitializer& ObjectInitializer};

    // ?��X    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // === ?�地?�鍵設置 ===

    // 設置?�地?�鍵
    UFUNCTION(BlueprintCallable, Category = "Localized Widget")
    void SetLocalizationKey(const FString& Key, const FString& Namespace = TEXT("Default")};

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetLocalizationKey() const { return LocalizationKey; }

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetNamespace() const { return Namespace; }

    // === ?�數?��? ===

    // 設置?�數
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameter(const FString& ParamName, const FString& Value};

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameters(const TMap<FString, FString>& Params};

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void ClearParameters(};

    // === ?�本?�新 ===

    // ?��X�新?�本
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void UpdateText(};

    // 強制?�新
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void ForceRefresh(};

    // === ?�本?��? ===

    // 設置?�本大�?�?    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTextCase(ELocalizedTextCase NewCase};

    UFUNCTION(BlueprintPure, Category = "Localized Widget|Formatting")
    ELocalizedTextCase GetTextCase() const { return TextCase; }

    // 設置?�斷?��?
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTruncation(ETextTruncation TruncationType, int32 MaxLength = 100};

    // === 屬�?===

    // ?�地?�鍵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString LocalizationKey;

    // ?��?空�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString Namespace;

    // 默�X�本 (?��X�本?��X�顯�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    FText DefaultText;

    // ?�新模�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    ELocalizationUpdateMode UpdateMode;

    // ?�本大�?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ELocalizedTextCase TextCase;

    // ?�斷設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ETextTruncation Truncation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting", meta = (EditCondition = "Truncation != ETextTruncation::None"))
    int32 MaxTextLength;

    // 使用富�X    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bUseRichText;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bAutoWrapText;

    // === 事件?��? ===

    // 語�X��X��X�調
    UFUNCTION()
    void OnLanguageChanged(EMingLanguage NewLanguage};

    // ?��X��?顯示?��X    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetCurrentText() const;

    // ?��X��X�地?��X(?��X
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetRawLocalizedText() const;

protected:
    // ?�數存儲
    UPROPERTY()
    TMap<FString, FString> Parameters;

    // ?��?顯示?��X    UPROPERTY()
    FText CurrentText;

    // 綁�X�本?��?系統
    void BindToLocalizationSystem(};

    // �?��綁�?
    void UnbindFromLocalizationSystem(};

    // ?�用?�本?��? (大�?寫、截?��?)
    FText ProcessText(const FText& RawText) const;

    // ?�用大�?寫�X    FText ApplyTextCase(const FText& Text) const;

    // ?�用?�斷
    FText ApplyTruncation(const FText& Text) const;

    // ?�數?��?
    FText ReplaceParameters(const FText& Text) const;

    // ?��X�地?�管?�器
    UMingLocalizationManager* GetLocalizationManager() const;
};

