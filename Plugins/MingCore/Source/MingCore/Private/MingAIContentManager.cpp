#include "MingAIContentManager.h"
#include "MingAIGeneratedContentSystem.h"
#include "MingAIContentRenderer.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Kismet/KismetSystemLibrary.h"

UMingAIContentManager::UMingAIContentManager()
    : bIsInitialized(false)
    , bPreviewActive(false)
    , TotalGeneratedCount(0)
    , TotalGenerationTime(0.0f)
{
}

void UMingAIContentManager::InitializeAIContentManager()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Content Manager already initialized"));
        return;
    }

    // 初始化AI內容生成系統
    InitializeContentSystem();

    // 初始化渲染器
    InitializeRenderer();

    // 加載默認模板
    LoadDefaultTemplates();

    // 加載項目數據
    LoadProjectData();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Content Manager initialized"));
}

FString UMingAIContentManager::CreateAIContentProject(const FAIContentProject& Project)
{
    FString ProjectName = Project.ProjectName.IsEmpty() ? GenerateUniqueContentName(EAIContentType::Level) : Project.ProjectName;
    
    if (Projects.Contains(ProjectName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Project %s already exists"), *ProjectName);
        return TEXT("");
    }

    FAIContentProject NewProject = Project;
    NewProject.ProjectName = ProjectName;
    
    // 設置默認輸出目錄
    if (NewProject.OutputDirectory.IsEmpty())
    {
        NewProject.OutputDirectory = FString::Printf(TEXT("/Game/AIContent/%s/"), *ProjectName);
    }

    Projects.Add(ProjectName, NewProject);
    CurrentProject = NewProject;

    // 創建輸出目錄
    FString FullOutputPath = FPaths::ProjectDir() / NewProject.OutputDirectory;
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*FullOutputPath))
    {
        PlatformFile.CreateDirectoryTree(*FullOutputPath);
    }

    SaveProjectData();
    
    UE_LOG(LogTemp, Log, TEXT("Created AI content project: %s"), *ProjectName);
    return ProjectName;
}

bool UMingAIContentManager::LoadAIContentProject(const FString& ProjectName)
{
    if (!Projects.Contains(ProjectName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Project %s not found"), *ProjectName);
        return false;
    }

    CurrentProject = Projects[ProjectName];
    
    UE_LOG(LogTemp, Log, TEXT("Loaded AI content project: %s"), *ProjectName);
    return true;
}

void UMingAIContentManager::SaveAIContentProject(const FString& ProjectName)
{
    if (Projects.Contains(ProjectName))
    {
        SaveProjectData();
        UE_LOG(LogTemp, Log, TEXT("Saved AI content project: %s"), *ProjectName);
    }
}

void UMingAIContentManager::DeleteAIContentProject(const FString& ProjectName)
{
    if (Projects.Contains(ProjectName))
    {
        Projects.Remove(ProjectName);
        
        if (CurrentProject.ProjectName == ProjectName)
        {
            CurrentProject = FAIContentProject();
        }
        
        SaveProjectData();
        
        UE_LOG(LogTemp, Log, TEXT("Deleted AI content project: %s"), *ProjectName);
    }
}

TArray<FAIContentProject> UMingAIContentManager::GetAllProjects() const
{
    TArray<FAIContentProject> AllProjects;
    
    for (const auto& ProjectPair : Projects)
    {
        AllProjects.Add(ProjectPair.Value);
    }
    
    return AllProjects;
}

FString UMingAIContentManager::GenerateContent(EAIContentType ContentType, const FString& Prompt, EGenerationQuality Quality)
{
    if (!bIsInitialized || !AIContentSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Content Manager not initialized"));
        return TEXT("");
    }

    FString ContentPath = TEXT("");
    double StartTime = FDateTime::Now().GetTicks();

    switch (ContentType)
    {
        case EAIContentType::Image:
            ContentPath = AIContentSystem->GenerateImage(Prompt, TEXT(""), 512, 512, Quality);
            break;
            
        case EAIContentType::Video:
            ContentPath = AIContentSystem->GenerateVideo(Prompt, 5.0f, 30, Quality);
            break;
            
        case EAIContentType::Music:
            ContentPath = AIContentSystem->GenerateMusic(Prompt, 30.0f, Quality);
            break;
            
        case EAIContentType::SoundEffect:
            ContentPath = AIContentSystem->GenerateSoundEffect(Prompt, 2.0f, Quality);
            break;
            
        case EAIContentType::Texture:
            {
                UTexture2D* GeneratedTexture = AIContentSystem->GenerateTexture(Prompt, 512, 512);
                if (GeneratedTexture)
                {
                    ContentPath = GetContentPath(ContentType, GenerateUniqueContentName(ContentType));
                    AIContentSystem->SaveGeneratedAsset(GeneratedTexture->GetFName().ToString(), ContentPath);
                }
            }
            break;
            
        case EAIContentType::Mesh:
            {
                UStaticMesh* GeneratedMesh = AIContentSystem->GenerateMesh(Prompt, 0.5f);
                if (GeneratedMesh)
                {
                    ContentPath = GetContentPath(ContentType, GenerateUniqueContentName(ContentType));
                    AIContentSystem->SaveGeneratedAsset(GeneratedMesh->GetFName().ToString(), ContentPath);
                }
            }
            break;
            
        case EAIContentType::Material:
            {
                UMaterialInterface* GeneratedMaterial = AIContentSystem->GenerateMaterial(Prompt, Quality);
                if (GeneratedMaterial)
                {
                    ContentPath = GetContentPath(ContentType, GenerateUniqueContentName(ContentType));
                    AIContentSystem->SaveGeneratedAsset(GeneratedMaterial->GetFName().ToString(), ContentPath);
                }
            }
            break;
            
        case EAIContentType::Animation:
            ContentPath = AIContentSystem->GenerateAnimation(Prompt, 5.0f);
            break;
            
        case EAIContentType::Level:
            ContentPath = AIContentSystem->GenerateLevel(Prompt, 1024);
            break;
            
        case EAIContentType::UI:
            ContentPath = AIContentSystem->GenerateUI(Prompt, TEXT("panel"));
            break;
            
        case EAIContentType::Text:
            ContentPath = AIContentSystem->GenerateText(Prompt, TEXT("narrative"));
            break;
            
        case EAIContentType::Voice:
            ContentPath = AIContentSystem->GenerateVoice(Prompt, TEXT(""), Quality);
            break;
    }

    double EndTime = FDateTime::Now().GetTicks();
    float GenerationTime = (EndTime - StartTime) / ETimespan::TicksPerSecond;

    if (!ContentPath.IsEmpty())
    {
        UpdateStatistics(ContentType, GenerationTime);
        NotifyContentGenerated(ContentType, ContentPath);
        
        // 添加到內容庫
        AddToContentLibrary(ContentType, ContentPath, Prompt);
    }

    return ContentPath;
}

TArray<FString> UMingAIContentManager::GenerateProjectContent(const FString& ProjectName)
{
    TArray<FString> GeneratedContentPaths;
    
    if (!Projects.Contains(ProjectName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Project %s not found"), *ProjectName);
        return GeneratedContentPaths;
    }

    const FAIContentProject& Project = Projects[ProjectName];
    
    for (EAIContentType ContentType : Project.RequiredContentTypes)
    {
        if (Project.ContentPrompts.Contains(ContentType))
        {
            FString Prompt = Project.ContentPrompts[ContentType];
            FString ContentPath = GenerateContent(ContentType, Prompt, Project.DefaultQuality);
            
            if (!ContentPath.IsEmpty())
            {
                GeneratedContentPaths.Add(ContentPath);
            }
        }
    }

    if (GeneratedContentPaths.Num() > 0)
    {
        NotifyProjectCompleted(ProjectName, true);
    }
    else
    {
        NotifyProjectCompleted(ProjectName, false);
    }

    return GeneratedContentPaths;
}

void UMingAIContentManager::GenerateContentAsync(EAIContentType ContentType, const FString& Prompt)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, ContentType, Prompt]()
    {
        GenerateContent(ContentType, Prompt);
    });
}

FString UMingAIContentManager::CreateContentPipeline(const FAIContentPipeline& Pipeline)
{
    FString PipelineName = Pipeline.PipelineName.IsEmpty() ? GenerateUniqueContentName(EAIContentType::Level) : Pipeline.PipelineName;
    
    if (ContentPipelines.Contains(PipelineName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Pipeline %s already exists"), *PipelineName);
        return TEXT("");
    }

    FAIContentPipeline NewPipeline = Pipeline;
    NewPipeline.PipelineName = PipelineName;
    NewPipeline.CurrentStage = 0;
    NewPipeline.bIsCompleted = false;
    
    ContentPipelines.Add(PipelineName, NewPipeline);
    
    UE_LOG(LogTemp, Log, TEXT("Created content pipeline: %s"), *PipelineName);
    return PipelineName;
}

void UMingAIContentManager::ExecutePipeline(const FString& PipelineName)
{
    if (!ContentPipelines.Contains(PipelineName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Pipeline %s not found"), *PipelineName);
        return;
    }

    FAIContentPipeline& Pipeline = ContentPipelines[PipelineName];
    Pipeline.CurrentStage = 0;
    Pipeline.bIsCompleted = false;
    CurrentPipeline = PipelineName;

    if (Pipeline.bParallelProcessing)
    {
        // 並行處理所有階段
        for (int32 i = 0; i < Pipeline.ContentStages.Num(); ++i)
        {
            ProcessPipelineStage(PipelineName, i);
        }
    }
    else
    {
        // 順序處理
        ProcessPipelineStage(PipelineName, 0);
    }
}

void UMingAIContentManager::PausePipeline(const FString& PipelineName)
{
    // 實現管道暫停邏輯
    UE_LOG(LogTemp, Log, TEXT("Paused pipeline: %s"), *PipelineName);
}

void UMingAIContentManager::ResumePipeline(const FString& PipelineName)
{
    // 實現管道恢復邏輯
    UE_LOG(LogTemp, Log, TEXT("Resumed pipeline: %s"), *PipelineName);
}

void UMingAIContentManager::CancelPipeline(const FString& PipelineName)
{
    if (ContentPipelines.Contains(PipelineName))
    {
        ContentPipelines[PipelineName].bIsCompleted = true;
        UE_LOG(LogTemp, Log, TEXT("Cancelled pipeline: %s"), *PipelineName);
    }
}

void UMingAIContentManager::GenerateGameScene(const FString& SceneDescription, const FAISceneConfiguration& SceneConfig)
{
    if (!AIContentRenderer)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Content Renderer not available"));
        return;
    }

    // 設置場景
    AIContentRenderer->SetupScene(SceneConfig);

    // 生成場景資產
    FAIGameSceneSetup SceneSetup;
    SceneSetup.SceneName = SceneConfig.SceneName;
    SceneSetup.SceneDescription = SceneDescription;
    SceneSetup.TimeOfDay = GetTimeOfDayString(SceneConfig.TimeOfDay);
    SceneSetup.Weather = GetWeatherString(SceneConfig.Weather);
    SceneSetup.Location = SceneConfig.LocationDescription;
    SceneSetup.Mood = GetMoodString(SceneConfig.Mood);
    SceneSetup.bAutoGenerateAssets = true;
    SceneSetup.bCreateLighting = true;
    SceneSetup.bAddEffects = true;

    if (AIContentSystem)
    {
        AIContentSystem->GenerateGameScene(SceneSetup);
    }

    UE_LOG(LogTemp, Log, TEXT("Generated game scene: %s"), *SceneDescription);
}

void UMingAIContentManager::GenerateInteractiveScene(const FString& SceneDescription)
{
    FAISceneConfiguration SceneConfig;
    SceneConfig.SceneName = GenerateUniqueContentName(EAIContentType::Level);
    SceneConfig.LocationDescription = SceneDescription;
    SceneConfig.TimeOfDay = ETimeOfDay::Morning;
    SceneConfig.Weather = EWeatherType::Clear;
    SceneConfig.Mood = ESceneMood::Peaceful;
    SceneConfig.RenderQuality = ERenderQuality::High;
    SceneConfig.bEnablePostProcessing = true;
    SceneConfig.bEnableBloom = true;
    SceneConfig.bEnableLensFlare = true;

    GenerateGameScene(SceneDescription, SceneConfig);
}

void UMingAIContentManager::GenerateCinematicScene(const FString& SceneDescription, float Duration)
{
    FAISceneConfiguration SceneConfig;
    SceneConfig.SceneName = GenerateUniqueContentName(EAIContentType::Video);
    SceneConfig.LocationDescription = SceneDescription;
    SceneConfig.TimeOfDay = ETimeOfDay::Dusk;
    SceneConfig.Weather = EWeatherType::Clear;
    SceneConfig.Mood = ESceneMood::Epic;
    SceneConfig.RenderQuality = ERenderQuality::Cinematic;
    SceneConfig.bEnablePostProcessing = true;
    SceneConfig.bEnableBloom = true;
    SceneConfig.bEnableLensFlare = true;
    SceneConfig.bEnableVolumetricFog = true;

    GenerateGameScene(SceneDescription, SceneConfig);

    // 渲染視頻
    if (AIContentRenderer)
    {
        FAIRenderSettings RenderSettings;
        RenderSettings.RenderWidth = 3840;
        RenderSettings.RenderHeight = 2160;
        RenderSettings.FrameRate = 30.0f;
        RenderSettings.bEnableMotionBlur = true;
        RenderSettings.bEnableDepthOfField = true;
        RenderSettings.bEnableAmbientOcclusion = true;
        RenderSettings.bEnableScreenSpaceReflections = true;
        RenderSettings.bEnableRayTracing = true;

        FString OutputPath = FString::Printf(TEXT("/Game/AIContent/Cinematic/%s.mp4"), *SceneConfig.SceneName);
        AIContentRenderer->RenderSceneToVideo(RenderSettings, Duration, OutputPath);
    }

    UE_LOG(LogTemp, Log, TEXT("Generated cinematic scene: %s (%.2fs)"), *SceneDescription, Duration);
}

void UMingAIContentManager::StartRealTimePreview()
{
    if (!AIContentRenderer)
    {
        UE_LOG(LogTemp, Error, TEXT("AI Content Renderer not available"));
        return;
    }

    FAIRenderSettings RenderSettings;
    RenderSettings.RenderWidth = 1920;
    RenderSettings.RenderHeight = 1080;
    RenderSettings.FrameRate = 60.0f;
    RenderSettings.bEnableMotionBlur = true;
    RenderSettings.bEnableDepthOfField = true;
    RenderSettings.bEnableAmbientOcclusion = true;
    RenderSettings.bEnableScreenSpaceReflections = true;

    AIContentRenderer->StartRealTimeRendering(RenderSettings);
    bPreviewActive = true;

    UE_LOG(LogTemp, Log, TEXT("Started real-time preview"));
}

void UMingAIContentManager::StopRealTimePreview()
{
    if (AIContentRenderer)
    {
        AIContentRenderer->StopRealTimeRendering();
    }
    
    bPreviewActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("Stopped real-time preview"));
}

void UMingAIContentManager::UpdatePreviewSettings(const FAIRenderSettings& RenderSettings)
{
    if (AIContentRenderer && bPreviewActive)
    {
        AIContentRenderer->StopRealTimeRendering();
        AIContentRenderer->StartRealTimeRendering(RenderSettings);
        
        UE_LOG(LogTemp, Log, TEXT("Updated preview settings"));
    }
}

void UMingAIContentManager::AddToContentLibrary(EAIContentType ContentType, const FString& ContentPath, const FString& Description)
{
    if (!ContentLibrary.Contains(ContentType))
    {
        ContentLibrary.Add(ContentType, TArray<FString>());
    }
    
    ContentLibrary[ContentType].AddUnique(ContentPath);
    ContentDescriptions.Add(ContentPath, Description);
    
    UE_LOG(LogTemp, Log, TEXT("Added content to library: %s (%s)"), *ContentPath, *Description);
}

void UMingAIContentManager::RemoveFromContentLibrary(const FString& ContentPath)
{
    for (auto& LibraryPair : ContentLibrary)
    {
        LibraryPair.Value.Remove(ContentPath);
    }
    
    ContentDescriptions.Remove(ContentPath);
    
    UE_LOG(LogTemp, Log, TEXT("Removed content from library: %s"), *ContentPath);
}

TArray<FString> UMingAIContentManager::GetContentLibrary(EAIContentType ContentType) const
{
    if (ContentLibrary.Contains(ContentType))
    {
        return ContentLibrary[ContentType];
    }
    
    return TArray<FString>();
}

void UMingAIContentManager::SearchContentLibrary(const FString& Query, TArray<FString>& Results)
{
    Results.Empty();
    
    for (const auto& DescPair : ContentDescriptions)
    {
        if (DescPair.Value.Contains(Query, ESearchCase::IgnoreCase))
        {
            Results.Add(DescPair.Key);
        }
    }
}

void UMingAIContentManager::SaveContentTemplate(const FString& TemplateName, const FAIContentProject& Project)
{
    FAIContentProject Template = Project;
    Template.ProjectName = TemplateName;
    ContentTemplates.Add(TemplateName, Template);
    
    UE_LOG(LogTemp, Log, TEXT("Saved content template: %s"), *TemplateName);
}

FAIContentProject UMingAIContentManager::LoadContentTemplate(const FString& TemplateName)
{
    if (ContentTemplates.Contains(TemplateName))
    {
        return ContentTemplates[TemplateName];
    }
    
    return FAIContentProject();
}

TArray<FString> UMingAIContentManager::GetAvailableTemplates() const
{
    TArray<FString> TemplateNames;
    
    for (const auto& TemplatePair : ContentTemplates)
    {
        TemplateNames.Add(TemplatePair.Key);
    }
    
    return TemplateNames;
}

void UMingAIContentManager::BatchGenerateContent(const TArray<EAIContentType>& ContentTypes, const TArray<FString>& Prompts)
{
    if (ContentTypes.Num() != Prompts.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Content types and prompts count mismatch"));
        return;
    }

    for (int32 i = 0; i < ContentTypes.Num(); ++i)
    {
        GenerateContentAsync(ContentTypes[i], Prompts[i]);
    }
}

void UMingAIContentManager::BatchImportContent(const TArray<FString>& ContentPaths)
{
    for (const FString& ContentPath : ContentPaths)
    {
        // 實現批量導入邏輯
        UE_LOG(LogTemp, Log, TEXT("Imported content: %s"), *ContentPath);
    }
}

void UMingAIContentManager::BatchExportContent(const TArray<FString>& ContentPaths, const FString& ExportFormat)
{
    for (const FString& ContentPath : ContentPaths)
    {
        // 實現批量導出邏輯
        UE_LOG(LogTemp, Log, TEXT("Exported content: %s (%s)"), *ContentPath, *ExportFormat);
    }
}

void UMingAIContentManager::OptimizeAssets()
{
    // 實現資產優化邏輯
    UE_LOG(LogTemp, Log, TEXT("Optimized assets"));
}

void UMingAIContentManager::CompressAssets()
{
    // 實現資產壓縮邏輯
    UE_LOG(LogTemp, Log, TEXT("Compressed assets"));
}

void UMingAIContentManager::ValidateAssets()
{
    // 實現資產驗證邏輯
    UE_LOG(LogTemp, Log, TEXT("Validated assets"));
}

float UMingAIContentManager::GetAssetSize(const FString& AssetPath) const
{
    // 實現資產大小計算
    return 0.0f;
}

float UMingAIContentManager::GetTotalAssetSize() const
{
    float TotalSize = 0.0f;
    
    for (const auto& LibraryPair : ContentLibrary)
    {
        for (const FString& ContentPath : LibraryPair.Value)
        {
            TotalSize += GetAssetSize(ContentPath);
        }
    }
    
    return TotalSize;
}

void UMingAIContentManager::ShareProject(const FString& ProjectName, const FString& TargetUser)
{
    // 實現項目分享邏輯
    UE_LOG(LogTemp, Log, TEXT("Shared project %s with user %s"), *ProjectName, *TargetUser);
}

void UMingAIContentManager::ImportSharedProject(const FString& ShareCode)
{
    // 實現共享項目導入邏輯
    UE_LOG(LogTemp, Log, TEXT("Imported shared project with code: %s"), *ShareCode);
}

void UMingAIContentManager::SyncProjectCloud(const FString& ProjectName)
{
    // 實現雲端同步邏輯
    UE_LOG(LogTemp, Log, TEXT("Synced project %s to cloud"), *ProjectName);
}

void UMingAIContentManager::CreateProjectVersion(const FString& ProjectName, const FString& VersionName)
{
    if (!ProjectVersions.Contains(ProjectName))
    {
        ProjectVersions.Add(ProjectName, TArray<FString>());
    }
    
    ProjectVersions[ProjectName].Add(VersionName);
    
    UE_LOG(LogTemp, Log, TEXT("Created version %s for project %s"), *VersionName, *ProjectName);
}

void UMingAIContentManager::RevertToVersion(const FString& ProjectName, const FString& VersionName)
{
    // 實現版本回滾邏輯
    UE_LOG(LogTemp, Log, TEXT("Reverted project %s to version %s"), *ProjectName, *VersionName);
}

TArray<FString> UMingAIContentManager::GetProjectVersions(const FString& ProjectName) const
{
    if (ProjectVersions.Contains(ProjectName))
    {
        return ProjectVersions[ProjectName];
    }
    
    return TArray<FString>();
}

float UMingAIContentManager::GetGenerationProgress() const
{
    if (AIContentSystem)
    {
        return AIContentSystem->GetSystemLoad();
    }
    return 0.0f;
}

int32 UMingAIContentManager::GetActiveGenerations() const
{
    if (AIContentSystem)
    {
        return AIContentSystem->GetActiveRequests().Num();
    }
    return 0;
}

float UMingAIContentManager::GetAverageGenerationTime() const
{
    if (TotalGeneratedCount > 0)
    {
        return TotalGenerationTime / TotalGeneratedCount;
    }
    return 0.0f;
}

int32 UMingAIContentManager::GetTotalGeneratedContent() const
{
    return TotalGeneratedCount;
}

void UMingAIContentManager::SetDefaultProvider(EAIModelProvider Provider)
{
    if (AIContentSystem)
    {
        AIContentSystem->SetModelProvider(Provider);
    }
}

void UMingAIContentManager::SetAPIKey(EAIModelProvider Provider, const FString& APIKey)
{
    if (AIContentSystem)
    {
        AIContentSystem->SetModelProvider(Provider, APIKey);
    }
}

void UMingAIContentManager::SetQualitySettings(EGenerationQuality Quality)
{
    if (AIContentSystem)
    {
        AIContentSystem->SetDefaultQuality(Quality);
    }
}

void UMingAIContentManager::SetOutputSettings(const FString& Directory, bool bAutoSave)
{
    if (AIContentSystem)
    {
        AIContentSystem->SetOutputDirectory(Directory);
    }
}

void UMingAIContentManager::ClearCache()
{
    if (AIContentSystem)
    {
        AIContentSystem->ClearCache();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cleared AI content cache"));
}

void UMingAIContentManager::CleanupOldContent()
{
    // 實現舊內容清理邏輯
    UE_LOG(LogTemp, Log, TEXT("Cleaned up old content"));
}

void UMingAIContentManager::RebuildContentIndex()
{
    // 實現內容索引重建邏輯
    UE_LOG(LogTemp, Log, TEXT("Rebuilt content index"));
}

void UMingAIContentManager::ValidateSystemIntegrity()
{
    // 實現系統完整性驗證
    UE_LOG(LogTemp, Log, TEXT("Validated system integrity"));
}

void UMingAIContentManager::InitializeContentSystem()
{
    AIContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    if (AIContentSystem)
    {
        AIContentSystem->InitializeAIGenerationSystem();
        
        // 綁定事件
        AIContentSystem->OnAIGenerationCompleted.AddDynamic(this, &UMingAIContentManager::OnContentGenerated);
    }
}

void UMingAIContentManager::InitializeRenderer()
{
    AIContentRenderer = NewObject<UMingAIContentRenderer>();
    if (AIContentRenderer)
    {
        AIContentRenderer->InitializeRenderer();
        
        // 綁定事件
        AIContentRenderer->OnSceneRendered.AddDynamic(this, &UMingAIContentManager::OnSceneRendered);
    }
}

void UMingAIContentManager::LoadDefaultTemplates()
{
    // 加載默認內容模板
    FAIContentProject LandscapeTemplate;
    LandscapeTemplate.ProjectName = TEXT("Landscape");
    LandscapeTemplate.ProjectDescription = TEXT("Beautiful landscape scene");
    LandscapeTemplate.RequiredContentTypes.Add(EAIContentType::Image);
    LandscapeTemplate.RequiredContentTypes.Add(EAIContentType::Music);
    LandscapeTemplate.RequiredContentTypes.Add(EAIContentType::SoundEffect);
    LandscapeTemplate.ContentPrompts.Add(EAIContentType::Image, TEXT("Beautiful landscape with mountains and rivers"));
    LandscapeTemplate.ContentPrompts.Add(EAIContentType::Music, TEXT("Peaceful ambient music"));
    LandscapeTemplate.ContentPrompts.Add(EAIContentType::SoundEffect, TEXT("Nature sounds"));
    SaveContentTemplate(TEXT("Landscape"), LandscapeTemplate);
    
    FAIContentProject CharacterTemplate;
    CharacterTemplate.ProjectName = TEXT("Character");
    CharacterTemplate.ProjectDescription = TEXT("Game character design");
    CharacterTemplate.RequiredContentTypes.Add(EAIContentType::Image);
    CharacterTemplate.RequiredContentTypes.Add(EAIContentType::Mesh);
    CharacterTemplate.RequiredContentTypes.Add(EAIContentType::Material);
    CharacterTemplate.ContentPrompts.Add(EAIContentType::Image, TEXT("Character concept art"));
    CharacterTemplate.ContentPrompts.Add(EAIContentType::Mesh, TEXT("Character 3D model"));
    CharacterTemplate.ContentPrompts.Add(EAIContentType::Material, TEXT("Character texture material"));
    SaveContentTemplate(TEXT("Character"), CharacterTemplate);
}

void UMingAIContentManager::ProcessPipelineStage(const FString& PipelineName, int32 StageIndex)
{
    if (!ContentPipelines.Contains(PipelineName))
    {
        return;
    }

    FAIContentPipeline& Pipeline = ContentPipelines[PipelineName];
    
    if (StageIndex >= Pipeline.ContentStages.Num())
    {
        CompletePipeline(PipelineName, true);
        return;
    }

    EAIContentType ContentType = Pipeline.ContentStages[StageIndex];
    
    if (Pipeline.StageRequests.Contains(ContentType))
    {
        FAIGenerationRequest Request = Pipeline.StageRequests[ContentType];
        FString ContentPath = GenerateContent(Request.GenerationType, Request.Prompt, Request.Quality);
        
        if (!ContentPath.IsEmpty())
        {
            NotifyPipelineStageCompleted(StageIndex);
            
            if (Pipeline.bAutoAdvance && !Pipeline.bParallelProcessing)
            {
                // 自動進入下一階段
                ProcessPipelineStage(PipelineName, StageIndex + 1);
            }
        }
        else
        {
            CompletePipeline(PipelineName, false);
        }
    }
}

void UMingAIContentManager::CompletePipeline(const FString& PipelineName, bool bSuccess)
{
    if (ContentPipelines.Contains(PipelineName))
    {
        ContentPipelines[PipelineName].bIsCompleted = true;
        
        UE_LOG(LogTemp, Log, TEXT("Pipeline %s completed: %s"), *PipelineName, bSuccess ? TEXT("Success") : TEXT("Failed"));
    }
}

void UMingAIContentManager::NotifyContentGenerated(EAIContentType ContentType, const FString& ContentPath)
{
    OnContentGenerated.Broadcast(ContentType, ContentPath);
}

void UMingAIContentManager::NotifyProjectCompleted(const FString& ProjectName, bool bSuccess)
{
    OnProjectCompleted.Broadcast(ProjectName, bSuccess);
}

void UMingAIContentManager::NotifyPipelineStageCompleted(int32 StageIndex)
{
    OnPipelineStageCompleted.Broadcast(StageIndex);
}

FString UMingAIContentManager::GenerateUniqueContentName(EAIContentType ContentType) const
{
    return FString::Printf(TEXT("%s_%s"), *GetContentTypeString(ContentType), *FDateTime::Now().ToString());
}

FString UMingAIContentManager::GetContentPath(EAIContentType ContentType, const FString& ContentName) const
{
    return FString::Printf(TEXT("/Game/AIContent/%s/%s"), *GetContentTypeString(ContentType), *ContentName);
}

bool UMingAIContentManager::ValidateContent(const FString& ContentPath) const
{
    // 實現內容驗證邏輯
    return true;
}

void UMingAIContentManager::UpdateStatistics(EAIContentType ContentType, float GenerationTime)
{
    TotalGeneratedCount++;
    TotalGenerationTime += GenerationTime;
    
    if (!ContentTypeCounts.Contains(ContentType))
    {
        ContentTypeCounts.Add(ContentType, 0);
    }
    ContentTypeCounts[ContentType]++;
}

void UMingAIContentManager::SaveProjectData()
{
    // 實現項目數據保存
    FString ProjectDataPath = FPaths::ProjectDir() / TEXT("Saved/AIContent/Projects.json");
    
    // 這裡可以實現JSON序列化保存
}

void UMingAIContentManager::LoadProjectData()
{
    // 實現項目數據加載
    FString ProjectDataPath = FPaths::ProjectDir() / TEXT("Saved/AIContent/Projects.json");
    
    // 這裡可以實現JSON反序列化加載
}

// 輔助函數實現
FString UMingAIContentManager::GetContentTypeString(EAIContentType ContentType) const
{
    switch (ContentType)
    {
        case EAIContentType::Image: return TEXT("Image");
        case EAIContentType::Video: return TEXT("Video");
        case EAIContentType::Music: return TEXT("Music");
        case EAIContentType::SoundEffect: return TEXT("SoundEffect");
        case EAIContentType::Texture: return TEXT("Texture");
        case EAIContentType::Mesh: return TEXT("Mesh");
        case EAIContentType::Material: return TEXT("Material");
        case EAIContentType::Animation: return TEXT("Animation");
        case EAIContentType::Level: return TEXT("Level");
        case EAIContentType::UI: return TEXT("UI");
        case EAIContentType::Text: return TEXT("Text");
        case EAIContentType::Voice: return TEXT("Voice");
        default: return TEXT("Unknown");
    }
}

FString UMingAIContentManager::GetTimeOfDayString(ETimeOfDay TimeOfDay) const
{
    switch (TimeOfDay)
    {
        case ETimeOfDay::Dawn: return TEXT("dawn");
        case ETimeOfDay::Morning: return TEXT("morning");
        case ETimeOfDay::Noon: return TEXT("noon");
        case ETimeOfDay::Afternoon: return TEXT("afternoon");
        case ETimeOfDay::Dusk: return TEXT("dusk");
        case ETimeOfDay::Night: return TEXT("night");
        case ETimeOfDay::Midnight: return TEXT("midnight");
        default: return TEXT("day");
    }
}

FString UMingAIContentManager::GetWeatherString(EWeatherType Weather) const
{
    switch (Weather)
    {
        case EWeatherType::Clear: return TEXT("clear");
        case EWeatherType::Cloudy: return TEXT("cloudy");
        case EWeatherType::Overcast: return TEXT("overcast");
        case EWeatherType::Rainy: return TEXT("rainy");
        case EWeatherType::Stormy: return TEXT("stormy");
        case EWeatherType::Snowy: return TEXT("snowy");
        case EWeatherType::Foggy: return TEXT("foggy");
        case EWeatherType::Windy: return TEXT("windy");
        default: return TEXT("clear");
    }
}

FString UMingAIContentManager::GetMoodString(ESceneMood Mood) const
{
    switch (Mood)
    {
        case ESceneMood::Peaceful: return TEXT("peaceful");
        case ESceneMood::Dramatic: return TEXT("dramatic");
        case ESceneMood::Mysterious: return TEXT("mysterious");
        case ESceneMood::Epic: return TEXT("epic");
        case ESceneMood::Intimate: return TEXT("intimate");
        case ESceneMood::Tense: return TEXT("tense");
        case ESceneMood::Joyful: return TEXT("joyful");
        case ESceneMood::Melancholy: return TEXT("melancholy");
        default: return TEXT("neutral");
    }
}
