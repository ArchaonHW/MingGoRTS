#include "MingAIAssetGenerator.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"

// 靜態實例初始化
UMingAIAssetGenerator* UMingAIAssetGenerator::Instance = nullptr;

UMingAIAssetGenerator::UMingAIAssetGenerator()
    : bIsInitialized(false)
{
    // 設置預設API端點
    StableDiffusionEndpoint = TEXT("http://localhost:7860");
    AIVAEndpoint = TEXT("https://api.aiva.ai");
    
    // 預分配容量
    GeneratedAssets.Reserve(100);
}

void UMingAIAssetGenerator::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }
    
    // 創建資產目錄
    for (int32 i = 0; i < static_cast<int32>(EAIAssetType::VoiceLine) + 1; ++i)
    {
        CreateAssetDirectory(static_cast<EAIAssetType>(i));
    }
    
    bIsInitialized = true;
    Instance = this;
    
    UE_LOG(LogTemp, Log, TEXT("AIAssetGenerator initialized"));
    UE_LOG(LogTemp, Log, TEXT("Stable Diffusion: %s"), *StableDiffusionEndpoint);
    UE_LOG(LogTemp, Log, TEXT("AIVA: %s"), *AIVAEndpoint);
}

void UMingAIAssetGenerator::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    GeneratedAssets.Empty();
    bIsInitialized = false;
    Instance = nullptr;
    
    UE_LOG(LogTemp, Log, TEXT("AIAssetGenerator shutdown"));
}

FAIAssetGenerationResult UMingAIAssetGenerator::GenerateAsset(const FAIAssetGenerationParams& Params)
{
    if (!bIsInitialized)
    {
        return HandleGenerationError(TEXT("AIAssetGenerator not initialized"));
    }
    
    FAIAssetGenerationResult Result;
    Result.AssetType = Params.AssetType;
    Result.AssetID = GenerateAssetID();
    
    // 記錄開始時間
    double StartTime = FPlatformTime::Seconds();
    
    UE_LOG(LogTemp, Log, TEXT("Generating AI asset: %s (Type: %d, Era: %d)"),
        *Result.AssetID, static_cast<int32>(Params.AssetType), static_cast<int32>(Params.EraStyle));
    
    // 根據資產類型調用相應的生成方法
    switch (Params.AssetType)
    {
    case EAIAssetType::CharacterPortrait:
    case EAIAssetType::Building:
    case EAIAssetType::Landscape:
    case EAIAssetType::Weapon:
    case EAIAssetType::Uniform:
    case EAIAssetType::Prop:
        // 圖像資產 - 使用Stable Diffusion
        {
            FString Prompt = GenerateStableDiffusionPrompt(Params);
            Result = CallStableDiffusionAPI(Prompt, Params.NegativePrompt, Params.ImageWidth, Params.ImageHeight);
        }
        break;
        
    case EAIAssetType::BackgroundMusic:
    case EAIAssetType::SoundEffect:
        // 音樂資產 - 使用AIVA
        {
            Result = CallAIVAAPI(Params.DetailedPrompt, Params.MusicDuration, Params.MusicStyle);
        }
        break;
        
    case EAIAssetType::VoiceLine:
        // 語音資產 - 可以使用TTS或其他服務
        {
            Result.ErrorMessage = TEXT("Voice line generation not implemented yet");
            UE_LOG(LogTemp, Warning, TEXT("Voice line generation not implemented"));
        }
        break;
    }
    
    // 計算生成時間
    Result.GenerationTime = FPlatformTime::Seconds() - StartTime;
    
    // 保存資產
    if (Result.bSuccess)
    {
        SaveGeneratedAsset(Result);
        
        // 生成縮略圖
        if (Params.AssetType <= EAIAssetType::Prop) // 圖像資產
        {
            Result.ThumbnailPath = GenerateThumbnail(Result.AssetPath, Params.AssetType);
        }
        
        // 添加到資產列表
        GeneratedAssets.Add(Result);
        
        UE_LOG(LogTemp, Log, TEXT("AI asset generated successfully: %s (%.2f seconds)"),
            *Result.AssetID, Result.GenerationTime);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AI asset generation failed: %s - %s"),
            *Result.AssetID, *Result.ErrorMessage);
    }
    
    return Result;
}

void UMingAIAssetGenerator::GenerateAssetAsync(const FAIAssetGenerationParams& Params)
{
    // 異步生成 - 可以使用AsyncTask或Thread
    UE_LOG(LogTemp, Log, TEXT("Async asset generation requested"));
    
    // 簡化處理：直接調用同步方法
    GenerateAsset(Params);
}

FAIAssetGenerationResult UMingAIAssetGenerator::GenerateCharacterPortrait(
    const FString& CharacterDescription, 
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    FAIAssetGenerationParams Params;
    Params.AssetType = EAIAssetType::CharacterPortrait;
    Params.EraStyle = EraStyle;
    Params.BaseDescription = CharacterDescription;
    Params.ImageWidth = ImageWidth;
    Params.ImageHeight = ImageHeight;
    Params.DetailedPrompt = GetRepublicEraPrompt(EraStyle, EAIAssetType::CharacterPortrait) + TEXT(" ") + CharacterDescription;
    
    return GenerateAsset(Params);
}

FAIAssetGenerationResult UMingAIAssetGenerator::GenerateBuilding(
    const FString& BuildingDescription,
    ERepublicEraStyle EraStyle,
    int32 ImageWidth,
    int32 ImageHeight)
{
    FAIAssetGenerationParams Params;
    Params.AssetType = EAIAssetType::Building;
    Params.EraStyle = EraStyle;
    Params.BaseDescription = BuildingDescription;
    Params.ImageWidth = ImageWidth;
    Params.ImageHeight = ImageHeight;
    Params.DetailedPrompt = GetRepublicEraPrompt(EraStyle, EAIAssetType::Building) + TEXT(" ") + BuildingDescription;
    
    return GenerateAsset(Params);
}

FAIAssetGenerationResult UMingAIAssetGenerator::GenerateBackgroundMusic(
    const FString& MusicDescription,
    ERepublicEraStyle EraStyle,
    float Duration,
    const FString& MusicStyle)
{
    FAIAssetGenerationParams Params;
    Params.AssetType = EAIAssetType::BackgroundMusic;
    Params.EraStyle = EraStyle;
    Params.BaseDescription = MusicDescription;
    Params.MusicDuration = Duration;
    Params.MusicStyle = MusicStyle;
    Params.DetailedPrompt = GetRepublicEraPrompt(EraStyle, EAIAssetType::BackgroundMusic) + TEXT(" ") + MusicDescription;
    
    return GenerateAsset(Params);
}

FAIAssetGenerationResult UMingAIAssetGenerator::GetGeneratedAsset(const FString& AssetID)
{
    for (const FAIAssetGenerationResult& Asset : GeneratedAssets)
    {
        if (Asset.AssetID == AssetID)
        {
            return Asset;
        }
    }
    
    return FAIAssetGenerationResult();
}

TArray<FAIAssetGenerationResult> UMingAIAssetGenerator::GetAllGeneratedAssets()
{
    return GeneratedAssets;
}

bool UMingAIAssetGenerator::DeleteAsset(const FString& AssetID)
{
    for (int32 i = 0; i < GeneratedAssets.Num(); ++i)
    {
        if (GeneratedAssets[i].AssetID == AssetID)
        {
            // 刪除文件
            if (!GeneratedAssets[i].AssetPath.IsEmpty())
            {
                IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
                PlatformFile.DeleteFile(*GeneratedAssets[i].AssetPath);
            }
            
            if (!GeneratedAssets[i].ThumbnailPath.IsEmpty())
            {
                IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
                PlatformFile.DeleteFile(*GeneratedAssets[i].ThumbnailPath);
            }
            
            GeneratedAssets.RemoveAt(i);
            
            UE_LOG(LogTemp, Log, TEXT("AI asset deleted: %s"), *AssetID);
            return true;
        }
    }
    
    return false;
}

void UMingAIAssetGenerator::SetStableDiffusionEndpoint(const FString& Endpoint)
{
    StableDiffusionEndpoint = Endpoint;
    UE_LOG(LogTemp, Log, TEXT("Stable Diffusion endpoint updated: %s"), *Endpoint);
}

void UMingAIAssetGenerator::SetAIVAEndpoint(const FString& Endpoint)
{
    AIVAEndpoint = Endpoint;
    UE_LOG(LogTemp, Log, TEXT("AIVA endpoint updated: %s"), *Endpoint);
}

FString UMingAIAssetGenerator::GetRepublicEraPrompt(ERepublicEraStyle EraStyle, EAIAssetType AssetType)
{
    FString EraPrompt;
    FString TypePrompt;
    
    // 民國時期風格提示詞
    switch (EraStyle)
    {
    case ERepublicEraStyle::EarlyRepublic:
        EraPrompt = TEXT("1912-1928 Chinese Republic era, early republican period, traditional Chinese clothing mixed with Western influences, revolutionary atmosphere");
        break;
        
    case ERepublicEraStyle::NanjingDecade:
        EraPrompt = TEXT("1928-1937 Nanjing decade, golden age of Republic of China, Art Deco architecture, modern Chinese nationalism, traditional culture revival");
        break;
        
    case ERepublicEraStyle::WarOfResistance:
        EraPrompt = TEXT("1937-1945 War of Resistance against Japan, wartime atmosphere, military uniforms, destroyed buildings, refugee camps, patriotic themes");
        break;
        
    case ERepublicEraStyle::CivilWar:
        EraPrompt = TEXT("1945-1949 Chinese Civil War, post-war reconstruction, political tension, divided China, communist vs nationalist themes");
        break;
    }
    
    // 資產類型提示詞
    switch (AssetType)
    {
    case EAIAssetType::CharacterPortrait:
        TypePrompt = TEXT("character portrait, realistic, detailed face, historical accuracy, Chinese person");
        break;
        
    case EAIAssetType::Building:
        TypePrompt = TEXT("architecture, building, historical structure, Chinese architecture, period-accurate");
        break;
        
    case EAIAssetType::Landscape:
        TypePrompt = TEXT("landscape, scenery, Chinese countryside or city, historical setting, atmospheric");
        break;
        
    case EAIAssetType::Weapon:
        TypePrompt = TEXT("weapon, military equipment, historical firearm, Chinese military, period weapon");
        break;
        
    case EAIAssetType::Uniform:
        TypePrompt = TEXT("uniform, military clothing, Chinese army uniform, period costume, detailed fabric");
        break;
        
    case EAIAssetType::Prop:
        TypePrompt = TEXT("prop, object, historical artifact, Chinese item, period detail");
        break;
        
    case EAIAssetType::BackgroundMusic:
        TypePrompt = TEXT("background music, orchestral, Chinese instruments, patriotic melody, dramatic score");
        break;
        
    case EAIAssetType::SoundEffect:
        TypePrompt = TEXT("sound effect, ambient sound, wartime audio, period-accurate sounds");
        break;
        
    case EAIAssetType::VoiceLine:
        TypePrompt = TEXT("voice line, Chinese dialogue, historical speech, period accent");
        break;
    }
    
    return EraPrompt + TEXT(", ") + TypePrompt;
}

UMingAIAssetGenerator* UMingAIAssetGenerator::Get()
{
    return Instance;
}

FString UMingAIAssetGenerator::GenerateAssetID()
{
    return FString::Printf(TEXT("AIAsset_%s_%d"), 
        *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), 
        FMath::RandRange(1000, 9999));
}

FString UMingAIAssetGenerator::GenerateStableDiffusionPrompt(const FAIAssetGenerationParams& Params)
{
    FString Prompt = Params.DetailedPrompt;
    
    if (Prompt.IsEmpty())
    {
        Prompt = GetRepublicEraPrompt(Params.EraStyle, Params.AssetType) + TEXT(" ") + Params.BaseDescription;
    }
    
    // 添加通用質量提升詞
    Prompt += TEXT(", high quality, detailed, historical accuracy, photorealistic, 8k");
    
    return Prompt;
}

FAIAssetGenerationResult UMingAIAssetGenerator::CallStableDiffusionAPI(const FString& Prompt, const FString& NegativePrompt, int32 Width, int32 Height)
{
    FAIAssetGenerationResult Result;
    
    // 模擬API調用 - 實際實作需要HTTP請求
    UE_LOG(LogTemp, Log, TEXT("Calling Stable Diffusion API..."));
    UE_LOG(LogTemp, Log, TEXT("Prompt: %s"), *Prompt);
    UE_LOG(LogTemp, Log, TEXT("Negative Prompt: %s"), *NegativePrompt);
    UE_LOG(LogTemp, Log, TEXT("Size: %dx%d"), Width, Height);
    
    // 模擬生成時間
    FPlatformProcess::Sleep(2.0f);
    
    // 模擬成功結果
    Result.bSuccess = true;
    Result.AssetPath = FString::Printf(TEXT("GeneratedAssets/Images/%s.png"), *Result.AssetID);
    
    UE_LOG(LogTemp, Log, TEXT("Stable Diffusion API call simulated"));
    
    return Result;
}

FAIAssetGenerationResult UMingAIAssetGenerator::CallAIVAAPI(const FString& MusicDescription, float Duration, const FString& Style)
{
    FAIAssetGenerationResult Result;
    
    // 模擬API調用
    UE_LOG(LogTemp, Log, TEXT("Calling AIVA API..."));
    UE_LOG(LogTemp, Log, TEXT("Description: %s"), *MusicDescription);
    UE_LOG(LogTemp, Log, TEXT("Duration: %.1f seconds"), Duration);
    UE_LOG(LogTemp, Log, TEXT("Style: %s"), *Style);
    
    // 模擬生成時間
    FPlatformProcess::Sleep(3.0f);
    
    // 模擬成功結果
    Result.bSuccess = true;
    Result.AssetPath = FString::Printf(TEXT("GeneratedAssets/Music/%s.wav"), *Result.AssetID);
    
    UE_LOG(LogTemp, Log, TEXT("AIVA API call simulated"));
    
    return Result;
}

bool UMingAIAssetGenerator::SaveGeneratedAsset(const FAIAssetGenerationResult& Result)
{
    // 模擬保存資產
    UE_LOG(LogTemp, Log, TEXT("Saving generated asset: %s"), *Result.AssetPath);
    
    // 實際實作需要將API返回的數據保存到文件
    return true;
}

FString UMingAIAssetGenerator::GenerateThumbnail(const FString& AssetPath, EAIAssetType AssetType)
{
    // 模擬生成縮略圖
    FString ThumbnailPath = FString::Printf(TEXT("GeneratedAssets/Thumbnails/%s_thumb.png"), *AssetPath);
    
    UE_LOG(LogTemp, Log, TEXT("Generating thumbnail: %s"), *ThumbnailPath);
    
    return ThumbnailPath;
}

bool UMingAIAssetGenerator::CreateAssetDirectory(EAIAssetType AssetType)
{
    FString DirectoryPath;
    
    switch (AssetType)
    {
    case EAIAssetType::CharacterPortrait:
    case EAIAssetType::Building:
    case EAIAssetType::Landscape:
    case EAIAssetType::Weapon:
    case EAIAssetType::Uniform:
    case EAIAssetType::Prop:
        DirectoryPath = TEXT("GeneratedAssets/Images");
        break;
        
    case EAIAssetType::BackgroundMusic:
    case EAIAssetType::SoundEffect:
        DirectoryPath = TEXT("GeneratedAssets/Music");
        break;
        
    case EAIAssetType::VoiceLine:
        DirectoryPath = TEXT("GeneratedAssets/Voice");
        break;
    }
    
    // 創建縮略圖目錄
    FString ThumbnailPath = TEXT("GeneratedAssets/Thumbnails");
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    bool bSuccess = PlatformFile.CreateDirectoryTree(*DirectoryPath);
    bSuccess &= PlatformFile.CreateDirectoryTree(*ThumbnailPath);
    
    UE_LOG(LogTemp, Log, TEXT("Created asset directory: %s"), *DirectoryPath);
    
    return bSuccess;
}

bool UMingAIAssetGenerator::ValidateAPIResponse(const FString& Response)
{
    // 驗證API響應
    return !Response.IsEmpty();
}

FAIAssetGenerationResult UMingAIAssetGenerator::HandleGenerationError(const FString& Error)
{
    FAIAssetGenerationResult Result;
    Result.bSuccess = false;
    Result.ErrorMessage = Error;
    
    UE_LOG(LogTemp, Error, TEXT("AI asset generation error: %s"), *Error);
    
    return Result;
}
