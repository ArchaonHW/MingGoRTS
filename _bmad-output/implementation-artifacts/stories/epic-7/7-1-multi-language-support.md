# Story 7.1: 多语言支持系统

## 基本信息

**Story ID**: 7-1-multi-language-support  
**所属 Epic**: Epic 7 - 本地化与国际系统  
**优先级**: High  
**状态**: Ready for Dev  
**预计工期**: 4-5 天  
**开发者**: TBD  

## 用户故事

作为 MingGoRTS 的全球玩家，
我希望游戏支持多种语言，
以便我能够用自己的母语舒适地游玩游戏。

## 验收标准

### 功能性要求

- [ ] 系统支持至少 10 种语言的完整本地化
  - 简体中文（主语言）
  - 繁体中文
  - 英语
  - 日语
  - 韩语
  - 法语
  - 德语
  - 西班牙语
  - 俄语
  - 阿拉伯语
- [ ] 所有游戏内文本（UI、对话、提示、菜单）都可通过本地化系统翻译
- [ ] 实现 `MingRTSLocalizationSystem` 核心类
- [ ] 实现 `MingRTSTextManager` 文本管理类
- [ ] 支持文本方向（LTR/RTL）自动适配
- [ ] 支持字体资源的动态加载
- [ ] 实现本地化文本的缓存机制
- [ ] 提供 Blueprint 接口供 UI 系统使用

### 非功能性要求

- [ ] 语言切换响应时间 < 100ms
- [ ] 内存占用增量 < 50MB（10 种语言）
- [ ] 文本查找时间 < 1ms
- [ ] 支持运行时动态加载新语言包
- [ ] 代码覆盖率 ≥ 80%

## 技术规格

### 核心组件

```cpp
// 主本地化系统
UCLASS()
class MINGRTS_API UMingRTSLocalizationSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetLanguage(ELanguageCode LanguageCode);
    
    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString GetLocalizedText(const FString& Key) const;
    
    UFUNCTION(BlueprintCallable, Category = "Localization")
    ETextDirection GetTextDirection() const;
    
private:
    TMap<ELanguageCode, TSharedPtr<FLanguagePack>> LoadedLanguages;
    ELanguageCode CurrentLanguage;
};

// 文本管理器
UCLASS()
class MINGRTS_API UMingRTSTextManager : public UObject
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, Category = "Text")
    FString GetText(const FString& Namespace, const FString& Key) const;
    
    UFUNCTION(BlueprintCallable, Category = "Text")
    FString FormatText(const FString& Key, const TArray<FString>& Args) const;
};
```

### 支持的语言代码

```cpp
UENUM(BlueprintType)
enum class ELanguageCode : uint8
{
    zh_CN     UMETA(DisplayName = "简体中文"),
    zh_TW     UMETA(DisplayName = "繁体中文"),
    en_US     UMETA(DisplayName = "English"),
    ja_JP     UMETA(DisplayName = "日本語"),
    ko_KR     UMETA(DisplayName = "한국어"),
    fr_FR     UMETA(DisplayName = "Français"),
    de_DE     UMETA(DisplayName = "Deutsch"),
    es_ES     UMETA(DisplayName = "Español"),
    ru_RU     UMETA(DisplayName = "Русский"),
    ar_SA     UMETA(DisplayName = "العربية")
};
```

### 文本方向

```cpp
UENUM(BlueprintType)
enum class ETextDirection : uint8
{
    LTR       UMETA(DisplayName = "Left to Right"),
    RTL       UMETA(DisplayName = "Right to Left"),
    Auto      UMETA(DisplayName = "Auto Detect")
};
```

## 文件结构

```
Content/Localization/
├── Game/
│   ├── zh-CN/
│   │   ├── Game.locres
│   │   └── Game.locmeta
│   ├── en-US/
│   ├── ja-JP/
│   └── ... (other languages)
├── UI/
│   ├── zh-CN/
│   ├── en-US/
│   └── ...
└── Dialogue/
    ├── zh-CN/
    ├── en-US/
    └── ...
```

## 任务分解

### Day 1: 核心架构设计
- [ ] 设计 `MingRTSLocalizationSystem` 类结构
- [ ] 设计语言包加载机制
- [ ] 设计文本查找和缓存系统
- [ ] 定义 Blueprint 接口

### Day 2: 核心实现
- [ ] 实现 `MingRTSLocalizationSystem`
- [ ] 实现 `MingRTSTextManager`
- [ ] 实现语言包加载器
- [ ] 实现文本方向检测

### Day 3: UE 集成
- [ ] 集成 UE5 原生本地化系统
- [ ] 实现 FText 封装
- [ ] 实现 UMG 本地化支持
- [ ] 添加 Slate 本地化支持

### Day 4: 测试与优化
- [ ] 编写单元测试
- [ ] 性能测试（加载时间、内存占用）
- [ ] 多语言切换测试
- [ ] RTL 布局测试

### Day 5: 文档与完善
- [ ] API 文档
- [ ] 使用指南
- [ ] 代码审查
- [ ] Bug 修复

## 依赖关系

### 前置依赖
- Epic 4.1: UI 系统（UMG 和 Slate 支持）
- Epic 6.1: 跨平台基础（文件系统抽象）

### 后续依赖
- Story 7.2: 文化适应（基于多语言支持）
- Story 7.3: 地区内容差异（基于多语言支持）

## 测试策略

### 单元测试
- 文本查找准确性
- 语言切换功能
- 缓存机制有效性
- 错误处理（缺失键、损坏文件）

### 集成测试
- 与 UI 系统的集成
- 与对话系统的集成
- 多场景语言切换

### 性能测试
- 大规模文本加载（10,000+ 条）
- 快速语言切换（100 次/秒）
- 内存占用监控

## 风险与缓解

| 风险 | 可能性 | 影响 | 缓解措施 |
|------|--------|------|----------|
| RTL 布局复杂度高 | 中 | 高 | 早期原型验证，使用 UE 原生 RTL 支持 |
| 字体文件过大 | 高 | 中 | 动态加载，按需下载字体包 |
| 翻译文件管理复杂 | 中 | 中 | 建立翻译管理系统（TMS）流程 |
| 与现有 UI 集成困难 | 低 | 高 | 渐进式迁移，保持向后兼容 |

## 参考文档

- [UE5 Localization Documentation](https://docs.unrealengine.com/5.0/en-US/localization-in-unreal-engine/)
- [Internationalization Best Practices](https://docs.unrealengine.com/5.0/en-US/internationalization-support-in-unreal-engine/)
- [Text Localization in Games](https://www.gamedeveloper.com/design/localization-strategies)

## 备注

- 建议优先完成简体中文和英语，其他语言可以后续迭代添加
- RTL 语言（阿拉伯语）的实现可能需要额外的 UI 布局调整
- 考虑集成第三方翻译管理系统（如 Crowdin、Loco）以便团队协作

---

**文档信息**  
版本: 1.0  
创建日期: 2026-03-22  
最后更新: 2026-03-22  
作者: MingGoRTS 开发团队  
状态: Ready for Dev
