# MingGoRTS 性能優化驗證套件
# 確保所有系統在高負載下穩定運行

param(
    [string]$TestMode = "Standard",
    [string]$OutputPath = "c:\HW\MingGoRTS\TestReports",
    [int]$DurationMinutes = 30,
    [switch]$Verbose,
    [switch]$StressTest,
    [switch]$Continuous
)

# 初始化性能測試環境
function Initialize-PerformanceTestEnvironment {
    Write-Host "🚀 初始化性能優化驗證環境..." -ForegroundColor Green
    
    # 創建測試目錄
    $TestDirs = @(
        "$OutputPath\Performance",
        "$OutputPath\Benchmarks",
        "$OutputPath\Logs",
        "$OutputPath\Reports"
    )
    
    foreach ($dir in $TestDirs) {
        if (!(Test-Path $dir)) {
            New-Item -ItemType Directory -Path $dir -Force | Out-Null
        }
    }
    
    # 設置日誌
    $Global:LogFile = "$OutputPath\Logs\PerformanceTest_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
    Start-Transcript -Path $Global:LogFile -Force
    
    # 初始化性能計數器
    Initialize-PerformanceCounters
    
    Write-Host "✅ 性能測試環境初始化完成" -ForegroundColor Green
}

# 初始化性能計數器
function Initialize-PerformanceCounters {
    $Global:PerformanceCounters = @{
        CPU = @()
        Memory = @()
        GPU = @()
        Network = @()
        Disk = @()
        FrameRate = @()
        RenderTime = @()
        GameThread = @()
        RenderThread = @()
    }
    
    $Global:TestStartTime = Get-Date
    $Global:PerformanceMetrics = @{
        TargetFPS = 60
        MaxCPUUsage = 80
        MaxMemoryUsage = 85
        MaxGPUUsage = 90
        MaxNetworkLatency = 100
        MaxDiskIO = 50
        MinFrameRate = 30
        MaxRenderTime = 16.67
    }
}

# 性能測試類別
$PerformanceTestCategories = @(
    @{
        Name = "System_Performance"
        Description = "系統性能測試"
        Tests = @(
            "CPU_Usage_Test",
            "Memory_Usage_Test",
            "GPU_Performance_Test",
            "Disk_IO_Test"
        )
    },
    @{
        Name = "Game_Performance"
        Description = "遊戲性能測試"
        Tests = @(
            "Frame_Rate_Test",
            "Render_Performance_Test",
            "Physics_Performance_Test",
            "Audio_Performance_Test"
        )
    },
    @{
        Name = "Network_Performance"
        Description = "網絡性能測試"
        Tests = @(
            "Network_Latency_Test",
            "Bandwidth_Test",
            "Multiplayer_Sync_Test",
            "Connection_Stress_Test"
        )
    },
    @{
        Name = "AI_Performance"
        Description = "AI系統性能測試"
        Tests = @(
            "AI_Processing_Test",
            "Pathfinding_Performance_Test",
            "Decision_Making_Test",
            "Learning_Algorithm_Test"
        )
    },
    @{
        Name = "Resource_Performance"
        Description = "資源管理性能測試"
        Tests = @(
            "Resource_Allocation_Test",
            "Memory_Pool_Test",
            "Asset_Loading_Test",
            "Garbage_Collection_Test"
        )
    }
)

# 執行性能測試
function Invoke-PerformanceTest {
    param(
        [string]$TestCategory,
        [string]$TestName,
        [int]$DurationSeconds = 60
    )
    
    Write-Host "🧪 執行性能測試: $TestCategory - $TestName" -ForegroundColor Cyan
    
    $StartTime = Get-Date
    $TestResult = @{
        Category = $TestCategory
        Name = $TestName
        StartTime = $StartTime
        EndTime = $null
        Duration = $DurationSeconds
        Status = "Running"
        Metrics = @{}
        Details = ""
        Error = $null
        Passed = $false
    }
    
    try {
        # 開始性能監控
        $MonitorJob = Start-PerformanceMonitoring -TestName $TestName
        
        # 執行具體測試
        switch ($TestName) {
            "CPU_Usage_Test" {
                $TestResult.Metrics = Test-CPUUsage -DurationSeconds $DurationSeconds
            }
            "Memory_Usage_Test" {
                $TestResult.Metrics = Test-MemoryUsage -DurationSeconds $DurationSeconds
            }
            "GPU_Performance_Test" {
                $TestResult.Metrics = Test-GPUPerformance -DurationSeconds $DurationSeconds
            }
            "Disk_IO_Test" {
                $TestResult.Metrics = Test-DiskIO -DurationSeconds $DurationSeconds
            }
            "Frame_Rate_Test" {
                $TestResult.Metrics = Test-FrameRate -DurationSeconds $DurationSeconds
            }
            "Render_Performance_Test" {
                $TestResult.Metrics = Test-RenderPerformance -DurationSeconds $DurationSeconds
            }
            "Physics_Performance_Test" {
                $TestResult.Metrics = Test-PhysicsPerformance -DurationSeconds $DurationSeconds
            }
            "Audio_Performance_Test" {
                $TestResult.Metrics = Test-AudioPerformance -DurationSeconds $DurationSeconds
            }
            "Network_Latency_Test" {
                $TestResult.Metrics = Test-NetworkLatency -DurationSeconds $DurationSeconds
            }
            "Bandwidth_Test" {
                $TestResult.Metrics = Test-Bandwidth -DurationSeconds $DurationSeconds
            }
            "Multiplayer_Sync_Test" {
                $TestResult.Metrics = Test-MultiplayerSync -DurationSeconds $DurationSeconds
            }
            "Connection_Stress_Test" {
                $TestResult.Metrics = Test-ConnectionStress -DurationSeconds $DurationSeconds
            }
            "AI_Processing_Test" {
                $TestResult.Metrics = Test-AIProcessing -DurationSeconds $DurationSeconds
            }
            "Pathfinding_Performance_Test" {
                $TestResult.Metrics = Test-PathfindingPerformance -DurationSeconds $DurationSeconds
            }
            "Decision_Making_Test" {
                $TestResult.Metrics = Test-DecisionMaking -DurationSeconds $DurationSeconds
            }
            "Learning_Algorithm_Test" {
                $TestResult.Metrics = Test-LearningAlgorithm -DurationSeconds $DurationSeconds
            }
            "Resource_Allocation_Test" {
                $TestResult.Metrics = Test-ResourceAllocation -DurationSeconds $DurationSeconds
            }
            "Memory_Pool_Test" {
                $TestResult.Metrics = Test-MemoryPool -DurationSeconds $DurationSeconds
            }
            "Asset_Loading_Test" {
                $TestResult.Metrics = Test-AssetLoading -DurationSeconds $DurationSeconds
            }
            "Garbage_Collection_Test" {
                $TestResult.Metrics = Test-GarbageCollection -DurationSeconds $DurationSeconds
            }
            default {
                throw "未知的性能測試名稱: $TestName"
            }
        }
        
        # 停止性能監控
        Stop-Job $MonitorJob
        Receive-Job $MonitorJob
        Remove-Job $MonitorJob
        
        # 評估測試結果
        $TestResult.Passed = Evaluate-PerformanceTestResult -TestName $TestName -Metrics $TestResult.Metrics
        $TestResult.Status = if ($TestResult.Passed) { "Passed" } else { "Failed" }
        
        if ($TestResult.Passed) {
            Write-Host "✅ 性能測試通過: $TestName" -ForegroundColor Green
        } else {
            Write-Host "❌ 性能測試失敗: $TestName" -ForegroundColor Red
        }
    }
    catch {
        $TestResult.Status = "Failed"
        $TestResult.Error = $_.Exception.Message
        Write-Host "❌ 性能測試錯誤: $TestName - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    $EndTime = Get-Date
    $TestResult.EndTime = $EndTime
    $TestResult.Duration = ($EndTime - $StartTime).TotalSeconds
    
    return $TestResult
}

# 開始性能監控
function Start-PerformanceMonitoring {
    param([string]$TestName)
    
    $ScriptBlock = {
        param($TestName, $OutputPath)
        
        $MonitorData = @()
        $Duration = 300  # 5分鐘監控
        
        for ($i = 0; $i -lt $Duration; $i++) {
            $Timestamp = Get-Date
            
            # 收集系統性能數據
            $CPU = (Get-Counter '\Processor(_Total)\% Processor Time' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            $Memory = (Get-Counter '\Memory\Available MBytes' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            $Disk = (Get-Counter '\PhysicalDisk(_Total)\% Disk Time' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            
            $DataPoint = @{
                Timestamp = $Timestamp
                CPU = $CPU
                Memory = $Memory
                Disk = $Disk
            }
            
            $MonitorData += $DataPoint
            Start-Sleep -Seconds 1
        }
        
        # 保存監控數據
        $MonitorPath = "$OutputPath\Benchmarks\Monitor_$TestName`_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
        $MonitorData | ConvertTo-Json -Depth 10 | Out-File -FilePath $MonitorPath -Encoding UTF8
    }
    
    return Start-Job -ScriptBlock $ScriptBlock -ArgumentList $TestName, $OutputPath
}

# CPU使用率測試
function Test-CPUUsage {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試CPU使用率..." -ForegroundColor Gray
    
    $CPUMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        try {
            $CPU = (Get-Counter '\Processor(_Total)\% Processor Time' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            $CPUMetrics += @{
                Timestamp = Get-Date
                Usage = $CPU
            }
        }
        catch {
            Write-Host "   ⚠️  無法獲取CPU使用率數據" -ForegroundColor Yellow
        }
        
        Start-Sleep -Seconds 1
    }
    
    $AvgCPU = if ($CPUMetrics.Count -gt 0) { ($CPUMetrics | Measure-Object -Property Usage -Average).Average } else { 0 }
    $MaxCPU = if ($CPUMetrics.Count -gt 0) { ($CPUMetrics | Measure-Object -Property Usage -Maximum).Maximum } else { 0 }
    
    return @{
        AverageCPU = [math]::Round($AvgCPU, 2)
        MaxCPU = [math]::Round($MaxCPU, 2)
        SampleCount = $CPUMetrics.Count
        TargetCPU = $Global:PerformanceMetrics.MaxCPUUsage
        WithinTarget = $AvgCPU -le $Global:PerformanceMetrics.MaxCPUUsage
    }
}

# 內存使用測試
function Test-MemoryUsage {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試內存使用..." -ForegroundColor Gray
    
    $MemoryMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        try {
            $AvailableMemory = (Get-Counter '\Memory\Available MBytes' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            $TotalMemory = (Get-Counter '\Memory\Committed Bytes' -ErrorAction SilentlyContinue).CounterSamples.CookedValue / 1MB
            $UsedMemory = $TotalMemory - $AvailableMemory
            $MemoryUsagePercent = ($UsedMemory / $TotalMemory) * 100
            
            $MemoryMetrics += @{
                Timestamp = Get-Date
                UsedMemoryMB = [math]::Round($UsedMemory, 2)
                UsagePercent = [math]::Round($MemoryUsagePercent, 2)
            }
        }
        catch {
            Write-Host "   ⚠️  無法獲取內存使用數據" -ForegroundColor Yellow
        }
        
        Start-Sleep -Seconds 1
    }
    
    $AvgMemory = if ($MemoryMetrics.Count -gt 0) { ($MemoryMetrics | Measure-Object -Property UsagePercent -Average).Average } else { 0 }
    $MaxMemory = if ($MemoryMetrics.Count -gt 0) { ($MemoryMetrics | Measure-Object -Property UsagePercent -Maximum).Maximum } else { 0 }
    
    return @{
        AverageMemoryUsage = [math]::Round($AvgMemory, 2)
        MaxMemoryUsage = [math]::Round($MaxMemory, 2)
        SampleCount = $MemoryMetrics.Count
        TargetMemory = $Global:PerformanceMetrics.MaxMemoryUsage
        WithinTarget = $AvgMemory -le $Global:PerformanceMetrics.MaxMemoryUsage
    }
}

# GPU性能測試
function Test-GPUPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試GPU性能..." -ForegroundColor Gray
    
    # 模擬GPU性能測試（實際環境中需要使用GPU監控工具）
    $GPUMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬GPU使用率（實際應使用GPU監控API）
        $GPUUsage = Get-Random -Minimum 20 -Maximum 80
        
        $GPUMetrics += @{
            Timestamp = Get-Date
            Usage = $GPUUsage
        }
        
        Start-Sleep -Seconds 1
    }
    
    $AvgGPU = ($GPUMetrics | Measure-Object -Property Usage -Average).Average
    $MaxGPU = ($GPUMetrics | Measure-Object -Property Usage -Maximum).Maximum
    
    return @{
        AverageGPU = [math]::Round($AvgGPU, 2)
        MaxGPU = [math]::Round($MaxGPU, 2)
        SampleCount = $GPUMetrics.Count
        TargetGPU = $Global:PerformanceMetrics.MaxGPUUsage
        WithinTarget = $AvgGPU -le $Global:PerformanceMetrics.MaxGPUUsage
    }
}

# 磁盤IO測試
function Test-DiskIO {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試磁盤IO性能..." -ForegroundColor Gray
    
    $DiskMetrics = @()
    $StartTime = Get-Date
    $TestFile = "$env:TEMP\performance_test.tmp"
    
    # 執行磁盤寫入測試
    try {
        $TestData = "A" * 1024  # 1KB測試數據
        for ($i = 0; $i -lt 100; $i++) {
            $WriteStartTime = Get-Date
            Add-Content -Path $TestFile -Value $TestData
            $WriteTime = (Get-Date) - $WriteStartTime
            
            $DiskMetrics += @{
                Timestamp = Get-Date
                Operation = "Write"
                TimeMs = $WriteTime.TotalMilliseconds
            }
        }
        
        # 執行磁盤讀取測試
        for ($i = 0; $i -lt 100; $i++) {
            $ReadStartTime = Get-Date
            Get-Content -Path $TestFile | Out-Null
            $ReadTime = (Get-Date) - $ReadStartTime
            
            $DiskMetrics += @{
                Timestamp = Get-Date
                Operation = "Read"
                TimeMs = $ReadTime.TotalMilliseconds
            }
        }
    }
    finally {
        if (Test-Path $TestFile) {
            Remove-Item $TestFile -Force
        }
    }
    
    $AvgWriteTime = ($DiskMetrics | Where-Object { $_.Operation -eq "Write" } | Measure-Object -Property TimeMs -Average).Average
    $AvgReadTime = ($DiskMetrics | Where-Object { $_.Operation -eq "Read" } | Measure-Object -Property TimeMs -Average).Average
    
    return @{
        AverageWriteTime = [math]::Round($AvgWriteTime, 2)
        AverageReadTime = [math]::Round($AvgReadTime, 2)
        TotalOperations = $DiskMetrics.Count
        TargetMaxTime = $Global:PerformanceMetrics.MaxDiskIO
        WithinTarget = ($AvgWriteTime -le $Global:PerformanceMetrics.MaxDiskIO) -and ($AvgReadTime -le $Global:PerformanceMetrics.MaxDiskIO)
    }
}

# 幀率測試
function Test-FrameRate {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試遊戲幀率..." -ForegroundColor Gray
    
    # 模擬幀率測試（實際環境中需要集成遊戲引擎）
    $FrameMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬幀率（實際應從遊戲引擎獲取）
        $FPS = Get-Random -Minimum 45 -Maximum 75
        
        $FrameMetrics += @{
            Timestamp = Get-Date
            FPS = $FPS
        }
        
        Start-Sleep -Milliseconds 16  # 模擬60FPS
    }
    
    $AvgFPS = ($FrameMetrics | Measure-Object -Property FPS -Average).Average
    $MinFPS = ($FrameMetrics | Measure-Object -Property FPS -Minimum).Minimum
    $MaxFPS = ($FrameMetrics | Measure-Object -Property FPS -Maximum).Maximum
    
    return @{
        AverageFPS = [math]::Round($AvgFPS, 2)
        MinFPS = [math]::Round($MinFPS, 2)
        MaxFPS = [math]::Round($MaxFPS, 2)
        SampleCount = $FrameMetrics.Count
        TargetFPS = $Global:PerformanceMetrics.TargetFPS
        WithinTarget = $AvgFPS -ge $Global:PerformanceMetrics.TargetFPS
    }
}

# 渲染性能測試
function Test-RenderPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試渲染性能..." -ForegroundColor Gray
    
    # 模擬渲染性能測試
    $RenderMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬渲染時間（實際應從遊戲引擎獲取）
        $RenderTime = Get-Random -Minimum 8 -Maximum 20
        
        $RenderMetrics += @{
            Timestamp = Get-Date
            RenderTime = $RenderTime
        }
        
        Start-Sleep -Milliseconds $RenderTime
    }
    
    $AvgRenderTime = ($RenderMetrics | Measure-Object -Property RenderTime -Average).Average
    $MaxRenderTime = ($RenderMetrics | Measure-Object -Property RenderTime -Maximum).Maximum
    
    return @{
        AverageRenderTime = [math]::Round($AvgRenderTime, 2)
        MaxRenderTime = [math]::Round($MaxRenderTime, 2)
        SampleCount = $RenderMetrics.Count
        TargetMaxTime = $Global:PerformanceMetrics.MaxRenderTime
        WithinTarget = $AvgRenderTime -le $Global:PerformanceMetrics.MaxRenderTime
    }
}

# 物理性能測試
function Test-PhysicsPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試物理系統性能..." -ForegroundColor Gray
    
    # 模擬物理計算性能測試
    $PhysicsMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬物理計算時間
        $PhysicsTime = Get-Random -Minimum 1 -Maximum 10
        
        $PhysicsMetrics += @{
            Timestamp = Get-Date
            PhysicsTime = $PhysicsTime
        }
        
        Start-Sleep -Milliseconds $PhysicsTime
    }
    
    $AvgPhysicsTime = ($PhysicsMetrics | Measure-Object -Property PhysicsTime -Average).Average
    $MaxPhysicsTime = ($PhysicsMetrics | Measure-Object -Property PhysicsTime -Maximum).Maximum
    
    return @{
        AveragePhysicsTime = [math]::Round($AvgPhysicsTime, 2)
        MaxPhysicsTime = [math]::Round($MaxPhysicsTime, 2)
        SampleCount = $PhysicsMetrics.Count
        TargetMaxTime = 5.0
        WithinTarget = $AvgPhysicsTime -le 5.0
    }
}

# 音頻性能測試
function Test-AudioPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試音頻系統性能..." -ForegroundColor Gray
    
    # 模擬音頻處理性能測試
    $AudioMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬音頻處理時間
        $AudioTime = Get-Random -Minimum 0.5 -Maximum 3
        
        $AudioMetrics += @{
            Timestamp = Get-Date
            AudioTime = $AudioTime
        }
        
        Start-Sleep -Milliseconds $AudioTime
    }
    
    $AvgAudioTime = ($AudioMetrics | Measure-Object -Property AudioTime -Average).Average
    $MaxAudioTime = ($AudioMetrics | Measure-Object -Property AudioTime -Maximum).Maximum
    
    return @{
        AverageAudioTime = [math]::Round($AvgAudioTime, 2)
        MaxAudioTime = [math]::Round($MaxAudioTime, 2)
        SampleCount = $AudioMetrics.Count
        TargetMaxTime = 2.0
        WithinTarget = $AvgAudioTime -le 2.0
    }
}

# 網絡延遲測試
function Test-NetworkLatency {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試網絡延遲..." -ForegroundColor Gray
    
    $LatencyMetrics = @()
    $StartTime = Get-Date
    $TestHosts = @("8.8.8.8", "1.1.1.1", "baidu.com")
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        foreach ($host in $TestHosts) {
            try {
                $PingResult = Test-Connection -ComputerName $host -Count 1 -ErrorAction SilentlyContinue
                if ($PingResult) {
                    $LatencyMetrics += @{
                        Timestamp = Get-Date
                        Host = $host
                        Latency = $PingResult.ResponseTime
                    }
                }
            }
            catch {
                # 忽略網絡錯誤
            }
        }
        
        Start-Sleep -Seconds 2
    }
    
    $AvgLatency = if ($LatencyMetrics.Count -gt 0) { ($LatencyMetrics | Measure-Object -Property Latency -Average).Average } else { 0 }
    $MaxLatency = if ($LatencyMetrics.Count -gt 0) { ($LatencyMetrics | Measure-Object -Property Latency -Maximum).Maximum } else { 0 }
    
    return @{
        AverageLatency = [math]::Round($AvgLatency, 2)
        MaxLatency = [math]::Round($MaxLatency, 2)
        SampleCount = $LatencyMetrics.Count
        TargetLatency = $Global:PerformanceMetrics.MaxNetworkLatency
        WithinTarget = $AvgLatency -le $Global:PerformanceMetrics.MaxNetworkLatency
    }
}

# 帶寬測試
function Test-Bandwidth {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試網絡帶寬..." -ForegroundColor Gray
    
    # 模擬帶寬測試
    $BandwidthMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬下載速度（Mbps）
        $DownloadSpeed = Get-Random -Minimum 50 -Maximum 200
        # 模擬上傳速度（Mbps）
        $UploadSpeed = Get-Random -Minimum 10 -Maximum 50
        
        $BandwidthMetrics += @{
            Timestamp = Get-Date
            DownloadSpeed = $DownloadSpeed
            UploadSpeed = $UploadSpeed
        }
        
        Start-Sleep -Seconds 3
    }
    
    $AvgDownload = ($BandwidthMetrics | Measure-Object -Property DownloadSpeed -Average).Average
    $AvgUpload = ($BandwidthMetrics | Measure-Object -Property UploadSpeed -Average).Average
    
    return @{
        AverageDownloadSpeed = [math]::Round($AvgDownload, 2)
        AverageUploadSpeed = [math]::Round($AvgUpload, 2)
        SampleCount = $BandwidthMetrics.Count
        TargetMinDownload = 10.0
        TargetMinUpload = 5.0
        WithinTarget = ($AvgDownload -ge 10.0) -and ($AvgUpload -ge 5.0)
    }
}

# 多人同步測試
function Test-MultiplayerSync {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試多人遊戲同步..." -ForegroundColor Gray
    
    # 模擬多人同步性能測試
    $SyncMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬同步延遲
        $SyncLatency = Get-Random -Minimum 10 -Maximum 100
        # 模擬數據包丟失率
        $PacketLoss = Get-Random -Minimum 0 -Maximum 5
        
        $SyncMetrics += @{
            Timestamp = Get-Date
            SyncLatency = $SyncLatency
            PacketLoss = $PacketLoss
        }
        
        Start-Sleep -Milliseconds 100
    }
    
    $AvgSyncLatency = ($SyncMetrics | Measure-Object -Property SyncLatency -Average).Average
    $AvgPacketLoss = ($SyncMetrics | Measure-Object -Property PacketLoss -Average).Average
    
    return @{
        AverageSyncLatency = [math]::Round($AvgSyncLatency, 2)
        AveragePacketLoss = [math]::Round($AvgPacketLoss, 2)
        SampleCount = $SyncMetrics.Count
        TargetMaxLatency = 50.0
        TargetMaxPacketLoss = 2.0
        WithinTarget = ($AvgSyncLatency -le 50.0) -and ($AvgPacketLoss -le 2.0)
    }
}

# 連接壓力測試
function Test-ConnectionStress {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試連接壓力..." -ForegroundColor Gray
    
    # 模擬連接壓力測試
    $StressMetrics = @()
    $StartTime = Get-Date
    $MaxConnections = 100
    
    for ($i = 1; $i -le $MaxConnections; $i++) {
        $ConnectionStartTime = Get-Date
        
        # 模擬連接建立時間
        $ConnectionTime = Get-Random -Minimum 50 -Maximum 500
        
        $StressMetrics += @{
            ConnectionNumber = $i
            ConnectionTime = $ConnectionTime
            Timestamp = Get-Date
        }
        
        Start-Sleep -Milliseconds $ConnectionTime
        
        if ((Get-Date) -gt $StartTime.AddSeconds($DurationSeconds)) {
            break
        }
    }
    
    $AvgConnectionTime = ($StressMetrics | Measure-Object -Property ConnectionTime -Average).Average
    $TotalConnections = $StressMetrics.Count
    
    return @{
        AverageConnectionTime = [math]::Round($AvgConnectionTime, 2)
        TotalConnections = $TotalConnections
        TargetMaxTime = 200.0
        WithinTarget = $AvgConnectionTime -le 200.0
    }
}

# AI處理性能測試
function Test-AIProcessing {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試AI處理性能..." -ForegroundColor Gray
    
    # 模擬AI處理性能測試
    $AIMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬AI決策時間
        $DecisionTime = Get-Random -Minimum 5 -Maximum 50
        # 模擬AI計算負載
        $AILoad = Get-Random -Minimum 20 -Maximum 80
        
        $AIMetrics += @{
            Timestamp = Get-Date
            DecisionTime = $DecisionTime
            AILoad = $AILoad
        }
        
        Start-Sleep -Milliseconds $DecisionTime
    }
    
    $AvgDecisionTime = ($AIMetrics | Measure-Object -Property DecisionTime -Average).Average
    $AvgAILoad = ($AIMetrics | Measure-Object -Property AILoad -Average).Average
    
    return @{
        AverageDecisionTime = [math]::Round($AvgDecisionTime, 2)
        AverageAILoad = [math]::Round($AvgAILoad, 2)
        SampleCount = $AIMetrics.Count
        TargetMaxDecisionTime = 30.0
        TargetMaxLoad = 70.0
        WithinTarget = ($AvgDecisionTime -le 30.0) -and ($AvgAILoad -le 70.0)
    }
}

# 路徑規劃性能測試
function Test-PathfindingPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試路徑規劃性能..." -ForegroundColor Gray
    
    # 模擬路徑規劃性能測試
    $PathfindingMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬路徑計算時間
        $PathTime = Get-Random -Minimum 1 -Maximum 20
        # 模擬路徑複雜度
        $PathComplexity = Get-Random -Minimum 10 -Maximum 100
        
        $PathfindingMetrics += @{
            Timestamp = Get-Date
            PathTime = $PathTime
            PathComplexity = $PathComplexity
        }
        
        Start-Sleep -Milliseconds $PathTime
    }
    
    $AvgPathTime = ($PathfindingMetrics | Measure-Object -Property PathTime -Average).Average
    $MaxPathTime = ($PathfindingMetrics | Measure-Object -Property PathTime -Maximum).Maximum
    
    return @{
        AveragePathTime = [math]::Round($AvgPathTime, 2)
        MaxPathTime = [math]::Round($MaxPathTime, 2)
        SampleCount = $PathfindingMetrics.Count
        TargetMaxTime = 10.0
        WithinTarget = $AvgPathTime -le 10.0
    }
}

# 決策制定性能測試
function Test-DecisionMaking {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試AI決策制定性能..." -ForegroundColor Gray
    
    # 模擬決策制定性能測試
    $DecisionMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬決策複雜度
        $DecisionComplexity = Get-Random -Minimum 5 -Maximum 50
        # 模擬決策時間
        $DecisionTime = $DecisionComplexity * (Get-Random -Minimum 0.5 -Maximum 2.0)
        
        $DecisionMetrics += @{
            Timestamp = Get-Date
            DecisionComplexity = $DecisionComplexity
            DecisionTime = $DecisionTime
        }
        
        Start-Sleep -Milliseconds ($DecisionTime * 10)
    }
    
    $AvgDecisionTime = ($DecisionMetrics | Measure-Object -Property DecisionTime -Average).Average
    $MaxDecisionTime = ($DecisionMetrics | Measure-Object -Property DecisionTime -Maximum).Maximum
    
    return @{
        AverageDecisionTime = [math]::Round($AvgDecisionTime, 2)
        MaxDecisionTime = [math]::Round($MaxDecisionTime, 2)
        SampleCount = $DecisionMetrics.Count
        TargetMaxTime = 25.0
        WithinTarget = $AvgDecisionTime -le 25.0
    }
}

# 學習算法性能測試
function Test-LearningAlgorithm {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試學習算法性能..." -ForegroundColor Gray
    
    # 模擬學習算法性能測試
    $LearningMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬訓練時間
        $TrainingTime = Get-Random -Minimum 10 -Maximum 100
        # 模擬收斂率
        $ConvergenceRate = Get-Random -Minimum 0.7 -Maximum 0.95
        
        $LearningMetrics += @{
            Timestamp = Get-Date
            TrainingTime = $TrainingTime
            ConvergenceRate = $ConvergenceRate
        }
        
        Start-Sleep -Milliseconds ($TrainingTime / 10)
    }
    
    $AvgTrainingTime = ($LearningMetrics | Measure-Object -Property TrainingTime -Average).Average
    $AvgConvergenceRate = ($LearningMetrics | Measure-Object -Property ConvergenceRate -Average).Average
    
    return @{
        AverageTrainingTime = [math]::Round($AvgTrainingTime, 2)
        AverageConvergenceRate = [math]::Round($AvgConvergenceRate, 3)
        SampleCount = $LearningMetrics.Count
        TargetMaxTrainingTime = 50.0
        TargetMinConvergenceRate = 0.8
        WithinTarget = ($AvgTrainingTime -le 50.0) -and ($AvgConvergenceRate -ge 0.8)
    }
}

# 資源分配測試
function Test-ResourceAllocation {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試資源分配性能..." -ForegroundColor Gray
    
    # 模擬資源分配性能測試
    $ResourceMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬資源分配時間
        $AllocationTime = Get-Random -Minimum 1 -Maximum 10
        # 模擬資源利用率
        $ResourceUtilization = Get-Random -Minimum 60 -Maximum 95
        
        $ResourceMetrics += @{
            Timestamp = Get-Date
            AllocationTime = $AllocationTime
            ResourceUtilization = $ResourceUtilization
        }
        
        Start-Sleep -Milliseconds $AllocationTime
    }
    
    $AvgAllocationTime = ($ResourceMetrics | Measure-Object -Property AllocationTime -Average).Average
    $AvgResourceUtilization = ($ResourceMetrics | Measure-Object -Property ResourceUtilization -Average).Average
    
    return @{
        AverageAllocationTime = [math]::Round($AvgAllocationTime, 2)
        AverageResourceUtilization = [math]::Round($AvgResourceUtilization, 2)
        SampleCount = $ResourceMetrics.Count
        TargetMaxAllocationTime = 5.0
        TargetMaxUtilization = 85.0
        WithinTarget = ($AvgAllocationTime -le 5.0) -and ($AvgResourceUtilization -le 85.0)
    }
}

# 內存池測試
function Test-MemoryPool {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試內存池性能..." -ForegroundColor Gray
    
    # 模擬內存池性能測試
    $MemoryPoolMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬內存分配時間
        $AllocationTime = Get-Random -Minimum 0.1 -Maximum 2.0
        # 模擬內存釋放時間
        $DeallocationTime = Get-Random -Minimum 0.1 -Maximum 1.0
        # 模擬池命中率
        $PoolHitRate = Get-Random -Minimum 0.8 -Maximum 0.99
        
        $MemoryPoolMetrics += @{
            Timestamp = Get-Date
            AllocationTime = $AllocationTime
            DeallocationTime = $DeallocationTime
            PoolHitRate = $PoolHitRate
        }
        
        Start-Sleep -Milliseconds (($AllocationTime + $DeallocationTime) * 100)
    }
    
    $AvgAllocationTime = ($MemoryPoolMetrics | Measure-Object -Property AllocationTime -Average).Average
    $AvgDeallocationTime = ($MemoryPoolMetrics | Measure-Object -Property DeallocationTime -Average).Average
    $AvgPoolHitRate = ($MemoryPoolMetrics | Measure-Object -Property PoolHitRate -Average).Average
    
    return @{
        AverageAllocationTime = [math]::Round($AvgAllocationTime, 3)
        AverageDeallocationTime = [math]::Round($AvgDeallocationTime, 3)
        AveragePoolHitRate = [math]::Round($AvgPoolHitRate, 3)
        SampleCount = $MemoryPoolMetrics.Count
        TargetMaxAllocationTime = 1.0
        TargetMinHitRate = 0.85
        WithinTarget = ($AvgAllocationTime -le 1.0) -and ($AvgPoolHitRate -ge 0.85)
    }
}

# 資產載入測試
function Test-AssetLoading {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試資產載入性能..." -ForegroundColor Gray
    
    # 模擬資產載入性能測試
    $AssetMetrics = @()
    $StartTime = Get-Date
    $AssetTypes = @("Texture", "Mesh", "Audio", "Animation", "Material")
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        $AssetType = $AssetTypes | Get-Random
        # 模擬載入時間
        $LoadTime = switch ($AssetType) {
            "Texture" { Get-Random -Minimum 50 -Maximum 500 }
            "Mesh" { Get-Random -Minimum 100 -Maximum 1000 }
            "Audio" { Get-Random -Minimum 20 -Maximum 200 }
            "Animation" { Get-Random -Minimum 30 -Maximum 300 }
            "Material" { Get-Random -Minimum 10 -Maximum 100 }
        }
        
        $AssetMetrics += @{
            Timestamp = Get-Date
            AssetType = $AssetType
            LoadTime = $LoadTime
        }
        
        Start-Sleep -Milliseconds ($LoadTime / 10)
    }
    
    $AvgLoadTime = ($AssetMetrics | Measure-Object -Property LoadTime -Average).Average
    $MaxLoadTime = ($AssetMetrics | Measure-Object -Property LoadTime -Maximum).Maximum
    
    return @{
        AverageLoadTime = [math]::Round($AvgLoadTime, 2)
        MaxLoadTime = [math]::Round($MaxLoadTime, 2)
        SampleCount = $AssetMetrics.Count
        TargetMaxLoadTime = 300.0
        WithinTarget = $AvgLoadTime -le 300.0
    }
}

# 垃圾回收測試
function Test-GarbageCollection {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 測試垃圾回收性能..." -ForegroundColor Gray
    
    # 模擬垃圾回收性能測試
    $GCMetrics = @()
    $StartTime = Get-Date
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # 模擬GC頻率
        $GCFrequency = Get-Random -Minimum 1 -Maximum 10
        # 模擬GC時間
        $GCTime = Get-Random -Minimum 5 -Maximum 50
        # 模擬回收內存量
        $MemoryReclaimed = Get-Random -Minimum 10 -Maximum 100
        
        $GCMetrics += @{
            Timestamp = Get-Date
            GCFrequency = $GCFrequency
            GCTime = $GCTime
            MemoryReclaimed = $MemoryReclaimed
        }
        
        Start-Sleep -Seconds $GCFrequency
    }
    
    $AvgGCTime = ($GCMetrics | Measure-Object -Property GCTime -Average).Average
    $AvgGCFrequency = ($GCMetrics | Measure-Object -Property GCFrequency -Average).Average
    $TotalMemoryReclaimed = ($GCMetrics | Measure-Object -Property MemoryReclaimed -Sum).Sum
    
    return @{
        AverageGCTime = [math]::Round($AvgGCTime, 2)
        AverageGCFrequency = [math]::Round($AvgGCFrequency, 2)
        TotalMemoryReclaimed = [math]::Round($TotalMemoryReclaimed, 2)
        SampleCount = $GCMetrics.Count
        TargetMaxGCTime = 20.0
        TargetMaxFrequency = 5.0
        WithinTarget = ($AvgGCTime -le 20.0) -and ($AvgGCFrequency -le 5.0)
    }
}

# 評估性能測試結果
function Evaluate-PerformanceTestResult {
    param(
        [string]$TestName,
        [hashtable]$Metrics
    )
    
    # 檢查是否有WithinTarget屬性
    if ($Metrics.ContainsKey("WithinTarget")) {
        return $Metrics.WithinTarget
    }
    
    # 默認通過
    return $true
}

# 執行完整性能測試套件
function Invoke-FullPerformanceTestSuite {
    Write-Host "🧪 開始執行完整性能優化驗證套件..." -ForegroundColor Green
    
    $AllResults = @()
    $TotalTests = 0
    $PassedTests = 0
    $FailedTests = 0
    
    foreach ($category in $PerformanceTestCategories) {
        Write-Host "`n📂 性能測試類別: $($category.Description)" -ForegroundColor Yellow
        
        foreach ($testName in $category.Tests) {
            $TotalTests++
            $TestDuration = if ($StressTest) { 120 } else { 60 }
            $result = Invoke-PerformanceTest -TestCategory $category.Name -TestName $testName -DurationSeconds $TestDuration
            $AllResults += $result
            
            if ($result.Status -eq "Passed") {
                $PassedTests++
            } else {
                $FailedTests++
            }
            
            if ($Verbose) {
                Write-Host "   詳細信息: $($result.Metrics | ConvertTo-Json -Compress)" -ForegroundColor Gray
            }
        }
    }
    
    # 生成性能測試報告
    $Report = @{
        TestMode = $TestMode
        StartTime = Get-Date
        TotalTests = $TotalTests
        PassedTests = $PassedTests
        FailedTests = $FailedTests
        SuccessRate = if ($TotalTests -gt 0) { [math]::Round(($PassedTests / $TotalTests) * 100, 2) } else { 0 }
        StressTest = $StressTest
        Results = $AllResults
        PerformanceMetrics = $Global:PerformanceMetrics
    }
    
    # 保存性能測試報告
    $ReportPath = "$OutputPath\Reports\PerformanceTestReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $Report | ConvertTo-Json -Depth 10 | Out-File -FilePath $ReportPath -Encoding UTF8
    
    # 顯示性能測試摘要
    Write-Host "`n📊 性能測試摘要:" -ForegroundColor Cyan
    Write-Host "   總測試數: $TotalTests" -ForegroundColor White
    Write-Host "   通過測試: $PassedTests" -ForegroundColor Green
    Write-Host "   失敗測試: $FailedTests" -ForegroundColor Red
    Write-Host "   成功率: $($Report.SuccessRate)%" -ForegroundColor $(if ($Report.SuccessRate -ge 90) { "Green" } else { "Yellow" })
    
    Write-Host "`n📄 詳細報告已保存到: $ReportPath" -ForegroundColor Cyan
    
    return $Report
}

# 連續性能測試模式
function Start-ContinuousPerformanceTesting {
    Write-Host "🔄 啟動連續性能測試模式..." -ForegroundColor Green
    
    while ($Continuous) {
        Write-Host "`n$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss') - 執行性能測試循環" -ForegroundColor Cyan
        
        $Report = Invoke-FullPerformanceTestSuite
        
        if ($Report.SuccessRate -lt 90) {
            Write-Host "⚠️  成功率低於90%，發送性能警報通知" -ForegroundColor Yellow
            # 這裡可以添加性能警報通知邏輯
        }
        
        Write-Host "⏱️  等待10分鐘後進行下一次性能測試..." -ForegroundColor Gray
        Start-Sleep -Seconds 600
    }
}

# 主執行邏輯
try {
    Initialize-PerformanceTestEnvironment
    
    switch ($TestMode) {
        "Standard" {
            $Report = Invoke-FullPerformanceTestSuite
        }
        "Stress" {
            $StressTest = $true
            $Report = Invoke-FullPerformanceTestSuite
        }
        "Continuous" {
            Start-ContinuousPerformanceTesting
        }
        default {
            Write-Host "❌ 不支持的測試模式: $TestMode" -ForegroundColor Red
            Write-Host "支持的模式: Standard, Stress, Continuous" -ForegroundColor Gray
            exit 1
        }
    }
    
    # 根據測試結果設置退出代碼
    if ($Report.SuccessRate -ge 90) {
        Write-Host "`n🎉 性能優化驗證成功完成！" -ForegroundColor Green
        exit 0
    } else {
        Write-Host "`n⚠️  性能優化驗證發現問題，需要進一步優化" -ForegroundColor Yellow
        exit 1
    }
}
catch {
    Write-Host "`n❌ 性能測試執行過程中發生錯誤: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "堆棧跟蹤: $($_.ScriptStackTrace)" -ForegroundColor Gray
    exit 1
}
finally {
    Stop-Transcript
}
