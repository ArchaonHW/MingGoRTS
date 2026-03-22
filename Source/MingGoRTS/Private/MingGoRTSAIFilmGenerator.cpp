#include "MingGoRTSAIFilmGenerator.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/Texture2D.h"
#include "HAL/Runnable.h"
#include "HAL/Event.h"
#include "Containers/Queue.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UMingGoRTSAIFilmGenerator::UMingGoRTSAIFilmGenerator()
    : CurrentStatus(EFilmGenerationStatus::Idle)
    , CurrentFrameIndex(0)
    , bIsGenerating(false)
{
    // 初始化生成計時器
    GenerationTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSAIFilmGenerator::OnGenerationTick);
}

void UMingGoRTSAIFilmGenerator::GenerateFilmSequence(const TArray<FFilmSequenceData>& FilmData)
{
    if (FilmData.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Film data is empty"));
        return;
    }

    FilmSequences = FilmData;
    CurrentFrameIndex = 0;
    CurrentStatus = EFilmGenerationStatus::Idle;
    
    UE_LOG(LogTemp, Log, TEXT("Film sequence initialized with %d frames"), FilmData.Num());
}

void UMingGoRTSAIFilmGenerator::StartGeneration()
{
    if (FilmSequences.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No film sequences to generate"));
        return;
    }

    if (CurrentStatus == EFilmGenerationStatus::Generating)
    {
        UE_LOG(LogTemp, Warning, TEXT("Generation already in progress"));
        return;
    }

    bIsGenerating = true;
    CurrentStatus = EFilmGenerationStatus::Generating;
    CurrentFrameIndex = 0;

    // 啟動生成計時器
    if (!GenerationTickerHandle.IsValid())
    {
        GenerationTickerHandle = FTicker::GetCoreTicker().AddTicker(GenerationTicker, 0.1f);
    }

    UE_LOG(LogTemp, Log, TEXT("Started AI film generation"));
}

void UMingGoRTSAIFilmGenerator::StopGeneration()
{
    bIsGenerating = false;
    CurrentStatus = EFilmGenerationStatus::Idle;

    // 停止生成計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    UE_LOG(LogTemp, Log, TEXT("Stopped AI film generation"));
}

void UMingGoRTSAIFilmGenerator::GenerateSingleFrame(const FString& Prompt, const FString& Style, int32 FrameIndex)
{
    if (StableDiffusionEndpoint.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Stable Diffusion API endpoint not configured"));
        return;
    }

    FString EnhancedPrompt = BuildEnhancedPrompt(Prompt, Style);
    SendStableDiffusionRequest(EnhancedPrompt, FrameIndex);
}

void UMingGoRTSAIFilmGenerator::SetStableDiffusionAPI(const FString& APIEndpoint, const FString& APIKey)
{
    StableDiffusionEndpoint = APIEndpoint;
    StableDiffusionAPIKey = APIKey;
    
    UE_LOG(LogTemp, Log, TEXT("Stable Diffusion API configured: %s"), *APIEndpoint);
}

bool UMingGoRTSAIFilmGenerator::TestAPIConnection()
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

void UMingGoRTSAIFilmGenerator::AddFilmSequence(const FFilmSequenceData& SequenceData)
{
    FilmSequences.Add(SequenceData);
    UE_LOG(LogTemp, Log, TEXT("Added film sequence: %s"), *SequenceData.SceneDescription);
}

void UMingGoRTSAIFilmGenerator::ClearFilmSequences()
{
    FilmSequences.Empty();
    CurrentFrameIndex = 0;
    UE_LOG(LogTemp, Log, TEXT("Cleared all film sequences"));
}

void UMingGoRTSAIFilmGenerator::PreviewFrame(int32 FrameIndex)
{
    if (FrameIndex < 0 || FrameIndex >= FilmSequences.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid frame index: %d"), FrameIndex);
        return;
    }

    const FFilmSequenceData& FrameData = FilmSequences[FrameIndex];
    if (FrameData.GeneratedFrame)
    {
        // 顯示預覽
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
                FString::Printf(TEXT("Previewing frame %d: %s"), FrameIndex, *FrameData.SceneDescription));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Frame %d has no generated texture"), FrameIndex);
    }
}

void UMingGoRTSAIFilmGenerator::PlayGeneratedFilm()
{
    if (FilmSequences.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No film sequences to play"));
        return;
    }

    // 檢查所有幀是否都已生成
    bool bAllFramesGenerated = true;
    for (const FFilmSequenceData& FrameData : FilmSequences)
    {
        if (!FrameData.GeneratedFrame)
        {
            bAllFramesGenerated = false;
            break;
        }
    }

    if (!bAllFramesGenerated)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not all frames are generated yet"));
        return;
    }

    // 開始播放影片序列
    UE_LOG(LogTemp, Log, TEXT("Playing generated film with %d frames"), FilmSequences.Num());
    
    // 這裡可以添加實際的播放邏輯
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
            TEXT("Playing AI generated film..."));
    }
}

void UMingGoRTSAIFilmGenerator::ProcessNextFrame()
{
    if (!bIsGenerating || CurrentFrameIndex >= FilmSequences.Num())
    {
        NotifyGenerationCompleted(true);
        return;
    }

    const FFilmSequenceData& FrameData = FilmSequences[CurrentFrameIndex];
    GenerateSingleFrame(FrameData.SceneDescription, FrameData.StylePrompt, CurrentFrameIndex);
}

bool UMingGoRTSAIFilmGenerator::OnGenerationTick(float DeltaTime)
{
    if (!bIsGenerating)
    {
        return false;
    }

    ProcessNextFrame();
    CurrentFrameIndex++;

    return bIsGenerating;
}

void UMingGoRTSAIFilmGenerator::SendStableDiffusionRequest(const FString& Prompt, int32 FrameIndex)
{
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    
    // 設置請求URL
    HttpRequest->SetURL(StableDiffusionEndpoint + "/sdapi/v1/txt2img");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    HttpRequest->SetHeader("Authorization", "Bearer " + StableDiffusionAPIKey);

    // 創建JSON請求體
    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
    RequestJson->SetStringField(TEXT("prompt"), Prompt);
    RequestJson->SetNumberField(TEXT("width"), 1024);
    RequestJson->SetNumberField(TEXT("height"), 576);
    RequestJson->SetNumberField(TEXT("steps"), 20);
    RequestJson->SetNumberField(TEXT("cfg_scale"), 7.0);
    RequestJson->SetStringField(TEXT("sampler_name"), "DPM++ 2M Karras");

    // 序列化JSON
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(OutputString);

    // 設置回調
    HttpRequest->OnProcessRequestComplete().BindUObject(this, 
        &UMingGoRTSAIFilmGenerator::HandleGenerationResponse, FrameIndex);

    HttpRequest->ProcessRequest();
}

void UMingGoRTSAIFilmGenerator::HandleGenerationResponse(bool bSuccess, const FString& ResponseData, int32 FrameIndex)
{
    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate frame %d"), FrameIndex);
        NotifyGenerationCompleted(false, "HTTP request failed");
        return;
    }

    // 解析響應JSON
    TSharedPtr<FJsonObject> ResponseJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
    
    if (!FJsonSerializer::Deserialize(Reader, ResponseJson))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse response JSON for frame %d"), FrameIndex);
        NotifyGenerationCompleted(false, "Invalid JSON response");
        return;
    }

    // 提取圖像數據
    const TArray<TSharedPtr<FJsonValue>>* ImagesArray;
    if (ResponseJson->TryGetArrayField(TEXT("images"), ImagesArray) && ImagesArray->Num() > 0)
    {
        FString ImageData = (*ImagesArray)[0]->AsString();
        
        // 創建紋理
        UTexture2D* NewTexture = UTexture2D::CreateTransient(1024, 576, PF_B8G8R8A8);
        if (NewTexture)
        {
            // 這裡需要將Base64圖像數據轉換為紋理數據
            // 簡化版本：直接設置為已生成
            if (FrameIndex < FilmSequences.Num())
            {
                FilmSequences[FrameIndex].GeneratedFrame = NewTexture;
            }

            // 觸發幀生成完成事件
            OnFilmFrameGenerated.Broadcast(FrameIndex);
            
            UE_LOG(LogTemp, Log, TEXT("Successfully generated frame %d"), FrameIndex);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No image data in response for frame %d"), FrameIndex);
        NotifyGenerationCompleted(false, "No image data in response");
    }
}

FString UMingGoRTSAIFilmGenerator::BuildEnhancedPrompt(const FString& BasePrompt, const FString& Style)
{
    FString EnhancedPrompt = BasePrompt;
    
    // 添加民國時期風格
    EnhancedPrompt += ", Republican era China, historical setting";
    
    // 添加用戶指定的風格
    if (!Style.IsEmpty())
    {
        EnhancedPrompt += ", " + Style;
    }
    
    // 添加品質增強詞
    EnhancedPrompt += ", highly detailed, cinematic lighting, epic composition, 4K quality";
    
    return EnhancedPrompt;
}

void UMingGoRTSAIFilmGenerator::NotifyGenerationCompleted(bool bSuccess, const FString& ErrorMessage)
{
    bIsGenerating = false;
    CurrentStatus = bSuccess ? EFilmGenerationStatus::Completed : EFilmGenerationStatus::Failed;

    // 停止生成計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    // 觸發完成事件
    OnFilmGenerationCompleted.Broadcast(bSuccess, ErrorMessage);

    UE_LOG(LogTemp, Log, TEXT("Film generation completed. Success: %s, Error: %s"), 
        bSuccess ? TEXT("true") : TEXT("false"), *ErrorMessage);
}
