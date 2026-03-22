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
 * 文化適應系統
 * 處理文化敏感內容和地區差異
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingCultureAdapter : public UObject
{
    GENERATED_BODY()

public:
    UMingCultureAdapter();

    // 初始化
    void Initialize();
    void Shutdown();

    UFUNCTION(BlueprintPure, Category = "Culture Adapter")
    bool IsInitialized() const { return bIsInitialized; }

    // === 文化適應檢查 ===

    // 檢查內容是否適合特定地區
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    EContentAppropriateness CheckContentAppropriateness(const FString& ContentID, const FString& RegionCode) const;

    // 獲取內容的文化敏感度
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    ECulturalSensitivity GetContentSensitivity(const FString& ContentID) const;

    // 檢查是否需要內容適應
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Check")
    bool NeedsAdaptation(const FString& ContentID, const FString& RegionCode) const;

    // === 內容適應 ===

    // 獲取適應後的內容ID
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    FString GetAdaptedContentID(const FString& OriginalContentID, const FString& RegionCode) const;

    // 應用文化適應規則
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    bool ApplyAdaptationRule(const FContentAdaptationRule& Rule);

    // 移除適應規則
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Adaptation")
    bool RemoveAdaptationRule(const FString& RuleID);

    // 獲取所有適應規則
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Adaptation")
    TArray<FContentAdaptationRule> GetAllAdaptationRules() const;

    // === 文化偏好 ===

    // 獲取地區文化偏好
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Preferences")
    FCulturalPreferences GetCulturalPreferences(const FString& RegionCode) const;

    // 設置地區文化偏好
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Preferences")
    void SetCulturalPreferences(const FString& RegionCode, const FCulturalPreferences& Preferences);

    // 檢查特定內容類型是否敏感
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Preferences")
    bool IsContentTypeSensitive(ECulturalContentType ContentType, const FString& RegionCode) const;

    // === 符號和顏色 ===

    // 獲取顏色在特定文化中的含義
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    FString GetColorMeaning(const FString& Color, const FString& RegionCode) const;

    // 獲取數字在特定文化中的含義
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    FString GetNumberMeaning(int32 Number, const FString& RegionCode) const;

    // 檢查顏色是否適合特定地區
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Symbols")
    bool IsColorAppropriate(const FString& Color, const FString& RegionCode) const;

    // 獲取替代顏色建議
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Symbols")
    TArray<FString> GetAlternativeColors(const FString& OriginalColor, const FString& RegionCode) const;

    // === 宗教和政治敏感內容 ===

    // 檢查是否包含宗教敏感內容
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Religion")
    bool ContainsReligiousContent(const FString& ContentID) const;

    // 檢查是否包含政治敏感內容
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Political")
    bool ContainsPoliticalContent(const FString& ContentID) const;

    // 獲取內容的替代版本 (去敏感化)
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Religion")
    FString GetSecularAlternative(const FString& ContentID) const;

    // === 歷史和文化內容 ===

    // 檢查歷史內容的準確性
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|History")
    bool ValidateHistoricalContent(const FString& ContentID, const FString& RegionCode) const;

    // 獲取歷史內容的修正建議
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|History")
    TArray<FString> GetHistoricalCorrections(const FString& ContentID, const FString& RegionCode) const;

    // === 視覺內容適應 ===

    // 獲取適合特定地區的視覺風格
    UFUNCTION(BlueprintPure, Category = "Culture Adapter|Visual")
    TArray<FString> GetPreferredVisualStyles(const FString& RegionCode) const;

    // 檢查圖像內容是否合適
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Visual")
    EContentAppropriateness CheckImageContent(const FString& ImageID, const FString& RegionCode) const;

    // === 內容過濾 ===

    // 過濾敏感內容
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Filtering")
    TArray<FString> FilterSensitiveContent(const TArray<FString>& ContentIDs, const FString& RegionCode) const;

    // 獲取內容警告信息
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Filtering")
    FString GetContentWarning(const FString& ContentID, const FString& RegionCode) const;

    // === 數據管理 ===

    // 加載文化適應數據
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    bool LoadCulturalData(const FString& FilePath);

    // 保存文化適應數據
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    bool SaveCulturalData(const FString& FilePath) const;

    // 註冊文化內容信息
    UFUNCTION(BlueprintCallable, Category = "Culture Adapter|Data")
    void RegisterCulturalContent(const FCulturalContentInfo& ContentInfo);

private:
    bool bIsInitialized;

    // 文化內容信息存儲
    UPROPERTY()
    TMap<FString, FCulturalContentInfo> CulturalContentRegistry;

    // 地區文化偏好
    UPROPERTY()
    TMap<FString, FCulturalPreferences> RegionalPreferences;

    // 內容適應規則
    UPROPERTY()
    TArray<FContentAdaptationRule> AdaptationRules;

    // 初始化默認文化數據
    void InitializeDefaultCulturalData();

    // 加載預設文化規則
    void LoadPresetRules();

    // 獲取適應規則
    const FContentAdaptationRule* FindAdaptationRule(const FString& ContentID, const FString& RegionCode) const;
};
