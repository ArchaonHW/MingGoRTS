// MingGoRTS API Orchestrator
// 統合所有從 PowerShell 轉換過來的 C++ API 系統

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "MingRTSScriptIntegrationAPI.h"
#include "MingRTSPotatoAIEnhancedAPI.h"
#include "MingRTSAutoDevelopmentAPI.h"
#include "MingRTSVersionControlAPI.h"
#include "MingRTSAPIOrchestrator.generated.h"

// 整合系統類型
UENUM(BlueprintType)
enum class EIntegratedAPIType : uint8
{
    ScriptIntegration     UMETA(DisplayName = "Script Integration"),
    PotatoAI             UMETA(DisplayName = "Potato AI Generator"),
    UE5Learning          UMETA(DisplayName = "UE5 Learning"),
    AutoDevelopment      UMETA(DisplayName = "Auto Development"),
    VersionControl       UMETA(DisplayName = "Version Control"),
    All                  UMETA(DisplayName = "All Systems")
};

// 系統健康狀態
USTRUCT(BlueprintType)
struct MINGGORTS_API FAPIOrchestratorHealth
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    bool bScriptIntegrationHealthy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    bool bPotatoAIHealthy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    bool bUE5LearningHealthy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    bool bAutoDevelopmentHealthy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    bool bVersionControlHealthy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    int32 OverallHealthScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Health")
    FString HealthReport;

    FAPIOrchestratorHealth()
    {
        bScriptIntegrationHealthy = false;
        bPotatoAIHealthy = false;
        bUE5LearningHealthy = false;
        bAutoDevelopmentHealthy = false;
        bVersionControlHealthy = false;
        OverallHealthScore = 0;
        HealthReport = TEXT("");
    }
};

// 系統執行結果
USTRUCT(BlueprintType)
struct MINGGORTS_API FAPIOrchestratorResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orchestrator Result")
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orchestrator Result")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orchestrator Result")
    TMap<EIntegratedAPIType, FString> SystemResults;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orchestrator Result")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orchestrator Result")
    FDateTime Timestamp;

    FAPIOrchestratorResult()
    {
        bSuccess = false;
        Message = TEXT("");
        SystemResults = TMap<EIntegratedAPIType, FString>();
        ExecutionTime = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

// 委派
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrchestratorInitialized, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemExecuted, EIntegratedAPIType, SystemType, const FString&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrchestratorHealthUpdated, const FAPIOrchestratorHealth&, Health);

/**
 * MingGoRTS API Orchestrator
 * 統合管理所有 PowerShell 轉換後的 C++ API 系統
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|API|Orchestrator")
class MINGGORTS_API UMingRTSAPIOrchestrator : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingRTSAPIOrchestrator();

    // 初始化整合系統
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    bool InitializeOrchestrator();

    // 執行特定整合系統
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecuteSystem(EIntegratedAPIType SystemType, const TMap<FString, FString>& Parameters);

    // 執行所有整合系統
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecuteAllSystems(const TMap<FString, FString>& Parameters);

    // 執行 PowerShell 腳本整合
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecuteScriptIntegration(const TMap<FString, FString>& Parameters);

    // 執行 Potato AI 程式碼生成
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecutePotatoAIGeneration(const FString& Requirement, const TMap<FString, FString>& Parameters);

    // 執行 UE5 學習系統
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecuteUE5Learning(const FString& LearningMode, const TMap<FString, FString>& Parameters);

    // 執行自動化開發系統
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecuteAutoDevelopment(EAutoDevelopmentMode Mode = EAutoDevelopmentMode::Full, EAutoDevelopmentTarget Target = EAutoDevelopmentTarget::Development);

    // 執行版本控制系統
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorResult ExecuteVersionControl(const FString& Operation, const TMap<FString, FString>& Parameters);

    // 系統健康檢查
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FAPIOrchestratorHealth CheckSystemHealth();

    // 取得系統狀態
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    bool IsSystemInitialized(EIntegratedAPIType SystemType);

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    TArray<EIntegratedAPIType> GetInitializedSystems();

    // 設定與配置
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    void SetAutoExecuteOnStart(bool bEnabled) { bAutoExecuteOnStart = bEnabled; }

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    bool GetAutoExecuteOnStart() const { return bAutoExecuteOnStart; }

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    void SetDefaultExecutionMode(const FString& Mode) { DefaultExecutionMode = Mode; }

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    FString GetDefaultExecutionMode() const { return DefaultExecutionMode; }

    // 產生整合報告
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    bool GenerateOrchestratorReport(const FString& ReportPath);

    // 取得子系統參考
    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    UMingRTSScriptIntegrationAPI* GetScriptIntegrationAPI();

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    UMingRTSPotatoAIEnhancedAPI* GetPotatoAIAPI();

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    UMingRTSUE5LearningAPI* GetUE5LearningAPI();

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    UMingRTSAutoDevelopmentAPI* GetAutoDevelopmentAPI();

    UFUNCTION(BlueprintCallable, Category = "API Orchestrator")
    UMingRTSVersionControlAPI* GetVersionControlAPI();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "API Orchestrator")
    FOnOrchestratorInitialized OnOrchestratorInitialized;

    UPROPERTY(BlueprintAssignable, Category = "API Orchestrator")
    FOnSystemExecuted OnSystemExecuted;

    UPROPERTY(BlueprintAssignable, Category = "API Orchestrator")
    FOnOrchestratorHealthUpdated OnOrchestratorHealthUpdated;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // 子系統參考
    UPROPERTY()
    UMingRTSScriptIntegrationAPI* ScriptIntegrationAPI;

    UPROPERTY()
    UMingRTSPotatoAIEnhancedAPI* PotatoAIAPI;

    UPROPERTY()
    UMingRTSUE5LearningAPI* UE5LearningAPI;

    UPROPERTY()
    UMingRTSAutoDevelopmentAPI* AutoDevelopmentAPI;

    UPROPERTY()
    UMingRTSVersionControlAPI* VersionControlAPI;

    // 配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool bAutoExecuteOnStart = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString DefaultExecutionMode = TEXT("Sequential");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    float HealthCheckInterval = 60.0f; // 秒

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool bEnablePerformanceMonitoring = true;

    // 狀態追蹤
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    TMap<EIntegratedAPIType, bool> SystemInitializedStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    FAPIOrchestratorHealth CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float TimeSinceLastHealthCheck;

    // 內部方法
    bool InitializeSubSystems();
    void InitializeScriptIntegration();
    void InitializePotatoAI();
    void InitializeUE5Learning();
    void InitializeAutoDevelopment();
    void InitializeVersionControl();
    
    void UpdateHealthStatus();
    void PerformHealthCheck();
    int32 CalculateHealthScore();
    
    FString GenerateHealthReport();
    FString GetSystemStatusString(EIntegratedAPIType SystemType);
    
    void LogOrchestratorEvent(const FString& Event, const FString& Details);
    void UpdatePerformanceMetrics();
};
