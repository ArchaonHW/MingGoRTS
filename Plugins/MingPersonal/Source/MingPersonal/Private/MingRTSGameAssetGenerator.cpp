#include "MingRTSGameAssetGenerator.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/GameInstance.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimSequence.h"
#include "PaperSprite.h"
#include "ImageUtils.h"
#include "KismetProceduralMeshLibrary.h"

UMingRTSGameAssetGenerator::UMingRTSGameAssetGenerator()
{
    CurrentWorld = nullptr;
    AIContentSystem = nullptr;
    ContentRenderer = nullptr;
    ContentManager = nullptr;
    
    MaxConcurrentGenerations = 5;
    GenerationTimeout = 300.0f;
    bEnableAutoBackup = true;
    bEnableCompression = true;
    DefaultAssetPath = TEXT("/Game/GeneratedAssets/");
    
    SupportedFormats = {TEXT("PNG"), TEXT("JPG"), TEXT("EXR"), TEXT("MP4"), TEXT("AVI"), TEXT("MP3"), TEXT("WAV"), TEXT("OGG"), TEXT("FBX"), TEXT("OBJ"), TEXT("GLTF")};
    
    TotalGeneratedAssets = 0;
    TotalGenerationTime = 0.0f;
}

void UMingRTSGameAssetGenerator::InitializeGameAssetGenerator(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("MingRTSGameAssetGenerator: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    
    // 獲取AI內容系統引用
    // 注意：在實際實現中，這些系統應該從遊戲實例或管理器中獲取
    // 這裡只是示例代碼
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSGameAssetGenerator initialized successfully"));
}

FString UMingRTSGameAssetGenerator::GenerateGameAsset(const FGameAssetRequirement& Requirement)
{
    // 驗證資產需求
    if (!ValidateAssetRequirement(Requirement))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid asset requirement"));
        return TEXT("");
    }
    
    // 生成唯一資產ID
    FString AssetID = GenerateUniqueAssetID();
    
    // 存儲生成任務
    ActiveGenerations.Add(AssetID, Requirement);
    
    UE_LOG(LogTemp, Log, TEXT("Started game asset generation: %s (%s)"), *Requirement.AssetName, *AssetID);
    
    // 開始處理資產生成
    FGeneratedGameAsset Asset = ProcessAssetGeneration(Requirement);
    Asset.AssetID = AssetID;
    
    // 存儲生成的資產
    GeneratedAssets.Add(AssetID, Asset);
    
    // 更新統計數據
    UpdateStatistics(Requirement.AssetCategory, Asset.GenerationTime);
    
    // 觸發資產生成完成事件
    OnGameAssetGenerated.Broadcast(AssetID, Asset);
    
    // 清理活動生成任務
    ActiveGenerations.Remove(AssetID);
    
    return AssetID;
}

FString UMingRTSGameAssetGenerator::GenerateGameAssetsBatch(const TArray<FGameAssetRequirement>& Requirements)
{
    FString RequestID = GenerateUniqueAssetID();
    
    UE_LOG(LogTemp, Log, TEXT("Started batch game asset generation: %d assets (%s)"), Requirements.Num(), *RequestID);
    
    // 處理批量生成
    ProcessBatchGeneration(RequestID, Requirements);
    
    return RequestID;
}

FString UMingRTSGameAssetGenerator::GenerateGameScene(const FString& SceneName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::GameScene;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = SceneName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Photorealistic;
    Requirement.Theme = TEXT("Ming Dynasty RTS Game Scene");
    Requirement.Context = TEXT("In-game scene for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Scenes/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateCharacterAsset(const FString& CharacterName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Character;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = CharacterName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Photorealistic;
    Requirement.Theme = TEXT("Ming Dynasty Character");
    Requirement.Context = TEXT("Character for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Characters/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateBuildingAsset(const FString& BuildingName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Building;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = BuildingName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Photorealistic;
    Requirement.Theme = TEXT("Ming Dynasty Architecture");
    Requirement.Context = TEXT("Building for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Buildings/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateEnvironmentAsset(const FString& EnvironmentName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Environment;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = EnvironmentName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Photorealistic;
    Requirement.Theme = TEXT("Ming Dynasty Environment");
    Requirement.Context = TEXT("Environment for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Environments/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateUIAsset(const FString& UIName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::UI;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = UIName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Modern;
    Requirement.Theme = TEXT("Ming Dynasty UI");
    Requirement.Context = TEXT("UI element for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("UI/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateVideoAsset(const FString& VideoName, const FString& Description, float Duration)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Video;
    Requirement.AssetPurpose = EGameAssetPurpose::Story;
    Requirement.AssetName = VideoName;
    Requirement.Description = Description;
    Requirement.Quality = EQualityLevel::High;
    Requirement.Style = EArtStyle::Cinematic;
    Requirement.Theme = TEXT("Ming Dynasty Cinematic");
    Requirement.Context = TEXT("Cinematic video for MingGoRTS");
    Requirement.CustomParameters.Add(TEXT("Duration"), FString::SanitizeFloat(Duration));
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Videos/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateMusicAsset(const FString& MusicName, const FString& Description, float Duration)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Music;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = MusicName;
    Requirement.Description = Description;
    Requirement.Quality = EQualityLevel::High;
    Requirement.Style = EArtStyle::Traditional;
    Requirement.Theme = TEXT("Ming Dynasty Music");
    Requirement.Context = TEXT("Background music for MingGoRTS");
    Requirement.CustomParameters.Add(TEXT("Duration"), FString::SanitizeFloat(Duration));
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Music/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateSoundEffectAsset(const FString& SoundName, const FString& Description)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::SoundEffect;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = SoundName;
    Requirement.Description = Description;
    Requirement.Quality = EQualityLevel::High;
    Requirement.Style = EArtStyle::Traditional;
    Requirement.Theme = TEXT("Ming Dynasty Sound Effects");
    Requirement.Context = TEXT("Sound effect for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("SoundEffects/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateTextureAsset(const FString& TextureName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Texture;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = TextureName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Photorealistic;
    Requirement.Theme = TEXT("Ming Dynasty Texture");
    Requirement.Context = TEXT("Texture for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Textures/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateModelAsset(const FString& ModelName, const FString& Description, EQualityLevel Quality)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Model;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = ModelName;
    Requirement.Description = Description;
    Requirement.Quality = Quality;
    Requirement.Style = EArtStyle::Photorealistic;
    Requirement.Theme = TEXT("Ming Dynasty 3D Model");
    Requirement.Context = TEXT("3D model for MingGoRTS");
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Models/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateAnimationAsset(const FString& AnimationName, const FString& Description, float Duration)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Animation;
    Requirement.AssetPurpose = EGameAssetPurpose::Gameplay;
    Requirement.AssetName = AnimationName;
    Requirement.Description = Description;
    Requirement.Quality = EQualityLevel::High;
    Requirement.Style = EArtStyle::Realistic;
    Requirement.Theme = TEXT("Ming Dynasty Animation");
    Requirement.Context = TEXT("Animation for MingGoRTS");
    Requirement.CustomParameters.Add(TEXT("Duration"), FString::SanitizeFloat(Duration));
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Animations/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateTutorialAsset(const FString& TutorialName, const FString& Description, bool bInteractive)
{
    FGameAssetRequirement Requirement;
    Requirement.AssetCategory = EGameAssetCategory::Tutorial;
    Requirement.AssetPurpose = EGameAssetPurpose::Tutorial;
    Requirement.AssetName = TutorialName;
    Requirement.Description = Description;
    Requirement.Quality = EQualityLevel::Standard;
    Requirement.Style = EArtStyle::Modern;
    Requirement.Theme = TEXT("MingGoRTS Tutorial");
    Requirement.Context = TEXT("Tutorial content for MingGoRTS");
    Requirement.CustomParameters.Add(TEXT("Interactive"), bInteractive ? TEXT("true") : TEXT("false"));
    Requirement.TargetPath = FPaths::Combine(DefaultAssetPath, TEXT("Tutorials/"));
    
    return GenerateGameAsset(Requirement);
}

FString UMingRTSGameAssetGenerator::GenerateUniversityGuide(const FUniversityGuideConfig& Config)
{
    FString GuideID = GenerateUniqueAssetID();
    
    UE_LOG(LogTemp, Log, TEXT("Started university guide generation: %s (%s)"), *Config.UniversityName, *GuideID);
    
    // 生成高校引導內容
    FString GuideContent = GenerateUniversityGuideContent(Config);
    
    // 創建生成的資產
    FGeneratedGameAsset GuideAsset;
    GuideAsset.AssetID = GuideID;
    GuideAsset.AssetName = FString::Printf(TEXT("%s_%s_Guide"), *Config.UniversityName, *GetUniversityContentTypeName(Config.ContentType));
    GuideAsset.AssetCategory = EGameAssetCategory::UniversityGuide;
    GuideAsset.AssetPurpose = EGameAssetPurpose::Education;
    GuideAsset.AssetPath = FPaths::Combine(DefaultAssetPath, TEXT("UniversityGuides/"));
    GuideAsset.Description = FString::Printf(TEXT("University guide for %s - %s"), *Config.UniversityName, *GetUniversityContentTypeName(Config.ContentType));
    GuideAsset.Quality = EQualityLevel::High;
    GuideAsset.Style = EArtStyle::Modern;
    GuideAsset.GeneratedAt = FDateTime::Now().ToString();
    GuideAsset.GenerationTime = 30.0f; // 預估時間
    
    // 添加標籤
    GuideAsset.Tags.Add(Config.UniversityName);
    GuideAsset.Tags.Add(GetUniversityContentTypeName(Config.ContentType));
    GuideAsset.Tags.Add(Config.Department);
    
    // 創建元數據
    GuideAsset.Metadata.Add(TEXT("UniversityName"), Config.UniversityName);
    GuideAsset.Metadata.Add(TEXT("Department"), Config.Department);
    GuideAsset.Metadata.Add(TEXT("ContentType"), GetUniversityContentTypeName(Config.ContentType));
    GuideAsset.Metadata.Add(TEXT("TargetAudience"), Config.TargetAudience);
    GuideAsset.Metadata.Add(TEXT("Duration"), Config.Duration);
    GuideAsset.Metadata.Add(TEXT("Language"), Config.Language);
    GuideAsset.Metadata.Add(TEXT("CulturalContext"), Config.CulturalContext);
    GuideAsset.Metadata.Add(TEXT("Interactive"), Config.bInteractive ? TEXT("true") : TEXT("false"));
    GuideAsset.Metadata.Add(TEXT("IncludeHistoricalContent"), Config.bIncludeHistoricalContent ? TEXT("true") : TEXT("false"));
    GuideAsset.Metadata.Add(TEXT("IncludeStudentTestimonials"), Config.bIncludeStudentTestimonials ? TEXT("true") : TEXT("false"));
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*GuideContent);
    GuideAsset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    // 存儲生成的資產
    GeneratedAssets.Add(GuideID, GuideAsset);
    
    // 更新統計數據
    UpdateStatistics(EGameAssetCategory::UniversityGuide, GuideAsset.GenerationTime);
    
    // 觸發高校引導生成完成事件
    OnUniversityGuideGenerated.Broadcast(GuideID, GuideAsset);
    
    return GuideID;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GetGeneratedAsset(const FString& AssetID) const
{
    if (!IsValidAssetID(AssetID))
    {
        return FGeneratedGameAsset();
    }
    
    const FGeneratedGameAsset* Asset = GeneratedAssets.Find(AssetID);
    return Asset ? *Asset : FGeneratedGameAsset();
}

TArray<FGeneratedGameAsset> UMingRTSGameAssetGenerator::GetAllGeneratedAssets() const
{
    TArray<FGeneratedGameAsset> AllAssets;
    
    for (const TPair<FString, FGeneratedGameAsset>& Pair : GeneratedAssets)
    {
        AllAssets.Add(Pair.Value);
    }
    
    return AllAssets;
}

TArray<FGeneratedGameAsset> UMingRTSGameAssetGenerator::GetAssetsByCategory(EGameAssetCategory Category) const
{
    TArray<FGeneratedGameAsset> CategoryAssets;
    
    for (const TPair<FString, FGeneratedGameAsset>& Pair : GeneratedAssets)
    {
        if (Pair.Value.AssetCategory == Category)
        {
            CategoryAssets.Add(Pair.Value);
        }
    }
    
    return CategoryAssets;
}

TArray<FGeneratedGameAsset> UMingRTSGameAssetGenerator::GetAssetsByPurpose(EGameAssetPurpose Purpose) const
{
    TArray<FGeneratedGameAsset> PurposeAssets;
    
    for (const TPair<FString, FGeneratedGameAsset>& Pair : GeneratedAssets)
    {
        if (Pair.Value.AssetPurpose == Purpose)
        {
            PurposeAssets.Add(Pair.Value);
        }
    }
    
    return PurposeAssets;
}

bool UMingRTSGameAssetGenerator::ExportAsset(const FString& AssetID, const FString& ExportPath)
{
    if (!IsValidAssetID(AssetID))
    {
        return false;
    }
    
    const FGeneratedGameAsset* Asset = GeneratedAssets.Find(AssetID);
    if (!Asset)
    {
        return false;
    }
    
    // 創建導出目錄
    FString FullPath = FPaths::Combine(ExportPath, Asset->AssetName);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    if (!PlatformFile.DirectoryExists(*FullPath))
    {
        PlatformFile.CreateDirectoryTree(*FullPath);
    }
    
    // 導出資產數據
    FString AssetFilePath = FPaths::Combine(FullPath, Asset->AssetName);
    FFileHelper::SaveArrayToFile(Asset->AssetData, *AssetFilePath);
    
    // 導出元數據
    FString MetadataPath = FPaths::Combine(FullPath, TEXT("metadata.json"));
    FString MetadataContent = GenerateAssetMetadataJSON(*Asset);
    FFileHelper::SaveStringToFile(MetadataContent, *MetadataPath);
    
    UE_LOG(LogTemp, Log, TEXT("Exported asset: %s to %s"), *AssetID, *FullPath);
    return true;
}

bool UMingRTSGameAssetGenerator::ExportAssetsBatch(const TArray<FString>& AssetIDs, const FString& ExportPath)
{
    bool bAllSuccess = true;
    
    for (const FString& AssetID : AssetIDs)
    {
        if (!ExportAsset(AssetID, ExportPath))
        {
            bAllSuccess = false;
        }
    }
    
    return bAllSuccess;
}

bool UMingRTSGameAssetGenerator::DeleteAsset(const FString& AssetID)
{
    if (!IsValidAssetID(AssetID))
    {
        return false;
    }
    
    if (GeneratedAssets.Remove(AssetID) > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Deleted asset: %s"), *AssetID);
        return true;
    }
    
    return false;
}

void UMingRTSGameAssetGenerator::ClearAllAssets()
{
    int32 AssetCount = GeneratedAssets.Num();
    GeneratedAssets.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all assets (%d assets removed)"), AssetCount);
}

TMap<EGameAssetCategory, int32> UMingRTSGameAssetGenerator::GetAssetStatistics() const
{
    return AssetCounts;
}

TMap<FString, float> UMingRTSGameAssetGenerator::GetGenerationStatistics() const
{
    TMap<FString, float> Statistics;
    
    Statistics.Add(TEXT("TotalGeneratedAssets"), TotalGeneratedAssets);
    Statistics.Add(TEXT("TotalGenerationTime"), TotalGenerationTime);
    Statistics.Add(TEXT("AverageGenerationTime"), TotalGeneratedAssets > 0 ? TotalGenerationTime / TotalGeneratedAssets : 0.0f);
    Statistics.Add(TEXT("ActiveGenerations"), ActiveGenerations.Num());
    
    return Statistics;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::ProcessAssetGeneration(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    
    float StartTime = CurrentWorld->GetTimeSeconds();
    
    // 根據資產類別生成不同的內容
    switch (Requirement.AssetCategory)
    {
        case EGameAssetCategory::GameScene:
            Asset = GenerateGameSceneAsset(Requirement);
            break;
        case EGameAssetCategory::Character:
            Asset = GenerateCharacterAsset(Requirement);
            break;
        case EGameAssetCategory::Building:
            Asset = GenerateBuildingAsset(Requirement);
            break;
        case EGameAssetCategory::Environment:
            Asset = GenerateEnvironmentAsset(Requirement);
            break;
        case EGameAssetCategory::UI:
            Asset = GenerateUIAsset(Requirement);
            break;
        case EGameAssetCategory::Video:
            Asset = GenerateVideoAsset(Requirement);
            break;
        case EGameAssetCategory::Music:
            Asset = GenerateMusicAsset(Requirement);
            break;
        case EGameAssetCategory::SoundEffect:
            Asset = GenerateSoundEffectAsset(Requirement);
            break;
        case EGameAssetCategory::Texture:
            Asset = GenerateTextureAsset(Requirement);
            break;
        case EGameAssetCategory::Model:
            Asset = GenerateModelAsset(Requirement);
            break;
        case EGameAssetCategory::Animation:
            Asset = GenerateAnimationAsset(Requirement);
            break;
        case EGameAssetCategory::Tutorial:
            Asset = GenerateTutorialAsset(Requirement);
            break;
        default:
            Asset = GenerateCustomAsset(Requirement);
            break;
    }
    
    // 設置基本屬性
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetCategory = Requirement.AssetCategory;
    Asset.AssetPurpose = Requirement.AssetPurpose;
    Asset.Description = Requirement.Description;
    Asset.Quality = Requirement.Quality;
    Asset.Style = Requirement.Style;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.GeneratedAt = FDateTime::Now().ToString();
    Asset.GenerationTime = CurrentWorld->GetTimeSeconds() - StartTime;
    Asset.Metadata = CreateAssetMetadata(Requirement);
    
    return Asset;
}

FString UMingRTSGameAssetGenerator::GenerateGameSceneContent(const FGameAssetRequirement& Requirement)
{
    // 生成遊戲場景內容
    FString SceneContent = FString::Printf(TEXT("Game Scene: %s\n"), *Requirement.AssetName);
    SceneContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    SceneContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    SceneContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    SceneContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    SceneContent += FString::Printf(TEXT("Context: %s\n"), *Requirement.Context);
    
    // 添加場景特定內容
    SceneContent += TEXT("\nScene Elements:\n");
    SceneContent += TEXT("- Environment: Ming Dynasty era architecture and landscape\n");
    SceneContent += TEXT("- Lighting: Dynamic day/night cycle\n");
    SceneContent += TEXT("- Atmosphere: Historical Chinese ambiance\n");
    SceneContent += TEXT("- Interactive Elements: Buildings, units, resources\n");
    
    return SceneContent;
}

FString UMingRTSGameAssetGenerator::GenerateCharacterContent(const FGameAssetRequirement& Requirement)
{
    // 生成角色內容
    FString CharacterContent = FString::Printf(TEXT("Character: %s\n"), *Requirement.AssetName);
    CharacterContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    CharacterContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    CharacterContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    CharacterContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加角色特定內容
    CharacterContent += TEXT("\nCharacter Features:\n");
    CharacterContent += TEXT("- Appearance: Ming Dynasty period clothing\n");
    CharacterContent += TEXT("- Animation: Authentic Chinese martial arts movements\n");
    CharacterContent += TEXT("- Voice: Traditional Chinese language\n");
    CharacterContent += TEXT("- Personality: Historical character traits\n");
    
    return CharacterContent;
}

FString UMingRTSGameAssetGenerator::GenerateBuildingContent(const FGameAssetRequirement& Requirement)
{
    // 生成建築內容
    FString BuildingContent = FString::Printf(TEXT("Building: %s\n"), *Requirement.AssetName);
    BuildingContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    BuildingContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    BuildingContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    BuildingContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加建築特定內容
    BuildingContent += TEXT("\nBuilding Features:\n");
    BuildingContent += TEXT("- Architecture: Ming Dynasty style\n");
    BuildingContent += TEXT("- Materials: Traditional Chinese building materials\n");
    BuildingContent += TEXT("- Function: Based on historical usage\n");
    BuildingContent += TEXT("- Details: Authentic decorative elements\n");
    
    return BuildingContent;
}

FString UMingRTSGameAssetGenerator::GenerateEnvironmentContent(const FGameAssetRequirement& Requirement)
{
    // 生成環境內容
    FString EnvironmentContent = FString::Printf(TEXT("Environment: %s\n"), *Requirement.AssetName);
    EnvironmentContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    EnvironmentContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    EnvironmentContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    EnvironmentContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加環境特定內容
    EnvironmentContent += TEXT("\nEnvironment Features:\n");
    EnvironmentContent += TEXT("- Landscape: Ming Dynasty era geography\n");
    EnvironmentContent += TEXT("- Vegetation: Traditional Chinese flora\n");
    EnvironmentContent += TEXT("- Weather: Dynamic weather system\n");
    EnvironmentContent += TEXT("- Wildlife: Authentic Chinese fauna\n");
    
    return EnvironmentContent;
}

FString UMingRTSGameAssetGenerator::GenerateUIContent(const FGameAssetRequirement& Requirement)
{
    // 生成UI內容
    FString UIContent = FString::Printf(TEXT("UI Element: %s\n"), *Requirement.AssetName);
    UIContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    UIContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    UIContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    UIContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加UI特定內容
    UIContent += TEXT("\nUI Features:\n");
    UIContent += TEXT("- Design: Ming Dynasty inspired interface\n");
    UIContent += TEXT("- Colors: Traditional Chinese color palette\n");
    UIContent += TEXT("- Typography: Chinese calligraphy fonts\n");
    UIContent += TEXT("- Icons: Traditional Chinese symbols\n");
    
    return UIContent;
}

FString UMingRTSGameAssetGenerator::GenerateVideoContent(const FGameAssetRequirement& Requirement)
{
    // 生成影片內容
    FString VideoContent = FString::Printf(TEXT("Video: %s\n"), *Requirement.AssetName);
    VideoContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    VideoContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    VideoContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    VideoContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 獲取持續時間
    FString DurationStr = Requirement.CustomParameters.FindRef(TEXT("Duration"));
    
    // 添加影片特定內容
    VideoContent += FString::Printf(TEXT("\nVideo Features:\n"));
    VideoContent += FString::Printf(TEXT("- Duration: %s\n"), *DurationStr);
    VideoContent += TEXT("- Resolution: 4K Ultra HD\n");
    VideoContent += TEXT("- Frame Rate: 60 FPS\n");
    VideoContent += TEXT("- Format: MP4 with H.264 encoding\n");
    VideoContent += TEXT("- Audio: Dolby Digital 5.1 surround sound\n");
    
    return VideoContent;
}

FString UMingRTSGameAssetGenerator::GenerateMusicContent(const FGameAssetRequirement& Requirement)
{
    // 生成音樂內容
    FString MusicContent = FString::Printf(TEXT("Music: %s\n"), *Requirement.AssetName);
    MusicContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    MusicContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    MusicContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    MusicContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 獲取持續時間
    FString DurationStr = Requirement.CustomParameters.FindRef(TEXT("Duration"));
    
    // 添加音樂特定內容
    MusicContent += FString::Printf(TEXT("\nMusic Features:\n"));
    MusicContent += FString::Printf(TEXT("- Duration: %s\n"), *DurationStr);
    MusicContent += TEXT("- Genre: Traditional Chinese orchestral\n");
    MusicContent += TEXT("- Instruments: Guqin, Pipa, Erhu, Dizi\n");
    MusicContent += TEXT("- Format: MP3 320kbps\n");
    MusicContent += TEXT("- Mood: Historical and epic\n");
    
    return MusicContent;
}

FString UMingRTSGameAssetGenerator::GenerateSoundEffectContent(const FGameAssetRequirement& Requirement)
{
    // 生成音效內容
    FString SoundEffectContent = FString::Printf(TEXT("Sound Effect: %s\n"), *Requirement.AssetName);
    SoundEffectContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    SoundEffectContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    SoundEffectContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    SoundEffectContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加音效特定內容
    SoundEffectContent += TEXT("\nSound Effect Features:\n");
    SoundEffectContent += TEXT("- Type: Authentic Chinese sound\n");
    SoundEffectContent += TEXT("- Format: WAV 24-bit 96kHz\n");
    SoundEffectContent += TEXT("- Processing: Minimal digital effects\n");
    SoundEffectContent += TEXT("- Source: Field recordings and synthesis\n");
    
    return SoundEffectContent;
}

FString UMingRTSGameAssetGenerator::GenerateTextureContent(const FGameAssetRequirement& Requirement)
{
    // 生成紋理內容
    FString TextureContent = FString::Printf(TEXT("Texture: %s\n"), *Requirement.AssetName);
    TextureContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    TextureContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    TextureContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    TextureContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加紋理特定內容
    TextureContent += TEXT("\nTexture Features:\n");
    TextureContent += TEXT("- Resolution: 4096x4096 pixels\n");
    TextureContent += TEXT("- Format: PNG with alpha channel\n");
    TextureContent += TEXT("- Color Space: sRGB\n");
    TextureContent += TEXT("- Compression: High quality\n");
    
    return TextureContent;
}

FString UMingRTSGameAssetGenerator::GenerateModelContent(const FGameAssetRequirement& Requirement)
{
    // 生成3D模型內容
    FString ModelContent = FString::Printf(TEXT("3D Model: %s\n"), *Requirement.AssetName);
    ModelContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    ModelContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    ModelContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    ModelContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 添加3D模型特定內容
    ModelContent += TEXT("\n3D Model Features:\n");
    ModelContent += TEXT("- Polygons: High-polygon count\n");
    ModelContent += TEXT("- Textures: 4K PBR materials\n");
    ModelContent += TEXT("- Rigging: Full skeletal rig\n");
    ModelContent += TEXT("- Format: FBX compatible\n");
    
    return ModelContent;
}

FString UMingRTSGameAssetGenerator::GenerateAnimationContent(const FGameAssetRequirement& Requirement)
{
    // 生成動畫內容
    FString AnimationContent = FString::Printf(TEXT("Animation: %s\n"), *Requirement.AssetName);
    AnimationContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    AnimationContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    AnimationContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    AnimationContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 獲取持續時間
    FString DurationStr = Requirement.CustomParameters.FindRef(TEXT("Duration"));
    
    // 添加動畫特定內容
    AnimationContent += FString::Printf(TEXT("\nAnimation Features:\n"));
    AnimationContent += FString::Printf(TEXT("- Duration: %s\n"), *DurationStr);
    AnimationContent += TEXT("- Frame Rate: 30 FPS\n");
    AnimationContent += TEXT("- Format: FBX animation\n");
    AnimationContent += TEXT("- Quality: Motion capture enhanced\n");
    
    return AnimationContent;
}

FString UMingRTSGameAssetGenerator::GenerateTutorialContent(const FGameAssetRequirement& Requirement)
{
    // 生成教學內容
    FString TutorialContent = FString::Printf(TEXT("Tutorial: %s\n"), *Requirement.AssetName);
    TutorialContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    TutorialContent += FString::Printf(TEXT("Theme: %s\n"), *Requirement.Theme);
    TutorialContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    TutorialContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    
    // 獲取交互性
    FString InteractiveStr = Requirement.CustomParameters.FindRef(TEXT("Interactive"));
    
    // 添加教學特定內容
    TutorialContent += FString::Printf(TEXT("\nTutorial Features:\n"));
    TutorialContent += FString::Printf(TEXT("- Interactive: %s\n"), *InteractiveStr);
    TutorialContent += TEXT("- Language: Chinese\n");
    TutorialContent += TEXT("- Format: Interactive HTML5\n");
    TutorialContent += TEXT("- Assessment: Built-in quizzes\n");
    
    return TutorialContent;
}

FString UMingRTSGameAssetGenerator::GenerateUniversityGuideContent(const FUniversityGuideConfig& Config)
{
    FString GuideContent;
    
    // 添加標題和基本信息
    GuideContent += FString::Printf(TEXT("# %s - %s\n\n"), *Config.UniversityName, *GetUniversityContentTypeName(Config.ContentType));
    GuideContent += FString::Printf(TEXT("**Department:** %s\n"), *Config.Department);
    GuideContent += FString::Printf(TEXT("**Target Audience:** %s\n"), *Config.TargetAudience);
    GuideContent += FString::Printf(TEXT("**Duration:** %s\n"), *Config.Duration);
    GuideContent += FString::Printf(TEXT("**Language:** %s\n"), *Config.Language);
    GuideContent += FString::Printf(TEXT("**Cultural Context:** %s\n"), *Config.CulturalContext);
    GuideContent += FString::Printf(TEXT("**Interactive:** %s\n"), Config.bInteractive ? TEXT("Yes") : TEXT("No"));
    GuideContent += FString::Printf(TEXT("**Include Historical Content:** %s\n"), Config.bIncludeHistoricalContent ? TEXT("Yes") : TEXT("No"));
    GuideContent += FString::Printf(TEXT("**Include Student Testimonials:** %s\n\n"), Config.bIncludeStudentTestimonials ? TEXT("Yes") : TEXT("No"));
    
    // 根據內容類型生成特定內容
    switch (Config.ContentType)
    {
        case EUniversityContentType::CampusTour:
            GuideContent += GenerateCampusTourContent(Config);
            break;
        case EUniversityContentType::AcademicIntro:
            GuideContent += GenerateAcademicIntroContent(Config);
            break;
        case EUniversityContentType::StudentLife:
            GuideContent += GenerateStudentLifeContent(Config);
            break;
        case EUniversityContentType::ResearchShowcase:
            GuideContent += GenerateResearchShowcaseContent(Config);
            break;
        case EUniversityContentType::CareerGuidance:
            GuideContent += GenerateCareerGuidanceContent(Config);
            break;
        case EUniversityContentType::HistoricalContent:
            GuideContent += GenerateHistoricalContentContent(Config);
            break;
        case EUniversityContentType::CulturalContent:
            GuideContent += GenerateCulturalContentContent(Config);
            break;
        case EUniversityContentType::TechnicalDemo:
            GuideContent += GenerateTechnicalDemoContent(Config);
            break;
        case EUniversityContentType::InteractiveGuide:
            GuideContent += GenerateInteractiveGuideContent(Config);
            break;
        default:
            GuideContent += GenerateCustomUniversityContent(Config);
            break;
    }
    
    // 添加結尾
    GuideContent += TEXT("\n---\n");
    GuideContent += TEXT("*This guide was generated using AI content generation technology.*\n");
    GuideContent += FString::Printf(TEXT("*Generated on: %s*\n"), *FDateTime::Now().ToString());
    
    return GuideContent;
}

bool UMingRTSGameAssetGenerator::ValidateAssetRequirement(const FGameAssetRequirement& Requirement) const
{
    if (Requirement.AssetName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Asset name cannot be empty"));
        return false;
    }
    
    if (Requirement.Description.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Asset description cannot be empty"));
        return false;
    }
    
    if (Requirement.TargetPath.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Asset target path cannot be empty"));
        return false;
    }
    
    return true;
}

void UMingRTSGameAssetGenerator::HandleAssetCompleted(const FString& AssetID, const FGeneratedGameAsset& Asset)
{
    UE_LOG(LogTemp, Log, TEXT("Asset generation completed: %s"), *AssetID);
    
    // 觸發事件
    OnGameAssetGenerated.Broadcast(AssetID, Asset);
}

void UMingRTSGameAssetGenerator::HandleAssetError(const FString& AssetID, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Asset generation error: %s - %s"), *AssetID, *ErrorMessage);
    
    // 清理失敗的生成任務
    ActiveGenerations.Remove(AssetID);
}

void UMingRTSGameAssetGenerator::UpdateGenerationProgress(const FString& AssetID, int32 Progress)
{
    // 觸發進度更新事件
    OnGameAssetGenerationProgress.Broadcast(AssetID, Progress);
}

FString UMingRTSGameAssetGenerator::GenerateUniqueAssetID() const
{
    return FString::Printf(TEXT("Asset_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingRTSGameAssetGenerator::GetAssetCategoryName(EGameAssetCategory Category) const
{
    switch (Category)
    {
        case EGameAssetCategory::GameScene: return TEXT("GameScene");
        case EGameAssetCategory::Character: return TEXT("Character");
        case EGameAssetCategory::Building: return TEXT("Building");
        case EGameAssetCategory::Environment: return TEXT("Environment");
        case EGameAssetCategory::UI: return TEXT("UI");
        case EGameAssetCategory::Video: return TEXT("Video");
        case EGameAssetCategory::Music: return TEXT("Music");
        case EGameAssetCategory::SoundEffect: return TEXT("SoundEffect");
        case EGameAssetCategory::Texture: return TEXT("Texture");
        case EGameAssetCategory::Model: return TEXT("Model");
        case EGameAssetCategory::Animation: return TEXT("Animation");
        case EGameAssetCategory::Tutorial: return TEXT("Tutorial");
        case EGameAssetCategory::UniversityGuide: return TEXT("UniversityGuide");
        default: return TEXT("Custom");
    }
}

FString UMingRTSGameAssetGenerator::GetAssetPurposeName(EGameAssetPurpose Purpose) const
{
    switch (Purpose)
    {
        case EGameAssetPurpose::Gameplay: return TEXT("Gameplay");
        case EGameAssetPurpose::Story: return TEXT("Story");
        case EGameAssetPurpose::Tutorial: return TEXT("Tutorial");
        case EGameAssetPurpose::Marketing: return TEXT("Marketing");
        case EGameAssetPurpose::Education: return TEXT("Education");
        case EGameAssetPurpose::University: return TEXT("University");
        case EGameAssetPurpose::Presentation: return TEXT("Presentation");
        case EGameAssetPurpose::Documentation: return TEXT("Documentation");
        default: return TEXT("Custom");
    }
}

FString UMingRTSGameAssetGenerator::GetUniversityContentTypeName(EUniversityContentType ContentType) const
{
    switch (ContentType)
    {
        case EUniversityContentType::CampusTour: return TEXT("CampusTour");
        case EUniversityContentType::AcademicIntro: return TEXT("AcademicIntro");
        case EUniversityContentType::StudentLife: return TEXT("StudentLife");
        case EUniversityContentType::ResearchShowcase: return TEXT("ResearchShowcase");
        case EUniversityContentType::CareerGuidance: return TEXT("CareerGuidance");
        case EUniversityContentType::HistoricalContent: return TEXT("HistoricalContent");
        case EUniversityContentType::CulturalContent: return TEXT("CulturalContent");
        case EUniversityContentType::TechnicalDemo: return TEXT("TechnicalDemo");
        case EUniversityContentType::InteractiveGuide: return TEXT("InteractiveGuide");
        default: return TEXT("Custom");
    }
}

TMap<FString, FString> UMingRTSGameAssetGenerator::CreateAssetMetadata(const FGameAssetRequirement& Requirement) const
{
    TMap<FString, FString> Metadata;
    
    Metadata.Add(TEXT("AssetName"), Requirement.AssetName);
    Metadata.Add(TEXT("Description"), Requirement.Description);
    Metadata.Add(TEXT("Category"), GetAssetCategoryName(Requirement.AssetCategory));
    Metadata.Add(TEXT("Purpose"), GetAssetPurposeName(Requirement.AssetPurpose));
    Metadata.Add(TEXT("Quality"), GetQualityLevelName(Requirement.Quality));
    Metadata.Add(TEXT("Style"), GetAssetStyleName(Requirement.Style));
    Metadata.Add(TEXT("Theme"), Requirement.Theme);
    Metadata.Add(TEXT("Context"), Requirement.Context);
    Metadata.Add(TEXT("IsUrgent"), Requirement.bIsUrgent ? TEXT("true") : TEXT("false"));
    Metadata.Add(TEXT("TargetPath"), Requirement.TargetPath);
    Metadata.Add(TEXT("GeneratedAt"), FDateTime::Now().ToString());
    
    return Metadata;
}

float UMingRTSGameAssetGenerator::CalculateEstimatedGenerationTime(const FGameAssetRequirement& Requirement) const
{
    float BaseTime = 10.0f; // 基礎時間（秒）
    
    // 根據質量級別調整時間
    switch (Requirement.Quality)
    {
        case EQualityLevel::Draft:
            BaseTime *= 0.5f;
            break;
        case EQualityLevel::Standard:
            BaseTime *= 1.0f;
            break;
        case EQualityLevel::High:
            BaseTime *= 2.0f;
            break;
        case EQualityLevel::Ultra:
            BaseTime *= 4.0f;
            break;
        case EQualityLevel::Cinematic:
            BaseTime *= 8.0f;
            break;
        default:
            break;
    }
    
    // 根據資產類別調整時間
    switch (Requirement.AssetCategory)
    {
        case EGameAssetCategory::GameScene:
            BaseTime *= 3.0f;
            break;
        case EGameAssetCategory::Video:
            BaseTime *= 5.0f;
            break;
        case EGameAssetCategory::Music:
            BaseTime *= 2.5f;
            break;
        case EGameAssetCategory::Model:
            BaseTime *= 2.0f;
            break;
        case EGameAssetCategory::Animation:
            BaseTime *= 1.8f;
            break;
        case EGameAssetCategory::UniversityGuide:
            BaseTime *= 1.5f;
            break;
        default:
            break;
    }
    
    return BaseTime;
}

void UMingRTSGameAssetGenerator::ProcessBatchGeneration(const FString& RequestID, const TArray<FGameAssetRequirement>& Requirements)
{
    TArray<FGeneratedGameAsset> GeneratedAssets;
    
    for (const FGameAssetRequirement& Requirement : Requirements)
    {
        FGeneratedGameAsset Asset = ProcessAssetGeneration(Requirement);
        FString AssetID = GenerateUniqueAssetID();
        Asset.AssetID = AssetID;
        
        GeneratedAssets.Add(Asset);
        GeneratedAssets.Add(AssetID, Asset);
        
        // 更新統計數據
        UpdateStatistics(Requirement.AssetCategory, Asset.GenerationTime);
        
        // 觸發單個資產生成完成事件
        OnGameAssetGenerated.Broadcast(AssetID, Asset);
    }
    
    // 觸發批量生成完成事件
    OnGameAssetGenerationCompleted.Broadcast(RequestID, GeneratedAssets);
}

bool UMingRTSGameAssetGenerator::IsValidAssetID(const FString& AssetID) const
{
    return !AssetID.IsEmpty() && GeneratedAssets.Contains(AssetID);
}

void UMingRTSGameAssetGenerator::LogAssetMessage(const FString& AssetID, const FString& Message, bool bIsError)
{
    if (bIsError)
    {
        UE_LOG(LogTemp, Error, TEXT("Asset %s: %s"), *AssetID, *Message);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Asset %s: %s"), *AssetID, *Message);
    }
}

void UMingRTSGameAssetGenerator::UpdateStatistics(EGameAssetCategory Category, float GenerationTime)
{
    // 更新資產計數
    int32* Count = AssetCounts.Find(Category);
    if (Count)
    {
        *Count += 1;
    }
    else
    {
        AssetCounts.Add(Category, 1);
    }
    
    // 更新總生成時間
    float* TotalTime = TotalGenerationTimes.Find(Category);
    if (TotalTime)
    {
        *TotalTime += GenerationTime;
    }
    else
    {
        TotalGenerationTimes.Add(Category, GenerationTime);
    }
    
    // 更新全局統計
    TotalGeneratedAssets++;
    TotalGenerationTime += GenerationTime;
}

void UMingRTSGameAssetGenerator::CleanupExpiredAssets()
{
    // 清理過期資產的邏輯
    // 這裡可以根據需要實現清理邏輯
}

void UMingRTSGameAssetGenerator::BackupAsset(const FGeneratedGameAsset& Asset)
{
    if (!bEnableAutoBackup)
    {
        return;
    }
    
    // 備份資產的邏輯
    FString BackupPath = FPaths::Combine(DefaultAssetPath, TEXT("Backups/"));
    FString BackupFileName = FString::Printf(TEXT("%s_%s.backup"), *Asset.AssetName, *FDateTime::Now().ToString());
    
    // 創建備份目錄
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*BackupPath))
    {
        PlatformFile.CreateDirectoryTree(*BackupPath);
    }
    
    // 保存備份
    FString FullBackupPath = FPaths::Combine(BackupPath, BackupFileName);
    FFileHelper::SaveArrayToFile(Asset.AssetData, *FullBackupPath);
    
    UE_LOG(LogTemp, Log, TEXT("Asset backed up: %s"), *Asset.AssetName);
}

TArray<uint8> UMingRTSGameAssetGenerator::CompressAssetData(const TArray<uint8>& AssetData) const
{
    if (!bEnableCompression)
    {
        return AssetData;
    }
    
    // 壓縮資產數據的邏輯
    // 這裡可以使用UE的壓縮功能
    return AssetData;
}

TArray<uint8> UMingRTSGameAssetGenerator::DecompressAssetData(const TArray<uint8>& CompressedData) const
{
    // 解壓縮資產數據的邏輯
    // 這裡可以使用UE的解壓縮功能
    return CompressedData;
}

TMap<FString, FString> UMingRTSGameAssetGenerator::GetSystemStatus() const
{
    TMap<FString, FString> Status;
    
    Status.Add(TEXT("CurrentWorld"), CurrentWorld ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("AIContentSystem"), AIContentSystem ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("ContentRenderer"), ContentRenderer ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("ContentManager"), ContentManager ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("TotalAssets"), FString::FromInt(GeneratedAssets.Num()));
    Status.Add(TEXT("ActiveGenerations"), FString::FromInt(ActiveGenerations.Num()));
    
    return Status;
}

// 高校引導內容生成函數
FString UMingRTSGameAssetGenerator::GenerateCampusTourContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 校園導覽\n\n");
    Content += FString::Printf(TEXT("歡迎來到%s的校園導覽！\n\n"), *Config.UniversityName);
    
    if (Config.bIncludeHistoricalContent)
    {
        Content += TEXT("### 歷史背景\n");
        Content += FString::Printf(TEXT("%s擁有悠久的歷史，始建於明朝時期，承載著深厚的文化底蘊。\n\n"), *Config.UniversityName);
    }
    
    Content += TEXT("### 主要建築\n");
    Content += TEXT("- **圖書館**: 收藏豐富的古籍和現代文獻\n");
    Content += TEXT("- **教學樓**: 現代化的教學設施\n");
    Content += TEXT("- **實驗室**: 先進的科研設備\n");
    Content += TEXT("- **體育館**: 完善的運動設施\n\n");
    
    Content += TEXT("### 校園生活\n");
    Content += TEXT("- 學生社團活動豐富多彩\n");
    Content += TEXT("- 體育競賽頻繁舉辦\n");
    Content += TEXT("- 文化活動精彩紛呈\n\n");
    
    if (Config.bInteractive)
    {
        Content += TEXT("### 互動體驗\n");
        Content += TEXT("- 360度全景校園導覽\n");
        Content += TEXT("- 虛擬實境校園體驗\n");
        Content += TEXT("- 互動式校園地圖\n\n");
    }
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateAcademicIntroContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 學術介紹\n\n");
    Content += FString::Printf(TEXT("%s%s的學術介紹！\n\n"), *Config.UniversityName, *Config.Department);
    
    Content += TEXT("### 學術特色\n");
    Content += TEXT("- 傳統與現代結合的教學理念\n");
    Content += TEXT("- 注重實踐與理論並重\n");
    Content += TEXT("- 國際化的學術交流\n");
    Content += TEXT("- 創新驅動的科研發展\n\n");
    
    Content += TEXT("### 主要學科\n");
    for (const FString& Topic : Config.KeyTopics)
    {
        Content += FString::Printf(TEXT("- %s\n"), *Topic);
    }
    Content += TEXT("\n");
    
    Content += TEXT("### 師資力量\n");
    Content += TEXT("- 資深教授領銜教學團隊\n");
    Content += TEXT("- 國際知名學者客座講學\n");
    Content += TEXT("- 青年教師快速成長\n");
    Content += TEXT("- 產學研結合的導師制度\n\n");
    
    if (Config.bIncludeStudentTestimonials)
    {
        Content += TEXT("### 學生心得\n");
        Content += TEXT("- \"在這裡學習讓我受益匪淺\"\n");
        Content += TEXT("- \"老師們都非常負責任\"\n");
        Content += TEXT("- \"學術氛圍非常濃厚\"\n\n");
    }
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateStudentLifeContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 學生生活\n\n");
    Content += FString::Printf(TEXT("歡迎了解%s的學生生活！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 住宿生活\n");
    Content += TEXT("- 現代化的學生宿舍\n");
    Content += TEXT("- 完善的生活設施\n");
    Content += TEXT("- 24小時安保系統\n");
    Content += TEXT("- 豐富的宿舍活動\n\n");
    
    Content += TEXT("### 飲食文化\n");
    Content += TEXT("- 多樣化的餐廳選擇\n");
    Content += TEXT("- 傳統美食與現代料理\n");
    Content += TEXT("- 營養均衡的膳食搭配\n");
    Content += TEXT("- 特殊飲食需求滿足\n\n");
    
    Content += TEXT("### 課外活動\n");
    Content += TEXT("- 學生社團超過100個\n");
    Content += TEXT("- 體育競技隊伍\n");
    Content += TEXT("- 藝術文化團體\n");
    Content += TEXT("- 志願服務組織\n\n");
    
    Content += TEXT("### 學生支持\n");
    Content += TEXT("- 心理健康諮詢服務\n");
    Content += TEXT("- 學業輔導中心\n");
    Content += TEXT("- 職業發展指導\n");
    Content += TEXT("- 經濟援助計劃\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateResearchShowcaseContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 科研展示\n\n");
    Content += FString::Printf(TEXT("%s%s的科研成果展示！\n\n"), *Config.UniversityName, *Config.Department);
    
    Content += TEXT("### 重點研究領域\n");
    for (const FString& Topic : Config.KeyTopics)
    {
        Content += FString::Printf(TEXT("- %s\n"), *Topic);
    }
    Content += TEXT("\n");
    
    Content += TEXT("### 代表性成果\n");
    Content += TEXT("- 國際頂級期刊論文發表\n");
    Content += TEXT("- 專利技術轉化應用\n");
    Content += TEXT("- 產學研合作項目\n");
    Content += TEXT("- 國際科研合作\n\n");
    
    Content += TEXT("### 研究設施\n");
    Content += TEXT("- 國家級重點實驗室\n");
    Content += TEXT("- 省部級科研平台\n");
    Content += TEXT("- 企業聯合研發中心\n");
    Content += TEXT("- 國際合作研究基地\n\n");
    
    Content += TEXT("### 研究團隊\n");
    Content += TEXT("- 跨學科研究團隊\n");
    Content += TEXT("- 國際合作研究組\n");
    Content += TEXT("- 青年科研人才\n");
    Content += TEXT("- 產業界專家顧問\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateCareerGuidanceContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 職業指導\n\n");
    Content += FString::Printf(TEXT("%s的職業發展指導！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 就業方向\n");
    Content += TEXT("- 國有企業和政府機關\n");
    Content += TEXT("- 外資企業和跨國公司\n");
    Content += TEXT("- 創新創業和自主創業\n");
    Content += TEXT("- 繼續深造和留學\n\n");
    
    Content += TEXT("### 職業服務\n");
    Content += TEXT("- 就業信息發布平台\n");
    Content += TEXT("- 校園招聘會活動\n");
    Content += TEXT("- 簡歷指導和面試培訓\n");
    Content += TEXT("- 職業規劃諮詢服務\n\n");
    
    Content += TEXT("### 校友網絡\n");
    Content += TEXT("- 校友會組織\n");
    Content += TEXT("- 行業交流平台\n");
    Content += TEXT("- 導師計劃\n");
    Content += TEXT("- 實習機會推薦\n\n");
    
    Content += TEXT("### 成功案例\n");
    Content += TEXT("- 行業領軍人物\n");
    Content += TEXT("- 創業成功典範\n");
    Content += TEXT("- 學術界精英\n");
    Content += TEXT("- 社會各界領袖\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateHistoricalContentContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 歷史文化\n\n");
    Content += FString::Printf(TEXT("%s的歷史文化介紹！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 建校歷程\n");
    Content += TEXT("- 明朝時期的創辦背景\n");
    Content += TEXT("- 歷史變遷與發展\n");
    Content += TEXT("- 重要歷史事件\n");
    Content += TEXT("- 傳統與現代的融合\n\n");
    
    Content += TEXT("### 文化傳承\n");
    Content += TEXT("- 儒家教育理念\n");
    Content += TEXT("- 書院文化傳統\n");
    Content += TEXT("- 現代教育創新\n");
    Content += TEXT("- 國際文化交流\n\n");
    
    Content += TEXT("### 名人事蹟\n");
    Content += TEXT("- 歷任著名校長\n");
    Content += TEXT("- 傑出校友代表\n");
    Content += TEXT("- 知名學者教授\n");
    Content += TEXT("- 社會各界貢獻\n\n");
    
    Content += TEXT("### 文化遺產\n");
    Content += TEXT("- 古建築保護\n");
    Content += TEXT("- 文獻資料收藏\n");
    Content += TEXT("- 非物質文化遺產\n");
    Content += TEXT("- 校史文化展覽\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateCulturalContentContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 文化活動\n\n");
    Content += FString::Printf(TEXT("%s的文化活動介紹！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 傳統文化\n");
    Content += TEXT("- 中國傳統節日慶祝\n");
    Content += TEXT("- 書法繪畫展覽\n");
    Content += TEXT("- 民族音樂演奏\n");
    Content += TEXT("- 戲曲文化表演\n\n");
    
    Content += TEXT("### 現代文化\n");
    Content += TEXT("- 現代藝術展覽\n");
    Content += TEXT("- 流行音樂會\n");
    Content += TEXT("- 電影放映活動\n");
    Content += TEXT("- 文學創作比賽\n\n");
    
    Content += TEXT("### 國際文化\n");
    Content += TEXT("- 國際文化交流節\n");
    Content += TEXT("- 外語文化活動\n");
    Content += TEXT("- 留學生文化展示\n");
    Content += TEXT("- 跨文化研討會\n\n");
    
    Content += TEXT("### 文化設施\n");
    Content += TEXT("- 藝術中心\n");
    Content += TEXT("- 音樂廳\n");
    Content += TEXT("- 美術館\n");
    Content += TEXT("- 文化活動中心\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateTechnicalDemoContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 技術演示\n\n");
    Content += FString::Printf(TEXT("%s的技術演示展示！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 實驗室設備\n");
    Content += TEXT("- 先進的儀器設備\n");
    Content += TEXT("- 專業的實驗環境\n");
    Content += TEXT("- 安全的操作規範\n");
    Content += TEXT("- 24小時開放服務\n\n");
    
    Content += TEXT("### 技術展示\n");
    for (const FString& Topic : Config.KeyTopics)
    {
        Content += FString::Printf(TEXT("- %s技術演示\n"), *Topic);
    }
    Content += TEXT("\n");
    
    Content += TEXT("### 創新項目\n");
    Content += TEXT("- 學生創新實驗項目\n");
    Content += TEXT("- 教師科研展示項目\n");
    Content += TEXT("- 產學研合作項目\n");
    Content += TEXT("- 國際合作項目\n\n");
    
    Content += TEXT("### 技術培訓\n");
    Content += TEXT("- 專業技能培訓\n");
    Content += TEXT("- 軟件操作指導\n");
    Content += TEXT("- 安全操作培訓\n");
    Content += TEXT("- 創新思維培養\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateInteractiveGuideContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 互動指南\n\n");
    Content += FString::Printf(TEXT("%s的互動指南！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 虛擬導覽\n");
    Content += TEXT("- 360度校園全景\n");
    Content += TEXT("- 虛擬實境體驗\n");
    Content += TEXT("- 增強現實導覽\n");
    Content += TEXT("- 在線互動平台\n\n");
    
    Content += TEXT("### 互動功能\n");
    Content += TEXT("- 實時問答系統\n");
    Content += TEXT("- 在線諮詢服務\n");
    Content += TEXT("- 互動式地圖\n");
    Content += TEXT("- 個性化推薦\n\n");
    
    Content += TEXT("### 學習工具\n");
    Content += TEXT("- 在線學習平台\n");
    Content += TEXT("- 知識測試系統\n");
    Content += TEXT("- 學習進度追蹤\n");
    Content += TEXT("- 成績分析報告\n\n");
    
    Content += TEXT("### 社交功能\n");
    Content += TEXT("- 學生交流社區\n");
    Content += TEXT("- 導師在線答疑\n");
    Content += TEXT("- 同學互助平台\n");
    Content += TEXT("- 校友聯絡網絡\n\n");
    
    return Content;
}

FString UMingRTSGameAssetGenerator::GenerateCustomUniversityContent(const FUniversityGuideConfig& Config)
{
    FString Content = TEXT("## 自定義內容\n\n");
    Content += FString::Printf(TEXT("%s的自定義內容！\n\n"), *Config.UniversityName);
    
    Content += TEXT("### 特色介紹\n");
    Content += TEXT("- 獨特的辦學理念\n");
    Content += TEXT("- 特色教學項目\n");
    Content += TEXT("- 專業優勢介紹\n");
    Content += TEXT("- 發展前景展望\n\n");
    
    Content += TEXT("### 個性化服務\n");
    Content += TEXT("- 定制化學習計劃\n");
    Content += TEXT("- 個人化指導方案\n");
    Content += TEXT("- 專屬服務通道\n");
    Content += TEXT("- 特殊需求滿足\n\n");
    
    return Content;
}

// 輔助函數實現
FString UMingRTSGameAssetGenerator::GetAssetStyleName(EArtStyle Style) const
{
    switch (Style)
    {
        case EArtStyle::Realistic: return TEXT("Realistic");
        case EArtStyle::Stylized: return TEXT("Stylized");
        case EArtStyle::Cartoon: return TEXT("Cartoon");
        case EArtStyle::Watercolor: return TEXT("Watercolor");
        case EArtStyle::InkWash: return TEXT("InkWash");
        case EArtStyle::OilPainting: return TEXT("OilPainting");
        case EArtStyle::PixelArt: return TEXT("PixelArt");
        case EArtStyle::LowPoly: return TEXT("LowPoly");
        case EArtStyle::CelShading: return TEXT("CelShading");
        case EArtStyle::Photorealistic: return TEXT("Photorealistic");
        default: return TEXT("Custom");
    }
}

FString UMingRTSGameAssetGenerator::GetQualityLevelName(EQualityLevel Quality) const
{
    switch (Quality)
    {
        case EQualityLevel::Draft: return TEXT("Draft");
        case EQualityLevel::Standard: return TEXT("Standard");
        case EQualityLevel::High: return TEXT("High");
        case EQualityLevel::Ultra: return TEXT("Ultra");
        case EQualityLevel::Cinematic: return TEXT("Cinematic");
        default: return TEXT("Custom");
    }
}

FString UMingRTSGameAssetGenerator::GenerateAssetMetadataJSON(const FGeneratedGameAsset& Asset) const
{
    FString Metadata;
    
    Metadata += TEXT("{\n");
    Metadata += FString::Printf(TEXT("  \"AssetID\": \"%s\",\n"), *Asset.AssetID);
    Metadata += FString::Printf(TEXT("  \"AssetName\": \"%s\",\n"), *Asset.AssetName);
    Metadata += FString::Printf(TEXT("  \"AssetCategory\": \"%s\",\n"), *GetAssetCategoryName(Asset.AssetCategory));
    Metadata += FString::Printf(TEXT("  \"AssetPurpose\": \"%s\",\n"), *GetAssetPurposeName(Asset.AssetPurpose));
    Metadata += FString::Printf(TEXT("  \"Description\": \"%s\",\n"), *Asset.Description);
    Metadata += FString::Printf(TEXT("  \"Quality\": \"%s\",\n"), *GetQualityLevelName(Asset.Quality));
    Metadata += FString::Printf(TEXT("  \"Style\": \"%s\",\n"), *GetAssetStyleName(Asset.Style));
    Metadata += FString::Printf(TEXT("  \"AssetPath\": \"%s\",\n"), *Asset.AssetPath);
    Metadata += FString::Printf(TEXT("  \"GeneratedAt\": \"%s\",\n"), *Asset.GeneratedAt);
    Metadata += FString::Printf(TEXT("  \"GenerationTime\": %.2f\n"), *Asset.GenerationTime);
    Metadata += TEXT("}\n");
    
    return Metadata;
}

// 資產生成輔助函數
FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateGameSceneAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Scene;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成場景內容
    FString SceneContent = GenerateGameSceneContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*SceneContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateCharacterAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Mesh;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成角色內容
    FString CharacterContent = GenerateCharacterContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*CharacterContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateBuildingAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Mesh;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成建築內容
    FString BuildingContent = GenerateBuildingContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*BuildingContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateEnvironmentAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Scene;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成環境內容
    FString EnvironmentContent = GenerateEnvironmentContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*EnvironmentContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateUIAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::UI;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成UI內容
    FString UIContent = GenerateUIContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*UIContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateVideoAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Video;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成影片內容
    FString VideoContent = GenerateVideoContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*VideoContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateMusicAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Music;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成音樂內容
    FString MusicContent = GenerateMusicContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*MusicContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateSoundEffectAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::SoundEffect;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成音效內容
    FString SoundEffectContent = GenerateSoundEffectContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*SoundEffectContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateTextureAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Texture;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成紋理內容
    FString TextureContent = GenerateTextureContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*TextureContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateModelAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Mesh;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成3D模型內容
    FString ModelContent = GenerateModelContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*ModelContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateAnimationAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Animation;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成動畫內容
    FString AnimationContent = GenerateAnimationContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*AnimationContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateTutorialAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Text;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成教學內容
    FString TutorialContent = GenerateTutorialContent(Requirement);
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*TutorialContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}

FGeneratedGameAsset UMingRTSGameAssetGenerator::GenerateCustomAsset(const FGameAssetRequirement& Requirement)
{
    FGeneratedGameAsset Asset;
    Asset.AssetType = EAIContentType::Custom;
    Asset.AssetName = Requirement.AssetName;
    Asset.AssetPath = Requirement.TargetPath;
    Asset.Description = Requirement.Description;
    
    // 生成自定義內容
    FString CustomContent = FString::Printf(TEXT("Custom Asset: %s\n"), *Requirement.AssetName);
    CustomContent += FString::Printf(TEXT("Description: %s\n"), *Requirement.Description);
    CustomContent += FString::Printf(TEXT("Category: %s\n"), *GetAssetCategoryName(Requirement.AssetCategory));
    CustomContent += FString::Printf(TEXT("Purpose: %s\n"), *GetAssetPurposeName(Requirement.AssetPurpose));
    CustomContent += FString::Printf(TEXT("Quality: %s\n"), *GetQualityLevelName(Requirement.Quality));
    CustomContent += FString::Printf(TEXT("Style: %s\n"), *GetAssetStyleName(Requirement.Style));
    
    // 將內容轉換為字節數組
    FTCHARToUTF8 Convert(*CustomContent);
    Asset.AssetData.Append((uint8*)Convert.Get(), Convert.Length());
    
    return Asset;
}
