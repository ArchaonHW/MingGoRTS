#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSBMADSystem.h"
#include "MingTaskAnalyzer.h"
#include "MingCompileAPI.h"
#include "MingRTSAPISystem.h"
#include "MingBMADProjectPlanner.generated.h"

/**
 * 專案方向枚舉
 */
UENUM(BlueprintType)
enum class EBMADProjectDirection : uint8
{
    Architecture,        // 架構優化
    Performance,         // 性能提升
    Security,           // 安全強化
    UserExperience,     // 用戶體驗
    ContentExpansion,    // 內容擴展
    TechnologyUpgrade,   // 技術升級
    MarketExpansion,     // 市場擴展
    Innovation          // 創新研發
};

/**
 * 專案優先級枚舉
 */
UENUM(BlueprintType)
enum class EBMADProjectPriority : uint8
{
    Critical,           // 關鍵 - 立即執行
    High,               // 高 - 優先執行
    Medium,             // 中 - 正常執行
    Low,                // 低 - 後續執行
    Research            // 研究階段
};

/**
 * 專案狀態枚舉
 */
UENUM(BlueprintType)
enum class EBMADProjectStatus : uint8
{
    Planning,           // 規劃中
    InProgress,         // 進行中
    Review,             // 審查中
    Completed,          // 已完成
    Paused,             // 暫停
    Cancelled           // 取消
};

/**
 * 專案規劃結構
 */
USTRUCT(BlueprintType)
struct FBMADProjectPlan
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    FString ProjectID;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    FString Title;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    EBMADProjectDirection Direction;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    EBMADProjectPriority Priority;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    EBMADProjectStatus Status;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> Objectives;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> KeyMilestones;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> RequiredResources;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> RiskFactors;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    float EstimatedDuration; // 週

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    float BudgetAllocation;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> AssignedAgents;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    FString DiscussionSessionID;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    FDateTime CreatedTime;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    FDateTime TargetCompletion;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    float ProgressPercentage;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> Deliverables;

    UPROPERTY(BlueprintReadWrite, Category = "Project Plan")
    TArray<FString> SuccessMetrics;

    FBMADProjectPlan()
    {
        ProjectID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Direction = EBMADProjectDirection::Architecture;
        Priority = EBMADProjectPriority::Medium;
        Status = EBMADProjectStatus::Planning;
        Objectives.Empty();
        KeyMilestones.Empty();
        RequiredResources.Empty();
        RiskFactors.Empty();
        EstimatedDuration = 4.0f;
        BudgetAllocation = 0.0f;
        AssignedAgents.Empty();
        DiscussionSessionID = TEXT("");
        CreatedTime = FDateTime::Now();
        TargetCompletion = FDateTime::Now();
        ProgressPercentage = 0.0f;
        Deliverables.Empty();
        SuccessMetrics.Empty();
    }
};

/**
 * 專案分析結果
 */
USTRUCT(BlueprintType)
struct FBMADProjectAnalysis
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    FString ProjectID;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    float FeasibilityScore;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    float RiskAssessment;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    float ROIProjection;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    TArray<FString> Strengths;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    TArray<FString> Weaknesses;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    TArray<FString> Opportunities;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    TArray<FString> Threats;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    TArray<FBMADInsight> KeyInsights;

    UPROPERTY(BlueprintReadOnly, Category = "Project Analysis")
    FDateTime AnalysisTime;

    FBMADProjectAnalysis()
    {
        ProjectID = TEXT("");
        FeasibilityScore = 0.0f;
        RiskAssessment = 0.0f;
        ROIProjection = 0.0f;
        Strengths.Empty();
        Weaknesses.Empty();
        Opportunities.Empty();
        Threats.Empty();
        Recommendations.Empty();
        KeyInsights.Empty();
        AnalysisTime = FDateTime::Now();
    }
};

/**
 * 路線圖結構
 */
USTRUCT(BlueprintType)
struct FBMADRoadmap
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FString RoadmapID;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FString Title;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    TArray<FBMADProjectPlan> Projects;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    TArray<FString> StrategicGoals;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    float TotalBudget;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FString VisionStatement;

    UPROPERTY(BlueprintReadOnly, Category = "Roadmap")
    FDateTime CreatedTime;

    FBMADRoadmap()
    {
        RoadmapID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Projects.Empty();
        StrategicGoals.Empty();
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
        TotalBudget = 0.0f;
        VisionStatement = TEXT("");
        CreatedTime = FDateTime::Now();
    }
};

/**
 * MingGoRTS BMAD 專案規劃器
 * 
 * 使用 BMAD 系統進行智能專案規劃和方向制定
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|BMAD Project Planner")
class MINGGORTS_API UMingBMADProjectPlanner : public UObject
{
    GENERATED_BODY()

public:
    UMingBMADProjectPlanner();

    // 初始化專案規劃器
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    void InitializeProjectPlanner();

    // 啟動 BMAD 系統並開始專案規劃
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    bool StartBMADProjectPlanning();

    // 創建專案計劃
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FString CreateProjectPlan(const FString& Title, const FString& Description, EBMADProjectDirection Direction);

    // 分析專案可行性
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FBMADProjectAnalysis AnalyzeProject(const FString& ProjectID);

    // 生成專案路線圖
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FBMADRoadmap GenerateRoadmap(const TArray<EBMADProjectDirection>& Directions, float TimeframeMonths);

    // 啟動專案方向討論
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FString StartDirectionDiscussion(EBMADProjectDirection Direction);

    // 獲取專案推薦
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    TArray<FBMADProjectPlan> GetProjectRecommendations();

    // 評估專案優先級
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    EBMADProjectPriority EvaluateProjectPriority(const FString& ProjectID);

    // 更新專案進度
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    bool UpdateProjectProgress(const FString& ProjectID, float Progress);

    // 獲取當前路線圖
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FBMADRoadmap GetCurrentRoadmap();

    // 生成專案報告
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FString GenerateProjectReport(const FString& ProjectID);

    // 導出路線圖
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    FString ExportRoadmap(const FString& RoadmapID);

    // 同步專案與系統狀態
    UFUNCTION(BlueprintCallable, Category = "BMAD Project Planner")
    void SyncProjectsWithSystemState();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectPlanCreated, const FString&, ProjectID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectAnalysisCompleted, const FString&, ProjectID, const FBMADProjectAnalysis&, Analysis);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoadmapGenerated, const FBMADRoadmap&, Roadmap);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDirectionDiscussionStarted, const FString&, DiscussionID, EBMADProjectDirection, Direction);

    UPROPERTY(BlueprintAssignable)
    FOnProjectPlanCreated OnProjectPlanCreated;

    UPROPERTY(BlueprintAssignable)
    FOnProjectAnalysisCompleted OnProjectAnalysisCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnRoadmapGenerated OnRoadmapGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnDirectionDiscussionStarted OnDirectionDiscussionStarted;

protected:
    // 分析系統當前狀態
    void AnalyzeCurrentSystemState();

    // 識別專案機會
    TArray<EBMADProjectDirection> IdentifyProjectOpportunities();

    // 創建專家討論組
    FString CreateExpertPanelForDirection(EBMADProjectDirection Direction);

    // 處理 BMAD 討論結果
    void HandleBMADDiscussionResults(const FString& SessionID);

    // 計算專案分數
    float CalculateProjectScore(const FBMADProjectPlan& Project);

    // 優化專案計劃
    void OptimizeProjectPlan(FBMADProjectPlan& Project);

    // 驗證專案可行性
    bool ValidateProjectFeasibility(const FBMADProjectPlan& Project);

private:
    // BMAD 系統實例
    UPROPERTY()
    TObjectPtr<UMingRTSBMADSystem> BMADSystem;

    // 任務分析器
    UPROPERTY()
    TObjectPtr<UMingTaskAnalyzer> TaskAnalyzer;

    // 編譯 API
    UPROPERTY()
    TObjectPtr<UMingCompileAPI> CompileAPI;

    // API 系統
    UPROPERTY()
    TObjectPtr<UMingRTSAPISystem> APISystem;

    // 專案計劃存儲
    UPROPERTY()
    TMap<FString, FBMADProjectPlan> ProjectPlans;

    // 專案分析結果
    UPROPERTY()
    TMap<FString, FBMADProjectAnalysis> ProjectAnalyses;

    // 當前路線圖
    UPROPERTY()
    FBMADRoadmap CurrentRoadmap;

    // 系統狀態分析
    UPROPERTY()
    TArray<FString> SystemStrengths;

    UPROPERTY()
    TArray<FString> SystemWeaknesses;

    UPROPERTY()
    TArray<FString> SystemOpportunities;

    UPROPERTY()
    TArray<FString> SystemThreats;

    // 配置參數
    UPROPERTY()
    bool bBMADSystemEnabled;

    UPROPERTY()
    float MaxConcurrentProjects;

    UPROPERTY()
    float DefaultProjectDuration;

    // 內部方法
    FString GenerateUniqueProjectID();
    FString GenerateUniqueRoadmapID();
    FString DirectionToString(EBMADProjectDirection Direction);
    EBMADProjectDirection StringToDirection(const FString& DirectionStr);
    void SaveProjectData();
    void LoadProjectData();
    void CleanupOldProjects();
};
