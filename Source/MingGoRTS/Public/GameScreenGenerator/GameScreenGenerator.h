// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 遊戲畫面生成器 - 主界面和遊戲場景生成

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "GameScreenGenerator.generated.h"

/**
 * 畫面類型枚舉
 */
UENUM(BlueprintType)
enum class EScreenType : uint8
{
    MainMenu		UMETA(DisplayName = "Main Menu"),
    FactionSelect	UMETA(DisplayName = "Faction Selection"),
    StrategicMap		UMETA(DisplayName = "Strategic Map"),
    BattleScene		UMETA(DisplayName = "Battle Scene"),
    CityView		UMETA(DisplayName = "City View"),
    DiplomacyPanel	UMETA(DisplayName = "Diplomacy Panel"),
    EconomyPanel	UMETA(DisplayName = "Economy Panel"),
    ResearchPanel	UMETA(DisplayName = "Research Panel"),
    SettingsMenu	UMETA(DisplayName = "Settings Menu"),
    LoadingScreen	UMETA(DisplayName = "Loading Screen")
};

/**
 * 畫面風格枚舉
 */
UENUM(BlueprintType)
enum class EScreenStyle : uint8
{
    Traditional		UMETA(DisplayName = "Traditional Chinese"),
    Modern			UMETA(DisplayName = "Modern"),
    Minimal			UMETA(DisplayName = "Minimal"),
    Vintage			UMETA(DisplayName = "Vintage"),
    Imperial		UMETA(DisplayName = "Imperial")
};

/**
 * 畫面解析度枚舉
 */
UENUM(BlueprintType)
enum class EScreenResolution : uint8
{
    HD_720p			UMETA(DisplayName = "1280x720"),
    FullHD_1080p	UMETA(DisplayName = "1920x1080"),
    QHD_1440p		UMETA(DisplayName = "2560x1440"),
    UHD_4K			UMETA(DisplayName = "3840x2160"),
    Custom			UMETA(DisplayName = "Custom")
};

/**
 * 畫面生成參數
 */
USTRUCT(BlueprintType)
struct FScreenGenerationParams
{
    GENERATED_BODY()

    /** 畫面類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenType ScreenType;

    /** 畫面風格 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenStyle ScreenStyle;

    /** 解析度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenResolution Resolution;

    /** 寬度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Width;

    /** 高度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Height;

    /** 背景顏色 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor BackgroundColor;

    /** 主題顏色 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor PrimaryColor;

    /** 次要顏色 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor SecondaryColor;

    /** 文字顏色 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor TextColor;

    /** 動畫效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableAnimation;

    /** 粒子效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableParticles;

    /** 光影效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableLighting;

    /** 質量級別 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 QualityLevel;

    FScreenGenerationParams()
        : ScreenType(EScreenType::MainMenu)
        , ScreenStyle(EScreenStyle::Traditional)
        , Resolution(EScreenResolution::FullHD_1080p)
        , Width(1920)
        , Height(1080)
        , BackgroundColor(FLinearColor(0.1f, 0.05f, 0.0f, 1.0f))
        , PrimaryColor(FLinearColor(0.8f, 0.6f, 0.2f, 1.0f))
        , SecondaryColor(FLinearColor(0.6f, 0.3f, 0.1f, 1.0f))
        , TextColor(FLinearColor(0.95f, 0.9f, 0.8f, 1.0f))
        , bEnableAnimation(true)
        , bEnableParticles(true)
        , bEnableLighting(true)
        , QualityLevel(3)
    {
    }
};

/**
 * 畫面元素數據
 */
USTRUCT(BlueprintType)
struct FScreenElement
{
    GENERATED_BODY()

    /** 元素類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementType;

    /** 元素名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementName;

    /** 位置 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Position;

    /** 大小 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Size;

    /** 旋轉 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Rotation;

    /** 透明度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Opacity;

    /** 顏色 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FLinearColor Color;

    /** 紋理路徑 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TexturePath;

    /** 文字內容 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TextContent;

    /** 字體大小 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    int32 FontSize;

    /** 是否可見 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    bool bVisible;

    /** 是否可交互 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    bool bInteractive;

    FScreenElement()
        : ElementType(TEXT(""))
        , ElementName(TEXT(""))
        , Position(FVector2D::ZeroVector)
        , Size(FVector2D(100.0f, 100.0f))
        , Rotation(0.0f)
        , Opacity(1.0f)
        , Color(FLinearColor::White)
        , TexturePath(TEXT(""))
        , TextContent(TEXT(""))
        , FontSize(24)
        , bVisible(true)
        , bInteractive(false)
    {
    }
};

/**
 * 生成的畫面數據
 */
USTRUCT(BlueprintType)
struct FGeneratedScreen
{
    GENERATED_BODY()

    /** 畫面ID */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FString ScreenID;

    /** 畫面類型 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenType ScreenType;

    /** 畫面風格 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenStyle ScreenStyle;

    /** 解析度 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenResolution Resolution;

    /** 寬度 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Width;

    /** 高度 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Height;

    /** 畫面元素 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    TArray<FScreenElement> Elements;

    /** 紋理 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    UTexture2D* ScreenTexture;

    /** 生成時間 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FDateTime GenerationTime;

    /** 文件路徑 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FString FilePath;

    FGeneratedScreen()
        : ScreenID(TEXT(""))
        , ScreenType(EScreenType::MainMenu)
        , ScreenStyle(EScreenStyle::Traditional)
        , Resolution(EScreenResolution::FullHD_1080p)
        , Width(1920)
        , Height(1080)
        , ScreenTexture(nullptr)
        , GenerationTime(FDateTime::Now())
        , FilePath(TEXT(""))
    {
    }
};

/**
 * 遊戲畫面生成器
 */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UGameScreenGenerator : public UObject
{
    GENERATED_BODY()

public:
    UGameScreenGenerator();

    /** 初始化畫面生成器 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void InitializeScreenGenerator();

    /** 生成畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateScreen(const FScreenGenerationParams& Params);

    /** 生成主菜單畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateMainMenu(const FScreenGenerationParams& Params);

    /** 生成勢力選擇畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateFactionSelect(const FScreenGenerationParams& Params);

    /** 生成戰略地圖畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateStrategicMap(const FScreenGenerationParams& Params);

    /** 生成戰鬥場景畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateBattleScene(const FScreenGenerationParams& Params);

    /** 生成城市視圖畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateCityView(const FScreenGenerationParams& Params);

    /** 生成外交面板畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateDiplomacyPanel(const FScreenGenerationParams& Params);

    /** 生成經濟面板畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateEconomyPanel(const FScreenGenerationParams& Params);

    /** 生成研究面板畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateResearchPanel(const FScreenGenerationParams& Params);

    /** 生成設置菜單畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateSettingsMenu(const FScreenGenerationParams& Params);

    /** 生成載入畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateLoadingScreen(const FScreenGenerationParams& Params);

    /** 獲取生成的畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FGeneratedScreen GetGeneratedScreen(const FString& ScreenID);

    /** 獲取所有生成的畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    TArray<FGeneratedScreen> GetAllGeneratedScreens();

    /** 刪除生成的畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool DeleteGeneratedScreen(const FString& ScreenID);

    /** 導出畫面為圖片 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool ExportScreenAsImage(const FString& ScreenID, const FString& FilePath);

    /** 預覽畫面 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void PreviewScreen(const FString& ScreenID);

    /** 應用畫面主題 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void ApplyScreenTheme(FScreenElement& Element, EScreenStyle Style);

    /** 添加動畫效果 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddAnimationEffect(FScreenElement& Element, const FString& AnimationType);

    /** 添加粒子效果 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddParticleEffect(FScreenElement& Element, const FString& ParticleType);

public:
    /** 畫面生成開始事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScreenGenerationStarted, const FString&, ScreenID);

    /** 畫面生成完成事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScreenGenerationCompleted, const FString&, ScreenID, const FGeneratedScreen&, GeneratedScreen);

    /** 畫面生成失敗事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScreenGenerationFailed, const FString&, ScreenID, const FString&, ErrorMessage);

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationStarted OnScreenGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationCompleted OnScreenGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationFailed OnScreenGenerationFailed;

protected:
    /** 創建畫面元素 */
    FScreenElement CreateScreenElement(const FString& Type, const FString& Name, const FVector2D& Position, const FVector2D& Size);

    /** 設置元素樣式 */
    void SetElementStyle(FScreenElement& Element, const FScreenGenerationParams& Params);

    /** 渲染畫面 */
    UTexture2D* RenderScreen(const TArray<FScreenElement>& Elements, const FScreenGenerationParams& Params);

    /** 保存畫面 */
    bool SaveScreen(const FGeneratedScreen& Screen);

    /** 加載畫面 */
    FGeneratedScreen LoadScreen(const FString& ScreenID);

    /** 生成畫面ID */
    FString GenerateScreenID();

    /** 驗證參數 */
    bool ValidateParams(const FScreenGenerationParams& Params);

    /** 獲取解析度設置 */
    void GetResolutionSettings(EScreenResolution Resolution, int32& OutWidth, int32& OutHeight);

    /** 應用風格主題 */
    void ApplyStyleTheme(FScreenGenerationParams& Params, EScreenStyle Style);

private:
    /** 生成的畫面列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    TArray<FGeneratedScreen> GeneratedScreens;

    /** 系統是否已初始化 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** 畫面輸出目錄 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FString OutputDirectory;

    /** 默認參數 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FScreenGenerationParams DefaultParams;

    /** 畫面計數器 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    int32 ScreenCounter;
};

/**
 * 畫面元素模板
 */
USTRUCT(BlueprintType)
struct FScreenElementTemplate
{
    GENERATED_BODY()

    /** 模板名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString TemplateName;

    /** 元素類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString ElementType;

    /** 默認大小 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FVector2D DefaultSize;

    /** 默認顏色 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FLinearColor DefaultColor;

    /** 默認紋理 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString DefaultTexture;

    /** 動畫效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString AnimationEffect;

    /** 粒子效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString ParticleEffect;

    FScreenElementTemplate()
        : TemplateName(TEXT(""))
        , ElementType(TEXT(""))
        , DefaultSize(FVector2D(100.0f, 100.0f))
        , DefaultColor(FLinearColor::White)
        , DefaultTexture(TEXT(""))
        , AnimationEffect(TEXT(""))
        , ParticleEffect(TEXT(""))
    {
    }
};

/**
 * 畫面模板管理器
 */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UScreenTemplateManager : public UObject
{
    GENERATED_BODY()

public:
    UScreenTemplateManager();

    /** 初始化模板管理器 */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void InitializeTemplateManager();

    /** 註冊模板 */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void RegisterTemplate(const FScreenElementTemplate& Template);

    /** 獲取模板 */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElementTemplate GetTemplate(const FString& TemplateName);

    /** 獲取所有模板 */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    TArray<FScreenElementTemplate> GetAllTemplates();

    /** 從模板創建元素 */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElement CreateElementFromTemplate(const FString& TemplateName, const FString& ElementName, const FVector2D& Position);

protected:
    /** 加載默認模板 */
    void LoadDefaultTemplates();

    /** 保存模板 */
    bool SaveTemplate(const FScreenElementTemplate& Template);

    /** 加載模板 */
    void LoadTemplates();

private:
    /** 模板列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    TArray<FScreenElementTemplate> Templates;

    /** 系統是否已初始化 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** 模板文件路徑 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    FString TemplateFilePath;
};
