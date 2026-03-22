#include "MingProceduralTextureSystem.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureDerivedData.h"
#include "Misc/SecureHash.h"

UMingProceduralTextureSystem::UMingProceduralTextureSystem()
    : CurrentSeed(-1)
{
}

void UMingProceduralTextureSystem::InitializeTextureSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Procedural Texture System"));
    
    // Initialize random seed if not set
    if (CurrentSeed < 0)
    {
        CurrentSeed = FMath::Rand();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Texture System initialized with seed: %d"), CurrentSeed);
}

void UMingProceduralTextureSystem::ShutdownTextureSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Texture System"));
    
    ClearTextureLibrary();
}

// Single Texture Generation
UTexture2D* UMingProceduralTextureSystem::GenerateTexture(const FMingTextureGenerationParams& Params)
{
    TArray<FColor> PixelData;
    
    // Set seed for reproducible results
    int32 EffectiveSeed = (Params.Seed >= 0) ? Params.Seed : CurrentSeed;
    FMath::RandInit(EffectiveSeed);
    
    // Generate pixel data based on texture type
    switch (Params.TextureType)
    {
    case EMingTextureType::Diffuse:
        PixelData = GenerateDiffusePixels(Params.MaterialType, Params.Resolution, Params);
        break;
    case EMingTextureType::Normal:
        {
            TArray<FColor> HeightData = GenerateHeightPixels(Params.MaterialType, Params.Resolution);
            PixelData = GenerateNormalPixels(HeightData, Params.Resolution, Params.Scale);
        }
        break;
    case EMingTextureType::Roughness:
        PixelData = GenerateRoughnessPixels(Params.MaterialType, Params.Resolution, Params.Roughness);
        break;
    case EMingTextureType::Metallic:
        PixelData = GenerateMetallicPixels(Params.MaterialType, Params.Resolution);
        break;
    case EMingTextureType::AmbientOcclusion:
        {
            TArray<FColor> HeightData = GenerateHeightPixels(Params.MaterialType, Params.Resolution);
            PixelData = GenerateAmbientOcclusionPixels(HeightData, Params.Resolution);
        }
        break;
    case EMingTextureType::Height:
        PixelData = GenerateHeightPixels(Params.MaterialType, Params.Resolution);
        break;
    default:
        PixelData = GenerateDiffusePixels(Params.MaterialType, Params.Resolution, Params);
        break;
    }
    
    // Apply weathering effects
    if (Params.WeatheringAmount > 0.0f)
    {
        ApplyAgingEffect(PixelData, Params.Resolution, Params.WeatheringAmount);
    }
    
    if (Params.DirtAmount > 0.0f)
    {
        ApplyDirtEffect(PixelData, Params.Resolution, Params.DirtAmount);
    }
    
    // Create texture from pixel data
    FString TextureName = FString::Printf(TEXT("ProcTex_%s_%s_%d"), 
        *GetMaterialTypeName(Params.MaterialType), 
        *GetTextureTypeName(Params.TextureType),
        EffectiveSeed);
    
    UTexture2D* GeneratedTexture = CreateTextureFromData(PixelData, Params.Resolution, TextureName);
    
    if (GeneratedTexture)
    {
        OnTextureGenerated.Broadcast(Params.TextureType, GeneratedTexture);
    }
    
    return GeneratedTexture;
}

UTexture2D* UMingProceduralTextureSystem::GenerateNoiseTexture(int32 Resolution, float Scale, int32 Seed)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    int32 EffectiveSeed = (Seed >= 0) ? Seed : CurrentSeed;
    FMath::RandInit(EffectiveSeed);
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            float NoiseValue = FractalBrownianMotion(X, Y, 4, 0.5f, Scale);
            uint8 GrayValue = FloatToByte(NoiseValue);
            PixelData.Add(FColor(GrayValue, GrayValue, GrayValue, 255));
        }
    }
    
    FString TextureName = FString::Printf(TEXT("NoiseTex_%d_%d"), Resolution, EffectiveSeed);
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GeneratePatternTexture(EMingTextureMaterial Material, int32 Resolution)
{
    TArray<FColor> PixelData;
    
    switch (Material)
    {
    case EMingTextureMaterial::Brick:
        PixelData = GenerateBrickPattern(Resolution, 0.08f, 0.0f);
        break;
    case EMingTextureMaterial::Wood:
        PixelData = GenerateWoodPattern(Resolution, 1.0f, 0.0f);
        break;
    case EMingTextureMaterial::Concrete:
        PixelData = GenerateConcretePattern(Resolution, 1.0f, 0.0f);
        break;
    case EMingTextureMaterial::Metal:
        PixelData = GenerateMetalPattern(Resolution, 0.0f, 0.0f);
        break;
    case EMingTextureMaterial::Fabric:
        PixelData = GenerateFabricPattern(Resolution, false, 0.0f);
        break;
    default:
        PixelData = GenerateConcretePattern(Resolution, 1.0f, 0.0f);
        break;
    }
    
    FString TextureName = FString::Printf(TEXT("Pattern_%s_%d"), *GetMaterialTypeName(Material), Resolution);
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

// Material Set Generation
FMingMaterialSet UMingProceduralTextureSystem::GenerateMaterialSet(const FString& MaterialName, EMingTextureMaterial MaterialType, int32 Resolution)
{
    FMingMaterialSet MaterialSet;
    MaterialSet.MaterialName = MaterialName;
    
    FMingTextureGenerationParams Params;
    Params.MaterialType = MaterialType;
    Params.Resolution = Resolution;
    
    // Generate Diffuse
    Params.TextureType = EMingTextureType::Diffuse;
    MaterialSet.DiffuseMap = GenerateTexture(Params);
    
    // Generate Normal (from height)
    Params.TextureType = EMingTextureType::Normal;
    MaterialSet.NormalMap = GenerateTexture(Params);
    
    // Generate Roughness
    Params.TextureType = EMingTextureType::Roughness;
    MaterialSet.RoughnessMap = GenerateTexture(Params);
    
    // Generate Metallic
    Params.TextureType = EMingTextureType::Metallic;
    MaterialSet.MetallicMap = GenerateTexture(Params);
    
    // Generate AO
    Params.TextureType = EMingTextureType::AmbientOcclusion;
    MaterialSet.AmbientOcclusionMap = GenerateTexture(Params);
    
    // Generate Height
    Params.TextureType = EMingTextureType::Height;
    MaterialSet.HeightMap = GenerateTexture(Params);
    
    // Store in library
    MaterialLibrary.Add(MaterialName, MaterialSet);
    
    OnMaterialSetCompleted.Broadcast(MaterialSet);
    
    return MaterialSet;
}

void UMingProceduralTextureSystem::GenerateMaterialSetAsync(const FString& MaterialName, EMingTextureMaterial MaterialType, int32 Resolution)
{
    // For async generation, we would use a background thread
    // For now, we call the synchronous version
    GenerateMaterialSet(MaterialName, MaterialType, Resolution);
}

// Republican Era Specific Generators
UTexture2D* UMingProceduralTextureSystem::GenerateRepublicanCamouflage(int32 Resolution, bool bWinterVariant)
{
    TArray<FColor> PixelData = GenerateCamouflagePattern(Resolution, bWinterVariant, 1.0f);
    FString TextureName = FString::Printf(TEXT("RepublicanCamo_%s"), bWinterVariant ? TEXT("Winter") : TEXT("Summer"));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateAgedPaperTexture(int32 Resolution, float AgingAmount)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    // Base paper color
    FLinearColor PaperColor(0.95f, 0.92f, 0.84f);
    FLinearColor AgingColor(0.7f, 0.65f, 0.5f);
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            float Noise = FractalBrownianMotion(X, Y, 3, 0.5f, 50.0f);
            float AgingFactor = Noise * AgingAmount;
            
            FLinearColor FinalColor = LerpColor(PaperColor, AgingColor, AgingFactor);
            
            // Add some stains
            if (FMath::RandRange(0.0f, 1.0f) < 0.01f * AgingAmount)
            {
                FLinearColor StainColor(0.5f, 0.4f, 0.3f);
                FinalColor = LerpColor(FinalColor, StainColor, 0.3f);
            }
            
            PixelData.Add(FColor(
                FloatToByte(FinalColor.R),
                FloatToByte(FinalColor.G),
                FloatToByte(FinalColor.B),
                255));
        }
    }
    
    FString TextureName = FString::Printf(TEXT("AgedPaper_%d"), FMath::RoundToInt(AgingAmount * 100));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateTraditionalFabric(int32 Resolution, bool bSilk)
{
    TArray<FColor> PixelData = GenerateFabricPattern(Resolution, bSilk, 0.0f);
    FString TextureName = FString::Printf(TEXT("TraditionalFabric_%s"), bSilk ? TEXT("Silk") : TEXT("Cotton"));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateWeatheredMetal(int32 Resolution, float RustAmount)
{
    TArray<FColor> PixelData = GenerateMetalPattern(Resolution, RustAmount, 0.5f);
    FString TextureName = FString::Printf(TEXT("WeatheredMetal_%d"), FMath::RoundToInt(RustAmount * 100));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateMudAndDirt(int32 Resolution, float Wetness)
{
    TArray<FColor> PixelData = GenerateDirtPattern(Resolution, Wetness, 0.0f);
    FString TextureName = FString::Printf(TEXT("MudDirt_Wet%d"), FMath::RoundToInt(Wetness * 100));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateTraditionalRoofTiles(int32 Resolution, bool bDamaged)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    // Tile color (traditional Chinese blue-gray)
    FLinearColor TileColor(0.3f, 0.35f, 0.4f);
    FLinearColor DamageColor(0.2f, 0.22f, 0.25f);
    
    int32 TileSize = Resolution / 16;
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            int32 TileX = X / TileSize;
            int32 TileY = Y / TileSize;
            int32 LocalX = X % TileSize;
            int32 LocalY = Y % TileSize;
            
            FLinearColor Color = TileColor;
            
            // Add tile edges
            if (LocalX < 2 || LocalY < 2)
            {
                Color = Color * 0.7f;
            }
            
            // Add damage if requested
            if (bDamaged && FMath::RandRange(0.0f, 1.0f) < 0.05f)
            {
                Color = LerpColor(Color, DamageColor, 0.5f);
            }
            
            // Add noise
            float Noise = FractalBrownianMotion(X, Y, 2, 0.5f, 20.0f) * 0.1f;
            Color = Color * (1.0f + Noise);
            
            PixelData.Add(FColor(
                FloatToByte(Color.R),
                FloatToByte(Color.G),
                FloatToByte(Color.B),
                255));
        }
    }
    
    FString TextureName = FString::Printf(TEXT("RoofTiles_%s"), bDamaged ? TEXT("Damaged") : TEXT("New"));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

// Utility Textures
UTexture2D* UMingProceduralTextureSystem::GenerateGradientTexture(FLinearColor ColorA, FLinearColor ColorB, int32 Resolution, bool bHorizontal)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            float T;
            if (bHorizontal)
            {
                T = static_cast<float>(X) / Resolution;
            }
            else
            {
                T = static_cast<float>(Y) / Resolution;
            }
            
            FLinearColor FinalColor = LerpColor(ColorA, ColorB, T);
            PixelData.Add(FColor(
                FloatToByte(FinalColor.R),
                FloatToByte(FinalColor.G),
                FloatToByte(FinalColor.B),
                255));
        }
    }
    
    FString TextureName = FString::Printf(TEXT("Gradient_%s"), bHorizontal ? TEXT("H") : TEXT("V"));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateNoiseNormalMap(int32 Resolution, float BumpScale)
{
    TArray<FColor> HeightData;
    HeightData.Reserve(Resolution * Resolution);
    
    // Generate height data
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            float Height = FractalBrownianMotion(X, Y, 4, 0.5f, 20.0f);
            uint8 HeightByte = FloatToByte(Height);
            HeightData.Add(FColor(HeightByte, HeightByte, HeightByte, 255));
        }
    }
    
    // Convert to normal map
    TArray<FColor> NormalData = GenerateNormalPixels(HeightData, Resolution, BumpScale);
    
    FString TextureName = FString::Printf(TEXT("NoiseNormal_%d"), FMath::RoundToInt(BumpScale * 10));
    return CreateTextureFromData(NormalData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateCloudTexture(int32 Resolution, float Density)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            float Noise = FractalBrownianMotion(X, Y, 5, 0.6f, 8.0f);
            float CloudValue = FMath::Clamp((Noise - (1.0f - Density)) / Density, 0.0f, 1.0f);
            
            uint8 GrayValue = FloatToByte(CloudValue);
            PixelData.Add(FColor(GrayValue, GrayValue, GrayValue, 255));
        }
    }
    
    FString TextureName = FString::Printf(TEXT("Cloud_D%d"), FMath::RoundToInt(Density * 100));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateWoodGrain(int32 Resolution, float GrainScale)
{
    TArray<FColor> PixelData = GenerateWoodPattern(Resolution, GrainScale, 0.0f);
    FString TextureName = FString::Printf(TEXT("WoodGrain_%d"), FMath::RoundToInt(GrainScale * 10));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

UTexture2D* UMingProceduralTextureSystem::GenerateBrickPattern(int32 Resolution, float MortarWidth)
{
    TArray<FColor> PixelData = GenerateBrickPattern(Resolution, MortarWidth, 0.0f);
    FString TextureName = FString::Printf(TEXT("Brick_M%d"), FMath::RoundToInt(MortarWidth * 100));
    return CreateTextureFromData(PixelData, Resolution, TextureName);
}

// Texture Operations
UTexture2D* UMingProceduralTextureSystem::CombineTextures(UTexture2D* TextureA, UTexture2D* TextureB, float BlendFactor)
{
    if (!TextureA || !TextureB)
    {
        return nullptr;
    }
    
    // This would require reading pixel data from both textures and blending
    // For now, return textureA as placeholder
    return TextureA;
}

UTexture2D* UMingProceduralTextureSystem::ApplyWeathering(UTexture2D* SourceTexture, float WeatheringAmount)
{
    // Apply weathering effects to existing texture
    // This would require reading the texture, modifying pixels, and creating new texture
    return SourceTexture;
}

UTexture2D* UMingProceduralTextureSystem::GenerateNormalFromHeight(UTexture2D* HeightTexture, float Strength)
{
    if (!HeightTexture)
    {
        return nullptr;
    }
    
    // Read height data and generate normals
    int32 Resolution = HeightTexture->GetSizeX();
    // Implementation would require accessing texture pixel data
    
    return nullptr;
}

UTexture2D* UMingProceduralTextureSystem::GenerateORMMap(UTexture2D* AmbientOcclusion, UTexture2D* Roughness, UTexture2D* Metallic)
{
    // Combine AO, Roughness, Metallic into ORM map
    // R = Ambient Occlusion, G = Roughness, B = Metallic
    if (!AmbientOcclusion || !Roughness || !Metallic)
    {
        return nullptr;
    }
    
    // Implementation would combine the three textures
    return Roughness;
}

// Material Creation
UMaterialInstanceDynamic* UMingProceduralTextureSystem::CreateMaterialInstance(const FMingMaterialSet& MaterialSet, UMaterialInterface* ParentMaterial)
{
    if (!ParentMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("No parent material provided for instance creation"));
        return nullptr;
    }
    
    UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, this);
    
    if (DynMaterial)
    {
        ApplyTexturesToMaterial(DynMaterial, MaterialSet);
    }
    
    return DynMaterial;
}

void UMingProceduralTextureSystem::ApplyTexturesToMaterial(UMaterialInstanceDynamic* MaterialInstance, const FMingMaterialSet& MaterialSet)
{
    if (!MaterialInstance)
    {
        return;
    }
    
    if (MaterialSet.DiffuseMap)
    {
        MaterialInstance->SetTextureParameterValue(TEXT("DiffuseMap"), MaterialSet.DiffuseMap);
    }
    
    if (MaterialSet.NormalMap)
    {
        MaterialInstance->SetTextureParameterValue(TEXT("NormalMap"), MaterialSet.NormalMap);
    }
    
    if (MaterialSet.RoughnessMap)
    {
        MaterialInstance->SetTextureParameterValue(TEXT("RoughnessMap"), MaterialSet.RoughnessMap);
    }
    
    if (MaterialSet.MetallicMap)
    {
        MaterialInstance->SetTextureParameterValue(TEXT("MetallicMap"), MaterialSet.MetallicMap);
    }
    
    if (MaterialSet.AmbientOcclusionMap)
    {
        MaterialInstance->SetTextureParameterValue(TEXT("AOMap"), MaterialSet.AmbientOcclusionMap);
    }
}

// Texture Library Management
void UMingProceduralTextureSystem::AddTextureToLibrary(const FString& Name, UTexture2D* Texture)
{
    if (Texture)
    {
        TextureLibrary.Add(Name, Texture);
        UE_LOG(LogTemp, Log, TEXT("Added texture '%s' to library"), *Name);
    }
}

UTexture2D* UMingProceduralTextureSystem::GetTextureFromLibrary(const FString& Name)
{
    UTexture2D** FoundTexture = TextureLibrary.Find(Name);
    if (FoundTexture)
    {
        return *FoundTexture;
    }
    return nullptr;
}

void UMingProceduralTextureSystem::RemoveTextureFromLibrary(const FString& Name)
{
    TextureLibrary.Remove(Name);
    UE_LOG(LogTemp, Log, TEXT("Removed texture '%s' from library"), *Name);
}

void UMingProceduralTextureSystem::ClearTextureLibrary()
{
    TextureLibrary.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared texture library"));
}

TArray<FString> UMingProceduralTextureSystem::GetTextureLibraryNames() const
{
    TArray<FString> Names;
    TextureLibrary.GetKeys(Names);
    return Names;
}

// Export/Import
bool UMingProceduralTextureSystem::ExportTexture(UTexture2D* Texture, const FString& FilePath)
{
    // Implementation would export texture to file
    UE_LOG(LogTemp, Log, TEXT("Exporting texture to: %s"), *FilePath);
    return false;
}

UTexture2D* UMingProceduralTextureSystem::ImportTexture(const FString& FilePath)
{
    // Implementation would import texture from file
    UE_LOG(LogTemp, Log, TEXT("Importing texture from: %s"), *FilePath);
    return nullptr;
}

// Utility Functions
FLinearColor UMingProceduralTextureSystem::GetMaterialBaseColor(EMingTextureMaterial Material) const
{
    switch (Material)
    {
    case EMingTextureMaterial::Concrete: return FLinearColor(0.7f, 0.7f, 0.7f);
    case EMingTextureMaterial::Brick: return FLinearColor(0.6f, 0.3f, 0.2f);
    case EMingTextureMaterial::Wood: return FLinearColor(0.4f, 0.25f, 0.15f);
    case EMingTextureMaterial::Metal: return FLinearColor(0.5f, 0.5f, 0.55f);
    case EMingTextureMaterial::Fabric: return FLinearColor(0.3f, 0.3f, 0.4f);
    case EMingTextureMaterial::Grass: return FLinearColor(0.2f, 0.5f, 0.1f);
    case EMingTextureMaterial::Dirt: return FLinearColor(0.3f, 0.2f, 0.15f);
    case EMingTextureMaterial::Stone: return FLinearColor(0.5f, 0.5f, 0.48f);
    case EMingTextureMaterial::Sand: return FLinearColor(0.76f, 0.7f, 0.5f);
    case EMingTextureMaterial::Snow: return FLinearColor(0.95f, 0.95f, 0.98f);
    case EMingTextureMaterial::Rust: return FLinearColor(0.55f, 0.25f, 0.1f);
    case EMingTextureMaterial::Camouflage: return FLinearColor(0.25f, 0.35f, 0.2f);
    default: return FLinearColor(0.5f, 0.5f, 0.5f);
    }
}

float UMingProceduralTextureSystem::GetMaterialRoughness(EMingTextureMaterial Material) const
{
    switch (Material)
    {
    case EMingTextureMaterial::Concrete: return 0.7f;
    case EMingTextureMaterial::Brick: return 0.8f;
    case EMingTextureMaterial::Wood: return 0.6f;
    case EMingTextureMaterial::Metal: return 0.3f;
    case EMingTextureMaterial::Fabric: return 0.9f;
    case EMingTextureMaterial::Grass: return 0.95f;
    case EMingTextureMaterial::Dirt: return 0.9f;
    case EMingTextureMaterial::Stone: return 0.6f;
    case EMingTextureMaterial::Sand: return 0.8f;
    case EMingTextureMaterial::Snow: return 0.4f;
    case EMingTextureMaterial::Rust: return 0.85f;
    case EMingTextureMaterial::Camouflage: return 0.8f;
    default: return 0.5f;
    }
}

float UMingProceduralTextureSystem::GetMaterialMetallic(EMingTextureMaterial Material) const
{
    switch (Material)
    {
    case EMingTextureMaterial::Metal: return 1.0f;
    case EMingTextureMaterial::Rust: return 0.8f;
    default: return 0.0f;
    }
}

void UMingProceduralTextureSystem::SetRandomSeed(int32 Seed)
{
    CurrentSeed = Seed;
    FMath::RandInit(Seed);
}

int32 UMingProceduralTextureSystem::GetRandomSeed() const
{
    return CurrentSeed;
}

// Internal generation functions
UTexture2D* UMingProceduralTextureSystem::CreateTextureFromData(const TArray<FColor>& PixelData, int32 Resolution, const FString& TextureName)
{
    UTexture2D* NewTexture = UTexture2D::CreateTransient(Resolution, Resolution, PF_B8G8R8A8);
    
    if (!NewTexture)
    {
        return nullptr;
    }
    
    NewTexture->CompressionSettings = TC_Default;
    NewTexture->SRGB = true;
    NewTexture->Filter = TF_Default;
    
    // Lock and fill the texture
    FTexture2DMipMap& Mip = NewTexture->GetPlatformData()->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
    
    FMemory::Memcpy(Data, PixelData.GetData(), PixelData.Num() * sizeof(FColor));
    
    Mip.BulkData.Unlock();
    NewTexture->UpdateResource();
    
    NewTexture->Rename(*TextureName);
    
    return NewTexture;
}

TArray<FColor> UMingProceduralTextureSystem::GenerateDiffusePixels(EMingTextureMaterial Material, int32 Resolution, const FMingTextureGenerationParams& Params)
{
    switch (Material)
    {
    case EMingTextureMaterial::Concrete: return GenerateConcretePattern(Resolution, Params.Scale, Params.WeatheringAmount);
    case EMingTextureMaterial::Brick: return GenerateBrickPattern(Resolution, 0.08f, Params.WeatheringAmount);
    case EMingTextureMaterial::Wood: return GenerateWoodPattern(Resolution, Params.Scale, Params.WeatheringAmount);
    case EMingTextureMaterial::Metal: return GenerateMetalPattern(Resolution, 0.0f, Params.WeatheringAmount);
    case EMingTextureMaterial::Fabric: return GenerateFabricPattern(Resolution, false, Params.WeatheringAmount);
    case EMingTextureMaterial::Grass: return GenerateGrassPattern(Resolution, Params.Scale, Params.WeatheringAmount);
    case EMingTextureMaterial::Dirt: return GenerateDirtPattern(Resolution, 0.0f, Params.WeatheringAmount);
    case EMingTextureMaterial::Stone: return GenerateStonePattern(Resolution, Params.Scale, Params.WeatheringAmount);
    case EMingTextureMaterial::Sand: return GenerateSandPattern(Resolution, Params.Scale, Params.WeatheringAmount);
    case EMingTextureMaterial::Snow: return GenerateSnowPattern(Resolution, Params.Scale, Params.WeatheringAmount);
    case EMingTextureMaterial::Rust: return GenerateRustPattern(Resolution, 0.5f);
    case EMingTextureMaterial::Camouflage: return GenerateCamouflagePattern(Resolution, false, Params.Scale);
    default: return GenerateConcretePattern(Resolution, 1.0f, 0.0f);
    }
}

// Pattern generators implementation would continue here...
// For brevity, I'll provide simplified implementations

TArray<FColor> UMingProceduralTextureSystem::GenerateConcretePattern(int32 Resolution, float Scale, float Weathering)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    FLinearColor BaseColor(0.7f, 0.7f, 0.7f);
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            float Noise = FractalBrownianMotion(X, Y, 4, 0.5f, 50.0f * Scale);
            FLinearColor Color = BaseColor * (0.9f + Noise * 0.2f);
            
            // Apply weathering
            if (Weathering > 0.0f)
            {
                Color = Color * (1.0f - Weathering * 0.3f);
            }
            
            PixelData.Add(FColor(
                FloatToByte(Color.R),
                FloatToByte(Color.G),
                FloatToByte(Color.B),
                255));
        }
    }
    
    return PixelData;
}

TArray<FColor> UMingProceduralTextureSystem::GenerateBrickPattern(int32 Resolution, float MortarWidth, float Weathering)
{
    TArray<FColor> PixelData;
    PixelData.Reserve(Resolution * Resolution);
    
    FLinearColor BrickColor(0.6f, 0.3f, 0.2f);
    FLinearColor MortarColor(0.75f, 0.75f, 0.7f);
    
    int32 BrickWidth = Resolution / 8;
    int32 BrickHeight = Resolution / 4;
    int32 MortarPixels = FMath::Max(1, FMath::RoundToInt(BrickWidth * MortarWidth));
    
    for (int32 Y = 0; Y < Resolution; Y++)
    {
        for (int32 X = 0; X < Resolution; X++)
        {
            int32 BrickX = X % BrickWidth;
            int32 BrickY = Y % BrickHeight;
            int32 Row = Y / BrickHeight;
            int32 Offset = (Row % 2) * (BrickWidth / 2);
            int32 AdjustedX = (X + Offset) % BrickWidth;
            
            FLinearColor Color;
            if (AdjustedX < MortarPixels || BrickX >= BrickWidth - MortarPixels || 
                BrickY < MortarPixels || BrickY >= BrickHeight - MortarPixels)
            {
                Color = MortarColor;
            }
            else
            {
                float Noise = FractalBrownianMotion(X, Y, 2, 0.5f, 30.0f);
                Color = BrickColor * (0.95f + Noise * 0.1f);
            }
            
            PixelData.Add(FColor(
                FloatToByte(Color.R),
                FloatToByte(Color.G),
                FloatToByte(Color.B),
                255));
        }
    }
    
    return PixelData;
}

// Helper implementations
FString UMingProceduralTextureSystem::GetMaterialTypeName(EMingTextureMaterial Material) const
{
    switch (Material)
    {
    case EMingTextureMaterial::Concrete: return TEXT("Concrete");
    case EMingTextureMaterial::Brick: return TEXT("Brick");
    case EMingTextureMaterial::Wood: return TEXT("Wood");
    case EMingTextureMaterial::Metal: return TEXT("Metal");
    case EMingTextureMaterial::Fabric: return TEXT("Fabric");
    case EMingTextureMaterial::Grass: return TEXT("Grass");
    case EMingTextureMaterial::Dirt: return TEXT("Dirt");
    case EMingTextureMaterial::Stone: return TEXT("Stone");
    case EMingTextureMaterial::Sand: return TEXT("Sand");
    case EMingTextureMaterial::Snow: return TEXT("Snow");
    case EMingTextureMaterial::Rust: return TEXT("Rust");
    case EMingTextureMaterial::Camouflage: return TEXT("Camo");
    default: return TEXT("Unknown");
    }
}

FString UMingProceduralTextureSystem::GetTextureTypeName(EMingTextureType Type) const
{
    switch (Type)
    {
    case EMingTextureType::Diffuse: return TEXT("Diffuse");
    case EMingTextureType::Normal: return TEXT("Normal");
    case EMingTextureType::Roughness: return TEXT("Roughness");
    case EMingTextureType::Metallic: return TEXT("Metallic");
    case EMingTextureType::AmbientOcclusion: return TEXT("AO");
    case EMingTextureType::Height: return TEXT("Height");
    case EMingTextureType::Emissive: return TEXT("Emissive");
    case EMingTextureType::Combined: return TEXT("ORM");
    default: return TEXT("Unknown");
    }
}

FLinearColor UMingProceduralTextureSystem::LerpColor(const FLinearColor& A, const FLinearColor& B, float T) const
{
    return A + (B - A) * T;
}

FLinearColor UMingProceduralTextureSystem::MultiplyColor(const FLinearColor& A, const FLinearColor& B) const
{
    return FLinearColor(A.R * B.R, A.G * B.G, A.B * B.B, A.A * B.A);
}

FLinearColor UMingProceduralTextureSystem::AddColor(const FLinearColor& A, const FLinearColor& B) const
{
    return FLinearColor(A.R + B.R, A.G + B.G, A.B + B.B, A.A + B.A);
}

uint8 UMingProceduralTextureSystem::FloatToByte(float Value) const
{
    return FMath::Clamp(FMath::RoundToInt(Value * 255.0f), 0, 255);
}

float UMingProceduralTextureSystem::PerlinNoise(float X, float Y, float Scale) const
{
    // Simplified Perlin noise implementation
    float ScaledX = X * Scale;
    float ScaledY = Y * Scale;
    
    return FMath::Sin(ScaledX) * FMath::Cos(ScaledY) * 0.5f + 0.5f;
}

float UMingProceduralTextureSystem::SimplexNoise(float X, float Y, float Scale) const
{
    // Simplified simplex noise
    return PerlinNoise(X, Y, Scale);
}

float UMingProceduralTextureSystem::ValueNoise(float X, float Y, float Scale) const
{
    float ScaledX = X * Scale;
    float ScaledY = Y * Scale;
    
    int32 XInt = FMath::FloorToInt(ScaledX);
    int32 YInt = FMath::FloorToInt(ScaledY);
    
    float FX = ScaledX - XInt;
    float FY = ScaledY - YInt;
    
    // Hash function for pseudo-random values
    float Random1 = FMath::Frac(FMath::Sin(XInt * 12.9898f + YInt * 78.233f) * 43758.5453f);
    float Random2 = FMath::Frac(FMath::Sin((XInt + 1) * 12.9898f + YInt * 78.233f) * 43758.5453f);
    float Random3 = FMath::Frac(FMath::Sin(XInt * 12.9898f + (YInt + 1) * 78.233f) * 43758.5453f);
    float Random4 = FMath::Frac(FMath::Sin((XInt + 1) * 12.9898f + (YInt + 1) * 78.233f) * 43758.5453f);
    
    // Bilinear interpolation
    float Interp1 = FMath::Lerp(Random1, Random2, FX);
    float Interp2 = FMath::Lerp(Random3, Random4, FX);
    
    return FMath::Lerp(Interp1, Interp2, FY);
}

float UMingProceduralTextureSystem::FractalBrownianMotion(float X, float Y, int32 Octaves, float Persistence, float Scale) const
{
    float Total = 0.0f;
    float Frequency = Scale;
    float Amplitude = 1.0f;
    float MaxValue = 0.0f;
    
    for (int32 I = 0; I < Octaves; I++)
    {
        Total += ValueNoise(X, Y, Frequency) * Amplitude;
        MaxValue += Amplitude;
        Amplitude *= Persistence;
        Frequency *= 2.0f;
    }
    
    return Total / MaxValue;
}
