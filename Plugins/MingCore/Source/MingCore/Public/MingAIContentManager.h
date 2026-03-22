#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIGeneratedContentSystem.h"
#include "MingAIContentRenderer.h"
#include "MingAIContentManager.generated.h"

class UWorld;
class AGameModeBase;
class APlayerController;

UENUM(BlueprintType)
enum class EAIContentType : uint8
{
    Image,          // ?ñÂ?
    Video,          // ÂΩ±Á?
    Music,          // ?≥Ê?
    SoundEffect,    // ?≥Ê?
    Texture,        // Ë≤ºÂ?
    Mesh,           // Ê®°Â?
    Material,       // ?êË≥™
    Animation,      // ?ïÁï´
    Level,          // ?úÂç°
    UI,             // UI
    Text,           // ?áÊú¨
    Voice           // Ë™ûÈü≥
};

USTRUCT(BlueprintType)
struct FAIContentProject
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    FString ProjectName;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    FString ProjectDescription;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    TArray<EAIContentType> RequiredContentTypes;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    TMap<EAIContentType, FString> ContentPrompts;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    FString TargetPlatform;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    EGenerationQuality DefaultQuality;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    bool bAutoGenerate;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    bool bRealTimePreview;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Project")
    FString OutputDirectory;

    FAIContentProject()
    {
        ProjectName = TEXT("");
        ProjectDescription = TEXT("");
        TargetPlatform = TEXT("PC");
        DefaultQuality = EGenerationQuality::Standard;
        bAutoGenerate = true;
        bRealTimePreview = true;
        OutputDirectory = TEXT("/Game/AIContent/");
    }
};

USTRUCT(BlueprintType)
struct FAIContentPipeline
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    FString PipelineName;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    TArray<EAIContentType> ContentStages;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    TMap<EAIContentType, FAIGenerationRequest> StageRequests;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    bool bParallelProcessing;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    bool bAutoAdvance;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    int32 CurrentStage;

    UPROPERTY(BlueprintReadWrite, Category = "AI Content Pipeline")
    bool bIsCompleted;

    FAIContentPipeline()
    {
        PipelineName = TEXT("");
        bParallelProcessing = false;
        bAutoAdvance = true;
        CurrentStage = 0;
        bIsCompleted = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContentGenerated, EAIContentType, ContentType, const FString&, ContentPath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectCompleted, const FString&, ProjectName, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPipelineStageCompleted, int32, StageIndex);

/**
 * AI ?ßÂÆπÁÆ°Á??? * Áµ±‰?ÁÆ°Á?AI?ßÂÆπ?üÊ??åÊ∏≤?ìÁ?ÂÆåÊï¥Á≥ªÁµ±
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAIContentManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIContentManager();

    // ?ùÂ??ñÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void InitializeAIContentManager();

    // ?ÖÁõÆÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FString CreateAIContentProject(const FAIContentProject& Project);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    bool LoadAIContentProject(const FString& ProjectName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SaveAIContentProject(const FString& ProjectName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void DeleteAIContentProject(const FString& ProjectName);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FAIContentProject> GetAllProjects() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    FAIContentProject GetCurrentProject() const { return CurrentProject; }

    // ?ßÂÆπ?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FString GenerateContent(EAIContentType ContentType, const FString& Prompt, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    TArray<FString> GenerateProjectContent(const FString& ProjectName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateContentAsync(EAIContentType ContentType, const FString& Prompt);

    // ?ßÂÆπÁÆ°È?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FString CreateContentPipeline(const FAIContentPipeline& Pipeline);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ExecutePipeline(const FString& PipelineName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void PausePipeline(const FString& PipelineName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ResumePipeline(const FString& PipelineName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void CancelPipeline(const FString& PipelineName);

    // ?¥ÊôØ?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateGameScene(const FString& SceneDescription, const FAISceneConfiguration& SceneConfig);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateInteractiveScene(const FString& SceneDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateCinematicScene(const FString& SceneDescription, float Duration);

    // ÂØ¶Ê??êË¶Ω
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void StartRealTimePreview();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void StopRealTimePreview();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void UpdatePreviewSettings(const FAIRenderSettings& RenderSettings);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    bool IsPreviewActive() const { return bPreviewActive; }

    // ?ßÂÆπÂ∫´ÁÆ°??    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void AddToContentLibrary(EAIContentType ContentType, const FString& ContentPath, const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void RemoveFromContentLibrary(const FString& ContentPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    TArray<FString> GetContentLibrary(EAIContentType ContentType) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SearchContentLibrary(const FString& Query, TArray<FString>& Results);

    // Ê®°Êùø?åÈ?Ë®?    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SaveContentTemplate(const FString& TemplateName, const FAIContentProject& Project);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FAIContentProject LoadContentTemplate(const FString& TemplateName);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FString> GetAvailableTemplates() const;

    // ?πÈ??ç‰?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void BatchGenerateContent(const TArray<EAIContentType>& ContentTypes, const TArray<FString>& Prompts);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void BatchImportContent(const TArray<FString>& ContentPaths);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void BatchExportContent(const TArray<FString>& ContentPaths, const FString& ExportFormat);

    // Ë≥áÁî¢ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void OptimizeAssets();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void CompressAssets();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ValidateAssets();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    float GetAssetSize(const FString& AssetPath) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    float GetTotalAssetSize() const;

    // ?î‰??üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ShareProject(const FString& ProjectName, const FString& TargetUser);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ImportSharedProject(const FString& ShareCode);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SyncProjectCloud(const FString& ProjectName);

    // ?àÊú¨?ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void CreateProjectVersion(const FString& ProjectName, const FString& VersionName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void RevertToVersion(const FString& ProjectName, const FString& VersionName);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FString> GetProjectVersions(const FString& ProjectName) const;

    // ?ßËÉΩ??éß
    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    float GetGenerationProgress() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    int32 GetActiveGenerations() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    float GetAverageGenerationTime() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    int32 GetTotalGeneratedContent() const;

    // ?çÁΩÆÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetDefaultProvider(EAIModelProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetAPIKey(EAIModelProvider Provider, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetQualitySettings(EGenerationQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetOutputSettings(const FString& Directory, bool bAutoSave);

    // Á≥ªÁµ±Á∂≠Ë≠∑
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void CleanupOldContent();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void RebuildContentIndex();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ValidateSystemIntegrity();

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnContentGenerated OnContentGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnProjectCompleted OnProjectCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnPipelineStageCompleted OnPipelineStageCompleted;

protected:
    // Á≥ªÁµ±ÁµÑ‰ª∂
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    UPROPERTY()
    TObjectPtr<UMingAIContentRenderer> AIContentRenderer;

    // ?ÖÁõÆÁÆ°Á?
    UPROPERTY()
    TMap<FString, FAIContentProject> Projects;

    UPROPERTY()
    FAIContentProject CurrentProject;

    // ?ßÂÆπÁÆ°È?
    UPROPERTY()
    TMap<FString, FAIContentPipeline> ContentPipelines;

    // ?ßÂÆπÂ∫?- Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<EAIContentType, TArray<FString>> ContentLibrary;

    UPROPERTY()
    TMap<FString, FString> ContentDescriptions;

    // Ê®°Êùø
    UPROPERTY()
    TMap<FString, FAIContentProject> ContentTemplates;

    // ?àÊú¨?ßÂà∂ - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<FString, TArray<FString>> ProjectVersions;

    // ?Ä??    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bPreviewActive;

    UPROPERTY()
    FString CurrentPipeline;

    UPROPERTY()
    TArray<FString> ActiveGenerations;

    // Áµ±Ë?
    UPROPERTY()
    int32 TotalGeneratedCount;

    UPROPERTY()
    float TotalGenerationTime;

    UPROPERTY()
    TMap<EAIContentType, int32> ContentTypeCounts;

    // ?ßÈÉ®?ΩÊï∏
    void InitializeContentSystem();
    void InitializeRenderer();
    void LoadDefaultTemplates();
    void ProcessPipelineStage(const FString& PipelineName, int32 StageIndex);
    void CompletePipeline(const FString& PipelineName, bool bSuccess);
    void NotifyContentGenerated(EAIContentType ContentType, const FString& ContentPath);
    void NotifyProjectCompleted(const FString& ProjectName, bool bSuccess);
    void NotifyPipelineStageCompleted(int32 StageIndex);

private:
    // ËºîÂä©?ΩÊï∏
    FString GenerateUniqueContentName(EAIContentType ContentType) const;
    FString GetContentPath(EAIContentType ContentType, const FString& ContentName) const;
    bool ValidateContent(const FString& ContentPath) const;
    void UpdateStatistics(EAIContentType ContentType, float GenerationTime);
    void SaveProjectData();
    void LoadProjectData();
};
