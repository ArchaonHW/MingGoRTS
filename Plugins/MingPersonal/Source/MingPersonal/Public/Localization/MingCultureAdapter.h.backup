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
 * ???X???系統
 * ???X??X??X?容??地??差異
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingCultureAdapter : public UObject
{
    GENERATED_BODY()

public:
    UMingCultureAdapter(};

    // ???X    void Initialize(};
    void Shutdown();

    UFUNCTION(BlueprintPure, Category = "Culture Adapter")
    bool IsInitialized() const { return bIsInitialized; }

    // === ???X???檢查 ===

    // 檢查??容??否???X??X???
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    EContentAppropriateness CheckContentAppropriateness(const FString& ContentID, const FString& RegionCode) const;

    // ???X?容???X??X?度
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    ECulturalSensitivity GetContentSensitivity(const FString& ContentID) const;

    // 檢查??否??要內容適X    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    bool NeedsAdaptation(const FString& ContentID, const FString& RegionCode) const;

    // === ??容???? ===

    // ???X???後?X?容ID
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    FString GetAdaptedContentID(const FString& OriginalContentID, const FString& RegionCode) const;

    // ??用???X???規??
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    bool ApplyAdaptationRule(const FContentAdaptationRule& Rule);

    // 移除????規??
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    bool RemoveAdaptationRule(const FString& RuleID);

    // ???X???適???X    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Adaptation")
    TArray<FContentAdaptationRule> GetAllAdaptationRules() const;

    // === ???X?好 ===

    // ???X??X??X?好
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Preferences")
    FCulturalPreferences GetCulturalPreferences(const FString& RegionCode) const;

    // 設置???X??X?好
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Preferences")
    void SetCulturalPreferences(const FString& RegionCode, const FCulturalPreferences& Preferences);

    // 檢查???X?容類?X?否????
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Preferences")
    bool IsContentTypeSensitive(ECulturalContentType ContentType, const FString& RegionCode) const;

    // === 符?X??X===

    // ????顏色??特定?X?中??含??    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    FString GetColorMeaning(const FString& Color, const FString& RegionCode) const;

    // ???X??X?特定?X?中??含??    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    FString GetNumberMeaning(int32 Number, const FString& RegionCode) const;

    // 檢查顏色??否???X??X???
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    bool IsColorAppropriate(const FString& Color, const FString& RegionCode) const;

    // ???X?代顏色建議
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Symbols")
    TArray<FString> GetAlternativeColors(const FString& OriginalColor, const FString& RegionCode) const;

    // === 宗?X?政治?X?內??===

    // 檢查??否??含宗?X??X?容
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Religion")
    bool ContainsReligiousContent(const FString& ContentID) const;

    // 檢查??否??含??治???X?容
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Political")
    bool ContainsPoliticalContent(const FString& ContentID) const;

    // ???X?容??替?X(???X???)
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Religion")
    FString GetSecularAlternative(const FString& ContentID) const;

    // === 歷史???X?內??===

    // 檢查歷史??容????確??    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|History")
    bool ValidateHistoricalContent(const FString& ContentID, const FString& RegionCode) const;

    // ????歷史??容??修??????    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|History")
    TArray<FString> GetHistoricalCorrections(const FString& ContentID, const FString& RegionCode) const;

    // === 視覺??容???? ===

    // ???X??X??X??X???覺風X    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Visual")
    TArray<FString> GetPreferredVisualStyles(const FString& RegionCode) const;

    // 檢查???X?容??否??適
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Visual")
    EContentAppropriateness CheckImageContent(const FString& ImageID, const FString& RegionCode) const;

    // === ??容??濾 ===

    // ??濾???X?容
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Filtering")
    TArray<FString> FilterSensitiveContent(const TArray<FString>& ContentIDs, const FString& RegionCode) const;

    // ???X?容警??信息
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Filtering")
    FString GetContentWarning(const FString& ContentID, const FString& RegionCode) const;

    // === ????管?? ===

    // ???X??X??X???
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    bool LoadCulturalData(const FString& FilePath);

    // 保?X??X??X???
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    bool SaveCulturalData(const FString& FilePath) const;

    // 註?X??X?容信息
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    void RegisterCulturalContent(const FCulturalContentInfo& ContentInfo);

private:
    bool bIsInitialized;

    // ???X?容信息存儲
    UPROPERTY()
    TMap<FString, FCulturalContentInfo> CulturalContentRegistry;

    // ???X??X?好
    UPROPERTY()
    TMap<FString, FCulturalPreferences> RegionalPreferences;

    // ??容????規??
    UPROPERTY()
    TArray<FContentAdaptationRule> AdaptationRules;

    // ???X???認?X?數X    void InitializeDefaultCulturalData(};

    // ???X?設????規??
    void LoadPresetRules();

    // ???X???規??
    const FContentAdaptationRule* FindAdaptationRule(const FString& ContentID, const FString& RegionCode) const;
};

