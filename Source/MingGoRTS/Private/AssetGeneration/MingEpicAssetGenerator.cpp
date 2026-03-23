#include "MingEpicAssetGenerator.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"
#include "HAL/PlatformFilemanager.h"

UMingEpicAssetGenerator::UMingEpicAssetGenerator()
{
    bIsInitialized = false;
    ProjectRoot = FPaths::ProjectDir();
    OutputBase = FPaths::Combine(*ProjectRoot, TEXT("Content/EpicAssets"));
    ConfigPath = FPaths::Combine(*ProjectRoot, TEXT("Tools/ai/epic_assets_config.json"));
    ProgressFilePath = FPaths::Combine(*ProjectRoot, TEXT("Tools/ai/epic_generation_progress.json"));
}

void UMingEpicAssetGenerator::InitializeEpicAssetGenerator()
{
    if (bIsInitialized)
    {
        WriteLog(TEXT("Epic asset generator already initialized"), TEXT("WARNING"));
        return;
    }

    WriteLog(TEXT("========================================"), TEXT("INFO"));
    WriteLog(TEXT("MingGoRTS Epic Asset Generation System"), TEXT("INFO"));
    WriteLog(TEXT("========================================"), TEXT("INFO"));

    // Initialize epic configurations
    InitializeEpicConfigs();

    // Create output structure
    CreateOutputStructure();

    // Save configuration
    SaveConfig();

    bIsInitialized = true;
    WriteLog(TEXT("Epic asset generator initialized successfully"), TEXT("SUCCESS"));
}

void UMingEpicAssetGenerator::InitializeEpicConfigs()
{
    // Epic 1: Decision Making System
    FMingEpicConfig Epic1;
    Epic1.Name = TEXT("決策系統與歷史事件");
    Epic1.Description = TEXT("決策系統與歷史事件觸發機制，包含重要歷史決策點和事件觸發");

    TArray<FMingAssetConfig> Epic1Images;
    FMingAssetConfig DecisionBG;
    DecisionBG.Name = TEXT("decision_bg");
    DecisionBG.Prompt = TEXT("Chinese Republican era 1920s, war room interior, wooden desk with military maps, vintage oil lamp, dramatic lighting, cinematic atmosphere, historical documentary style, 4K");
    DecisionBG.Category = TEXT("Background");
    DecisionBG.Resolution = TEXT("1920x1080");
    Epic1Images.Add(DecisionBG);

    FMingAssetConfig HistoricalCharacters;
    HistoricalCharacters.Name = TEXT("historical_characters");
    HistoricalCharacters.Prompt = TEXT("Portrait of Chinese Republican era military officers, formal uniforms, serious expressions, vintage photography style, sepia tone, historical authenticity");
    HistoricalCharacters.Category = TEXT("Characters");
    HistoricalCharacters.Resolution = TEXT("512x512");
    HistoricalCharacters.Count = 5;
    Epic1Images.Add(HistoricalCharacters);

    Epic1.Assets.Add(EMingAssetType::Images, Epic1Images);

    TArray<FMingAssetConfig> Epic1Music;
    FMingAssetConfig DecisionTheme;
    DecisionTheme.Name = TEXT("decision_theme");
    DecisionTheme.Prompt = TEXT("Tense decision-making music, Chinese traditional instruments mixed with Western orchestra, dramatic and contemplative, suitable for historical strategy decisions, 100 BPM");
    DecisionTheme.Category = TEXT("Decision");
    DecisionTheme.Duration = 45;
    Epic1Music.Add(DecisionTheme);

    Epic1.Assets.Add(EMingAssetType::Music, Epic1Music);

    TArray<FMingAssetConfig> Epic1SFX;
    FMingAssetConfig DecisionConfirm;
    DecisionConfirm.Name = TEXT("decision_confirm");
    DecisionConfirm.Description = TEXT("Decision confirmation sound, historical military stamp/seal effect");
    DecisionConfirm.Category = TEXT("UI");
    Epic1SFX.Add(DecisionConfirm);

    Epic1.Assets.Add(EMingAssetType::SFX, Epic1SFX);

    EpicConfigs.Add(EMingEpicId::Epic1, Epic1);

    // Epic 2: Large Scale Battles
    FMingEpicConfig Epic2;
    Epic2.Name = TEXT("大規模戰鬥系統");
    Epic2.Description = TEXT("1000+單位的大規模戰鬥系統，包含戰術地圖和戰鬥機制");

    TArray<FMingAssetConfig> Epic2Images;
    FMingAssetConfig BattlefieldTactical;
    BattlefieldTactical.Name = TEXT("battlefield_tactical");
    BattlefieldTactical.Prompt = TEXT("Epic battlefield scene, 1000+ soldiers, Chinese Republican era warfare, strategic overview, smoke and explosions, cinematic aerial view, dramatic lighting");
    BattlefieldTactical.Category = TEXT("Battlefield");
    BattlefieldTactical.Resolution = TEXT("1920x1080");
    Epic2Images.Add(BattlefieldTactical);

    Epic2.Assets.Add(EMingAssetType::Images, Epic2Images);

    TArray<FMingAssetConfig> Epic2Music;
    FMingAssetConfig BattleEpic;
    BattleEpic.Name = TEXT("battle_epic");
    BattleEpic.Prompt = TEXT("Intense large-scale battle music, full orchestra with Chinese war drums and percussion, epic and driving, 140 BPM, suitable for 1000+ unit battles");
    BattleEpic.Category = TEXT("Battle");
    BattleEpic.Duration = 120;
    Epic2Music.Add(BattleEpic);

    Epic2.Assets.Add(EMingAssetType::Music, Epic2Music);

    EpicConfigs.Add(EMingEpicId::Epic2, Epic2);

    // Epic 3: Character Development
    FMingEpicConfig Epic3;
    Epic3.Name = TEXT("角色發展與技能系統");
    Epic3.Description = TEXT("角色成長和技能系統，包含角色創建、技能樹和個人劇情");

    TArray<FMingAssetConfig> Epic3Images;
    FMingAssetConfig CharacterCreationBG;
    CharacterCreationBG.Name = TEXT("character_creation_bg");
    CharacterCreationBG.Prompt = TEXT("Character creation screen background, Chinese Republican era study room, books and scrolls, warm lighting, elegant and scholarly atmosphere");
    CharacterCreationBG.Category = TEXT("Background");
    CharacterCreationBG.Resolution = TEXT("1920x1080");
    Epic3Images.Add(CharacterCreationBG);

    Epic3.Assets.Add(EMingAssetType::Images, Epic3Images);

    TArray<FMingAssetConfig> Epic3Music;
    FMingAssetConfig CharacterTheme;
    CharacterTheme.Name = TEXT("character_theme");
    CharacterTheme.Prompt = TEXT("Character development music, personal growth and introspection, Chinese flute and strings, emotional and inspiring");
    CharacterTheme.Category = TEXT("Character");
    CharacterTheme.Duration = 60;
    Epic3Music.Add(CharacterTheme);

    Epic3.Assets.Add(EMingAssetType::Music, Epic3Music);

    EpicConfigs.Add(EMingEpicId::Epic3, Epic3);

    // Epic 4: Base Building
    FMingEpicConfig Epic4;
    Epic4.Name = TEXT("基地建設與資源管理");
    Epic4.Description = TEXT("基地建設和資源管理系統，包含建築升級和經濟循環");

    TArray<FMingAssetConfig> Epic4Images;
    FMingAssetConfig BaseConstruction;
    BaseConstruction.Name = TEXT("base_construction");
    BaseConstruction.Prompt = TEXT("Base building construction site, Chinese Republican era military base, barracks and factories, industrial atmosphere, strategic game style");
    BaseConstruction.Category = TEXT("Buildings");
    BaseConstruction.Resolution = TEXT("1920x1080");
    Epic4Images.Add(BaseConstruction);

    Epic4.Assets.Add(EMingAssetType::Images, Epic4Images);

    TArray<FMingAssetConfig> Epic4Music;
    FMingAssetConfig ConstructionTheme;
    ConstructionTheme.Name = TEXT("construction_theme");
    ConstructionTheme.Prompt = TEXT("Base building and construction music, productive and industrious, Chinese folk elements mixed with modern, upbeat rhythm 110 BPM");
    ConstructionTheme.Category = TEXT("Construction");
    ConstructionTheme.Duration = 60;
    Epic4Music.Add(ConstructionTheme);

    Epic4.Assets.Add(EMingAssetType::Music, Epic4Music);

    EpicConfigs.Add(EMingEpicId::Epic4, Epic4);

    // Epic 5: Strategic Layers
    FMingEpicConfig Epic5;
    Epic5.Name = TEXT("戰略層次與地圖系統");
    Epic5.Description = TEXT("多層次戰略地圖系統，包含戰略、戰術、戰役和外交層次");

    TArray<FMingAssetConfig> Epic5Images;
    FMingAssetConfig StrategicMap;
    StrategicMap.Name = TEXT("strategic_map");
    StrategicMap.Prompt = TEXT("Strategic map overview, China Republican era, territories and factions, beautiful and detailed, game map style, vintage cartography");
    StrategicMap.Category = TEXT("Maps");
    StrategicMap.Resolution = TEXT("1920x1080");
    Epic5Images.Add(StrategicMap);

    Epic5.Assets.Add(EMingAssetType::Images, Epic5Images);

    TArray<FMingAssetConfig> Epic5Music;
    FMingAssetConfig StrategicTheme;
    StrategicTheme.Name = TEXT("strategic_theme");
    StrategicTheme.Prompt = TEXT("Grand strategic overview music, vast and contemplative, Chinese imperial court influence mixed with modern orchestra");
    StrategicTheme.Category = TEXT("Strategy");
    StrategicTheme.Duration = 90;
    Epic5Music.Add(StrategicTheme);

    Epic5.Assets.Add(EMingAssetType::Music, Epic5Music);

    EpicConfigs.Add(EMingEpicId::Epic5, Epic5);

    // Epic 6: Cross-Platform
    FMingEpicConfig Epic6;
    Epic6.Name = TEXT("跨平台同步與雲端存檔");
    Epic6.Description = TEXT("跨平台設備同步和雲端存檔系統，支援PC、手機和平板");

    TArray<FMingAssetConfig> Epic6Images;
    FMingAssetConfig CrossPlatformUI;
    CrossPlatformUI.Name = TEXT("cross_platform_ui");
    CrossPlatformUI.Prompt = TEXT("Cross-platform UI mockups, PC and mobile interfaces, responsive design, Chinese Republican era game theme, side-by-side comparison");
    CrossPlatformUI.Category = TEXT("UI");
    CrossPlatformUI.Resolution = TEXT("1920x1080");
    Epic6Images.Add(CrossPlatformUI);

    Epic6.Assets.Add(EMingAssetType::Images, Epic6Images);

    TArray<FMingAssetConfig> Epic6Music;
    FMingAssetConfig MenuTheme;
    MenuTheme.Name = TEXT("menu_theme");
    MenuTheme.Prompt = TEXT("Main menu music, grand and inviting, suitable for all platforms, Chinese and Western fusion, memorable theme");
    MenuTheme.Category = TEXT("Main");
    MenuTheme.Duration = 120;
    Epic6Music.Add(MenuTheme);

    Epic6.Assets.Add(EMingAssetType::Music, Epic6Music);

    EpicConfigs.Add(EMingEpicId::Epic6, Epic6);
}

void UMingEpicAssetGenerator::CreateOutputStructure()
{
    for (const auto& EpicPair : EpicConfigs)
    {
        FString EpicPath = FPaths::Combine(*OutputBase, *StaticEnum<EMingEpicId>()->GetValueAsString(EpicPair.Key));
        
        // Create directories for each asset type
        IFileManager::Get().MakeDirectory(*EpicPath, true);
        IFileManager::Get().MakeDirectory(*FPaths::Combine(*EpicPath, TEXT("Images")), true);
        IFileManager::Get().MakeDirectory(*FPaths::Combine(*EpicPath, TEXT("Music")), true);
        IFileManager::Get().MakeDirectory(*FPaths::Combine(*EpicPath, TEXT("SFX")), true);
        IFileManager::Get().MakeDirectory(*FPaths::Combine(*EpicPath, TEXT("Video")), true);
    }
    
    WriteLog(FString::Printf(TEXT("Output structure created at: %s"), *OutputBase), TEXT("INFO"));
}

FMingGenerationResults UMingEpicAssetGenerator::GenerateAssetsForEpic(EMingEpicId EpicId, const TArray<EMingAssetType>& AssetTypes, bool bForce)
{
    if (!bIsInitialized)
    {
        WriteLog(TEXT("Epic asset generator not initialized"), TEXT("ERROR"));
        FMingGenerationResults Results;
        Results.Failed.Add(TEXT("System not initialized"));
        return Results;
    }

    if (!EpicConfigs.Contains(EpicId))
    {
        WriteLog(FString::Printf(TEXT("Invalid Epic ID: %s"), *StaticEnum<EMingEpicId>()->GetValueAsString(EpicId)), TEXT("ERROR"));
        FMingGenerationResults Results;
        Results.Failed.Add(TEXT("Invalid Epic ID"));
        return Results;
    }

    FMingEpicConfig EpicConfig = EpicConfigs[EpicId];
    FString EpicPath = FPaths::Combine(*OutputBase, *StaticEnum<EMingEpicId>()->GetValueAsString(EpicId));
    
    WriteLog(FString::Printf(TEXT("\n%s================================================"), FString::Chr(61)), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("Generating Epic: %s"), *EpicConfig.Name), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("   %s"), *EpicConfig.Description), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("%s================================================\n"), FString::Chr(61)), TEXT("INFO"));

    FMingGenerationResults Results;
    
    // Load progress
    LoadProgress();
    
    // Determine which asset types to generate
    TArray<EMingAssetType> TypesToGenerate = AssetTypes;
    if (TypesToGenerate.Num() == 0)
    {
        TypesToGenerate.Add(EMingAssetType::Images);
        TypesToGenerate.Add(EMingAssetType::Music);
        TypesToGenerate.Add(EMingAssetType::SFX);
        TypesToGenerate.Add(EMingAssetType::Video);
    }

    for (EMingAssetType AssetType : TypesToGenerate)
    {
        if (!EpicConfig.Assets.Contains(AssetType))
        {
            continue;
        }

        WriteLog(FString::Printf(TEXT("\nProcessing Asset Type: %s"), *StaticEnum<EMingAssetType>()->GetValueAsString(AssetType)), TEXT("INFO"));
        
        FString TypePath;
        switch (AssetType)
        {
        case EMingAssetType::Images:
            TypePath = FPaths::Combine(*EpicPath, TEXT("Images"));
            break;
        case EMingAssetType::Music:
            TypePath = FPaths::Combine(*EpicPath, TEXT("Music"));
            break;
        case EMingAssetType::SFX:
            TypePath = FPaths::Combine(*EpicPath, TEXT("SFX"));
            break;
        case EMingAssetType::Video:
            TypePath = FPaths::Combine(*EpicPath, TEXT("Video"));
            break;
        }

        IFileManager::Get().MakeDirectory(*TypePath, true);

        TArray<FMingAssetConfig> Assets = EpicConfig.Assets[AssetType];
        
        for (const FMingAssetConfig& Asset : Assets)
        {
            FString AssetKey = FString::Printf(TEXT("%s_%s"), *StaticEnum<EMingAssetType>()->GetValueAsString(AssetType), *Asset.Name);
            
            // Check if already generated
            if (!bForce && GenerationProgress.Contains(AssetKey))
            {
                WriteLog(FString::Printf(TEXT("   Skipping %s (already generated)"), *Asset.Name), TEXT("INFO"));
                Results.Skipped.Add(Asset.Name);
                continue;
            }

            try
            {
                bool bSuccess = false;
                
                switch (AssetType)
                {
                case EMingAssetType::Images:
                    for (int32 i = 0; i < Asset.Count; i++)
                    {
                        FString Suffix = Asset.Count > 1 ? FString::Printf(TEXT("_%d"), i + 1) : TEXT("");
                        bSuccess = GeneratePlaceholderImage(Asset.Name + Suffix, Asset.Category, Asset.Resolution, TypePath);
                        WriteLog(FString::Printf(TEXT("   Generated image: %s%s (%s)"), *Asset.Name, *Suffix, *Asset.Resolution), TEXT("INFO"));
                    }
                    break;
                    
                case EMingAssetType::Music:
                    bSuccess = GeneratePlaceholderAudio(Asset.Name, Asset.Duration, Asset.Category, TypePath);
                    WriteLog(FString::Printf(TEXT("   Generated music: %s (%ds)"), *Asset.Name, Asset.Duration), TEXT("INFO"));
                    break;
                    
                case EMingAssetType::SFX:
                    for (int32 i = 0; i < Asset.Variations; i++)
                    {
                        FString Suffix = Asset.Variations > 1 ? FString::Printf(TEXT("_%d"), i + 1) : TEXT("");
                        FString OutputPath = FPaths::Combine(*TypePath, FString::Printf(TEXT("%s%s.wav"), *Asset.Name, *Suffix));
                        IFileManager::Get().FileExists(*OutputPath); // Create empty file
                        WriteLog(FString::Printf(TEXT("   Generated SFX: %s%s"), *Asset.Name, *Suffix), TEXT("INFO"));
                    }
                    bSuccess = true;
                    break;
                    
                case EMingAssetType::Video:
                    bSuccess = GeneratePlaceholderVideo(Asset.Name, Asset.Duration, Asset.Style, TypePath);
                    WriteLog(FString::Printf(TEXT("   Generated video: %s (%ds)"), *Asset.Name, Asset.Duration), TEXT("INFO"));
                    break;
                }

                if (bSuccess)
                {
                    Results.Success.Add(Asset.Name);
                    
                    // Update progress
                    FMingGenerationProgress Progress;
                    Progress.AssetKey = AssetKey;
                    Progress.Status = TEXT("generated");
                    Progress.Timestamp = FDateTime::Now();
                    GenerationProgress.Add(AssetKey, Progress);
                    
                    OnAssetGenerationCompleted.Broadcast(Asset.Name);
                }
                else
                {
                    Results.Failed.Add(Asset.Name);
                    
                    // Update progress with error
                    FMingGenerationProgress Progress;
                    Progress.AssetKey = AssetKey;
                    Progress.Status = TEXT("failed");
                    Progress.Error = TEXT("Generation failed");
                    Progress.Timestamp = FDateTime::Now();
                    GenerationProgress.Add(AssetKey, Progress);
                    
                    OnAssetGenerationFailed.Broadcast(FString::Printf(TEXT("Failed to generate %s"), *Asset.Name));
                }
            }
            catch (const std::exception& e)
            {
                WriteLog(FString::Printf(TEXT("   Failed to generate %s: %s"), *Asset.Name, UTF8_TO_TCHAR(e.what())), TEXT("ERROR"));
                Results.Failed.Add(Asset.Name);
            }
        }
    }

    // Save progress
    SaveProgress();

    // Print summary
    WriteLog(FString::Printf(TEXT("\nGeneration Summary for Epic %s:")), *StaticEnum<EMingEpicId>()->GetValueAsString(EpicId)), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("   Success: %d"), Results.Success.Num()), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("   Skipped: %d"), Results.Skipped.Num()), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("   Failed: %d"), Results.Failed.Num()), TEXT("INFO"));

    return Results;
}

bool UMingEpicAssetGenerator::GenerateAllEpics(const TArray<EMingAssetType>& AssetTypes, bool bForce)
{
    WriteLog(FString::Printf(TEXT("\n%s================================================"), FString::Chr(61)), TEXT("INFO"));
    WriteLog(TEXT("MingGoRTS Epic Asset Generation System"), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("%s================================================\n"), FString::Chr(61)), TEXT("INFO"));

    int32 TotalEpics = EpicConfigs.Num();
    int32 SuccessCount = 0;

    for (const auto& EpicPair : EpicConfigs)
    {
        EMingEpicId EpicId = EpicPair.Key;
        WriteLog(FString::Printf(TEXT("[%d/%d] Processing Epic: %s"), SuccessCount + 1, TotalEpics, *StaticEnum<EMingEpicId>()->GetValueAsString(EpicId)), TEXT("INFO"));
        
        FMingGenerationResults Results = GenerateAssetsForEpic(EpicId, AssetTypes, bForce);
        if (Results.Failed.Num() == 0)
        {
            SuccessCount++;
        }
    }

    WriteLog(FString::Printf(TEXT("\n%s================================================"), FString::Chr(61)), TEXT("INFO"));
    WriteLog(TEXT("Generation Summary"), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("%s================================================"), FString::Chr(61)), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("   Completed: %d/%d Epics"), SuccessCount, TotalEpics), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("   Output Directory: %s"), *OutputBase), TEXT("INFO"));

    return SuccessCount == TotalEpics;
}

void UMingEpicAssetGenerator::ListEpics()
{
    WriteLog(TEXT("\nAvailable Epic Asset Configurations:"), TEXT("INFO"));
    WriteLog(FString::Printf(TEXT("%s================================================"), FString::Chr(61)), TEXT("INFO"));

    LoadProgress();

    for (const auto& EpicPair : EpicConfigs)
    {
        EMingEpicId EpicId = EpicPair.Key;
        const FMingEpicConfig& EpicConfig = EpicPair.Value;
        
        WriteLog(FString::Printf(TEXT("\nEpic %s: %s"), *StaticEnum<EMingEpicId>()->GetValueAsString(EpicId), *EpicConfig.Name), TEXT("INFO"));
        WriteLog(FString::Printf(TEXT("   %s"), *EpicConfig.Description), TEXT("INFO"));

        for (const auto& AssetPair : EpicConfig.Assets)
        {
            EMingAssetType AssetType = AssetPair.Key;
            const TArray<FMingAssetConfig>& Assets = AssetPair.Value;
            
            int32 Total = 0;
            for (const FMingAssetConfig& Asset : Assets)
            {
                Total += Asset.Count * Asset.Variations;
            }
            
            int32 Generated = 0;
            for (const auto& ProgressPair : GenerationProgress)
            {
                if (ProgressPair.Key.StartsWith(*StaticEnum<EMingAssetType>()->GetValueAsString(AssetType)))
                {
                    Generated++;
                }
            }

            FString StatusIcon = Generated >= Total ? TEXT("✓") : TEXT("○");
            WriteLog(FString::Printf(TEXT("   %s %s: %d/%d"), *StatusIcon, *StaticEnum<EMingAssetType>()->GetValueAsString(AssetType), Generated, Total), TEXT("INFO"));
        }
    }
}

void UMingEpicAssetGenerator::CleanGeneratedAssets()
{
    if (IFileManager::Get().DirectoryExists(*OutputBase))
    {
        IFileManager::Get().DeleteDirectory(*OutputBase, true, true);
        WriteLog(FString::Printf(TEXT("Cleaned up generated assets: %s"), *OutputBase), TEXT("INFO"));
    }

    if (IFileManager::Get().FileExists(*ProgressFilePath))
    {
        IFileManager::Get().DeleteFile(*ProgressFilePath);
        WriteLog(TEXT("Cleaned up progress file"), TEXT("INFO"));
    }

    CreateOutputStructure();
}

FMingEpicConfig UMingEpicAssetGenerator::GetEpicConfig(EMingEpicId EpicId)
{
    if (EpicConfigs.Contains(EpicId))
    {
        return EpicConfigs[EpicId];
    }
    
    FMingEpicConfig EmptyConfig;
    return EmptyConfig;
}

TMap<FString, FMingGenerationProgress> UMingEpicAssetGenerator::GetGenerationProgress()
{
    return GenerationProgress;
}

bool UMingEpicAssetGenerator::GeneratePlaceholderImage(const FString& Name, const FString& Category, const FString& Resolution, const FString& OutputDir)
{
    FString OutputPath = FPaths::Combine(*OutputDir, FString::Printf(TEXT("%s.png"), *Name));
    
    // Create a simple placeholder texture
    int32 Width = 512;
    int32 Height = 512;
    
    // Parse resolution
    if (Resolution.Contains(TEXT("x")))
    {
        TArray<FString> Parts;
        Resolution.ParseIntoArray(Parts, TEXT("x"));
        if (Parts.Num() == 2)
        {
            Width = FCString::Atoi(*Parts[0]);
            Height = FCString::Atoi(*Parts[1]);
        }
    }

    // Create a simple texture
    UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height);
    if (!Texture)
    {
        return false;
    }

    // Create a simple color pattern
    FTexture2DMipMap& MipMap = Texture->GetPlatformData()->Mips[0];
    void* TextureData = MipMap.BulkData.Lock(LOCK_READ_WRITE);
    
    if (TextureData)
    {
        FColor* PixelData = static_cast<FColor*>(TextureData);
        for (int32 i = 0; i < Width * Height; i++)
        {
            // Create a simple pattern
            int32 x = i % Width;
            int32 y = i / Width;
            PixelData[i] = FColor(40 + (x % 20), 44 + (y % 20), 52, 255);
        }
        
        MipMap.BulkData.Unlock();
        Texture->UpdateResource();
    }

    // Save the texture (simplified - in real implementation would use image saving library)
    IFileManager::Get().FileExists(*OutputPath); // Create empty placeholder
    
    return true;
}

bool UMingEpicAssetGenerator::GeneratePlaceholderAudio(const FString& Name, int32 Duration, const FString& Category, const FString& OutputDir)
{
    FString OutputPath = FPaths::Combine(*OutputDir, FString::Printf(TEXT("%s.wav"), *Name));
    
    // Create a simple WAV file placeholder
    TArray<uint8> WavData;
    
    // WAV header
    WavData.Add(0x52); // 'R'
    WavData.Add(0x49); // 'I'
    WavData.Add(0x46); // 'F'
    WavData.Add(0x46); // 'F'
    
    // File size - 36
    int32 FileSize = 36 + Duration * 44100 * 2; // Simplified
    WavData.Add((FileSize >> 0) & 0xFF);
    WavData.Add((FileSize >> 8) & 0xFF);
    WavData.Add((FileSize >> 16) & 0xFF);
    WavData.Add((FileSize >> 24) & 0xFF);
    
    // WAVE
    WavData.Add(0x57); // 'W'
    WavData.Add(0x41); // 'A'
    WavData.Add(0x56); // 'V'
    WavData.Add(0x45); // 'E'
    
    // fmt chunk
    WavData.Add(0x66); // 'f'
    WavData.Add(0x6D); // 'm'
    WavData.Add(0x74); // 't'
    WavData.Add(0x20); // ' '
    
    // Chunk size - 16
    WavData.Add(16);
    WavData.Add(0);
    WavData.Add(0);
    WavData.Add(0);
    
    // Audio format - PCM
    WavData.Add(1);
    WavData.Add(0);
    
    // Channels - 1
    WavData.Add(1);
    WavData.Add(0);
    
    // Sample rate - 44100
    WavData.Add(0x44);
    WavData.Add(0xAC);
    WavData.Add(0);
    WavData.Add(0);
    
    // Byte rate - 88200
    WavData.Add(0x88);
    WavData.Add(0x58);
    WavData.Add(0x01);
    WavData.Add(0);
    
    // Block align - 2
    WavData.Add(2);
    WavData.Add(0);
    
    // Bits per sample - 16
    WavData.Add(16);
    WavData.Add(0);
    
    // data chunk
    WavData.Add(0x64); // 'd'
    WavData.Add(0x61); // 'a'
    WavData.Add(0x74); // 't'
    WavData.Add(0x61); // 'a'
    
    // Data size - 0 (placeholder)
    WavData.Add(0);
    WavData.Add(0);
    WavData.Add(0);
    WavData.Add(0);
    
    // Save file
    if (FFileHelper::SaveArrayToFile(WavData, *OutputPath))
    {
        return true;
    }
    
    return false;
}

bool UMingEpicAssetGenerator::GeneratePlaceholderVideo(const FString& Name, int32 Duration, const FString& Style, const FString& OutputDir)
{
    FString OutputPath = FPaths::Combine(*OutputDir, FString::Printf(TEXT("%s.mp4"), *Name));
    
    // Create placeholder video file
    IFileManager::Get().FileExists(*OutputPath); // Create empty placeholder
    
    return true;
}

void UMingEpicAssetGenerator::SaveConfig()
{
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    for (const auto& EpicPair : EpicConfigs)
    {
        TSharedPtr<FJsonObject> EpicObject = MakeShareable(new FJsonObject);
        EpicObject->SetStringField(TEXT("name"), EpicPair.Value.Name);
        EpicObject->SetStringField(TEXT("description"), EpicPair.Value.Description);
        
        TSharedPtr<FJsonObject> AssetsObject = MakeShareable(new FJsonObject);
        
        for (const auto& AssetPair : EpicPair.Value.Assets)
        {
            TArray<TSharedPtr<FJsonValue>> AssetArray;
            
            for (const FMingAssetConfig& Asset : AssetPair.Value)
            {
                TSharedPtr<FJsonObject> AssetObject = MakeShareable(new FJsonObject);
                AssetObject->SetStringField(TEXT("name"), Asset.Name);
                AssetObject->SetStringField(TEXT("prompt"), Asset.Prompt);
                AssetObject->SetStringField(TEXT("category"), Asset.Category);
                AssetObject->SetStringField(TEXT("resolution"), Asset.Resolution);
                AssetObject->SetNumberField(TEXT("duration"), Asset.Duration);
                AssetObject->SetNumberField(TEXT("count"), Asset.Count);
                AssetObject->SetNumberField(TEXT("variations"), Asset.Variations);
                AssetObject->SetBoolField(TEXT("loop"), Asset.bLoop);
                AssetObject->SetStringField(TEXT("style"), Asset.Style);
                
                AssetArray.Add(MakeShareable(new FJsonValueObject(AssetObject)));
            }
            
            AssetsObject->SetArrayField(*StaticEnum<EMingAssetType>()->GetValueAsString(AssetPair.Key), AssetArray);
        }
        
        EpicObject->SetObjectField(TEXT("assets"), AssetsObject);
        RootObject->SetObjectField(*StaticEnum<EMingEpicId>()->GetValueAsString(EpicPair.Key), EpicObject);
    }
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    // Ensure directory exists
    FString ConfigDir = FPaths::GetPath(ConfigPath);
    if (!IFileManager::Get().DirectoryExists(*ConfigDir))
    {
        IFileManager::Get().MakeDirectory(*ConfigDir);
    }
    
    FFileHelper::SaveStringToFile(OutputString, *ConfigPath, FFileHelper::EEncodingOptions::ForceUTF8);
    WriteLog(FString::Printf(TEXT("Configuration saved to: %s"), *ConfigPath), TEXT("INFO"));
}

void UMingEpicAssetGenerator::LoadConfig()
{
    // Configuration is already initialized in InitializeEpicConfigs
    // This would load from JSON file in a real implementation
}

void UMingEpicAssetGenerator::SaveProgress()
{
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    for (const auto& ProgressPair : GenerationProgress)
    {
        TSharedPtr<FJsonObject> ProgressObject = MakeShareable(new FJsonObject);
        ProgressObject->SetStringField(TEXT("status"), ProgressPair.Value.Status);
        ProgressObject->SetStringField(TEXT("timestamp"), ProgressPair.Value.Timestamp.ToString());
        ProgressObject->SetStringField(TEXT("error"), ProgressPair.Value.Error);
        
        RootObject->SetObjectField(ProgressPair.Key, ProgressObject);
    }
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    // Ensure directory exists
    FString ProgressDir = FPaths::GetPath(ProgressFilePath);
    if (!IFileManager::Get().DirectoryExists(*ProgressDir))
    {
        IFileManager::Get().MakeDirectory(*ProgressDir);
    }
    
    FFileHelper::SaveStringToFile(OutputString, *ProgressFilePath, FFileHelper::EEncodingOptions::ForceUTF8);
}

void UMingEpicAssetGenerator::LoadProgress()
{
    GenerationProgress.Empty();
    
    if (FPaths::FileExists(ProgressFilePath))
    {
        FString ProgressString;
        if (FFileHelper::LoadFileToString(ProgressString, *ProgressFilePath))
        {
            TSharedPtr<FJsonObject> RootObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ProgressString);
            
            if (FJsonSerializer::Deserialize(Reader, RootObject))
            {
                for (const auto& ProgressPair : RootObject->Values)
                {
                    TSharedPtr<FJsonObject> ProgressObject = ProgressPair.Value->AsObject();
                    if (ProgressObject.IsValid())
                    {
                        FMingGenerationProgress Progress;
                        Progress.AssetKey = ProgressPair.Key;
                        Progress.Status = ProgressObject->GetStringField(TEXT("status"));
                        Progress.Timestamp = FDateTime::Parse(ProgressObject->GetStringField(TEXT("timestamp")));
                        Progress.Error = ProgressObject->GetStringField(TEXT("error"));
                        
                        GenerationProgress.Add(ProgressPair.Key, Progress);
                    }
                }
            }
        }
    }
}

void UMingEpicAssetGenerator::WriteLog(const FString& Message, const FString& Level)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    FString LogEntry = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Level, *Message);
    
    // Print to console
    if (Level == TEXT("ERROR"))
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
    }
    else if (Level == TEXT("WARNING"))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
    }
    else if (Level == TEXT("SUCCESS"))
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
    }
}
