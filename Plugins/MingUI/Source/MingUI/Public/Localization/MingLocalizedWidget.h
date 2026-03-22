#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingLocalizedWidget.generated.h"

// Forward declarations
class UMingLocalizationManager;

// Text case options
UENUM(BlueprintType)
enum class ELocalizedTextCase: uint8 {
    Default            UMETA(DisplayName = "Default"),
    Uppercase          UMETA(DisplayName = "Uppercase"),
    Lowercase          UMETA(DisplayName = "Lowercase"),
    TitleCase          UMETA(DisplayName = "Title Case"),
    SentenceCase       UMETA(DisplayName = "Sentence Case")
};

// Text truncation options
UENUM(BlueprintType)
enum class ETextTruncation: uint8 {
    None               UMETA(DisplayName = "None"),
    Character          UMETA(DisplayName = "Character"),
    Word               UMETA(DisplayName = "Word"),
    Ellipsis           UMETA(DisplayName = "Ellipsis")
};

// Text update mode
UENUM(BlueprintType)
enum class ELocalizationUpdateMode: uint8 {
    Auto               UMETA(DisplayName = "Auto"),
    Manual             UMETA(DisplayName = "Manual"),
    OnLanguageChanged  UMETA(DisplayName = "On Language Changed")
};

/**
 * ??�a目標數量?? * 目標數量�y?X??X??X?���摧毀 */
UCLASS(ClassGroup = (Localization), Blueprintable, meta = (DisableNativeTick))
class MINGPERSONAL_API UMingLocalizedWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLocalizedWidget(const FObjectInitializer& ObjectInitializer);

    // ???X
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // === ??�a??��]�m ===

    // �]�m??�a??��
    UFUNCTION(BlueprintCallable, Category = "Localized Widget")
    void SetLocalizationKey(const FString& Key, const FString& Namespace = TEXT("Default")};

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetLocalizationKey() const { return LocalizationKey; }

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetNamespace() const { return Namespace; }

    // === ??��摧毀 ===

    // �]�m??��
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameter(const FString& ParamName, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameters(const TMap<FString, FString>& Params);

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void ClearParameters();

    // === ??��??�s ===

    // ???X?�s??��
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void UpdateText();

    // �j��??�s
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void ForceRefresh();

    // === ??��摧毀 ===

    // �]�m??���j摧毀
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTextCase(ELocalizedTextCase NewCase);

    UFUNCTION(BlueprintPure, Category = "Localized Widget|Formatting")
    ELocalizedTextCase GetTextCase() const { return TextCase; }

    // �]�m??�_摧毀
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTruncation(ETextTruncation TruncationType, int32 MaxLength = 100);

    // === ��??===

    // ??�a??��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString LocalizationKey;

    // 摧毀��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString Namespace;

    // �q?X?�� (???X?��???X?��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    FText DefaultText;

    // ??�s��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    ELocalizationUpdateMode UpdateMode;

    // ??���j摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ELocalizedTextCase TextCase;

    // ??�_�]�m
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ETextTruncation Truncation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting", meta = (EditCondition = "Truncation != ETextTruncation::None"))
    int32 MaxTextLength;

    // �ϥδI?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bUseRichText;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bAutoWrapText;

    // === �ƥ�摧毀 ===

    // �y?X??X??X?��
    UFUNCTION()
    void OnLanguageChanged(EMingLanguage NewLanguage);

    // 目標數量���???X
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetCurrentText() const;

    // 故事重要性?�a???X(???X
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetRawLocalizedText() const;

protected:
    // ??�Ʀs�x
    UPROPERTY()
    TMap<FString, FString> Parameters;

    // 摧毀���???X
    UPROPERTY()
    FText CurrentText;

    // �j?X?��摧毀�t��
    void BindToLocalizationSystem();

    // 摧毀�j??
    void UnbindFromLocalizationSystem();

    // ??��??��摧毀 (�j??�g�B�I摧毀)
    FText ProcessText(const FText& RawText) const;

    // ??�Τj??�g?X
    FText ApplyTextCase(const FText& Text) const;

    // ??��??�_
    FText ApplyTruncation(const FText& Text) const;

    // ??��摧毀
    FText ReplaceParameters(const FText& Text) const;

    // ???X?�a??��??��
    UMingLocalizationManager* GetLocalizationManager() const;
};

