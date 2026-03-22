// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ?Šæˆ²?«é¢?Ÿæ???- ä¸»ç??¢å??Šæˆ²?´æ™¯?Ÿæ?

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "GameScreenGenerator.generated.h"

/**
 * ?«é¢é¡å??šè?
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
 * ?«é¢é¢¨æ ¼?šè?
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
 * ?«é¢è§??åº¦æ??? */
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
 * ?«é¢?Ÿæ??ƒæ•¸
 */
USTRUCT(BlueprintType)
struct FScreenGenerationParams
{
    GENERATED_BODY()

    /** ?«é¢é¡å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenType ScreenType;

    /** ?«é¢é¢¨æ ¼ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenStyle ScreenStyle;

    /** è§??åº?*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenResolution Resolution;

    /** å¯¬åº¦ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Width;

    /** é«˜åº¦ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Height;

    /** ?Œæ™¯é¡è‰² */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor BackgroundColor;

    /** ä¸»é?é¡è‰² */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor PrimaryColor;

    /** æ¬¡è?é¡è‰² */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor SecondaryColor;

    /** ?‡å?é¡è‰² */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor TextColor;

    /** ?•ç•«?ˆæ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableAnimation;

    /** ç²’å??ˆæ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableParticles;

    /** ?‰å½±?ˆæ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableLighting;

    /** è³ªé?ç´šåˆ¥ */
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
 * ?«é¢?ƒç??¸æ?
 */
USTRUCT(BlueprintType)
struct FScreenElement
{
    GENERATED_BODY()

    /** ?ƒç?é¡å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementType;

    /** ?ƒç??ç¨± */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementName;

    /** ä½ç½® */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Position;

    /** å¤§å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Size;

    /** ?‹è? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Rotation;

    /** ?æ?åº?*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Opacity;

    /** é¡è‰² */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FLinearColor Color;

    /** ç´‹ç?è·¯å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TexturePath;

    /** ?‡å??§å®¹ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TextContent;

    /** å­—é?å¤§å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    int32 FontSize;

    /** ?¯å¦?¯è? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    bool bVisible;

    /** ?¯å¦?¯äº¤äº?*/
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
 * ?Ÿæ??„ç•«?¢æ•¸?? */
USTRUCT(BlueprintType)
struct FGeneratedScreen
{
    GENERATED_BODY()

    /** ?«é¢ID */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FString ScreenID;

    /** ?«é¢é¡å? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenType ScreenType;

    /** ?«é¢é¢¨æ ¼ */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenStyle ScreenStyle;

    /** è§??åº?*/
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenResolution Resolution;

    /** å¯¬åº¦ */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Width;

    /** é«˜åº¦ */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Height;

    /** ?«é¢?ƒç? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    TArray<FScreenElement> Elements;

    /** ç´‹ç? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    UTexture2D* ScreenTexture;

    /** ?Ÿæ??‚é? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FDateTime GenerationTime;

    /** ?‡ä»¶è·¯å? */
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
 * ?Šæˆ²?«é¢?Ÿæ??? */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UGameScreenGenerator : public UObject
{
    GENERATED_BODY()

public:
    UGameScreenGenerator(};

    /** ?å??–ç•«?¢ç??å™¨ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void InitializeScreenGenerator(};

    /** ?Ÿæ??«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateScreen(const FScreenGenerationParams& Params};

    /** ?Ÿæ?ä¸»è??®ç•«??*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateMainMenu(const FScreenGenerationParams& Params};

    /** ?Ÿæ??¢å??¸æ??«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateFactionSelect(const FScreenGenerationParams& Params};

    /** ?Ÿæ??°ç•¥?°å??«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateStrategicMap(const FScreenGenerationParams& Params};

    /** ?Ÿæ??°é¬¥?´æ™¯?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateBattleScene(const FScreenGenerationParams& Params};

    /** ?Ÿæ??å?è¦–å??«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateCityView(const FScreenGenerationParams& Params};

    /** ?Ÿæ?å¤–äº¤?¢æ¿?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateDiplomacyPanel(const FScreenGenerationParams& Params};

    /** ?Ÿæ?ç¶“æ??¢æ¿?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateEconomyPanel(const FScreenGenerationParams& Params};

    /** ?Ÿæ??”ç©¶?¢æ¿?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateResearchPanel(const FScreenGenerationParams& Params};

    /** ?Ÿæ?è¨­ç½®?œå–®?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateSettingsMenu(const FScreenGenerationParams& Params};

    /** ?Ÿæ?è¼‰å…¥?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateLoadingScreen(const FScreenGenerationParams& Params};

    /** ?²å??Ÿæ??„ç•«??*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FGeneratedScreen GetGeneratedScreen(const FString& ScreenID};

    /** ?²å??€?‰ç??ç??«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    TArray<FGeneratedScreen> GetAllGeneratedScreens(};

    /** ?ªé™¤?Ÿæ??„ç•«??*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool DeleteGeneratedScreen(const FString& ScreenID};

    /** å°å‡º?«é¢?ºå???*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool ExportScreenAsImage(const FString& ScreenID, const FString& FilePath};

    /** ?è¦½?«é¢ */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void PreviewScreen(const FString& ScreenID};

    /** ?‰ç”¨?«é¢ä¸»é? */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void ApplyScreenTheme(FScreenElement& Element, EScreenStyle Style};

    /** æ·»å??•ç•«?ˆæ? */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddAnimationEffect(FScreenElement& Element, const FString& AnimationType};

    /** æ·»å?ç²’å??ˆæ? */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddParticleEffect(FScreenElement& Element, const FString& ParticleType};

public:
    /** ?«é¢?Ÿæ??‹å?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScreenGenerationStarted, const FString&, ScreenID};

    /** ?«é¢?Ÿæ?å®Œæ?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScreenGenerationCompleted, const FString&, ScreenID, const FGeneratedScreen&, GeneratedScreen};

    /** ?«é¢?Ÿæ?å¤±æ?äº‹ä»¶ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScreenGenerationFailed, const FString&, ScreenID, const FString&, ErrorMessage};

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationStarted OnScreenGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationCompleted OnScreenGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationFailed OnScreenGenerationFailed;

protected:
    /** ?µå»º?«é¢?ƒç? */
    FScreenElement CreateScreenElement(const FString& Type, const FString& Name, const FVector2D& Position, const FVector2D& Size};

    /** è¨­ç½®?ƒç?æ¨?? */
    void SetElementStyle(FScreenElement& Element, const FScreenGenerationParams& Params};

    /** æ¸²æ??«é¢ */
    UTexture2D* RenderScreen(const TArray<FScreenElement>& Elements, const FScreenGenerationParams& Params};

    /** ä¿å??«é¢ */
    bool SaveScreen(const FGeneratedScreen& Screen};

    /** ? è??«é¢ */
    FGeneratedScreen LoadScreen(const FString& ScreenID};

    /** ?Ÿæ??«é¢ID */
    FString GenerateScreenID(};

    /** é©—è??ƒæ•¸ */
    bool ValidateParams(const FScreenGenerationParams& Params};

    /** ?²å?è§??åº¦è¨­ç½?*/
    void GetResolutionSettings(EScreenResolution Resolution, int32& OutWidth, int32& OutHeight};

    /** ?‰ç”¨é¢¨æ ¼ä¸»é? */
    void ApplyStyleTheme(FScreenGenerationParams& Params, EScreenStyle Style};

private:
    /** ?Ÿæ??„ç•«?¢å?è¡?*/
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    TArray<FGeneratedScreen> GeneratedScreens;

    /** ç³»çµ±?¯å¦å·²å?å§‹å? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ?«é¢è¼¸å‡º?®é? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FString OutputDirectory;

    /** é»˜è??ƒæ•¸ */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FScreenGenerationParams DefaultParams;

    /** ?«é¢è¨ˆæ•¸??*/
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    int32 ScreenCounter;
};

/**
 * ?«é¢?ƒç?æ¨¡æ¿
 */
USTRUCT(BlueprintType)
struct FScreenElementTemplate
{
    GENERATED_BODY()

    /** æ¨¡æ¿?ç¨± */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString TemplateName;

    /** ?ƒç?é¡å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString ElementType;

    /** é»˜è?å¤§å? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FVector2D DefaultSize;

    /** é»˜è?é¡è‰² */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FLinearColor DefaultColor;

    /** é»˜è?ç´‹ç? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString DefaultTexture;

    /** ?•ç•«?ˆæ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString AnimationEffect;

    /** ç²’å??ˆæ? */
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
 * ?«é¢æ¨¡æ¿ç®¡ç??? */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UScreenTemplateManager : public UObject
{
    GENERATED_BODY()

public:
    UScreenTemplateManager(};

    /** ?å??–æ¨¡?¿ç®¡?†å™¨ */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void InitializeTemplateManager(};

    /** è¨»å?æ¨¡æ¿ */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void RegisterTemplate(const FScreenElementTemplate& Template};

    /** ?²å?æ¨¡æ¿ */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElementTemplate GetTemplate(const FString& TemplateName};

    /** ?²å??€?‰æ¨¡??*/
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    TArray<FScreenElementTemplate> GetAllTemplates(};

    /** å¾æ¨¡?¿å‰µå»ºå?ç´?*/
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElement CreateElementFromTemplate(const FString& TemplateName, const FString& ElementName, const FVector2D& Position};

protected:
    /** ? è?é»˜è?æ¨¡æ¿ */
    void LoadDefaultTemplates(};

    /** ä¿å?æ¨¡æ¿ */
    bool SaveTemplate(const FScreenElementTemplate& Template};

    /** ? è?æ¨¡æ¿ */
    void LoadTemplates(};

private:
    /** æ¨¡æ¿?—è¡¨ */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    TArray<FScreenElementTemplate> Templates;

    /** ç³»çµ±?¯å¦å·²å?å§‹å? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** æ¨¡æ¿?‡ä»¶è·¯å? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    FString TemplateFilePath;
};

