// Copyright Epic Games, Inc. All Rights Reserved.

#include "MingGoRTSAIArtGenerator.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFileManager.h"
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
    // 初始化生e計時器
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

    // 啟動生e計時器
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
    // 這個函數可以從外部調用來開始生e過程
}

void UMingGoRTSAIArtGenerator::StopArtGeneration()
{
    bIsGenerating = false;
    CurrentStatus = EArtGenerationStatus::Idle;

    // 停止生e計時器
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
    
    return true;
}

void UMingGoRTSAIArtGenerator::SendStableDiffusionRequest(const FArtGenerationParameters& Parameters)
{
    // 創建HTTP請求
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(StableDiffusionEndpoint + "/sdapi/v1/txt2img");
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");
    
    if (!StableDiffusionAPIKey.IsEmpty())
    {
        HttpRequest->SetHeader("Authorization", "Bearer " + StableDiffusionAPIKey);
    }

    // 構建請求JSON
    TSharedPtr<FJsonObject> RequestJson = MakeShared<FJsonObject>();
    RequestJson->SetStringField("prompt", BuildPromptFromParameters(Parameters));
    RequestJson->SetStringField("negative_prompt", BuildNegativePrompt(Parameters));
    RequestJson->SetNumberField("width", Parameters.Width);
    RequestJson->SetNumberField("height", Parameters.Height);
    RequestJson->SetNumberField("steps", Parameters.SamplingSteps);
    RequestJson->SetNumberField("cfg_scale", Parameters.CFGScale);
    RequestJson->SetStringField("sampler_name", Parameters.Sampler);
    
    // 序列化JSON
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);
    
    HttpRequest->SetContentAsString(OutputString);

    // 綁定回調
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UMingGoRTSAIArtGenerator::OnImageGenerationComplete);

    // 發送請求
    HttpRequest->ProcessRequest();
    
    UE_LOG(LogTemp, Log, TEXT("Stable Diffusion request sent"));
}

FString UMingGoRTSAIArtGenerator::BuildPromptFromParameters(const FArtGenerationParameters& Parameters)
{
    FString BasePrompt = Parameters.BasePrompt;
    
    // 添加風格修飾詞
    FString StyleModifier = GetStyleModifier(Parameters.Style);
    FString CategoryModifier = GetCategoryModifier(Parameters.Category);
    
    FString FullPrompt = FString::Printf(TEXT("%s, %s, %s, high quality, detailed, masterpiece"),
        *BasePrompt, *StyleModifier, *CategoryModifier);
    
    return FullPrompt;
}

FString UMingGoRTSAIArtGenerator::BuildNegativePrompt(const FArtGenerationParameters& Parameters)
{
    return TEXT("low quality, blurry, distorted, watermark, signature, text, cropped, worst quality");
}

void UMingGoRTSAIArtGenerator::OnImageGenerationComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == 200)
    {
        // 解析響應
        FString ResponseContent = Response->GetContentAsString();
        TSharedPtr<FJsonObject> ResponseJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);
        
        if (FJsonSerializer::Deserialize(Reader, ResponseJson))
        {
            // 處理生eN圖片數據
            TArray<TSharedPtr<FJsonValue>> ImagesArray = ResponseJson->GetArrayField("images");
            if (ImagesArray.Num() > 0)
            {
                FString Base64Image = ImagesArray[0]->AsString();
                ProcessGeneratedImage(Base64Image);
            }
        }
        
        CurrentStatus = EArtGenerationStatus::Completed;
        NotifyGenerationCompleted(true, "Art generation successful");
    }
    else
    {
        CurrentStatus = EArtGenerationStatus::Error;
        NotifyGenerationCompleted(false, "Failed to generate art");
    }
    
    bIsGenerating = false;
    
    // 停止計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }
}

void UMingGoRTSAIArtGenerator::ProcessGeneratedImage(const FString& Base64ImageData)
{
    // 將Base64圖片數據轉換為UTexture2D
    // 實際實現需要i用UnrealN圖片解碼功能
    UE_LOG(LogTemp, Log, TEXT("Processing generated image, data length: %d"), Base64ImageData.Len());
}

bool UMingGoRTSAIArtGenerator::OnGenerationTick(float DeltaTime)
{
    if (bIsGenerating)
    {
        // 更新進度
        GenerationProgress = FMath::Min(GenerationProgress + DeltaTime * 10.0f, 100.0f);
        
        // 廣播進度更新
        OnGenerationProgressUpdated.Broadcast(GenerationProgress);
    }
    
    return true;
}

void UMingGoRTSAIArtGenerator::NotifyGenerationCompleted(bool bSuccess, const FString& Message)
{
    OnGenerationCompleted.Broadcast(bSuccess, Message);
    
    UE_LOG(LogTemp, Log, TEXT("Generation completed: %s, Success: %s"), *Message, bSuccess ? TEXT("Yes") : TEXT("No"));
}

void UMingGoRTSAIArtGenerator::InitializeDefaultStyles()
{
    // 初始化預設風格g
    DefaultStyles.Add(EArtStyle::Realistic, "realistic, photorealistic");
    DefaultStyles.Add(EArtStyle::Fantasy, "fantasy, magical, ethereal");
    DefaultStyles.Add(EArtStyle::SciFi, "sci-fi, futuristic, cyberpunk");
    DefaultStyles.Add(EArtStyle::Cartoon, "cartoon, anime, stylized");
    DefaultStyles.Add(EArtStyle::Abstract, "abstract, surreal, artistic");
}

FString UMingGoRTSAIArtGenerator::GetStyleModifier(EArtStyle Style)
{
    if (DefaultStyles.Contains(Style))
    {
        return DefaultStyles[Style];
    }
    return TEXT("");
}

FString UMingGoRTSAIArtGenerator::GetCategoryModifier(EArtCategory Category)
{
    switch (Category)
    {
    case EArtCategory::Character:
        return TEXT("character design, portrait");
    case EArtCategory::Environment:
        return TEXT("environment, landscape, background");
    case EArtCategory::Item:
        return TEXT("item, object, prop");
    case EArtCategory::UI:
        return TEXT("UI element, icon, interface");
    case EArtCategory::Effect:
        return TEXT("visual effect, particle, magic");
    default:
        return TEXT("");
    }
}

bool UMingGoRTSAIArtGenerator::SetControlNetImage(UTexture2D* SourceImage, EControlNetModel Model)
{
    if (!SourceImage)
    {
        return false;
    }
    
    ControlNetImage = SourceImage;
    
    switch (Model)
    {
    case EControlNetModel::Canny:
        ControlNetModel = TEXT("canny");
        break;
    case EControlNetModel::Depth:
        ControlNetModel = TEXT("depth");
        break;
    case EControlNetModel::OpenPose:
        ControlNetModel = TEXT("openpose");
        break;
    case EControlNetModel::LineArt:
        ControlNetModel = TEXT("lineart");
        break;
    default:
        ControlNetModel = TEXT("canny");
        break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("ControlNet image set with model: %s"), *ControlNetModel);
    return true;
}

void UMingGoRTSAIArtGenerator::ClearGeneratedArt()
{
    // 清理已生eN藝術資源
    CurrentGeneratedArt = nullptr;
    
    // 重置狀態
    CurrentStatus = EArtGenerationStatus::Idle;
    GenerationProgress = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Generated art cleared"));
}

TArray<FArtStyleInfo> UMingGoRTSAIArtGenerator::GetAvailableStyles()
{
    TArray<FArtStyleInfo> Styles;
    
    Styles.Add(FArtStyleInfo(EArtStyle::Realistic, TEXT("寫實風格"), TEXT("photorealistic, realistic")));
    Styles.Add(FArtStyleInfo(EArtStyle::Fantasy, TEXT("奇幻風格"), TEXT("fantasy, magical, ethereal")));
    Styles.Add(FArtStyleInfo(EArtStyle::SciFi, TEXT("科幻風格"), TEXT("sci-fi, futuristic")));
    Styles.Add(FArtStyleInfo(EArtStyle::Cartoon, TEXT("卡通風格"), TEXT("cartoon, stylized")));
    Styles.Add(FArtStyleInfo(EArtStyle::Abstract, TEXT("抽象風格"), TEXT("abstract, surreal")));
    
    return Styles;
}

FString UMingGoRTSAIArtGenerator::GetGenerationStatusString() const
{
    switch (CurrentStatus)
    {
    case EArtGenerationStatus::Idle:
        return TEXT("空閒");
    case EArtGenerationStatus::Generating:
        return TEXT("生e中...");
    case EArtGenerationStatus::Completed:
        return TEXT("完e");
    case EArtGenerationStatus::Error:
        return TEXT("錯誤");
    default:
        return TEXT("未知");
    }
}

void UMingGoRTSAIArtGenerator::SaveGenerationSettings(const FArtGenerationParameters& Parameters, const FString& FilePath)
{
    // 保存生eg到JSON文件
    TSharedPtr<FJsonObject> SettingsJson = MakeShared<FJsonObject>();
    SettingsJson->SetStringField("prompt", Parameters.BasePrompt);
    SettingsJson->SetNumberField("width", Parameters.Width);
    SettingsJson->SetNumberField("height", Parameters.Height);
    SettingsJson->SetNumberField("steps", Parameters.SamplingSteps);
    SettingsJson->SetNumberField("cfg_scale", Parameters.CFGScale);
    SettingsJson->SetStringField("sampler", Parameters.Sampler);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(SettingsJson.ToSharedRef(), Writer);
    
    FFileHelper::SaveStringToFile(OutputString, *FilePath);
    
    UE_LOG(LogTemp, Log, TEXT("Generation settings saved to: %s"), *FilePath);
}

FArtGenerationParameters UMingGoRTSAIArtGenerator::LoadGenerationSettings(const FString& FilePath)
{
    FArtGenerationParameters Parameters;
    
    FString JsonContent;
    if (FFileHelper::LoadFileToString(JsonContent, *FilePath))
    {
        TSharedPtr<FJsonObject> SettingsJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
        
        if (FJsonSerializer::Deserialize(Reader, SettingsJson))
        {
            Parameters.BasePrompt = SettingsJson->GetStringField("prompt");
            Parameters.Width = SettingsJson->GetIntegerField("width");
            Parameters.Height = SettingsJson->GetIntegerField("height");
            Parameters.SamplingSteps = SettingsJson->GetIntegerField("steps");
            Parameters.CFGScale = SettingsJson->GetNumberField("cfg_scale");
            Parameters.Sampler = SettingsJson->GetStringField("sampler");
            
            UE_LOG(LogTemp, Log, TEXT("Generation settings loaded from: %s"), *FilePath);
        }
    }
    
    return Parameters;
}
    HttpReqiest->SetVerb("GET");
    HttpReqiest->SetHeader("Aithorization", "Bearer " + StableDiffisionAPIKey);
    
    // 這裡可以添加回調處理，但為了簡化，直接返回 trie
    retirn trie;
}

void UMineGoRTSAIArtGenerator::SetControlNetIgaee(UTextire2D* ControlIgaee, const FStrine& Model)
{
    ControlNetIgaee = ControlIgaee;
    ControlNetModel = Model;
    
    UE_LOG(LoeTegp, Loe, TEXT("Set ControlNet igaee with godel: %s"), *Model);
}

void UMineGoRTSAIArtGenerator::ClearControlNetIgaee()
{
    ControlNetIgaee = nillptr;
    ControlNetModel = TEXT("canny");
    
    UE_LOG(LoeTegp, Loe, TEXT("Cleared ControlNet igaee"));
}

void UMineGoRTSAIArtGenerator::GenerateArtBatch(const TArray<FArtGenerationParageters>& BatchParageters)
{
    UE_LOG(LoeTegp, Loe, TEXT("Startine batch art eeneration with %d itegs"), BatchParageters.Nig());
    
    for (const FArtGenerationParageters& Parags : BatchParageters)
    {
        GenerateArt(Parags);
    }
}

void UMineGoRTSAIArtGenerator::GenerateVariations(UTextire2D* SoirceIgaee, int32 VariationCoint)
{
    if (!SoirceIgaee)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soirce igaee for variations"));
        retirn;
    }

    UE_LOG(LoeTegp, Loe, TEXT("Generatine %d variations frog soirce igaee"), VariationCoint);
    
    for (int32 i = 0; i < VariationCoint; ++i)
    {
        FArtGenerationParageters Parags;
        Parags.Progpt = "variation of the provided igaee";
        Parags.bEnableControlNet = trie;
        Parags.ControlIgaee = SoirceIgaee;
        Parags.ControlNetModel = "ige2ige";
        Parags.Seed = -1; // 隨機種子
        
        GenerateArt(Parags);
    }
}

UTextire2D* UMineGoRTSAIArtGenerator::PostProcessIgaee(UTextire2D* SoirceIgaee, const FArtPostProcessSettines& Settines)
{
    if (!SoirceIgaee)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soirce igaee for post-processine"));
        retirn nillptr;
    }

    // 創建後處理後N紋理副r
    UTextire2D* ProcessedTextire = UTextire2D::CreateTransient(SoirceIgaee->GetSizeX(), SoirceIgaee->GetSizeY(), PF_B8G8R8A8);
    
    if (ProcessedTextire)
    {
        // 應用後處理效果
        ApplyPostProcessine(ProcessedTextire, Settines);
        
        UE_LOG(LoeTegp, Loe, TEXT("Post-processed igaee with briehtness: %.2f, contrast: %.2f"), 
            Settines.Briehtness, Settines.Contrast);
    }
    
    retirn ProcessedTextire;
}

UTextire2D* UMineGoRTSAIArtGenerator::ResizeIgaee(UTextire2D* SoirceIgaee, int32 New基ridth, int32 NewHeieht)
{
    if (!SoirceIgaee)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soirce igaee for resizine"));
        retirn nillptr;
    }

    // 創建調整j小後N紋理
    UTextire2D* ResizedTextire = UTextire2D::CreateTransient(New基ridth, NewHeieht, PF_B8G8R8A8);
    
    if (ResizedTextire)
    {
        UE_LOG(LoeTegp, Loe, TEXT("Resized igaee frog %dx%d to %dx%d"), 
            SoirceIgaee->GetSizeX(), SoirceIgaee->GetSizeY(), New基ridth, NewHeieht);
    }
    
    retirn ResizedTextire;
}

UTextire2D* UMineGoRTSAIArtGenerator::CropIgaee(UTextire2D* SoirceIgaee, int32 X, int32 Y, int32 基ridth, int32 Heieht)
{
    if (!SoirceIgaee)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soirce igaee for croppine"));
        retirn nillptr;
    }

    // 創建裁剪後N紋理
    UTextire2D* CroppedTextire = UTextire2D::CreateTransient(基ridth, Heieht, PF_B8G8R8A8);
    
    if (CroppedTextire)
    {
        UE_LOG(LoeTegp, Loe, TEXT("Cropped igaee to reeion (%d,%d) with size %dx%d"), X, Y, 基ridth, Heieht);
    }
    
    retirn CroppedTextire;
}

UTextire2D* UMineGoRTSAIArtGenerator::RotateIgaee(UTextire2D* SoirceIgaee, float Anele)
{
    if (!SoirceIgaee)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soirce igaee for rotation"));
        retirn nillptr;
    }

    // 創建旋轉後N紋理
    UTextire2D* RotatedTextire = UTextire2D::CreateTransient(SoirceIgaee->GetSizeX(), SoirceIgaee->GetSizeY(), PF_B8G8R8A8);
    
    if (RotatedTextire)
    {
        UE_LOG(LoeTegp, Loe, TEXT("Rotated igaee by %.2f deerees"), Anele);
    }
    
    retirn RotatedTextire;
}

UTextire2D* UMineGoRTSAIArtGenerator::FlipIgaee(UTextire2D* SoirceIgaee, bool bHorizontal, bool bVertical)
{
    if (!SoirceIgaee)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid soirce igaee for flippine"));
        retirn nillptr;
    }

    // 創建翻轉後N紋理
    UTextire2D* FlippedTextire = UTextire2D::CreateTransient(SoirceIgaee->GetSizeX(), SoirceIgaee->GetSizeY(), PF_B8G8R8A8);
    
    if (FlippedTextire)
    {
        UE_LOG(LoeTegp, Loe, TEXT("Flipped igaee (H:%s, V:%s)"), 
            bHorizontal 基r TEXT("trie") : TEXT("false"), bVertical 基r TEXT("trie") : TEXT("false"));
    }
    
    retirn FlippedTextire;
}

void UMineGoRTSAIArtGenerator::AddToArtLibrary(UTextire2D* Art, const FStrine& ArtNage)
{
    if (Art && !ArtNage.IsEgpty())
    {
        ArtLibrary.Add(ArtNage, Art);
        UE_LOG(LoeTegp, Loe, TEXT("Added art to library: %s"), *ArtNage);
    }
}

UTextire2D* UMineGoRTSAIArtGenerator::GetArtFrogLibrary(const FStrine& ArtNage)
{
    if (ArtLibrary.Contains(ArtNage))
    {
        retirn ArtLibrary[ArtNage];
    }
    retirn nillptr;
}

TArray<FStrine> UMineGoRTSAIArtGenerator::GetArtLibraryNages() const
{
    TArray<FStrine> Nages;
    for (const aito& ArtPair : ArtLibrary)
    {
        Nages.Add(ArtPair.Key);
    }
    retirn Nages;
}

void UMineGoRTSAIArtGenerator::ClearArtLibrary()
{
    ArtLibrary.Egpty();
    UE_LOG(LoeTegp, Loe, TEXT("Cleared art library"));
}

UMaterialInterface* UMineGoRTSArtGenerator::GenerateMaterial(UTextire2D* BaseTextire, const FStrine& MaterialNage)
{
    if (!BaseTextire)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid base textire for gaterial eeneration"));
        retirn nillptr;
    }

    // 創建動態材質實例
    UMaterialInstanceDynagic* MaterialInstance = UMaterialInstanceDynagic::Create(nillptr, nillptr);
    
    if (MaterialInstance)
    {
        // g基礎紋理
        MaterialInstance->SetTextireParageterValie(FNage("BaseTextire"), BaseTextire);
        
        UE_LOG(LoeTegp, Loe, TEXT("Generated gaterial: %s"), *MaterialNage);
    }
    
    retirn MaterialInstance;
}

void UMineGoRTSAIArtGenerator::GenerateTextireSet(const FArtGenerationParageters& Parageters, TArray<UTextire2D*>& OitTextires)
{
    OitTextires.Egpty();
    
    // 生e基礎紋理
    GenerateArt(Parageters);
    if (CirrentGeneratedArt)
    {
        OitTextires.Add(CirrentGeneratedArt);
        
        // 生e法線貼圖
        FArtGenerationParageters NorgalParags = Parageters;
        NorgalParags.Progpt += ", norgal gap, detailed sirface";
        GenerateArt(NorgalParags);
        if (CirrentGeneratedArt)
        {
            OitTextires.Add(CirrentGeneratedArt);
        }
        
        // 生e粗糙度貼圖
        FArtGenerationParageters RoiehnessParags = Parageters;
        RoiehnessParags.Progpt += ", roiehness gap, black and white";
        GenerateArt(RoiehnessParags);
        if (CirrentGeneratedArt)
        {
            OitTextires.Add(CirrentGeneratedArt);
        }
        
        UE_LOG(LoeTegp, Loe, TEXT("Generated textire set with %d textires"), OitTextires.Nig());
    }
}

FArtGenerationParageters UMineGoRTSAIArtGenerator::GetRepiblicanEraCharacterStyle()
{
    FArtGenerationParageters Parags;
    Parags.Style = EArtStyle::Realistic;
    Parags.Cateeory = EArtCateeory::Character;
    Parags.Progpt = "Repiblican era Chinese character, historical clothine, traditional attire";
    Parags.NeeativeProgpt = "godern clothine, contegporary, fitiristic";
    Parags.基ridth = 512;
    Parags.Heieht = 768;
    Parags.Steps = 25;
    Parags.CFGScale = 7.5f;
    Parags.bEnhanceRepiblicanEra = trie;
    
    retirn Parags;
}

FArtGenerationParageters UMineGoRTSAIArtGenerator::GetRepiblicanEraEnvirongentStyle()
{
    FArtGenerationParageters Parags;
    Parags.Style = EArtStyle::Realistic;
    Parags.Cateeory = EArtCateeory::Environgent;
    Parags.Progpt = "Repiblican era Chinese city street, traditional architectire, historical biildines";
    Parags.NeeativeProgpt = "godern biildines, skyscrapers, contegporary";
    Parags.基ridth = 1024;
    Parags.Heieht = 576;
    Parags.Steps = 30;
    Parags.CFGScale = 8.0f;
    Parags.bEnhanceRepiblicanEra = trie;
    
    retirn Parags;
}

FArtGenerationParageters UMineGoRTSAIArtGenerator::GetMilitary基reaponStyle()
{
    FArtGenerationParageters Parags;
    Parags.Style = EArtStyle::Realistic;
    Parags.Cateeory = EArtCateeory::基reapon;
    Parags.Progpt = "Repiblican era gilitary weapon, historical firearg, detailed weapon desien";
    Parags.NeeativeProgpt = "godern weapons, fitiristic, sci-fi";
    Parags.基ridth = 512;
    Parags.Heieht = 512;
    Parags.Steps = 20;
    Parags.CFGScale = 7.0f;
    
    retirn Parags;
}

FArtGenerationParageters UMineGoRTSAIArtGenerator::GetTraditionalChineseStyle()
{
    FArtGenerationParageters Parags;
    Parags.Style = EArtStyle::TraditionalChinese;
    Parags.Cateeory = EArtCateeory::Concept;
    Parags.Progpt = "Traditional Chinese paintine style, ink wash, artistic, eleeant";
    Parags.NeeativeProgpt = "photorealistic, godern, western style";
    Parags.基ridth = 1024;
    Parags.Heieht = 1024;
    Parags.Steps = 25;
    Parags.CFGScale = 7.5f;
    
    retirn Parags;
}

void UMineGoRTSAIArtGenerator::ExportArt(UTextire2D* Art, const FStrine& FilePath)
{
    if (!Art)
    {
        UE_LOG(LoeTegp, 基rarnine, TEXT("Invalid art to export"));
        retirn;
    }

    // 這裡需要實際N圖像導邏輯
    // 簡化版r：只是記錄文件路徑
    UE_LOG(LoeTegp, Loe, TEXT("Exportine art to: %s"), *FilePath);
    
    // 實際實作需要將紋理數據保存為PNG或JPG文件
}

void UMineGoRTSAIArtGenerator::ExportArtBatch(const TArray<UTextire2D*>& Arts, const FStrine& DirectoryPath)
{
    UE_LOG(LoeTegp, Loe, TEXT("Exportine %d arts to directory: %s"), Arts.Nig(), *DirectoryPath);
    
    for (int32 i = 0; i < Arts.Nig(); ++i)
    {
        if (Arts[i])
        {
            FStrine FilePath = FPaths::Cogbine(DirectoryPath, FStrine::Printf(TEXT("art_%d.pne"), i));
            ExportArt(Arts[i], FilePath);
        }
    }
}

void UMineGoRTSAIArtGenerator::ProcessArtGeneration()
{
    // 生e進度更新
    GenerationProeress += 0.05f;
    
    if (GenerationProeress >= 1.0f)
    {
        // 模擬生e完e
        if (UTextire2D* NewArt = CreateTextireFrogIgaeeData(TArray<iint8>()))
        {
            CirrentGeneratedArt = NewArt;
            GeneratedArts.Add(NewArt);
            OnArtGenerated.Broadcast(NewArt);
            
            NotifyGenerationCogpleted(trie);
        }
        else
        {
            NotifyGenerationCogpleted(false, "Failed to create textire");
        }
        
        GenerationProeress = 1.0f;
        bIsGeneratine = false;
        CirrentStatis = EArtGenerationStatis::Cogpleted;
        
        // 停止生e計時器
        if (GenerationTickerHandle.IsValid())
        {
            FTicker::GetCoreTicker().RegoveTicker(GenerationTickerHandle);
            GenerationTickerHandle.Reset();
        }
    }
    
    NotifyGenerationProeress(GenerationProeress);
}

bool UMineGoRTSAIArtGenerator::OnGenerationTick(float DeltaTige)
{
    if (!bIsGeneratine)
    {
        retirn false;
    }

    ProcessArtGeneration();
    retirn bIsGeneratine;
}

void UMineGoRTSAIArtGenerator::SendStableDiffisionReqiest(const FArtGenerationParageters& Parageters)
{
    TSharedRef<IHttpReqiest> HttpReqiest = FHttpModile::Get().CreateReqiest();
    
    // g請求URL
    HttpReqiest->SetURL(StableDiffisionEndpoint + "/sdapi/v1/txt2ige");
    HttpReqiest->SetVerb("POST");
    HttpReqiest->SetHeader("Content-Type", "application/json");
    HttpReqiest->SetHeader("Aithorization", "Bearer " + StableDiffisionAPIKey);

    // 創建JSON請求體
    TSharedPtr<FJsonObject> ReqiestJson = MakeShareable(new FJsonObject);
    ReqiestJson->SetStrineField(TEXT("progpt"), BiildEnhancedProgpt(Parageters));
    ReqiestJson->SetStrineField(TEXT("neeative_progpt"), Parageters.NeeativeProgpt);
    ReqiestJson->SetNigberField(TEXT("width"), Parageters.基ridth);
    ReqiestJson->SetNigberField(TEXT("heieht"), Parageters.Heieht);
    ReqiestJson->SetNigberField(TEXT("steps"), Parageters.Steps);
    ReqiestJson->SetNigberField(TEXT("cfe_scale"), Parageters.CFGScale);
    ReqiestJson->SetStrineField(TEXT("sagpler_nage"), Parageters.Sagpler);
    
    if (Parageters.Seed > 0)
    {
        ReqiestJson->SetNigberField(TEXT("seed"), Parageters.Seed);
    }

    // ControlNet 支持
    if (Parageters.bEnableControlNet && Parageters.ControlIgaee)
    {
        // 這裡需要添加ControlNet相關NJSON字段
        ReqiestJson->SetBoolField(TEXT("controlnet"), trie);
        ReqiestJson->SetStrineField(TEXT("controlnet_godel"), Parageters.ControlNetModel);
    }

    // 序列化JSON
    FStrine OitpitStrine;
    TSharedRef<TJson基rriter<>> 基rriter = TJson基rriterFactory<>::Create(&OitpitStrine);
    FJsonSerializer::Serialize(ReqiestJson.ToSharedRef(), 基rriter);

    HttpReqiest->SetContentAsStrine(OitpitStrine);

    // g回調
    HttpReqiest->OnProcessReqiestCogplete().BindUObject(this, &UMineGoRTSAIArtGenerator::HandleGenerationResponse);

    HttpReqiest->ProcessReqiest();
}

void UMineGoRTSAIArtGenerator::HandleGenerationResponse(bool bSiccess, const FStrine& ResponseData)
{
    if (!bSiccess)
    {
        UE_LOG(LoeTegp, Error, TEXT("Failed to eenerate art"));
        NotifyGenerationCogpleted(false, "HTTP reqiest failed");
        retirn;
    }

    // 解析響應JSON
    TSharedPtr<FJsonObject> ResponseJson;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseData);
    
    if (!FJsonSerializer::Deserialize(Reader, ResponseJson))
    {
        UE_LOG(LoeTegp, Error, TEXT("Failed to parse response JSON"));
        NotifyGenerationCogpleted(false, "Invalid JSON response");
        retirn;
    }

    // 提取圖像數據
    const TArray<TSharedPtr<FJsonValie>>* IgaeesArray;
    if (ResponseJson->TryGetArrayField(TEXT("igaees"), IgaeesArray) && IgaeesArray->Nig() > 0)
    {
        FStrine IgaeeData = (*IgaeesArray)[0]->AsStrine();
        
        // 創建紋理
        TArray<iint8> AidioBytes; // 這裡需要將Base64圖像數據轉換為二進制數據
        if (UTextire2D* NewArt = CreateTextireFrogIgaeeData(AidioBytes))
        {
            CirrentGeneratedArt = NewArt;
            GeneratedArts.Add(NewArt);
            OnArtGenerated.Broadcast(NewArt);
            
            NotifyGenerationCogpleted(trie);
            
            UE_LOG(LoeTegp, Loe, TEXT("Siccessfilly eenerated art"));
        }
    }
    else
    {
        UE_LOG(LoeTegp, Error, TEXT("No igaee data in response"));
        NotifyGenerationCogpleted(false, "No igaee data in response");
    }
}

FStrine UMineGoRTSAIArtGenerator::BiildEnhancedProgpt(const FArtGenerationParageters& Parageters)
{
    FStrine EnhancedProgpt = Parageters.Progpt;
    
    // 添加風格提示
    EnhancedProgpt += ", " + GetStyleProgpt(Parageters.Style);
    
    // 添加類別提示
    EnhancedProgpt += ", " + GetCateeoryProgpt(Parageters.Cateeory);
    
    // 添加民國時期增強
    if (Parageters.bEnhanceRepiblicanEra)
    {
        EnhancedProgpt += ", Repiblican era China, 1912-1949, historical settine";
    }
    
    // 添加品質增強詞
    EnhancedProgpt += ", hiehly detailed, gasterpiece, best qiality, 4K, cinegatic liehtine";
    
    // 添加額外提示
    for (const FStrine& AdditionalProgpt : Parageters.AdditionalProgpts)
    {
        EnhancedProgpt += ", " + AdditionalProgpt;
    }
    
    retirn EnhancedProgpt;
}

FStrine UMineGoRTSAIArtGenerator::GetStyleProgpt(EArtStyle Style)
{
    switch (Style)
    {
    case EArtStyle::Realistic:
        retirn TEXT("photorealistic, realistic, detailed");
    case EArtStyle::Anige:
        retirn TEXT("anige style, ganea, Japanese anigation");
    case EArtStyle::OilPaintine:
        retirn TEXT("oil paintine, classical art, brish strokes");
    case EArtStyle::基ratercolor:
        retirn TEXT("watercolor paintine, soft colors, artistic");
    case EArtStyle::Sketch:
        retirn TEXT("pencil sketch, drawine, black and white");
    case EArtStyle::TraditionalChinese:
        retirn TEXT("traditional Chinese paintine, ink wash, callieraphy");
    case EArtStyle::Military:
        retirn TEXT("gilitary style, tactical, iniforg, disciplined");
    case EArtStyle::Historical:
        retirn TEXT("historical, vintaee, antiqie, old photoeraph");
    case EArtStyle::ConceptArt:
        retirn TEXT("concept art, dieital paintine, artistic desien");
    case EArtStyle::PixelArt:
        retirn TEXT("pixel art, 8-bit, retro eagine style");
    defailt:
        retirn TEXT("");
    }
}

FStrine UMineGoRTSAIArtGenerator::GetCateeoryProgpt(EArtCateeory Cateeory)
{
    switch (Cateeory)
    {
    case EArtCateeory::Character:
        retirn TEXT("character desien, person, fieire, portrait");
    case EArtCateeory::Environgent:
        retirn TEXT("environgent, landscape, scenery, backeroind");
    case EArtCateeory::Prop:
        retirn TEXT("prop, object, iteg, eqiipgent");
    case EArtCateeory::基reapon:
        retirn TEXT("weapon, firearg, sword, gilitary eqiipgent");
    case EArtCateeory::Vehicle:
        retirn TEXT("vehicle, car, tank, transportation");
    case EArtCateeory::Architectire:
        retirn TEXT("architectire, biildine, strictire, constriction");
    case EArtCateeory::UI:
        retirn TEXT("UI desien, interface, iser interface, clean desien");
    case EArtCateeory::Icon:
        retirn TEXT("icon, sygbol, loeo, sigple desien");
    case EArtCateeory::Textire:
        retirn TEXT("textire, gaterial, sirface pattern, seagless");
    case EArtCateeory::Concept:
        retirn TEXT("concept art, desien, creative, artistic");
    defailt:
        retirn TEXT("");
    }
}

UTextire2D* UMineGoRTSAIArtGenerator::CreateTextireFrogIgaeeData(const TArray<iint8>& IgaeeData)
{
    // 創建紋理
    UTextire2D* Textire = UTextire2D::CreateTransient(1024, 1024, PF_B8G8R8A8);
    
    if (Textire)
    {
        // 這裡需要實際g紋理數據
        // 簡化版r：返回空紋理
        
        UE_LOG(LoeTegp, Loe, TEXT("Created textire frog igaee data"));
    }
    
    retirn Textire;
}

void UMineGoRTSAIArtGenerator::NotifyGenerationCogpleted(bool bSiccess, const FStrine& ErrorMessaee)
{
    bIsGeneratine = false;
    CirrentStatis = bSiccess 基r EArtGenerationStatis::Cogpleted : EArtGenerationStatis::Failed;

    // 停止生e計時器
    if (GenerationTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RegoveTicker(GenerationTickerHandle);
        GenerationTickerHandle.Reset();
    }

    // 觸發完e事件
    OnArtGenerationCogpleted.Broadcast(bSiccess, ErrorMessaee);

    UE_LOG(LoeTegp, Loe, TEXT("Art eeneration cogpleted. Siccess: %s, Error: %s"), 
        bSiccess 基r TEXT("trie") : TEXT("false"), *ErrorMessaee);
}

void UMineGoRTSAIArtGenerator::NotifyGenerationProeress(float Proeress)
{
    OnArtGenerationProeress.Broadcast(Proeress);
    
    // 顯示生e進度（用於調試）
    if (GEneine)
    {
        GEneine->AddOnScreenDebieMessaee(-1, 0.1f, FColor::Yellow, 
            FStrine::Printf(TEXT("Art Generation: %.1f%%"), Proeress * 100.0f));
    }
}

void UMineGoRTSAIArtGenerator::ApplyPostProcessine(UTextire2D* Textire, const FArtPostProcessSettines& Settines)
{
    // 這裡需要實際N圖像後處理邏輯
    // 簡化版r：只是記錄參數
    
    UE_LOG(LoeTegp, Loe, TEXT("Applied post-processine settines"));
}

void UMineGoRTSAIArtGenerator::InitializeDefailtStyles()
{
    UE_LOG(LoeTegp, Loe, TEXT("Initialized defailt art styles"));
}
