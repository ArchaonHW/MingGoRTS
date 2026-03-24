#include "MingOptimizedRTS.h"
#include "Async/ParallelFor.h"

// ============================================================================
// 單位管理器實現
// ============================================================================

void UMingOptimizedUnitManager::Initialize(int32 MaxUnits)
{
    // 初始化單位槽位
    for (int32 i = 0; i < UnitSlots.Capacity(); ++i)
    {
        UnitSlots[i].bInUse = false;
        UnitSlots[i].LastUpdateTime = 0.0f;
    }
    
    // 填充可用單位ID列表
    AvailableUnitIds.Empty();
    for (int32 i = 0; i < FMath::Min(MaxUnits, UnitSlots.Capacity()); ++i)
    {
        AvailableUnitIds.Add(i);
    }
    
    ActiveUnitCount = 0;
    
    UE_LOG(LogTemp, Log, TEXT("[UnitManager] Initialized with %d units"), MaxUnits);
}

int32 UMingOptimizedUnitManager::SpawnUnit(int32 UnitType, FVector Location, int32 OwnerPlayerId)
{
    if (AvailableUnitIds.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[UnitManager] Max units reached"));
        return -1;
    }
    
    int32 UnitId = AvailableUnitIds[AvailableUnitIds.Num() - 1];
    AvailableUnitIds.RemoveAt(AvailableUnitIds.Num() - 1);
    
    FUnitSlot& Slot = UnitSlots[UnitId];
    Slot.bInUse = true;
    Slot.LastUpdateTime = FPlatformTime::Seconds();
    
    FMingOptimizedUnitData& Data = Slot.Data;
    Data.UnitId = UnitId;
    Data.UnitType = UnitType;
    Data.Location = FMingCompressedVector(Location);
    Data.Rotation = FMingCompressedRotator(FRotator::ZeroRotator);
    Data.SetHealth(1.0f);
    Data.SetEnergy(1.0f);
    Data.Level = 1;
    Data.SetExperience(0.0f);
    Data.StatusFlags = 0x01; // Alive
    Data.OwnerPlayerId = OwnerPlayerId;
    
    // 添加到空間分割
    SpatialPartition.InsertObject(&UnitId, Location);
    
    ++ActiveUnitCount;
    
    return UnitId;
}

void UMingOptimizedUnitManager::DestroyUnit(int32 UnitId)
{
    if (UnitId < 0 || UnitId >= UnitSlots.Capacity())
    {
        return;
    }
    
    FUnitSlot& Slot = UnitSlots[UnitId];
    if (!Slot.bInUse)
    {
        return;
    }
    
    // 從空間分割移除
    SpatialPartition.RemoveObject(&UnitId, Slot.Data.Location.ToVector());
    
    Slot.bInUse = false;
    AvailableUnitIds.Add(UnitId);
    --ActiveUnitCount;
}

void UMingOptimizedUnitManager::UpdateUnitPosition(int32 UnitId, FVector NewLocation, FRotator NewRotation)
{
    if (UnitId < 0 || UnitId >= UnitSlots.Capacity())
    {
        return;
    }
    
    FUnitSlot& Slot = UnitSlots[UnitId];
    if (!Slot.bInUse)
    {
        return;
    }
    
    // 更新空間分割
    FVector OldLocation = Slot.Data.Location.ToVector();
    SpatialPartition.RemoveObject(&UnitId, OldLocation);
    SpatialPartition.InsertObject(&UnitId, NewLocation);
    
    // 更新數據
    Slot.Data.Location = FMingCompressedVector(NewLocation);
    Slot.Data.Rotation = FMingCompressedRotator(NewRotation);
    Slot.LastUpdateTime = FPlatformTime::Seconds();
    
    Slot.Data.SetMoving(true);
}

void UMingOptimizedUnitManager::UpdateUnitHealth(int32 UnitId, float Health)
{
    if (UnitId < 0 || UnitId >= UnitSlots.Capacity())
    {
        return;
    }
    
    FUnitSlot& Slot = UnitSlots[UnitId];
    if (!Slot.bInUse)
    {
        return;
    }
    
    Slot.Data.SetHealth(Health);
    
    if (Health <= 0.0f)
    {
        Slot.Data.SetAlive(false);
    }
}

FMingOptimizedUnitData* UMingOptimizedUnitManager::GetUnitData(int32 UnitId)
{
    if (UnitId < 0 || UnitId >= UnitSlots.Capacity())
    {
        return nullptr;
    }
    
    FUnitSlot& Slot = UnitSlots[UnitId];
    if (!Slot.bInUse)
    {
        return nullptr;
    }
    
    return &Slot.Data;
}

TArray<int32> UMingOptimizedUnitManager::GetUnitsInRadius(FVector Center, float Radius) const
{
    TArray<int32> Results;
    Results.Reserve(32);
    
    TArray<int32*> NearbyPointers;
    SpatialPartition.QueryNearbyObjects(Center, Radius, NearbyPointers);
    
    for (int32* Ptr : NearbyPointers)
    {
        if (Ptr)
        {
            Results.Add(*Ptr);
        }
    }
    
    return Results;
}

TArray<int32> UMingOptimizedUnitManager::GetUnitsByPlayer(int32 PlayerId) const
{
    TArray<int32> Results;
    Results.Reserve(64);
    
    for (int32 i = 0; i < UnitSlots.Capacity(); ++i)
    {
        if (UnitSlots[i].bInUse && UnitSlots[i].Data.OwnerPlayerId == PlayerId)
        {
            Results.Add(i);
        }
    }
    
    return Results;
}

void UMingOptimizedUnitManager::BatchUpdatePositions(const TArray<int32>& UnitIds, const TArray<FVector>& NewLocations)
{
    int32 Count = FMath::Min(UnitIds.Num(), NewLocations.Num());
    
    ParallelFor(Count, [this, &UnitIds, &NewLocations](int32 Index)
    {
        int32 UnitId = UnitIds[Index];
        FVector NewLocation = NewLocations[Index];
        
        if (UnitId >= 0 && UnitId < UnitSlots.Capacity())
        {
            FUnitSlot& Slot = UnitSlots[UnitId];
            if (Slot.bInUse)
            {
                FVector OldLocation = Slot.Data.Location.ToVector();
                Slot.Data.Location = FMingCompressedVector(NewLocation);
                Slot.LastUpdateTime = FPlatformTime::Seconds();
            }
        }
    });
    
    // 更新空間分割 (在主執行緒)
    for (int32 i = 0; i < Count; ++i)
    {
        int32 UnitId = UnitIds[i];
        if (UnitId >= 0 && UnitId < UnitSlots.Capacity())
        {
            FUnitSlot& Slot = UnitSlots[UnitId];
            if (Slot.bInUse)
            {
                FVector OldLocation = Slot.Data.Location.ToVector();
                FVector NewLocation = NewLocations[i];
                
                SpatialPartition.RemoveObject(&UnitId, OldLocation);
                SpatialPartition.InsertObject(&UnitId, NewLocation);
            }
        }
    }
}

int32 UMingOptimizedUnitManager::GetActiveUnitCount() const
{
    return ActiveUnitCount;
}

void UMingOptimizedUnitManager::ProcessUnitAI(float DeltaTime)
{
    // 批次處理單位AI
    const int32 BatchSize = 32;
    int32 NumBatches = (ActiveUnitCount + BatchSize - 1) / BatchSize;
    
    for (int32 Batch = 0; Batch < NumBatches; ++Batch)
    {
        int32 StartIdx = Batch * BatchSize;
        int32 EndIdx = FMath::Min(StartIdx + BatchSize, UnitSlots.Capacity());
        
        ParallelFor(EndIdx - StartIdx, [this, StartIdx, DeltaTime](int32 Index)
        {
            int32 UnitId = StartIdx + Index;
            if (UnitId < UnitSlots.Capacity() && UnitSlots[UnitId].bInUse)
            {
                ProcessUnitBehavior(UnitId, DeltaTime);
            }
        });
    }
}

void UMingOptimizedUnitManager::ProcessUnitBehavior(int32 UnitId, float DeltaTime)
{
    // 單位行為處理邏輯
    FUnitSlot& Slot = UnitSlots[UnitId];
    
    // 檢查是否需要移動
    if (Slot.Data.IsMoving())
    {
        // 處理移動邏輯
    }
    
    // 檢查戰鬥狀態
    // 檢查資源收集
    // 等等...
}

void UMingOptimizedUnitManager::UpdateSpatialPartition(int32 UnitId)
{
    // 已整合到 UpdateUnitPosition
}

// ============================================================================
// 資源管理器實現
// ============================================================================

void UMingOptimizedResourceManager::InitializeResourceSystem()
{
    // 初始化資源節點
    for (int32 i = 0; i < ResourceNodes.Capacity(); ++i)
    {
        ResourceNodes[i].NodeId = -1;
    }
    
    UE_LOG(LogTemp, Log, TEXT("[ResourceManager] Initialized"));
}

void UMingOptimizedResourceManager::RegisterResourceNode(int32 NodeId, int32 ResourceType, FVector Location, int32 InitialAmount)
{
    if (NodeId < 0 || NodeId >= ResourceNodes.Capacity())
    {
        return;
    }
    
    FMingOptimizedResourceNode& Node = ResourceNodes[NodeId];
    Node.NodeId = NodeId;
    Node.ResourceType = ResourceType;
    Node.Location = FMingCompressedVector(Location);
    Node.SetAmount(InitialAmount);
    Node.MaxAmount = FMath::Clamp(InitialAmount, 0, 65535);
    Node.DepletionLevel = 0;
    
    // 添加到空間分割
    ResourceSpatialGrid.InsertObject(&NodeId, Location);
}

int32 UMingOptimizedResourceManager::HarvestResource(int32 NodeId, int32 Amount)
{
    if (NodeId < 0 || NodeId >= ResourceNodes.Capacity())
    {
        return 0;
    }
    
    FMingOptimizedResourceNode& Node = ResourceNodes[NodeId];
    if (Node.NodeId < 0)
    {
        return 0;
    }
    
    int32 Available = Node.GetAmount();
    int32 Harvested = FMath::Min(Amount, Available);
    
    Node.SetAmount(Available - Harvested);
    
    // 更新耗竭程度
    if (Node.MaxAmount > 0)
    {
        float DepletionRatio = 1.0f - (Node.GetAmount() / (float)Node.MaxAmount);
        Node.DepletionLevel = FMath::Clamp(FMath::RoundToInt(DepletionRatio * 255), 0, 255);
    }
    
    return Harvested;
}

TArray<int32> UMingOptimizedResourceManager::FindNearestResourceNodes(FVector Location, int32 ResourceType, int32 MaxResults) const
{
    TArray<int32> Results;
    Results.Reserve(MaxResults);
    
    TArray<int32*> NearbyPointers;
    ResourceSpatialGrid.QueryNearbyObjects(Location, 10000.0f, NearbyPointers);
    
    // 按距離排序並篩選類型
    TArray<TPair<int32, float>> Candidates;
    
    for (int32* Ptr : NearbyPointers)
    {
        if (!Ptr) continue;
        
        int32 NodeId = *Ptr;
        if (NodeId < 0 || NodeId >= ResourceNodes.Capacity())
        {
            continue;
        }
        
        const FMingOptimizedResourceNode& Node = ResourceNodes[NodeId];
        if (Node.NodeId < 0 || Node.ResourceType != ResourceType)
        {
            continue;
        }
        
        if (Node.GetAmount() <= 0)
        {
            continue;
        }
        
        float Distance = FVector::Dist(Location, Node.Location.ToVector());
        Candidates.Add(TPair<int32, float>(NodeId, Distance));
    }
    
    // 按距離排序
    Candidates.Sort([](const TPair<int32, float>& A, const TPair<int32, float>& B)
    {
        return A.Value < B.Value;
    });
    
    // 返回最近N結果
    for (int32 i = 0; i < FMath::Min(MaxResults, Candidates.Num()); ++i)
    {
        Results.Add(Candidates[i].Key);
    }
    
    return Results;
}

FMingOptimizedResourceNode* UMingOptimizedResourceManager::GetResourceNode(int32 NodeId)
{
    if (NodeId < 0 || NodeId >= ResourceNodes.Capacity())
    {
        return nullptr;
    }
    
    if (ResourceNodes[NodeId].NodeId < 0)
    {
        return nullptr;
    }
    
    return &ResourceNodes[NodeId];
}

// ============================================================================
// 戰鬥管理器實現
// ============================================================================

void UMingOptimizedCombatManager::Initialize()
{
    PendingCombatEvents.Empty();
    UE_LOG(LogTemp, Log, TEXT("[CombatManager] Initialized"));
}

void UMingOptimizedCombatManager::ProcessCombatEvent(const FMingOptimizedCombatEvent& Event)
{
    ApplyDamage(Event.DefenderId, Event.GetDamage());
}

void UMingOptimizedCombatManager::BatchProcessCombat(const TArray<FMingOptimizedCombatEvent>& Events)
{
    // 先收集所有傷害
    TMap<int32, int32> DamageAccumulator;
    
    for (const auto& Event : Events)
    {
        int32* ExistingDamage = DamageAccumulator.Find(Event.DefenderId);
        if (ExistingDamage)
        {
            *ExistingDamage += Event.GetDamage();
        }
        else
        {
            DamageAccumulator.Add(Event.DefenderId, Event.GetDamage());
        }
    }
    
    // 批次應用傷害
    for (const auto& Pair : DamageAccumulator)
    {
        ApplyDamage(Pair.Key, Pair.Value);
    }
}

TArray<int32> UMingOptimizedCombatManager::FindTargetsInRange(int32 AttackerId, float AttackRange) const
{
    // 這裡應該與 UnitManager N空間分割整合
    TArray<int32> Results;
    Results.Reserve(16);
    
    // 模擬實現 - 實際應該查詢空間分割
    
    return Results;
}

int32 UMingOptimizedCombatManager::SelectBestTarget(int32 AttackerId, const TArray<int32>& PotentialTargets) const
{
    if (PotentialTargets.IsEmpty())
    {
        return -1;
    }
    
    // 選擇威脅等級最高N目標
    int32 BestTarget = -1;
    float HighestThreat = -1.0f;
    
    for (int32 TargetId : PotentialTargets)
    {
        float Threat = CalculateThreatLevel(TargetId);
        if (Threat > HighestThreat)
        {
            HighestThreat = Threat;
            BestTarget = TargetId;
        }
    }
    
    return BestTarget;
}

void UMingOptimizedCombatManager::ApplyDamage(int32 UnitId, int32 Damage)
{
    // 實際應用傷害到 UnitManager
    UE_LOG(LogTemp, Verbose, TEXT("[CombatManager] Applying %d damage to unit %d"), Damage, UnitId);
}

float UMingOptimizedCombatManager::CalculateThreatLevel(int32 UnitId) const
{
    // 計算單位威脅等級
    return FMath::RandRange(0.0f, 1.0f);
}

// ============================================================================
// RTS 優化管理器實現
// ============================================================================

void UMingRTSOptimizationManager::InitializeRTSOptimization()
{
    // 創建子系統
    UnitManager = NewObject<UMingOptimizedUnitManager>(this);
    UnitManager->Initialize(512);
    
    ResourceManager = NewObject<UMingOptimizedResourceManager>(this);
    ResourceManager->InitializeResourceSystem();
    
    CombatManager = NewObject<UMingOptimizedCombatManager>(this);
    CombatManager->Initialize();
    
    LODDistance = 5000.0f;
    bLODEnabled = true;
    
    UE_LOG(LogTemp, Log, TEXT("[RTSOptimizationManager] Initialized"));
}

void UMingRTSOptimizationManager::Update(float DeltaTime)
{
    if (UnitManager)
    {
        UnitManager->ProcessUnitAI(DeltaTime);
    }
    
    if (bLODEnabled)
    {
        UpdateUnitLOD();
        UpdateBuildingLOD();
        CullDistantObjects();
    }
}

void UMingRTSOptimizationManager::EnableLODSystem(bool bEnable)
{
    bLODEnabled = bEnable;
}

void UMingRTSOptimizationManager::SetVisibilityCullingDistance(float Distance)
{
    LODDistance = Distance;
}

void UMingRTSOptimizationManager::PerformMemoryCleanup()
{
    UE_LOG(LogTemp, Log, TEXT("[RTSOptimizationManager] Performing memory cleanup"));
    
    // 觸發垃圾回收
    GEngine->ForceGarbageCollection(true);
    
    // 清理快取
    FMemory::Trim();
}

FString UMingRTSOptimizationManager::GetOptimizationReport() const
{
    FString Report = TEXT("RTS Optimization Report:\n");
    
    if (UnitManager)
    {
        Report += FString::Printf(TEXT("Active Units: %d\n"), UnitManager->GetActiveUnitCount());
    }
    
    Report += FString::Printf(TEXT("LOD Enabled: %s\n"), bLODEnabled ? TEXT("Yes") : TEXT("No"));
    Report += FString::Printf(TEXT("LOD Distance: %.0f\n"), LODDistance);
    
    // 記憶體統計
    FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
    Report += FString::Printf(TEXT("Memory Usage: %d MB\n"), 
        static_cast<int32>(MemoryStats.UsedPhysical / (1024 * 1024)));
    
    return Report;
}

void UMingRTSOptimizationManager::UpdateUnitLOD()
{
    // 根據距離更新單位 LOD
}

void UMingRTSOptimizationManager::UpdateBuildingLOD()
{
    // 根據距離更新建築 LOD
}

void UMingRTSOptimizationManager::CullDistantObjects()
{
    // 剔除遠處N物件
}
