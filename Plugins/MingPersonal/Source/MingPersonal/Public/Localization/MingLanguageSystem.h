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
 * èªè?ç³»çµ±
 * ç®¡ç?å¤šè?è¨€?¯æ??Œç¿»è­¯å??? */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingLanguageSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingLanguageSystem(};

    // ?å???    void Initialize(};
    void Shutdown(};

    UFUNCTION(BlueprintPure, Category = "Language System")
    bool IsInitialized() const { return bIsInitialized; }

    // === èªè?æª¢æ¸¬ ===

    // ?ªå?æª¢æ¸¬?‡æœ¬èªè?
    UFUNCTION(BlueprintCallable, Category = "Language System|Detection")
    FString DetectLanguage(const FString& Text) const;

    // æª¢æ¸¬èªè?ç½®ä¿¡åº?    UFUNCTION(BlueprintCallable, Category = "Language System|Detection")
    float GetLanguageConfidence(const FString& Text, const FString& LanguageCode) const;

    // === ç¿»è­¯?Ÿèƒ½ ===

    // æ©Ÿå™¨ç¿»è­¯ (?ºç?å¯¦ç¾)
    UFUNCTION(BlueprintCallable, Category = "Language System|Translation")
    FString TranslateText(const FString& Text, const FString& SourceLanguage, const FString& TargetLanguage) const;

    // ?¹é?ç¿»è­¯
    UFUNCTION(BlueprintCallable, Category = "Language System|Translation")
    TMap<FString, FString> TranslateBatch(const TArray<FString>& Texts, const FString& SourceLanguage, const FString& TargetLanguage) const;

    // ç¿»è­¯ä¸¦æ›¿?›å???    UFUNCTION(BlueprintCallable, Category = "Language System|Translation")
    FString TranslateWithParams(const FString& Text, const TMap<FString, FString>& Params, const FString& SourceLanguage, const FString& TargetLanguage) const;

    // === ?‡æœ¬?•ç? ===

    // ?²å??‡æœ¬å­—ç¬¦??(?ƒæ…®äºæ´²èªè?)
    UFUNCTION(BlueprintPure, Category = "Language System|Text Processing")
    int32 GetCharacterCount(const FString& Text, const FString& LanguageCode) const;

    // æª¢æŸ¥?¯å¦?€è¦æ?è¡?    UFUNCTION(BlueprintPure, Category = "Language System|Text Processing")
    bool NeedsLineBreak(const FString& Text, float MaxWidth, float CharWidth) const;

    // ?‡æœ¬?ªæ–·
    UFUNCTION(BlueprintPure, Category = "Language System|Text Processing")
    FString TruncateText(const FString& Text, int32 MaxLength, const FString& Suffix = TEXT("...")) const;

    // === ä¸­æ??¹æ??Ÿèƒ½ ===

    // ç°¡é?è½‰ç?é«?    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    FString SimplifiedToTraditional(const FString& Text) const;

    // ç¹é?è½‰ç°¡é«?    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    FString TraditionalToSimplified(const FString& Text) const;

    // ?²å?æ¼¢å??¼éŸ³
    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    FString GetPinyin(const FString& ChineseText) const;

    // ?²å?æ¼¢å?ç­†ç•«??    UFUNCTION(BlueprintCallable, Category = "Language System|Chinese")
    int32 GetStrokeCount(const FString& ChineseChar) const;

    // === èªè?çµ±è? ===

    // è¨ˆç?ç¿»è­¯çµ±è?
    UFUNCTION(BlueprintCallable, Category = "Language System|Stats")
    FLanguageStats CalculateStats(const FString& LanguageCode) const;

    // æ¯”è??©å€‹è?è¨€?ˆæœ¬?„å·®??    UFUNCTION(BlueprintCallable, Category = "Language System|Stats")
    TArray<FString> CompareLanguageVersions(const FString& LanguageCode1, const FString& LanguageCode2) const;

    // ?¥æ‰¾ç¼ºå¤±?„ç¿»è­?    UFUNCTION(BlueprintCallable, Category = "Language System|Stats")
    TArray<FString> FindMissingTranslations(const FString& BaseLanguage, const FString& TargetLanguage) const;

    // === é©—è??Œè³ª??===

    // é©—è?ç¿»è­¯è³ªé?
    UFUNCTION(BlueprintCallable, Category = "Language System|Quality")
    ETranslationQuality ValidateTranslation(const FString& SourceText, const FString& TranslatedText) const;

    // æª¢æŸ¥ç¿»è­¯ä¸€?´æ€?    UFUNCTION(BlueprintCallable, Category = "Language System|Quality")
    bool CheckTranslationConsistency(const FString& Key, const TArray<FString>& LanguageCodes) const;

    // ?²å?ç¿»è­¯å»ºè­°
    UFUNCTION(BlueprintCallable, Category = "Language System|Quality")
    TArray<FString> GetTranslationSuggestions(const FString& SourceText, const FString& TargetLanguage) const;

    // === è¼”åŠ©?Ÿèƒ½ ===

    // ?²å?èªè??„é–±è®€?¹å?
    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    bool IsRightToLeft(const FString& LanguageCode) const;

    // ?²å?èªè??„é?èªå?é«?    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    FString GetDefaultFont(const FString& LanguageCode) const;

    // ?²å?èªè??„æ?é»ç¬¦?Ÿé¢¨??    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    FString GetPunctuationStyle(const FString& LanguageCode) const;

    // ?²å?èªè??¯æ??„å?é«”å?è¡?    UFUNCTION(BlueprintPure, Category = "Language System|Utils")
    TArray<FString> GetSupportedFonts(const FString& LanguageCode) const;

private:
    bool bIsInitialized;

    // èªè??¹å¾µ?¸æ?
    TMap<FString, TArray<FString>> LanguagePatterns;

    // ? è?èªè?æ¨¡å?
    void LoadLanguagePatterns(};

    // ?å??–è?è¨€?¸æ?
    void InitializeLanguageData(};

    // æ¼¢å?ç°¡ç?å°ç…§è¡?(ç°¡å?å¯¦ç¾)
    TMap<TCHAR, TCHAR> SimplifiedToTraditionalMap;
    TMap<TCHAR, TCHAR> TraditionalToSimplifiedMap;

    // ? è?ç°¡ç?å°ç…§è¡?    void LoadChineseConversionTables(};
};

