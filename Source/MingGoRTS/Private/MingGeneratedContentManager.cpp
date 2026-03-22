#include "MingGeneratedContentManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Engine/Engine.h"

bool UMingGeneratedContentManager::LoadGeneratedContent(const FString& ContentPath)
{
    FString FullPath = FPaths::ProjectContentDir() + TEXT("MingGoRTS/Generated/") + ContentPath;
    
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FullPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Generated content not found: %s"), *FullPath);
        return false;
    }
    
    // Load JSON configuration
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load content config: %s"), *FullPath);
        return false;
    }
    
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded generated content: %s"), *ContentPath);
        return true;
    }
    
    return false;
}

TArray<FGeneratedAsset> UMingGeneratedContentManager::GetGeneratedAssets(const FString& AssetType)
{
    TArray<FGeneratedAsset> Assets;
    
    FString ContentDir = FPaths::ProjectContentDir() + TEXT("MingGoRTS/Generated/") + AssetType;
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    FFileStatData StatData;
    
    if (PlatformFile.GetStatData(*ContentDir, StatData))
    {
        // Iterate through directory and collect assets
        TArray<FString> FoundFiles;
        PlatformFile.FindFiles(FoundFiles, *ContentDir, TEXT(".json"));
        
        for (const FString& File : FoundFiles)
        {
            FGeneratedAsset Asset;
            Asset.AssetName = FPaths::GetBaseFilename(File);
            Asset.AssetType = AssetType;
            Asset.ConfigPath = FString::Printf(TEXT("MingGoRTS/Generated/%s/%s"), *AssetType, *File);
            
            // Load quality from JSON
            FString FullPath = ContentDir + "/" + File;
            FString JsonString;
            if (FFileHelper::LoadFileToString(JsonString, *FullPath))
            {
                TSharedPtr<FJsonObject> JsonObject;
                TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
                if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
                {
                    Asset.Quality = JsonObject->GetStringField(TEXT("Quality"));
                }
            }
            
            Assets.Add(Asset);
        }
    }
    
    return Assets;
}

void UMingGeneratedContentManager::InitializeGeneratedContent()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingGoRTS Generated Content System..."));
    
    // Load all content types
    TArray<FString> ContentTypes = {TEXT("Scenes"), TEXT("Images"), TEXT("Videos"), TEXT("Music"), TEXT("Sounds")};
    
    for (const FString& ContentType : ContentTypes)
    {
        TArray<FGeneratedAsset> Assets = GetGeneratedAssets(ContentType);
        UE_LOG(LogTemp, Log, TEXT("Loaded %d %s assets"), Assets.Num(), *ContentType);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated Content System initialized successfully!"));
}
