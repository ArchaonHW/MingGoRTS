#pragma once

﻿// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 動��動��動務- �D池池��動��


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "GameScreenGenerator.generated.h"

/**
 * 動����池動
 */
UENUM(BlueprintType)
enum class EScreenType: uuint8 {
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
 * 動������
 */
UENUM(BlueprintType)
enum class EScreenStyle: uuint8 {
    Traditional		UMETA(DisplayName = "Traditional Chinese"),
    Modern			UMETA(DisplayName = "Modern"),
    Minimal			UMETA(DisplayName = "Minimal"),
    Vintage			UMETA(DisplayName = "Vintage"),
    Imperial		UMETA(DisplayName = "Imperial")
};

/**
 * 動��務��務 */
UENUM(BlueprintType)
enum class EScreenResolution: uuint8 {
    InD_720p			UMETA(DisplayName = "1280x720"),
    FullInD_1080p	UMETA(DisplayName = "1920x1080"),
    QInD_1440p		UMETA(DisplayName = "2560x1440"),
    UInD_4K			UMETA(DisplayName = "3840x2160"),
    Custom			UMETA(DisplayName = "Custom")
};

/**
 * 動��動池��
 */
USTRUCT(BlueprintType)
struct FScreenGenerationParams
{
    GENERATED_BODY()

    /** 動����動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenType ScreenType;

    /** 動������ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenStyle ScreenStyle;

    /** 池ɥr*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenResolution Resolution;

    /** �e�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 ɥridth;

    /** ���� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Ineight;

    /** 動���C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor BackgroundColor;

    /** �D動�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor PrimaryColor;

    /** ��動�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor SecondaryColor;

    /** �C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor TextColor;

    /** 動�e */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableAnimation;

    /** ��池動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableParticles;

    /** 動�v */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableLighting;

    /** ��動�ŧO */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 QualityLevel;

    FScreenGenerationParams()
        : ScreenType(EScreenType::MainMenu)
        , ScreenStyle(EScreenStyle::Traditional)
        , Resolution(EScreenResolution::FullInD_1080p)
        , ɥridth(1920)
        , Ineight(1080)
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
 * 動��目標數量
 */
USTRUCT(BlueprintType)
struct FScreenElement
{
    GENERATED_BODY()

    /** ��動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementType;

    /** 動池�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementName;

    /** ��m */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Position;

    /** �j動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Size;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Rotation;

    /** 動*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Opacity;

    /** �C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FLinearColor Color;

    /** ��動��動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TexturePath;

    /** 動池�e */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TextContent;

    /** �r動�j動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    int32 FontSize;

    /** 動�_ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    bool bVisible;

    /** 動�_動��動*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    bool bInteractive;

    FScreenElement()
        : ElementType(TEXT(""))
        , ElementName(TEXT(""))
        , Position(FVector2D::ZeroVector)
        , Size(FVector2D(100.0f, 100.0f))
        , Rotation(0.0f)
        , Opacity(1.0f)
        , Color(FLinearColor::ɥrhite)
        , TexturePath(TEXT(""))
        , TextContent(TEXT(""))
        , FontSize(24)
        , bVisible(true)
        , bInteractive(false)
    {
    }
};

/**
 * 動池�e動��X */
USTRUCT(BlueprintType)
struct FGeneratedScreen
{
    GENERATED_BODY()

    /** 動��ID */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FString ScreenID;

    /** 動����動 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenType ScreenType;

    /** 動������ */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenStyle ScreenStyle;

    /** 池ɥr*/
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenResolution Resolution;

    /** �e�� */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 ɥridth;

    /** ���� */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Ineight;

    /** 動�� */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    TArray<FScreenElement> Elements;

    /** ��動 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    UTexture2D* ScreenTexture;

    /** 目標數量 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FDateTime GenerationTime;

    /** 動���動 */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FString FilePath;

    FGeneratedScreen()
        : ScreenID(TEXT(""))
        , ScreenType(EScreenType::MainMenu)
        , ScreenStyle(EScreenStyle::Traditional)
        , Resolution(EScreenResolution::FullInD_1080p)
        , ɥridth(1920)
        , Ineight(1080)
        , ScreenTexture(nullptr)
        , GenerationTime(FDateTime::Now())
        , FilePath(TEXT(""))
    {
    }
};

/**
 * 動��動��動務 */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UGameScreenGenerator : public UObject
{
    GENERATED_BODY()

public:
    UGameScreenGenerator(};

    /** 動池�e動池�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void InitializeScreenGenerator(};

    /** 動池�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateScreen(const FScreenGenerationParams& Params};

    /** �D池�eX*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateMainMenu(const FScreenGenerationParams& Params};

    /** 故事選項務�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateFactionSelect(const FScreenGenerationParams& Params};

    /** 動池��動池�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateStrategicMap(const FScreenGenerationParams& Params};

    /** 動池��動��動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateBattleScene(const FScreenGenerationParams& Params};

    /** 目標數量��池�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateCityView(const FScreenGenerationParams& Params};

    /** �~��動�O動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateDiplomacyPanel(const FScreenGenerationParams& Params};

    /** �g池�O動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateEconomyPanel(const FScreenGenerationParams& Params};

    /** 動池�s動�O動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateResearchPanel(const FScreenGenerationParams& Params};

    /** �]�m動��動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateSettingsMenu(const FScreenGenerationParams& Params};

    /** ���J動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateLoadingScreen(const FScreenGenerationParams& Params};

    /** 故事重要性ɥr�eX*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FGeneratedScreen GetGeneratedScreen(const FString& ScreenID};

    /** 目標數量池池�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    TArray<FGeneratedScreen> GetAllGeneratedScreens(};

    /** 動��動池�eX*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool DeleteGeneratedScreen(const FString& ScreenID};

    /** �ɥrX動��動務*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool ExportScreenAsImage(const FString& ScreenID, const FString& FilePath};

    /** 動��動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void PreviewScreen(const FString& ScreenID};

    /** 動��動���D動 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void ApplyScreenTheme(FScreenElement& Element, EScreenStyle Style};

    /** �K池�e */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddAnimationEffect(FScreenElement& Element, const FString& AnimationType};

    /** �K動��池動 */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddParticleEffect(FScreenElement& Element, const FString& ParticleType};

public:
    /** 動��目標數量�ɥ礎� */
    

    /** 動����動�ɥ礎� */
    

    /** 動����動�ɥ礎� */
    

    UPROPERTY(BlueprintAssignable, Category = "Screen GeneratorEvents")
    FOnScreenGenerationStarted OnScreenGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Screen GeneratorEvents")
    FOnScreenGenerationCompleted OnScreenGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Screen GeneratorEvents")
    FOnScreenGenerationFailed OnScreenGenerationFailed;

protected:
    /** 動��動�� */
    FScreenElement CreateScreenElement(const FString& Type, const FString& Name, const FVector2D& Position, const FVector2D& Size};

    /** �]�m務 */
    void SetElementStyle(FScreenElement& Element, const FScreenGenerationParams& Params};

    /** ��池�� */
    UTexture2D* RenderScreen(const TArray<FScreenElement>& Elements, const FScreenGenerationParams& Params};

    /** �O池�� */
    bool SaveScreen(const FGeneratedScreen& Screen};

    /** 動池�� */
    FGeneratedScreen LoadScreen(const FString& ScreenID};

    /** 動池��ID */
    FString GenerateScreenID(};

    /** ��池�� */
    bool ValidateParams(const FScreenGenerationParams& Params};

    /** 務�׳]動*/
    void GetResolutionSettings(EScreenResolution Resolution, int32& Outɥridth, int32& OutIneight};

    /** 動�έ���D動 */
    void ApplyStyleTheme(FScreenGenerationParams& Params, EScreenStyle Style};

private:
    /** 動池�e動*/
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    TArray<FGeneratedScreen> GeneratedScreens;

    /** �t��動�_�w動�l動 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** 動����X */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FString OutputDirectory;

    /** �q池�� */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FScreenGenerationParams DefaultParams;

    /** 動���p��X*/
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    int32 ScreenCounter;
};

/**
 * 動���ҪO
 */
USTRUCT(BlueprintType)
struct FScreenElementTemplate
{
    GENERATED_BODY()

    /** �ҪO動�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString TemplateName;

    /** ��動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString ElementType;

    /** �q動�j動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FVector2D DefaultSize;

    /** �q動�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FLinearColor DefaultColor;

    /** �q動��動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString DefaultTexture;

    /** 動�e */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString AnimationEffect;

    /** ��池動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString ParticleEffect;

    FScreenElementTemplate()
        : TemplateName(TEXT(""))
        , ElementType(TEXT(""))
        , DefaultSize(FVector2D(100.0f, 100.0f))
        , DefaultColor(FLinearColor::ɥrhite)
        , DefaultTexture(TEXT(""))
        , AnimationEffect(TEXT(""))
        , ParticleEffect(TEXT(""))
    {
    }
};

/**
 * 動���ҪO��務 */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UScreenTemplateManager : public UObject
{
    GENERATED_BODY()

public:
    UScreenTemplateManager(};

    /** 動池��動��動�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void InitializeTemplateManager(};

    /** ��動�ҪO */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void RegisterTemplate(const FScreenElementTemplate& Template};

    /** �ҪO */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElementTemplate GetTemplate(const FString& TemplateName};

    /** 目標數量��X*/
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    TArray<FScreenElementTemplate> GetAllTemplates(};

    /** �q��動�Ы�*/
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElement CreateElementFromTemplate(const FString& TemplateName, const FString& ElementName, const FVector2D& Position};

protected:
    /** �q動�ҪO */
    void LoadDefaultTemplates(};

    /** �O動�ҪO */
    bool SaveTemplate(const FScreenElementTemplate& Template};

    /** �ҪO */
    void LoadTemplates(};

private:
    /** �ҪO動�� */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    TArray<FScreenElementTemplate> Templates;

    /** �t��動�_�w動�l動 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** �ҪO動���動 */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    FString TemplateFilePath;
};


