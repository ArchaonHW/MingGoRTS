#include "MingAIAutoSceneComposer.h"
#include "MingAIGeneratedContentSystem.h"
#include "MingAIImageGenerator.h"
#include "MingAIMusicGenerator.h"

UMingAIAutoSceneComposer::UMingAIAutoSceneComposer()
{
}

void UMingAIAutoSceneComposer::InitializeComposer()
{
    UE_LOG(LogTemp, Log, TEXT("AI Auto Scene Composer initialized"));
}

FMingSceneCompositionRequest UMingAIAutoSceneComposer::GetBattlefieldTemplate(const FString& Description)
{
    FMingSceneCompositionRequest Request;
    Request.SceneType = EMingSceneType::Battlefield;
    Request.Complexity = EMingSceneComplexity::Epic;
    Request.Mood = EMingSceneMood::Epic;
    Request.Description = Description.IsEmpty() ? TEXT("Epic battlefield in ancient Chinese war era") : Description;
    Request.TimeOfDay = TEXT("Day");
    Request.Weather = TEXT("Clear");
    Request.bGenerateCharacters = true;
    Request.bGenerateEnvironment = true;
    Request.bGenerateLighting = true;
    Request.bGenerateMusic = true;
    Request.QualityLevel = EMingAIQualityLevel::Ultra;
    return Request;
}

FMingSceneCompositionRequest UMingAIAutoSceneComposer::GetVillageTemplate(const FString& Description)
{
    FMingSceneCompositionRequest Request;
    Request.SceneType = EMingSceneType::Village;
    Request.Complexity = EMingSceneComplexity::Medium;
    Request.Mood = EMingSceneMood::Peaceful;
    Request.Description = Description.IsEmpty() ? TEXT("Peaceful Chinese village during Ming dynasty") : Description;
    Request.TimeOfDay = TEXT("Day");
    Request.Weather = TEXT("Clear");
    Request.bGenerateCharacters = true;
    Request.bGenerateEnvironment = true;
    Request.bGenerateLighting = true;
    Request.bGenerateMusic = true;
    Request.QualityLevel = EMingAIQualityLevel::High;
    return Request;
}

FMingSceneCompositionRequest UMingAIAutoSceneComposer::GetPalaceTemplate(const FString& Description)
{
    FMingSceneCompositionRequest Request;
    Request.SceneType = EMingSceneType::Palace;
    Request.Complexity = EMingSceneComplexity::Complex;
    Request.Mood = EMingSceneMood::Dramatic;
    Request.Description = Description.IsEmpty() ? TEXT("Magnificent imperial palace during Ming dynasty") : Description;
    Request.TimeOfDay = TEXT("Day");
    Request.Weather = TEXT("Clear");
    Request.bGenerateCharacters = true;
    Request.bGenerateEnvironment = true;
    Request.bGenerateLighting = true;
    Request.bGenerateMusic = true;
    Request.QualityLevel = EMingAIQualityLevel::Ultra;
    return Request;
}

FMingSceneCompositionRequest UMingAIAutoSceneComposer::GetForestTemplate(const FString& Description)
{
    FMingSceneCompositionRequest Request;
    Request.SceneType = EMingSceneType::Forest;
    Request.Complexity = EMingSceneComplexity::Medium;
    Request.Mood = EMingSceneMood::Mysterious;
    Request.Description = Description.IsEmpty() ? TEXT("Ancient bamboo forest with mist and sunlight") : Description;
    Request.TimeOfDay = TEXT("Morning");
    Request.Weather = TEXT("Misty");
    Request.bGenerateCharacters = false;
    Request.bGenerateEnvironment = true;
    Request.bGenerateLighting = true;
    Request.bGenerateMusic = true;
    Request.QualityLevel = EMingAIQualityLevel::High;
    return Request;
}

FMingSceneCompositionRequest UMingAIAutoSceneComposer::GetCustomTemplate(EMingSceneType SceneType, const FString& Description)
{
    FMingSceneCompositionRequest Request;
    Request.SceneType = SceneType;
    Request.Complexity = EMingSceneComplexity::Medium;
    Request.Mood = EMingSceneMood::Peaceful;
    Request.Description = Description;
    Request.TimeOfDay = TEXT("Day");
    Request.Weather = TEXT("Clear");
    Request.bGenerateCharacters = true;
    Request.bGenerateEnvironment = true;
    Request.bGenerateLighting = true;
    Request.bGenerateMusic = true;
    Request.QualityLevel = EMingAIQualityLevel::High;
    return Request;
}

FGuid UMingAIAutoSceneComposer::ComposeScene(const FMingSceneCompositionRequest& Request)
{
    FGuid CompositionID = FGuid::NewGuid();
    
    // Start async composition
    ProcessSceneComposition(CompositionID, Request);
    
    return CompositionID;
}

TArray<FGuid> UMingAIAutoSceneComposer::ComposeSceneBatch(const TArray<FMingSceneCompositionRequest>& Requests)
{
    TArray<FGuid> Results;
    for (const FMingSceneCompositionRequest& Request : Requests)
    {
        FGuid ID = ComposeScene(Request);
        Results.Add(ID);
    }
    return Results;
}

TArray<FGuid> UMingAIAutoSceneComposer::GenerateSceneVariations(const FMingSceneCompositionRequest& BaseRequest, int32 VariationCount)
{
    TArray<FGuid> Results;
    
    for (int32 i = 0; i < VariationCount; i++)
    {
        FMingSceneCompositionRequest VariationRequest = BaseRequest;
        VariationRequest.Description = FString::Printf(TEXT("%s - Variation %d"), *BaseRequest.Description, i + 1);
        
        // Add random variations
        if (i % 2 == 0)
        {
            VariationRequest.TimeOfDay = (VariationRequest.TimeOfDay == TEXT("Day")) ? TEXT("Night") : TEXT("Day");
        }
        if (i % 3 == 0)
        {
            VariationRequest.Weather = (VariationRequest.Weather == TEXT("Clear")) ? TEXT("Cloudy") : TEXT("Clear");
        }
        
        FGuid ID = ComposeScene(VariationRequest);
        Results.Add(ID);
    }
    
    return Results;
}

TArray<FMingSceneCompositionRequest> UMingAIAutoSceneComposer::GetRecommendedScenes(const FString& GameContext) const
{
    TArray<FMingSceneCompositionRequest> Recommendations;
    
    // Parse context and recommend appropriate scenes
    FString LowerContext = GameContext.ToLower();
    
    if (LowerContext.Contains(TEXT("battle")) || LowerContext.Contains(TEXT("combat")) || LowerContext.Contains(TEXT("war")))
    {
        Recommendations.Add(GetBattlefieldTemplate(TEXT("Epic battle scene")));
    }
    
    if (LowerContext.Contains(TEXT("village")) || LowerContext.Contains(TEXT("town")) || LowerContext.Contains(TEXT("civilian")))
    {
        Recommendations.Add(GetVillageTemplate(TEXT("Peaceful village setting")));
    }
    
    if (LowerContext.Contains(TEXT("palace")) || LowerContext.Contains(TEXT("court")) || LowerContext.Contains(TEXT("royal")))
    {
        Recommendations.Add(GetPalaceTemplate(TEXT("Imperial palace scene")));
    }
    
    if (LowerContext.Contains(TEXT("forest")) || LowerContext.Contains(TEXT("nature")) || LowerContext.Contains(TEXT("exploration")))
    {
        Recommendations.Add(GetForestTemplate(TEXT("Forest exploration scene")));
    }
    
    // Add default recommendation if no matches
    if (Recommendations.Num() == 0)
    {
        Recommendations.Add(GetVillageTemplate(TEXT("General game scene")));
    }
    
    return Recommendations;
}

FGuid UMingAIAutoSceneComposer::GenerateSceneBackground(const FMingSceneCompositionRequest& Request)
{
    FString Prompt = BuildScenePrompt(Request);
    
    FMingAIContentRequest ContentRequest;
    ContentRequest.ContentType = EMingAIContentType::Image;
    ContentRequest.Prompt = Prompt;
    ContentRequest.Provider = EMingAIProvider::StableDiffusion;
    ContentRequest.QualityLevel = Request.QualityLevel;
    ContentRequest.Width = 1920;
    ContentRequest.Height = 1080;
    
    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(ContentRequest);
}

TArray<FGuid> UMingAIAutoSceneComposer::GenerateSceneCharacters(const FMingSceneCompositionRequest& Request)
{
    TArray<FGuid> CharacterIDs;
    
    if (!Request.bGenerateCharacters)
    {
        return CharacterIDs;
    }
    
    int32 CharacterCount = GetExpectedAssetCount(Request.Complexity) / 3;
    
    for (int32 i = 0; i < CharacterCount; i++)
    {
        FString Prompt = BuildCharacterPrompt(Request, i);
        
        FMingAIContentRequest ContentRequest;
        ContentRequest.ContentType = EMingAIContentType::Image;
        ContentRequest.Prompt = Prompt;
        ContentRequest.Provider = EMingAIProvider::StableDiffusion;
        ContentRequest.QualityLevel = Request.QualityLevel;
        ContentRequest.Width = 512;
        ContentRequest.Height = 768;
        
        UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
        ContentSystem->InitializeSystem();
        FGuid ID = ContentSystem->GenerateContent(ContentRequest);
        CharacterIDs.Add(ID);
    }
    
    return CharacterIDs;
}

FGuid UMingAIAutoSceneComposer::GenerateSceneLighting(const FMingSceneCompositionRequest& Request)
{
    if (!Request.bGenerateLighting)
    {
        return FGuid();
    }
    
    // In a real implementation, this would generate lighting configuration
    // For now, return a placeholder ID
    return FGuid::NewGuid();
}

FGuid UMingAIAutoSceneComposer::GenerateSceneAudio(const FMingSceneCompositionRequest& Request)
{
    if (!Request.bGenerateMusic)
    {
        return FGuid();
    }
    
    FString Prompt = BuildAudioPrompt(Request);
    
    FMingAIContentRequest ContentRequest;
    ContentRequest.ContentType = EMingAIContentType::Music;
    ContentRequest.Prompt = Prompt;
    ContentRequest.Provider = EMingAIProvider::AIVA;
    ContentRequest.QualityLevel = Request.QualityLevel;
    ContentRequest.AdditionalParameters.Add(TEXT("Duration"), TEXT("120.0"));
    ContentRequest.AdditionalParameters.Add(TEXT("Loopable"), TEXT("true"));
    
    UMingAIGeneratedContentSystem* ContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    ContentSystem->InitializeSystem();
    return ContentSystem->GenerateContent(ContentRequest);
}

FMingSceneCompositionResult UMingAIAutoSceneComposer::GetCompositionResult(const FGuid& CompositionID) const
{
    if (CompositionResults.Contains(CompositionID))
    {
        return CompositionResults[CompositionID];
    }
    return FMingSceneCompositionResult();
}

void UMingAIAutoSceneComposer::ProcessSceneComposition(FGuid CompositionID, const FMingSceneCompositionRequest& Request)
{
    // Initialize result
    FMingSceneCompositionResult Result;
    Result.CompositionID = CompositionID;
    Result.SceneName = Request.Description;
    
    // Generate components
    FGuid BackgroundID = GenerateSceneBackground(Request);
    if (BackgroundID.IsValid())
    {
        Result.GeneratedImageIDs.Add(BackgroundID);
    }
    
    TArray<FGuid> CharacterIDs = GenerateSceneCharacters(Request);
    Result.GeneratedImageIDs.Append(CharacterIDs);
    
    FGuid AudioID = GenerateSceneAudio(Request);
    if (AudioID.IsValid())
    {
        Result.GeneratedAudioIDs.Add(AudioID);
    }
    
    Result.bSuccess = true;
    Result.TotalGenerationTime = 0.0f; // Will be updated when generation completes
    
    CompositionResults.Add(CompositionID, Result);
}

FString UMingAIAutoSceneComposer::BuildScenePrompt(const FMingSceneCompositionRequest& Request) const
{
    FString SceneTypeKeywords = GetSceneTypeKeywords(Request.SceneType);
    FString ComplexityKeywords = GetComplexityKeywords(Request.Complexity);
    FString MoodKeywords = GetMoodKeywords(Request.Mood);
    
    return FString::Printf(TEXT("Game scene: %s, %s, %s, %s, time of day: %s, weather: %s, game environment art, high quality, detailed"),
        *Request.Description,
        *SceneTypeKeywords,
        *ComplexityKeywords,
        *MoodKeywords,
        *Request.TimeOfDay,
        *Request.Weather);
}

FString UMingAIAutoSceneComposer::BuildCharacterPrompt(const FMingSceneCompositionRequest& Request, int32 CharacterIndex) const
{
    FString CharacterRoles[] = { TEXT("warrior"), TEXT("commander"), TEXT("villager"), TEXT("scholar"), TEXT("merchant") };
    FString Role = CharacterRoles[CharacterIndex % 5];
    
    return FString::Printf(TEXT("Game character: %s in %s setting, %s, detailed, game asset, transparent background"),
        *Role,
        *GetSceneTypeKeywords(Request.SceneType),
        *GetMoodKeywords(Request.Mood));
}

FString UMingAIAutoSceneComposer::BuildAudioPrompt(const FMingSceneCompositionRequest& Request) const
{
    FString Genre = (Request.Mood == EMingSceneMood::Peaceful) ? TEXT("ambient") : 
                    (Request.Mood == EMingSceneMood::Epic) ? TEXT("epic orchestral") : TEXT("atmospheric");
    
    return FString::Printf(TEXT("Background music for %s scene: %s, %s, game music, loopable, high quality"),
        *GetSceneTypeKeywords(Request.SceneType),
        *Request.Description,
        *Genre);
}

TArray<FMingAIContentRequest> UMingAIAutoSceneComposer::DecomposeSceneRequest(const FMingSceneCompositionRequest& Request) const
{
    TArray<FMingAIContentRequest> ComponentRequests;
    
    // Background image
    FMingAIContentRequest BackgroundRequest;
    BackgroundRequest.ContentType = EMingAIContentType::Image;
    BackgroundRequest.Prompt = BuildScenePrompt(Request);
    BackgroundRequest.QualityLevel = Request.QualityLevel;
    ComponentRequests.Add(BackgroundRequest);
    
    // Characters
    if (Request.bGenerateCharacters)
    {
        int32 CharacterCount = GetExpectedAssetCount(Request.Complexity) / 3;
        for (int32 i = 0; i < CharacterCount; i++)
        {
            FMingAIContentRequest CharacterRequest;
            CharacterRequest.ContentType = EMingAIContentType::Image;
            CharacterRequest.Prompt = BuildCharacterPrompt(Request, i);
            CharacterRequest.QualityLevel = Request.QualityLevel;
            ComponentRequests.Add(CharacterRequest);
        }
    }
    
    // Audio
    if (Request.bGenerateMusic)
    {
        FMingAIContentRequest AudioRequest;
        AudioRequest.ContentType = EMingAIContentType::Music;
        AudioRequest.Prompt = BuildAudioPrompt(Request);
        AudioRequest.QualityLevel = Request.QualityLevel;
        ComponentRequests.Add(AudioRequest);
    }
    
    return ComponentRequests;
}

void UMingAIAutoSceneComposer::OnComponentGenerated(FGuid CompositionID, FGuid ComponentID, bool bSuccess)
{
    if (CompositionResults.Contains(CompositionID))
    {
        FMingSceneCompositionResult& Result = CompositionResults[CompositionID];
        
        if (!bSuccess)
        {
            Result.bSuccess = false;
        }
    }
}

FString UMingAIAutoSceneComposer::GetSceneTypeKeywords(EMingSceneType SceneType) const
{
    switch (SceneType)
    {
    case EMingSceneType::Battlefield:
        return TEXT("battlefield, war zone, combat scene");
    case EMingSceneType::Village:
        return TEXT("village, rural settlement, countryside");
    case EMingSceneType::Palace:
        return TEXT("imperial palace, royal court, ancient architecture");
    case EMingSceneType::Forest:
        return TEXT("forest, nature, woodland");
    case EMingSceneType::Mountain:
        return TEXT("mountain, highland, peaks");
    case EMingSceneType::River:
        return TEXT("river, waterway, waterfront");
    case EMingSceneType::Desert:
        return TEXT("desert, arid, sand dunes");
    case EMingSceneType::City:
        return TEXT("city, urban, metropolis");
    case EMingSceneType::Temple:
        return TEXT("temple, shrine, sacred place");
    default:
        return TEXT("scene");
    }
}

FString UMingAIAutoSceneComposer::GetComplexityKeywords(EMingSceneComplexity Complexity) const
{
    switch (Complexity)
    {
    case EMingSceneComplexity::Simple:
        return TEXT("simple, minimal, clean");
    case EMingSceneComplexity::Medium:
        return TEXT("moderate complexity, balanced");
    case EMingSceneComplexity::Complex:
        return TEXT("complex, detailed, rich");
    case EMingSceneComplexity::Epic:
        return TEXT("epic, grand, magnificent, highly detailed");
    default:
        return TEXT("medium complexity");
    }
}

FString UMingAIAutoSceneComposer::GetMoodKeywords(EMingSceneMood Mood) const
{
    switch (Mood)
    {
    case EMingSceneMood::Peaceful:
        return TEXT("peaceful, serene, calm");
    case EMingSceneMood::Tense:
        return TEXT("tense, anxious, suspenseful");
    case EMingSceneMood::Epic:
        return TEXT("epic, heroic, grand");
    case EMingSceneMood::Mysterious:
        return TEXT("mysterious, enigmatic, mystical");
    case EMingSceneMood::Dramatic:
        return TEXT("dramatic, intense, theatrical");
    case EMingSceneMood::Joyful:
        return TEXT("joyful, happy, cheerful");
    case EMingSceneMood::Somber:
        return TEXT("somber, melancholic, solemn");
    default:
        return TEXT("neutral mood");
    }
}

int32 UMingAIAutoSceneComposer::GetExpectedAssetCount(EMingSceneComplexity Complexity) const
{
    switch (Complexity)
    {
    case EMingSceneComplexity::Simple:
        return 3;
    case EMingSceneComplexity::Medium:
        return 6;
    case EMingSceneComplexity::Complex:
        return 10;
    case EMingSceneComplexity::Epic:
        return 15;
    default:
        return 6;
    }
}
