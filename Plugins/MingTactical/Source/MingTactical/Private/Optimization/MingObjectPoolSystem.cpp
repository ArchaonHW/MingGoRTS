#include "Optimization/MingObjectPoolSystem.h"
#include "MingCombatAI.h"
#include "Units/MingTacticalUnit.h"
#include "Engine/World.h"

UMingObjectPoolSystem::UMingObjectPoolSystem()
    : MaxPoolSize(5000)
    , InitialUnitPoolSize(100)
    , InitialAIPoolSize(20)
    , AutoExpandThreshold(0.2f)
    , bAutoExpandEnabled(true)
    , bIsInitialized(false)
    , TotalCreatedCount(0)
    , TotalReusedCount(0)
    , TotalUnitAcquires(0)
    , TotalAIAcquires(0)
    , LastExpandTime(0.0f)
    , ExpandCooldown(5.0f)
{
}

void UMingObjectPoolSystem::Initialize(int32 InInitialUnitPoolSize, int32 InInitialAIPoolSize, int32 InMaxPoolSize)
{
    if (bIsInitialized) return;

    InitialUnitPoolSize = InInitialUnitPoolSize;
    InitialAIPoolSize = InInitialAIPoolSize;
    MaxPoolSize = InMaxPoolSize;

    // 预分配数组空间
    UnitPool.Reserve(MaxPoolSize);
    AIPool.Reserve(FMath::Min(InitialAIPoolSize * 2, 100));

    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("ObjectPoolSystem initialized - UnitPool: %d, AIPool: %d, Max: %d"),
        InitialUnitPoolSize, InitialAIPoolSize, MaxPoolSize);
}

void UMingObjectPoolSystem::Shutdown()
{
    if (!bIsInitialized) return;

    // 销毁所有单位
    for (auto& Item : UnitPool)
    {
        if (Item.Unit.IsValid())
        {
            DestroyUnit(Item.Unit.Get());
        }
    }

    // 销毁所有AI
    for (auto& Item : AIPool)
    {
        if (Item.AI.IsValid())
        {
            DestroyAI(Item.AI.Get());
        }
    }

    UnitPool.Empty();
    AIPool.Empty();

    bIsInitialized = false;
}

void UMingObjectPoolSystem::PrepopulatePools()
{
    if (!bIsInitialized) return;

    // 预创建单位
    int32 UnitsToCreate = InitialUnitPoolSize - GetAvailableUnitCount();
    for (int32 i = 0; i < UnitsToCreate; ++i)
    {
        AMingTacticalUnit* Unit = CreateNewUnit(nullptr);
        if (Unit)
        {
            FUnitPoolItem Item;
            Item.Unit = Unit;
            Item.State = EPoolObjectState::Available;
            Item.LastUsedTime = FPlatformTime::Seconds();
            UnitPool.Add(Item);
        }
    }

    // 预创建AI
    int32 AIsToCreate = InitialAIPoolSize - GetAvailableAICount();
    for (int32 i = 0; i < AIsToCreate; ++i)
    {
        AMingCombatAI* AI = CreateNewAI(nullptr);
        if (AI)
        {
            FAIPoolItem Item;
            Item.AI = AI;
            Item.State = EPoolObjectState::Available;
            Item.LastUsedTime = FPlatformTime::Seconds();
            AIPool.Add(Item);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Prepulated pools - Units: %d, AIs: %d"),
        UnitsToCreate, AIsToCreate);
}

AMingTacticalUnit* UMingObjectPoolSystem::AcquireUnit(UClass* UnitClass)
{
    if (!bIsInitialized) return nullptr;

    TotalUnitAcquires++;

    // 查找可用的单位
    int32 AvailableIndex = FindAvailableUnitIndex();
    
    if (AvailableIndex >= 0)
    {
        // 复用现有单位
        FUnitPoolItem& Item = UnitPool[AvailableIndex];
        Item.State = EPoolObjectState::InUse;
        Item.LastUsedTime = FPlatformTime::Seconds();
        Item.UseCount++;
        
        TotalReusedCount++;
        
        // 重置单位状态
        if (Item.Unit.IsValid())
        {
            ResetUnit(Item.Unit.Get());
            return Item.Unit.Get();
        }
    }

    // 没有可用单位，检查是否需要扩展
    if (bAutoExpandEnabled && UnitPool.Num() < MaxPoolSize)
    {
        CheckAutoExpand();
        
        // 再次尝试获取
        AvailableIndex = FindAvailableUnitIndex();
        if (AvailableIndex >= 0)
        {
            FUnitPoolItem& Item = UnitPool[AvailableIndex];
            Item.State = EPoolObjectState::InUse;
            Item.LastUsedTime = FPlatformTime::Seconds();
            Item.UseCount++;
            TotalReusedCount++;
            
            if (Item.Unit.IsValid())
            {
                ResetUnit(Item.Unit.Get());
                return Item.Unit.Get();
            }
        }
    }

    // 创建新单位（如果未达到上限）
    if (UnitPool.Num() < MaxPoolSize)
    {
        AMingTacticalUnit* Unit = CreateNewUnit(UnitClass);
        if (Unit)
        {
            FUnitPoolItem Item;
            Item.Unit = Unit;
            Item.State = EPoolObjectState::InUse;
            Item.LastUsedTime = FPlatformTime::Seconds();
            Item.UseCount = 1;
            UnitPool.Add(Item);
            
            TotalCreatedCount++;
            return Unit;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Object pool exhausted! Max size: %d"), MaxPoolSize);
    return nullptr;
}

void UMingObjectPoolSystem::ReturnUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit) return;

    int32 Index = FindUnitIndex(Unit);
    if (Index >= 0)
    {
        FUnitPoolItem& Item = UnitPool[Index];
        Item.State = EPoolObjectState::Available;
        Item.LastUsedTime = FPlatformTime::Seconds();
        
        // 重置单位状态
        ResetUnit(Unit);
        
        // 隐藏单位
        Unit->SetActorHiddenInGame(true);
        Unit->SetActorEnableCollision(false);
    }
}

TArray<AMingTacticalUnit*> UMingObjectPoolSystem::AcquireUnits(int32 Count, UClass* UnitClass)
{
    TArray<AMingTacticalUnit*> Result;
    Result.Reserve(Count);

    for (int32 i = 0; i < Count; ++i)
    {
        AMingTacticalUnit* Unit = AcquireUnit(UnitClass);
        if (Unit)
        {
            Result.Add(Unit);
        }
        else
        {
            // 池耗尽，返回已获取的
            break;
        }
    }

    return Result;
}

void UMingObjectPoolSystem::ReturnUnits(const TArray<AMingTacticalUnit*>& Units)
{
    for (AMingTacticalUnit* Unit : Units)
    {
        ReturnUnit(Unit);
    }
}

AMingCombatAI* UMingObjectPoolSystem::AcquireAI(UClass* AIClass)
{
    if (!bIsInitialized) return nullptr;

    TotalAIAcquires++;

    int32 AvailableIndex = FindAvailableAIIndex();
    
    if (AvailableIndex >= 0)
    {
        FAIPoolItem& Item = AIPool[AvailableIndex];
        Item.State = EPoolObjectState::InUse;
        Item.LastUsedTime = FPlatformTime::Seconds();
        Item.UseCount++;
        TotalReusedCount++;
        
        if (Item.AI.IsValid())
        {
            ResetAI(Item.AI.Get());
            return Item.AI.Get();
        }
    }

    // 创建新AI
    AMingCombatAI* AI = CreateNewAI(AIClass);
    if (AI)
    {
        FAIPoolItem Item;
        Item.AI = AI;
        Item.State = EPoolObjectState::InUse;
        Item.LastUsedTime = FPlatformTime::Seconds();
        Item.UseCount = 1;
        AIPool.Add(Item);
        
        TotalCreatedCount++;
        return AI;
    }

    return nullptr;
}

void UMingObjectPoolSystem::ReturnAI(AMingCombatAI* AI)
{
    if (!bIsInitialized || !AI) return;

    int32 Index = FindAIIndex(AI);
    if (Index >= 0)
    {
        FAIPoolItem& Item = AIPool[Index];
        Item.State = EPoolObjectState::Available;
        Item.LastUsedTime = FPlatformTime::Seconds();
        
        ResetAI(AI);
    }
}

void UMingObjectPoolSystem::ExpandUnitPool(int32 AdditionalCount)
{
    if (!bIsInitialized) return;
    
    float CurrentTime = FPlatformTime::Seconds();
    if (CurrentTime - LastExpandTime < ExpandCooldown)
    {
        return; // 冷却中
    }

    int32 ActualCount = FMath::Min(AdditionalCount, MaxPoolSize - UnitPool.Num());
    
    for (int32 i = 0; i < ActualCount; ++i)
    {
        AMingTacticalUnit* Unit = CreateNewUnit(nullptr);
        if (Unit)
        {
            FUnitPoolItem Item;
            Item.Unit = Unit;
            Item.State = EPoolObjectState::Available;
            Item.LastUsedTime = CurrentTime;
            UnitPool.Add(Item);
            TotalCreatedCount++;
        }
    }

    LastExpandTime = CurrentTime;
    
    UE_LOG(LogTemp, Log, TEXT("Expanded unit pool by %d (now: %d)"), 
        ActualCount, UnitPool.Num());
}

void UMingObjectPoolSystem::ExpandAIPool(int32 AdditionalCount)
{
    if (!bIsInitialized) return;

    for (int32 i = 0; i < AdditionalCount; ++i)
    {
        AMingCombatAI* AI = CreateNewAI(nullptr);
        if (AI)
        {
            FAIPoolItem Item;
            Item.AI = AI;
            Item.State = EPoolObjectState::Available;
            Item.LastUsedTime = FPlatformTime::Seconds();
            AIPool.Add(Item);
            TotalCreatedCount++;
        }
    }
}

void UMingObjectPoolSystem::ShrinkUnusedPools()
{
    if (!bIsInitialized) return;

    float CurrentTime = FPlatformTime::Seconds();
    float UnusedThreshold = 30.0f; // 30秒未使用

    // 清理单位池
    for (int32 i = UnitPool.Num() - 1; i >= 0; --i)
    {
        FUnitPoolItem& Item = UnitPool[i];
        if (Item.State == EPoolObjectState::Available)
        {
            if (CurrentTime - Item.LastUsedTime > UnusedThreshold)
            {
                if (Item.Unit.IsValid())
                {
                    DestroyUnit(Item.Unit.Get());
                }
                UnitPool.RemoveAt(i);
            }
        }
    }

    // 清理AI池
    for (int32 i = AIPool.Num() - 1; i >= 0; --i)
    {
        FAIPoolItem& Item = AIPool[i];
        if (Item.State == EPoolObjectState::Available)
        {
            if (CurrentTime - Item.LastUsedTime > UnusedThreshold)
            {
                if (Item.AI.IsValid())
                {
                    DestroyAI(Item.AI.Get());
                }
                AIPool.RemoveAt(i);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Shrunk pools - Units: %d, AIs: %d"),
        UnitPool.Num(), AIPool.Num());
}

void UMingObjectPoolSystem::ForceClearAll()
{
    if (!bIsInitialized) return;

    // 归还所有使用中的对象
    for (auto& Item : UnitPool)
    {
        if (Item.State == EPoolObjectState::InUse)
        {
            ReturnUnit(Item.Unit.Get());
        }
    }

    for (auto& Item : AIPool)
    {
        if (Item.State == EPoolObjectState::InUse)
        {
            ReturnAI(Item.AI.Get());
        }
    }

    // 销毁所有对象
    Shutdown();
}

void UMingObjectPoolSystem::SetMaxPoolSize(int32 NewMaxSize)
{
    MaxPoolSize = FMath::Max(100, NewMaxSize);
}

void UMingObjectPoolSystem::SetAutoExpandThreshold(float Percentage)
{
    AutoExpandThreshold = FMath::Clamp(Percentage, 0.0f, 1.0f);
}

void UMingObjectPoolSystem::SetAutoExpandEnabled(bool bEnabled)
{
    bAutoExpandEnabled = bEnabled;
}

int32 UMingObjectPoolSystem::GetAvailableUnitCount() const
{
    int32 Count = 0;
    for (const auto& Item : UnitPool)
    {
        if (Item.State == EPoolObjectState::Available)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingObjectPoolSystem::GetInUseUnitCount() const
{
    int32 Count = 0;
    for (const auto& Item : UnitPool)
    {
        if (Item.State == EPoolObjectState::InUse)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingObjectPoolSystem::GetAvailableAICount() const
{
    int32 Count = 0;
    for (const auto& Item : AIPool)
    {
        if (Item.State == EPoolObjectState::Available)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingObjectPoolSystem::GetInUseAICount() const
{
    int32 Count = 0;
    for (const auto& Item : AIPool)
    {
        if (Item.State == EPoolObjectState::InUse)
        {
            Count++;
        }
    }
    return Count;
}

float UMingObjectPoolSystem::GetUnitPoolUtilization() const
{
    if (UnitPool.Num() == 0) return 0.0f;
    return static_cast<float>(GetInUseUnitCount()) / UnitPool.Num();
}

float UMingObjectPoolSystem::GetAIPoolUtilization() const
{
    if (AIPool.Num() == 0) return 0.0f;
    return static_cast<float>(GetInUseAICount()) / AIPool.Num();
}

float UMingObjectPoolSystem::GetAverageReuseRate() const
{
    int32 TotalAcquires = TotalUnitAcquires + TotalAIAcquires;
    if (TotalAcquires == 0) return 0.0f;
    return static_cast<float>(TotalReusedCount) / TotalAcquires;
}

void UMingObjectPoolSystem::PrintDebugInfo()
{
    UE_LOG(LogTemp, Log, TEXT("=== Object Pool System ==="));
    UE_LOG(LogTemp, Log, TEXT("Unit Pool: %d total, %d available, %d in use (%.1f%%)"),
        GetTotalUnitPoolSize(), GetAvailableUnitCount(), GetInUseUnitCount(),
        GetUnitPoolUtilization() * 100.0f);
    UE_LOG(LogTemp, Log, TEXT("AI Pool: %d total, %d available, %d in use (%.1f%%)"),
        GetTotalAIPoolSize(), GetAvailableAICount(), GetInUseAICount(),
        GetAIPoolUtilization() * 100.0f);
    UE_LOG(LogTemp, Log, TEXT("Total Created: %d, Total Reused: %d"),
        TotalCreatedCount, TotalReusedCount);
    UE_LOG(LogTemp, Log, TEXT("Reuse Rate: %.1f%%"), GetAverageReuseRate() * 100.0f);
    UE_LOG(LogTemp, Log, TEXT("=========================="));
}

FString UMingObjectPoolSystem::GetDebugString() const
{
    return FString::Printf(TEXT("Units: %d/%d | AIs: %d/%d | Reuse: %.1f%%"),
        GetInUseUnitCount(), GetTotalUnitPoolSize(),
        GetInUseAICount(), GetTotalAIPoolSize(),
        GetAverageReuseRate() * 100.0f);
}

bool UMingObjectPoolSystem::ValidateIntegrity()
{
    bool bValid = true;

    // 验证单位池
    for (const auto& Item : UnitPool)
    {
        if (Item.State == EPoolObjectState::InUse && !Item.Unit.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("In-use unit is invalid"));
            bValid = false;
        }
    }

    // 验证AI池
    for (const auto& Item : AIPool)
    {
        if (Item.State == EPoolObjectState::InUse && !Item.AI.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("In-use AI is invalid"));
            bValid = false;
        }
    }

    return bValid;
}

void UMingObjectPoolSystem::DumpPoolContents()
{
    UE_LOG(LogTemp, Log, TEXT("=== Unit Pool Contents ==="));
    for (int32 i = 0; i < UnitPool.Num(); ++i)
    {
        const FUnitPoolItem& Item = UnitPool[i];
        UE_LOG(LogTemp, Log, TEXT("[%d] State: %s, UseCount: %d, Unit: %s"),
            i,
            *UEnum::GetValueAsString(Item.State),
            Item.UseCount,
            Item.Unit.IsValid() ? *Item.Unit->GetName() : TEXT("INVALID"));
    }

    UE_LOG(LogTemp, Log, TEXT("=== AI Pool Contents ==="));
    for (int32 i = 0; i < AIPool.Num(); ++i)
    {
        const FAIPoolItem& Item = AIPool[i];
        UE_LOG(LogTemp, Log, TEXT("[%d] State: %s, UseCount: %d, AI: %s"),
            i,
            *UEnum::GetValueAsString(Item.State),
            Item.UseCount,
            Item.AI.IsValid() ? *Item.AI->GetName() : TEXT("INVALID"));
    }
}

void UMingObjectPoolSystem::Tick(float DeltaTime)
{
    if (!bIsInitialized) return;

    static float CleanupTimer = 0.0f;
    CleanupTimer += DeltaTime;

    // 每10秒执行一次清理
    if (CleanupTimer >= 10.0f)
    {
        CleanupInvalidReferences();
        
        // 如果可用对象少于20%，自动扩展
        if (bAutoExpandEnabled)
        {
            CheckAutoExpand();
        }
        
        CleanupTimer = 0.0f;
    }
}

AMingTacticalUnit* UMingObjectPoolSystem::CreateNewUnit(UClass* UnitClass)
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    UClass* ClassToUse = UnitClass ? UnitClass : DefaultUnitClass.Get();
    if (!ClassToUse)
    {
        UE_LOG(LogTemp, Warning, TEXT("No unit class specified for pool"));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    AMingTacticalUnit* Unit = World->SpawnActor<AMingTacticalUnit>(ClassToUse, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    
    if (Unit)
    {
        // 初始状态设置为隐藏
        Unit->SetActorHiddenInGame(true);
        Unit->SetActorEnableCollision(false);
    }

    return Unit;
}

AMingCombatAI* UMingObjectPoolSystem::CreateNewAI(UClass* AIClass)
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    UClass* ClassToUse = AIClass ? AIClass : DefaultAIClass.Get();
    if (!ClassToUse)
    {
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    AMingCombatAI* AI = World->SpawnActor<AMingCombatAI>(ClassToUse, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    return AI;
}

void UMingObjectPoolSystem::ResetUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;

    // 重置单位状态
    Unit->SetActorLocation(FVector::ZeroVector);
    Unit->SetActorRotation(FRotator::ZeroRotator);
    
    // 这里可以添加更多的重置逻辑
    // 例如：清除命令队列、重置生命值等
}

void UMingObjectPoolSystem::ResetAI(AMingCombatAI* AI)
{
    if (!AI) return;

    // 重置AI状态
    // 这里可以添加更多的重置逻辑
}

void UMingObjectPoolSystem::DestroyUnit(AMingTacticalUnit* Unit)
{
    if (Unit)
    {
        Unit->Destroy();
    }
}

void UMingObjectPoolSystem::DestroyAI(AMingCombatAI* AI)
{
    if (AI)
    {
        AI->Destroy();
    }
}

void UMingObjectPoolSystem::CheckAutoExpand()
{
    if (UnitPool.Num() >= MaxPoolSize) return;

    int32 Available = GetAvailableUnitCount();
    float AvailableRatio = UnitPool.Num() > 0 ? static_cast<float>(Available) / UnitPool.Num() : 0.0f;

    if (AvailableRatio < AutoExpandThreshold)
    {
        // 可用对象不足，自动扩展
        int32 ExpandAmount = FMath::Min(InitialUnitPoolSize / 2, MaxPoolSize - UnitPool.Num());
        if (ExpandAmount > 0)
        {
            ExpandUnitPool(ExpandAmount);
        }
    }
}

void UMingObjectPoolSystem::CleanupInvalidReferences()
{
    // 清理单位池中的无效引用
    for (auto& Item : UnitPool)
    {
        if (!Item.Unit.IsValid())
        {
            Item.State = EPoolObjectState::Unused;
        }
    }

    // 清理AI池中的无效引用
    for (auto& Item : AIPool)
    {
        if (!Item.AI.IsValid())
        {
            Item.State = EPoolObjectState::Unused;
        }
    }
}

int32 UMingObjectPoolSystem::FindAvailableUnitIndex() const
{
    for (int32 i = 0; i < UnitPool.Num(); ++i)
    {
        if (UnitPool[i].State == EPoolObjectState::Available && UnitPool[i].Unit.IsValid())
        {
            return i;
        }
    }
    return -1;
}

int32 UMingObjectPoolSystem::FindAvailableAIIndex() const
{
    for (int32 i = 0; i < AIPool.Num(); ++i)
    {
        if (AIPool[i].State == EPoolObjectState::Available && AIPool[i].AI.IsValid())
        {
            return i;
        }
    }
    return -1;
}

int32 UMingObjectPoolSystem::FindUnitIndex(AMingTacticalUnit* Unit) const
{
    if (!Unit) return -1;

    for (int32 i = 0; i < UnitPool.Num(); ++i)
    {
        if (UnitPool[i].Unit.Get() == Unit)
        {
            return i;
        }
    }
    return -1;
}

int32 UMingObjectPoolSystem::FindAIIndex(AMingCombatAI* AI) const
{
    if (!AI) return -1;

    for (int32 i = 0; i < AIPool.Num(); ++i)
    {
        if (AIPool[i].AI.Get() == AI)
        {
            return i;
        }
    }
    return -1;
}
