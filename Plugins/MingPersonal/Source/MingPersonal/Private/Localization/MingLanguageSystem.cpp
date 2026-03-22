#include "Localization/MingLanguageSystem.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"

UMingLanguageSystem::UMingLanguageSystem()
    : bInitialized(false)
{
}

void UMingLanguageSystem::InitializeLanguageSystem()
{
    if (bInitialized)
    {
        return;
    }

    // 初始化語言檢測數據
    InitializeLanguageDetection();

    // 加載翻譯記憶庫
    LoadTranslationMemory();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("MingLanguageSystem initialized"));
}

FString UMingLanguageSystem::DetectLanguage(const FString& Text)
{
    // 簡單的語言檢測邏輯
    // 檢查字符範圍來判斷語言

    bool bHasChinese = false;
    bool bHasJapanese = false;
    bool bHasKorean = false;
    bool bHasArabic = false;
    bool bHasHebrew = false;
    bool bHasCyrillic = false;

    for (const TCHAR& Char : Text)
    {
        // 中日韓統一表意文字
        if (Char >= 0x4E00 && Char <= 0x9FFF)
        {
            bHasChinese = true;
        }
        // 平假名
        else if (Char >= 0x3040 && Char <= 0x309F)
        {
            bHasJapanese = true;
        }
        // 片假名
        else if (Char >= 0x30A0 && Char <= 0x30FF)
        {
            bHasJapanese = true;
        }
        // 韓文
        else if (Char >= 0xAC00 && Char <= 0xD7AF)
        {
            bHasKorean = true;
        }
        // 阿拉伯文
        else if (Char >= 0x0600 && Char <= 0x06FF)
        {
            bHasArabic = true;
        }
        // 希伯來文
        else if (Char >= 0x0590 && Char <= 0x05FF)
        {
            bHasHebrew = true;
        }
        // 西里爾字母
        else if (Char >= 0x0400 && Char <= 0x04FF)
        {
            bHasCyrillic = true;
        }
    }

    if (bHasArabic) return TEXT("ar");
    if (bHasHebrew) return TEXT("he");
    if (bHasKorean) return TEXT("ko");
    if (bHasJapanese) return TEXT("ja");
    if (bHasCyrillic) return TEXT("ru");
    if (bHasChinese) return TEXT("zh");

    // 默認為英文
    return TEXT("en");
}

FString UMingLanguageSystem::TranslateText(const FString& Text, const FString& SourceLanguage, const FString& TargetLanguage)
{
    // 構建翻譯鍵
    FString TranslationKey = FString::Printf(TEXT("%s|%s|%s"), *SourceLanguage, *TargetLanguage, *Text);

    // 檢查翻譯記憶庫
    if (FString* CachedTranslation = TranslationMemory.Find(TranslationKey))
    {
        return *CachedTranslation;
    }

    // 如果沒有緩存，返回原文（實際實現會調用翻譯API）
    // 這裡是一個占位符實現
    UE_LOG(LogTemp, Warning, TEXT("Translation not found in memory: %s"), *TranslationKey);

    return Text;
}

bool UMingLanguageSystem::IsRightToLeftLanguage(const FString& LanguageCode)
{
    // 常見的RTL語言
    static const TArray<FString> RTLLanguages = {
        TEXT("ar"),  // 阿拉伯語
        TEXT("he"),  // 希伯來語
        TEXT("fa"),  // 波斯語
        TEXT("ur"),  // 烏爾都語
    };

    return RTLLanguages.Contains(LanguageCode);
}

FString UMingLanguageSystem::ConvertToSimplifiedChinese(const FString& Text)
{
    // 簡化字轉換邏輯
    // 實際實現需要使用繁簡轉換庫
    // 這裡是簡單的占位符實現
    FString Result = Text;

    // 一些常見的繁簡對應
    static const TMap<FString, FString> TraditionalToSimplified = {
        {TEXT("\u570b"), TEXT("\u56fd")},  // 國 -> 国
        {TEXT("\u8a9e"), TEXT("\u8bed")},  // 語 -> 语
        {TEXT("\u570b"), TEXT("\u56fd")},  // 國 -> 国
        {TEXT("\u9577"), TEXT("\u957f")},  // 長 -> 长
        {TEXT("\u7c21"), TEXT("\u7b80")},  // 簡 -> 简
        {TEXT("\u9ad4"), TEXT("\u4f53")},  // 體 -> 体
    };

    for (const auto& Pair : TraditionalToSimplified)
    {
        Result = Result.Replace(*Pair.Key, *Pair.Value);
    }

    return Result;
}

FString UMingLanguageSystem::ConvertToTraditionalChinese(const FString& Text)
{
    // 繁體字轉換邏輯
    // 實際實現需要使用繁簡轉換庫
    // 這裡是簡單的占位符實現
    FString Result = Text;

    // 一些常見的簡繁對應（反向）
    static const TMap<FString, FString> SimplifiedToTraditional = {
        {TEXT("\u56fd"), TEXT("\u570b")},  // 国 -> 國
        {TEXT("\u8bed"), TEXT("\u8a9e")},  // 语 -> 語
        {TEXT("\u957f"), TEXT("\u9577")},  // 长 -> 長
        {TEXT("\u7b80"), TEXT("\u7c21")},  // 简 -> 簡
        {TEXT("\u4f53"), TEXT("\u9ad4")},  // 体 -> 體
    };

    for (const auto& Pair : SimplifiedToTraditional)
    {
        Result = Result.Replace(*Pair.Key, *Pair.Value);
    }

    return Result;
}

FString UMingLanguageSystem::GetPinyin(const FString& ChineseText)
{
    // 拼音獲取邏輯
    // 實際實現需要使用拼音庫
    // 這裡返回占位符
    return FString::Printf(TEXT("pinyin(%s)"), *ChineseText.Left(10));
}

int32 UMingLanguageSystem::GetStrokeCount(const TCHAR ChineseChar)
{
    // 筆畫數獲取邏輯
    // 實際實現需要使用筆畫數據庫
    // 這裡返回占位符
    // 常用字的筆畫數（簡化實現）
    static const TMap<TCHAR, int32> CommonStrokeCounts = {
        {TEXT('\u4e00'), 1},  // 一
        {TEXT('\u4e8c'), 2},  // 二
        {TEXT('\u4e09'), 3},  // 三
        {TEXT('\u56db'), 5},  // 四
        {TEXT('\u4e94'), 4},  // 五
        {TEXT('\u516d'), 4},  // 六
        {TEXT('\u4e03'), 2},  // 七
        {TEXT('\u516b'), 2},  // 八
        {TEXT('\u4e5d'), 2},  // 九
        {TEXT('\u5341'), 2},  // 十
    };

    if (const int32* Count = CommonStrokeCounts.Find(ChineseChar))
    {
        return *Count;
    }

    return 0;  // 未知
}

int32 UMingLanguageSystem::GetCharacterCount(const FString& Text)
{
    // 計算字符數（考慮多字節字符）
    return Text.Len();
}

int32 UMingLanguageSystem::GetWordCount(const FString& Text)
{
    // 計算詞數
    // 對於中文，簡單地按字符數估算
    // 對於英文等語言，按空格分割
    FString DetectedLang = DetectLanguage(Text);

    if (DetectedLang == TEXT("zh") || DetectedLang == TEXT("ja") || DetectedLang == TEXT("ko"))
    {
        // 中日韓語言：詞數約為字符數的一半（粗略估算）
        return FMath::Max(1, Text.Len() / 2);
    }
    else
    {
        // 其他語言：按空格分割
        TArray<FString> Words;
        Text.ParseIntoArray(Words, TEXT(" "), true);
        return Words.Num();
    }
}

bool UMingLanguageSystem::IsChineseCharacter(const TCHAR Char)
{
    // 檢查是否為中日韓統一表意文字
    return (Char >= 0x4E00 && Char <= 0x9FFF) ||
           (Char >= 0x3400 && Char <= 0x4DBF) ||
           (Char >= 0x20000 && Char <= 0x2A6DF);
}

bool UMingLanguageSystem::IsFullWidthCharacter(const TCHAR Char)
{
    // 檢查是否為全角字符
    return (Char >= 0xFF01 && Char <= 0xFF5E) ||  // 全角ASCII
           (Char >= 0xFFE0 && Char <= 0xFFE6) ||  // 全角符號
           (Char >= 0x3000 && Char <= 0x303F) ||  // 中日韓符號和標點
           IsChineseCharacter(Char);
}

ETranslationQuality UMingLanguageSystem::ValidateTranslation(const FString& SourceText, const FString& TranslatedText)
{
    // 簡單的翻譯質量驗證
    // 檢查長度比例
    float LengthRatio = static_cast<float>(TranslatedText.Len()) / FMath::Max(1, SourceText.Len());

    // 檢查是否為空
    if (TranslatedText.IsEmpty())
    {
        return ETranslationQuality::Draft;
    }

    // 檢查長度是否合理
    if (LengthRatio < 0.1f || LengthRatio > 5.0f)
    {
        return ETranslationQuality::Draft;
    }

    // 檢查是否包含明顯的錯誤標記
    if (TranslatedText.Contains(TEXT("[Untranslated]")) ||
        TranslatedText.Contains(TEXT("ERROR")))
    {
        return ETranslationQuality::Draft;
    }

    // 簡單的質量評估
    if (LengthRatio >= 0.5f && LengthRatio <= 2.0f)
    {
        return ETranslationQuality::Standard;
    }

    return ETranslationQuality::Draft;
}

float UMingLanguageSystem::GetTranslationCoverage() const
{
    // 計算翻譯覆蓋率
    // 這裡是占位符實現
    return 0.0f;
}

int32 UMingLanguageSystem::GetMissingTranslationCount() const
{
    // 獲取缺失翻譯的數量
    // 這裡是占位符實現
    return 0;
}

FString UMingLanguageSystem::GetLastError() const
{
    return LastErrorMessage;
}

void UMingLanguageSystem::InitializeLanguageDetection()
{
    // 初始化語言檢測數據
    // 實際實現會加載語言模型或字典
    UE_LOG(LogTemp, Log, TEXT("Language detection initialized"));
}

void UMingLanguageSystem::LoadTranslationMemory()
{
    // 加載翻譯記憶庫
    // 實際實現會從文件或數據庫加載
    UE_LOG(LogTemp, Log, TEXT("Translation memory loaded"));
}

void UMingLanguageSystem::SaveTranslationMemory()
{
    // 保存翻譯記憶庫
    // 實際實現會保存到文件或數據庫
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("TranslationMemory.json");

    // 轉換為JSON
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    for (const auto& Pair : TranslationMemory)
    {
        JsonObject->SetStringField(Pair.Key, Pair.Value);
    }

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FFileHelper::SaveStringToFile(JsonString, *SavePath);

    UE_LOG(LogTemp, Log, TEXT("Translation memory saved to: %s"), *SavePath);
}
