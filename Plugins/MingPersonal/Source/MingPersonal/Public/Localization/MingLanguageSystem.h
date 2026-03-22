#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLanguageSystem.generated.h"

// 支援的語言
UENUM(BlueprintType)
enum class EMingSupportedLanguage: uint8 {
    English,        // 英文
    TraditionalChinese, // 繁體中文
    SimplifiedChinese,  // 簡體中文
    Japanese,       // 日文
    Korean,         // 韓文
    French,         // 法文
    German,         // 德文
    Spanish,        // 西班牙文
    Russian         // 俄文
};

// 語言資料類型
UENUM(BlueprintType)
enum class ELanguageDataType: uint8 {
    Text,           // 文字
    Audio,          // 音頻
    Subtitle,       // 字幕
    UI,             // 用戶界面
    Tutorial        // 教程
};

// 本地化資料
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FLocalizationData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Key;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingSupportedLanguage, FString> Translations;

    UPROPERTY(BlueprintReadOnly)
    ELanguageDataType DataType;

    UPROPERTY(BlueprintReadOnly)
    FString Context;
};

/**
 * 語言系統
 * 負責多語言支援和本地化
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingLanguageSystem : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingLanguageSystem();

    // 初始化語言系統
    UFUNCTION(BlueprintCallable, Category = "Language")
    void InitializeLanguageSystem();

    // 設置當前語言
    UFUNCTION(BlueprintCallable, Category = "Language")
    void SetCurrentLanguage(EMingSupportedLanguage Language);

    // 獲取當前語言
    UFUNCTION(BlueprintCallable, Category = "Language")
    EMingSupportedLanguage GetCurrentLanguage() const;

    // 獲取本地化文字
    UFUNCTION(BlueprintCallable, Category = "Language")
    FString GetLocalizedText(const FString& Key) const;

    // 獲取特定語言的文字
    UFUNCTION(BlueprintCallable, Category = "Language")
    FString GetTextForLanguage(const FString& Key, EMingSupportedLanguage Language) const;

    // 添加本地化資料
    UFUNCTION(BlueprintCallable, Category = "Language")
    void AddLocalizationData(const FLocalizationData& Data);

    // 載入語言包
    UFUNCTION(BlueprintCallable, Category = "Language")
    bool LoadLanguagePack(EMingSupportedLanguage Language);

    // 保存語言包
    UFUNCTION(BlueprintCallable, Category = "Language")
    bool SaveLanguagePack(EMingSupportedLanguage Language);

    // 簡繁轉換
    UFUNCTION(BlueprintCallable, Category = "Language")
    FString ConvertSimplifiedToTraditional(const FString& SimplifiedText) const;

    UFUNCTION(BlueprintCallable, Category = "Language")
    FString ConvertTraditionalToSimplified(const FString& TraditionalText) const;

    // 獲取支援的語言列表
    UFUNCTION(BlueprintCallable, Category = "Language")
    TArray<EMingSupportedLanguage> GetSupportedLanguages() const;

    // 檢查語言是否支援
    UFUNCTION(BlueprintCallable, Category = "Language")
    bool IsLanguageSupported(EMingSupportedLanguage Language) const;

    // 獲取語言顯示名稱
    UFUNCTION(BlueprintCallable, Category = "Language")
    FString GetLanguageDisplayName(EMingSupportedLanguage Language) const;

    // 自動檢測系統語言
    UFUNCTION(BlueprintCallable, Category = "Language")
    EMingSupportedLanguage DetectSystemLanguage() const;

protected:
    // 當前語言
    UPROPERTY(BlueprintReadOnly)
    EMingSupportedLanguage CurrentLanguage;

    // 本地化資料庫
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FLocalizationData> LocalizationDatabase;

    // 支援的語言列表
    UPROPERTY(BlueprintReadOnly)
    TArray<EMingSupportedLanguage> SupportedLanguages;

    // 語言包路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Language|Settings")
    FString LanguagePackPath;

    // 自動檢測語言
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Language|Settings")
    bool bAutoDetectLanguage;

    // 初始化語言資料
    void InitializeLanguageData();

    // 載入簡繁轉換對照表
    void LoadChineseConversionTables();

    // 簡繁轉換對照表
    TMap<TCHAR, TCHAR> SimplifiedToTraditionalMap;
    TMap<TCHAR, TCHAR> TraditionalToSimplifiedMap;
};
