#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSystemDependencyManager.generated.h"

UENUM(BlueprintType)
enum class ESystemInitPriority : uint8
{
    Critical = 0,        // 核心系統 - 必須首先初始化
    High = 1,           // 高優先級 - 依賴核心系統
    Medium = 2,         // 中優先級 - 依賴高優先級系統
    Low = 3,            // 低優先級 - 最後初始化
    Deferred = 4        // 延遲初始化 - 可在運行時初始化
};

UENUM(BlueprintType)
enum class ESystemDependencyType : uint8
{
    Required,           // 必需依賴 - 必須滿足
    Optional,           // 可選依賴 - 可以缺失
    Weak,               // 弱依賴 - 僅用於優化
    Conflict,           // 衝突依賴 - 不能共存
    Exclusive           // 排他依賴 - 只能選擇一個
};

USTRUCT(BlueprintType)
struct FSystemDependency
{
    GENERATED_BODY()

    UPROPERTY()
    FString SystemName;

    UPROPERTY()
    ESystemDependencyType DependencyType;

    UPROPERTY()
    FString VersionRequirement;

    UPROPERTY()
    TMap<FString, FString> AdditionalRequirements;

    FSystemDependency()
        : DependencyType(ESystemDependencyType::Required)
    {}
};

USTRUCT(BlueprintType)
struct FSystemInitInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString SystemName;

    UPROPERTY()
    ESystemInitPriority Priority;

    UPROPERTY()
    TArray<FSystemDependency> Dependencies;

    UPROPERTY()
    TArray<FString> Conflicts;

    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsEnabled;

    UPROPERTY()
    float InitTimeout;

    UPROPERTY()
    FDateTime InitStartTime;

    UPROPERTY()
    FDateTime InitEndTime;

    UPROPERTY()
    FString InitError;

    FSystemInitInfo()
        : Priority(ESystemInitPriority::Medium)
        , bIsInitialized(false)
        , bIsEnabled(true)
        , InitTimeout(30.0f)
    {}
};

/**
 * 系統依賴管理器 - 負責系統初始化順序和依賴管理
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSystemDependencyManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSystemDependencyManager();

    // 系統註冊和管理
    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool RegisterSystem(const FString& SystemName, ESystemInitPriority Priority);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool AddDependency(const FString& SystemName, const FString& DependencyName, ESystemDependencyType Type);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool AddConflict(const FString& SystemName, const FString& ConflictName);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool SetSystemEnabled(const FString& SystemName, bool bEnabled);

    // 依賴解析和初始化順序計算
    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FString> CalculateInitOrder();

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool ValidateDependencies();

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FString> GetDependencyChain(const FString& SystemName);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool HasCircularDependency();

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FString> GetCircularDependencyChain();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool InitializeSystem(const FString& SystemName);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool InitializeAllSystems();

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool InitializeSystemsWithPriority(ESystemInitPriority MinPriority, ESystemInitPriority MaxPriority);

    // 系統狀態查詢
    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool IsSystemInitialized(const FString& SystemName) const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool CanInitializeSystem(const FString& SystemName) const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FString> GetInitializedSystems() const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FString> GetPendingSystems() const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FString> GetFailedSystems() const;

    // 錯誤處理和恢復
    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool ReinitializeSystem(const FString& SystemName);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool ShutdownSystem(const FString& SystemName);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool ShutdownAllSystems();

    // 配置和監控
    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    void SetInitTimeout(float TimeoutSeconds);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    FSystemInitInfo GetSystemInfo(const FString& SystemName) const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    TArray<FSystemInitInfo> GetAllSystemInfo() const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    FString GenerateDependencyGraph() const;

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool SaveDependencyConfiguration(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "System Dependency")
    bool LoadDependencyConfiguration(const FString& FilePath);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemInitialized, const FString&, SystemName, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSystemShutdown, const FString&, SystemName, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDependencyError, const FString&, SystemName, const FString&, DependencyName, const FString&, Error);

    UPROPERTY(BlueprintAssignable, Category = "System Dependency")
    FOnSystemInitialized OnSystemInitialized;

    UPROPERTY(BlueprintAssignable, Category = "System Dependency")
    FOnSystemShutdown OnSystemShutdown;

    UPROPERTY(BlueprintAssignable, Category = "System Dependency")
    FOnDependencyError OnDependencyError;

protected:
    // 內部輔助方法
    bool TopologicalSort(TArray<FString>& SortedSystems);
    bool DFSVisit(const FString& SystemName, TSet<FString>& Visited, TSet<FString>& RecursionStack, TArray<FString>& SortedSystems);
    void MarkSystemInitialized(const FString& SystemName, bool bSuccess, const FString& Error = TEXT(""));
    void MarkSystemShutdown(const FString& SystemName, bool bSuccess);
    bool ValidateSystemDependencies(const FString& SystemName) const;

private:
    // 系統註冊表
    UPROPERTY()
    TMap<FString, FSystemInitInfo> SystemRegistry;

    // 依賴關係圖
    UPROPERTY()
    TMap<FString, TArray<FString>> DependencyGraph;

    // 反向依賴圖
    UPROPERTY()
    TMap<FString, TArray<FString>> ReverseDependencyGraph;

    // 初始化順序緩存
    UPROPERTY()
    TArray<FString> CachedInitOrder;

    UPROPERTY()
    bool bInitOrderValid;

    // 配置
    UPROPERTY()
    float DefaultInitTimeout;

    UPROPERTY()
    bool bAutoResolveConflicts;

    UPROPERTY()
    bool bEnableDetailedLogging;
};
