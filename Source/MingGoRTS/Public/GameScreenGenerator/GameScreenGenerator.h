// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ??��??��???X- �D?X??X?��??��????

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "GameScreenGenerator.generated.h"

/**
 * ??����?X???
 */
UENUM(BlueprintType)
enum class EScreenType: uint8 {
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
 * ??������????
 */
UENUM(BlueprintType)
enum class EScreenStyle: uint8 {
    Traditional		UMETA(DisplayName = "Traditional Chinese"),
    Modern			UMETA(DisplayName = "Modern"),
    Minimal			UMETA(DisplayName = "Minimal"),
    Vintage			UMETA(DisplayName = "Vintage"),
    Imperial		UMETA(DisplayName = "Imperial")
};

/**
 * ??��?X��?X */
UENUM(BlueprintType)
enum class EScreenResolution: uint8 {
    HD_720p			UMETA(DisplayName = "1280x720"),
    FullHD_1080p	UMETA(DisplayName = "1920x1080"),
    QHD_1440p		UMETA(DisplayName = "2560x1440"),
    UHD_4K			UMETA(DisplayName = "3840x2160"),
    Custom			UMETA(DisplayName = "Custom")
};

/**
 * ??��???X?��
 */
USTRUCT(BlueprintType)
struct FScreenGenerationParams
{
    GENERATED_BODY()

    /** ??����?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenType ScreenType;

    /** ??������ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenStyle ScreenStyle;

    /** ?X??*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    EScreenResolution Resolution;

    /** �e�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Width;

    /** ���� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    int32 Height;

    /** ??���C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor BackgroundColor;

    /** �D??�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor PrimaryColor;

    /** ��??�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor SecondaryColor;

    /** ????�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    FLinearColor TextColor;

    /** ??�e???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableAnimation;

    /** ��?X??? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableParticles;

    /** ??�v???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Generation")
    bool bEnableLighting;

    /** ��??�ŧO */
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
 * ??��???X???
 */
USTRUCT(BlueprintType)
struct FScreenElement
{
    GENERATED_BODY()

    /** ????��?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementType;

    /** ???X?�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString ElementName;

    /** ��m */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Position;

    /** �j?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FVector2D Size;

    /** ???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Rotation;

    /** ??????*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    float Opacity;

    /** �C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FLinearColor Color;

    /** ��??��?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TexturePath;

    /** ???X?�e */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    FString TextContent;

    /** �r??�j?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    int32 FontSize;

    /** ??�_???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Element")
    bool bVisible;

    /** ??�_??��??*/
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
 * ???X?�e??��X */
USTRUCT(BlueprintType)
struct FGeneratedScreen
{
    GENERATED_BODY()

    /** ??��ID */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FString ScreenID;

    /** ??����?? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenType ScreenType;

    /** ??������ */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenStyle ScreenStyle;

    /** ?X??*/
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    EScreenResolution Resolution;

    /** �e�� */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Width;

    /** ���� */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    int32 Height;

    /** ??��???? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    TArray<FScreenElement> Elements;

    /** ��?? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    UTexture2D* ScreenTexture;

    /** ???X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Generated Screen")
    FDateTime GenerationTime;

    /** ??���?? */
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
 * ??��??��???X */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UGameScreenGenerator : public UObject
{
    GENERATED_BODY()

public:
    UGameScreenGenerator();

    /** ???X?�e???X?�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void InitializeScreenGenerator();

    /** ???X?�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateScreen(const FScreenGenerationParams& Params);

    /** ????�D?X?�eX*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateMainMenu(const FScreenGenerationParams& Params);

    /** ???X??X??X?�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateFactionSelect(const FScreenGenerationParams& Params);

    /** ???X?��???X?�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateStrategicMap(const FScreenGenerationParams& Params);

    /** ???X?��??��??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateBattleScene(const FScreenGenerationParams& Params);

    /** ???X???��?X?�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateCityView(const FScreenGenerationParams& Params);

    /** ????�~��??�O??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateDiplomacyPanel(const FScreenGenerationParams& Params);

    /** ????�g?X?�O??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateEconomyPanel(const FScreenGenerationParams& Params);

    /** ???X?�s??�O??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateResearchPanel(const FScreenGenerationParams& Params);

    /** ????�]�m??��??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateSettingsMenu(const FScreenGenerationParams& Params);

    /** ????���J??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FString GenerateLoadingScreen(const FScreenGenerationParams& Params);

    /** ???X??X?�eX*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    FGeneratedScreen GetGeneratedScreen(const FString& ScreenID);

    /** ???X????X??X?�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    TArray<FGeneratedScreen> GetAllGeneratedScreens();

    /** ??��???X?�eX*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool DeleteGeneratedScreen(const FString& ScreenID);

    /** �ɥX??��???X*/
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    bool ExportScreenAsImage(const FString& ScreenID, const FString& FilePath);

    /** ??��??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void PreviewScreen(const FString& ScreenID);

    /** ??��??���D?? */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void ApplyScreenTheme(FScreenElement& Element, EScreenStyle Style);

    /** �K?X?�e???? */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddAnimationEffect(FScreenElement& Element, const FString& AnimationType);

    /** �K??��?X??? */
    UFUNCTION(BlueprintCallable, Category = "Screen Generator")
    void AddParticleEffect(FScreenElement& Element, const FString& ParticleType);

public:
    /** ??��???X???�ƥ� */
    

    /** ??��????��??�ƥ� */
    

    /** ??��????��??�ƥ� */
    

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationStarted OnScreenGenerationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationCompleted OnScreenGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Screen Generator|Events")
    FOnScreenGenerationFailed OnScreenGenerationFailed;

protected:
    /** ??��??��???? */
    FScreenElement CreateScreenElement(const FString& Type, const FString& Name, const FVector2D& Position, const FVector2D& Size);

    /** �]�m?????X */
    void SetElementStyle(FScreenElement& Element, const FScreenGenerationParams& Params);

    /** ��?X?�� */
    UTexture2D* RenderScreen(const TArray<FScreenElement>& Elements, const FScreenGenerationParams& Params);

    /** �O?X?�� */
    bool SaveScreen(const FGeneratedScreen& Screen);

    /** ???X?�� */
    FGeneratedScreen LoadScreen(const FString& ScreenID);

    /** ???X?��ID */
    FString GenerateScreenID();

    /** ��?X?�� */
    bool ValidateParams(const FScreenGenerationParams& Params);

    /** ?????X�׳]??*/
    void GetResolutionSettings(EScreenResolution Resolution, int32& OutWidth, int32& OutHeight);

    /** ??�έ���D?? */
    void ApplyStyleTheme(FScreenGenerationParams& Params, EScreenStyle Style);

private:
    /** ???X?�e??????*/
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    TArray<FGeneratedScreen> GeneratedScreens;

    /** �t��??�_�w??�l?? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ??����X???? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FString OutputDirectory;

    /** �q?X?�� */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    FScreenGenerationParams DefaultParams;

    /** ??���p��X*/
    UPROPERTY(BlueprintReadOnly, Category = "Screen Generator", meta = (AllowPrivateAccess = "true"))
    int32 ScreenCounter;
};

/**
 * ??��????�ҪO
 */
USTRUCT(BlueprintType)
struct FScreenElementTemplate
{
    GENERATED_BODY()

    /** �ҪO??�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString TemplateName;

    /** ????��?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString ElementType;

    /** �q??�j?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FVector2D DefaultSize;

    /** �q??�C�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FLinearColor DefaultColor;

    /** �q??��?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString DefaultTexture;

    /** ??�e???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen Template")
    FString AnimationEffect;

    /** ��?X??? */
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
 * ??���ҪO��?X */
UCLASS(BlueprintType, Blueprintable)
class GAMESCREENGENERATOR_API UScreenTemplateManager : public UObject
{
    GENERATED_BODY()

public:
    UScreenTemplateManager();

    /** ???X?��??��??�� */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void InitializeTemplateManager();

    /** ��??�ҪO */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    void RegisterTemplate(const FScreenElementTemplate& Template);

    /** ????�ҪO */
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElementTemplate GetTemplate(const FString& TemplateName);

    /** ???X???��X*/
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    TArray<FScreenElementTemplate> GetAllTemplates();

    /** �q��??�Ы�????*/
    UFUNCTION(BlueprintCallable, Category = "Screen Template Manager")
    FScreenElement CreateElementFromTemplate(const FString& TemplateName, const FString& ElementName, const FVector2D& Position);

protected:
    /** ????�q??�ҪO */
    void LoadDefaultTemplates();

    /** �O??�ҪO */
    bool SaveTemplate(const FScreenElementTemplate& Template);

    /** ????�ҪO */
    void LoadTemplates();

private:
    /** �ҪO??�� */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    TArray<FScreenElementTemplate> Templates;

    /** �t��??�_�w??�l?? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** �ҪO??���?? */
    UPROPERTY(BlueprintReadOnly, Category = "Screen Template Manager", meta = (AllowPrivateAccess = "true"))
    FString TemplateFilePath;
};


