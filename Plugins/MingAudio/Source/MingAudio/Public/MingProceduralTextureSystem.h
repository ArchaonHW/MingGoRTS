#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MingProceduralTextureSystem.generated.h"

UENUM(BlueprintType)
enum class EMingTextureType : uint8
{
    Diffuse UMETA(DisplayName = "Diffuse"),
    Normal UMETA(DisplayName = "Normal"),
    Roughness UMETA(DisplayName = "Roughness"),
    Metallic UMETA(DisplayName = "Metallic"),
    AmbientOcclusion UMETA(DisplayName = "Ambient Occlusion"),
    Height UMETA(DisplayName = "Height"),
    Emissive UMETA(DisplayName = "Emissive"),
    Combined UMETA(DisplayName = "Combined ORM")
};

UENUM(BlueprintType)
enum class EMingTextureMaterial : uint8
{
    Concrete UMETA(DisplayName = "Concrete"),
    Brick UMETA(DisplayName = "Brick"),
    Wood UMETA(DisplayName = "Wood"),
    Metal UMETA(DisplayName = "Metal"),
    Fabric UMETA(DisplayName = "Fabric"),
    Grass UMETA(DisplayName = "Grass"),
    Dirt UMETA(DisplayName = "Dirt"),
    Stone UMETA(DisplayName = "Stone"),
    Sand UMETA(DisplayName = "Sand"),
    Snow UMETA(DisplayName = "Snow"),
    Rust UMETA(DisplayName = "Rust"),
    Camouflage UMETA(DisplayName = "Camouflage")
};

UENUM(BlueprintType)
enum class EMingRepublicanEraStyle : uint8
{
    Traditional UMETA(DisplayName = "Traditional Chinese"),
    Military UMETA(DisplayName = "Republican Military"),
    Industrial UMETA(DisplayName = "Early Industrial"),
    Urban UMETA(DisplayName = "Republican Urban"),
    Rural UMETA(DisplayName = "Republican Rural"),
    Weathered UMETA(DisplayName = "Weathered/Aged")
};

USTRUCT(BlueprintType)
struct FMingTextureGenerationParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    EMingTextureType TextureType = EMingTextureType::Diffuse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    EMingTextureMaterial MaterialType = EMingTextureMaterial::Concrete;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    EMingRepublicanEraStyle Style = EMingRepublicanEraStyle::Traditional;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    int32 Resolution = 1024;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    int32 Seed = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    float Roughness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    float WeatheringAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    float DirtAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    float Scale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
    TArray<FLinearColor> ColorPalette;

    FMingTextureGenerationParams()
        : TextureType(EMingTextureType::Diffuse)
        , MaterialType(EMingTextureMaterial::Concrete)
        , Style(EMingRepublicanEraStyle::Traditional)
        , Resolution(1024)
        , Seed(-1)
        , Roughness(0.5f)
        , WeatheringAmount(0.0f)
        , DirtAmount(0.0f)
        , Scale(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingMaterialSet
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    UTexture2D* DiffuseMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    UTexture2D* NormalMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    UTexture2D* RoughnessMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    UTexture2D* MetallicMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    UTexture2D* AmbientOcclusionMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    UTexture2D* HeightMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Set")
    FString MaterialName;

    FMingMaterialSet()
        : DiffuseMap(nullptr)
        , NormalMap(nullptr)
        , RoughnessMap(nullptr)
        , MetallicMap(nullptr)
        , AmbientOcclusionMap(nullptr)
        , HeightMap(nullptr)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTextureGenerated, EMingTextureType, TextureType, UTexture2D*, GeneratedTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaterialSetCompleted, const FMingMaterialSet&, MaterialSet};

/**
 * Procedural Texture Generation System
 * Generates textures using runtime algorithms and noise functions
 */
UCLASS(ClassGroup = (Rendering), Blueprintable)
class MINGAUDIO_API UMingProceduralTextureSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingProceduralTextureSystem();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture")
    void InitializeTextureSystem();

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture")
    void ShutdownTextureSystem();

    // Single Texture Generation
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Single")
    UTexture2D* GenerateTexture(const FMingTextureGenerationParams& Params};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Single")
    UTexture2D* GenerateNoiseTexture(int32 Resolution, float Scale, int32 Seed = -1};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Single")
    UTexture2D* GeneratePatternTexture(EMingTextureMaterial Material, int32 Resolution};

    // Material Set Generation (Complete PBR Material)
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Material Set")
    FMingMaterialSet GenerateMaterialSet(const FString& MaterialName, EMingTextureMaterial MaterialType, int32 Resolution = 1024};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Material Set")
    void GenerateMaterialSetAsync(const FString& MaterialName, EMingTextureMaterial MaterialType, int32 Resolution = 1024};

    // Republican Era Specific Generators
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Republican Era")
    UTexture2D* GenerateRepublicanCamouflage(int32 Resolution = 1024, bool bWinterVariant = false};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Republican Era")
    UTexture2D* GenerateAgedPaperTexture(int32 Resolution = 512, float AgingAmount = 0.5f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Republican Era")
    UTexture2D* GenerateTraditionalFabric(int32 Resolution = 1024, bool bSilk = false};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Republican Era")
    UTexture2D* GenerateWeatheredMetal(int32 Resolution = 1024, float RustAmount = 0.3f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Republican Era")
    UTexture2D* GenerateMudAndDirt(int32 Resolution = 1024, float Wetness = 0.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Republican Era")
    UTexture2D* GenerateTraditionalRoofTiles(int32 Resolution = 1024, bool bDamaged = false};

    // Utility Textures
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Utility")
    UTexture2D* GenerateGradientTexture(FLinearColor ColorA, FLinearColor ColorB, int32 Resolution = 256, bool bHorizontal = true};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Utility")
    UTexture2D* GenerateNoiseNormalMap(int32 Resolution = 1024, float BumpScale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Utility")
    UTexture2D* GenerateCloudTexture(int32 Resolution = 512, float Density = 0.5f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Utility")
    UTexture2D* GenerateWoodGrain(int32 Resolution = 1024, float GrainScale = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Utility")
    UTexture2D* GenerateBrickPattern(int32 Resolution = 1024, float MortarWidth = 0.1f};

    // Texture Operations
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Operations")
    UTexture2D* CombineTextures(UTexture2D* TextureA, UTexture2D* TextureB, float BlendFactor = 0.5f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Operations")
    UTexture2D* ApplyWeathering(UTexture2D* SourceTexture, float WeatheringAmount};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Operations")
    UTexture2D* GenerateNormalFromHeight(UTexture2D* HeightTexture, float Strength = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Operations")
    UTexture2D* GenerateORMMap(UTexture2D* AmbientOcclusion, UTexture2D* Roughness, UTexture2D* Metallic};

    // Material Creation
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Material")
    UMaterialInstanceDynamic* CreateMaterialInstance(const FMingMaterialSet& MaterialSet, UMaterialInterface* ParentMaterial = nullptr};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Material")
    void ApplyTexturesToMaterial(UMaterialInstanceDynamic* MaterialInstance, const FMingMaterialSet& MaterialSet};

    // Texture Library Management
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Library")
    void AddTextureToLibrary(const FString& Name, UTexture2D* Texture};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Library")
    UTexture2D* GetTextureFromLibrary(const FString& Name};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Library")
    void RemoveTextureFromLibrary(const FString& Name};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Library")
    void ClearTextureLibrary();

    UFUNCTION(BlueprintPure, Category = "Procedural Texture|Library")
    TArray<FString> GetTextureLibraryNames() const;

    // Export/Import
    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|IO")
    bool ExportTexture(UTexture2D* Texture, const FString& FilePath};

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|IO")
    UTexture2D* ImportTexture(const FString& FilePath};

    // Utility Functions
    UFUNCTION(BlueprintPure, Category = "Procedural Texture|Utility")
    FLinearColor GetMaterialBaseColor(EMingTextureMaterial Material) const;

    UFUNCTION(BlueprintPure, Category = "Procedural Texture|Utility")
    float GetMaterialRoughness(EMingTextureMaterial Material) const;

    UFUNCTION(BlueprintPure, Category = "Procedural Texture|Utility")
    float GetMaterialMetallic(EMingTextureMaterial Material) const;

    UFUNCTION(BlueprintCallable, Category = "Procedural Texture|Utility")
    void SetRandomSeed(int32 Seed};

    UFUNCTION(BlueprintPure, Category = "Procedural Texture|Utility")
    int32 GetRandomSeed() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTextureGenerated OnTextureGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMaterialSetCompleted OnMaterialSetCompleted;

protected:
    UPROPERTY()
    int32 CurrentSeed;

    UPROPERTY()
    TMap<FString, UTexture2D*> TextureLibrary;

    UPROPERTY()
    TMap<FString, FMingMaterialSet> MaterialLibrary;

    // Internal generation functions
    UTexture2D* CreateTextureFromData(const TArray<FColor>& PixelData, int32 Resolution, const FString& TextureName};
    TArray<FColor> GenerateDiffusePixels(EMingTextureMaterial Material, int32 Resolution, const FMingTextureGenerationParams& Params};
    TArray<FColor> GenerateNormalPixels(const TArray<FColor>& HeightData, int32 Resolution, float Strength};
    TArray<FColor> GenerateRoughnessPixels(EMingTextureMaterial Material, int32 Resolution, float BaseRoughness};
    TArray<FColor> GenerateMetallicPixels(EMingTextureMaterial Material, int32 Resolution};
    TArray<FColor> GenerateAmbientOcclusionPixels(const TArray<FColor>& HeightData, int32 Resolution};
    TArray<FColor> GenerateHeightPixels(EMingTextureMaterial Material, int32 Resolution};

    // Noise functions
    float PerlinNoise(float X, float Y, float Scale) const;
    float SimplexNoise(float X, float Y, float Scale) const;
    float ValueNoise(float X, float Y, float Scale) const;
    float FractalBrownianMotion(float X, float Y, int32 Octaves, float Persistence, float Scale) const;

    // Pattern generators
    TArray<FColor> GenerateConcretePattern(int32 Resolution, float Scale, float Weathering};
    TArray<FColor> GenerateBrickPattern(int32 Resolution, float MortarWidth, float Weathering};
    TArray<FColor> GenerateWoodPattern(int32 Resolution, float GrainScale, float Weathering};
    TArray<FColor> GenerateMetalPattern(int32 Resolution, float RustAmount, float Weathering};
    TArray<FColor> GenerateFabricPattern(int32 Resolution, bool bSilk, float Weathering};
    TArray<FColor> GenerateGrassPattern(int32 Resolution, float Scale, float Weathering};
    TArray<FColor> GenerateDirtPattern(int32 Resolution, float Wetness, float Weathering};
    TArray<FColor> GenerateStonePattern(int32 Resolution, float Scale, float Weathering};
    TArray<FColor> GenerateSandPattern(int32 Resolution, float Scale, float Weathering};
    TArray<FColor> GenerateSnowPattern(int32 Resolution, float Scale, float Weathering};
    TArray<FColor> GenerateRustPattern(int32 Resolution, float RustAmount};
    TArray<FColor> GenerateCamouflagePattern(int32 Resolution, bool bWinter, float Scale};

    // Color utilities
    FLinearColor LerpColor(const FLinearColor& A, const FLinearColor& B, float T) const;
    FLinearColor MultiplyColor(const FLinearColor& A, const FLinearColor& B) const;
    FLinearColor AddColor(const FLinearColor& A, const FLinearColor& B) const;
    uint8 FloatToByte(float Value) const;

    // Weathering effects
    void ApplyAgingEffect(TArray<FColor>& PixelData, int32 Resolution, float AgingAmount};
    void ApplyDirtEffect(TArray<FColor>& PixelData, int32 Resolution, float DirtAmount};
    void ApplyScratchesEffect(TArray<FColor>& PixelData, int32 Resolution, float ScratchAmount};
    void ApplyStainEffect(TArray<FColor>& PixelData, int32 Resolution, float StainAmount};
};

