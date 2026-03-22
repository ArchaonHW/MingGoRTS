#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Internationalization/Text.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "MingLocalizationManager.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingLanguageSystem;
class UMingCultureAdapter;
class UMingRegionalContentManager;

// Supported languages
UENUM(BlueprintType)
enum class EMingLanguage : uint8
{
    ChineseSimplified  UMETA(DisplayName = "Chinese (Simplified)"),
    ChineseTraditional UMETA(DisplayName = "Chinese (Traditional)"),
    English            UMETA(DisplayName = "English"),
    Japanese           UMETA(DisplayName = "Japanese"),
    Korean             UMETA(DisplayName = "Korean"),
    French             UMETA(DisplayName = "French"),
    German             UMETA(DisplayName = "German"),
    Spanish            UMETA(DisplayName = "Spanish"),
    Russian            UMETA(DisplayName = "Russian"),
    Arabic             UMETA(DisplayName = "Arabic"),
    Portuguese         UMETA(DisplayName = "Portuguese"),
    Italian            UMETA(DisplayName = "Italian"),
    Dutch              UMETA(DisplayName = "Dutch"),
    Polish             UMETA(DisplayName = "Polish"),
    Turkish            UMETA(DisplayName = "Turkish"),
    Vietnamese         UMETA(DisplayName = "Vietnamese"),
    Thai               UMETA(DisplayName = "Thai"),
    Indonesian         UMETA(DisplayName = "Indonesian")
};

// Supported regions
UENUM(BlueprintType)
enum class EMingRegion : uint8
{
    MainlandChina      UMETA(DisplayName = "Mainland China"),
    Taiwan             UMETA(DisplayName = "Taiwan"),
    HongKong           UMETA(DisplayName = "Hong Kong"),
    Macau              UMETA(DisplayName = "Macau"),
    Japan              UMETA(DisplayName = "Japan"),
    SouthKorea         UMETA(DisplayName = "South Korea"),
    UnitedStates       UMETA(DisplayName = "United States"),
    UnitedKingdom      UMETA(DisplayName = "United Kingdom"),
    Germany            UMETA(DisplayName = "Germany"),
    France             UMETA(DisplayName = "France"),
    Russia             UMETA(DisplayName = "Russia"),
    Brazil             UMETA(DisplayName = "Brazil"),
    India              UMETA(DisplayName = "India"),
    SoutheastAsia      UMETA(DisplayName = "Southeast Asia"),
    MiddleEast         UMETA(DisplayName = "Middle East"),
    Global             UMETA(DisplayName = "Global")
};

// Text category for organization
UENUM(BlueprintType)
enum class EMingTextCategory : uint8
{
    General            UMETA(DisplayName = "General"),
    UI                 UMETA(DisplayName = "UI"),
    Dialogue           UMETA(DisplayName = "Dialogue"),
    Quest              UMETA(DisplayName = "Quest"),
    Tutorial           UMETA(DisplayName = "Tutorial"),
    Item               UMETA(DisplayName = "Item"),
    Character          UMETA(DisplayName = "Character"),
    Location           UMETA(DisplayName = "Location"),
    System             UMETA(DisplayName = "System"),
    Help               UMETA(DisplayName = "Help"),
    Achievement        UMETA(DisplayName = "Achievement"),
    Notification       UMETA(DisplayName = "Notification")
};

// Localization entry structure
USTRUCT(BlueprintType)
struct FLocalizationEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Key;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SourceText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText LocalizedText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTextCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bNeedsReview;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LastModifiedBy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastModifiedTime;

    FLocalizationEntry()
        : Category(EMingTextCategory::General)
        , bNeedsReview(false)
    {}
};

// Language pack info
USTRUCT(BlueprintType)
struct FLanguagePackInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMingLanguage Language;

    UPROPERTY(BlueprintReadOnly)
    FString LanguageName;

    UPROPERTY(BlueprintReadOnly)
    FString NativeName;

    UPROPERTY(BlueprintReadOnly)
    FString ISOCode;

    UPROPERTY(BlueprintReadOnly)
    bool bIsInstalled;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly)
    int32 EntryCount;

    UPROPERTY(BlueprintReadOnly)
    int32 TranslationProgress;

    UPROPERTY(BlueprintReadOnly)
    float PackSizeMB;

    UPROPERTY(BlueprintReadOnly)
    FString Version;

    FLanguagePackInfo()
        : Language(EMingLanguage::ChineseSimplified)
        , bIsInstalled(false)
        , bIsActive(false)
        , EntryCount(0)
        , TranslationProgress(0)
        , PackSizeMB(0.0f)
    {}
};

// Delegate declarations
declare dynamic_multicast_delegate(FOnLanguageChanged, EMingLanguage, NewLanguage);
declare dynamic_multicast_delegate(FOnRegionChanged, EMingRegion, NewRegion);
declare dynamic_multicast_delegate(FOnLocalizationLoaded, bool, bSuccess);

/**
 * 本地化管理器
 * 統一管理遊戲的多語言支持和地區適配
 */
UCLASS(ClassGroup = (Localization), Blueprintable)
class MINGPERSONAL_API UMingLocalizationManager : public UObject
{
    GENERATED_BODY()

public:
    UMingLocalizationManager();

    // 初始化本地化管理器
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void InitializeLocalization(UMingPersonalManager* InPersonalManager);

    // 關閉本地化管理器
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void ShutdownLocalization();

    UFUNCTION(BlueprintPure, Category = "Localization")
    bool IsInitialized() const { return bIsInitialized; }

    // === 語言管理 ===

    // 設置當前語言
    UFUNCTION(BlueprintCallable, Category = "Localization|Language")
    bool SetLanguage(EMingLanguage NewLanguage);

    UFUNCTION(BlueprintPure, Category = "Localization|Language")
    EMingLanguage GetCurrentLanguage() const { return CurrentLanguage; }

    // 獲取語言名稱
    UFUNCTION(BlueprintPure, Category = "Localization|Language")
    FString GetLanguageName(EMingLanguage Language) const;

    UFUNCTION(BlueprintPure, Category = "Localization|Language")
    FString GetCurrentLanguageName() const;

    // 獲取支持的所有語言
    UFUNCTION(BlueprintPure, Category = "Localization|Language")
    TArray<EMingLanguage> GetSupportedLanguages() const;

    // 檢查語言是否可用
    UFUNCTION(BlueprintPure, Category = "Localization|Language")
    bool IsLanguageAvailable(EMingLanguage Language) const;

    // 獲取語言包信息
    UFUNCTION(BlueprintPure, Category = "Localization|Language")
    FLanguagePackInfo GetLanguagePackInfo(EMingLanguage Language) const;

    // 下載語言包
    UFUNCTION(BlueprintCallable, Category = "Localization|Language")
    bool DownloadLanguagePack(EMingLanguage Language);

    // 卸載語言包
    UFUNCTION(BlueprintCallable, Category = "Localization|Language")
    bool UnloadLanguagePack(EMingLanguage Language);

    // === 地區管理 ===

    // 設置地區
    UFUNCTION(BlueprintCallable, Category = "Localization|Region")
    void SetRegion(EMingRegion NewRegion);

    UFUNCTION(BlueprintPure, Category = "Localization|Region")
    EMingRegion GetCurrentRegion() const { return CurrentRegion; }

    // 獲取地區名稱
    UFUNCTION(BlueprintPure, Category = "Localization|Region")
    FString GetRegionName(EMingRegion Region) const;

    // 獲取地區代碼 (ISO)
    UFUNCTION(BlueprintPure, Category = "Localization|Region")
    FString GetRegionCode(EMingRegion Region) const;

    // 根據系統設置自動檢測地區
    UFUNCTION(BlueprintCallable, Category = "Localization|Region")
    EMingRegion AutoDetectRegion() const;

    // === 文本本地化 ===

    // 獲取本地化文本
    UFUNCTION(BlueprintPure, Category = "Localization|Text", meta = (DisplayName = "Get Localized Text"))
    FText GetLocalizedText(const FString& Key, const FString& Namespace = TEXT("Default")) const;

    // 獲取帶參數的本地化文本
    UFUNCTION(BlueprintPure, Category = "Localization|Text")
    FText GetLocalizedTextWithParams(const FString& Key, const TMap<FString, FString>& Params, const FString& Namespace = TEXT("Default")) const;

    // 獲取本地化字符串
    UFUNCTION(BlueprintPure, Category = "Localization|Text")
    FString GetLocalizedString(const FString& Key, const FString& Namespace = TEXT("Default")) const;

    // 添加/更新本地化條目
    UFUNCTION(BlueprintCallable, Category = "Localization|Text")
    void AddLocalizationEntry(const FLocalizationEntry& Entry, EMingLanguage TargetLanguage);

    // 批量添加本地化條目
    UFUNCTION(BlueprintCallable, Category = "Localization|Text")
    void AddLocalizationEntries(const TArray<FLocalizationEntry>& Entries, EMingLanguage TargetLanguage);

    // 導出本地化文件
    UFUNCTION(BlueprintCallable, Category = "Localization|ImportExport")
    bool ExportLocalizationToCSV(const FString& FilePath, EMingLanguage Language) const;

    UFUNCTION(BlueprintCallable, Category = "Localization|ImportExport")
    bool ExportLocalizationToJSON(const FString& FilePath, EMingLanguage Language) const;

    // 導入本地化文件
    UFUNCTION(BlueprintCallable, Category = "Localization|ImportExport")
    bool ImportLocalizationFromCSV(const FString& FilePath, EMingLanguage TargetLanguage);

    UFUNCTION(BlueprintCallable, Category = "Localization|ImportExport")
    bool ImportLocalizationFromJSON(const FString& FilePath, EMingLanguage TargetLanguage);

    // === 格式化 ===

    // 格式化數字 (根據地區)
    UFUNCTION(BlueprintPure, Category = "Localization|Formatting")
    FString FormatNumber(int32 Number) const;

    UFUNCTION(BlueprintPure, Category = "Localization|Formatting")
    FString FormatNumberFloat(float Number, int32 DecimalPlaces = 2) const;

    // 格式化日期時間
    UFUNCTION(BlueprintPure, Category = "Localization|Formatting")
    FString FormatDateTime(const FDateTime& DateTime, const FString& Format = TEXT("yyyy-MM-dd")) const;

    // 格式化貨幣
    UFUNCTION(BlueprintPure, Category = "Localization|Formatting")
    FString FormatCurrency(float Amount, const FString& CurrencyCode = TEXT("CNY")) const;

    // === 文化適應 ===

    // 獲取文化特定設置
    UFUNCTION(BlueprintPure, Category = "Localization|Culture")
    FString GetDateFormat() const;

    UFUNCTION(BlueprintPure, Category = "Localization|Culture")
    FString GetTimeFormat() const;

    UFUNCTION(BlueprintPure, Category = "Localization|Culture")
    FString GetNumberFormat() const;

    UFUNCTION(BlueprintPure, Category = "Localization|Culture")
    FString GetCurrencySymbol() const;

    // 判斷是否從右到左書寫
    UFUNCTION(BlueprintPure, Category = "Localization|Culture")
    bool IsRightToLeft() const;

    // === 統計和報告 ===

    // 獲取翻譯完成度
    UFUNCTION(BlueprintPure, Category = "Localization|Stats")
    int32 GetTranslationProgress(EMingLanguage Language) const;

    // 獲取總條目數
    UFUNCTION(BlueprintPure, Category = "Localization|Stats")
    int32 GetTotalEntryCount() const;

    // 獲取待審查條目數
    UFUNCTION(BlueprintPure, Category = "Localization|Stats")
    int32 GetPendingReviewCount(EMingLanguage Language) const;

    // 生成本地化報告
    UFUNCTION(BlueprintCallable, Category = "Localization|Stats")
    FString GenerateLocalizationReport() const;

    // === 子系統訪問 ===

    UFUNCTION(BlueprintPure, Category = "Localization")
    UMingLanguageSystem* GetLanguageSystem() const { return LanguageSystem; }

    UFUNCTION(BlueprintPure, Category = "Localization")
    UMingCultureAdapter* GetCultureAdapter() const { return CultureAdapter; }

    UFUNCTION(BlueprintPure, Category = "Localization")
    UMingRegionalContentManager* GetRegionalContentManager() const { return RegionalContentManager; }

    // === 事件 ===

    UPROPERTY(BlueprintAssignable, Category = "Localization|Events")
    FOnLanguageChanged OnLanguageChanged;

    UPROPERTY(BlueprintAssignable, Category = "Localization|Events")
    FOnRegionChanged OnRegionChanged;

    UPROPERTY(BlueprintAssignable, Category = "Localization|Events")
    FOnLocalizationLoaded OnLocalizationLoaded;

    // 靜態訪問器
    UFUNCTION(BlueprintPure, Category = "Localization", meta = (WorldContext = "WorldContextObject"))
    static UMingLocalizationManager* GetLocalizationManager(UObject* WorldContextObject);

protected:
    // 初始化子系統
    void InitializeSubsystems();

    // 加載語言包
    bool LoadLanguagePack(EMingLanguage Language);

    // 保存當前語言設置
    void SaveLanguagePreference();

    // 加載語言偏好設置
    void LoadLanguagePreference();

    // 獲取語言文件名
    FString GetLanguageFileName(EMingLanguage Language) const;

    // 獲取語言文件路徑
    FString GetLanguageFilePath(EMingLanguage Language) const;

    // 解析語言文件
    bool ParseLanguageFile(const FString& FilePath, EMingLanguage Language);

private:
    // 初始化狀態
    UPROPERTY()
    bool bIsInitialized;

    // 當前語言
    UPROPERTY()
    EMingLanguage CurrentLanguage;

    // 當前地區
    UPROPERTY()
    EMingRegion CurrentRegion;

    // 本地化條目存儲 (Language -> (Key -> Entry))
    UPROPERTY()
    TMap<EMingLanguage, TMap<FString, FLocalizationEntry>> LocalizationData;

    // 安裝的語言包
    UPROPERTY()
    TArray<EMingLanguage> InstalledLanguages;

    // 子系統
    UPROPERTY()
    TObjectPtr<UMingLanguageSystem> LanguageSystem;

    UPROPERTY()
    TObjectPtr<UMingCultureAdapter> CultureAdapter;

    UPROPERTY()
    TObjectPtr<UMingRegionalContentManager> RegionalContentManager;

    UPROPERTY()
    TObjectPtr<UMingPersonalManager> PersonalManager;

    // 默認命名空間
    UPROPERTY()
    FString DefaultNamespace;
};
