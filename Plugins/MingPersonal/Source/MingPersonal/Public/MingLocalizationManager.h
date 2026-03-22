#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLocalizationManager.generated.h"

/**
 * 支持的語言枚舉
 */
UENUM(BlueprintType)
enum class EMingLanguage : uint8
{
    ChineseTraditional  UMETA(DisplayName = "繁體中文"),
    ChineseSimplified   UMETA(DisplayName = "簡體中文"),
    English             UMETA(DisplayName = "English"),
    Japanese            UMETA(DisplayName = "日本語"),
    Korean              UMETA(DisplayName = "한국어"),
    French              UMETA(DisplayName = "Français"),
    German              UMETA(DisplayName = "Deutsch"),
    Spanish             UMETA(DisplayName = "Español")
};

/**
 * 支持的區域枚舉
 */
UENUM(BlueprintType)
enum class EMingRegion : uint8
{
    Taiwan              UMETA(DisplayName = "台灣"),
    MainlandChina       UMETA(DisplayName = "中國大陸"),
    HongKong            UMETA(DisplayName = "香港"),
    Japan               UMETA(DisplayName = "日本"),
    Korea               UMETA(DisplayName = "韓國"),
    UnitedStates        UMETA(DisplayName = "美國"),
    Europe              UMETA(DisplayName = "歐洲"),
    SoutheastAsia       UMETA(DisplayName = "東南亞")
};

/**
 * 本地化文本結構
 */
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FLocalizedText
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString Key;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString Namespace;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    TMap<EMingLanguage, FString> Translations;

    FLocalizedText()
    {
        Key = TEXT("");
        Namespace = TEXT("Default");
    }
};

/**
 * 語言包信息結構
 */
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FLanguagePackInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    EMingLanguage Language;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString LanguageName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString NativeName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    int32 TotalStrings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    int64 FileSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    bool bIsDownloaded;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    bool bIsDefault;

    FLanguagePackInfo()
    {
        Language = EMingLanguage::ChineseTraditional;
        LanguageName = TEXT("Traditional Chinese");
        NativeName = TEXT("繁體中文");
        Version = TEXT("1.0");
        TotalStrings = 0;
        FileSize = 0;
        bIsDownloaded = false;
        bIsDefault = false;
    }
};

/**
 * 文化適應配置
 */
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FCulturalAdaptationConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    EMingRegion TargetRegion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    bool bAdaptHistoricalReferences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    bool bAdaptCulturalContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    bool bAdaptUIElements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    bool bAdaptAudioContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    TArray<FString> SensitiveContentFilters;

    FCulturalAdaptationConfig()
    {
        TargetRegion = EMingRegion::Taiwan;
        bAdaptHistoricalReferences = true;
        bAdaptCulturalContent = true;
        bAdaptUIElements = true;
        bAdaptAudioContent = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLanguageChanged, EMingLanguage, NewLanguage, EMingLanguage, OldLanguage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionChanged, EMingRegion, NewRegion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLanguagePackDownloaded, EMingLanguage, Language);

/**
 * 本地化管理器
 * 負責多語言支持、區域適應和文化本地化
 */
UCLASS()
class MINGPERSONAL_API UMingLocalizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingLocalizationManager();

    // 初始化與關閉
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void InitializeLocalization(UObject* InOwner);

    UFUNCTION(BlueprintCallable, Category = "Localization")
    void Shutdown();

    // 語言設置
    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool SetLanguage(EMingLanguage NewLanguage);

    UFUNCTION(BlueprintPure, Category = "Localization")
    EMingLanguage GetCurrentLanguage() const { return CurrentLanguage; }

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetCurrentLanguageName() const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetLanguageName(EMingLanguage Language) const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    TArray<EMingLanguage> GetSupportedLanguages() const;

    // 區域設置
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetRegion(EMingRegion NewRegion);

    UFUNCTION(BlueprintPure, Category = "Localization")
    EMingRegion GetCurrentRegion() const { return CurrentRegion; }

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetRegionName(EMingRegion Region) const;

    // 文本本地化
    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetLocalizedString(const FString& Key, const FString& Namespace = TEXT("Default")) const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetLocalizedStringWithParams(const FString& Key, const TMap<FString, FString>& Params, const FString& Namespace = TEXT("Default")) const;

    // 語言包管理
    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool DownloadLanguagePack(EMingLanguage Language);

    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool IsLanguagePackDownloaded(EMingLanguage Language) const;

    UFUNCTION(BlueprintPure, Category = "Localization")
    FLanguagePackInfo GetLanguagePackInfo(EMingLanguage Language) const;

    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetDefaultLanguage(EMingLanguage Language);

    // 文化適應
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetCulturalAdaptationConfig(const FCulturalAdaptationConfig& Config);

    UFUNCTION(BlueprintPure, Category = "Localization")
    FCulturalAdaptationConfig GetCulturalAdaptationConfig() const { return CulturalConfig; }

    UFUNCTION(BlueprintPure, Category = "Localization")
    FString GetCulturallyAdaptedContent(const FString& ContentKey, EMingContentType ContentType) const;

    // 載入與保存
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void LoadLocalizationData();

    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SaveLocalizationSettings();

    // 動態文本註冊
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void RegisterLocalizedText(const FLocalizedText& TextData);

    UFUNCTION(BlueprintCallable, Category = "Localization")
    void UnregisterLocalizedText(const FString& Key, const FString& Namespace = TEXT("Default"));

    // 批量操作
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void RegisterMultipleTexts(const TArray<FLocalizedText>& Texts);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Localization Events")
    FOnLanguageChanged OnLanguageChanged;

    UPROPERTY(BlueprintAssignable, Category = "Localization Events")
    FOnRegionChanged OnRegionChanged;

    UPROPERTY(BlueprintAssignable, Category = "Localization Events")
    FOnLanguagePackDownloaded OnLanguagePackDownloaded;

private:
    // 當前設置
    UPROPERTY()
    EMingLanguage CurrentLanguage;

    UPROPERTY()
    EMingRegion CurrentRegion;

    UPROPERTY()
    EMingLanguage DefaultLanguage;

    // 數據存儲
    UPROPERTY()
    TMap<FString, FLocalizedText> LocalizedTextMap;

    UPROPERTY()
    TMap<EMingLanguage, FLanguagePackInfo> LanguagePackMap;

    UPROPERTY()
    FCulturalAdaptationConfig CulturalConfig;

    UPROPERTY()
    TObjectPtr<UObject> Owner;

    bool bIsInitialized;

    // 內部方法
    void InitializeDefaultLanguagePacks();
    void InitializeDefaultTexts();
    FString GenerateTextKey(const FString& Key, const FString& Namespace) const;
    FString ProcessTextParams(const FString& Text, const TMap<FString, FString>& Params) const;
    void ApplyCulturalAdaptations();
};
