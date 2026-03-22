#include "MingRTSBatchGenerationSystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Engine/GameInstance.h"

UMingRTSBatchGenerationSystem::UMingRTSBatchGenerationSystem()
{
    CurrentWorld = nullptr;
    AIContentSystem = nullptr;
    ContentRenderer = nullptr;
    ContentManager = nullptr;
    
    MaxConcurrentBatches = 3;
    MaxItemsPerBatch = 50;
    BatchUpdateInterval = 1.0f;
    bEnableAutoOptimization = true;
    bEnableCaching = true;
    bEnableParallelProcessing = true;
}

void UMingRTSBatchGenerationSystem::InitializeBatchGenerationSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("MingRTSBatchGenerationSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    
    // 获取AI内容系统引用
    // 注意：在實際實現中，這些系統應該從遊戲實例或管理器中獲取
    // 這裡只是示例代碼
    
    // 初始化批次模板
    InitializeBatchTemplates();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSBatchGenerationSystem initialized successfully"));
}

FString UMingRTSBatchGenerationSystem::StartBatchGeneration(const FAIBatchConfiguration& Configuration)
{
    // 验证批次配置
    if (!ValidateBatchConfiguration(Configuration))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid batch configuration"));
        return TEXT("");
    }
    
    // 检查批次限制
    if (!CheckBatchLimits(Configuration))
    {
        UE_LOG(LogTemp, Error, TEXT("Batch limits exceeded"));
        return TEXT("");
    }
    
    // 生成唯一批次ID
    FString BatchID = GenerateUniqueBatchID();
    
    // 存储批次配置
    BatchConfigurations.Add(BatchID, Configuration);
    
    // 初始化批次结果
    FAIBatchResult Result;
    Result.BatchID = BatchID;
    Result.BatchName = Configuration.BatchName;
    Result.Status = EBatchStatus::Pending;
    Result.TotalItems = Configuration.ItemCount;
    Result.StartTime = CurrentWorld->GetTimeSeconds();
    BatchResults.Add(BatchID, Result);
    
    // 设置批次状态
    BatchStatuses.Add(BatchID, EBatchStatus::Pending);
    BatchProgress.Add(BatchID, 0);
    BatchPriorities.Add(BatchID, 1); // 默认优先级
    
    UE_LOG(LogTemp, Log, TEXT("Started batch generation: %s (%s)"), *Configuration.BatchName, *BatchID);
    
    // 触发批次开始事件
    OnBatchStarted.Broadcast(BatchID);
    
    // 开始处理批次
    ProcessBatchGeneration(BatchID);
    
    return BatchID;
}

bool UMingRTSBatchGenerationSystem::PauseBatchGeneration(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    EBatchStatus* Status = BatchStatuses.Find(BatchID);
    if (Status && *Status == EBatchStatus::Running)
    {
        *Status = EBatchStatus::Paused;
        UE_LOG(LogTemp, Log, TEXT("Paused batch generation: %s"), *BatchID);
        return true;
    }
    
    return false;
}

bool UMingRTSBatchGenerationSystem::ResumeBatchGeneration(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    EBatchStatus* Status = BatchStatuses.Find(BatchID);
    if (Status && *Status == EBatchStatus::Paused)
    {
        *Status = EBatchStatus::Running;
        UE_LOG(LogTemp, Log, TEXT("Resumed batch generation: %s"), *BatchID);
        
        // 继续处理批次
        ProcessBatchGeneration(BatchID);
        return true;
    }
    
    return false;
}

bool UMingRTSBatchGenerationSystem::CancelBatchGeneration(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    EBatchStatus* Status = BatchStatuses.Find(BatchID);
    if (Status)
    {
        *Status = EBatchStatus::Cancelled;
        
        FAIBatchResult* Result = BatchResults.Find(BatchID);
        if (Result)
        {
            Result->Status = EBatchStatus::Cancelled;
            Result->EndTime = CurrentWorld->GetTimeSeconds();
            Result->TotalDuration = Result->EndTime - Result->StartTime;
        }
        
        UE_LOG(LogTemp, Log, TEXT("Cancelled batch generation: %s"), *BatchID);
        return true;
    }
    
    return false;
}

EBatchStatus UMingRTSBatchGenerationSystem::GetBatchStatus(const FString& BatchID) const
{
    if (!IsValidBatchID(BatchID))
    {
        return EBatchStatus::Custom;
    }
    
    const EBatchStatus* Status = BatchStatuses.Find(BatchID);
    return Status ? *Status : EBatchStatus::Custom;
}

int32 UMingRTSBatchGenerationSystem::GetBatchProgress(const FString& BatchID) const
{
    if (!IsValidBatchID(BatchID))
    {
        return 0;
    }
    
    const int32* Progress = BatchProgress.Find(BatchID);
    return Progress ? *Progress : 0;
}

FAIBatchResult UMingRTSBatchGenerationSystem::GetBatchResult(const FString& BatchID) const
{
    if (!IsValidBatchID(BatchID))
    {
        return FAIBatchResult();
    }
    
    const FAIBatchResult* Result = BatchResults.Find(BatchID);
    return Result ? *Result : FAIBatchResult();
}

TArray<FAIBatchResult> UMingRTSBatchGenerationSystem::GetAllBatches() const
{
    TArray<FAIBatchResult> AllBatches;
    
    for (const TPair<FString, FAIBatchResult>& Pair : BatchResults)
    {
        AllBatches.Add(Pair.Value);
    }
    
    return AllBatches;
}

bool UMingRTSBatchGenerationSystem::ExportBatch(const FString& BatchID, const FString& OutputPath)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    const FAIBatchResult* Result = BatchResults.Find(BatchID);
    if (!Result || Result->Status != EBatchStatus::Completed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot export incomplete batch: %s"), *BatchID);
        return false;
    }
    
    // 创建导出目录
    FString FullPath = FPaths::Combine(OutputPath, Result->BatchName);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    if (!PlatformFile.DirectoryExists(*FullPath))
    {
        PlatformFile.CreateDirectoryTree(*FullPath);
    }
    
    // 导出每个生成的资产
    for (const FAIGeneratedAsset& Asset : Result->GeneratedAssets)
    {
        FString AssetPath = FPaths::Combine(FullPath, Asset.AssetName);
        
        if (Asset.AssetType == EAIContentType::Image)
        {
            // 导出图像文件
            FFileHelper::SaveArrayToFile(Asset.AssetData, *AssetPath);
        }
        else if (Asset.AssetType == EAIContentType::Audio)
        {
            // 导出音频文件
            FFileHelper::SaveArrayToFile(Asset.AssetData, *AssetPath);
        }
        // 其他类型的导出...
    }
    
    // 导出批次报告
    FString ReportPath = FPaths::Combine(FullPath, TEXT("BatchReport.txt"));
    FString ReportContent = GenerateBatchReport(*Result);
    FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
    
    UE_LOG(LogTemp, Log, TEXT("Exported batch: %s to %s"), *BatchID, *FullPath);
    return true;
}

bool UMingRTSBatchGenerationSystem::ExportBatchAs(const FString& BatchID, EExportFormat Format)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    // 根据格式导出批次
    switch (Format)
    {
        case EExportFormat::UnrealPackage:
            return ExportBatchAsUnrealPackage(BatchID);
        case EExportFormat::Standalone:
            return ExportBatchAsStandalone(BatchID);
        case EExportFormat::WebPackage:
            return ExportBatchAsWebPackage(BatchID);
        default:
            return ExportBatch(BatchID, TEXT("/Game/GeneratedContent/Exports/"));
    }
}

FString UMingRTSBatchGenerationSystem::CreateBatchSharePackage(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return TEXT("");
    }
    
    const FAIBatchResult* Result = BatchResults.Find(BatchID);
    if (!Result || Result->Status != EBatchStatus::Completed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot share incomplete batch: %s"), *BatchID);
        return TEXT("");
    }
    
    // 生成分享ID
    FString ShareID = GenerateUniqueBatchID();
    
    // 创建分享包
    FString SharePath = FPaths::Combine(TEXT("/Game/GeneratedContent/Shares/"), ShareID);
    
    // 复制批次内容到分享目录
    ExportBatch(BatchID, SharePath);
    
    // 创建分享元数据
    FString MetadataPath = FPaths::Combine(SharePath, TEXT("ShareMetadata.json"));
    FString MetadataContent = GenerateShareMetadata(*Result, ShareID);
    FFileHelper::SaveStringToFile(MetadataContent, *MetadataPath);
    
    UE_LOG(LogTemp, Log, TEXT("Created share package: %s"), *ShareID);
    return ShareID;
}

bool UMingRTSBatchGenerationSystem::ShareBatchWithTeam(const FString& ShareID, const TArray<FString>& TeamMemberIDs)
{
    // 在实际实现中，这里应该与团队协作系统集成
    // 这里只是示例代码
    
    UE_LOG(LogTemp, Log, TEXT("Shared batch %s with %d team members"), *ShareID, TeamMemberIDs.Num());
    return true;
}

FAIBatchTemplate UMingRTSBatchGenerationSystem::GetBatchTemplate(const FString& TemplateName) const
{
    const FAIBatchTemplate* Template = BatchTemplates.Find(TemplateName);
    return Template ? *Template : FAIBatchTemplate();
}

TArray<FAIBatchTemplate> UMingRTSBatchGenerationSystem::GetAllBatchTemplates() const
{
    TArray<FAIBatchTemplate> AllTemplates;
    
    for (const TPair<FString, FAIBatchTemplate>& Pair : BatchTemplates)
    {
        AllTemplates.Add(Pair.Value);
    }
    
    return AllTemplates;
}

FAIBatchConfiguration UMingRTSBatchGenerationSystem::CreateConfigFromTemplate(const FAIBatchTemplate& Template) const
{
    return Template.DefaultConfiguration;
}

bool UMingRTSBatchGenerationSystem::RegisterConditionalBatch(const FAIBatchCondition& Condition, const FAIBatchConfiguration& Configuration)
{
    FString ConditionID = GenerateUniqueBatchID();
    
    ConditionalBatches.Add(ConditionID, Condition);
    
    // 存储配置以备条件触发时使用
    FString ConfigKey = FString::Printf(TEXT("Condition_%s"), *ConditionID);
    BatchConfigurations.Add(ConfigKey, Configuration);
    
    UE_LOG(LogTemp, Log, TEXT("Registered conditional batch: %s"), *ConditionID);
    return true;
}

bool UMingRTSBatchGenerationSystem::UnregisterConditionalBatch(const FString& ConditionID)
{
    if (ConditionalBatches.Remove(ConditionID) > 0)
    {
        FString ConfigKey = FString::Printf(TEXT("Condition_%s"), *ConditionID);
        BatchConfigurations.Remove(ConfigKey);
        
        UE_LOG(LogTemp, Log, TEXT("Unregistered conditional batch: %s"), *ConditionID);
        return true;
    }
    
    return false;
}

void UMingRTSBatchGenerationSystem::ProvideBatchFeedback(const FString& BatchID, const FString& Feedback)
{
    if (!IsValidBatchID(BatchID))
    {
        return;
    }
    
    FAIBatchResult* Result = BatchResults.Find(BatchID);
    if (Result)
    {
        Result->Warnings.Add(Feedback);
        
        // 如果启用了迭代优化，根据反馈优化结果
        if (bEnableAutoOptimization)
        {
            OptimizeBatchResults(BatchID);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Received feedback for batch %s: %s"), *BatchID, *Feedback);
    }
}

bool UMingRTSBatchGenerationSystem::RegenerateBatchItem(const FString& BatchID, const FString& AssetID)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    FAIBatchConfiguration* Config = BatchConfigurations.Find(BatchID);
    FAIBatchResult* Result = BatchResults.Find(BatchID);
    
    if (!Config || !Result)
    {
        return false;
    }
    
    // 查找要重新生成的资产
    for (int32 i = 0; i < Result->GeneratedAssets.Num(); i++)
    {
        if (Result->GeneratedAssets[i].AssetID == AssetID)
        {
            // 重新生成资产
            FAIGeneratedAsset NewAsset = GenerateBatchItem(*Config, i);
            Result->GeneratedAssets[i] = NewAsset;
            
            // 触发资产完成事件
            OnBatchItemCompleted.Broadcast(BatchID, NewAsset);
            
            UE_LOG(LogTemp, Log, TEXT("Regenerated batch item: %s"), *AssetID);
            return true;
        }
    }
    
    return false;
}

bool UMingRTSBatchGenerationSystem::SetBatchPriority(const FString& BatchID, int32 Priority)
{
    if (!IsValidBatchID(BatchID))
    {
        return false;
    }
    
    int32* CurrentPriority = BatchPriorities.Find(BatchID);
    if (CurrentPriority)
    {
        *CurrentPriority = Priority;
        
        // 重新排序批次队列
        ProcessBatchQueue();
        
        UE_LOG(LogTemp, Log, TEXT("Set batch priority: %s = %d"), *BatchID, Priority);
        return true;
    }
    
    return false;
}

TMap<FString, float> UMingRTSBatchGenerationSystem::GetBatchStatistics() const
{
    TMap<FString, float> Statistics;
    
    int32 TotalBatches = BatchResults.Num();
    int32 CompletedBatches = 0;
    int32 FailedBatches = 0;
    int32 RunningBatches = 0;
    float TotalGeneratedAssets = 0.0f;
    float AverageGenerationTime = 0.0f;
    
    for (const TPair<FString, FAIBatchResult>& Pair : BatchResults)
    {
        const FAIBatchResult& Result = Pair.Value;
        
        switch (Result.Status)
        {
            case EBatchStatus::Completed:
                CompletedBatches++;
                break;
            case EBatchStatus::Failed:
                FailedBatches++;
                break;
            case EBatchStatus::Running:
                RunningBatches++;
                break;
            default:
                break;
        }
        
        TotalGeneratedAssets += Result.GeneratedAssets.Num();
        AverageGenerationTime += Result.TotalDuration;
    }
    
    if (CompletedBatches > 0)
    {
        AverageGenerationTime /= CompletedBatches;
    }
    
    Statistics.Add(TEXT("TotalBatches"), TotalBatches);
    Statistics.Add(TEXT("CompletedBatches"), CompletedBatches);
    Statistics.Add(TEXT("FailedBatches"), FailedBatches);
    Statistics.Add(TEXT("RunningBatches"), RunningBatches);
    Statistics.Add(TEXT("TotalGeneratedAssets"), TotalGeneratedAssets);
    Statistics.Add(TEXT("AverageGenerationTime"), AverageGenerationTime);
    Statistics.Add(TEXT("SuccessRate"), TotalBatches > 0 ? (float)CompletedBatches / TotalBatches * 100.0f : 0.0f);
    
    return Statistics;
}

void UMingRTSBatchGenerationSystem::ProcessBatchGeneration(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return;
    }
    
    FAIBatchConfiguration* Config = BatchConfigurations.Find(BatchID);
    FAIBatchResult* Result = BatchResults.Find(BatchID);
    EBatchStatus* Status = BatchStatuses.Find(BatchID);
    
    if (!Config || !Result || !Status)
    {
        return;
    }
    
    if (*Status != EBatchStatus::Pending && *Status != EBatchStatus::Running)
    {
        return;
    }
    
    *Status = EBatchStatus::Running;
    
    UE_LOG(LogTemp, Log, TEXT("Processing batch generation: %s"), *BatchID);
    
    // 检查依赖关系
    if (!CheckDependencies(*Config))
    {
        HandleBatchError(BatchID, TEXT("Dependencies not met"));
        return;
    }
    
    // 生成批次项目
    for (int32 i = 0; i < Config->ItemCount; i++)
    {
        if (*Status == EBatchStatus::Cancelled || *Status == EBatchStatus::Paused)
        {
            break;
        }
        
        // 生成单个项目
        FAIGeneratedAsset Asset = GenerateBatchItem(*Config, i);
        
        if (!Asset.AssetID.IsEmpty())
        {
            Result->GeneratedAssets.Add(Asset);
            Result->SuccessCount++;
            
            // 触发资产完成事件
            OnBatchItemCompleted.Broadcast(BatchID, Asset);
        }
        else
        {
            Result->ErrorCount++;
        }
        
        // 更新进度
        UpdateBatchProgress(BatchID, i + 1);
        
        // 添加延迟以避免过度占用资源
        FPlatformProcess::Sleep(BatchUpdateInterval);
    }
    
    // 处理依赖关系
    ProcessDependencies(*Config, Result->GeneratedAssets);
    
    // 完成批次生成
    CompleteBatchGeneration(BatchID);
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateBatchItem(const FAIBatchConfiguration& Config, int32 ItemIndex)
{
    FAIGeneratedAsset Asset;
    
    // 生成基础提示词
    FString BasePrompt = Config.Theme;
    
    // 应用变化规则
    FString FinalPrompt = ApplyVariationRules(BasePrompt, Config, ItemIndex);
    
    // 根据批次类型生成不同的内容
    switch (Config.BatchType)
    {
        case EBatchType::SceneBatch:
            Asset = GenerateSceneAsset(FinalPrompt, Config);
            break;
        case EBatchType::CharacterBatch:
            Asset = GenerateCharacterAsset(FinalPrompt, Config);
            break;
        case EBatchType::BuildingBatch:
            Asset = GenerateBuildingAsset(FinalPrompt, Config);
            break;
        case EBatchType::UIBatch:
            Asset = GenerateUIAsset(FinalPrompt, Config);
            break;
        case EBatchType::AudioBatch:
            Asset = GenerateAudioAsset(FinalPrompt, Config);
            break;
        case EBatchType::ProjectBatch:
            Asset = GenerateProjectAsset(FinalPrompt, Config);
            break;
        default:
            Asset = GenerateCustomAsset(FinalPrompt, Config);
            break;
    }
    
    return Asset;
}

FString UMingRTSBatchGenerationSystem::ApplyVariationRules(const FString& BasePrompt, const FAIBatchConfiguration& Config, int32 ItemIndex)
{
    FString FinalPrompt = BasePrompt;
    
    for (EVariationType Variation : Config.VariationRules)
    {
        switch (Variation)
        {
            case EVariationType::TimeOfDay:
                FinalPrompt += FString::Printf(TEXT(", time of day: %s"), *GetTimeOfDayVariation(ItemIndex));
                break;
            case EVariationType::Weather:
                FinalPrompt += FString::Printf(TEXT(", weather: %s"), *GetWeatherVariation(ItemIndex));
                break;
            case EVariationType::Camera:
                FinalPrompt += FString::Printf(TEXT(", camera angle: %s"), *GetCameraVariation(ItemIndex));
                break;
            case EVariationType::Lighting:
                FinalPrompt += FString::Printf(TEXT(", lighting: %s"), *GetLightingVariation(ItemIndex));
                break;
            case EVariationType::Pose:
                FinalPrompt += FString::Printf(TEXT(", pose: %s"), *GetPoseVariation(ItemIndex));
                break;
            case EVariationType::Equipment:
                FinalPrompt += FString::Printf(TEXT(", equipment: %s"), *GetEquipmentVariation(ItemIndex));
                break;
            case EVariationType::Expression:
                FinalPrompt += FString::Printf(TEXT(", expression: %s"), *GetExpressionVariation(ItemIndex));
                break;
            case EVariationType::Action:
                FinalPrompt += FString::Printf(TEXT(", action: %s"), *GetActionVariation(ItemIndex));
                break;
            case EVariationType::Style:
                FinalPrompt += FString::Printf(TEXT(", style: %s"), *GetStyleVariation(ItemIndex));
                break;
            case EVariationType::Mood:
                FinalPrompt += FString::Printf(TEXT(", mood: %s"), *GetMoodVariation(ItemIndex));
                break;
            default:
                break;
        }
    }
    
    return FinalPrompt;
}

bool UMingRTSBatchGenerationSystem::ValidateBatchConfiguration(const FAIBatchConfiguration& Config) const
{
    if (Config.BatchName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Batch name cannot be empty"));
        return false;
    }
    
    if (Config.ItemCount <= 0 || Config.ItemCount > MaxItemsPerBatch)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid item count: %d (max: %d)"), Config.ItemCount, MaxItemsPerBatch);
        return false;
    }
    
    if (Config.Theme.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Batch theme cannot be empty"));
        return false;
    }
    
    return true;
}

void UMingRTSBatchGenerationSystem::HandleConditionTrigger(const FAIBatchCondition& Condition)
{
    // 检查条件是否满足
    bool bConditionMet = false;
    
    switch (Condition.ConditionType)
    {
        case EConditionType::GameContext:
            bConditionMet = CheckGameContext(Condition.Context);
            break;
        case EConditionType::TimeTrigger:
            bConditionMet = CheckTimeTrigger(Condition.Context);
            break;
        case EConditionType::EventTrigger:
            bConditionMet = CheckEventTrigger(Condition.Context);
            break;
        case EConditionType::UserAction:
            bConditionMet = CheckUserAction(Condition.Context);
            break;
        case EConditionType::ResourceAvailable:
            bConditionMet = CheckResourceAvailable(Condition.RequiredAssets);
            break;
        default:
            break;
    }
    
    if (bConditionMet)
    {
        // 查找对应的配置并启动批次
        for (const TPair<FString, FAIBatchCondition>& Pair : ConditionalBatches)
        {
            if (Pair.Value.Context == Condition.Context)
            {
                FString ConfigKey = FString::Printf(TEXT("Condition_%s"), *Pair.Key);
                FAIBatchConfiguration* Config = BatchConfigurations.Find(ConfigKey);
                
                if (Config)
                {
                    StartBatchGeneration(*Config);
                    
                    if (!Condition.bPersistent)
                    {
                        UnregisterConditionalBatch(Pair.Key);
                    }
                }
                break;
            }
        }
    }
}

void UMingRTSBatchGenerationSystem::OptimizeBatchResults(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return;
    }
    
    FAIBatchResult* Result = BatchResults.Find(BatchID);
    FAIBatchConfiguration* Config = BatchConfigurations.Find(BatchID);
    
    if (!Result || !Config || !Config->bEnableIterativeOptimization)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Optimizing batch results: %s"), *BatchID);
    
    // 根据优化标准优化结果
    for (EOptimizationCriteria Criteria : Config->OptimizationCriteria)
    {
        switch (Criteria)
        {
            case EOptimizationCriteria::VisualQuality:
                OptimizeForVisualQuality(BatchID);
                break;
            case EOptimizationCriteria::Performance:
                OptimizeForPerformance(BatchID);
                break;
            case EOptimizationCriteria::FileSize:
                OptimizeForFileSize(BatchID);
                break;
            case EOptimizationCriteria::LoadingTime:
                OptimizeForLoadingTime(BatchID);
                break;
            case EOptimizationCriteria::MemoryUsage:
                OptimizeForMemoryUsage(BatchID);
                break;
            default:
                break;
        }
    }
}

void UMingRTSBatchGenerationSystem::HandleBatchError(const FString& BatchID, const FString& ErrorMessage)
{
    if (!IsValidBatchID(BatchID))
    {
        return;
    }
    
    FAIBatchResult* Result = BatchResults.Find(BatchID);
    EBatchStatus* Status = BatchStatuses.Find(BatchID);
    
    if (Result && Status)
    {
        *Status = EBatchStatus::Failed;
        Result->Status = EBatchStatus::Failed;
        Result->EndTime = CurrentWorld->GetTimeSeconds();
        Result->TotalDuration = Result->EndTime - Result->StartTime;
        Result->ErrorMessages.Add(ErrorMessage);
        
        UE_LOG(LogTemp, Error, TEXT("Batch error: %s - %s"), *BatchID, *ErrorMessage);
        
        // 触发错误事件
        OnBatchError.Broadcast(BatchID, ErrorMessage);
    }
}

void UMingRTSBatchGenerationSystem::UpdateBatchProgress(const FString& BatchID, int32 Completed)
{
    if (!IsValidBatchID(BatchID))
    {
        return;
    }
    
    int32* Progress = BatchProgress.Find(BatchID);
    if (Progress)
    {
        *Progress = Completed;
        
        // 触发进度事件
        OnBatchProgress.Broadcast(BatchID, Completed);
    }
}

void UMingRTSBatchGenerationSystem::CompleteBatchGeneration(const FString& BatchID)
{
    if (!IsValidBatchID(BatchID))
    {
        return;
    }
    
    FAIBatchResult* Result = BatchResults.Find(BatchID);
    EBatchStatus* Status = BatchStatuses.Find(BatchID);
    
    if (Result && Status)
    {
        *Status = EBatchStatus::Completed;
        Result->Status = EBatchStatus::Completed;
        Result->EndTime = CurrentWorld->GetTimeSeconds();
        Result->TotalDuration = Result->EndTime - Result->StartTime;
        
        UE_LOG(LogTemp, Log, TEXT("Completed batch generation: %s"), *BatchID);
        
        // 触发完成事件
        OnBatchCompleted.Broadcast(*Result);
        
        // 清理完成的批次
        CleanupCompletedBatches();
    }
}

void UMingRTSBatchGenerationSystem::InitializeBatchTemplates()
{
    // 完整RTS游戏素材包
    FAIBatchTemplate CompleteRTSTemplate;
    CompleteRTSTemplate.TemplateName = TEXT("Complete RTS Game");
    CompleteRTSTemplate.Description = TEXT("Complete RTS game asset package");
    CompleteRTSTemplate.BatchType = EBatchType::ProjectBatch;
    CompleteRTSTemplate.DefaultConfiguration.BatchType = EBatchType::ProjectBatch;
    CompleteRTSTemplate.DefaultConfiguration.BatchName = TEXT("Complete RTS Game Assets");
    CompleteRTSTemplate.DefaultConfiguration.ItemCount = 20;
    CompleteRTSTemplate.DefaultConfiguration.Quality = EQualityLevel::High;
    CompleteRTSTemplate.DefaultConfiguration.Style = EArtStyle::Realistic;
    CompleteRTSTemplate.DefaultConfiguration.Theme = TEXT("Ming dynasty RTS game complete asset package");
    CompleteRTSTemplate.RecommendedVariations.Add(TEXT("TimeOfDay"));
    CompleteRTSTemplate.RecommendedVariations.Add(TEXT("Weather"));
    CompleteRTSTemplate.RecommendedVariations.Add(TEXT("Lighting"));
    CompleteRTSTemplate.UseCases.Add(TEXT("Complete game development"));
    CompleteRTSTemplate.UseCases.Add(TEXT("Prototype creation"));
    BatchTemplates.Add(TEXT("Complete RTS Game"), CompleteRTSTemplate);
    
    // 明朝主题素材包
    FAIBatchTemplate MingDynastyTemplate;
    MingDynastyTemplate.TemplateName = TEXT("Ming Dynasty Theme");
    MingDynastyTemplate.Description = TEXT("Ming dynasty themed asset package");
    MingDynastyTemplate.BatchType = EBatchType::ProjectBatch;
    MingDynastyTemplate.DefaultConfiguration.BatchType = EBatchType::ProjectBatch;
    MingDynastyTemplate.DefaultConfiguration.BatchName = TEXT("Ming Dynasty Theme Assets");
    MingDynastyTemplate.DefaultConfiguration.ItemCount = 15;
    MingDynastyTemplate.DefaultConfiguration.Quality = EQualityLevel::Ultra;
    MingDynastyTemplate.DefaultConfiguration.Style = EArtStyle::Photorealistic;
    MingDynastyTemplate.DefaultConfiguration.Theme = TEXT("Ming dynasty historical theme");
    MingDynastyTemplate.RecommendedVariations.Add(TEXT("Style"));
    MingDynastyTemplate.RecommendedVariations.Add(TEXT("Mood"));
    MingDynastyTemplate.UseCases.Add(TEXT("Historical games"));
    MingDynastyTemplate.UseCases.Add(TEXT("Educational content"));
    BatchTemplates.Add(TEXT("Ming Dynasty Theme"), MingDynastyTemplate);
    
    // 战斗场景系列
    FAIBatchTemplate BattleScenesTemplate;
    BattleScenesTemplate.TemplateName = TEXT("Battle Scenes");
    BattleScenesTemplate.Description = TEXT("Battle scene series");
    BattleScenesTemplate.BatchType = EBatchType::SceneBatch;
    BattleScenesTemplate.DefaultConfiguration.BatchType = EBatchType::SceneBatch;
    BattleScenesTemplate.DefaultConfiguration.BatchName = TEXT("Battle Scene Series");
    BattleScenesTemplate.DefaultConfiguration.ItemCount = 10;
    BattleScenesTemplate.DefaultConfiguration.Quality = EQualityLevel::High;
    BattleScenesTemplate.DefaultConfiguration.Style = EArtStyle::Realistic;
    BattleScenesTemplate.DefaultConfiguration.Theme = TEXT("Epic battle scenes");
    BattleScenesTemplate.RecommendedVariations.Add(TEXT("TimeOfDay"));
    BattleScenesTemplate.RecommendedVariations.Add(TEXT("Weather"));
    BattleScenesTemplate.RecommendedVariations.Add(TEXT("Camera"));
    BattleScenesTemplate.UseCases.Add(TEXT("Battle sequences"));
    BattleScenesTemplate.UseCases.Add(TEXT("Cinematic scenes"));
    BatchTemplates.Add(TEXT("Battle Scenes"), BattleScenesTemplate);
    
    // 角色素材包
    FAIBatchTemplate CharacterPackTemplate;
    CharacterPackTemplate.TemplateName = TEXT("Character Pack");
    CharacterPackTemplate.Description = TEXT("Character asset package");
    CharacterPackTemplate.BatchType = EBatchType::CharacterBatch;
    CharacterPackTemplate.DefaultConfiguration.BatchType = EBatchType::CharacterBatch;
    CharacterPackTemplate.DefaultConfiguration.BatchName = TEXT("Character Asset Pack");
    CharacterPackTemplate.DefaultConfiguration.ItemCount = 8;
    CharacterPackTemplate.DefaultConfiguration.Quality = EQualityLevel::Ultra;
    CharacterPackTemplate.DefaultConfiguration.Style = EArtStyle::Photorealistic;
    CharacterPackTemplate.DefaultConfiguration.Theme = TEXT("Ming dynasty characters");
    CharacterPackTemplate.RecommendedVariations.Add(TEXT("Pose"));
    CharacterPackTemplate.RecommendedVariations.Add(TEXT("Equipment"));
    CharacterPackTemplate.RecommendedVariations.Add(TEXT("Expression"));
    CharacterPackTemplate.UseCases.Add(TEXT("Character design"));
    CharacterPackTemplate.UseCases.Add(TEXT("Game characters"));
    BatchTemplates.Add(TEXT("Character Pack"), CharacterPackTemplate);
    
    // UI界面套件
    FAIBatchTemplate UITemplate;
    UITemplate.TemplateName = TEXT("UI Kit");
    UITemplate.Description = TEXT("UI interface kit");
    UITemplate.BatchType = EBatchType::UIBatch;
    UITemplate.DefaultConfiguration.BatchType = EBatchType::UIBatch;
    UITemplate.DefaultConfiguration.BatchName = TEXT("UI Interface Kit");
    UITemplate.DefaultConfiguration.ItemCount = 12;
    UITemplate.DefaultConfiguration.Quality = EQualityLevel::High;
    UITemplate.DefaultConfiguration.Style = EArtStyle::Modern;
    UITemplate.DefaultConfiguration.Theme = TEXT("Ming dynasty themed UI");
    UITemplate.RecommendedVariations.Add(TEXT("Style"));
    UITemplate.RecommendedVariations.Add(TEXT("Mood"));
    UITemplate.UseCases.Add(TEXT("UI design"));
    UITemplate.UseCases.Add(TEXT("Interface creation"));
    BatchTemplates.Add(TEXT("UI Kit"), UITemplate);
    
    // 音效音乐包
    FAIBatchTemplate AudioPackTemplate;
    AudioPackTemplate.TemplateName = TEXT("Audio Pack");
    AudioPackTemplate.Description = TEXT("Audio and music package");
    AudioPackTemplate.BatchType = EBatchType::AudioBatch;
    AudioPackTemplate.DefaultConfiguration.BatchType = EBatchType::AudioBatch;
    AudioPackTemplate.DefaultConfiguration.BatchName = TEXT("Audio Music Pack");
    AudioPackTemplate.DefaultConfiguration.ItemCount = 6;
    AudioPackTemplate.DefaultConfiguration.Quality = EQualityLevel::High;
    AudioPackTemplate.DefaultConfiguration.Style = EArtStyle::Traditional;
    AudioPackTemplate.DefaultConfiguration.Theme = TEXT("Traditional Chinese music and sound effects");
    AudioPackTemplate.RecommendedVariations.Add(TEXT("Mood"));
    AudioPackTemplate.RecommendedVariations.Add(TEXT("Style"));
    AudioPackTemplate.UseCases.Add(TEXT("Game audio"));
    AudioPackTemplate.UseCases.Add(TEXT("Background music"));
    BatchTemplates.Add(TEXT("Audio Pack"), AudioPackTemplate);
    
    UE_LOG(LogTemp, Log, TEXT("Initialized %d batch templates"), BatchTemplates.Num());
}

FString UMingRTSBatchGenerationSystem::GenerateUniqueBatchID() const
{
    return FString::Printf(TEXT("Batch_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

float UMingRTSBatchGenerationSystem::CalculateEstimatedTime(const FAIBatchConfiguration& Config) const
{
    float BaseTime = 5.0f; // 基础时间（秒）
    
    // 根据质量级别调整时间
    switch (Config.Quality)
    {
        case EQualityLevel::Draft:
            BaseTime *= 0.5f;
            break;
        case EQualityLevel::Standard:
            BaseTime *= 1.0f;
            break;
        case EQualityLevel::High:
            BaseTime *= 2.0f;
            break;
        case EQualityLevel::Ultra:
            BaseTime *= 4.0f;
            break;
        case EQualityLevel::Cinematic:
            BaseTime *= 8.0f;
            break;
        default:
            break;
    }
    
    // 根据项目数量调整时间
    BaseTime *= Config.ItemCount;
    
    // 根据批次类型调整时间
    switch (Config.BatchType)
    {
        case EBatchType::SceneBatch:
            BaseTime *= 1.5f;
            break;
        case EBatchType::CharacterBatch:
            BaseTime *= 1.2f;
            break;
        case EBatchType::BuildingBatch:
            BaseTime *= 1.3f;
            break;
        case EBatchType::UIBatch:
            BaseTime *= 0.8f;
            break;
        case EBatchType::AudioBatch:
            BaseTime *= 1.0f;
            break;
        case EBatchType::ProjectBatch:
            BaseTime *= 2.0f;
            break;
        default:
            break;
    }
    
    return BaseTime;
}

bool UMingRTSBatchGenerationSystem::CheckDependencies(const FAIBatchConfiguration& Config) const
{
    // 检查依赖关系
    for (const TPair<EAssetType, EAssetType>& Dependency : Config.Dependencies)
    {
        // 在实际实现中，这里应该检查依赖的资产是否已生成
        // 这里只是示例代码
    }
    
    return true;
}

void UMingRTSBatchGenerationSystem::ProcessDependencies(const FAIBatchConfiguration& Config, TArray<FAIGeneratedAsset>& Assets)
{
    // 处理依赖关系
    for (const TPair<EAssetType, EAssetType>& Dependency : Config.Dependencies)
    {
        // 在实际实现中，这里应该处理依赖关系
        // 这里只是示例代码
    }
}

// 辅助函数实现
FString UMingRTSBatchGenerationSystem::GetBatchTypeName(EBatchType BatchType) const
{
    switch (BatchType)
    {
        case EBatchType::SceneBatch: return TEXT("SceneBatch");
        case EBatchType::CharacterBatch: return TEXT("CharacterBatch");
        case EBatchType::BuildingBatch: return TEXT("BuildingBatch");
        case EBatchType::UIBatch: return TEXT("UIBatch");
        case EBatchType::AudioBatch: return TEXT("AudioBatch");
        case EBatchType::ProjectBatch: return TEXT("ProjectBatch");
        default: return TEXT("Custom");
    }
}

FString UMingRTSBatchGenerationSystem::GetVariationTypeName(EVariationType VariationType) const
{
    switch (VariationType)
    {
        case EVariationType::TimeOfDay: return TEXT("TimeOfDay");
        case EVariationType::Weather: return TEXT("Weather");
        case EVariationType::Camera: return TEXT("Camera");
        case EVariationType::Lighting: return TEXT("Lighting");
        case EVariationType::Pose: return TEXT("Pose");
        case EVariationType::Equipment: return TEXT("Equipment");
        case EVariationType::Expression: return TEXT("Expression");
        case EVariationType::Action: return TEXT("Action");
        case EVariationType::Style: return TEXT("Style");
        case EVariationType::Mood: return TEXT("Mood");
        default: return TEXT("Custom");
    }
}

FString UMingRTSBatchGenerationSystem::GetConditionTypeName(EConditionType ConditionType) const
{
    switch (ConditionType)
    {
        case EConditionType::GameContext: return TEXT("GameContext");
        case EConditionType::TimeTrigger: return TEXT("TimeTrigger");
        case EConditionType::EventTrigger: return TEXT("EventTrigger");
        case EConditionType::UserAction: return TEXT("UserAction");
        case EConditionType::ResourceAvailable: return TEXT("ResourceAvailable");
        default: return TEXT("Custom");
    }
}

bool UMingRTSBatchGenerationSystem::IsValidBatchID(const FString& BatchID) const
{
    return !BatchID.IsEmpty() && BatchConfigurations.Contains(BatchID);
}

void UMingRTSBatchGenerationSystem::CleanupCompletedBatches()
{
    // 清理完成的批次（保留最近的结果）
    const int32 MaxCompletedBatches = 10;
    int32 CompletedCount = 0;
    
    TArray<FString> BatchesToRemove;
    
    for (const TPair<FString, FAIBatchResult>& Pair : BatchResults)
    {
        if (Pair.Value.Status == EBatchStatus::Completed)
        {
            CompletedCount++;
            if (CompletedCount > MaxCompletedBatches)
            {
                BatchesToRemove.Add(Pair.Key);
            }
        }
    }
    
    for (const FString& BatchID : BatchesToRemove)
    {
        BatchConfigurations.Remove(BatchID);
        BatchResults.Remove(BatchID);
        BatchStatuses.Remove(BatchID);
        BatchProgress.Remove(BatchID);
        BatchPriorities.Remove(BatchID);
    }
}

void UMingRTSBatchGenerationSystem::LogBatchMessage(const FString& BatchID, const FString& Message, bool bIsError)
{
    if (bIsError)
    {
        UE_LOG(LogTemp, Error, TEXT("Batch %s: %s"), *BatchID, *Message);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Batch %s: %s"), *BatchID, *Message);
    }
}

void UMingRTSBatchGenerationSystem::UpdateBatchStatistics()
{
    // 更新批次统计信息
    TMap<FString, float> Statistics = GetBatchStatistics();
    
    UE_LOG(LogTemp, Log, TEXT("Batch Statistics:"));
    for (const TPair<FString, float>& Pair : Statistics)
    {
        UE_LOG(LogTemp, Log, TEXT("  %s: %.2f"), *Pair.Key, Pair.Value);
    }
}

void UMingRTSBatchGenerationSystem::ProcessBatchQueue()
{
    // 处理批次队列（根据优先级排序）
    TArray<FString> SortedBatches;
    
    // 收集所有待处理的批次
    for (const TPair<FString, EBatchStatus>& Pair : BatchStatuses)
    {
        if (Pair.Value == EBatchStatus::Pending)
        {
            SortedBatches.Add(Pair.Key);
        }
    }
    
    // 按优先级排序
    SortedBatches.Sort([this](const FString& A, const FString& B)
    {
        const int32* PriorityA = BatchPriorities.Find(A);
        const int32* PriorityB = BatchPriorities.Find(B);
        
        int32 PriorityValueA = PriorityA ? *PriorityA : 0;
        int32 PriorityValueB = PriorityB ? *PriorityB : 0;
        
        return PriorityValueA > PriorityValueB;
    });
    
    // 处理排序后的批次
    int32 RunningCount = 0;
    for (const FString& BatchID : SortedBatches)
    {
        if (RunningCount >= MaxConcurrentBatches)
        {
            break;
        }
        
        EBatchStatus* Status = BatchStatuses.Find(BatchID);
        if (Status && *Status == EBatchStatus::Pending)
        {
            ProcessBatchGeneration(BatchID);
            RunningCount++;
        }
    }
}

bool UMingRTSBatchGenerationSystem::CheckBatchLimits(const FAIBatchConfiguration& Config) const
{
    // 检查当前运行的批次数量
    int32 RunningCount = 0;
    
    for (const TPair<FString, EBatchStatus>& Pair : BatchStatuses)
    {
        if (Pair.Value == EBatchStatus::Running)
        {
            RunningCount++;
        }
    }
    
    return RunningCount < MaxConcurrentBatches;
}

TMap<FString, FString> UMingRTSBatchGenerationSystem::GetSystemStatus() const
{
    TMap<FString, FString> Status;
    
    Status.Add(TEXT("CurrentWorld"), CurrentWorld ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("AIContentSystem"), AIContentSystem ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("ContentRenderer"), ContentRenderer ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("ContentManager"), ContentManager ? TEXT("Available") : TEXT("Not Available"));
    Status.Add(TEXT("TotalBatches"), FString::FromInt(BatchResults.Num()));
    Status.Add(TEXT("RunningBatches"), FString::FromInt(RunningCount));
    
    return Status;
}

// 变化生成辅助函数
FString UMingRTSBatchGenerationSystem::GetTimeOfDayVariation(int32 Index) const
{
    TArray<FString> TimeVariations = {
        TEXT("dawn"), TEXT("morning"), TEXT("noon"), TEXT("afternoon"),
        TEXT("dusk"), TEXT("evening"), TEXT("night"), TEXT("midnight")
    };
    
    return TimeVariations[Index % TimeVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetWeatherVariation(int32 Index) const
{
    TArray<FString> WeatherVariations = {
        TEXT("clear"), TEXT("cloudy"), TEXT("rainy"), TEXT("stormy"),
        TEXT("foggy"), TEXT("snowy"), TEXT("windy"), TEXT("calm")
    };
    
    return WeatherVariations[Index % WeatherVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetCameraVariation(int32 Index) const
{
    TArray<FString> CameraVariations = {
        TEXT("wide shot"), TEXT("medium shot"), TEXT("close-up"), TEXT("extreme close-up"),
        TEXT("high angle"), TEXT("low angle"), TEXT("dutch angle"), TEXT("bird's eye view")
    };
    
    return CameraVariations[Index % CameraVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetLightingVariation(int32 Index) const
{
    TArray<FString> LightingVariations = {
        TEXT("natural light"), TEXT("dramatic lighting"), TEXT("soft lighting"), TEXT("hard lighting"),
        TEXT("backlighting"), TEXT("rim lighting"), TEXT("studio lighting"), TEXT("golden hour")
    };
    
    return LightingVariations[Index % LightingVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetPoseVariation(int32 Index) const
{
    TArray<FString> PoseVariations = {
        TEXT("standing"), TEXT("sitting"), TEXT("running"), TEXT("jumping"),
        TEXT("fighting stance"), TEXT("defensive pose"), TEXT("attacking pose"), TEXT("idle stance")
    };
    
    return PoseVariations[Index % PoseVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetEquipmentVariation(int32 Index) const
{
    TArray<FString> EquipmentVariations = {
        TEXT("light armor"), TEXT("heavy armor"), TEXT("no armor"), TEXT("ceremonial armor"),
        TEXT("sword and shield"), TEXT("bow and arrow"), TEXT("spear"), TEXT("halberd")
    };
    
    return EquipmentVariations[Index % EquipmentVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetExpressionVariation(int32 Index) const
{
    TArray<FString> ExpressionVariations = {
        TEXT("neutral"), TEXT("happy"), TEXT("angry"), TEXT("sad"),
        TEXT("determined"), TEXT("fearful"), TEXT("surprised"), TEXT("confident")
    };
    
    return ExpressionVariations[Index % ExpressionVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetActionVariation(int32 Index) const
{
    TArray<FString> ActionVariations = {
        TEXT("standing still"), TEXT("walking"), TEXT("running"), TEXT("fighting"),
        TEXT("defending"), TEXT("attacking"), TEXT("casting spell"), TEXT("using item")
    };
    
    return ActionVariations[Index % ActionVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetStyleVariation(int32 Index) const
{
    TArray<FString> StyleVariations = {
        TEXT("realistic"), TEXT("stylized"), TEXT("cartoon"), TEXT("watercolor"),
        TEXT("ink wash"), TEXT("oil painting"), TEXT("pixel art"), TEXT("low poly")
    };
    
    return StyleVariations[Index % StyleVariations.Num()];
}

FString UMingRTSBatchGenerationSystem::GetMoodVariation(int32 Index) const
{
    TArray<FString> MoodVariations = {
        TEXT("peaceful"), TEXT("dramatic"), TEXT("mysterious"), TEXT("epic"),
        TEXT("tense"), TEXT("joyful"), TEXT("somber"), TEXT("heroic")
    };
    
    return MoodVariations[Index % MoodVariations.Num()];
}

// 资产生成辅助函数
FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateSceneAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::Scene;
    Asset.AssetName = FString::Printf(TEXT("Scene_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成场景
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/Scenes/");
    
    return Asset;
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateCharacterAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::Mesh;
    Asset.AssetName = FString::Printf(TEXT("Character_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成角色
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/Characters/");
    
    return Asset;
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateBuildingAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::Mesh;
    Asset.AssetName = FString::Printf(TEXT("Building_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成建筑
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/Buildings/");
    
    return Asset;
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateUIAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::UI;
    Asset.AssetName = FString::Printf(TEXT("UI_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成UI
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/UI/");
    
    return Asset;
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateAudioAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::Audio;
    Asset.AssetName = FString::Printf(TEXT("Audio_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成音频
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/Audio/");
    
    return Asset;
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateProjectAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::Project;
    Asset.AssetName = FString::Printf(TEXT("Project_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成项目
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/Projects/");
    
    return Asset;
}

FAIGeneratedAsset UMingRTSBatchGenerationSystem::GenerateCustomAsset(const FString& Prompt, const FAIBatchConfiguration& Config)
{
    FAIGeneratedAsset Asset;
    Asset.AssetType = EAIContentType::Custom;
    Asset.AssetName = FString::Printf(TEXT("Custom_%s"), *FDateTime::Now().ToString());
    Asset.AssetID = GenerateUniqueBatchID();
    Asset.Quality = Config.Quality;
    Asset.Style = Config.Style;
    
    // 在实际实现中，这里应该调用AI内容系统生成自定义资产
    // 这里只是示例代码
    Asset.AssetDescription = Prompt;
    Asset.AssetPath = TEXT("/Game/GeneratedContent/Custom/");
    
    return Asset;
}

// 条件检查辅助函数
bool UMingRTSBatchGenerationSystem::CheckGameContext(const FString& Context) const
{
    // 在实际实现中，这里应该检查游戏上下文
    // 这里只是示例代码
    return Context.Contains(TEXT("level")) || Context.Contains(TEXT("stage"));
}

bool UMingRTSBatchGenerationSystem::CheckTimeTrigger(const FString& Context) const
{
    // 在实际实现中，这里应该检查时间触发条件
    // 这里只是示例代码
    return FDateTime::Now().GetHour() >= 9 && FDateTime::Now().GetHour() <= 17;
}

bool UMingRTSBatchGenerationSystem::CheckEventTrigger(const FString& Context) const
{
    // 在实际实现中，这里应该检查事件触发条件
    // 这里只是示例代码
    return Context.Contains(TEXT("event")) || Context.Contains(TEXT("trigger"));
}

bool UMingRTSBatchGenerationSystem::CheckUserAction(const FString& Context) const
{
    // 在实际实现中，这里应该检查用户动作
    // 这里只是示例代码
    return Context.Contains(TEXT("click")) || Context.Contains(TEXT("action"));
}

bool UMingRTSBatchGenerationSystem::CheckResourceAvailable(const TArray<EAssetType>& RequiredAssets) const
{
    // 在实际实现中，这里应该检查资源是否可用
    // 这里只是示例代码
    return RequiredAssets.Num() > 0;
}

// 优化辅助函数
void UMingRTSBatchGenerationSystem::OptimizeForVisualQuality(const FString& BatchID)
{
    // 视觉质量优化
    UE_LOG(LogTemp, Log, TEXT("Optimizing batch %s for visual quality"), *BatchID);
}

void UMingRTSBatchGenerationSystem::OptimizeForPerformance(const FString& BatchID)
{
    // 性能优化
    UE_LOG(LogTemp, Log, TEXT("Optimizing batch %s for performance"), *BatchID);
}

void UMingRTSBatchGenerationSystem::OptimizeForFileSize(const FString& BatchID)
{
    // 文件大小优化
    UE_LOG(LogTemp, Log, TEXT("Optimizing batch %s for file size"), *BatchID);
}

void UMingRTSBatchGenerationSystem::OptimizeForLoadingTime(const FString& BatchID)
{
    // 加载时间优化
    UE_LOG(LogTemp, Log, TEXT("Optimizing batch %s for loading time"), *BatchID);
}

void UMingRTSBatchGenerationSystem::OptimizeForMemoryUsage(const FString& BatchID)
{
    // 内存使用优化
    UE_LOG(LogTemp, Log, TEXT("Optimizing batch %s for memory usage"), *BatchID);
}

// 导出辅助函数
bool UMingRTSBatchGenerationSystem::ExportBatchAsUnrealPackage(const FString& BatchID)
{
    // 导出为Unreal包
    UE_LOG(LogTemp, Log, TEXT("Exporting batch %s as Unreal package"), *BatchID);
    return true;
}

bool UMingRTSBatchGenerationSystem::ExportBatchAsStandalone(const FString& BatchID)
{
    // 导出为独立格式
    UE_LOG(LogTemp, Log, TEXT("Exporting batch %s as standalone"), *BatchID);
    return true;
}

bool UMingRTSBatchGenerationSystem::ExportBatchAsWebPackage(const FString& BatchID)
{
    // 导出为Web包
    UE_LOG(LogTemp, Log, TEXT("Exporting batch %s as web package"), *BatchID);
    return true;
}

// 报告生成辅助函数
FString UMingRTSBatchGenerationSystem::GenerateBatchReport(const FAIBatchResult& Result) const
{
    FString Report;
    
    Report += FString::Printf(TEXT("# Batch Generation Report\n\n"));
    Report += FString::Printf(TEXT("## Batch Information\n\n"));
    Report += FString::Printf(TEXT("- **Batch ID**: %s\n"), *Result.BatchID);
    Report += FString::Printf(TEXT("- **Batch Name**: %s\n"), *Result.BatchName);
    Report += FString::Printf(TEXT("- **Status**: %s\n"), *GetBatchStatusName(Result.Status));
    Report += FString::Printf(TEXT("- **Total Items**: %d\n"), Result.TotalItems);
    Report += FString::Printf(TEXT("- **Success Count**: %d\n"), Result.SuccessCount);
    Report += FString::Printf(TEXT("- **Error Count**: %d\n"), Result.ErrorCount);
    Report += FString::Printf(TEXT("- **Skipped Count**: %d\n"), Result.SkippedCount);
    Report += FString::Printf(TEXT("- **Start Time**: %.2f\n"), Result.StartTime);
    Report += FString::Printf(TEXT("- **End Time**: %.2f\n"), Result.EndTime);
    Report += FString::Printf(TEXT("- **Total Duration**: %.2f seconds\n\n"), Result.TotalDuration);
    
    Report += FString::Printf(TEXT("## Generated Assets\n\n"));
    for (const FAIGeneratedAsset& Asset : Result.GeneratedAssets)
    {
        Report += FString::Printf(TEXT("- **%s**: %s\n"), *Asset.AssetName, *Asset.AssetDescription);
    }
    
    if (Result.ErrorMessages.Num() > 0)
    {
        Report += FString::Printf(TEXT("\n## Errors\n\n"));
        for (const FString& Error : Result.ErrorMessages)
        {
            Report += FString::Printf(TEXT("- %s\n"), *Error);
        }
    }
    
    if (Result.Warnings.Num() > 0)
    {
        Report += FString::Printf(TEXT("\n## Warnings\n\n"));
        for (const FString& Warning : Result.Warnings)
        {
            Report += FString::Printf(TEXT("- %s\n"), *Warning);
        }
    }
    
    return Report;
}

FString UMingRTSBatchGenerationSystem::GenerateShareMetadata(const FAIBatchResult& Result, const FString& ShareID) const
{
    FString Metadata;
    
    Metadata += TEXT("{\n");
    Metadata += FString::Printf(TEXT("  \"shareID\": \"%s\",\n"), *ShareID);
    Metadata += FString::Printf(TEXT("  \"batchID\": \"%s\",\n"), *Result.BatchID);
    Metadata += FString::Printf(TEXT("  \"batchName\": \"%s\",\n"), *Result.BatchName);
    Metadata += FString::Printf(TEXT("  \"status\": \"%s\",\n"), *GetBatchStatusName(Result.Status));
    Metadata += FString::Printf(TEXT("  \"totalItems\": %d,\n"), Result.TotalItems);
    Metadata += FString::Printf(TEXT("  \"successCount\": %d,\n"), Result.SuccessCount);
    Metadata += FString::Printf(TEXT("  \"generatedAt\": \"%.2f\"\n"), Result.StartTime);
    Metadata += TEXT("}\n");
    
    return Metadata;
}

FString UMingRTSBatchGenerationSystem::GetBatchStatusName(EBatchStatus Status) const
{
    switch (Status)
    {
        case EBatchStatus::Pending: return TEXT("Pending");
        case EBatchStatus::Running: return TEXT("Running");
        case EBatchStatus::Paused: return TEXT("Paused");
        case EBatchStatus::Completed: return TEXT("Completed");
        case EBatchStatus::Failed: return TEXT("Failed");
        case EBatchStatus::Cancelled: return TEXT("Cancelled");
        default: return TEXT("Custom");
    }
}
