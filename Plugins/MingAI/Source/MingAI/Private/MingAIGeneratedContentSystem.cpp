#include "MingAIGeneratedContentSystem.h"
#include "Engine/Engine.h"
#include "Http.h"
#include "Json.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UMingAIGeneratedContentSystem::UMingAIGeneratedContentSystem()
    : TotalGenerationsCount(0)
    , SuccessfulGenerationsCount(0)
    , FailedGenerationsCount(0)
    , bInitialized(false)
{
}

void UMingAIGeneratedContentSystem::InitializeSystem()
{
    if (bInitialized)
    {
        return;
    }

    // Setup default provider configurations
    FMingAIProviderConfig StableDiffusionConfig;
    StableDiffusionConfig.Provider = EMingAIProvider::StableDiffusion;
    StableDiffusionConfig.ApiEndpoint = TEXT("http://localhost:7860/sdapi/v1");
    StableDiffusionConfig.bEnabled = true;
    ProviderConfigs.Add(EMingAIProvider::StableDiffusion, StableDiffusionConfig);

    FMingAIProviderConfig OpenAIConfig;
    OpenAIConfig.Provider = EMingAIProvider::OpenAI;
    OpenAIConfig.ApiEndpoint = TEXT("https://api.openai.com/v1");
    OpenAIConfig.bEnabled = false;
    ProviderConfigs.Add(EMingAIProvider::OpenAI, OpenAIConfig);

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Content Generation System initialized"));
}

void UMingAIGeneratedContentSystem::ShutdownSystem()
{
    // Cancel all active generations
    for (const FGuid& RequestID : ActiveRequests)
    {
        CancelGeneration(RequestID);
    }

    PendingRequests.Empty();
    CompletedResults.Empty();
    ActiveRequests.Empty();
    ProviderConfigs.Empty();

    bInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("AI Content Generation System shutdown"));
}

void UMingAIGeneratedContentSystem::SetProviderConfig(EMingAIProvider Provider, const FMingAIProviderConfig& Config)
{
    ProviderConfigs.Add(Provider, Config);
}

FMingAIProviderConfig UMingAIGeneratedContentSystem::GetProviderConfig(EMingAIProvider Provider) const
{
    if (ProviderConfigs.Contains(Provider))
    {
        return ProviderConfigs[Provider];
    }
    return FMingAIProviderConfig();
}

FGuid UMingAIGeneratedContentSystem::GenerateContent(const FMingAIContentRequest& Request)
{
    if (!bInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Content Generation System not initialized"));
        return FGuid();
    }

    if (!ValidateRequest(Request))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid AI content generation request"));
        return FGuid();
    }

    FGuid RequestID = FGuid::NewGuid();
    PendingRequests.Add(RequestID, Request);
    ActiveRequests.Add(RequestID);

    TotalGenerationsCount++;

    // Start generation process
    ProcessNextInQueue();

    UE_LOG(LogTemp, Log, TEXT("Started AI content generation: %s"), *RequestID.ToString());
    return RequestID;
}

bool UMingAIGeneratedContentSystem::CancelGeneration(const FGuid& RequestID)
{
    if (!ActiveRequests.Contains(RequestID))
    {
        return false;
    }

    // Mark as cancelled
    FMingAIContentResult Result;
    Result.RequestID = RequestID;
    Result.Status = EMingAIGenerationStatus::Cancelled;
    CompletedResults.Add(RequestID, Result);

    ActiveRequests.Remove(RequestID);
    PendingRequests.Remove(RequestID);

    UE_LOG(LogTemp, Log, TEXT("Cancelled AI content generation: %s"), *RequestID.ToString());
    return true;
}

EMingAIGenerationStatus UMingAIGeneratedContentSystem::GetGenerationStatus(const FGuid& RequestID) const
{
    if (ActiveRequests.Contains(RequestID))
    {
        return EMingAIGenerationStatus::Generating;
    }

    if (CompletedResults.Contains(RequestID))
    {
        return CompletedResults[RequestID].Status;
    }

    return EMingAIGenerationStatus::Idle;
}

TArray<FGuid> UMingAIGeneratedContentSystem::GetActiveGenerations() const
{
    TArray<FGuid> Result;
    for (const FGuid& RequestID : ActiveRequests)
    {
        Result.Add(RequestID);
    }
    return Result;
}

FGuid UMingAIGeneratedContentSystem::GenerateImage(const FString& Prompt, EMingAIQualityLevel Quality, int32 Width, int32 Height)
{
    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Image;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = Quality;
    Request.Width = Width;
    Request.Height = Height;

    return GenerateContent(Request);
}

FGuid UMingAIGeneratedContentSystem::GenerateMusic(const FString& Prompt, float Duration, EMingAIQualityLevel Quality)
{
    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Music;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::AIVA;
    Request.QualityLevel = Quality;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));

    return GenerateContent(Request);
}

FGuid UMingAIGeneratedContentSystem::GenerateSoundEffect(const FString& Prompt, float Duration, EMingAIQualityLevel Quality)
{
    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::SoundEffect;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::Custom;
    Request.QualityLevel = Quality;
    Request.AdditionalParameters.Add(TEXT("Duration"), FString::Printf(TEXT("%.2f"), Duration));

    return GenerateContent(Request);
}

FGuid UMingAIGeneratedContentSystem::GenerateVoice(const FString& Text, const FString& VoiceID, EMingAIQualityLevel Quality)
{
    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Voice;
    Request.Prompt = Text;
    Request.Provider = EMingAIProvider::ElevenLabs;
    Request.QualityLevel = Quality;
    Request.AdditionalParameters.Add(TEXT("VoiceID"), VoiceID);

    return GenerateContent(Request);
}

FGuid UMingAIGeneratedContentSystem::GenerateTexture(const FString& Prompt, EMingAIQualityLevel Quality, int32 Width, int32 Height)
{
    FMingAIContentRequest Request;
    Request.ContentType = EMingAIContentType::Texture;
    Request.Prompt = Prompt;
    Request.Provider = EMingAIProvider::StableDiffusion;
    Request.QualityLevel = Quality;
    Request.Width = Width;
    Request.Height = Height;

    return GenerateContent(Request);
}

FMingAIContentResult UMingAIGeneratedContentSystem::GetGenerationResult(const FGuid& RequestID) const
{
    if (CompletedResults.Contains(RequestID))
    {
        return CompletedResults[RequestID];
    }
    return FMingAIContentResult();
}

void UMingAIGeneratedContentSystem::ClearCompletedGenerations()
{
    CompletedResults.Empty();
}

float UMingAIGeneratedContentSystem::GetAverageGenerationTime() const
{
    if (GenerationTimes.Num() == 0)
    {
        return 0.0f;
    }

    float Total = 0.0f;
    for (float Time : GenerationTimes)
    {
        Total += Time;
    }

    return Total / GenerationTimes.Num();
}

void UMingAIGeneratedContentSystem::ProcessNextInQueue()
{
    for (const auto& Pair : PendingRequests)
    {
        const FGuid& RequestID = Pair.Key;
        const FMingAIContentRequest& Request = Pair.Value;

        if (ActiveRequests.Contains(RequestID))
        {
            SendHttpRequest(RequestID, Request);
        }
    }
}

void UMingAIGeneratedContentSystem::SendHttpRequest(const FGuid& RequestID, const FMingAIContentRequest& Request)
{
    FMingAIProviderConfig Config = GetProviderConfig(Request.Provider);
    if (!Config.bEnabled)
    {
        ProcessGenerationResult(RequestID, false, TEXT(""), TEXT("Provider not enabled"));
        return;
    }

    FString Endpoint = GetProviderEndpoint(Request.Provider);
    FString Payload = BuildRequestPayload(Request);

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(Endpoint);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetContentAsString(Payload);

    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UMingAIGeneratedContentSystem::HandleHttpResponse, RequestID);

    HttpRequest->ProcessRequest();

    UpdateGenerationProgress(RequestID, 0.1f, TEXT("Request sent"));
}

void UMingAIGeneratedContentSystem::HandleHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FGuid RequestID)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        ProcessGenerationResult(RequestID, false, TEXT(""), TEXT("HTTP request failed"));
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    FString ResponseContent = Response->GetContentAsString();

    if (ResponseCode == 200)
    {
        // Parse response and extract file data
        // This is a simplified implementation
        FString OutputPath = GetOutputDirectory(PendingRequests[RequestID].ContentType) / GenerateUniqueFileName(PendingRequests[RequestID].ContentType);
        
        // In real implementation, decode base64 image data and save to file
        ProcessGenerationResult(RequestID, true, OutputPath, TEXT(""));
    }
    else
    {
        ProcessGenerationResult(RequestID, false, TEXT(""), FString::Printf(TEXT("HTTP error: %d"), ResponseCode));
    }
}

void UMingAIGeneratedContentSystem::ProcessGenerationResult(FGuid RequestID, bool bSuccess, const FString& FilePath, const FString& Error)
{
    FMingAIContentResult Result;
    Result.RequestID = RequestID;
    Result.FilePath = FilePath;
    Result.ErrorMessage = Error;

    if (PendingRequests.Contains(RequestID))
    {
        Result.ContentType = PendingRequests[RequestID].ContentType;
    }

    if (bSuccess)
    {
        Result.Status = EMingAIGenerationStatus::Completed;
        Result.GenerationTime = FPlatformTime::Seconds();
        Result.GenerationTimeStamp = FDateTime::Now();
        SuccessfulGenerationsCount++;
        OnGenerationCompleted.Broadcast(RequestID, Result);
    }
    else
    {
        Result.Status = EMingAIGenerationStatus::Failed;
        FailedGenerationsCount++;
        OnGenerationFailed.Broadcast(RequestID, Error);
    }

    CompletedResults.Add(RequestID, Result);
    ActiveRequests.Remove(RequestID);
    PendingRequests.Remove(RequestID);
}

void UMingAIGeneratedContentSystem::UpdateGenerationProgress(FGuid RequestID, float Progress, const FString& Status)
{
    OnGenerationProgress.Broadcast(RequestID, Progress, Status);
}

FString UMingAIGeneratedContentSystem::GetProviderEndpoint(EMingAIProvider Provider) const
{
    if (ProviderConfigs.Contains(Provider))
    {
        FMingAIProviderConfig Config = ProviderConfigs[Provider];
        
        switch (Provider)
        {
        case EMingAIProvider::StableDiffusion:
            return Config.ApiEndpoint + TEXT("/txt2img");
        case EMingAIProvider::OpenAI:
            return Config.ApiEndpoint + TEXT("/images/generations");
        case EMingAIProvider::AIVA:
            return Config.ApiEndpoint + TEXT("/compose");
        case EMingAIProvider::ElevenLabs:
            return Config.ApiEndpoint + TEXT("/text-to-speech");
        default:
            return Config.ApiEndpoint;
        }
    }
    return FString();
}

FString UMingAIGeneratedContentSystem::BuildRequestPayload(const FMingAIContentRequest& Request) const
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetStringField(TEXT("prompt"), Request.Prompt);
    
    if (!Request.NegativePrompt.IsEmpty())
    {
        JsonObject->SetStringField(TEXT("negative_prompt"), Request.NegativePrompt);
    }

    if (Request.ContentType == EMingAIContentType::Image || Request.ContentType == EMingAIContentType::Texture)
    {
        JsonObject->SetNumberField(TEXT("width"), Request.Width);
        JsonObject->SetNumberField(TEXT("height"), Request.Height);
        JsonObject->SetNumberField(TEXT("steps"), Request.Steps);
        JsonObject->SetNumberField(TEXT("cfg_scale"), Request.GuidanceScale);
        
        if (Request.Seed >= 0)
        {
            JsonObject->SetNumberField(TEXT("seed"), Request.Seed);
        }
    }

    // Add additional parameters
    for (const auto& Pair : Request.AdditionalParameters)
    {
        JsonObject->SetStringField(Pair.Key, Pair.Value);
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return OutputString;
}

bool UMingAIGeneratedContentSystem::ValidateRequest(const FMingAIContentRequest& Request) const
{
    if (Request.ContentType == EMingAIContentType::None)
    {
        return false;
    }

    if (Request.Prompt.IsEmpty())
    {
        return false;
    }

    if (Request.Provider == EMingAIProvider::None)
    {
        return false;
    }

    return true;
}

FString UMingAIGeneratedContentSystem::GetOutputDirectory(EMingAIContentType ContentType) const
{
    FString BaseDir = FPaths::ProjectSavedDir() / TEXT("AIContent");
    
    switch (ContentType)
    {
    case EMingAIContentType::Image:
        return BaseDir / TEXT("Images");
    case EMingAIContentType::Music:
        return BaseDir / TEXT("Music");
    case EMingAIContentType::SoundEffect:
        return BaseDir / TEXT("SoundEffects");
    case EMingAIContentType::Texture:
        return BaseDir / TEXT("Textures");
    case EMingAIContentType::Voice:
        return BaseDir / TEXT("Voice");
    default:
        return BaseDir / TEXT("Other");
    }
}

FString UMingAIGeneratedContentSystem::GenerateUniqueFileName(EMingAIContentType ContentType) const
{
    FDateTime Now = FDateTime::Now();
    FString TimeStamp = Now.ToString(TEXT("%Y%m%d_%H%M%S"));
    FGuid Guid = FGuid::NewGuid();

    FString Extension = TEXT("png");
    switch (ContentType)
    {
    case EMingAIContentType::Image:
    case EMingAIContentType::Texture:
        Extension = TEXT("png");
        break;
    case EMingAIContentType::Music:
    case EMingAIContentType::SoundEffect:
        Extension = TEXT("wav");
        break;
    case EMingAIContentType::Voice:
        Extension = TEXT("mp3");
        break;
    default:
        Extension = TEXT("bin");
    }

    return FString::Printf(TEXT("%s_%s.%s"), *TimeStamp, *Guid.ToString(EGuidFormats::DigitsWithHyphensLower), *Extension);
}
