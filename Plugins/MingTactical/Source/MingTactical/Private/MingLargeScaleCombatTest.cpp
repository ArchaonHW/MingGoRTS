#include "MingLargeScaleCombatTest.h"
#include "Units/MingTacticalUnit.h"
#include "Managers/MingSelectionManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

UMingLargeScaleCombatTest::UMingLargeScaleCombatTest()
    : bIsTestRunning(false)
    , TestStartTime(0.0f)
    , TotalFrameTime(0.0f)
    , FrameCount(0)
    , AverageFPS(0.0f)
    , MinFPS(999.0f)
    , MaxFPS(0.0f)
    , LastFrameTime(0.0f)
    , SelectionManager(nullptr)
{
}

void UMingLargeScaleCombatTest::InitializeTest()
{
    if (SelectionManager)
    {
        return; // 已經初始化
    }

    // 獲取選擇管理器
    SelectionManager = UMingSelectionManager::Get();
    if (!SelectionManager)
    {
        UE_LOG(LogTemp, Error, TEXT("SelectionManager not available for performance test"));
        return;
    }

    // 確保選擇管理器已初始化
    SelectionManager->Initialize();

    UE_LOG(LogTemp, Log, TEXT("LargeScaleCombatTest initialized"));
}

void UMingLargeScaleCombatTest::CleanupTest()
{
    StopPerformanceTest();

    // 銷毀所有測試單位
    for (AMingTacticalUnit* Unit : TestUnits)
    {
        if (Unit)
        {
            Unit->Destroy();
        }
    }
    TestUnits.Empty();

    UE_LOG(LogTemp, Log, TEXT("LargeScaleCombatTest cleaned up"));
}

void UMingLargeScaleCombatTest::CreateTestUnits(int32 UnitCount)
{
    if (!SelectionManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create units: SelectionManager not initialized"));
        return;
    }

    // 清理現有單位
    CleanupTest();

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create units: No valid world"));
        return;
    }

    // 計算網格布局
    const int32 GridSize = FMath::CeilToInt(FMath::Sqrt(UnitCount));
    const float Spacing = 200.0f; // 單位間距
    const FVector StartLocation = FVector(-GridSize * Spacing / 2, -GridSize * Spacing / 2, 0.0f);

    UE_LOG(LogTemp, Log, TEXT("Creating %d test units in %dx%d grid"), UnitCount, GridSize, GridSize);

    // 創建單位
    for (int32 i = 0; i < UnitCount; ++i)
    {
        const int32 X = i % GridSize;
        const int32 Y = i / GridSize;
        const FVector Location = StartLocation + FVector(X * Spacing, Y * Spacing, 0.0f);

        // 交替創建不同隊伍的單位
        const int32 TeamId = (i < UnitCount / 2) ? 1 : 2;
        const EUnitType UnitType = static_cast<EUnitType>(i % 5); // 循環使用不同單位類型

        AMingTacticalUnit* Unit = CreateTestUnit(Location, TeamId, UnitType);
        if (Unit)
        {
            TestUnits.Add(Unit);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Created %d test units successfully"), TestUnits.Num());
}

AMingTacticalUnit* UMingLargeScaleCombatTest::CreateTestUnit(const FVector& Location, int32 TeamId, EUnitType UnitType)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AMingTacticalUnit* Unit = World->SpawnActor<AMingTacticalUnit>(AMingTacticalUnit::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
    if (Unit)
    {
        // 初始化單位
        Unit->InitializeUnit(UnitType, TeamId);

        // 根據單位類型調整屬性
        switch (UnitType)
        {
        case EUnitType::Infantry:
            Unit->UnitStats.MoveSpeed = 250.0f;
            Unit->UnitStats.AttackDamage = 15.0f;
            Unit->UnitStats.MaxHealth = 100.0f;
            break;
        case EUnitType::Cavalry:
            Unit->UnitStats.MoveSpeed = 400.0f;
            Unit->UnitStats.AttackDamage = 25.0f;
            Unit->UnitStats.MaxHealth = 150.0f;
            break;
        case EUnitType::Artillery:
            Unit->UnitStats.MoveSpeed = 150.0f;
            Unit->UnitStats.AttackDamage = 50.0f;
            Unit->UnitStats.AttackRange = 300.0f;
            Unit->UnitStats.MaxHealth = 75.0f;
            break;
        case EUnitType::Support:
            Unit->UnitStats.MoveSpeed = 200.0f;
            Unit->UnitStats.AttackDamage = 10.0f;
            Unit->UnitStats.MaxHealth = 80.0f;
            break;
        case EUnitType::Commander:
            Unit->UnitStats.MoveSpeed = 300.0f;
            Unit->UnitStats.AttackDamage = 30.0f;
            Unit->UnitStats.MaxHealth = 200.0f;
            Unit->UnitStats.SightRange = 800.0f;
            break;
        }

        Unit->UnitStats.CurrentHealth = Unit->UnitStats.MaxHealth;
    }

    return Unit;
}

void UMingLargeScaleCombatTest::StartPerformanceTest()
{
    if (bIsTestRunning)
    {
        UE_LOG(LogTemp, Warning, TEXT("Performance test already running"));
        return;
    }

    if (TestUnits.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot start test: No units created"));
        return;
    }

    ResetPerformanceStats();
    bIsTestRunning = true;
    TestStartTime = GetWorld()->GetTimeSeconds();

    UE_LOG(LogTemp, Log, TEXT("Starting performance test with %d units"), TestUnits.Num());

    // 設置Tick以監控性能
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            Tick(0.016f); // 模擬60 FPS
        });
    }
}

void UMingLargeScaleCombatTest::StopPerformanceTest()
{
    if (!bIsTestRunning)
    {
        return;
    }

    bIsTestRunning = false;
    
    const float TestDuration = GetWorld()->GetTimeSeconds() - TestStartTime;
    
    UE_LOG(LogTemp, Log, TEXT("Performance test stopped after %.2f seconds"), TestDuration);
    UE_LOG(LogTemp, Log, TEXT("Performance Results:"));
    UE_LOG(LogTemp, Log, TEXT("  Average FPS: %.2f"), AverageFPS);
    UE_LOG(LogTemp, Log, TEXT("  Min FPS: %.2f"), MinFPS);
    UE_LOG(LogTemp, Log, TEXT("  Max FPS: %.2f"), MaxFPS);
    UE_LOG(LogTemp, Log, TEXT("  Frame Count: %d"), FrameCount);
    UE_LOG(LogTemp, Log, TEXT("  Test %s"), IsPerformanceTestPass() ? TEXT("PASSED") : TEXT("FAILED"));
}

void UMingLargeScaleCombatTest::Tick(float DeltaTime)
{
    if (!bIsTestRunning)
    {
        return;
    }

    UpdatePerformanceStats(DeltaTime);
    SimulateUnitBehavior(DeltaTime);

    // 繼續下一幀
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick([this]()
        {
            Tick(0.016f); // 模擬60 FPS
        });
    }
}

void UMingLargeScaleCombatTest::UpdatePerformanceStats(float DeltaTime)
{
    FrameCount++;
    TotalFrameTime += DeltaTime;
    LastFrameTime = DeltaTime;

    // 計算當前FPS
    const float CurrentFPS = 1.0f / DeltaTime;
    
    // 更新FPS統計
    MinFPS = FMath::Min(MinFPS, CurrentFPS);
    MaxFPS = FMath::Max(MaxFPS, CurrentFPS);
    AverageFPS = TotalFrameTime > 0.0f ? FrameCount / TotalFrameTime : 0.0f;

    // 每100幀輸出一次統計
    if (FrameCount % 100 == 0)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Frame %d: FPS=%.2f, Avg=%.2f, Min=%.2f, Max=%.2f"), 
            FrameCount, CurrentFPS, AverageFPS, MinFPS, MaxFPS);
    }
}

void UMingLargeScaleCombatTest::ResetPerformanceStats()
{
    TotalFrameTime = 0.0f;
    FrameCount = 0;
    AverageFPS = 0.0f;
    MinFPS = 999.0f;
    MaxFPS = 0.0f;
    LastFrameTime = 0.0f;
}

void UMingLargeScaleCombatTest::SimulateUnitBehavior(float DeltaTime)
{
    // 每5秒執行一次不同的測試
    static float TestTimer = 0.0f;
    TestTimer += DeltaTime;

    if (TestTimer >= 5.0f)
    {
        TestTimer = 0.0f;

        // 循環執行不同測試
        static int32 TestPhase = 0;
        TestPhase = (TestPhase + 1) % 3;

        switch (TestPhase)
        {
        case 0:
            TestUnitSelection();
            break;
        case 1:
            TestUnitMovement();
            break;
        case 2:
            TestUnitCombat();
            break;
        }
    }
}

void UMingLargeScaleCombatTest::TestUnitSelection()
{
    if (!SelectionManager || TestUnits.Num() == 0)
    {
        return;
    }

    // 隨機選擇一些單位
    const int32 SelectionCount = FMath::Min(50, TestUnits.Num() / 4);
    TArray<AMingTacticalUnit*> UnitsToSelect;

    for (int32 i = 0; i < SelectionCount; ++i)
    {
        const int32 RandomIndex = FMath::RandRange(0, TestUnits.Num() - 1);
        AMingTacticalUnit* Unit = TestUnits[RandomIndex];
        if (Unit && Unit->IsAlive())
        {
            UnitsToSelect.Add(Unit);
        }
    }

    if (UnitsToSelect.Num() > 0)
    {
        SelectionManager->SelectUnits(UnitsToSelect, false);
        UE_LOG(LogTemp, Verbose, TEXT("Selected %d units for selection test"), UnitsToSelect.Num());
    }
}

void UMingLargeScaleCombatTest::TestUnitMovement()
{
    if (TestUnits.Num() == 0)
    {
        return;
    }

    // 讓一半的單位移動到隨機位置
    const int32 MoveCount = TestUnits.Num() / 2;
    
    for (int32 i = 0; i < MoveCount; ++i)
    {
        AMingTacticalUnit* Unit = TestUnits[i];
        if (Unit && Unit->IsAlive() && Unit->GetMovementComponent())
        {
            // 生成隨機目標位置
            const FVector RandomOffset = FVector(
                FMath::RandRange(-500.0f, 500.0f),
                FMath::RandRange(-500.0f, 500.0f),
                0.0f
            );
            const FVector TargetLocation = Unit->GetActorLocation() + RandomOffset;

            Unit->GetMovementComponent()->MoveToLocation(TargetLocation, false);
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("Moved %d units for movement test"), MoveCount);
}

void UMingLargeScaleCombatTest::TestUnitCombat()
{
    if (TestUnits.Num() < 2)
    {
        return;
    }

    // 讓不同隊伍的單位互相攻擊
    int32 CombatCount = 0;
    
    for (int32 i = 0; i < TestUnits.Num() && CombatCount < 20; ++i)
    {
        AMingTacticalUnit* Attacker = TestUnits[i];
        if (!Attacker || !Attacker->IsAlive() || Attacker->TeamId != 1)
        {
            continue;
        }

        // 尋找敵方目標
        for (int32 j = 0; j < TestUnits.Num(); ++j)
        {
            AMingTacticalUnit* Target = TestUnits[j];
            if (Target && Target->IsAlive() && Target->TeamId == 2)
            {
                // 檢查是否在攻擊範圍內
                const float Distance = FVector::Dist(Attacker->GetActorLocation(), Target->GetActorLocation());
                if (Distance <= Attacker->UnitStats.AttackRange)
                {
                    Attacker->GetCombatComponent()->AttackTarget(Target);
                    CombatCount++;
                    break;
                }
            }
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("Initiated %d combat actions"), CombatCount);
}

bool UMingLargeScaleCombatTest::IsPerformanceTestPass() const
{
    // 性能標準：平均FPS >= 30，最低FPS >= 20
    return AverageFPS >= 30.0f && MinFPS >= 20.0f;
}
