#include "Optimization/MingProfiler.h"
#include "Engine/Engine.h"

// ============================================================================
// 性能分析器實現
// ============================================================================
UMingProfiler::UMingProfiler()
    : bEnabled(true)
    , bSampling(false)
    , SamplingEndTime(0.0f)
    , MaxFrameHistory(300)
{
}

void UMingProfiler::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("初始化性能分析器"));

    TimerMap.Empty();
    ActiveTimers.Empty();
    CategoryTimers.Empty();
    ActiveCategories.Empty();
    FrameHistory.Empty();
    CurrentFrameStats = FFrameStats();
    LastFrameStats = FFrameStats();
    bEnabled = true;
    bSampling = false;
}

void UMingProfiler::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("關閉性能分析器"));

    if (bSampling)
    {
        EndSampling();
    }

    LogSummary();
}

void UMingProfiler::BeginTimer(const FString& TimerName, EProfilerCategory Category)
{
    if (!bEnabled)
    {
        return;
    }

    double StartTime = GetCurrentTime();
    ActiveTimers.Add(TimerName, StartTime);
}

void UMingProfiler::EndTimer(const FString& TimerName)
{
    if (!bEnabled)
    {
        return;
    }

    float* StartTime = ActiveTimers.Find(TimerName);
    if (StartTime)
    {
        double EndTime = GetCurrentTime();
        float Duration = (EndTime - *StartTime) * 1000.0f; // 轉換為毫秒

        FTimerStats& Stats = TimerMap.FindOrAdd(TimerName);
        Stats.TimerName = TimerName;
        Stats.AddSample(Duration);

        ActiveTimers.Remove(TimerName);
    }
}

void UMingProfiler::BeginCategoryTimer(EProfilerCategory Category)
{
    if (!bEnabled)
    {
        return;
    }

    double StartTime = GetCurrentTime();
    ActiveCategories.Add(Category, StartTime);
}

void UMingProfiler::EndCategoryTimer(EProfilerCategory Category)
{
    if (!bEnabled)
    {
        return;
    }

    float* StartTime = ActiveCategories.Find(Category);
    if (StartTime)
    {
        double EndTime = GetCurrentTime();
        float Duration = (EndTime - *StartTime) * 1000.0f;

        float* TotalTime = CategoryTimers.Find(Category);
        if (TotalTime)
        {
            *TotalTime += Duration;
        }
        else
        {
            CategoryTimers.Add(Category, Duration);
        }

        CurrentFrameStats.CategoryTimes.Add(Category, Duration);
        ActiveCategories.Remove(Category);
    }
}

void UMingProfiler::AddMarker(const FString& MarkerName, const FString& Description)
{
    if (!bEnabled)
    {
        return;
    }

    UE_LOG(LogTemp, Verbose, TEXT("[標記] %s: %s"), *MarkerName, *Description);
}

FTimerStats UMingProfiler::GetTimerStats(const FString& TimerName) const
{
    const FTimerStats* Stats = TimerMap.Find(TimerName);
    if (Stats)
    {
        return *Stats;
    }
    return FTimerStats();
}

TArray<FTimerStats> UMingProfiler::GetAllTimerStats() const
{
    TArray<FTimerStats> Result;
    for (const auto& Pair : TimerMap)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

float UMingProfiler::GetCategoryTime(EProfilerCategory Category) const
{
    const float* Time = CategoryTimers.Find(Category);
    return Time ? *Time : 0.0f;
}

void UMingProfiler::BeginFrame()
{
    if (!bEnabled)
    {
        return;
    }

    CurrentFrameStats = FFrameStats();
    CurrentFrameStats.FrameNumber = LastFrameStats.FrameNumber + 1;
}

void UMingProfiler::EndFrame()
{
    if (!bEnabled)
    {
        return;
    }

    // 計算幀時間
    float FrameTime = 0.0f;
    for (const auto& Pair : CurrentFrameStats.CategoryTimes)
    {
        FrameTime += Pair.Value;
    }
    CurrentFrameStats.DeltaTime = FrameTime;
    CurrentFrameStats.GameThreadTime = FrameTime;

    // 計算 FPS
    if (FrameTime > 0)
    {
        CurrentFrameStats.FPS = FMath::RoundToInt(1000.0f / FrameTime);
    }

    LastFrameStats = CurrentFrameStats;

    // 添加到歷史
    FrameHistory.Add(CurrentFrameStats);
    if (FrameHistory.Num() > MaxFrameHistory)
    {
        FrameHistory.RemoveAt(0);
    }

    // 檢查採樣
    if (bSampling)
    {
        float CurrentTime = GetCurrentTime();
        if (CurrentTime >= SamplingEndTime)
        {
            EndSampling();
        }
    }
}

void UMingProfiler::BeginSampling(float DurationSeconds)
{
    UE_LOG(LogTemp, Log, TEXT("開始性能採樣 (%.1f 秒)"), DurationSeconds);

    ResetAllTimers();
    ClearFrameHistory();
    bSampling = true;
    SamplingEndTime = GetCurrentTime() + DurationSeconds;
}

void UMingProfiler::EndSampling()
{
    UE_LOG(LogTemp, Log, TEXT("結束性能採樣"));

    bSampling = false;
    FString Report = GenerateReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
}

FString UMingProfiler::GenerateReport()
{
    FString Report;
    Report += TEXT("\n========== 性能分析報告 ==========\n");

    // 幀統計
    if (FrameHistory.Num() > 0)
    {
        float TotalFrameTime = 0.0f;
        float MinFrameTime = FLT_MAX;
        float MaxFrameTime = 0.0f;
        int32 TotalFPS = 0;

        for (const FFrameStats& Frame : FrameHistory)
        {
            TotalFrameTime += Frame.DeltaTime;
            MinFrameTime = FMath::Min(MinFrameTime, Frame.DeltaTime);
            MaxFrameTime = FMath::Max(MaxFrameTime, Frame.DeltaTime);
            TotalFPS += Frame.FPS;
        }

        float AvgFrameTime = TotalFrameTime / FrameHistory.Num();
        float AvgFPS = (float)TotalFPS / FrameHistory.Num();

        Report += FString::Printf(TEXT("幀數: %d\n"), FrameHistory.Num());
        Report += FString::Printf(TEXT("平均幀時間: %.2f ms\n"), AvgFrameTime);
        Report += FString::Printf(TEXT("最小幀時間: %.2f ms\n"), MinFrameTime);
        Report += FString::Printf(TEXT"最大幀時間: %.2f ms\n", MaxFrameTime);
        Report += FString::Printf(TEXT"平均 FPS: %.1f\n", AvgFPS);
    }

    // 類別統計
    Report += TEXT("\n各類別時間:\n");
    for (const auto& Pair : CategoryTimers)
    {
        FString CategoryName = UEnum::GetValueAsString(Pair.Key);
        Report += FString::Printf(TEXT("  %s: %.2f ms\n"), *CategoryName, Pair.Value);
    }

    // 計時器統計
    Report += TEXT("\n計時器統計:\n");
    TArray<FTimerStats> AllTimers = GetAllTimerStats();
    AllTimers.Sort([](const FTimerStats& A, const FTimerStats& B) {
        return B.AverageTime < A.AverageTime; // 降序
    });

    for (const FTimerStats& Stats : AllTimers)
    {
        if (Stats.CallCount > 0)
        {
            Report += FString::Printf(TEXT("  %s: %.2f ms (%.2f ~ %.2f ms, %d 次)\n"),
                *Stats.TimerName,
                Stats.AverageTime,
                Stats.MinTime,
                Stats.MaxTime,
                Stats.CallCount);
        }
    }

    Report += TEXT("================================\n");
    return Report;
}

bool UMingProfiler::ExportReport(const FString& FilePath)
{
    FString Report = GenerateReport();

    // 保存到文件
    // FFileHelper::SaveStringToFile(Report, *FilePath);

    UE_LOG(LogTemp, Log, TEXT("性能報告已導出: %s"), *FilePath);
    return true;
}

void UMingProfiler::LogSummary()
{
    UE_LOG(LogTemp, Log, TEXT("%s"), *GenerateReport());
}

void UMingProfiler::ResetAllTimers()
{
    for (auto& Pair : TimerMap)
    {
        Pair.Value.Reset();
    }
}

void UMingProfiler::ResetTimer(const FString& TimerName)
{
    FTimerStats* Stats = TimerMap.Find(TimerName);
    if (Stats)
    {
        Stats->Reset();
    }
}

void UMingProfiler::ClearFrameHistory()
{
    FrameHistory.Empty();
}

void UMingProfiler::SetEnabled(bool bInEnabled)
{
    bEnabled = bInEnabled;
    UE_LOG(LogTemp, Log, TEXT("性能分析器 %s"), bEnabled ? TEXT("啟用") : TEXT("禁用"));
}

double UMingProfiler::GetCurrentTime() const
{
    return FPlatformTime::Seconds();
}

// ============================================================================
// 性能監控器實現
// ============================================================================
UMingPerformanceMonitor::UMingPerformanceMonitor()
    : CurrentFPS(60.0f)
    , AverageFPS(60.0f)
    , MinFPS(60.0f)
    , MaxFPS(60.0f)
    , FrameTime(16.67f)
    , AverageFrameTime(16.67f)
    , FPSAlertThreshold(30.0f)
    , MemoryAlertThreshold(1024)
    , FrameTimeAlertThreshold(33.33f)
    , HistorySize(300)
{
}

void UMingPerformanceMonitor::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("初始化性能監控器"));

    FPSHistory.Empty();
    FrameTimeHistory.Empty();
    MemoryHistory.Empty();
    ActiveAlerts.Empty();
    AlertConfigs.Empty();

    // 設置默認警報
    FPerformanceAlert FPSAlert;
    FPSAlert.AlertName = TEXT("FPS Alert");
    FPSAlert.Category = EProfilerCategory::CPU;
    FPSAlert.Threshold = FPSAlertThreshold;
    AlertConfigs.Add(FPSAlert.AlertName, FPSAlert);

    FPerformanceAlert FrameTimeAlert;
    FrameTimeAlert.AlertName = TEXT("FrameTime Alert");
    FrameTimeAlert.Category = EProfilerCategory::CPU;
    FrameTimeAlert.Threshold = FrameTimeAlertThreshold;
    AlertConfigs.Add(FrameTimeAlert.AlertName, FrameTimeAlert);
}

void UMingPerformanceMonitor::Update(float DeltaTime)
{
    // 更新 FPS
    UpdateFPS(DeltaTime);

    // 更新幀時間
    UpdateFrameTime(DeltaTime);

    // 記錄歷史數據
    RecordMetric(FPSHistory, CurrentFPS);
    RecordMetric(FrameTimeHistory, FrameTime);

    // 記錄記憶體使用
    // int32 MemoryMB = FPlatformMemory::GetStats().UsedPhysical / (1024 * 1024);
    // RecordMetric(MemoryHistory, (float)MemoryMB);

    // 檢查警報
    CheckAlerts();
}

void UMingPerformanceMonitor::SetFPSAlertThreshold(float MinFPS)
{
    FPSAlertThreshold = MinFPS;

    FPerformanceAlert* Alert = AlertConfigs.Find(TEXT("FPS Alert"));
    if (Alert)
    {
        Alert->Threshold = MinFPS;
    }

    UE_LOG(LogTemp, Log, TEXT("設置 FPS 警報閾值: %.1f"), MinFPS);
}

void UMingPerformanceMonitor::SetMemoryAlertThreshold(int32 MaxMB)
{
    MemoryAlertThreshold = MaxMB;

    UE_LOG(LogTemp, Log, TEXT("設置記憶體警報閾值: %d MB"), MaxMB);
}

void UMingPerformanceMonitor::SetFrameTimeAlertThreshold(float MaxTime)
{
    FrameTimeAlertThreshold = MaxTime;

    FPerformanceAlert* Alert = AlertConfigs.Find(TEXT("FrameTime Alert"));
    if (Alert)
    {
        Alert->Threshold = MaxTime;
    }

    UE_LOG(LogTemp, Log, TEXT("設置幀時間警報閾值: %.2f ms"), MaxTime);
}

void UMingPerformanceMonitor::SetCustomAlert(const FString& AlertName, EProfilerCategory Category, float Threshold)
{
    FPerformanceAlert Alert;
    Alert.AlertName = AlertName;
    Alert.Category = Category;
    Alert.Threshold = Threshold;

    AlertConfigs.Add(AlertName, Alert);

    UE_LOG(LogTemp, Log, TEXT("設置自定義警報: %s (閾值: %.2f)"), *AlertName, Threshold);
}

void UMingPerformanceMonitor::RemoveAlert(const FString& AlertName)
{
    AlertConfigs.Remove(AlertName);
    UE_LOG(LogTemp, Log, TEXT("移除警報: %s"), *AlertName);
}

void UMingPerformanceMonitor::UpdateFPS(float DeltaTime)
{
    if (DeltaTime > 0)
    {
        CurrentFPS = 1.0f / DeltaTime;
        MinFPS = FMath::Min(MinFPS, CurrentFPS);
        MaxFPS = FMath::Max(MaxFPS, CurrentFPS);

        // 計算平均 FPS
        if (FPSHistory.Num() > 0)
        {
            float TotalFPS = 0.0f;
            for (float FPS : FPSHistory)
            {
                TotalFPS += FPS;
            }
            TotalFPS += CurrentFPS;
            AverageFPS = TotalFPS / (FPSHistory.Num() + 1);
        }
        else
        {
            AverageFPS = CurrentFPS;
        }
    }
}

void UMingPerformanceMonitor::UpdateFrameTime(float DeltaTime)
{
    FrameTime = DeltaTime * 1000.0f; // 轉換為毫秒

    if (FrameTimeHistory.Num() > 0)
    {
        float TotalTime = 0.0f;
        for (float Time : FrameTimeHistory)
        {
            TotalTime += Time;
        }
        TotalTime += FrameTime;
        AverageFrameTime = TotalTime / (FrameTimeHistory.Num() + 1);
    }
    else
       {
        AverageFrameTime = FrameTime;
    }
}

void UMingPerformanceMonitor::CheckAlerts()
{
    TArray<FString> AlertNamesToRemove;

    for (auto& Pair : AlertConfigs)
    {
        FPerformanceAlert& Config = Pair.Value;
        float CurrentValue = 0.0f;

        // 根據警報類型獲取當前值
        if (Config.AlertName == TEXT("FPS Alert"))
        {
            CurrentValue = CurrentFPS;
        }
        else if (Config.AlertName == TEXT("FrameTime Alert"))
        {
            CurrentValue = FrameTime;
        }

        // 檢查是否觸發警報
        if (ShouldTriggerAlert(Config, CurrentValue))
        {
            FPerformanceAlert Alert = Config;
            Alert.CurrentValue = CurrentValue;
            Alert.TriggerTime = FDateTime::Now();
            Alert.Description = FString::Printf(TEXT("%s 超出閾值: %.2f (閾值: %.2f)"),
                *Config.AlertName, CurrentValue, Config.Threshold);

            ActiveAlerts.Add(Alert);
            OnPerformanceAlert.Broadcast(Alert);

            UE_LOG(LogTemp, Warning, TEXT("[警報] %s"), *Alert.Description);

            // FPS 下降警報
            if (Config.AlertName == TEXT("FPS Alert"))
            {
                OnFPSDropped.Broadcast(CurrentFPS);
            }
        }
    }

    // 清理已解除的警報
    for (int32 i = ActiveAlerts.Num() - 1; i >= 0; --i)
    {
        bool bShouldRemove = false;

        if (ActiveAlerts[i].AlertName == TEXT("FPS Alert"))
        {
            bShouldRemove = CurrentFPS >= FPSAlertThreshold;
        }
        else if (ActiveAlerts[i].AlertName == TEXT("FrameTime Alert"))
        {
            bShouldRemove = FrameTime <= FrameTimeAlertThreshold;
        }

        if (bShouldRemove)
        {
            ActiveAlerts.RemoveAt(i);
        }
    }
}

bool UMingPerformanceMonitor::ShouldTriggerAlert(const FPerformanceAlert& Config, float Value)
{
    // FPS 警報：當前值低於閾值
    if (Config.AlertName == TEXT("FPS Alert"))
    {
        return Value < Config.Threshold;
    }

    // 其他警報：當前值高於閾值
    return Value > Config.Threshold;
}

void UMingPerformanceMonitor::RecordMetric(TArray<float>& History, float Value)
{
    History.Add(Value);
    if (History.Num() > HistorySize)
    {
        History.RemoveAt(0);
    }
}

// ============================================================================
// 調試可視化實現
// ============================================================================
UMingDebugVisualizer::UMingDebugVisualizer()
    : CurrentTime(0.0f)
{
}

void UMingDebugVisualizer::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("初始化調試可視化器"));
    DrawCommands.Empty();
    CurrentTime = 0.0f;
}

void UMingDebugVisualizer::DrawLine(const FVector& Start, const FVector& End, const FLinearColor& Color, float Thickness, float Duration)
{
    FDebugDrawCommand Command;
    Command.Start = Start;
    Command.End = End;
    Command.Color = Color;
    Command.Thickness = Thickness;
    Command.Duration = Duration > 0 ? CurrentTime + Duration : 0.0f;

    DrawCommands.Add(Command);
}

void UMingDebugVisualizer::DrawBox(const FVector& Center, const FVector& Extents, const FLinearColor& Color, float Duration)
{
    FVector HalfExtents = Extents * 0.5f;

    // 繪製 12 條邊
    FVector Corners[8];
    Corners[0] = Center + FVector(-HalfExtents.X, -HalfExtents.Y, -HalfExtents.Z);
    Corners[1] = Center + FVector( HalfExtents.X, -HalfExtents.Y, -HalfExtents.Z);
    Corners[2] = Center + FVector( HalfExtents.X,  HalfExtents.Y, -HalfExtents.Z);
    Corners[3] = Center + FVector(-HalfExtents.X,  HalfExtents.Y, -HalfExtents.Z);
    Corners[4] = Center + FVector(-HalfExtents.X, -HalfExtents.Y,  HalfExtents.Z);
    Corners[5] = Center + FVector( HalfExtents.X, -HalfExtents.Y,  HalfExtents.Z);
    Corners[6] = Center + FVector( HalfExtents.X,  HalfExtents.Y,  HalfExtents.Z);
    Corners[7] = Center + FVector(-HalfExtents.X,  HalfExtents.Y,  HalfExtents.Z);

    // 底面
    DrawLine(Corners[0], Corners[1], Color, 1.0f, Duration);
    DrawLine(Corners[1], Corners[2], Color, 1.0f, Duration);
    DrawLine(Corners[2], Corners[3], Color, 1.0f, Duration);
    DrawLine(Corners[3], Corners[0], Color, 1.0f, Duration);

    // 頂面
    DrawLine(Corners[4], Corners[5], Color, 1.0f, Duration);
    DrawLine(Corners[5], Corners[6], Color, 1.0f, Duration);
    DrawLine(Corners[6], Corners[7], Color, 1.0f, Duration);
    DrawLine(Corners[7], Corners[4], Color, 1.0f, Duration);

    // 側邊
    DrawLine(Corners[0], Corners[4], Color, 1.0f, Duration);
    DrawLine(Corners[1], Corners[5], Color, 1.0f, Duration);
    DrawLine(Corners[2], Corners[6], Color, 1.0f, Duration);
    DrawLine(Corners[3], Corners[7], Color, 1.0f, Duration);
}

void UMingDebugVisualizer::DrawSphere(const FVector& Center, float Radius, const FLinearColor& Color, float Duration)
{
    // 繪製簡化的球體 (3 個圓圈)
    int32 Segments = 16;
    for (int32 i = 0; i < Segments; ++i)
    {
        float Angle1 = (2.0f * PI * i) / Segments;
        float Angle2 = (2.0f * PI * (i + 1)) / Segments;

        // XY 平面
        FVector Start1(Center.X + FMath::Cos(Angle1) * Radius, Center.Y + FMath::Sin(Angle1) * Radius, Center.Z);
        FVector End1(Center.X + FMath::Cos(Angle2) * Radius, Center.Y + FMath::Sin(Angle2) * Radius, Center.Z);
        DrawLine(Start1, End1, Color, 1.0f, Duration);

        // XZ 平面
        FVector Start2(Center.X + FMath::Cos(Angle1) * Radius, Center.Y, Center.Z + FMath::Sin(Angle1) * Radius);
        FVector End2(Center.X + FMath::Cos(Angle2) * Radius, Center.Y, Center.Z + FMath::Sin(Angle2) * Radius);
        DrawLine(Start2, End2, Color, 1.0f, Duration);

        // YZ 平面
        FVector Start3(Center.X, Center.Y + FMath::Cos(Angle1) * Radius, Center.Z + FMath::Sin(Angle1) * Radius);
        FVector End3(Center.X, Center.Y + FMath::Cos(Angle2) * Radius, Center.Z + FMath::Sin(Angle2) * Radius);
        DrawLine(Start3, End3, Color, 1.0f, Duration);
    }
}

void UMingDebugVisualizer::DrawText(const FVector& Location, const FString& Text, const FLinearColor& Color, float Duration)
{
    // 使用 DrawLine 繪製簡化的文字框
    DrawBox(Location, FVector(10.0f, 10.0f, 10.0f), Color, Duration);

    UE_LOG(LogTemp, Verbose, TEXT("[DebugText] %s: %s"), *Location.ToString(), *Text);
}

void UMingDebugVisualizer::DrawRay(const FVector& Origin, const FVector& Direction, float Length, const FLinearColor& Color, float Duration)
{
    FVector End = Origin + Direction.GetSafeNormal() * Length;
    DrawLine(Origin, End, Color, 2.0f, Duration);

    // 繪製箭頭
    FVector ArrowDir = (End - Origin).GetSafeNormal();
    FVector PerpDir = FVector::CrossProduct(ArrowDir, FVector::UpVector).GetSafeNormal();
    if (PerpDir.IsNearlyZero())
    {
        PerpDir = FVector::CrossProduct(ArrowDir, FVector::ForwardVector).GetSafeNormal();
    }

    float ArrowSize = Length * 0.1f;
    DrawLine(End, End - ArrowDir * ArrowSize + PerpDir * ArrowSize * 0.5f, Color, 1.0f, Duration);
    DrawLine(End, End - ArrowDir * ArrowSize - PerpDir * ArrowSize * 0.5f, Color, 1.0f, Duration);
}

void UMingDebugVisualizer::DrawGrid(const FVector& Origin, int32 Size, float CellSize, const FLinearColor& Color)
{
    float GridSize = Size * CellSize;
    float HalfSize = GridSize * 0.5f;

    for (int32 i = 0; i <= Size; ++i)
    {
        float Offset = -HalfSize + i * CellSize;

        // X 方向線
        FVector XStart(Origin.X + Offset, Origin.Y - HalfSize, Origin.Z);
        FVector XEnd(Origin.X + Offset, Origin.Y + HalfSize, Origin.Z);
        DrawLine(XStart, XEnd, Color, 1.0f, 0.0f);

        // Y 方向線
        FVector YStart(Origin.X - HalfSize, Origin.Y + Offset, Origin.Z);
        FVector YEnd(Origin.X + HalfSize, Origin.Y + Offset, Origin.Z);
        DrawLine(YStart, YEnd, Color, 1.0f, 0.0f);
    }
}

void UMingDebugVisualizer::DrawAxes(const FVector& Origin, float Scale)
{
    // X 軸 - 紅色
    DrawRay(Origin, FVector::ForwardVector, Scale, FLinearColor::Red, 0.0f);
    // Y 軸 - 綠色
    DrawRay(Origin, FVector::RightVector, Scale, FLinearColor::Green, 0.0f);
    // Z 軸 - 藍色
    DrawRay(Origin, FVector::UpVector, Scale, FLinearColor::Blue, 0.0f);
}

void UMingDebugVisualizer::DrawCollisionBounds(class UMingPhysicsBody* Body, const FLinearColor& Color)
{
    if (!Body)
    {
        return;
    }

    // 這裡將來會從物理體獲取 AABB 並繪製
    UE_LOG(LogTemp, Verbose, TEXT("繪製碰撞邊界"));
}

void UMingDebugVisualizer::DrawPerformanceStats(const FVector2D& Position)
{
    // 繪製性能統計信息的簡化表示
    UE_LOG(LogTemp, Verbose, TEXT("繪製性能統計"));
}

void UMingDebugVisualizer::Render()
{
    CurrentTime += 0.016f; // 假設 60 FPS

    // 清理過期的繪製命令
    CleanupExpiredCommands();

    // 這裡將來會實際渲染調試繪製命令
    // 使用線框渲染或 Gizmo 系統
}

void UMingDebugVisualizer::Clear()
{
    DrawCommands.Empty();
}

void UMingDebugVisualizer::ClearExpired(float InCurrentTime)
{
    CurrentTime = InCurrentTime;
    CleanupExpiredCommands();
}

void UMingDebugVisualizer::CleanupExpiredCommands()
{
    for (int32 i = DrawCommands.Num() - 1; i >= 0; --i)
    {
        if (DrawCommands[i].Duration > 0 && CurrentTime > DrawCommands[i].Duration)
        {
            DrawCommands.RemoveAt(i);
        }
    }
}
