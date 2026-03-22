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
 * 本地化文本組件
 * 支持自動語言切換的文本顯示組件
 */
UCLASS(ClassGroup = (Localization), Blueprintable, meta = (DisableNativeTick))
class MINGPERSONAL_API UMingLocalizedWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLocalizedWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // === 本地化鍵設置 ===

    // 設置本地化鍵
    UFUNCTION(BlueprintCallable, Category = "Localized Widget")
    void SetLocalizationKey(const FString& Key, const FString& Namespace = TEXT("Default"));

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetLocalizationKey() const { return LocalizationKey; }

    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FString GetNamespace() const { return Namespace; }

    // === 參數替換 ===

    // 設置參數
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameter(const FString& ParamName, const FString& Value);

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void SetParameters(const TMap<FString, FString>& Params);

    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Parameters")
    void ClearParameters();

    // === 文本更新 ===

    // 手動更新文本
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void UpdateText();

    // 強制刷新
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Update")
    void ForceRefresh();

    // === 文本處理 ===

    // 設置文本大小寫
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTextCase(ELocalizedTextCase NewCase);

    UFUNCTION(BlueprintPure, Category = "Localized Widget|Formatting")
    ELocalizedTextCase GetTextCase() const { return TextCase; }

    // 設置截斷選項
    UFUNCTION(BlueprintCallable, Category = "Localized Widget|Formatting")
    void SetTruncation(ETextTruncation TruncationType, int32 MaxLength = 100);

    // === 屬性 ===

    // 本地化鍵
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString LocalizationKey;

    // 命名空間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget", meta = (ExposeOnSpawn = true))
    FString Namespace;

    // 默認文本 (找不到本地化時顯示)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    FText DefaultText;

    // 更新模式
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    ELocalizationUpdateMode UpdateMode;

    // 文本大小寫
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ELocalizedTextCase TextCase;

    // 截斷設置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting")
    ETextTruncation Truncation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget|Formatting", meta = (EditCondition = "Truncation != ETextTruncation::None"))
    int32 MaxTextLength;

    // 使用富文本
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bUseRichText;

    // 自動換行
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localized Widget")
    bool bAutoWrapText;

    // === 事件處理 ===

    // 語言改變時的回調
    UFUNCTION()
    void OnLanguageChanged(EMingLanguage NewLanguage);

    // 獲取當前顯示的文本
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetCurrentText() const;

    // 獲取原始本地化文本 (未處理)
    UFUNCTION(BlueprintPure, Category = "Localized Widget")
    FText GetRawLocalizedText() const;

protected:
    // 參數存儲
    UPROPERTY()
    TMap<FString, FString> Parameters;

    // 當前顯示的文本
    UPROPERTY()
    FText CurrentText;

    // 綁定到本地化系統
    void BindToLocalizationSystem();

    // 解除綁定
    void UnbindFromLocalizationSystem();

    // 應用文本處理 (大小寫、截斷等)
    FText ProcessText(const FText& RawText) const;

    // 應用大小寫轉換
    FText ApplyTextCase(const FText& Text) const;

    // 應用截斷
    FText ApplyTruncation(const FText& Text) const;

    // 參數替換
    FText ReplaceParameters(const FText& Text) const;

    // 獲取本地化管理器
    UMingLocalizationManager* GetLocalizationManager() const;
};
