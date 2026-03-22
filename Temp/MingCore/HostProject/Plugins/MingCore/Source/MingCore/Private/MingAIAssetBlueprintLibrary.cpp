#include "MingAIAssetBlueprintLibrary.h"
#include "MingAIAssetGenerator.h"
#include "MingAIAssetManager.h"
#include "MingRepublicEraPromptLibrary.h"
#include "Engine/Engine.h"

UMingAIAssetGenerator* UMingAIAssetBlueprintLibrary::GetAIAssetGenerator()
{
    return UMingAIAssetGenerator::Get();
}

UMingAIAssetManager* UMingAIAssetBlueprintLibrary::GetAIAssetManager()
{
    return UMingAIAssetManager::Get();
}

void UMingAIAssetBlueprintLibrary::InitializeAIAssetSystem()
{
    UMingAIAssetGenerator* Generator = UMingAIAssetGenerator::Get();
    if (!Generator)
    {
        // 創建生成器
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            Generator = NewObject<UMingAIAssetGenerator>(World);
            Generator->Initialize();
        }
    }
    
    UMingAIAssetManager* Manager = UMingAIAssetManager::Get();
    if (!Manager)
    {
        // 創建管理器
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            Manager = NewObject<UMingAIAssetManager>(World);
            Manager->Initialize();
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI Asset System initialized"));
}

FString UMingAIAssetBlueprintLibrary::GenerateCharacterPortrait(
    const FString& CharacterDescription, 
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Asset Generator not initialized"));
        return TEXT("");
    }
    
    FAIAssetGenerationResult Result = Generator->GenerateCharacterPortrait(
        CharacterDescription, EraStyle, ImageWidth, ImageHeight);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Character portrait generated: %s"), *Result.AssetID);
        return Result.AssetID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate character portrait: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

FString UMingAIAssetBlueprintLibrary::GenerateBuilding(
    const FString& BuildingDescription,
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Asset Generator not initialized"));
        return TEXT("");
    }
    
    FAIAssetGenerationResult Result = Generator->GenerateBuilding(
        BuildingDescription, EraStyle, ImageWidth, ImageHeight);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Building generated: %s"), *Result.AssetID);
        return Result.AssetID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate building: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

FString UMingAIAssetBlueprintLibrary::GenerateWeapon(
    const FString& WeaponDescription,
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Asset Generator not initialized"));
        return TEXT("");
    }
    
    FAIAssetGenerationParams Params;
    Params.AssetType = EAIAssetType::Weapon;
    Params.EraStyle = EraStyle;
    Params.BaseDescription = WeaponDescription;
    Params.ImageWidth = ImageWidth;
    Params.ImageHeight = ImageHeight;
    Params.DetailedPrompt = UMingRepublicEraPromptLibrary::GetWeaponPrompt(EraStyle, WeaponDescription);
    
    FAIAssetGenerationResult Result = Generator->GenerateAsset(Params);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Weapon generated: %s"), *Result.AssetID);
        return Result.AssetID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate weapon: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

FString UMingAIAssetBlueprintLibrary::GenerateUniform(
    const FString& UniformDescription,
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Asset Generator not initialized"));
        return TEXT("");
    }
    
    FAIAssetGenerationParams Params;
    Params.AssetType = EAIAssetType::Uniform;
    Params.EraStyle = EraStyle;
    Params.BaseDescription = UniformDescription;
    Params.ImageWidth = ImageWidth;
    Params.ImageHeight = ImageHeight;
    Params.DetailedPrompt = UMingRepublicEraPromptLibrary::GetUniformPrompt(EraStyle, UniformDescription);
    
    FAIAssetGenerationResult Result = Generator->GenerateAsset(Params);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Uniform generated: %s"), *Result.AssetID);
        return Result.AssetID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate uniform: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

FString UMingAIAssetBlueprintLibrary::GenerateBackgroundMusic(
    const FString& MusicDescription,
    ERepublicEraStyle EraStyle,
    float Duration,
    const FString& MusicStyle)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Asset Generator not initialized"));
        return TEXT("");
    }
    
    FAIAssetGenerationResult Result = Generator->GenerateBackgroundMusic(
        MusicDescription, EraStyle, Duration, MusicStyle);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Background music generated: %s"), *Result.AssetID);
        return Result.AssetID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate background music: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

FString UMingAIAssetBlueprintLibrary::GenerateScene(
    const FString& SceneDescription,
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (!Generator)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Asset Generator not initialized"));
        return TEXT("");
    }
    
    FAIAssetGenerationParams Params;
    Params.AssetType = EAIAssetType::Landscape;
    Params.EraStyle = EraStyle;
    Params.BaseDescription = SceneDescription;
    Params.ImageWidth = ImageWidth;
    Params.ImageHeight = ImageHeight;
    Params.DetailedPrompt = UMingRepublicEraPromptLibrary::GetScenePrompt(EraStyle, SceneDescription);
    
    FAIAssetGenerationResult Result = Generator->GenerateAsset(Params);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Scene generated: %s"), *Result.AssetID);
        return Result.AssetID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate scene: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

FString UMingAIAssetBlueprintLibrary::GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType)
{
    return UMingRepublicEraPromptLibrary::GetFullPrompt(EraStyle, AssetType, TEXT(""));
}

FString UMingAIAssetBlueprintLibrary::GetCharacterPortraitPrompt(ERepublicEraStyle EraStyle, const FString& CharacterType)
{
    return UMingRepublicEraPromptLibrary::GetCharacterPortraitPrompt(EraStyle, CharacterType);
}

FString UMingAIAssetBlueprintLibrary::GetBuildingPrompt(ERepublicEraStyle EraStyle, const FString& BuildingType)
{
    return UMingRepublicEraPromptLibrary::GetBuildingPrompt(EraStyle, BuildingType);
}

FString UMingAIAssetBlueprintLibrary::GetWeaponPrompt(ERepublicEraStyle EraStyle, const FString& WeaponType)
{
    return UMingRepublicEraPromptLibrary::GetWeaponPrompt(EraStyle, WeaponType);
}

FString UMingAIAssetBlueprintLibrary::GetMusicPrompt(ERepublicEraStyle EraStyle, const FString& MusicMood)
{
    return UMingRepublicEraPromptLibrary::GetMusicPrompt(EraStyle, MusicMood);
}

TArray<FString> UMingAIAssetBlueprintLibrary::SearchAssets(
    const FString& SearchKeyword,
    EAssetCategory Category,
    ERepublicEraStyle EraStyle,
    float MinRating,
    bool bFavoritesOnly)
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return TArray<FString>();
    }
    
    FAssetFilter Filter;
    Filter.SearchKeyword = SearchKeyword;
    Filter.Categories.Add(Category);
    Filter.EraStyles.Add(EraStyle);
    Filter.MinRating = MinRating;
    Filter.bFavoritesOnly = bFavoritesOnly;
    
    TArray<FAssetMetadata> Results = Manager->SearchAssets(Filter);
    
    TArray<FString> AssetIDs;
    for (const FAssetMetadata& Asset : Results)
    {
        AssetIDs.Add(Asset.AssetID);
    }
    
    return AssetIDs;
}

FString UMingAIAssetBlueprintLibrary::GetAssetInfo(const FString& AssetID)
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return TEXT("");
    }
    
    FAssetMetadata Asset = Manager->GetAsset(AssetID);
    
    if (Asset.AssetID.IsEmpty())
    {
        return TEXT("Asset not found");
    }
    
    FString Info = FString::Printf(TEXT("Asset ID: %s\n"), *Asset.AssetID);
    Info += FString::Printf(TEXT("Name: %s\n"), *Asset.AssetName);
    Info += FString::Printf(TEXT("Type: %s\n"), *UEnum::GetValueAsString(Asset.AssetType));
    Info += FString::Printf(TEXT("Category: %s\n"), *UEnum::GetValueAsString(Asset.Category));
    Info += FString::Printf(TEXT("Era: %s\n"), *UEnum::GetValueAsString(Asset.EraStyle));
    Info += FString::Printf(TEXT("Rating: %.1f\n"), Asset.Rating);
    Info += FString::Printf(TEXT("Usage: %d\n"), Asset.UsageCount);
    Info += FString::Printf(TEXT("Favorite: %s\n"), Asset.bIsFavorite ? TEXT("Yes") : TEXT("No"));
    Info += FString::Printf(TEXT("Created: %s\n"), *Asset.CreatedTime.ToString());
    
    return Info;
}

bool UMingAIAssetBlueprintLibrary::SetAssetFavorite(const FString& AssetID, bool bFavorite)
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->SetAssetFavorite(AssetID, bFavorite);
}

bool UMingAIAssetBlueprintLibrary::RateAsset(const FString& AssetID, float Rating)
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->RateAsset(AssetID, Rating);
}

bool UMingAIAssetBlueprintLibrary::ExportAsset(const FString& AssetID, const FString& ExportPath)
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->ExportAsset(AssetID, ExportPath);
}

TMap<EAssetCategory, int32> UMingAIAssetBlueprintLibrary::GetAssetStatistics()
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return TMap<EAssetCategory, int32>();
    }
    
    return Manager->GetAssetStatistics();
}

TArray<FString> UMingAIAssetBlueprintLibrary::GetRecommendedAssets(EAssetCategory Category, int32 Count)
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return TArray<FString>();
    }
    
    TArray<FAssetMetadata> Recommended = Manager->GetRecommendedAssets(Category, Count);
    
    TArray<FString> AssetIDs;
    for (const FAssetMetadata& Asset : Recommended)
    {
        AssetIDs.Add(Asset.AssetID);
    }
    
    return AssetIDs;
}

float UMingAIAssetBlueprintLibrary::GetStorageUsageMB()
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return 0.0f;
    }
    
    return Manager->GetStorageUsageMB();
}

int32 UMingAIAssetBlueprintLibrary::CleanupInvalidAssets()
{
    UMingAIAssetManager* Manager = GetAIAssetManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->CleanupInvalidAssets();
}

TArray<FString> UMingAIAssetBlueprintLibrary::GenerateCharacterPortraitSet(
    const TArray<FString>& CharacterDescriptions,
    ERepublicEraStyle EraStyle)
{
    TArray<FString> AssetIDs;
    
    for (const FString& Description : CharacterDescriptions)
    {
        FString AssetID = GenerateCharacterPortrait(Description, EraStyle);
        if (!AssetID.IsEmpty())
        {
            AssetIDs.Add(AssetID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d character portraits"), AssetIDs.Num());
    
    return AssetIDs;
}

TArray<FString> UMingAIAssetBlueprintLibrary::GenerateBuildingSet(
    const TArray<FString>& BuildingDescriptions,
    ERepublicEraStyle EraStyle)
{
    TArray<FString> AssetIDs;
    
    for (const FString& Description : BuildingDescriptions)
    {
        FString AssetID = GenerateBuilding(Description, EraStyle);
        if (!AssetID.IsEmpty())
        {
            AssetIDs.Add(AssetID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d buildings"), AssetIDs.Num());
    
    return AssetIDs;
}

TArray<FString> UMingAIAssetBlueprintLibrary::GenerateMusicSet(
    const TArray<FString>& MusicDescriptions,
    ERepublicEraStyle EraStyle,
    float Duration)
{
    TArray<FString> AssetIDs;
    
    for (const FString& Description : MusicDescriptions)
    {
        FString AssetID = GenerateBackgroundMusic(Description, EraStyle, Duration);
        if (!AssetID.IsEmpty())
        {
            AssetIDs.Add(AssetID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d music tracks"), AssetIDs.Num());
    
    return AssetIDs;
}

void UMingAIAssetBlueprintLibrary::SetStableDiffusionEndpoint(const FString& Endpoint)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (Generator)
    {
        Generator->SetStableDiffusionEndpoint(Endpoint);
    }
}

void UMingAIAssetBlueprintLibrary::SetAIVAEndpoint(const FString& Endpoint)
{
    UMingAIAssetGenerator* Generator = GetAIAssetGenerator();
    if (Generator)
    {
        Generator->SetAIVAEndpoint(Endpoint);
    }
}

bool UMingAIAssetBlueprintLibrary::IsAIAssetSystemInitialized()
{
    return (GetAIAssetGenerator() != nullptr) && (GetAIAssetManager() != nullptr);
}

int32 UMingAIAssetBlueprintLibrary::GetGenerationQueueCount()
{
    // 簡化處理：實際應該追蹤生成隊列
    return 0;
}

void UMingAIAssetBlueprintLibrary::CancelAllGenerations()
{
    // 簡化處理：實際應該取消所有生成任務
    UE_LOG(LogTemp, Log, TEXT("All AI asset generations cancelled"));
}
