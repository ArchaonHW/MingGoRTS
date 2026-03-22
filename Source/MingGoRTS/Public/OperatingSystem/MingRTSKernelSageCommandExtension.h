// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OperatingSystem/MingRTSKernel.h"
#include "MingRTSKernelSageCommandExtension.generated.h"

class UMingSageCharacterSystem;
class UMingThreePowerSystem;
class UMingMoralAuthority;
class UMingStrategyAuthority;
class UMingMilitaryAuthority;
class UMingWuXingRhythmSystem;
class UMingAntiFallSystem;

/**
 * 至聖者指揮學系統調用類型
 */
UENUM(BlueprintType)
enum class ESageCommandSystemCall: uint8 {
    // 角色系統調用
    CreateSageCharacter,       // 創建指揮者角色
    GetCharacterTraits,        // 獲取角色特性
    UseEvilStrategy,          // 使用逆策
    UseRighteousStrategy,     // 使用正策
    CheckFallStatus,          // 檢查墮落狀態
    PerformAtonement,         // 執行贖罪
    
    // 三權系統調用
    CoordinateThreePowers,    // 協調三權
    CheckMoralAuthority,      // 檢查道權
    ExecuteStrategySwitch,    // 執行策略切換
    ExecuteMilitaryCommand,   // 執行軍事命令
    
    // 五行系統調用
    StartWuXingCycle,         // 啟動五行循環
    AdvanceWuXingPhase,       // 推進五行階段
    GetCurrentWuXingEffects,  // 獲取當前五行效果
    
    // 防墮系統調用
    UpdateFallStatus,         // 更新墮落狀態
    CreateAtonementTask,      // 創建贖罪任務
    CompleteAtonementTask,    // 完成贖罪任務
    
    // 綜合調用
    GetSageCommandStatus,     // 獲取指揮學系統狀態
    ExecuteSageDecision,      // 執行至聖決策
    
    Count
};

/**
 * 至聖者指揮學系統狀態結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCommandKernelStatus
{
    GENERATED_BODY()

    // 系統是否啟用
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    bool bSystemEnabled = true;

    // 當前角色類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 CurrentCharacterType = 0;

    // 當前墮落值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 CurrentFallValue = 0;

    // 墮落閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 FallThreshold = 100;

    // 是否已墮落
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    bool bIsFallen = false;

    // 當前五行階段
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 CurrentWuXingPhase = 0;

    // 三權平衡值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 ThreePowerBalance = 100;

    // 系統健康狀況
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    bool bSystemHealthy = true;

    // 當前策略方向 (0=正, 1=逆)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 CurrentStrategyDirection = 0;

    // 連續使用逆策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 ConsecutiveEvilUses = 0;

    // 贖罪任務數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 ActiveAtonementTasks = 0;

    // 總正策使用次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 TotalRighteousUses = 0;

    // 總逆策使用次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 TotalEvilUses = 0;

    FSageCommandKernelStatus()
        : bSystemEnabled(true)
        , CurrentCharacterType(0)
        , CurrentFallValue(0)
        , FallThreshold(100)
        , bIsFallen(false)
        , CurrentWuXingPhase(0)
        , ThreePowerBalance(100)
        , bSystemHealthy(true)
        , CurrentStrategyDirection(0)
        , ConsecutiveEvilUses(0)
        , ActiveAtonementTasks(0)
        , TotalRighteousUses(0)
        , TotalEvilUses(0)
    {}
};

/**
 * 至聖者指揮學內核請求結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCommandKernelRequest
{
    GENERATED_BODY()

    // 調用類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    ESageCommandSystemCall CallType = ESageCommandSystemCall::GetSageCommandStatus;

    // 整數參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 IntParam1 = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 IntParam2 = 0;

    // 浮點參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    float FloatParam1 = 0.0f;

    // 字符串參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    FString StringParam1;

    // 請求時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    FDateTime RequestTime;

    FSageCommandKernelRequest()
        : RequestTime(FDateTime::Now())
    {}
};

/**
 * 至聖者指揮學內核響應結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCommandKernelResponse
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    bool bSuccess = false;

    // 整數結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    int32 IntResult = 0;

    // 浮點結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    float FloatResult = 0.0f;

    // 字符串結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    FString StringResult;

    // 系統狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    FSageCommandKernelStatus SystemStatus;

    // 錯誤信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    FString ErrorMessage;

    // 響應時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCommandKernel")
    FDateTime ResponseTime;

    FSageCommandKernelResponse()
        : ResponseTime(FDateTime::Now())
    {}
};

/**
 * MingGoRTS 作業系統內核 - 至聖者指揮學擴展
 * 將指揮學系統整合到OS核心，提供系統級別的指揮哲學支持
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSKernelSageCommandExtension : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSKernelSageCommandExtension();

    // 初始化擴展
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool InitializeSageCommandExtension();

    // 綁定到內核
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool BindToKernel(UMingRTSKernel* Kernel);

    // 執行指揮學系統調用
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FSageCommandKernelResponse ExecuteSageCommandCall(const FSageCommandKernelRequest& Request);

    // 獲取系統狀態
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FSageCommandKernelStatus GetSageCommandStatus() const;

    // 設置當前角色類型
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool SetCharacterType(int32 CharacterType, const FString& CharacterName);

    // 執行策略 (正策或逆策)
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FSageCommandKernelResponse ExecuteStrategy(int32 StrategyType, bool bIsEvil);

    // 協調三權
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool CoordinateThreePowers();

    // 啟動五行循環
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool StartWuXingCycle();

    // 推進五行階段
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool AdvanceWuXingPhase();

    // 檢查並更新墮落狀態
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FSageCommandKernelResponse UpdateFallStatus(int32 FallValueDelta, bool bIsEvil);

    // 創建贖罪任務
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    int32 CreateAtonementTask(int32 DifficultyLevel);

    // 完成贖罪任務
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool CompleteAtonementTask(int32 TaskID);

    // 獲取道德建議
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FString GetMoralAdvice() const;

    // 獲取策略建議
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FString GetStrategyRecommendation() const;

    // 獲取五行建議
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    FString GetWuXingRecommendation() const;

    // 檢查系統健康
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool IsSystemHealthy() const;

    // 註冊為系統服務
    UFUNCTION(BlueprintCallable, Category = "Kernel|SageCommand")
    bool RegisterAsSystemService(UMingRTSKernel* Kernel);

    // 系統調用處理器 (供內核調用)
    UFUNCTION()
    FSystemCallResponse HandleSageCommandSystemCall(const FSystemCallRequest& Request);

    // 事件：角色墮落
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterFallen);
    UPROPERTY(BlueprintAssignable, Category = "Kernel|SageCommand")
    FOnCharacterFallen OnCharacterFallen;

    // 事件：三權失衡
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThreePowerImbalance);
    UPROPERTY(BlueprintAssignable, Category = "Kernel|SageCommand")
    FOnThreePowerImbalance OnThreePowerImbalance;

    // 事件：五行階段轉換
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWuXingPhaseChanged, int32, NewPhase);
    UPROPERTY(BlueprintAssignable, Category = "Kernel|SageCommand")
    FOnWuXingPhaseChanged OnWuXingPhaseChanged;

    // 事件：系統警告
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSageCommandWarning, const FString&, WarningMessage);
    UPROPERTY(BlueprintAssignable, Category = "Kernel|SageCommand")
    FOnSageCommandWarning OnSageCommandWarning;

protected:
    // 初始化子系統
    void InitializeSubSystems();

    // 更新系統狀態
    void UpdateSystemStatus();

    // 處理具體系統調用
    FSageCommandKernelResponse HandleCreateCharacter(const FSageCommandKernelRequest& Request);
    FSageCommandKernelResponse HandleUseStrategy(const FSageCommandKernelRequest& Request);
    FSageCommandKernelResponse HandleCheckFall(const FSageCommandKernelRequest& Request);
    FSageCommandKernelResponse HandleCoordinatePowers(const FSageCommandKernelRequest& Request);
    FSageCommandKernelResponse HandleWuXingOperation(const FSageCommandKernelRequest& Request);
    FSageCommandKernelResponse HandleAtonement(const FSageCommandKernelRequest& Request);
    FSageCommandKernelResponse HandleGetStatus(const FSageCommandKernelRequest& Request);

    // 內核回調處理
    UFUNCTION()
    void OnKernelStateChanged(EKernelState OldState, EKernelState NewState);

    // 定時更新
    UFUNCTION()
    void OnSystemUpdate();

protected:
    // 子系統引用
    UPROPERTY()
    TObjectPtr<UMingSageCharacterSystem> CharacterSystem;

    UPROPERTY()
    TObjectPtr<UMingThreePowerSystem> ThreePowerSystem;

    UPROPERTY()
    TObjectPtr<UMingMoralAuthority> MoralAuthority;

    UPROPERTY()
    TObjectPtr<UMingStrategyAuthority> StrategyAuthority;

    UPROPERTY()
    TObjectPtr<UMingMilitaryAuthority> MilitaryAuthority;

    UPROPERTY()
    TObjectPtr<UMingWuXingRhythmSystem> WuXingRhythmSystem;

    UPROPERTY()
    TObjectPtr<UMingAntiFallSystem> AntiFallSystem;

    // 綁定的內核
    UPROPERTY()
    TObjectPtr<UMingRTSKernel> BoundKernel;

    // 系統狀態緩存
    UPROPERTY()
    FSageCommandKernelStatus CachedStatus;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 系統服務名稱
    UPROPERTY(EditDefaultsOnly, Category = "Kernel|SageCommand")
    FString SystemServiceName = TEXT("SageCommand");

    // 更新間隔 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "Kernel|SageCommand")
    float UpdateInterval = 1.0f;

    // 內部計時器
    UPROPERTY()
    FTimerHandle UpdateTimerHandle;
};
