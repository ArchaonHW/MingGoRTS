# MingRTSLocalizationEnhancedSystem 系統文檔 / MingRTSLocalizationEnhancedSystem System Documentation

## 概述 / Overview

MingRTSLocalizationEnhancedSystem 是一個增強型本地化和國際化系統，支持20種語言、17種地區文化適應，提供完整的翻譯管理、文化內容適應和全球化支持。

MingRTSLocalizationEnhancedSystem is an enhanced localization and internationalization system supporting 20 languages and 17 regional cultural adaptations, providing complete translation management, cultural content adaptation, and globalization support.

## 核心功能 / Core Features

### 1. 語言支持 / Language Support
支持20種語言 / Supports 20 languages：
- **English**: 英語 / English
- **SimplifiedChinese**: 簡體中文 / Simplified Chinese
- **TraditionalChinese**: 繁體中文 / Traditional Chinese
- **Japanese**: 日語 / Japanese
- **Korean**: 韓語 / Korean
- **French**: 法語 / French
- **German**: 德語 / German
- **Spanish**: 西班牙語 / Spanish
- **Italian**: 意大利語 / Italian
- **Russian**: 俄語 / Russian
- **Arabic**: 阿拉伯語（RTL）/ Arabic (RTL)
- **Portuguese**: 葡萄牙語 / Portuguese
- **Hindi**: 印地語 / Hindi
- **Thai**: 泰語 / Thai
- **Vietnamese**: 越南語 / Vietnamese
- **Indonesian**: 印尼語 / Indonesian
- **Turkish**: 土耳其語 / Turkish
- **Polish**: 波蘭語 / Polish
- **Dutch**: 荷蘭語 / Dutch
- **Custom**: 自定義 / Custom

### 2. 地區支持 / Region Support
支持17種地區 / Supports 17 regions：
- **Global**: 全球 / Global
- **Asia**: 亞洲 / Asia
- **Europe**: 歐洲 / Europe
- **Americas**: 美洲 / Americas
- **Africa**: 非洲 / Africa
- **Oceania**: 大洋洲 / Oceania
- **MiddleEast**: 中東 / Middle East
- **China**: 中國 / China
- **Japan**: 日本 / Japan
- **Korea**: 韓國 / Korea
- **SoutheastAsia**: 東南亞 / Southeast Asia
- **India**: 印度 / India
- **WesternEurope**: 西歐 / Western Europe
- **EasternEurope**: 東歐 / Eastern Europe
- **NorthAmerica**: 北美 / North America
- **SouthAmerica**: 南美 / South America
- **Custom**: 自定義 / Custom

### 3. 文本方向 / Text Directions
- **LTR**: 左到右 / Left to Right (大部分語言 / Most languages)
- **RTL**: 右到左 / Right to Left (阿拉伯語、希伯來語 / Arabic, Hebrew)
- **TTB**: 上到下 / Top to Bottom (傳統中文 / Traditional Chinese)
- **Auto**: 自動檢測 / Auto-detect
- **Custom**: 自定義 / Custom

## API 參考 / API Reference

### 系统初始化

```cpp
// 初始化本地化系统
void InitializeLocalizationSystem();

// 关闭本地化系统
void ShutdownLocalizationSystem();

// 获取系统状态
bool IsInitialized() const;
```

### 语言管理

```cpp
// 设置当前语言
void SetLanguage(ELocalizedLanguage Language);

// 获取当前语言
ELocalizedLanguage GetCurrentLanguage() const;

// 获取语言名称
FString GetLanguageName(ELocalizedLanguage Language) const;

// 获取语言代码
FString GetLanguageCode(ELocalizedLanguage Language) const;

// 获取文本方向
ETextDirection GetTextDirection(ELocalizedLanguage Language) const;

// 检查语言是否支持
bool IsLanguageSupported(ELocalizedLanguage Language) const;

// 获取支持的语言列表
TArray<ELocalizedLanguage> GetSupportedLanguages() const;
```

### 翻译功能

```cpp
// 获取本地化文本
FString GetLocalizedText(const FString& Key, const FString& Namespace = TEXT(""));

// 格式化本地化文本
FString FormatLocalizedText(const FString& Key, const TArray<FString>& Args);

// 带复数形式的翻译
FString GetLocalizedPluralText(const FString& Key, int32 Count);

// 获取本地化富文本（支持HTML标签）
FText GetLocalizedRichText(const FString& Key);

// 批量获取翻译
TMap<FString, FString> GetLocalizedTexts(const TArray<FString>& Keys);
```

### 地区适应

```cpp
// 设置当前地区
void SetRegion(ELocalizationRegion Region);

// 获取当前地区
ELocalizationRegion GetCurrentRegion() const;

// 获取地区信息
FRegionInfo GetRegionInfo(ELocalizationRegion Region) const;

// 获取地区游戏参数
FRegionalGameplayParams GetRegionalGameplayParams() const;

// 获取文化适应内容
FString GetAdaptedContent(const FString& ContentID) const;

// 检查地区节日
bool IsRegionalHoliday(const FDateTime& Date) const;

// 获取地区节日列表
TArray<FRegionalHoliday> GetRegionalHolidays() const;
```

### 格式化

```cpp
// 格式化日期
FString FormatDate(const FDateTime& Date, EDateFormat Format = EDateFormat::Auto);

// 格式化时间
FString FormatTime(const FDateTime& Time, ETimeFormat Format = ETimeFormat::Auto);

// 格式化数字
FString FormatNumber(float Number, ENumberFormat Format = ENumberFormat::Auto);

// 格式化货币
FString FormatCurrency(float Amount, ECurrencySymbol Currency = ECurrencySymbol::Auto);

// 获取日期格式字符串
FString GetDateFormatString(EDateFormat Format) const;

// 获取时间格式字符串
FString GetTimeFormatString(ETimeFormat Format) const;
```

### 翻译管理

```cpp
// 加载翻译文件
bool LoadTranslationFile(const FString& FilePath, ELocalizedLanguage Language);

// 卸载翻译文件
void UnloadTranslationFile(const FString& FilePath);

// 重新加载所有翻译
void ReloadAllTranslations();

// 添加翻译
void AddTranslation(const FString& Key, ELocalizedLanguage Language, const FString& Text);

// 更新翻译
void UpdateTranslation(const FString& Key, ELocalizedLanguage Language, const FString& Text);

// 导出翻译
bool ExportTranslations(const FString& FilePath, ELocalizedLanguage Language);

// 导入翻译
bool ImportTranslations(const FString& FilePath, ELocalizedLanguage Language);
```

### 自动翻译

```cpp
// 启用自动翻译
void EnableAutoTranslation(bool bEnable);

// 请求自动翻译
FString RequestAutoTranslation(const FString& Text, ELocalizedLanguage TargetLanguage);

// 检查翻译质量
float CheckTranslationQuality(const FString& Original, const FString& Translated);

// 提交翻译修正
void SubmitTranslationCorrection(const FString& Key, const FString& CorrectedText);
```

## 数据结构

### FRegionInfo
```cpp
struct FRegionInfo
{
    ELocalizationRegion Region;         // 地区
    FString RegionName;                 // 地区名称
    FString RegionCode;                 // 地区代码
    TArray<ELocalizedLanguage> Languages; // 常用语言
    ECurrencySymbol DefaultCurrency;    // 默认货币
    EDateFormat DefaultDateFormat;      // 默认日期格式
    ETimeFormat DefaultTimeFormat;      // 默认时间格式
    ETextDirection DefaultTextDirection; // 默认文本方向
    TMap<FString, FString> CulturalSettings; // 文化设置
};
```

### FRegionalGameplayParams
```cpp
struct FRegionalGameplayParams
{
    float DifficultyModifier;           // 难度修正
    float ResourceAbundance;            // 资源丰富度
    float PopulationDensity;            // 人口密度
    float TradeEfficiency;              // 贸易效率
    float CulturalInfluence;            // 文化影响力
    TArray<FString> UniqueUnits;        // 特色单位
    TArray<FString> UniqueBuildings;    // 特色建筑
    TArray<FString> RegionalEvents;     // 地区事件
};
```

### FTranslationEntry
```cpp
struct FTranslationEntry
{
    FString Key;                        // 键
    TMap<ELocalizedLanguage, FString> Translations; // 各语言翻译
    FString Namespace;                  // 命名空间
    FString Context;                    // 上下文
    FString SourceText;                 // 源文本
    bool bIsVerified;                   // 是否已验证
    int32 UsageCount;                   // 使用次数
};
```

### FRegionalHoliday
```cpp
struct FRegionalHoliday
{
    FString HolidayName;                // 节日名称
    FString HolidayID;                  // 节日ID
    FDateTime Date;                     // 日期
    int32 DurationDays;                 // 持续天数
    FString Description;                // 描述
    TArray<FString> Traditions;         // 传统习俗
    bool bIsPublicHoliday;              // 是否公共假期
};
```

## 事件委托

```cpp
// 语言变更
FOnLanguageChanged OnLanguageChanged;

// 地区变更
FOnRegionChanged OnRegionChanged;

// 文本方向变更
FOnTextDirectionChanged OnTextDirectionChanged;

// 翻译更新
FOnTranslationUpdated OnTranslationUpdated;
```

## 使用示例

### 初始化系统
```cpp
UMingRTSLocalizationEnhancedSystem* Localization = NewObject<UMingRTSLocalizationEnhancedSystem>();
Localization->InitializeLocalizationSystem();

// 加载翻译文件
Localization->LoadTranslationFile(TEXT("Content/Localization/zh_CN.locres"), 
    ELocalizedLanguage::SimplifiedChinese);
```

### 切换语言
```cpp
// 绑定事件
Localization->OnLanguageChanged.AddDynamic(this, &AMyGameMode::OnLanguageChanged);

// 切换语言
Localization->SetLanguage(ELocalizedLanguage::Japanese);

void AMyGameMode::OnLanguageChanged(ELocalizedLanguage NewLanguage)
{
    UE_LOG(LogTemp, Log, TEXT("Language changed to: %s"), 
        *Localization->GetLanguageName(NewLanguage));
    RefreshUI();
}
```

### 获取翻译
```cpp
// 简单翻译
FString StartText = Localization->GetLocalizedText(TEXT("UI.StartButton"));

// 带参数的翻译
TArray<FString> Args = {TEXT("100"), TEXT("Gold")};
FString ScoreText = Localization->FormatLocalizedText(TEXT("UI.ScoreDisplay"), Args);
// 结果: "You have earned 100 Gold"

// 复数翻译
FString UnitsText = Localization->GetLocalizedPluralText(TEXT("UI.Units"), 5);
// 结果根据语言复数规则可能为: "5 units" 或 "5 unit"
```

### 地区适应
```cpp
// 设置地区
Localization->SetRegion(ELocalizationRegion::China);

// 获取地区游戏参数
FRegionalGameplayParams Params = Localization->GetRegionalGameplayParams();
UE_LOG(LogTemp, Log, TEXT("Resource Abundance: %.2f"), Params.ResourceAbundance);

// 获取适应内容
FString WelcomeText = Localization->GetAdaptedContent(TEXT("WelcomeMessage"));
// 根据地区可能显示不同的欢迎消息
```

### 格式化
```cpp
// 格式化日期
FDateTime Now = FDateTime::Now();
FString DateStr = Localization->FormatDate(Now, EDateFormat::DMY);
// 结果: "23/03/2026" (欧洲格式)

// 格式化货币
FString Price = Localization->FormatCurrency(99.99f, ECurrencySymbol::USD);
// 结果: "$99.99" (美国) 或 "99,99 $" (法国)
```

### 翻译管理
```cpp
// 添加自定义翻译
Localization->AddTranslation(TEXT("Custom.Key"), ELocalizedLanguage::French, 
    TEXT("Texte personnalisé"));

// 导出翻译供翻译团队使用
Localization->ExportTranslations(TEXT("Exports/Translations_EN.csv"), 
    ELocalizedLanguage::English);

// 导入已完成的翻译
Localization->ImportTranslations(TEXT("Imports/Translations_FR.csv"), 
    ELocalizedLanguage::French);
```

## 性能考虑

- **翻译速度**: < 10ms 每次翻译
- **内存缓存**: 智能缓存常用翻译
- **延迟加载**: 非当前语言资源延迟加载
- **批量操作**: 批量翻译减少开销

## 最佳实践

1. **命名规范**: 使用点分层次命名（UI.StartButton）
2. **上下文注释**: 为翻译人员提供上下文说明
3. **复数规则**: 正确配置每种语言的复数规则
4. **RTL支持**: 为RTL语言提供UI镜像支持
5. **文化敏感**: 避免文化不敏感的内容
6. **定期更新**: 定期同步翻译文件

## 扩展性

系统支持以下扩展：
- 新语言支持
- 自定义地区设置
- 第三方翻译服务集成
- 社区翻译贡献系统
- 云端翻译管理
