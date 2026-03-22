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
    Image,          // 圖像
    Video,          // 影片
    Music,          // 音樂
    SoundEffect,    // 音效
    Texture,        // 貼圖
    Mesh,           // 模型
    Material,       // 材質
    Animation,      // 動畫
    Level,          // 關卡
    UI,             // UI
    Text,           // 文本
    Voice           // 語音
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
 * AI 內容管理器
 * 統一管理AI內容生成和渲染的完整系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAIContentManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAIContentManager();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void InitializeAIContentManager();

    // 項目管理
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

    // 內容生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FString GenerateContent(EAIContentType ContentType, const FString& Prompt, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    TArray<FString> GenerateProjectContent(const FString& ProjectName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateContentAsync(EAIContentType ContentType, const FString& Prompt);

    // 內容管道
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

    // 場景生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateGameScene(const FString& SceneDescription, const FAISceneConfiguration& SceneConfig);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateInteractiveScene(const FString& SceneDescription);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void GenerateCinematicScene(const FString& SceneDescription, float Duration);

    // 實時預覽
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void StartRealTimePreview();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void StopRealTimePreview();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void UpdatePreviewSettings(const FAIRenderSettings& RenderSettings);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    bool IsPreviewActive() const { return bPreviewActive; }

    // 內容庫管理
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void AddToContentLibrary(EAIContentType ContentType, const FString& ContentPath, const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void RemoveFromContentLibrary(const FString& ContentPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    TArray<FString> GetContentLibrary(EAIContentType ContentType) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SearchContentLibrary(const FString& Query, TArray<FString>& Results);

    // 模板和預設
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SaveContentTemplate(const FString& TemplateName, const FAIContentProject& Project);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    FAIContentProject LoadContentTemplate(const FString& TemplateName);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FString> GetAvailableTemplates() const;

    // 批量操作
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void BatchGenerateContent(const TArray<EAIContentType>& ContentTypes, const TArray<FString>& Prompts);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void BatchImportContent(const TArray<FString>& ContentPaths);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void BatchExportContent(const TArray<FString>& ContentPaths, const FString& ExportFormat);

    // 資產管理
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

    // 協作功能
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ShareProject(const FString& ProjectName, const FString& TargetUser);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ImportSharedProject(const FString& ShareCode);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SyncProjectCloud(const FString& ProjectName);

    // 版本控制
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void CreateProjectVersion(const FString& ProjectName, const FString& VersionName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void RevertToVersion(const FString& ProjectName, const FString& VersionName);

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    TArray<FString> GetProjectVersions(const FString& ProjectName) const;

    // 性能監控
    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    float GetGenerationProgress() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    int32 GetActiveGenerations() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    float GetAverageGenerationTime() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Manager")
    int32 GetTotalGeneratedContent() const;

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetDefaultProvider(EAIModelProvider Provider);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetAPIKey(EAIModelProvider Provider, const FString& APIKey);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetQualitySettings(EGenerationQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void SetOutputSettings(const FString& Directory, bool bAutoSave);

    // 系統維護
    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void CleanupOldContent();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void RebuildContentIndex();

    UFUNCTION(BlueprintCallable, Category = "AI Content Manager")
    void ValidateSystemIntegrity();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnContentGenerated OnContentGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnProjectCompleted OnProjectCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Content Events")
    FOnPipelineStageCompleted OnPipelineStageCompleted;

protected:
    // 系統組件
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    UPROPERTY()
    TObjectPtr<UMingAIContentRenderer> AIContentRenderer;

    // 項目管理
    UPROPERTY()
    TMap<FString, FAIContentProject> Projects;

    UPROPERTY()
    FAIContentProject CurrentProject;

    // 內容管道
    UPROPERTY()
    TMap<FString, FAIContentPipeline> ContentPipelines;

    // 內容庫 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<EAIContentType, TArray<FString>> ContentLibrary;

    UPROPERTY()
    TMap<FString, FString> ContentDescriptions;

    // 模板
    UPROPERTY()
    TMap<FString, FAIContentProject> ContentTemplates;

    // 版本控制 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<FString>> ProjectVersions;

    // 狀態
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bPreviewActive;

    UPROPERTY()
    FString CurrentPipeline;

    UPROPERTY()
    TArray<FString> ActiveGenerations;

    // 統計
    UPROPERTY()
    int32 TotalGeneratedCount;

    UPROPERTY()
    float TotalGenerationTime;

    UPROPERTY()
    TMap<EAIContentType, int32> ContentTypeCounts;

    // 內部函數
    void InitializeContentSystem();
    void InitializeRenderer();
    void LoadDefaultTemplates();
    void ProcessPipelineStage(const FString& PipelineName, int32 StageIndex);
    void CompletePipeline(const FString& PipelineName, bool bSuccess);
    void NotifyContentGenerated(EAIContentType ContentType, const FString& ContentPath);
    void NotifyProjectCompleted(const FString& ProjectName, bool bSuccess);
    void NotifyPipelineStageCompleted(int32 StageIndex);

private:
    // 輔助函數
    FString GenerateUniqueContentName(EAIContentType ContentType) const;
    FString GetContentPath(EAIContentType ContentType, const FString& ContentName) const;
    bool ValidateContent(const FString& ContentPath) const;
    void UpdateStatistics(EAIContentType ContentType, float GenerationTime);
    void SaveProjectData();
    void LoadProjectData();
};
