#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingOfficeBridge.generated.h"

/**
 * MingGoRTS 办公界面桥接系统
 * 将游戏状态同步到Star-Office-UI像素办公界面
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingOfficeBridge : public UObject
{
    GENERATED_BODY()

public:
    UMingOfficeBridge();

    // ============================================================================
    // 办公状态枚举 (对应Star-Office-UI的6种状态)
    // ============================================================================
    UENUM(BlueprintType)
    enum class EMingOfficeState : uint8
    {
        Idle        UMETA(DisplayName = "idle - 待机"),
        Writing     UMETA(DisplayName = "writing - 记录"),
        Researching UMETA(DisplayName = "researching - 研究"),
        Executing   UMETA(DisplayName = "executing - 执行"),
        Syncing     UMETA(DisplayName = "syncing - 同步"),
        Error       UMETA(DisplayName = "error - 错误")
    };

    // ============================================================================
    // 系统初始化和连接
    // ============================================================================
    
    /** 初始化办公界面桥接 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void InitializeOfficeBridge();

    /** 检查办公界面服务是否可用 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    bool IsOfficeUIServiceAvailable();

    /** 启动本地办公界面服务 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    bool StartOfficeUIService();

    // ============================================================================
    // 状态同步
    // ============================================================================

    /** 更新办公状态 (主接口) */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void UpdateOfficeState(EMingOfficeState NewState, const FString& Description);

    /** 从游戏状态自动映射到办公状态 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SyncGameStateToOffice(const FString& GameStateName);

    // ============================================================================
    // 常用状态快捷方法
    // ============================================================================

    /** 设置为待机状态 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetIdle(const FString& Description = TEXT("待命中"));

    /** 设置为研究状态 (战略规划) */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetResearching(const FString& Description = TEXT("制定战略中"));

    /** 设置为执行状态 (战斗) */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetExecuting(const FString& Description = TEXT("战斗执行中"));

    /** 设置为同步状态 (多人) */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetSyncing(const FString& Description = TEXT("同步多人进度"));

    /** 设置为错误状态 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetError(const FString& Description = TEXT("发现问题，排查中"));

    /** 设置为记录状态 (资源管理) */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetWriting(const FString& Description = TEXT("整理资源数据"));

    // ============================================================================
    // 事件和回调
    // ============================================================================

    /** 办公状态改变事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOfficeStateChanged, EMingOfficeState, OldState, EMingOfficeState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "Ming Office UI")
    FOnOfficeStateChanged OnOfficeStateChanged;

    /** 服务连接状态改变 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServiceConnectionChanged, bool, bIsConnected);
    UPROPERTY(BlueprintAssignable, Category = "Ming Office UI")
    FOnServiceConnectionChanged OnServiceConnectionChanged;

    // ============================================================================
    // 配置和URL
    // ============================================================================

    /** 获取办公界面URL */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    FString GetOfficeUIUrl() const { return OfficeUIBaseUrl; }

    /** 设置办公界面服务地址 */
    UFUNCTION(BlueprintCallable, Category = "Ming Office UI")
    void SetOfficeUIBaseUrl(const FString& NewUrl) { OfficeUIBaseUrl = NewUrl; }

protected:
    /** 办公界面服务基础URL */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ming Office UI")
    FString OfficeUIBaseUrl = TEXT("http://127.0.0.1:19000");

    /** 当前办公状态 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ming Office UI")
    EMingOfficeState CurrentOfficeState = EMingOfficeState::Idle;

    /** 是否已连接到服务 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ming Office UI")
    bool bIsConnected = false;

    /** 发送HTTP请求更新状态 */
    void SendStateUpdateRequest(EMingOfficeState State, const FString& Description);

    /** 检查服务健康状态 */
    void CheckServiceHealth();

    /** HTTP请求回调 */
    void OnStateUpdateResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

private:
    /** 状态映射表：游戏状态 -> 办公状态 */
    TMap<FString, EMingOfficeState> GameStateToOfficeStateMap;

    /** 初始化状态映射 */
    void InitializeStateMapping();
};
