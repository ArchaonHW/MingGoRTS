#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLanguageSystem.generated.h"

// Forward declarations
class UMingLocalizationManager;

// Language proficiency levels
UENUM(BlueprintType)
enum class ELanguageProficiency : uint8
{
    Native             UMETA(DisplayName = "Native"),
    Fluent             UMETA(DisplayName = "Fluent"),
    Advanced           UMETA(DisplayName = "Advanced"),
    Intermediate       UMETA(DisplayName = "Intermediate"),
    Basic              UMETA(DisplayName = "Basic"),
    None               UMETA(DisplayName = "None")
};

// Translation quality levels
UENUM(BlueprintType)
enum class ETranslationQuality : uint8
{
    Professional       UMETA(DisplayName = "Professional"),
    Good               UMETA(DisplayName = "Good"),
    Acceptable         UMETA(DisplayName = "Acceptable"),
    Machine            UMETA(DisplayName = "Machine"),
    Draft              UMETA(DisplayName = "Draft")
};

// Language variant (for Chinese)
UENUM(BlueprintType)
enum class EChineseVariant : uint8
{
    Simplified         UMETA(DisplayName = "Simplified"),
    Traditional        UMETA(DisplayName = "Traditional"),
    Both               UMETA(DisplayName = "Both")
};

// Language pair for translation
USTRUCT(BlueprintType)
struct FLanguagePair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SourceLanguage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetLanguage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETranslationQuality Quality;

    FLanguagePair()
        : Quality(ETranslationQuality::Acceptable)
    {}
};

// Language statistics
USTRUCT(BlueprintType)
struct FLanguageStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString LanguageCode;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalStrings;

    UPROPERTY(BlueprintReadOnly)
    int32 TranslatedStrings;

    UPROPERTY(BlueprintReadOnly)
    int32 ReviewedStrings;

    UPROPERTY(BlueprintReadOnly)
    float TranslationProgress;

    UPROPERTY(BlueprintReadOnly)
    float ReviewProgress;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> MissingKeys;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> OutdatedKeys;

    FLanguageStats()
        : TotalStrings(0)
        , TranslatedStrings(0)
        , ReviewedStrings(0)
        , TranslationProgress(0.0f)
        , ReviewProgress(0.0f)
    {}
};

/**
 * 語言系統
 * 管理多語言支持和翻譯功能
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingLanguageSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingLanguageSystem();

    // 初始化
    void Initialize();
    void Shutdown();

    UFUNCTION(BlueprintPure, Category = "Language System")
    bool IsInitialized() const { return bIsInitialized; }

    // === 語言檢測 ===

    // 自動檢測文本語言
    UFUNCTION(BlueprintCallable, Category = "Language System|Detection")
    FString DetectLanguage(const FString& Text) const;

    // 檢測語言置信度
    UFUNCTION(BlueprintCallable, Category = "Language System|Detection")
    float GetLanguageConfidence(const FString& Text, const FString& LanguageCode) const;

    // === 翻譯功能 ===

    // 機器翻譯 (基礎實現)
    UFUNCTION(BlueprintCallable, Category = "Language System|Translation")
    FString TranslateText(const FString& Text, const FString& SourceLanguage, const FString& TargetLanguage) const;

    // 批量翻譯
    UFUNCTION(BlueprintCallable, Category = "Language System|Translation")
    TMap<FString, FString> TranslateBatch(const TArray<FString>& Texts, const FString& SourceLanguage, const FString& TargetLanguage) const;

    // 翻譯並替換參數
    UFUNCTION(BlueprintCallable, Category = "Language System|Translation")
    FString TranslateWithParams(const FString& Text, const TMap<FString, FString>& Params, const FString& SourceLanguage, const FString& TargetLanguage) const;

    // === 文本處理 ===

    // 獲取文本字符數 (考慮亞洲語言)
    UFUNCTION(BlueprintPure, Category = "Language System|Text Processing")
    int32 GetCharacterCount(const FString& Text, const FString& LanguageCode) const;

    // 檢查是否需要換行
    UFUNCTION(BlueprintPure, Category = "Language System|Text Processing")
    bool NeedsLineBreak(const FString& Text, float MaxWidth, float CharWidth) const;

    // 文本截斷
    UFUNCTION(BlueprintPure, Category = "Language System|Text Processing")
    FString TruncateText(const FString& Text, int32 MaxLength, const FString& Suffix = TEXT("...")) const;

    // === 中文特有功能 ===

    // 簡體轉繁體
    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    FString SimplifiedToTraditional(const FString& Text) const;

    // 繁體轉簡體
    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    FString TraditionalToSimplified(const FString& Text) const;

    // 獲取漢字拼音
    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    FString GetPinyin(const FString& ChineseText) const;

    // 獲取漢字筆畫數
    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    int32 GetStrokeCount(const FString& ChineseChar) const;

    // === 語言統計 ===

    // 計算翻譯統計
    UFUNCTION(BlueprintCallable, Category = "Language System|Stats")
    FLanguageStats CalculateStats(const FString& LanguageCode) const;

    // 比較兩個語言版本的差異
    UFUNCTION(BlueprintCallable, Category = "Language System|Stats")
    TArray<FString> CompareLanguageVersions(const FString& LanguageCode1, const FString& LanguageCode2) const;

    // 查找缺失的翻譯
    UFUNCTION(BlueprintCallable, Category = "Language System|Stats")
    TArray<FString> FindMissingTranslations(const FString& BaseLanguage, const FString& TargetLanguage) const;

    // === 驗證和質量 ===

    // 驗證翻譯質量
    UFUNCTION(BlueprintCallable, Category = "Language System|Quality")
    ETranslationQuality ValidateTranslation(const FString& SourceText, const FString& TranslatedText) const;

    // 檢查翻譯一致性
    UFUNCTION(BlueprintCallable, Category = "Language System|Quality")
    bool CheckTranslationConsistency(const FString& Key, const TArray<FString>& LanguageCodes) const;

    // 獲取翻譯建議
    UFUNCTION(BlueprintCallable, Category = "Language System|Quality")
    TArray<FString> GetTranslationSuggestions(const FString& SourceText, const FString& TargetLanguage) const;

    // === 輔助功能 ===

    // 獲取語言的閱讀方向
    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    bool IsRightToLeft(const FString& LanguageCode) const;

    // 獲取語言的默認字體
    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    FString GetDefaultFont(const FString& LanguageCode) const;

    // 獲取語言的標點符號風格
    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    FString GetPunctuationStyle(const FString& LanguageCode) const;

    // 獲取語言支持的字體列表
    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    TArray<FString> GetSupportedFonts(const FString& LanguageCode) const;

private:
    bool bIsInitialized;

    // 語言特徵數據
    TMap<FString, TArray<FString>> LanguagePatterns;

    // 加載語言模式
    void LoadLanguagePatterns();

    // 初始化語言數據
    void InitializeLanguageData();

    // 漢字簡繁對照表 (簡化實現)
    TMap<TCHAR, TCHAR> SimplifiedToTraditionalMap;
    TMap<TCHAR, TCHAR> TraditionalToSimplifiedMap;

    // 加載簡繁對照表
    void LoadChineseConversionTables();
};
