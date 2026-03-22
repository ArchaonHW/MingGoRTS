#include "System/MingRTSSystemDependencyManager.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "JsonObjectConverter.h"
#include "TimerManager.h"
#include "MingGoRTS.h"

UMingRTSSystemDependencyManager::UMingRTSSystemDependencyManager()
    : DefaultInitTimeout(30.0f)
    , bAutoResolveConflicts(true)
    , bEnableDetailedLogging(true)
    , bInitOrderValid(false)
{
}

bool UMingRTSSystemDependencyManager::RegisterSystem(const FString& SystemName, ESystemInitPriority Priority)
{
    if (SystemName.IsEmpty())
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Cannot register system with empty name"));
        return false;
    }

    if (SystemRegistry.Contains(SystemName))
    {
        UE_LOG(LogMingGoRTS, Warning, TEXT("SystemDependencyManager: System '%s' already registered"), *SystemName);
        return false;
    }

    FSystemInitInfo SystemInfo;
    SystemInfo.SystemName = SystemName;
    SystemInfo.Priority = Priority;
    SystemInfo.bIsInitialized = false;
    SystemInfo.bIsEnabled = true;
    SystemInfo.InitTimeout = DefaultInitTimeout;

    SystemRegistry.Add(SystemName, SystemInfo);
    DependencyGraph.Add(SystemName, TArray<FString>());
    ReverseDependencyGraph.Add(SystemName, TArray<FString>());

    bInitOrderValid = false;

    if (bEnableDetailedLogging)
    {
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Registered system '%s' with priority %d"), 
               *SystemName, static_cast<int32>(Priority));
    }

    return true;
}

bool UMingRTSSystemDependencyManager::AddDependency(const FString& SystemName, const FString& DependencyName, ESystemDependencyType Type)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: System '%s' not registered"), *SystemName);
        return false;
    }

    if (!SystemRegistry.Contains(DependencyName))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Dependency '%s' not registered"), *DependencyName);
        return false;
    }

    FSystemDependency Dependency;
    Dependency.SystemName = DependencyName;
    Dependency.DependencyType = Type;

    FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    SystemInfo.Dependencies.Add(Dependency);

    // 更新依賴圖
    if (!DependencyGraph[SystemName].Contains(DependencyName))
    {
        DependencyGraph[SystemName].Add(DependencyName);
        ReverseDependencyGraph[DependencyName].Add(SystemName);
    }

    bInitOrderValid = false;

    if (bEnableDetailedLogging)
    {
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Added dependency '%s' -> '%s' (Type: %d)"), 
               *SystemName, *DependencyName, static_cast<int32>(Type));
    }

    return true;
}

bool UMingRTSSystemDependencyManager::AddConflict(const FString& SystemName, const FString& ConflictName)
{
    if (!SystemRegistry.Contains(SystemName) || !SystemRegistry.Contains(ConflictName))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: One or both systems not registered"));
        return false;
    }

    FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    if (!SystemInfo.Conflicts.Contains(ConflictName))
    {
        SystemInfo.Conflicts.Add(ConflictName);
    }

    FSystemInitInfo& ConflictInfo = SystemRegistry[ConflictName];
    if (!ConflictInfo.Conflicts.Contains(SystemName))
    {
        ConflictInfo.Conflicts.Add(SystemName);
    }

    if (bEnableDetailedLogging)
    {
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Added conflict between '%s' and '%s'"), 
               *SystemName, *ConflictName);
    }

    return true;
}

bool UMingRTSSystemDependencyManager::SetSystemEnabled(const FString& SystemName, bool bEnabled)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: System '%s' not registered"), *SystemName);
        return false;
    }

    SystemRegistry[SystemName].bIsEnabled = bEnabled;

    if (bEnableDetailedLogging)
    {
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: System '%s' %s"), 
               *SystemName, bEnabled ? TEXT("enabled") : TEXT("disabled"));
    }

    return true;
}

TArray<FString> UMingRTSSystemDependencyManager::CalculateInitOrder()
{
    if (bInitOrderValid && !CachedInitOrder.IsEmpty())
    {
        return CachedInitOrder;
    }

    TArray<FString> SortedSystems;
    if (!TopologicalSort(SortedSystems))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Circular dependency detected"));
        return TArray<FString>();
    }

    // 按優先級排序
    SortedSystems.Sort([this](const FString& A, const FString& B)
    {
        const FSystemInitInfo& InfoA = SystemRegistry[A];
        const FSystemInitInfo& InfoB = SystemRegistry[B];
        
        if (InfoA.Priority != InfoB.Priority)
        {
            return static_cast<int32>(InfoA.Priority) < static_cast<int32>(InfoB.Priority);
        }
        
        return A < B; // 字母順序作為次要排序
    });

    CachedInitOrder = SortedSystems;
    bInitOrderValid = true;

    if (bEnableDetailedLogging)
    {
        FString OrderString;
        for (const FString& System : SortedSystems)
        {
            OrderString += System + TEXT(" -> ");
        }
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Calculated init order: %s"), *OrderString);
    }

    return SortedSystems;
}

bool UMingRTSSystemDependencyManager::ValidateDependencies()
{
    if (HasCircularDependency())
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Circular dependencies detected"));
        return false;
    }

    for (const auto& SystemPair : SystemRegistry)
    {
        const FString& SystemName = SystemPair.Key;
        const FSystemInitInfo& SystemInfo = SystemPair.Value;

        if (!ValidateSystemDependencies(SystemName))
        {
            return false;
        }
    }

    UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: All dependencies validated successfully"));
    return true;
}

TArray<FString> UMingRTSSystemDependencyManager::GetDependencyChain(const FString& SystemName)
{
    TArray<FString> Chain;
    TSet<FString> Visited;

    if (!SystemRegistry.Contains(SystemName))
    {
        return Chain;
    }

    // 遞歸獲取所有依賴
    TArray<FString> Stack;
    Stack.Add(SystemName);

    while (!Stack.IsEmpty())
    {
        FString Current = Stack.Pop();
        if (Visited.Contains(Current))
        {
            continue;
        }

        Visited.Add(Current);
        Chain.Add(Current);

        const FSystemInitInfo& SystemInfo = SystemRegistry[Current];
        for (const FSystemDependency& Dep : SystemInfo.Dependencies)
        {
            if (!Visited.Contains(Dep.SystemName))
            {
                Stack.Add(Dep.SystemName);
            }
        }
    }

    return Chain;
}

bool UMingRTSSystemDependencyManager::HasCircularDependency()
{
    TSet<FString> Visited;
    TSet<FString> RecursionStack;

    for (const auto& SystemPair : SystemRegistry)
    {
        if (!Visited.Contains(SystemPair.Key))
        {
            if (DFSVisit(SystemPair.Key, Visited, RecursionStack, TArray<FString>()))
            {
                return true;
            }
        }
    }

    return false;
}

TArray<FString> UMingRTSSystemDependencyManager::GetCircularDependencyChain()
{
    TArray<FString> Chain;
    TSet<FString> Visited;
    TSet<FString> RecursionStack;

    for (const auto& SystemPair : SystemRegistry)
    {
        if (!Visited.Contains(SystemPair.Key))
        {
            if (DFSVisit(SystemPair.Key, Visited, RecursionStack, Chain))
            {
                return Chain;
            }
        }
    }

    return Chain;
}

bool UMingRTSSystemDependencyManager::InitializeSystem(const FString& SystemName)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: System '%s' not registered"), *SystemName);
        return false;
    }

    FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    
    if (SystemInfo.bIsInitialized)
    {
        UE_LOG(LogMingGoRTS, Warning, TEXT("SystemDependencyManager: System '%s' already initialized"), *SystemName);
        return true;
    }

    if (!SystemInfo.bIsEnabled)
    {
        UE_LOG(LogMingGoRTS, Warning, TEXT("SystemDependencyManager: System '%s' is disabled"), *SystemName);
        return false;
    }

    // 檢查依賴
    if (!ValidateSystemDependencies(SystemName))
    {
        FString Error = FString::Printf(TEXT("Dependencies not satisfied for system '%s'"), *SystemName);
        MarkSystemInitialized(SystemName, false, Error);
        return false;
    }

    // 檢查衝突
    for (const FString& Conflict : SystemInfo.Conflicts)
    {
        if (IsSystemInitialized(Conflict))
        {
            FString Error = FString::Printf(TEXT("System '%s' conflicts with initialized system '%s'"), *SystemName, *Conflict);
            MarkSystemInitialized(SystemName, false, Error);
            return false;
        }
    }

    SystemInfo.InitStartTime = FDateTime::Now();

    if (bEnableDetailedLogging)
    {
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Initializing system '%s'"), *SystemName);
    }

    // 這裡應該調用實際的系統初始化方法
    // 由於這是依賴管理器，我們假設初始化成功
    bool bSuccess = true;
    
    SystemInfo.InitEndTime = FDateTime::Now();
    MarkSystemInitialized(SystemName, bSuccess);

    return bSuccess;
}

bool UMingRTSSystemDependencyManager::InitializeAllSystems()
{
    if (!ValidateDependencies())
    {
        return false;
    }

    TArray<FString> InitOrder = CalculateInitOrder();
    bool bAllSuccess = true;

    for (const FString& SystemName : InitOrder)
    {
        if (!InitializeSystem(SystemName))
        {
            bAllSuccess = false;
            UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Failed to initialize system '%s'"), *SystemName);
        }
    }

    return bAllSuccess;
}

bool UMingRTSSystemDependencyManager::InitializeSystemsWithPriority(ESystemInitPriority MinPriority, ESystemInitPriority MaxPriority)
{
    TArray<FString> InitOrder = CalculateInitOrder();
    bool bAllSuccess = true;

    for (const FString& SystemName : InitOrder)
    {
        const FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
        
        if (SystemInfo.Priority >= MinPriority && SystemInfo.Priority <= MaxPriority)
        {
            if (!InitializeSystem(SystemName))
            {
                bAllSuccess = false;
            }
        }
    }

    return bAllSuccess;
}

bool UMingRTSSystemDependencyManager::IsSystemInitialized(const FString& SystemName) const
{
    if (!SystemRegistry.Contains(SystemName))
    {
        return false;
    }

    return SystemRegistry[SystemName].bIsInitialized;
}

bool UMingRTSSystemDependencyManager::CanInitializeSystem(const FString& SystemName) const
{
    if (!SystemRegistry.Contains(SystemName))
    {
        return false;
    }

    const FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    
    if (SystemInfo.bIsInitialized || !SystemInfo.bIsEnabled)
    {
        return false;
    }

    return ValidateSystemDependencies(SystemName);
}

TArray<FString> UMingRTSSystemDependencyManager::GetInitializedSystems() const
{
    TArray<FString> InitializedSystems;

    for (const auto& SystemPair : SystemRegistry)
    {
        if (SystemPair.Value.bIsInitialized)
        {
            InitializedSystems.Add(SystemPair.Key);
        }
    }

    return InitializedSystems;
}

TArray<FString> UMingRTSSystemDependencyManager::GetPendingSystems() const
{
    TArray<FString> PendingSystems;

    for (const auto& SystemPair : SystemRegistry)
    {
        const FSystemInitInfo& SystemInfo = SystemPair.Value;
        if (!SystemInfo.bIsInitialized && SystemInfo.bIsEnabled)
        {
            PendingSystems.Add(SystemPair.Key);
        }
    }

    return PendingSystems;
}

TArray<FString> UMingRTSSystemDependencyManager::GetFailedSystems() const
{
    TArray<FString> FailedSystems;

    for (const auto& SystemPair : SystemRegistry)
    {
        const FSystemInitInfo& SystemInfo = SystemPair.Value;
        if (!SystemInfo.bIsInitialized && !SystemInfo.InitError.IsEmpty())
        {
            FailedSystems.Add(SystemPair.Key);
        }
    }

    return FailedSystems;
}

bool UMingRTSSystemDependencyManager::ReinitializeSystem(const FString& SystemName)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        return false;
    }

    ShutdownSystem(SystemName);
    return InitializeSystem(SystemName);
}

bool UMingRTSSystemDependencyManager::ShutdownSystem(const FString& SystemName)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: System '%s' not registered"), *SystemName);
        return false;
    }

    FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    
    if (!SystemInfo.bIsInitialized)
    {
        UE_LOG(LogMingGoRTS, Warning, TEXT("SystemDependencyManager: System '%s' not initialized"), *SystemName);
        return true;
    }

    // 檢查是否有其他系統依賴此系統
    for (const FString& Dependent : ReverseDependencyGraph[SystemName])
    {
        if (IsSystemInitialized(Dependent))
        {
            UE_LOG(LogMingGoRTS, Warning, TEXT("SystemDependencyManager: System '%s' has dependent '%s' still initialized"), 
                   *SystemName, *Dependent);
        }
    }

    if (bEnableDetailedLogging)
    {
        UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Shutting down system '%s'"), *SystemName);
    }

    // 這裡應該調用實際的系統關閉方法
    bool bSuccess = true;
    
    MarkSystemShutdown(SystemName, bSuccess);
    return bSuccess;
}

bool UMingRTSSystemDependencyManager::ShutdownAllSystems()
{
    TArray<FString> InitOrder = CalculateInitOrder();
    bool bAllSuccess = true;

    // 按相反順序關閉系統
    for (int32 i = InitOrder.Num() - 1; i >= 0; i--)
    {
        const FString& SystemName = InitOrder[i];
        if (!ShutdownSystem(SystemName))
        {
            bAllSuccess = false;
        }
    }

    return bAllSuccess;
}

void UMingRTSSystemDependencyManager::SetInitTimeout(float TimeoutSeconds)
{
    DefaultInitTimeout = TimeoutSeconds;
    
    for (auto& SystemPair : SystemRegistry)
    {
        SystemPair.Value.InitTimeout = TimeoutSeconds;
    }
}

FSystemInitInfo UMingRTSSystemDependencyManager::GetSystemInfo(const FString& SystemName) const
{
    if (SystemRegistry.Contains(SystemName))
    {
        return SystemRegistry[SystemName];
    }

    return FSystemInitInfo();
}

TArray<FSystemInitInfo> UMingRTSSystemDependencyManager::GetAllSystemInfo() const
{
    TArray<FSystemInitInfo> AllInfo;

    for (const auto& SystemPair : SystemRegistry)
    {
        AllInfo.Add(SystemPair.Value);
    }

    return AllInfo;
}

FString UMingRTSSystemDependencyManager::GenerateDependencyGraph() const
{
    FString GraphString = TEXT("Dependency Graph:\n");

    for (const auto& SystemPair : SystemRegistry)
    {
        const FString& SystemName = SystemPair.Key;
        const FSystemInitInfo& SystemInfo = SystemPair.Value;

        GraphString += FString::Printf(TEXT("%s [%s] %s\n"), 
            *SystemName,
            *StaticEnum<ESystemInitPriority>()->GetValueAsString(SystemInfo.Priority),
            SystemInfo.bIsInitialized ? TEXT("(Initialized)") : TEXT("(Not Initialized)"));

        for (const FSystemDependency& Dep : SystemInfo.Dependencies)
        {
            GraphString += FString::Printf(TEXT("  -> %s [%s]\n"), 
                *Dep.SystemName,
                *StaticEnum<ESystemDependencyType>()->GetValueAsString(Dep.DependencyType));
        }
    }

    return GraphString;
}

bool UMingRTSSystemDependencyManager::SaveDependencyConfiguration(const FString& FilePath)
{
    TArray<FSystemInitInfo> AllInfo = GetAllSystemInfo();
    
    FString JsonString;
    if (!FJsonObjectConverter::UStructToJsonObjectString(AllInfo, JsonString))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Failed to serialize dependency configuration"));
        return false;
    }

    if (!FFileHelper::SaveStringToFile(JsonString, *FilePath))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Failed to save dependency configuration to '%s'"), *FilePath);
        return false;
    }

    UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Saved dependency configuration to '%s'"), *FilePath);
    return true;
}

bool UMingRTSSystemDependencyManager::LoadDependencyConfiguration(const FString& FilePath)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Failed to load dependency configuration from '%s'"), *FilePath);
        return false;
    }

    TArray<FSystemInitInfo> LoadedInfo;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &LoadedInfo))
    {
        UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Failed to deserialize dependency configuration"));
        return false;
    }

    // 清空現有註冊
    SystemRegistry.Empty();
    DependencyGraph.Empty();
    ReverseDependencyGraph.Empty();
    bInitOrderValid = false;

    // 重新註冊系統
    for (const FSystemInitInfo& Info : LoadedInfo)
    {
        RegisterSystem(Info.SystemName, Info.Priority);
        
        for (const FSystemDependency& Dep : Info.Dependencies)
        {
            AddDependency(Info.SystemName, Dep.SystemName, Dep.DependencyType);
        }
        
        for (const FString& Conflict : Info.Conflicts)
        {
            AddConflict(Info.SystemName, Conflict);
        }
    }

    UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: Loaded dependency configuration from '%s'"), *FilePath);
    return true;
}

// Protected methods

bool UMingRTSSystemDependencyManager::TopologicalSort(TArray<FString>& SortedSystems)
{
    TSet<FString> Visited;
    TSet<FString> RecursionStack;

    for (const auto& SystemPair : SystemRegistry)
    {
        if (!Visited.Contains(SystemPair.Key))
        {
            if (DFSVisit(SystemPair.Key, Visited, RecursionStack, SortedSystems))
            {
                return false; // Circular dependency detected
            }
        }
    }

    return true;
}

bool UMingRTSSystemDependencyManager::DFSVisit(const FString& SystemName, TSet<FString>& Visited, TSet<FString>& RecursionStack, TArray<FString>& SortedSystems)
{
    Visited.Add(SystemName);
    RecursionStack.Add(SystemName);

    for (const FString& Dependency : DependencyGraph[SystemName])
    {
        if (!Visited.Contains(Dependency))
        {
            if (DFSVisit(Dependency, Visited, RecursionStack, SortedSystems))
            {
                return true; // Circular dependency detected
            }
        }
        else if (RecursionStack.Contains(Dependency))
        {
            return true; // Circular dependency detected
        }
    }

    RecursionStack.Remove(SystemName);
    SortedSystems.Add(SystemName);
    return false;
}

void UMingRTSSystemDependencyManager::MarkSystemInitialized(const FString& SystemName, bool bSuccess, const FString& Error)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        return;
    }

    FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    SystemInfo.bIsInitialized = bSuccess;
    SystemInfo.InitEndTime = FDateTime::Now();
    
    if (!bSuccess)
    {
        SystemInfo.InitError = Error;
    }

    OnSystemInitialized.Broadcast(SystemName, bSuccess);

    if (bEnableDetailedLogging)
    {
        if (bSuccess)
        {
            UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: System '%s' initialized successfully"), *SystemName);
        }
        else
        {
            UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: System '%s' initialization failed: %s"), *SystemName, *Error);
        }
    }
}

void UMingRTSSystemDependencyManager::MarkSystemShutdown(const FString& SystemName, bool bSuccess)
{
    if (!SystemRegistry.Contains(SystemName))
    {
        return;
    }

    FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];
    SystemInfo.bIsInitialized = false;
    SystemInfo.InitError = TEXT("");

    OnSystemShutdown.Broadcast(SystemName, bSuccess);

    if (bEnableDetailedLogging)
    {
        if (bSuccess)
        {
            UE_LOG(LogMingGoRTS, Log, TEXT("SystemDependencyManager: System '%s' shutdown successfully"), *SystemName);
        }
        else
        {
            UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: System '%s' shutdown failed"), *SystemName);
        }
    }
}

bool UMingRTSSystemDependencyManager::ValidateSystemDependencies(const FString& SystemName) const
{
    if (!SystemRegistry.Contains(SystemName))
    {
        return false;
    }

    const FSystemInitInfo& SystemInfo = SystemRegistry[SystemName];

    for (const FSystemDependency& Dep : SystemInfo.Dependencies)
    {
        switch (Dep.DependencyType)
        {
            case ESystemDependencyType::Required:
                if (!IsSystemInitialized(Dep.SystemName))
                {
                    UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Required dependency '%s' not initialized for system '%s'"), 
                           *Dep.SystemName, *SystemName);
                    return false;
                }
                break;

            case ESystemDependencyType::Optional:
                // Optional dependencies don't block initialization
                break;

            case ESystemDependencyType::Weak:
                // Weak dependencies don't block initialization
                break;

            case ESystemDependencyType::Conflict:
                if (IsSystemInitialized(Dep.SystemName))
                {
                    UE_LOG(LogMingGoRTS, Error, TEXT("SystemDependencyManager: Conflict dependency '%s' is initialized for system '%s'"), 
                           *Dep.SystemName, *SystemName);
                    return false;
                }
                break;

            case ESystemDependencyType::Exclusive:
                // Exclusive dependencies require special handling
                break;
        }
    }

    return true;
}
