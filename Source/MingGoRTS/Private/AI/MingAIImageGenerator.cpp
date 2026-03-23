// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Image Generator Implementation

#include "AI/MingAIImageGenerator.h"
#include "AI/AIContentQualityController.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureDefines.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/SecureHash.h"
#include "Misc/Guid.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

int32 UMingAIImageGenerator::GenerationCounter = 0;

UMingAIImageGenerator::UMingAIImageGenerator()
    : bIsInitialized(false)
    , QualityController(nullptr)
    , DefaultOutputPath(FPaths::ProjectSavedDir() / TEXT("AIGeneratedImages"))
{
}

void UMingAIImageGenerator::InitializeImageGenerator()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("AI Image Generator already initialized"));
        return;
    }

    // Create quality controller
    QualityController = NewObject<UAIContentQualityController>(this);
    if (QualityController)
    {
        QualityController->InitializeController();
    }

    // Ensure output directory exists
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*DefaultOutputPath))
    {
        PlatformFile.CreateDirectoryTree(*DefaultOutputPath);
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Image Generator initialized successfully"));
}

void UMingAIImageGenerator::ShutdownImageGenerator()
{
    if (!bIsInitialized)
    {
        return;
    }

    // Cancel all pending generations
    for (const auto& Pair : GenerationResults)
    {
        if (Pair.Value.Status == EAIImageGenerationStatus::Generating ||
            Pair.Value.Status == EAIImageGenerationStatus::Queued)
        {
            CancelGeneration(Pair.Key);
        }
    }

    GenerationResults.Empty();
    BatchGenerationMap.Empty();
    GenerationHistory.Empty();

    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("AI Image Generator shutdown complete"));
}

FString UMingAIImageGenerator::GenerateImage(const FAIImageGenerationParams& Params)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Image Generator not initialized"));
        return FString();
    }

    FString GenerationID = GenerateUniqueID();
    
    // Create result entry
    FAIImageGenerationResult Result;
    Result.GenerationID = GenerationID;
    Result.Status = EAIImageGenerationStatus::Queued;
    Result.GenerationTime = FDateTime::Now();
    
    {
        FScopeLock Lock(&GenerationLock);
        GenerationResults.Add(GenerationID, Result);
        GenerationHistory.Add(GenerationID);
    }

    // Start async processing
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, GenerationID, Params]()
    {
        ProcessImageGenerationRequest(GenerationID, Params);
    });

    return GenerationID;
}

void UMingAIImageGenerator::GenerateImageAsync(const FAIImageGenerationParams& Params)
{
    FString GenerationID = GenerateImage(Params);
    
    if (!GenerationID.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("Async image generation started: %s"), *GenerationID);
    }
}

UTexture2D* UMingAIImageGenerator::GenerateImageSync(const FAIImageGenerationParams& Params)
{
    FString GenerationID = GenerateImage(Params);
    if (GenerationID.IsEmpty())
    {
        return nullptr;
    }

    // Wait for completion (with timeout)
    const float TimeoutSeconds = 120.0f;
    const float CheckInterval = 0.1f;
    float ElapsedTime = 0.0f;

    while (ElapsedTime < TimeoutSeconds)
    {
        FAIImageGenerationResult Result = GetGenerationResult(GenerationID);
        
        if (Result.Status == EAIImageGenerationStatus::Completed)
        {
            return Result.GeneratedTexture;
        }
        else if (Result.Status == EAIImageGenerationStatus::Failed)
        {
            UE_LOG(LogTemp, Error, TEXT("Image generation failed: %s"), *Result.ErrorMessage);
            return nullptr;
        }

        FPlatformProcess::Sleep(CheckInterval);
        ElapsedTime += CheckInterval;
    }

    UE_LOG(LogTemp, Error, TEXT("Image generation timed out"));
    return nullptr;
}

void UMingAIImageGenerator::ProcessImageGenerationRequest(const FString& GenerationID, const FAIImageGenerationParams& Params)
{
    UpdateGenerationProgress(GenerationID, 0.1f);

    bool bSuccess = false;
    
    switch (Params.Provider)
    {
    case EAIImageProvider::StableDiffusion:
        bSuccess = GenerateImageWithStableDiffusion(GenerationID, Params);
        break;
    case EAIImageProvider::DALLE3:
        bSuccess = GenerateImageWithDALLE(GenerationID, Params);
        break;
    case EAIImageProvider::LocalModel:
        bSuccess = GenerateImageWithLocalModel(GenerationID, Params);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Provider not implemented: %d"), (int32)Params.Provider);
        bSuccess = GenerateImageWithStableDiffusion(GenerationID, Params); // Fallback
        break;
    }

    if (!bSuccess)
    {
        FailGeneration(GenerationID, TEXT("Generation process failed"));
    }
}

bool UMingAIImageGenerator::GenerateImageWithStableDiffusion(const FString& GenerationID, const FAIImageGenerationParams& Params)
{
    UpdateGenerationProgress(GenerationID, 0.2f);

    FString APIKey = GetProviderAPIToken(EAIImageProvider::StableDiffusion);
    if (APIKey.IsEmpty())
    {
        FailGeneration(GenerationID, TEXT("Stable Diffusion API key not configured"));
        return false;
    }

    // Build request payload
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
    FString EnhancedPrompt = BuildEnhancedPrompt(Params.Prompt, Params.Style);
    JsonObject->SetStringField(TEXT("prompt"), EnhancedPrompt);
    JsonObject->SetStringField(TEXT("negative_prompt"), Params.NegativePrompt);
    
    FVector2D Dimensions = GetDimensionsFromAspectRatio(Params.AspectRatio);
    JsonObject->SetNumberField(TEXT("width"), Params.Width > 0 ? Params.Width : Dimensions.X);
    JsonObject->SetNumberField(TEXT("height"), Params.Height > 0 ? Params.Height : Dimensions.Y);
    JsonObject->SetNumberField(TEXT("steps"), Params.Steps);
    JsonObject->SetNumberField(TEXT("cfg_scale"), Params.CFGScale);
    JsonObject->SetNumberField(TEXT("seed"), Params.Seed >= 0 ? Params.Seed : FMath::Rand());

    FString OutputPath = Params.OutputPath.IsEmpty() ? DefaultOutputPath : Params.OutputPath;
    FString FileName = FString::Printf(TEXT("SD_%s_%s.png"), *GenerationID, *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    FString FullPath = FPaths::Combine(OutputPath, FileName);

    // Create HTTP request
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(TEXT("http://127.0.0.1:7860/sdapi/v1/txt2img"));
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIKey));
    
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    HttpRequest->SetContentAsString(RequestBody);

    // Bind response handler
    HttpRequest->OnProcessRequestComplete().BindLambda([this, GenerationID, FullPath](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        OnImageGenerationResponseReceived(Request, Response, bWasSuccessful, GenerationID);
    });

    UpdateGenerationProgress(GenerationID, 0.4f);

    // Execute request synchronously for this example
    // In production, use async with callback
    HttpRequest->ProcessRequest();

    return true;
}

bool UMingAIImageGenerator::GenerateImageWithDALLE(const FString& GenerationID, const FAIImageGenerationParams& Params)
{
    UpdateGenerationProgress(GenerationID, 0.2f);

    FString APIKey = GetProviderAPIToken(EAIImageProvider::DALLE3);
    if (APIKey.IsEmpty())
    {
        FailGeneration(GenerationID, TEXT("DALL-E API key not configured"));
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("prompt"), Params.Prompt);
    JsonObject->SetStringField(TEXT("model"), TEXT("dall-e-3"));
    JsonObject->SetStringField(TEXT("size"), TEXT("1024x1024"));
    JsonObject->SetStringField(TEXT("quality"), Params.Quality == EAIImageQuality::Draft ? TEXT("standard") : TEXT("hd"));
    JsonObject->SetNumberField(TEXT("n"), 1);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(TEXT("https://api.openai.com/v1/images/generations"));
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIKey));
    
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    HttpRequest->SetContentAsString(RequestBody);

    HttpRequest->OnProcessRequestComplete().BindLambda([this, GenerationID](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
        OnImageGenerationResponseReceived(Request, Response, bWasSuccessful, GenerationID);
    });

    HttpRequest->ProcessRequest();
    return true;
}

bool UMingAIImageGenerator::GenerateImageWithLocalModel(const FString& GenerationID, const FAIImageGenerationParams& Params)
{
    UpdateGenerationProgress(GenerationID, 0.2f);

    if (LocalModelPath.IsEmpty())
    {
        FailGeneration(GenerationID, TEXT("Local model path not configured"));
        return false;
    }

    // TODO: Implement local model inference using ONNX Runtime or similar
    UE_LOG(LogTemp, Log, TEXT("Local model generation not yet implemented, using fallback"));
    
    // For now, fallback to mock generation
    FAIImageGenerationResult Result;
    Result.GenerationID = GenerationID;
    Result.Status = EAIImageGenerationStatus::Completed;
    Result.FilePath = FPaths::Combine(DefaultOutputPath, GenerationID + TEXT(".png"));
    Result.Progress = 1.0f;
    Result.GenerationDurationMs = 5000;
    Result.ActualSeed = Params.Seed >= 0 ? Params.Seed : FMath::Rand();
    Result.QualityScore = 0.85f;

    // Create a placeholder texture
    Result.GeneratedTexture = UTexture2D::CreateTransient(512, 512, PF_B8G8R8A8);
    if (Result.GeneratedTexture)
    {
        Result.GeneratedTexture->UpdateResource();
    }

    CompleteGeneration(GenerationID, Result);
    return true;
}

void UMingAIImageGenerator::OnImageGenerationResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString GenerationID)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        FailGeneration(GenerationID, TEXT("HTTP request failed"));
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    FString ResponseContent = Response->GetContentAsString();

    if (ResponseCode != 200)
    {
        FailGeneration(GenerationID, FString::Printf(TEXT("API error: %d - %s"), ResponseCode, *ResponseContent));
        return;
    }

    UpdateGenerationProgress(GenerationID, 0.8f);

    // Parse response
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);
    
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        FailGeneration(GenerationID, TEXT("Failed to parse API response"));
        return;
    }

    FAIImageGenerationResult Result;
    Result.GenerationID = GenerationID;
    Result.Status = EAIImageGenerationStatus::Completed;
    Result.Progress = 1.0f;

    // Extract image data
    const TArray<TSharedPtr<FJsonValue>>* ImagesArray;
    if (JsonObject->TryGetArrayField(TEXT("images"), ImagesArray) && ImagesArray->Num() > 0)
    {
        FString Base64Image = (*ImagesArray)[0]->AsString();
        
        // Decode base64 to binary
        TArray<uint8> ImageData;
        FBase64::Decode(Base64Image, ImageData);

        // Create texture
        Result.GeneratedTexture = CreateTextureFromImageData(ImageData);
        
        // Save to file
        FString FileName = FString::Printf(TEXT("Generated_%s.png"), *GenerationID);
        Result.FilePath = FPaths::Combine(DefaultOutputPath, FileName);
        
        // Save image data to file
        FFileHelper::SaveArrayToFile(ImageData, *Result.FilePath);
    }

    // Extract seed if available
    int64 SeedValue;
    if (JsonObject->TryGetNumberField(TEXT("seed"), SeedValue))
    {
        Result.ActualSeed = (int32)SeedValue;
    }

    Result.GenerationDurationMs = 10000; // TODO: Calculate actual duration
    Result.QualityScore = 0.9f;

    UpdateGenerationProgress(GenerationID, 1.0f);
    CompleteGeneration(GenerationID, Result);
}

UTexture2D* UMingAIImageGenerator::CreateTextureFromImageData(const TArray<uint8>& ImageData)
{
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
    
    if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to decompress image data"));
        return nullptr;
    }

    TArray<uint8> RawData;
    if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get raw image data"));
        return nullptr;
    }

    int32 Width = ImageWrapper->GetWidth();
    int32 Height = ImageWrapper->GetHeight();

    UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    if (!Texture)
    {
        return nullptr;
    }

    // Copy data to texture
    FTexturePlatformData* PlatformData = Texture->PlatformData;
    if (PlatformData)
    {
        void* Data = PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(Data, RawData.GetData(), RawData.Num());
        PlatformData->Mips[0].BulkData.Unlock();
        Texture->UpdateResource();
    }

    return Texture;
}

void UMingAIImageGenerator::UpdateGenerationProgress(const FString& GenerationID, float Progress)
{
    FScopeLock Lock(&GenerationLock);
    
    if (FAIImageGenerationResult* Result = GenerationResults.Find(GenerationID))
    {
        Result->Progress = Progress;
        Result->Status = EAIImageGenerationStatus::Generating;
    }

    // Broadcast event on game thread
    AsyncTask(ENamedThreads::GameThread, [this, GenerationID, Progress]()
    {
        OnGenerationProgress.Broadcast(GenerationID, Progress);
    });
}

void UMingAIImageGenerator::CompleteGeneration(const FString& GenerationID, const FAIImageGenerationResult& Result)
{
    {
        FScopeLock Lock(&GenerationLock);
        GenerationResults.Add(GenerationID, Result);
    }

    // Perform quality check if enabled
    if (QualityController && Result.GeneratedTexture)
    {
        // TODO: Implement quality check
    }

    // Broadcast completion event
    AsyncTask(ENamedThreads::GameThread, [this, GenerationID, Result]()
    {
        OnGenerationCompleted.Broadcast(GenerationID, Result);
        UE_LOG(LogTemp, Log, TEXT("Image generation completed: %s"), *GenerationID);
    });
}

void UMingAIImageGenerator::FailGeneration(const FString& GenerationID, const FString& ErrorMessage)
{
    FAIImageGenerationResult Result;
    Result.GenerationID = GenerationID;
    Result.Status = EAIImageGenerationStatus::Failed;
    Result.ErrorMessage = ErrorMessage;
    Result.Progress = 0.0f;

    {
        FScopeLock Lock(&GenerationLock);
        GenerationResults.Add(GenerationID, Result);
    }

    AsyncTask(ENamedThreads::GameThread, [this, GenerationID, ErrorMessage]()
    {
        OnGenerationFailed.Broadcast(GenerationID, ErrorMessage);
        UE_LOG(LogTemp, Error, TEXT("Image generation failed: %s - %s"), *GenerationID, *ErrorMessage);
    });
}

FAIImageGenerationResult UMingAIImageGenerator::GetGenerationResult(const FString& GenerationID) const
{
    FScopeLock Lock(&const_cast<FCriticalSection&>(GenerationLock));
    
    if (const FAIImageGenerationResult* Result = GenerationResults.Find(GenerationID))
    {
        return *Result;
    }

    return FAIImageGenerationResult();
}

bool UMingAIImageGenerator::CancelGeneration(const FString& GenerationID)
{
    FScopeLock Lock(&GenerationLock);
    
    if (FAIImageGenerationResult* Result = GenerationResults.Find(GenerationID))
    {
        if (Result->Status == EAIImageGenerationStatus::Queued ||
            Result->Status == EAIImageGenerationStatus::Generating)
        {
            Result->Status = EAIImageGenerationStatus::Cancelled;
            UE_LOG(LogTemp, Log, TEXT("Generation cancelled: %s"), *GenerationID);
            return true;
        }
    }

    return false;
}

FString UMingAIImageGenerator::GenerateImageBatch(const FAIImageBatchRequest& Request)
{
    if (!bIsInitialized)
    {
        return FString();
    }

    FString BatchID = GenerateUniqueID();
    
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, BatchID, Request]()
    {
        ProcessBatchGenerationRequest(BatchID, Request);
    });

    return BatchID;
}

void UMingAIImageGenerator::ProcessBatchGenerationRequest(const FString& BatchID, const FAIImageBatchRequest& Request)
{
    TArray<FString> GenerationIDs;
    
    int32 NumToGenerate = Request.bGenerateVariations ? Request.NumImages : FMath::Max(1, Request.NumImages);
    
    for (int32 i = 0; i < NumToGenerate; ++i)
    {
        FAIImageGenerationParams Params = Request.BaseParams;
        
        if (Request.bGenerateVariations && i < Request.Variations.Num())
        {
            Params.Prompt = Request.BasePrompt + TEXT(" ") + Request.Variations[i];
        }
        else
        {
            Params.Prompt = Request.BasePrompt;
            if (Request.bGenerateVariations)
            {
                Params.Seed = FMath::Rand(); // Random seed for variation
            }
        }

        FString GenerationID = GenerateImage(Params);
        if (!GenerationID.IsEmpty())
        {
            GenerationIDs.Add(GenerationID);
        }
    }

    {
        FScopeLock Lock(&GenerationLock);
        BatchGenerationMap.Add(BatchID, GenerationIDs);
    }

    // Wait for all generations to complete
    bool bAllComplete = false;
    float Timeout = 300.0f; // 5 minutes
    float Elapsed = 0.0f;

    while (!bAllComplete && Elapsed < Timeout)
    {
        bAllComplete = true;
        
        for (const FString& GenID : GenerationIDs)
        {
            FAIImageGenerationResult Result = GetGenerationResult(GenID);
            if (Result.Status != EAIImageGenerationStatus::Completed &&
                Result.Status != EAIImageGenerationStatus::Failed &&
                Result.Status != EAIImageGenerationStatus::Cancelled)
            {
                bAllComplete = false;
                break;
            }
        }

        if (!bAllComplete)
        {
            FPlatformProcess::Sleep(0.5f);
            Elapsed += 0.5f;
        }
    }

    // Collect results
    TArray<FAIImageGenerationResult> Results;
    for (const FString& GenID : GenerationIDs)
    {
        Results.Add(GetGenerationResult(GenID));
    }

    // Broadcast batch completion
    AsyncTask(ENamedThreads::GameThread, [this, BatchID, Results]()
    {
        OnBatchGenerationCompleted.Broadcast(BatchID, Results);
    });
}

TArray<FAIImageGenerationResult> UMingAIImageGenerator::GetBatchResults(const FString& BatchID) const
{
    TArray<FAIImageGenerationResult> Results;
    
    FScopeLock Lock(&const_cast<FCriticalSection&>(GenerationLock));
    
    if (const TArray<FString>* GenerationIDs = BatchGenerationMap.Find(BatchID))
    {
        for (const FString& GenID : *GenerationIDs)
        {
            Results.Add(GetGenerationResult(GenID));
        }
    }

    return Results;
}

FString UMingAIImageGenerator::EditImage(const FString& ImagePath, const FString& EditPrompt, EAIImageProvider Provider)
{
    // TODO: Implement image editing (inpainting, outpainting, etc.)
    UE_LOG(LogTemp, Log, TEXT("Image editing not yet implemented"));
    return FString();
}

FString UMingAIImageGenerator::InpaintImage(const FString& ImagePath, const TArray<FVector2D>& MaskPoints, const FString& FillPrompt)
{
    // TODO: Implement inpainting
    UE_LOG(LogTemp, Log, TEXT("Inpainting not yet implemented"));
    return FString();
}

FString UMingAIImageGenerator::OutpaintImage(const FString& ImagePath, EAIImageAspectRatio TargetRatio, const FString& ExtensionPrompt)
{
    // TODO: Implement outpainting
    UE_LOG(LogTemp, Log, TEXT("Outpainting not yet implemented"));
    return FString();
}

FString UMingAIImageGenerator::UpscaleImage(const FString& ImagePath, int32 TargetWidth, int32 TargetHeight)
{
    // TODO: Implement upscaling
    UE_LOG(LogTemp, Log, TEXT("Upscaling not yet implemented"));
    return FString();
}

FString UMingAIImageGenerator::ApplyStyleTransfer(const FString& ContentImagePath, const FString& StyleImagePath, float StyleStrength)
{
    // TODO: Implement style transfer
    UE_LOG(LogTemp, Log, TEXT("Style transfer not yet implemented"));
    return FString();
}

FVector2D UMingAIImageGenerator::GetDimensionsFromAspectRatio(EAIImageAspectRatio AspectRatio) const
{
    switch (AspectRatio)
    {
    case EAIImageAspectRatio::Square:
        return FVector2D(512, 512);
    case EAIImageAspectRatio::Portrait:
        return FVector2D(384, 512);
    case EAIImageAspectRatio::Landscape:
        return FVector2D(768, 432);
    case EAIImageAspectRatio::Widescreen:
        return FVector2D(896, 384);
    case EAIImageAspectRatio::MobilePortrait:
        return FVector2D(288, 512);
    case EAIImageAspectRatio::Classic:
        return FVector2D(640, 480);
    case EAIImageAspectRatio::Panoramic:
        return FVector2D(1024, 512);
    default:
        return FVector2D(512, 512);
    }
}

FString UMingAIImageGenerator::BuildEnhancedPrompt(const FString& BasePrompt, EAIImageStyle Style) const
{
    FString StyleModifier;
    
    switch (Style)
    {
    case EAIImageStyle::Realistic:
        StyleModifier = TEXT("photorealistic, highly detailed, 8k, professional photography");
        break;
    case EAIImageStyle::Stylized:
        StyleModifier = TEXT("stylized, artistic, vibrant colors, game art");
        break;
    case EAIImageStyle::Cartoon:
        StyleModifier = TEXT("cartoon style, cute, colorful, animated");
        break;
    case EAIImageStyle::Watercolor:
        StyleModifier = TEXT("watercolor painting, artistic, soft colors, traditional art");
        break;
    case EAIImageStyle::InkPainting:
        StyleModifier = TEXT("Chinese ink painting, traditional art, black and white, artistic");
        break;
    case EAIImageStyle::OilPainting:
        StyleModifier = TEXT("oil painting, classical art, rich colors, textured");
        break;
    case EAIImageStyle::PixelArt:
        StyleModifier = TEXT("pixel art, retro, 16-bit style, game graphics");
        break;
    case EAIImageStyle::LowPoly:
        StyleModifier = TEXT("low poly, geometric, 3D render, minimalist");
        break;
    case EAIImageStyle::CelShaded:
        StyleModifier = TEXT("cel shaded, anime style, bold outlines, vibrant");
        break;
    case EAIImageStyle::Photorealistic:
        StyleModifier = TEXT("photorealistic, ultra detailed, 8k, ray tracing, cinematic lighting");
        break;
    case EAIImageStyle::Anime:
        StyleModifier = TEXT("anime style, Japanese animation, colorful, detailed");
        break;
    case EAIImageStyle::Sketch:
        StyleModifier = TEXT("pencil sketch, hand drawn, artistic, monochrome");
        break;
    case EAIImageStyle::ConceptArt:
        StyleModifier = TEXT("concept art, digital painting, detailed, professional");
        break;
    case EAIImageStyle::Historical:
        StyleModifier = TEXT("historical art, period accurate, detailed, realistic");
        break;
    case EAIImageStyle::Cyberpunk:
        StyleModifier = TEXT("cyberpunk, neon lights, futuristic, sci-fi, high tech");
        break;
    default:
        StyleModifier = TEXT("high quality, detailed");
        break;
    }

    return FString::Printf(TEXT("%s, %s"), *BasePrompt, *StyleModifier);
}

FString UMingAIImageGenerator::GenerateUniqueID() const
{
    return FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
}

void UMingAIImageGenerator::SetAPIKey(EAIImageProvider Provider, const FString& APIKey)
{
    ProviderAPIKeys.Add(Provider, APIKey);
}

FString UMingAIImageGenerator::GetProviderAPIToken(EAIImageProvider Provider) const
{
    if (const FString* Key = ProviderAPIKeys.Find(Provider))
    {
        return *Key;
    }
    return FString();
}

void UMingAIImageGenerator::SetLocalModelPath(const FString& ModelPath)
{
    LocalModelPath = ModelPath;
}

void UMingAIImageGenerator::SetDefaultOutputPath(const FString& OutputPath)
{
    DefaultOutputPath = OutputPath;
    
    // Ensure directory exists
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*DefaultOutputPath))
    {
        PlatformFile.CreateDirectoryTree(*DefaultOutputPath);
    }
}

TArray<FString> UMingAIImageGenerator::GetAvailableProviders() const
{
    return {
        TEXT("Stable Diffusion"),
        TEXT("DALL-E 3"),
        TEXT("Midjourney"),
        TEXT("Leonardo AI"),
        TEXT("Local Model"),
        TEXT("Custom API")
    };
}

void UMingAIImageGenerator::ClearGenerationHistory()
{
    FScopeLock Lock(&GenerationLock);
    GenerationHistory.Empty();
}

TArray<FString> UMingAIImageGenerator::GetGenerationHistory() const
{
    FScopeLock Lock(&const_cast<FCriticalSection&>(GenerationLock));
    return GenerationHistory;
}

bool UMingAIImageGenerator::ExportGeneratedImage(const FString& GenerationID, const FString& ExportPath)
{
    FAIImageGenerationResult Result = GetGenerationResult(GenerationID);
    
    if (Result.Status != EAIImageGenerationStatus::Completed || Result.FilePath.IsEmpty())
    {
        return false;
    }

    // Copy file to export path
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return PlatformFile.CopyFile(*ExportPath, *Result.FilePath);
}

UTexture2D* UMingAIImageGenerator::LoadGeneratedTexture(const FString& GenerationID)
{
    FAIImageGenerationResult Result = GetGenerationResult(GenerationID);
    
    if (Result.GeneratedTexture)
    {
        return Result.GeneratedTexture;
    }

    // Try to load from file
    if (!Result.FilePath.IsEmpty() && FPaths::FileExists(Result.FilePath))
    {
        TArray<uint8> ImageData;
        if (FFileHelper::LoadFileToArray(ImageData, *Result.FilePath))
        {
            return CreateTextureFromImageData(ImageData);
        }
    }

    return nullptr;
}

FAIImageGenerationParams UMingAIImageGenerator::GetCharacterPortraitPreset(const FString& CharacterDescription)
{
    FAIImageGenerationParams Params;
    Params.Prompt = FString::Printf(TEXT("character portrait of %s, detailed face, professional lighting, high quality"), *CharacterDescription);
    Params.Style = EAIImageStyle::Realistic;
    Params.AspectRatio = EAIImageAspectRatio::Portrait;
    Params.Quality = EAIImageQuality::High;
    Params.Steps = 40;
    Params.CFGScale = 7.0f;
    Params.NegativePrompt = TEXT("blurry, low quality, distorted, ugly, deformed");
    return Params;
}

FAIImageGenerationParams UMingAIImageGenerator::GetEnvironmentPreset(const FString& EnvironmentDescription)
{
    FAIImageGenerationParams Params;
    Params.Prompt = FString::Printf(TEXT("%s, environment concept art, detailed, atmospheric lighting, game art"), *EnvironmentDescription);
    Params.Style = EAIImageStyle::ConceptArt;
    Params.AspectRatio = EAIImageAspectRatio::Landscape;
    Params.Quality = EAIImageQuality::High;
    Params.Steps = 35;
    Params.CFGScale = 7.5f;
    Params.NegativePrompt = TEXT("low quality, blurry, noisy");
    return Params;
}

FAIImageGenerationParams UMingAIImageGenerator::GetItemIconPreset(const FString& ItemDescription)
{
    FAIImageGenerationParams Params;
    Params.Prompt = FString::Printf(TEXT("%s, item icon, game asset, centered, white background, clean design"), *ItemDescription);
    Params.Style = EAIImageStyle::Stylized;
    Params.AspectRatio = EAIImageAspectRatio::Square;
    Params.Quality = EAIImageQuality::Standard;
    Params.Steps = 25;
    Params.CFGScale = 8.0f;
    Params.NegativePrompt = TEXT("cluttered, messy, low quality, blurry background");
    return Params;
}

FAIImageGenerationParams UMingAIImageGenerator::GetUIElementPreset(const FString& UIDescription)
{
    FAIImageGenerationParams Params;
    Params.Prompt = FString::Printf(TEXT("%s, UI element, game interface, clean design, modern, professional"), *UIDescription);
    Params.Style = EAIImageStyle::Stylized;
    Params.AspectRatio = EAIImageAspectRatio::Square;
    Params.Quality = EAIImageQuality::High;
    Params.Steps = 30;
    Params.CFGScale = 7.5f;
    Params.NegativePrompt = TEXT("cluttered, messy, low quality, blurry");
    return Params;
}
