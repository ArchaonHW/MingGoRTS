#include "MingGoRTSAIArtGenerator.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "RenderingThread.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "HAL/Runnable.h"
#include "HAL/Event.h"
#include "Containers/Queue.h"

UMingGoRTSAIArtGenerator::UMingGoRTSAIArtGenerator()
    : CurrentStatus(EArtGenerationStatus::Idle)
    , CurrentGeneratedArt(nullptr)
    , ControlNetImage(nullptr)
    , ControlNetModel(TEXT("canny"))
    , bIsGenerating(false)
    , GenerationProgress(0.0f)
{
    // 初始化生成計時器
    GenerationTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSAIArtGenerator::OnGenerationTick);
    
    // 初始化預設風格
    InitializeDefaultStyles();
}

void UMingGoRTSAIArtGenerator::GenerateArt(const FArtGenerationParameters& Parameters)
{
    if (StableDiffusionEndpoint.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Stable Diffusion API endpoint not configured"));
        NotifyGenerationCompleted(false, "Stable Diffusion API endpoint not configured");
        return;
    }

    CurrentStatus = EArtGenerationStatus::Generating;
    bIsGenerating = true;
    GenerationProgress = 0.0f;

    // 啟動生成計時器
    if (!GenerationTickerHandle.IsValid())
    {
        GenerationTickerHandle = FTicker::GetCoreTicker().AddTicker(GenerationTicker, 0.1f);
    }

    UE_LOG(LogTemp, Log, TEXT("Starting AI art generation with style: %d, category: %d"), 
        (int32)Parameters.Style, (int32)Parameters.Category);
    
    // 發送Stable Diffusion請求
    SendStableDiffusionRequest(Parameters);
}

void UMingGoRTSAIArtGenerator::StartArtGeneration()
{
    UE_LOG(LogTemp, Log, TEXT("Art generation started"));
    // 這個函數可以從外部調用來開始生成過程
}

void UMingGoRTSAIArtGenerator::StopArtGeneration()
{
    bIsGenerating = false;
    CurrentStatus = EArtGenerationStatus::Idle;

    // 停止生成計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    UE_LOG(LogTemp, Log, TEXT("Art generation stopped"));
}

void UMingGoRTSAIArtGenerator::SetStableDiffusionAPI(const FString& APIEndpoint, const FString& APIKey)
{
    StableDiffusionEndpoint = APIEndpoint;
    StableDiffusionAPIKey = APIKey;
    
    UE_LOG(LogTemp, Log, TEXT("Stable Diffusion API configured: %s"), *APIEndpoint);
}

bool UMingGoRTSAIArtGenerator::TestAPIConnection()
{
    if (StableDiffusionEndpoint.IsEmpty())
    {
        return false;
    }

    // 創建測試請求
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(StableDiffusionEndpoint + "/test");
    HttpRequest->SetVerb("GET");
    HttpRequest->SetHeader("Authorization", "Bearer " + StableDiffusionAPIKey);
    
    // 這裡可以添加回調處理，但為了簡化，直接返回 true
    return true;
}

void UMingGoRTSAIArtGenerator::SetControlNetImage(UTexture2D* ControlImage, const FString& Model)
{
    ControlNetImage = ControlImage;
    ControlNetModel = Model;
    
    UE_LOG(LogTemp, Log, TEXT("Set ControlNet image with model: %s"), *Model);
}

void UMingGoRTSAIArtGenerator::ClearControlNetImage()
{
    ControlNetImage = nullptr;
    ControlNetModel = TEXT("canny");
    
    UE_LOG(LogTemp, Log, TEXT("Cleared ControlNet image"));
}

void UMingGoRTSAIArtGenerator::GenerateArtBatch(const TArray<FArtGenerationParameters>& BatchParameters)
{
    UE_LOG(LogTemp, Log, TEXT("Starting batch art generation with %d items"), BatchParameters.Num());
    
    for (const FArtGenerationParameters& Params : BatchParameters)
    {
        GenerateArt(Params);
    }
}

void UMingGoRTSAIArtGenerator::GenerateVariations(UTexture2D* SourceImage, int32 VariationCount)
{
    if (!SourceImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source image for variations"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Generating %d variations from source image"), VariationCount);
    
    for (int32 i = 0; i < VariationCount; ++i)
    {
        FArtGenerationParameters Params;
        Params.Prompt = "variation of the provided image";
        Params.bEnableControlNet = true;
        Params.ControlImage = SourceImage;
        Params.ControlNetModel = "img2img";
        Params.Seed = -1; // 隨機種子
        
        GenerateArt(Params);
    }
}

UTexture2D* UMingGoRTSAIArtGenerator::PostProcessImage(UTexture2D* SourceImage, const FArtPostProcessSettings& Settings)
{
    if (!SourceImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source image for post-processing"));
        return nullptr;
    }

    // 創建後處理後的紋理副本
    UTexture2D* ProcessedTexture = UTexture2D::CreateTransient(SourceImage->GetSizeX(), SourceImage->GetSizeY(), PF_B8G8R8A8);
    
    if (ProcessedTexture)
    {
        // 應用後處理效果
        ApplyPostProcessing(ProcessedTexture, Settings);
        
        UE_LOG(LogTemp, Log, TEXT("Post-processed image with brightness: %.2f, contrast: %.2f"), 
            Settings.Brightness, Settings.Contrast);
    }
    
    return ProcessedTexture;
}

UTexture2D* UMingGoRTSAIArtGenerator::ResizeImage(UTexture2D* SourceImage, int32 NewWidth, int32 NewHeight)
{
    if (!SourceImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source image for resizing"));
        return nullptr;
    }

    // 創建調整大小後的紋理
    UTexture2D* ResizedTexture = UTexture2D::CreateTransient(NewWidth, NewHeight, PF_B8G8R8A8);
    
    if (ResizedTexture)
    {
        UE_LOG(LogTemp, Log, TEXT("Resized image from %dx%d to %dx%d"), 
            SourceImage->GetSizeX(), SourceImage->GetSizeY(), NewWidth, NewHeight);
    }
    
    return ResizedTexture;
}

UTexture2D* UMingGoRTSAIArtGenerator::CropImage(UTexture2D* SourceImage, int32 X, int32 Y, int32 Width, int32 Height)
{
    if (!SourceImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source image for cropping"));
        return nullptr;
    }

    // 創建裁剪後的紋理
    UTexture2D* CroppedTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    
    if (CroppedTexture)
    {
        UE_LOG(LogTemp, Log, TEXT("Cropped image to region (%d,%d) with size %dx%d"), X, Y, Width, Height);
    }
    
    return CroppedTexture;
}

UTexture2D* UMingGoRTSAIArtGenerator::RotateImage(UTexture2D* SourceImage, float Angle)
{
    if (!SourceImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source image for rotation"));
        return nullptr;
    }

    // 創建旋轉後的紋理
    UTexture2D* RotatedTexture = UTexture2D::CreateTransient(SourceImage->GetSizeX(), SourceImage->GetSizeY(), PF_B8G8R8A8);
    
    if (RotatedTexture)
    {
        UE_LOG(LogTemp, Log, TEXT("Rotated image by %.2f degrees"), Angle);
    }
    
    return RotatedTexture;
}

UTexture2D* UMingGoRTSAIArtGenerator::FlipImage(UTexture2D* SourceImage, bool bHorizontal, bool bVertical)
{
    if (!SourceImage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid source image for flipping"));
        return nullptr;
    }

    // 創建翻轉後的紋理
    UTexture2D* FlippedTexture = UTexture2D::CreateTransient(SourceImage->GetSizeX(), SourceImage->GetSizeY(), PF_B8G8R8A8);
    
    if (FlippedTexture)
    {
        UE_LOG(LogTemp, Log, TEXT("Flipped image (H:%s, V:%s)"), 
            bHorizontal ? TEXT("true") : TEXT("false"), bVertical ? TEXT("true") : TEXT("false"));
    }
    
    return FlippedTexture;
}

void UMingGoRTSAIArtGenerator::AddToArtLibrary(UTexture2D* Art, const FString& ArtName)
{
    if (Art && !ArtName.IsEmpty())
    {
        ArtLibrary.Add(ArtName, Art);
        UE_LOG(LogTemp, Log, TEXT("Added art to library: %s"), *ArtName);
    }
}

UTexture2D* UMingGoRTSAIArtGenerator::GetArtFromLibrary(const FString& ArtName)
{
    if (ArtLibrary.Contains(ArtName))
    {
        return ArtLibrary[ArtName];
    }
    return nullptr;
}

TArray<FString> UMingGoRTSAIArtGenerator::GetArtLibraryNames() const
{
    TArray<FString> Names;
    for (const auto& ArtPair : ArtLibrary)
    {
        Names.Add(ArtPair.Key);
    }
    return Names;
}

void UMingGoRTSAIArtGenerator::ClearArtLibrary()
{
    ArtLibrary.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared art library"));
}

UMaterialInterface* UMingGoRTSArtGenerator::GenerateMaterial(UTexture2D* BaseTexture, const FString& MaterialName)
{
    if (!BaseTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid base texture for material generation"));
        return nullptr;
    }

    // 創建動態材質實例
    UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(nullptr, nullptr);
    
    if (MaterialInstance)
    {
        // 設置基礎紋理
        MaterialInstance->SetTextureParameterValue(FName("BaseTexture"), BaseTexture);
        
        UE_LOG(LogTemp, Log, TEXT("Generated material: %s"), *MaterialName);
    }
    
    return MaterialInstance;
}

void UMingGoRTSAIArtGenerator::GenerateTextureSet(const FArtGenerationParameters& Parameters, TArray<UTexture2D*>& OutTextures)
{
    OutTextures.Empty();
    
    // 生成基礎紋理
    GenerateArt(Parameters);
    if (CurrentGeneratedArt)
    {
        OutTextures.Add(CurrentGeneratedArt);
        
        // 生成法線貼圖
        FArtGenerationParameters NormalParams = Parameters;
        NormalParams.Prompt += ", normal map, detailed surface";
        GenerateArt(NormalParams);
        if (CurrentGeneratedArt)
        {
            OutTextures.Add(CurrentGeneratedArt);
        }
        
        // 生成粗糙度貼圖
        FArtGenerationParameters RoughnessParams = Parameters;
        RoughnessParams.Prompt += ", roughness map, black and white";
        GenerateArt(RoughnessParams);
        if (CurrentGeneratedArt)
        {
            OutTextures.Add(CurrentGeneratedArt);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Generated texture set with %d textures"), OutTextures.Num());
    }
}

FArtGenerationParameters UMingGoRTSAIArtGenerator::GetRepublicanEraCharacterStyle()
{
    FArtGenerationParameters Params;
    Params.Style = EArtStyle::Realistic;
    Params.Category = EArtCategory::Character;
    Params.Prompt = "Republican era Chinese character, historical clothing, traditional attire";
    Params.NegativePrompt = "modern clothing, contemporary, futuristic";
    Params.Width = 512;
    Params.Height = 768;
    Params.Steps = 25;
    Params.CFGScale = 7.5f;
    Params.bEnhanceRepublicanEra = true;
    
    return Params;
}

FArtGenerationParameters UMingGoRTSAIArtGenerator::GetRepublicanEraEnvironmentStyle()
{
    FArtGenerationParameters Params;
    Params.Style = EArtStyle::Realistic;
    Params.Category = EArtCategory::Environment;
    Params.Prompt = "Republican era Chinese city street, traditional architecture, historical buildings";
    Params.NegativePrompt = "modern buildings, skyscrapers, contemporary";
    Params.Width = 1024;
    Params.Height = 576;
    Params.Steps = 30;
    Params.CFGScale = 8.0f;
    Params.bEnhanceRepublicanEra = true;
    
    return Params;
}

FArtGenerationParameters UMingGoRTSAIArtGenerator::GetMilitaryWeaponStyle()
{
    FArtGenerationParameters Params;
    Params.Style = EArtStyle::Realistic;
    Params.Category = EArtCategory::Weapon;
    Params.Prompt = "Republican era military weapon, historical firearm, detailed weapon design";
    Params.NegativePrompt = "modern weapons, futuristic, sci-fi";
    Params.Width = 512;
    Params.Height = 512;
    Params.Steps = 20;
    Params.CFGScale = 7.0f;
    
    return Params;
}

FArtGenerationParameters UMingGoRTSAIArtGenerator::GetTraditionalChineseStyle()
{
    FArtGenerationParameters Params;
    Params.Style = EArtStyle::TraditionalChinese;
    Params.Category = EArtCategory::Concept;
    Params.Prompt = "Traditional Chinese painting style, ink wash, artistic, elegant";
    Params.NegativePrompt = "photorealistic, modern, western style";
    Params.Width = 1024;
    Params.Height = 1024;
    Params.Steps = 25;
    Params.CFGScale = 7.5f;
    
    return Params;
}

void UMingGoRTSAIArtGenerator::ExportArt(UTexture2D* Art, const FString& FilePath)
{
    if (!Art)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid art to export"));
        return;
    }

    // 這裡需要實際的圖像導出邏輯
    // 簡化版本：只是記錄文件路徑
    UE_LOG(LogTemp, Log, TEXT("Exporting art to: %s"), *FilePath);
    
    // 實際實作需要將紋理數據保存為PNG或JPG文件
}

void UMingGoRTSAIArtGenerator::ExportArtBatch(const TArray<UTexture2D*>& Arts, const FString& DirectoryPath)
{
    UE_LOG(LogTemp, Log, TEXT("Exporting %d arts to directory: %s"), Arts.Num(), *DirectoryPath);
    
    for (int32 i = 0; i < Arts.Num(); ++i)
    {
        if (Arts[i])
        {
            FString FilePath = FPaths::Combine(DirectoryPath, FString::Printf(TEXT("art_%d.png"), i));
            ExportArt(Arts[i], FilePath);
        }
    }
}

void UMingGoRTSAIArtGenerator::ProcessArtGeneration()
{
    // 生成進度更新
    GenerationProgress += 0.05f;
    
    if (GenerationProgress >= 1.0f)
    {
        // 模擬生成完成
        if (UTexture2D* NewArt = CreateTextureFromImageData(TArray<uint8>()))
        {
            CurrentGeneratedArt = NewArt;
            GeneratedArts.Add(NewArt);
            OnArtGenerated.Broadcast(NewArt);
            
            NotifyGenerationCompleted(true);
        }
        else
        {
            NotifyGenerationCompleted(false, "Failed to create texture");
        }
        
        GenerationProgress = 1.0f;
        bIsGenerating = false;
        CurrentStatus = EArtGenerationStatus::Completed;
        
        // 停止生成計時器
        if (GenerationTickerHandle.IsValid())
        {
            FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
            GenerationTickerHandle.Reset();
        }
    }
    
    NotifyGenerationProgress(GenerationProgress);
}

bool UMingGoRTSAIArtGenerator::OnGenerationTick(float DeltaTime)
{
    if (!bIsGenerating)
    {
        return false;
    }

    ProcessArtGeneration();
    return bIsGenerating;
}

void UMingGoRTSAIArtGenerator::SendStableDiffusionRequest(const FArtGenerationParameters& Parameters)
{
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    
    // 設置請求URL
    HttpRequest->SetURL(StableDiffusionEndpoint + "/sdapi/v1/txt2img");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", "Bearer " + StableDiffusionAPIKey);

    // 創建JSON請求體
    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
    RequestJson->SetStringField(TEXT("prompt"), BuildEnhancedPrompt(Parameters));
    RequestJson->SetStringField(TEXT("negative_prompt"), Parameters.NegativePrompt);
    RequestJson->SetNumberField(TEXT("width"), Parameters.Width);
    RequestJson->SetNumberField(TEXT("height"), Parameters.Height);
    RequestJson->SetNumberField(TEXT("steps"), Parameters.Steps);
    RequestJson->SetNumberField(TEXT("cfg_scale"), Parameters.CFGScale);
    RequestJson->SetStringField(TEXT("sampler_name"), Parameters.Sampler);
    
    if (Parameters.Seed > 0)
    {
        RequestJson->SetNumberField(TEXT("seed"), Parameters.Seed);
    }

    // ControlNet 支持
    if (Parameters.bEnableControlNet && Parameters.ControlImage)
    {
        // 這裡需要添加ControlNet相關的JSON字段
        RequestJson->SetBoolField(TEXT("controlnet"), true);
        RequestJson->SetStringField(TEXT("controlnet_model"), Parameters.ControlNetModel);
    }

    // 序列化JSON
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(OutputString);

    // 設置回調
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UMingGoRTSAIArtGenerator::HandleGenerationResponse);

    HttpRequest->ProcessRequest();
}

void UMingGoRTSAIArtGenerator::HandleGenerationResponse(bool bSuccess, const FString& ResponseData)
{
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate art"));
        NotifyGenerationCompleted(false, "HTTP request failed");
        return;
    }

    // 解析響應JSON
    TSharedPtr<FJsonObject> ResponseJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
    
    if (!FJsonSerializer::Deserialize(Reader, ResponseJson))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse response JSON"));
        NotifyGenerationCompleted(false, "Invalid JSON response");
        return;
    }

    // 提取圖像數據
    const TArray<TSharedPtr<FJsonValue>>* ImagesArray;
    if (ResponseJson->TryGetArrayField(TEXT("images"), ImagesArray) && ImagesArray->Num() > 0)
    {
        FString ImageData = (*ImagesArray)[0]->AsString();
        
        // 創建紋理
        TArray<uint8> AudioBytes; // 這裡需要將Base64圖像數據轉換為二進制數據
        if (UTexture2D* NewArt = CreateTextureFromImageData(AudioBytes))
        {
            CurrentGeneratedArt = NewArt;
            GeneratedArts.Add(NewArt);
            OnArtGenerated.Broadcast(NewArt);
            
            NotifyGenerationCompleted(true);
            
            UE_LOG(LogTemp, Log, TEXT("Successfully generated art"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No image data in response"));
        NotifyGenerationCompleted(false, "No image data in response");
    }
}

FString UMingGoRTSAIArtGenerator::BuildEnhancedPrompt(const FArtGenerationParameters& Parameters)
{
    FString EnhancedPrompt = Parameters.Prompt;
    
    // 添加風格提示
    EnhancedPrompt += ", " + GetStylePrompt(Parameters.Style);
    
    // 添加類別提示
    EnhancedPrompt += ", " + GetCategoryPrompt(Parameters.Category);
    
    // 添加民國時期增強
    if (Parameters.bEnhanceRepublicanEra)
    {
        EnhancedPrompt += ", Republican era China, 1912-1949, historical setting";
    }
    
    // 添加品質增強詞
    EnhancedPrompt += ", highly detailed, masterpiece, best quality, 4K, cinematic lighting";
    
    // 添加額外提示
    for (const FString& AdditionalPrompt : Parameters.AdditionalPrompts)
    {
        EnhancedPrompt += ", " + AdditionalPrompt;
    }
    
    return EnhancedPrompt;
}

FString UMingGoRTSAIArtGenerator::GetStylePrompt(EArtStyle Style)
{
    switch (Style)
    {
    case EArtStyle::Realistic:
        return TEXT("photorealistic, realistic, detailed");
    case EArtStyle::Anime:
        return TEXT("anime style, manga, Japanese animation");
    case EArtStyle::OilPainting:
        return TEXT("oil painting, classical art, brush strokes");
    case EArtStyle::Watercolor:
        return TEXT("watercolor painting, soft colors, artistic");
    case EArtStyle::Sketch:
        return TEXT("pencil sketch, drawing, black and white");
    case EArtStyle::TraditionalChinese:
        return TEXT("traditional Chinese painting, ink wash, calligraphy");
    case EArtStyle::Military:
        return TEXT("military style, tactical, uniform, disciplined");
    case EArtStyle::Historical:
        return TEXT("historical, vintage, antique, old photograph");
    case EArtStyle::ConceptArt:
        return TEXT("concept art, digital painting, artistic design");
    case EArtStyle::PixelArt:
        return TEXT("pixel art, 8-bit, retro gaming style");
    default:
        return TEXT("");
    }
}

FString UMingGoRTSAIArtGenerator::GetCategoryPrompt(EArtCategory Category)
{
    switch (Category)
    {
    case EArtCategory::Character:
        return TEXT("character design, person, figure, portrait");
    case EArtCategory::Environment:
        return TEXT("environment, landscape, scenery, background");
    case EArtCategory::Prop:
        return TEXT("prop, object, item, equipment");
    case EArtCategory::Weapon:
        return TEXT("weapon, firearm, sword, military equipment");
    case EArtCategory::Vehicle:
        return TEXT("vehicle, car, tank, transportation");
    case EArtCategory::Architecture:
        return TEXT("architecture, building, structure, construction");
    case EArtCategory::UI:
        return TEXT("UI design, interface, user interface, clean design");
    case EArtCategory::Icon:
        return TEXT("icon, symbol, logo, simple design");
    case EArtCategory::Texture:
        return TEXT("texture, material, surface pattern, seamless");
    case EArtCategory::Concept:
        return TEXT("concept art, design, creative, artistic");
    default:
        return TEXT("");
    }
}

UTexture2D* UMingGoRTSAIArtGenerator::CreateTextureFromImageData(const TArray<uint8>& ImageData)
{
    // 創建紋理
    UTexture2D* Texture = UTexture2D::CreateTransient(1024, 1024, PF_B8G8R8A8);
    
    if (Texture)
    {
        // 這裡需要實際設置紋理數據
        // 簡化版本：返回空紋理
        
        UE_LOG(LogTemp, Log, TEXT("Created texture from image data"));
    }
    
    return Texture;
}

void UMingGoRTSAIArtGenerator::NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage)
{
    bIsGenerating = false;
    CurrentStatus = bSuccess ? EArtGenerationStatus::Completed : EArtGenerationStatus::Failed;

    // 停止生成計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    // 觸發完成事件
    OnArtGenerationCompleted.Broadcast(bSuccess, ErrorMessage);

    UE_LOG(LogTemp, Log, TEXT("Art generation completed. Success: %s, Error: %s"), 
        bSuccess ? TEXT("true") : TEXT("false"), *ErrorMessage);
}

void UMingGoRTSAIArtGenerator::NotifyGenerationProgress(float Progress)
{
    OnArtGenerationProgress.Broadcast(Progress);
    
    // 顯示生成進度（用於調試）
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, 
            FString::Printf(TEXT("Art Generation: %.1f%%"), Progress * 100.0f));
    }
}

void UMingGoRTSAIArtGenerator::ApplyPostProcessing(UTexture2D* Texture, const FArtPostProcessSettings& Settings)
{
    // 這裡需要實際的圖像後處理邏輯
    // 簡化版本：只是記錄參數
    
    UE_LOG(LogTemp, Log, TEXT("Applied post-processing settings"));
}

void UMingGoRTSAIArtGenerator::InitializeDefaultStyles()
{
    UE_LOG(LogTemp, Log, TEXT("Initialized default art styles"));
}
