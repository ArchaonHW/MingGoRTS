#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCultureAdapter.generated.h"

// Cultural sensitivity levels
UENUM(BlueprintType)
enum class ECulturalSensitivity : uint8
{
    None               UMETA(DisplayName = "None"),
    Low                UMETA(DisplayName = "Low"),
    Medium             UMETA(DisplayName = "Medium"),
    High               UMETA(DisplayName = "High"),
    Critical           UMETA(DisplayName = "Critical")
};

// Content appropriateness
UENUM(BlueprintType)
enum class EContentAppropriateness : uint8
{
    Appropriate        UMETA(DisplayName = "Appropriate"),
    Caution            UMETA(DisplayName = "Caution"),
    Inappropriate      UMETA(DisplayName = "Inappropriate"),
    Blocked            UMETA(DisplayName = "Blocked"),
    NeedsReview        UMETA(DisplayName = "Needs Review")
};

// Cultural content type
UENUM(BlueprintType)
enum class ECulturalContentType : uint8
{
    Religious          UMETA(DisplayName = "Religious"),
    Political          UMETA(DisplayName = "Political"),
    Historical         UMETA(DisplayName = "Historical"),
    Social             UMETA(DisplayName = "Social"),
    Visual             UMETA(DisplayName = "Visual"),
    Audio              UMETA(DisplayName = "Audio"),
    Text               UMETA(DisplayName = "Text"),
    Symbol             UMETA(DisplayName = "Symbol"),
    Gesture            UMETA(DisplayName = "Gesture"),
    Color              UMETA(DisplayName = "Color"),
    Number             UMETA(DisplayName = "Number"),
    Holiday            UMETA(DisplayName = "Holiday")
};

// Cultural content information
USTRUCT(BlueprintType)
struct FCulturalContentInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECulturalContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECulturalSensitivity SensitivityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AlternativeContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Explanation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresAdaptation;

    FCulturalContentInfo()
        : ContentType(ECulturalContentType::Text)
        , SensitivityLevel(ECulturalSensitivity::None)
        , bRequiresAdaptation(false)
    {}
};

// Cultural preferences for a region
USTRUCT(BlueprintType)
struct FCulturalPreferences
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegionCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ECulturalContentType> SensitiveContentTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ColorMeanings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, FString> NumberMeanings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TabooTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PreferredVisualStyles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresGenderSeparation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRestrictsReligiousContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasPoliticalRestrictions;

    FCulturalPreferences()
        : bRequiresGenderSeparation(false)
        , bRestrictsReligiousContent(false)
        , bHasPoliticalRestrictions(false)
    {}
};

// Content adaptation rule
USTRUCT(BlueprintType)
struct FContentAdaptationRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RuleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SourceContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetRegion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AdaptedContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECulturalContentType ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AdaptationDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;

    FContentAdaptationRule()
        : ContentType(ECulturalContentType::Text)
        , bIsActive(true)
    {}
};

/**
 * ?‡å??©æ?ç³»çµ±
 * ?•ç??‡å??æ??§å®¹?Œåœ°?€å·®ç•°
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingCultureAdapter : public UObject
{
    GENERATED_BODY()

public:
    UMingCultureAdapter(};

    // ?å???    void Initialize(};
    void Shutdown(};

    UFUNCTION(BlueprintPure, Category = "Culture Adapter")
    bool IsInitialized() const { return bIsInitialized; }

    // === ?‡å??©æ?æª¢æŸ¥ ===

    // æª¢æŸ¥?§å®¹?¯å¦?©å??¹å??°å?
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    EContentAppropriateness CheckContentAppropriateness(const FString& ContentID, const FString& RegionCode) const;

    // ?²å??§å®¹?„æ??–æ??Ÿåº¦
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    ECulturalSensitivity GetContentSensitivity(const FString& ContentID) const;

    // æª¢æŸ¥?¯å¦?€è¦å…§å®¹é©??    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    bool NeedsAdaptation(const FString& ContentID, const FString& RegionCode) const;

    // === ?§å®¹?©æ? ===

    // ?²å??©æ?å¾Œç??§å®¹ID
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    FString GetAdaptedContentID(const FString& OriginalContentID, const FString& RegionCode) const;

    // ?‰ç”¨?‡å??©æ?è¦å?
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    bool ApplyAdaptationRule(const FContentAdaptationRule& Rule};

    // ç§»é™¤?©æ?è¦å?
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    bool RemoveAdaptationRule(const FString& RuleID};

    // ?²å??€?‰é©?‰è???    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Adaptation")
    TArray<FContentAdaptationRule> GetAllAdaptationRules() const;

    // === ?‡å??å¥½ ===

    // ?²å??°å??‡å??å¥½
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Preferences")
    FCulturalPreferences GetCulturalPreferences(const FString& RegionCode) const;

    // è¨­ç½®?°å??‡å??å¥½
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Preferences")
    void SetCulturalPreferences(const FString& RegionCode, const FCulturalPreferences& Preferences};

    // æª¢æŸ¥?¹å??§å®¹é¡å??¯å¦?æ?
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Preferences")
    bool IsContentTypeSensitive(ECulturalContentType ContentType, const FString& RegionCode) const;

    // === ç¬¦è??Œé???===

    // ?²å?é¡è‰²?¨ç‰¹å®šæ??–ä¸­?„å«ç¾?    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    FString GetColorMeaning(const FString& Color, const FString& RegionCode) const;

    // ?²å??¸å??¨ç‰¹å®šæ??–ä¸­?„å«ç¾?    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    FString GetNumberMeaning(int32 Number, const FString& RegionCode) const;

    // æª¢æŸ¥é¡è‰²?¯å¦?©å??¹å??°å?
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    bool IsColorAppropriate(const FString& Color, const FString& RegionCode) const;

    // ?²å??¿ä»£é¡è‰²å»ºè­°
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Symbols")
    TArray<FString> GetAlternativeColors(const FString& OriginalColor, const FString& RegionCode) const;

    // === å®—æ??Œæ”¿æ²»æ??Ÿå…§å®?===

    // æª¢æŸ¥?¯å¦?…å«å®—æ??æ??§å®¹
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Religion")
    bool ContainsReligiousContent(const FString& ContentID) const;

    // æª¢æŸ¥?¯å¦?…å«?¿æ²»?æ??§å®¹
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Political")
    bool ContainsPoliticalContent(const FString& ContentID) const;

    // ?²å??§å®¹?„æ›¿ä»????(?»æ??Ÿå?)
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Religion")
    FString GetSecularAlternative(const FString& ContentID) const;

    // === æ­·å²?Œæ??–å…§å®?===

    // æª¢æŸ¥æ­·å²?§å®¹?„æ?ç¢ºæ€?    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|History")
    bool ValidateHistoricalContent(const FString& ContentID, const FString& RegionCode) const;

    // ?²å?æ­·å²?§å®¹?„ä¿®æ­?»ºè­?    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|History")
    TArray<FString> GetHistoricalCorrections(const FString& ContentID, const FString& RegionCode) const;

    // === è¦–è¦º?§å®¹?©æ? ===

    // ?²å??©å??¹å??°å??„è?è¦ºé¢¨??    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Visual")
    TArray<FString> GetPreferredVisualStyles(const FString& RegionCode) const;

    // æª¢æŸ¥?–å??§å®¹?¯å¦?ˆé©
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Visual")
    EContentAppropriateness CheckImageContent(const FString& ImageID, const FString& RegionCode) const;

    // === ?§å®¹?æ¿¾ ===

    // ?æ¿¾?æ??§å®¹
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Filtering")
    TArray<FString> FilterSensitiveContent(const TArray<FString>& ContentIDs, const FString& RegionCode) const;

    // ?²å??§å®¹è­¦å?ä¿¡æ¯
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Filtering")
    FString GetContentWarning(const FString& ContentID, const FString& RegionCode) const;

    // === ?¸æ?ç®¡ç? ===

    // ? è??‡å??©æ??¸æ?
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    bool LoadCulturalData(const FString& FilePath};

    // ä¿å??‡å??©æ??¸æ?
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    bool SaveCulturalData(const FString& FilePath) const;

    // è¨»å??‡å??§å®¹ä¿¡æ¯
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    void RegisterCulturalContent(const FCulturalContentInfo& ContentInfo};

private:
    bool bIsInitialized;

    // ?‡å??§å®¹ä¿¡æ¯å­˜å„²
    UPROPERTY()
    TMap<FString, FCulturalContentInfo> CulturalContentRegistry;

    // ?°å??‡å??å¥½
    UPROPERTY()
    TMap<FString, FCulturalPreferences> RegionalPreferences;

    // ?§å®¹?©æ?è¦å?
    UPROPERTY()
    TArray<FContentAdaptationRule> AdaptationRules;

    // ?å??–é?èªæ??–æ•¸??    void InitializeDefaultCulturalData(};

    // ? è??è¨­?‡å?è¦å?
    void LoadPresetRules(};

    // ?²å??©æ?è¦å?
    const FContentAdaptationRule* FindAdaptationRule(const FString& ContentID, const FString& RegionCode) const;
};

