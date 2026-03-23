#include "MingBMADProjectPlanner.h"
#include "MingRTSBMADSystem.h"
#include "MingTaskAnalyzer.h"
#include "MingCompileAPI.h"
#include "MingRTSAPISystem.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingBMADProjectPlanner::UMingBMADProjectPlanner()
{
    BMADSystem = nullptr;
    TaskAnalyzer = nullptr;
    CompileAPI = nullptr;
    APISystem = nullptr;
    
    bBMADSystemEnabled = true;
    MaxConcurrentProjects = 5.0f;
    DefaultProjectDuration = 4.0f;
    
    SystemStrengths.Empty();
    SystemWeaknesses.Empty();
    SystemOpportunities.Empty();
    SystemThreats.Empty();
}

void UMingBMADProjectPlanner::InitializeProjectPlanner()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingBMADProjectPlanner..."));

    // 初始化 BMAD 系統
    if (bBMADSystemEnabled)
    {
        BMADSystem = NewObject<UMingRTSBMADSystem>();
        BMADSystem->InitializeBMADSystem();
        
        // 綁定 BMAD 事件
        BMADSystem->OnDiscussionStarted.AddDynamic(this, &UMingBMADProjectPlanner::OnDirectionDiscussionStarted);
        BMADSystem->OnInsightGenerated.AddDynamic(this, &UMingBMADProjectPlanner::OnBMADInsightGenerated);
    }

    // 初始化任務分析器
    TaskAnalyzer = NewObject<UMingTaskAnalyzer>();
    TaskAnalyzer->InitializeTaskAnalyzer();

    // 初始化編譯 API
    CompileAPI = NewObject<UMingCompileAPI>();
    CompileAPI->InitializeCompileAPI();

    // 獲取 API 系統
    APISystem = GetMingRTSAPISystem();

    // 加載現有專案數據
    LoadProjectData();

    // 分析系統當前狀態
    AnalyzeCurrentSystemState();

    UE_LOG(LogTemp, Log, TEXT("MingBMADProjectPlanner initialized successfully"));
}

bool UMingBMADProjectPlanner::StartBMADProjectPlanning()
{
    UE_LOG(LogTemp, Log, TEXT("Starting BMAD Project Planning..."));

    if (!BMADSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("BMAD System is not initialized"));
        return false;
    }

    // 識別專案機會
    TArray<EBMADProjectDirection> Opportunities = IdentifyProjectOpportunities();
    
    if (Opportunities.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No project opportunities identified"));
        return false;
    }

    // 為每個機會啟動專家討論
    for (EBMADProjectDirection Direction : Opportunities)
    {
        FString DiscussionID = StartDirectionDiscussion(Direction);
        if (!DiscussionID.IsEmpty())
        {
            UE_LOG(LogTemp, Log, TEXT("Started direction discussion for %s: %s"), 
                *DirectionToString(Direction), *DiscussionID);
        }
    }

    return true;
}

FString UMingBMADProjectPlanner::CreateProjectPlan(const FString& Title, const FString& Description, EBMADProjectDirection Direction)
{
    FString ProjectID = GenerateUniqueProjectID();
    
    FBMADProjectPlan Project;
    Project.ProjectID = ProjectID;
    Project.Title = Title;
    Project.Description = Description;
    Project.Direction = Direction;
    Project.Status = EBMADProjectStatus::Planning;
    Project.CreatedTime = FDateTime::Now();
    Project.TargetCompletion = FDateTime::Now() + FTimespan::FromDays(DefaultProjectDuration * 7);
    Project.ProgressPercentage = 0.0f;

    // 根據方向設置默認優先級
    switch (Direction)
    {
        case EBMADProjectDirection::Security:
            Project.Priority = EBMADProjectPriority::Critical;
            break;
        case EBMADProjectDirection::Performance:
            Project.Priority = EBMADProjectPriority::High;
            break;
        case EBMADProjectDirection::Architecture:
            Project.Priority = EBMADProjectPriority::High;
            break;
        default:
            Project.Priority = EBMADProjectPriority::Medium;
            break;
    }

    // 設置基礎目標
    Project.Objectives.Add(FString::Printf(TEXT("完成 %s 的核心功能"), *Title));
    Project.Objectives.Add(TEXT("確保系統穩定性和性能"));
    Project.Objectives.Add(TEXT("提供完整的文檔和測試"));

    // 設置關鍵里程碑
    Project.KeyMilestones.Add(TEXT("需求分析和設計完成"));
    Project.KeyMilestones.Add(TEXT("核心功能開發完成"));
    Project.KeyMilestones.Add(TEXT("測試和驗證完成"));
    Project.KeyMilestones.Add(TEXT("部署和上線完成"));

    // 設置所需資源
    Project.RequiredResources.Add(TEXT("開發團隊"));
    Project.RequiredResources.Add(TEXT("測試環境"));
    Project.RequiredResources.Add(TEXT("部署基礎設施"));

    // 設置交付物
    Project.Deliverables.Add(TEXT("功能完整的系統"));
    Project.Deliverables.Add(TEXT("技術文檔"));
    Project.Deliverables.Add(TEXT("用戶手冊"));
    Project.Deliverables.Add(TEXT("測試報告"));

    // 設置成功指標
    Project.SuccessMetrics.Add(TEXT("功能完整性 > 95%"));
    Project.SuccessMetrics.Add(TEXT("性能提升 > 20%"));
    Project.SuccessMetrics.Add(TEXT("用戶滿意度 > 4.0/5.0"));

    // 優化專案計劃
    OptimizeProjectPlan(Project);

    // 保存專案計劃
    ProjectPlans.Add(ProjectID, Project);
    SaveProjectData();

    // 觸發事件
    OnProjectPlanCreated.Broadcast(ProjectID);

    UE_LOG(LogTemp, Log, TEXT("Created project plan: %s (%s)"), *ProjectID, *Title);
    
    return ProjectID;
}

FBMADProjectAnalysis UMingBMADProjectPlanner::AnalyzeProject(const FString& ProjectID)
{
    FBMADProjectAnalysis Analysis;
    Analysis.ProjectID = ProjectID;
    Analysis.AnalysisTime = FDateTime::Now();

    if (!ProjectPlans.Contains(ProjectID))
    {
        UE_LOG(LogTemp, Error, TEXT("Project not found: %s"), *ProjectID);
        return Analysis;
    }

    const FBMADProjectPlan& Project = ProjectPlans[ProjectID];

    // 啟動 BMAD 分析討論
    if (BMADSystem)
    {
        FString DiscussionID = BMADSystem->StartDiscussionSession(
            FString::Printf(TEXT("專案分析: %s"), *Project.Title),
            EBMADDiscussionType::Planning,
            FString::Printf(TEXT("分析專案 %s 的可行性、風險和投資回報率"), *Project.Title)
        );

        // 添加相關專家
        TArray<FString> ExpertAgents = BMADSystem->FindExpertAgents(TEXT("analysis"));
        for (const FString& AgentID : ExpertAgents)
        {
            BMADSystem->AddParticipantToSession(DiscussionID, AgentID);
        }

        // 發送專案信息給專家
        FString ProjectInfo = FString::Printf(TEXT(
            "專案標題: %s\n"
            "描述: %s\n"
            "方向: %s\n"
            "優先級: %s\n"
            "預估時長: %.1f 週\n"
            "目標數量: %d\n"
            "風險因素數量: %d"
        ), 
            *Project.Title,
            *Project.Description,
            *DirectionToString(Project.Direction),
            *PriorityToString(Project.Priority),
            Project.EstimatedDuration,
            Project.Objectives.Num(),
            Project.RiskFactors.Num()
        );

        BMADSystem->SendMessageToSession(DiscussionID, ExpertAgents[0], ProjectInfo, EBMADEmotionState::Neutral);

        // 等待討論完成並提取洞見
        FPlatformProcess::Sleep(2.0f); // 等待討論處理
        Analysis.KeyInsights = BMADSystem->ExtractInsights(DiscussionID);
    }

    // 計算可行性分數
    Analysis.FeasibilityScore = CalculateFeasibilityScore(Project);

    // 計算風險評估
    Analysis.RiskAssessment = CalculateRiskAssessment(Project);

    // 計算投資回報率預測
    Analysis.ROIProjection = CalculateROIProjection(Project);

    // 執行 SWOT 分析
    PerformSWOTAnalysis(Project, Analysis);

    // 生成建議
    GenerateRecommendations(Project, Analysis);

    // 保存分析結果
    ProjectAnalyses.Add(ProjectID, Analysis);
    SaveProjectData();

    // 觸發事件
    OnProjectAnalysisCompleted.Broadcast(ProjectID, Analysis);

    UE_LOG(LogTemp, Log, TEXT("Completed project analysis for %s: Feasibility=%.2f, Risk=%.2f, ROI=%.2f"), 
        *ProjectID, Analysis.FeasibilityScore, Analysis.RiskAssessment, Analysis.ROIProjection);

    return Analysis;
}

FBMADRoadmap UMingBMADProjectPlanner::GenerateRoadmap(const TArray<EBMADProjectDirection>& Directions, float TimeframeMonths)
{
    FBMADRoadmap Roadmap;
    Roadmap.RoadmapID = GenerateUniqueRoadmapID();
    Roadmap.Title = TEXT("MingGoRTS 發展路線圖");
    Roadmap.Description = TEXT("基於 BMAD 系統分析的智能發展路線圖");
    Roadmap.StartTime = FDateTime::Now();
    Roadmap.EndTime = FDateTime::Now() + FTimespan::FromDays(TimeframeMonths * 30);
    Roadmap.CreatedTime = FDateTime::Now();
    Roadmap.VisionStatement = TEXT("將 MingGoRTS 打造成領先的歷史策略遊戲，提供卓越的用戶體驗和技術創新");

    // 設置戰略目標
    Roadmap.StrategicGoals.Add(TEXT("技術架構現代化"));
    Roadmap.StrategicGoals.Add(TEXT("用戶體驗優化"));
    Roadmap.StrategicGoals.Add(TEXT("市場份額擴大"));
    Roadmap.StrategicGoals.Add(TEXT("創新功能開發"));
    Roadmap.StrategicGoals.Add(TEXT("生態系統建設"));

    // 為每個方向生成專案
    float ProjectDuration = TimeframeMonths / Directions.Num();
    for (EBMADProjectDirection Direction : Directions)
    {
        FString ProjectTitle = GenerateProjectTitleForDirection(Direction);
        FString ProjectDescription = GenerateProjectDescriptionForDirection(Direction);
        
        FString ProjectID = CreateProjectPlan(ProjectTitle, ProjectDescription, Direction);
        
        if (ProjectPlans.Contains(ProjectID))
        {
            FBMADProjectPlan& Project = ProjectPlans[ProjectID];
            Project.EstimatedDuration = ProjectDuration;
            
            // 設置時間線
            int32 DirectionIndex = Directions.IndexOf(Direction);
            FDateTime ProjectStart = Roadmap.StartTime + FTimespan::FromDays(DirectionIndex * ProjectDuration * 30);
            FDateTime ProjectEnd = ProjectStart + FTimespan::FromDays(ProjectDuration * 30);
            
            Project.CreatedTime = ProjectStart;
            Project.TargetCompletion = ProjectEnd;
            
            Roadmap.Projects.Add(Project);
        }
    }

    // 計算總預算
    Roadmap.TotalBudget = CalculateTotalRoadmapBudget(Roadmap);

    // 優化路線圖
    OptimizeRoadmap(Roadmap);

    // 保存路線圖
    CurrentRoadmap = Roadmap;
    SaveProjectData();

    // 觸發事件
    OnRoadmapGenerated.Broadcast(Roadmap);

    UE_LOG(LogTemp, Log, TEXT("Generated roadmap with %d projects over %.1f months"), 
        Roadmap.Projects.Num(), TimeframeMonths);

    return Roadmap;
}

FString UMingBMADProjectPlanner::StartDirectionDiscussion(EBMADProjectDirection Direction)
{
    if (!BMADSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("BMAD System is not available"));
        return TEXT("");
    }

    FString DirectionStr = DirectionToString(Direction);
    FString DiscussionTopic = FString::Printf(TEXT("%s 方向規劃討論"), *DirectionStr);
    
    FString DiscussionID = BMADSystem->StartDiscussionSession(
        DiscussionTopic,
        EBMADDiscussionType::Planning,
        FString::Printf(TEXT("討論 MingGoRTS 在 %s 方向的發展策略和機會"), *DirectionStr)
    );

    if (!DiscussionID.IsEmpty())
    {
        // 創建專家討論組
        FString ExpertPanelID = CreateExpertPanelForDirection(Direction);
        
        // 發送啟動消息
        FString ContextMessage = FString::Printf(TEXT(
            "基於當前系統狀態分析：\n"
            "優勢: %s\n"
            "劣勢: %s\n"
            "機會: %s\n"
            "威脅: %s\n\n"
            "請專家組討論 %s 方向的具體實施策略。"
        ), 
            *FString::Join(SystemStrengths, TEXT(", ")),
            *FString::Join(SystemWeaknesses, TEXT(", ")),
            *FString::Join(SystemOpportunities, TEXT(", ")),
            *FString::Join(SystemThreats, TEXT(", ")),
            *DirectionStr
        );

        BMADSystem->SendMessageToSession(DiscussionID, TEXT("COORDINATOR"), ContextMessage, EBMADEmotionState::Analytical);

        // 觸發事件
        OnDirectionDiscussionStarted.Broadcast(DiscussionID, Direction);
    }

    return DiscussionID;
}

TArray<FBMADProjectPlan> UMingBMADProjectPlanner::GetProjectRecommendations()
{
    TArray<FBMADProjectPlan> Recommendations;
    
    // 分析當前路線圖
    if (CurrentRoadmap.Projects.Num() > 0)
    {
        // 基於路線圖推薦專案
        for (const FBMADProjectPlan& Project : CurrentRoadmap.Projects)
        {
            if (Project.Status == EBMADProjectStatus::Planning || Project.Status == EBMADProjectStatus::InProgress)
            {
                Recommendations.Add(Project);
            }
        }
    }
    else
    {
        // 生成基於系統狀態的推薦專案
        TArray<EBMADProjectDirection> PriorityDirections = IdentifyProjectOpportunities();
        
        for (EBMADProjectDirection Direction : PriorityDirections)
        {
            FString Title = GenerateProjectTitleForDirection(Direction);
            FString Description = GenerateProjectDescriptionForDirection(Direction);
            
            FBMADProjectPlan RecommendedProject;
            RecommendedProject.ProjectID = GenerateUniqueProjectID();
            RecommendedProject.Title = Title;
            RecommendedProject.Description = Description;
            RecommendedProject.Direction = Direction;
            RecommendedProject.Priority = EvaluateDirectionPriority(Direction);
            RecommendedProject.Status = EBMADProjectStatus::Planning;
            
            Recommendations.Add(RecommendedProject);
        }
    }

    // 按優先級排序
    Recommendations.Sort([](const FBMADProjectPlan& A, const FBMADProjectPlan& B)
    {
        return static_cast<int32>(A.Priority) < static_cast<int32>(B.Priority);
    });

    return Recommendations;
}

EBMADProjectPriority UMingBMADProjectPlanner::EvaluateProjectPriority(const FString& ProjectID)
{
    if (!ProjectPlans.Contains(ProjectID))
    {
        return EBMADProjectPriority::Medium;
    }

    const FBMADProjectPlan& Project = ProjectPlans[ProjectID];
    
    // 如果已有分析結果，使用分析結果
    if (ProjectAnalyses.Contains(ProjectID))
    {
        const FBMADProjectAnalysis& Analysis = ProjectAnalyses[ProjectID];
        
        // 基於風險和 ROI 評估優先級
        if (Analysis.RiskAssessment > 0.7f)
        {
            return EBMADProjectPriority::Critical;
        }
        else if (Analysis.ROIProjection > 0.8f)
        {
            return EBMADProjectPriority::High;
        }
        else if (Analysis.FeasibilityScore > 0.6f)
        {
            return EBMADProjectPriority::Medium;
        }
        else
        {
            return EBMADProjectPriority::Low;
        }
    }

    // 基於方向評估優先級
    return EvaluateDirectionPriority(Project.Direction);
}

bool UMingBMADProjectPlanner::UpdateProjectProgress(const FString& ProjectID, float Progress)
{
    if (!ProjectPlans.Contains(ProjectID))
    {
        UE_LOG(LogTemp, Error, TEXT("Project not found: %s"), *ProjectID);
        return false;
    }

    FBMADProjectPlan& Project = ProjectPlans[ProjectID];
    Project.ProgressPercentage = FMath::Clamp(Progress, 0.0f, 100.0f);
    
    // 更新狀態
    if (Project.ProgressPercentage >= 100.0f)
    {
        Project.Status = EBMADProjectStatus::Completed;
    }
    else if (Project.ProgressPercentage > 0.0f)
    {
        Project.Status = EBMADProjectStatus::InProgress;
    }

    SaveProjectData();

    UE_LOG(LogTemp, Log, TEXT("Updated project progress: %s = %.1f%%"), *ProjectID, Project.ProgressPercentage);
    
    return true;
}

FBMADRoadmap UMingBMADProjectPlanner::GetCurrentRoadmap()
{
    return CurrentRoadmap;
}

FString UMingBMADProjectPlanner::GenerateProjectReport(const FString& ProjectID)
{
    if (!ProjectPlans.Contains(ProjectID))
    {
        UE_LOG(LogTemp, Error, TEXT("Project not found: %s"), *ProjectID);
        return TEXT("");
    }

    const FBMADProjectPlan& Project = ProjectPlans[ProjectID];
    
    FString ReportContent = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingGoRTS 專案報告</title>\n");
    ReportContent += TEXT("<meta charset=\"UTF-8\">\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: 'Microsoft JhengHei', Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px; margin-bottom: 30px; text-align: center; }\n");
    ReportContent += TEXT(".section { background: white; padding: 25px; border-radius: 10px; margin-bottom: 25px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".progress-bar { width: 100%; height: 20px; background-color: #e0e0e0; border-radius: 10px; overflow: hidden; }\n");
    ReportContent += TEXT(".progress-fill { height: 100%; background-color: #4CAF50; transition: width 0.3s ease; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 15px; }\n");
    ReportContent += TEXT("th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; font-weight: bold; }\n");
    ReportContent += TEXT("</style>\n</head>\n<body>\n");

    // 頁眉
    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += FString::Printf(TEXT("<h1>%s</h1>\n"), *Project.Title);
    ReportContent += FString::Printf(TEXT("<p>生成時間: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += TEXT("</div>\n");

    // 專案概覽
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>📋 專案概覽</h2>\n");
    ReportContent += FString::Printf(TEXT("<p><strong>描述:</strong> %s</p>\n"), *Project.Description);
    ReportContent += FString::Printf(TEXT("<p><strong>方向:</strong> %s</p>\n"), *DirectionToString(Project.Direction));
    ReportContent += FString::Printf(TEXT("<p><strong>優先級:</strong> %s</p>\n"), *PriorityToString(Project.Priority));
    ReportContent += FString::Printf(TEXT("<p><strong>狀態:</strong> %s</p>\n"), *StatusToString(Project.Status));
    ReportContent += FString::Printf(TEXT("<p><strong>創建時間:</strong> %s</p>\n"), *Project.CreatedTime.ToString());
    ReportContent += FString::Printf(TEXT("<p><strong>目標完成時間:</strong> %s</p>\n"), *Project.TargetCompletion.ToString());
    ReportContent += TEXT("</div>\n");

    // 進度信息
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>📊 進度信息</h2>\n");
    ReportContent += FString::Printf(TEXT("<p><strong>進度:</strong> %.1f%%</p>\n"), Project.ProgressPercentage);
    ReportContent += TEXT("<div class=\"progress-bar\">\n");
    ReportContent += FString::Printf(TEXT("<div class=\"progress-fill\" style=\"width: %.1f%%\"></div>\n"), Project.ProgressPercentage);
    ReportContent += TEXT("</div>\n");
    ReportContent += FString::Printf(TEXT("<p><strong>預估時長:</strong> %.1f 週</p>\n"), Project.EstimatedDuration);
    ReportContent += FString::Printf(TEXT("<p><strong>預算分配:</strong> $%.2f</p>\n"), Project.BudgetAllocation);
    ReportContent += TEXT("</div>\n");

    // 目標和里程碑
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>🎯 目標和里程碑</h2>\n");
    ReportContent += TEXT("<h3>目標</h3>\n<ul>\n");
    for (const FString& Objective : Project.Objectives)
    {
        ReportContent += FString::Printf(TEXT("<li>%s</li>\n"), *Objective);
    }
    ReportContent += TEXT("</ul>\n<h3>關鍵里程碑</h3>\n<ul>\n");
    for (const FString& Milestone : Project.KeyMilestones)
    {
        ReportContent += FString::Printf(TEXT("<li>%s</li>\n"), *Milestone);
    }
    ReportContent += TEXT("</ul>\n</div>\n");

    // 資源和風險
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>🔧 資源和風險</h2>\n");
    ReportContent += TEXT("<h3>所需資源</h3>\n<ul>\n");
    for (const FString& Resource : Project.RequiredResources)
    {
        ReportContent += FString::Printf(TEXT("<li>%s</li>\n"), *Resource);
    }
    ReportContent += TEXT("</ul>\n<h3>風險因素</h3>\n<ul>\n");
    for (const FString& Risk : Project.RiskFactors)
    {
        ReportContent += FString::Printf(TEXT("<li>%s</li>\n"), *Risk);
    }
    ReportContent += TEXT("</ul>\n</div>\n");

    // 分析結果
    if (ProjectAnalyses.Contains(ProjectID))
    {
        const FBMADProjectAnalysis& Analysis = ProjectAnalyses[ProjectID];
        
        ReportContent += TEXT("<div class=\"section\">\n");
        ReportContent += TEXT("<h2>📈 分析結果</h2>\n");
        ReportContent += FString::Printf(TEXT("<p><strong>可行性分數:</strong> %.2f/1.00</p>\n"), Analysis.FeasibilityScore);
        ReportContent += FString::Printf(TEXT("<p><strong>風險評估:</strong> %.2f/1.00</p>\n"), Analysis.RiskAssessment);
        ReportContent += FString::Printf(TEXT("<p><strong>投資回報率預測:</strong> %.2f%%</p>\n"), Analysis.ROIProjection * 100);
        
        if (Analysis.Recommendations.Num() > 0)
        {
            ReportContent += TEXT("<h3>建議</h3>\n<ul>\n");
            for (const FString& Recommendation : Analysis.Recommendations)
            {
                ReportContent += FString::Printf(TEXT("<li>%s</li>\n"), *Recommendation);
            }
            ReportContent += TEXT("</ul>\n");
        }
        
        ReportContent += TEXT("</div>\n");
    }

    ReportContent += TEXT("</body>\n</html>");

    return ReportContent;
}

FString UMingBMADProjectPlanner::ExportRoadmap(const FString& RoadmapID)
{
    FBMADRoadmap RoadmapToExport;
    
    if (RoadmapID.IsEmpty() || RoadmapID == CurrentRoadmap.RoadmapID)
    {
        RoadmapToExport = CurrentRoadmap;
    }
    else
    {
        // 如果指定了特定路線圖ID，從存儲中查找（這裡簡化處理）
        RoadmapToExport = CurrentRoadmap;
    }

    // 生成 JSON 格式的路線圖
    TSharedPtr<FJsonObject> RoadmapObject = MakeShareable(new FJsonObject);
    
    RoadmapObject->SetStringField(TEXT("RoadmapID"), RoadmapToExport.RoadmapID);
    RoadmapObject->SetStringField(TEXT("Title"), RoadmapToExport.Title);
    RoadmapObject->SetStringField(TEXT("Description"), RoadmapToExport.Description);
    RoadmapObject->SetStringField(TEXT("VisionStatement"), RoadmapToExport.VisionStatement);
    RoadmapObject->SetStringField(TEXT("StartTime"), RoadmapToExport.StartTime.ToString());
    RoadmapObject->SetStringField(TEXT("EndTime"), RoadmapToExport.EndTime.ToString());
    RoadmapObject->SetNumberField(TEXT("TotalBudget"), RoadmapToExport.TotalBudget);
    
    // 添加戰略目標
    TArray<TSharedPtr<FJsonValue>> GoalsArray;
    for (const FString& Goal : RoadmapToExport.StrategicGoals)
    {
        GoalsArray.Add(MakeShareable(new FJsonValueString(Goal)));
    }
    RoadmapObject->SetArrayField(TEXT("StrategicGoals"), GoalsArray);
    
    // 添加專案
    TArray<TSharedPtr<FJsonValue>> ProjectsArray;
    for (const FBMADProjectPlan& Project : RoadmapToExport.Projects)
    {
        TSharedPtr<FJsonObject> ProjectObject = MakeShareable(new FJsonObject);
        ProjectObject->SetStringField(TEXT("ProjectID"), Project.ProjectID);
        ProjectObject->SetStringField(TEXT("Title"), Project.Title);
        ProjectObject->SetStringField(TEXT("Description"), Project.Description);
        ProjectObject->SetNumberField(TEXT("Direction"), static_cast<int32>(Project.Direction));
        ProjectObject->SetNumberField(TEXT("Priority"), static_cast<int32>(Project.Priority));
        ProjectObject->SetNumberField(TEXT("Status"), static_cast<int32>(Project.Status));
        ProjectObject->SetNumberField(TEXT("ProgressPercentage"), Project.ProgressPercentage);
        ProjectObject->SetNumberField(TEXT("EstimatedDuration"), Project.EstimatedDuration);
        ProjectObject->SetNumberField(TEXT("BudgetAllocation"), Project.BudgetAllocation);
        ProjectObject->SetStringField(TEXT("CreatedTime"), Project.CreatedTime.ToString());
        ProjectObject->SetStringField(TEXT("TargetCompletion"), Project.TargetCompletion.ToString());
        
        ProjectsArray.Add(MakeShareable(new FJsonValueObject(ProjectObject)));
    }
    RoadmapObject->SetArrayField(TEXT("Projects"), ProjectsArray);
    
    // 序列化為字符串
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RoadmapObject.ToSharedRef(), Writer);
    
    return OutputString;
}

void UMingBMADProjectPlanner::SyncProjectsWithSystemState()
{
    UE_LOG(LogTemp, Log, TEXT("Syncing projects with system state..."));

    // 分析當前系統狀態
    AnalyzeCurrentSystemState();

    // 更新專案狀態
    for (auto& ProjectPair : ProjectPlans)
    {
        FBMADProjectPlan& Project = ProjectPair.Value;
        
        // 根據系統狀態調整專案優先級
        EBMADProjectPriority NewPriority = EvaluateProjectPriority(Project.ProjectID);
        if (Project.Priority != NewPriority)
        {
            UE_LOG(LogTemp, Log, TEXT("Updated project priority: %s from %s to %s"), 
                *Project.ProjectID, *PriorityToString(Project.Priority), *PriorityToString(NewPriority));
            Project.Priority = NewPriority;
        }
        
        // 檢查專案依賴
        CheckProjectDependencies(Project);
        
        // 更新風險評估
        UpdateProjectRiskAssessment(Project);
    }

    // 保存更新後的數據
    SaveProjectData();

    UE_LOG(LogTemp, Log, TEXT("Project synchronization completed"));
}

// 私有方法實現

void UMingBMADProjectPlanner::AnalyzeCurrentSystemState()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing current system state..."));

    SystemStrengths.Empty();
    SystemWeaknesses.Empty();
    SystemOpportunities.Empty();
    SystemThreats.Empty();

    // 分析系統優勢
    SystemStrengths.Add(TEXT("完整的 C++ API 系統"));
    SystemStrengths.Add(TEXT("強大的 BMAD 多AI討論系統"));
    SystemStrengths.Add(TEXT("自動化任務分析能力"));
    SystemStrengths.Add(TEXT("模組化系統架構"));
    SystemStrengths.Add(TEXT("全面的測試覆蓋"));

    // 分析系統劣勢
    SystemWeaknesses.Add(TEXT("部分系統集成不完整"));
    SystemWeaknesses.Add(TEXT("性能優化空間較大"));
    SystemWeaknesses.Add(TEXT("用戶文檔需要完善"));
    SystemWeaknesses.Add(TEXT("國際化支持有限"));
    SystemWeaknesses.Add(TEXT("多線程安全性需要加強"));

    // 分析機會
    SystemOpportunities.Add(TEXT("AI 技術快速發展"));
    SystemOpportunities.Add(TEXT("雲端服務需求增長"));
    SystemOpportunities.Add(TEXT("移動設備普及"));
    SystemOpportunities.Add(TEXT("遊戲市場擴大"));
    SystemOpportunities.Add(TEXT("開源生態繁榮"));

    // 分析威脅
    SystemThreats.Add(TEXT("技術競爭激烈"));
    SystemThreats.Add(TEXT("用戶需求變化快"));
    SystemThreats.Add(TEXT("安全風險增加"));
    SystemThreats.Add(TEXT("法規要求嚴格"));
    SystemThreats.Add(TEXT("人才短缺"));

    UE_LOG(LogTemp, Log, TEXT("System state analysis completed: %d strengths, %d weaknesses, %d opportunities, %d threats"), 
        SystemStrengths.Num(), SystemWeaknesses.Num(), SystemOpportunities.Num(), SystemThreats.Num());
}

TArray<EBMADProjectDirection> UMingBMADProjectPlanner::IdentifyProjectOpportunities()
{
    TArray<EBMADProjectDirection> Opportunities;

    // 基於系統分析識別機會
    if (SystemWeaknesses.Contains(TEXT("性能優化空間較大")))
    {
        Opportunities.Add(EBMADProjectDirection::Performance);
    }
    
    if (SystemWeaknesses.Contains(TEXT("部分系統集成不完整")))
    {
        Opportunities.Add(EBMADProjectDirection::Architecture);
    }
    
    if (SystemWeaknesses.Contains(TEXT("安全風險增加")))
    {
        Opportunities.Add(EBMADProjectDirection::Security);
    }
    
    if (SystemWeaknesses.Contains(TEXT("用戶文檔需要完善")))
    {
        Opportunities.Add(EBMADProjectDirection::UserExperience);
    }
    
    if (SystemOpportunities.Contains(TEXT("AI 技術快速發展")))
    {
        Opportunities.Add(EBMADProjectDirection::Innovation);
    }
    
    if (SystemOpportunities.Contains(TEXT("遊戲市場擴大")))
    {
        Opportunities.Add(EBMADProjectDirection::MarketExpansion);
    }
    
    if (SystemOpportunities.Contains(TEXT("雲端服務需求增長")))
    {
        Opportunities.Add(EBMADProjectDirection::TechnologyUpgrade);
    }

    // 確保至少有一些基本方向
    if (Opportunities.Num() == 0)
    {
        Opportunities.Add(EBMADProjectDirection::Architecture);
        Opportunities.Add(EBMADProjectDirection::Performance);
        Opportunities.Add(EBMADProjectDirection::UserExperience);
    }

    return Opportunities;
}

FString UMingBMADProjectPlanner::CreateExpertPanelForDirection(EBMADProjectDirection Direction)
{
    if (!BMADSystem)
    {
        return TEXT("");
    }

    TArray<FString> ExpertAgents;
    
    // 根據方向選擇相關專家
    switch (Direction)
    {
        case EBMADProjectDirection::Architecture:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("architecture"));
            break;
        case EBMADProjectDirection::Performance:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("performance"));
            break;
        case EBMADProjectDirection::Security:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("security"));
            break;
        case EBMADProjectDirection::UserExperience:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("design"));
            break;
        case EBMADProjectDirection::ContentExpansion:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("content"));
            break;
        case EBMADProjectDirection::TechnologyUpgrade:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("development"));
            break;
        case EBMADProjectDirection::MarketExpansion:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("analysis"));
            break;
        case EBMADProjectDirection::Innovation:
            ExpertAgents = BMADSystem->FindExpertAgents(TEXT("innovation"));
            break;
        default:
            ExpertAgents.Add(TEXT("ARCH_001"));
            ExpertAgents.Add(TEXT("DEV_001"));
            ExpertAgents.Add(TEXT("TEST_001"));
            break;
    }

    // 如果沒有找到足夠的專家，添加默認專家
    if (ExpertAgents.Num() < 3)
    {
        ExpertAgents.Add(TEXT("ARCH_001"));
        ExpertAgents.Add(TEXT("DEV_001"));
        ExpertAgents.Add(TEXT("TEST_001"));
    }

    FString PanelID = BMADSystem->CreateExpertDiscussionGroup(
        ExpertAgents,
        FString::Printf(TEXT("%s 專家組"), *DirectionToString(Direction))
    );

    return PanelID;
}

void UMingBMADProjectPlanner::HandleBMADDiscussionResults(const FString& SessionID)
{
    if (!BMADSystem)
    {
        return;
    }

    // 獲取討論會話
    FBMADDiscussionSession Session = BMADSystem->GetDiscussionSession(SessionID);
    
    // 提取洞見
    TArray<FBMADInsight> Insights = BMADSystem->ExtractInsights(SessionID);
    
    // 生成決策
    TArray<FString> Decisions = BMADSystem->GenerateDecisions(SessionID);
    
    // 創建行動項目
    TArray<FString> ActionItems = BMADSystem->CreateActionItems(SessionID);

    UE_LOG(LogTemp, Log, TEXT("BMAD Discussion Results for %s: %d insights, %d decisions, %d action items"), 
        *SessionID, Insights.Num(), Decisions.Num(), ActionItems.Num());

    // 處理洞見和決策
    ProcessInsightsAndDecisions(SessionID, Insights, Decisions, ActionItems);
}

float UMingBMADProjectPlanner::CalculateProjectScore(const FBMADProjectPlan& Project)
{
    float Score = 0.0f;

    // 優先級權重
    switch (Project.Priority)
    {
        case EBMADProjectPriority::Critical:
            Score += 0.3f;
            break;
        case EBMADProjectPriority::High:
            Score += 0.25f;
            break;
        case EBMADProjectPriority::Medium:
            Score += 0.2f;
            break;
        case EBMADProjectPriority::Low:
            Score += 0.15f;
            break;
        case EBMADProjectPriority::Research:
            Score += 0.1f;
            break;
    }

    // 方向重要性
    switch (Project.Direction)
    {
        case EBMADProjectDirection::Security:
            Score += 0.25f;
            break;
        case EBMADProjectDirection::Architecture:
            Score += 0.2f;
            break;
        case EBMADProjectDirection::Performance:
            Score += 0.2f;
            break;
        case EBMADProjectDirection::UserExperience:
            Score += 0.15f;
            break;
        default:
            Score += 0.1f;
            break;
    }

    // 完整性評分
    float CompletenessScore = 0.0f;
    CompletenessScore += Project.Objectives.Num() > 0 ? 0.1f : 0.0f;
    CompletenessScore += Project.KeyMilestones.Num() > 0 ? 0.1f : 0.0f;
    CompletenessScore += Project.RequiredResources.Num() > 0 ? 0.1f : 0.0f;
    CompletenessScore += Project.Deliverables.Num() > 0 ? 0.1f : 0.0f;
    CompletenessScore += Project.SuccessMetrics.Num() > 0 ? 0.1f : 0.0f;

    Score += CompletenessScore;

    return FMath::Clamp(Score, 0.0f, 1.0f);
}

void UMingBMADProjectPlanner::OptimizeProjectPlan(FBMADProjectPlan& Project)
{
    // 優化時長
    if (Project.EstimatedDuration <= 0.0f)
    {
        Project.EstimatedDuration = DefaultProjectDuration;
    }

    // 優化預算
    if (Project.BudgetAllocation <= 0.0f)
    {
        Project.BudgetAllocation = CalculateDefaultBudget(Project);
    }

    // 確保基本目標
    if (Project.Objectives.Num() == 0)
    {
        Project.Objectives.Add(TEXT("完成專案核心功能"));
        Project.Objectives.Add(TEXT("確保質量和穩定性"));
        Project.Objectives.Add(TEXT("提供完整文檔"));
    }

    // 確保基本里程碑
    if (Project.KeyMilestones.Num() == 0)
    {
        Project.KeyMilestones.Add(TEXT("需求分析完成"));
        Project.KeyMilestones.Add(TEXT("開發完成"));
        Project.KeyMilestones.Add(TEXT("測試完成"));
        Project.KeyMilestones.Add(TEXT("部署完成"));
    }

    // 確保基本資源
    if (Project.RequiredResources.Num() == 0)
    {
        Project.RequiredResources.Add(TEXT("開發人員"));
        Project.RequiredResources.Add(TEXT("測試環境"));
        Project.RequiredResources.Add(TEXT("部署工具"));
    }
}

bool UMingBMADProjectPlanner::ValidateProjectFeasibility(const FBMADProjectPlan& Project)
{
    // 檢查基本可行性
    if (Project.Title.IsEmpty() || Project.Description.IsEmpty())
    {
        return false;
    }

    if (Project.EstimatedDuration <= 0.0f)
    {
        return false;
    }

    if (Project.Objectives.Num() == 0)
    {
        return false;
    }

    // 檢查資源可用性
    if (Project.RequiredResources.Num() > 0)
    {
        // 這裡可以添加更複雜的資源可用性檢查
    }

    return true;
}

// 輔助方法實現

FString UMingBMADProjectPlanner::GenerateUniqueProjectID()
{
    static int32 ProjectCounter = 1;
    return FString::Printf(TEXT("PROJ_%04d"), ProjectCounter++);
}

FString UMingBMADProjectPlanner::GenerateUniqueRoadmapID()
{
    static int32 RoadmapCounter = 1;
    return FString::Printf(TEXT("ROADMAP_%04d"), RoadmapCounter++);
}

FString UMingBMADProjectPlanner::DirectionToString(EBMADProjectDirection Direction)
{
    switch (Direction)
    {
        case EBMADProjectDirection::Architecture: return TEXT("架構優化");
        case EBMADProjectDirection::Performance: return TEXT("性能提升");
        case EBMADProjectDirection::Security: return TEXT("安全強化");
        case EBMADProjectDirection::UserExperience: return TEXT("用戶體驗");
        case EBMADProjectDirection::ContentExpansion: return TEXT("內容擴展");
        case EBMADProjectDirection::TechnologyUpgrade: return TEXT("技術升級");
        case EBMADProjectDirection::MarketExpansion: return TEXT("市場擴展");
        case EBMADProjectDirection::Innovation: return TEXT("創新研發");
        default: return TEXT("未知方向");
    }
}

EBMADProjectDirection UMingBMADProjectPlanner::StringToDirection(const FString& DirectionStr)
{
    if (DirectionStr == TEXT("架構優化")) return EBMADProjectDirection::Architecture;
    if (DirectionStr == TEXT("性能提升")) return EBMADProjectDirection::Performance;
    if (DirectionStr == TEXT("安全強化")) return EBMADProjectDirection::Security;
    if (DirectionStr == TEXT("用戶體驗")) return EBMADProjectDirection::UserExperience;
    if (DirectionStr == TEXT("內容擴展")) return EBMADProjectDirection::ContentExpansion;
    if (DirectionStr == TEXT("技術升級")) return EBMADProjectDirection::TechnologyUpgrade;
    if (DirectionStr == TEXT("市場擴展")) return EBMADProjectDirection::MarketExpansion;
    if (DirectionStr == TEXT("創新研發")) return EBMADProjectDirection::Innovation;
    
    return EBMADProjectDirection::Architecture;
}

FString UMingBMADProjectPlanner::PriorityToString(EBMADProjectPriority Priority)
{
    switch (Priority)
    {
        case EBMADProjectPriority::Critical: return TEXT("關鍵");
        case EBMADProjectPriority::High: return TEXT("高");
        case EBMADProjectPriority::Medium: return TEXT("中");
        case EBMADProjectPriority::Low: return TEXT("低");
        case EBMADProjectPriority::Research: return TEXT("研究");
        default: return TEXT("未知");
    }
}

FString UMingBMADProjectPlanner::StatusToString(EBMADProjectStatus Status)
{
    switch (Status)
    {
        case EBMADProjectStatus::Planning: return TEXT("規劃中");
        case EBMADProjectStatus::InProgress: return TEXT("進行中");
        case EBMADProjectStatus::Review: return TEXT("審查中");
        case EBMADProjectStatus::Completed: return TEXT("已完成");
        case EBMADProjectStatus::Paused: return TEXT("暫停");
        case EBMADProjectStatus::Cancelled: return TEXT("取消");
        default: return TEXT("未知");
    }
}

FString UMingBMADProjectPlanner::GenerateProjectTitleForDirection(EBMADProjectDirection Direction)
{
    switch (Direction)
    {
        case EBMADProjectDirection::Architecture:
            return TEXT("系統架構現代化升級");
        case EBMADProjectDirection::Performance:
            return TEXT("系統性能優化計劃");
        case EBMADProjectDirection::Security:
            return TEXT("安全防護體系強化");
        case EBMADProjectDirection::UserExperience:
            return TEXT("用戶體驗全面提升");
        case EBMADProjectDirection::ContentExpansion:
            return TEXT("遊戲內容擴展計劃");
        case EBMADProjectDirection::TechnologyUpgrade:
            return TEXT("核心技術升級項目");
        case EBMADProjectDirection::MarketExpansion:
            return TEXT("市場擴張戰略項目");
        case EBMADProjectDirection::Innovation:
            return TEXT("創新功能研發計劃");
        default:
            return TEXT("新專案計劃");
    }
}

FString UMingBMADProjectPlanner::GenerateProjectDescriptionForDirection(EBMADProjectDirection Direction)
{
    switch (Direction)
    {
        case EBMADProjectDirection::Architecture:
            return TEXT("升級系統架構，提高可擴展性、可維護性和性能，支持未來業務發展需求");
        case EBMADProjectDirection::Performance:
            return TEXT("全面優化系統性能，包括響應時間、吞吐量、資源使用率等關鍵指標");
        case EBMADProjectDirection::Security:
            return TEXT("加強安全防護措施，建立完善的安全體系，保護系統和用戶數據安全");
        case EBMADProjectDirection::UserExperience:
            return TEXT("優化用戶界面和交互體驗，提高用戶滿意度和使用效率");
        case EBMADProjectDirection::ContentExpansion:
            return TEXT("擴展遊戲內容，增加新的遊戲模式、角色、場景和功能，提升用戶體驗");
        case EBMADProjectDirection::TechnologyUpgrade:
            return TEXT("升級核心技术，採用先進的技術框架和工具，提高開發效率和系統性能");
        case EBMADProjectDirection::MarketExpansion:
            return TEXT("拓展市場份額，開發新功能吸引更多用戶，提升商業價值");
        case EBMADProjectDirection::Innovation:
            return TEXT("研發創新功能，引入AI、VR/AR等新技術，提供獨特的用戶體驗");
        default:
            return TEXT("新專案描述");
    }
}

EBMADProjectPriority UMingBMADProjectPlanner::EvaluateDirectionPriority(EBMADProjectDirection Direction)
{
    switch (Direction)
    {
        case EBMADProjectDirection::Security:
            return EBMADProjectPriority::Critical;
        case EBMADProjectDirection::Architecture:
            return EBMADProjectPriority::High;
        case EBMADProjectDirection::Performance:
            return EBMADProjectPriority::High;
        case EBMADProjectDirection::UserExperience:
            return EBMADProjectPriority::Medium;
        case EBMADProjectDirection::Innovation:
            return EBMADProjectPriority::Medium;
        case EBMADProjectDirection::ContentExpansion:
            return EBMADProjectPriority::Medium;
        case EBMADProjectDirection::TechnologyUpgrade:
            return EBMADProjectPriority::Low;
        case EBMADProjectDirection::MarketExpansion:
            return EBMADProjectPriority::Low;
        default:
            return EBMADProjectPriority::Medium;
    }
}

float UMingBMADProjectPlanner::CalculateDefaultBudget(const FBMADProjectPlan& Project)
{
    float BaseBudget = 10000.0f; // 基礎預算
    
    // 根據優先級調整
    switch (Project.Priority)
    {
        case EBMADProjectPriority::Critical:
            BaseBudget *= 2.0f;
            break;
        case EBMADProjectPriority::High:
            BaseBudget *= 1.5f;
            break;
        case EBMADProjectPriority::Medium:
            BaseBudget *= 1.0f;
            break;
        case EBMADProjectPriority::Low:
            BaseBudget *= 0.7f;
            break;
        case EBMADProjectPriority::Research:
            BaseBudget *= 0.5f;
            break;
    }
    
    // 根據時長調整
    BaseBudget *= Project.EstimatedDuration / 4.0f; // 基於4週標準
    
    return BaseBudget;
}

void UMingBMADProjectPlanner::SaveProjectData()
{
    // 保存專案數據到文件
    FString ProjectDataPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Saved/BMADProjectData.json"));
    
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    // 保存專案計劃
    TArray<TSharedPtr<FJsonValue>> ProjectsArray;
    for (const auto& ProjectPair : ProjectPlans)
    {
        // 這裡可以添加更複雜的序列化邏輯
    }
    RootObject->SetArrayField(TEXT("ProjectPlans"), ProjectsArray);
    
    // 保存路線圖
    if (!CurrentRoadmap.RoadmapID.IsEmpty())
    {
        // 保存當前路線圖
    }
    
    // 序列化並保存
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    FFileHelper::SaveStringToFile(OutputString, *ProjectDataPath);
}

void UMingBMADProjectPlanner::LoadProjectData()
{
    // 從文件加載專案數據
    FString ProjectDataPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Saved/BMADProjectData.json"));
    
    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *ProjectDataPath))
    {
        // 解析 JSON 並恢復數據
        TSharedPtr<FJsonObject> RootObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);
        
        if (FJsonSerializer::Deserialize(RootObject, Reader))
        {
            // 恢復專案計劃
            // 恢復路線圖
        }
    }
}

void UMingBMADProjectPlanner::CleanupOldProjects()
{
    // 清理過期的專案數據
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan ExpiryPeriod = FTimespan::FromDays(365); // 1年過期
    
    TArray<FString> ProjectsToRemove;
    
    for (auto& ProjectPair : ProjectPlans)
    {
        const FBMADProjectPlan& Project = ProjectPair.Value;
        if (CurrentTime - Project.CreatedTime > ExpiryPeriod)
        {
            ProjectsToRemove.Add(ProjectPair.Key);
        }
    }
    
    for (const FString& ProjectID : ProjectsToRemove)
    {
        ProjectPlans.Remove(ProjectID);
        ProjectAnalyses.Remove(ProjectID);
    }
    
    if (ProjectsToRemove.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Cleaned up %d expired projects"), ProjectsToRemove.Num());
        SaveProjectData();
    }
}

// 事件處理
void UMingBMADProjectPlanner::OnDirectionDiscussionStarted(const FString& DiscussionID, EBMADProjectDirection Direction)
{
    UE_LOG(LogTemp, Log, TEXT("Direction discussion started: %s for %s"), *DiscussionID, *DirectionToString(Direction));
}

void UMingBMADProjectPlanner::OnBMADInsightGenerated(const FString& SessionID, const FBMADInsight& Insight)
{
    UE_LOG(LogTemp, Log, TEXT("BMAD Insight generated: %s - %s"), *SessionID, *Insight.Content);
    
    // 處理生成的洞見
    ProcessGeneratedInsight(SessionID, Insight);
}

void UMingBMADProjectPlanner::ProcessGeneratedInsight(const FString& SessionID, const FBMADInsight& Insight)
{
    // 根據洞見內容更新相關專案
    if (Insight.Content.Contains(TEXT("架構")))
    {
        UpdateProjectsForDirection(EBMADProjectDirection::Architecture, Insight);
    }
    else if (Insight.Content.Contains(TEXT("性能")))
    {
        UpdateProjectsForDirection(EBMADProjectDirection::Performance, Insight);
    }
    else if (Insight.Content.Contains(TEXT("安全")))
    {
        UpdateProjectsForDirection(EBMADProjectDirection::Security, Insight);
    }
    // ... 其他方向的處理
}

void UMingBMADProjectPlanner::UpdateProjectsForDirection(EBMADProjectDirection Direction, const FBMADInsight& Insight)
{
    for (auto& ProjectPair : ProjectPlans)
    {
        FBMADProjectPlan& Project = ProjectPair.Value;
        if (Project.Direction == Direction)
        {
            // 根據洞見調整專案
            if (Insight.Importance > 0.8f)
            {
                Project.Priority = EBMADProjectPriority::High;
            }
            
            // 添加洞見到專案說明
            Project.Description += FString::Printf(TEXT("\n\n洞見: %s"), *Insight.Content);
        }
    }
}

// 其他輔助方法的實現
float UMingBMADProjectPlanner::CalculateFeasibilityScore(const FBMADProjectPlan& Project)
{
    float Score = 0.5f; // 基礎分數
    
    // 根據目標數量調整
    Score += FMath::Clamp(Project.Objectives.Num() * 0.1f, 0.0f, 0.3f);
    
    // 根據資源可用性調整
    Score += FMath::Clamp(Project.RequiredResources.Num() * 0.05f, 0.0f, 0.2f);
    
    // 根據時長合理性調整
    if (Project.EstimatedDuration > 0.0f && Project.EstimatedDuration <= 12.0f)
    {
        Score += 0.1f;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingBMADProjectPlanner::CalculateRiskAssessment(const FBMADProjectPlan& Project)
{
    float Risk = 0.3f; // 基礎風險
    
    // 根據複雜度調整
    if (Project.EstimatedDuration > 8.0f)
    {
        Risk += 0.2f;
    }
    
    // 根據風險因素數量調整
    Risk += FMath::Clamp(Project.RiskFactors.Num() * 0.1f, 0.0f, 0.3f);
    
    // 根據資源需求調整
    if (Project.RequiredResources.Num() > 5)
    {
        Risk += 0.1f;
    }
    
    return FMath::Clamp(Risk, 0.0f, 1.0f);
}

float UMingBMADProjectPlanner::CalculateROIProjection(const FBMADProjectPlan& Project)
{
    float ROI = 0.5f; // 基礎 ROI
    
    // 根據優先級調整
    switch (Project.Priority)
    {
        case EBMADProjectPriority::Critical:
            ROI += 0.3f;
            break;
        case EBMADProjectPriority::High:
            ROI += 0.2f;
            break;
        case EBMADProjectPriority::Medium:
            ROI += 0.1f;
            break;
        default:
            break;
    }
    
    // 根據方向調整
    switch (Project.Direction)
    {
        case EBMADProjectDirection::MarketExpansion:
            ROI += 0.2f;
            break;
        case EBMADProjectDirection::Innovation:
            ROI += 0.15f;
            break;
        case EBMADProjectDirection::UserExperience:
            ROI += 0.1f;
            break;
        default:
            break;
    }
    
    return FMath::Clamp(ROI, 0.0f, 1.0f);
}

void UMingBMADProjectPlanner::PerformSWOTAnalysis(const FBMADProjectPlan& Project, FBMADProjectAnalysis& Analysis)
{
    // 執行 SWOT 分析
    Analysis.Strengths = SystemStrengths;
    Analysis.Weaknesses = SystemWeaknesses;
    Analysis.Opportunities = SystemOpportunities;
    Analysis.Threats = SystemThreats;
    
    // 根據專案特點調整
    if (Project.Direction == EBMADProjectDirection::Security)
    {
        Analysis.Threats.Add(TEXT("安全威脅持續增加"));
        Analysis.Opportunities.Add(TEXT("安全需求市場增長"));
    }
}

void UMingBMADProjectPlanner::GenerateRecommendations(const FBMADProjectPlan& Project, FBMADProjectAnalysis& Analysis)
{
    Analysis.Recommendations.Empty();
    
    // 基於分析結果生成建議
    if (Analysis.FeasibilityScore < 0.5f)
    {
        Analysis.Recommendations.Add(TEXT("建議重新評估專案可行性"));
        Analysis.Recommendations.Add(TEXT("考慮簡化專案範圍"));
    }
    
    if (Analysis.RiskAssessment > 0.7f)
    {
        Analysis.Recommendations.Add(TEXT("建議制定詳細風險緩解計劃"));
        Analysis.Recommendations.Add(TEXT("考慮分階段實施"));
    }
    
    if (Analysis.ROIProjection < 0.4f)
    {
        Analysis.Recommendations.Add(TEXT("建議重新評估商業價值"));
        Analysis.Recommendations.Add(TEXT("考慮調整專案目標"));
    }
    
    // 通用建議
    Analysis.Recommendations.Add(TEXT("建議定期監控專案進度"));
    Analysis.Recommendations.Add(TEXT("建議建立有效的溝通機制"));
}

float UMingBMADProjectPlanner::CalculateTotalRoadmapBudget(const FBMADRoadmap& Roadmap)
{
    float TotalBudget = 0.0f;
    
    for (const FBMADProjectPlan& Project : Roadmap.Projects)
    {
        TotalBudget += Project.BudgetAllocation;
    }
    
    return TotalBudget;
}

void UMingBMADProjectPlanner::OptimizeRoadmap(FBMADRoadmap& Roadmap)
{
    // 按優先級排序專案
    Roadmap.Projects.Sort([](const FBMADProjectPlan& A, const FBMADProjectPlan& B)
    {
        return static_cast<int32>(A.Priority) < static_cast<int32>(B.Priority);
    });
    
    // 調整時間線以避免資源衝突
    AdjustTimelineForResourceConflicts(Roadmap);
}

void UMingBMADProjectPlanner::AdjustTimelineForResourceConflicts(FBMADRoadmap& Roadmap)
{
    // 簡化的時間線調整邏輯
    FDateTime CurrentTime = Roadmap.StartTime;
    
    for (FBMADProjectPlan& Project : Roadmap.Projects)
    {
        Project.CreatedTime = CurrentTime;
        Project.TargetCompletion = CurrentTime + FTimespan::FromDays(Project.EstimatedDuration * 7);
        CurrentTime = Project.TargetCompletion + FTimespan::FromDays(7); // 1週緩衝
    }
}

void UMingBMADProjectPlanner::CheckProjectDependencies(FBMADProjectPlan& Project)
{
    // 檢查專案依賴關係
    // 這裡可以添加更複雜的依賴檢查邏輯
}

void UMingBMADProjectPlanner::UpdateProjectRiskAssessment(FBMADProjectPlan& Project)
{
    // 更新專案風險評估
    // 基於當前系統狀態和外部因素重新評估風險
}

void UMingBMADProjectPlanner::ProcessInsightsAndDecisions(const FString& SessionID, const TArray<FBMADInsight>& Insights, const TArray<FString>& Decisions, const TArray<FString>& ActionItems)
{
    // 處理 BMAD 討論的洞見、決策和行動項目
    UE_LOG(LogTemp, Log, TEXT("Processing %d insights, %d decisions, %d action items from session %s"), 
        Insights.Num(), Decisions.Num(), ActionItems.Num(), *SessionID);
    
    // 根據結果更新相關專案
    for (const FBMADInsight& Insight : Insights)
    {
        ProcessGeneratedInsight(SessionID, Insight);
    }
    
    // 處理決策
    for (const FString& Decision : Decisions)
    {
        ProcessDecision(SessionID, Decision);
    }
    
    // 處理行動項目
    for (const FString& ActionItem : ActionItems)
    {
        ProcessActionItem(SessionID, ActionItem);
    }
}

void UMingBMADProjectPlanner::ProcessDecision(const FString& SessionID, const FString& Decision)
{
    // 處理決策
    UE_LOG(LogTemp, Log, TEXT("Processing decision: %s"), *Decision);
    
    // 根據決策內容更新專案
    if (Decision.Contains(TEXT("優先級")))
    {
        // 調整專案優先級
    }
    else if (Decision.Contains(TEXT("範圍")))
    {
        // 調整專案範圍
    }
    else if (Decision.Contains(TEXT("資源")))
    {
        // 調整資源分配
    }
}

void UMingBMADProjectPlanner::ProcessActionItem(const FString& SessionID, const FString& ActionItem)
{
    // 處理行動項目
    UE_LOG(LogTemp, Log, TEXT("Processing action item: %s"), *ActionItem);
    
    // 創建新的專案或更新現有專案
    if (ActionItem.Contains(TEXT("創建")) || ActionItem.Contains(TEXT("建立")))
    {
        // 可能需要創建新專案
    }
    else if (ActionItem.Contains(TEXT("分析")) || ActionItem.Contains(TEXT("評估")))
    {
        // 可能需要分析現有專案
    }
}
