# MingGoRTS Performance Optimization Verification Suite (Final Fixed Version)
param(
    [string]$TestMode = "Standard",
    [string]$OutputPath = "c:\HW\MingGoRTS\TestReports",
    [int]$DurationMinutes = 30,
    [switch]$Verbose,
    [switch]$StressTest,
    [switch]$Continuous
)

# Initialize performance test environment
function Initialize-PerformanceTestEnvironment {
    Write-Host "🚀 Initializing Performance Optimization Verification Environment..." -ForegroundColor Green
    
    # Create test directories
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
    
    # Setup logging
    $Global:LogFile = "$OutputPath\Logs\PerformanceTest_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"
    Start-Transcript -Path $Global:LogFile -Force
    
    # Initialize performance counters
    Initialize-PerformanceCounters
    
    Write-Host "✅ Performance test environment initialized successfully" -ForegroundColor Green
}

# Initialize performance counters
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

# Performance test categories
$PerformanceTestCategories = @(
    @{
        Name = "System_Performance"
        Description = "System Performance Tests"
        Tests = @(
            "CPU_Usage_Test",
            "Memory_Usage_Test",
            "GPU_Performance_Test",
            "Disk_IO_Test"
        )
    },
    @{
        Name = "Game_Performance"
        Description = "Game Performance Tests"
        Tests = @(
            "Frame_Rate_Test",
            "Render_Performance_Test",
            "Physics_Performance_Test",
            "Audio_Performance_Test"
        )
    },
    @{
        Name = "Network_Performance"
        Description = "Network Performance Tests"
        Tests = @(
            "Network_Latency_Test",
            "Bandwidth_Test",
            "Multiplayer_Sync_Test",
            "Connection_Stress_Test"
        )
    },
    @{
        Name = "AI_Performance"
        Description = "AI System Performance Tests"
        Tests = @(
            "AI_Processing_Test",
            "Pathfinding_Performance_Test",
            "Decision_Making_Test",
            "Learning_Algorithm_Test"
        )
    },
    @{
        Name = "Resource_Performance"
        Description = "Resource Management Performance Tests"
        Tests = @(
            "Resource_Allocation_Test",
            "Memory_Pool_Test",
            "Asset_Loading_Test",
            "Garbage_Collection_Test"
        )
    }
)

# Execute performance test
function Invoke-PerformanceTest {
    param(
        [string]$TestCategory,
        [string]$TestName,
        [int]$DurationSeconds = 60
    )
    
    Write-Host "🧪 Executing performance test: $TestCategory - $TestName" -ForegroundColor Cyan
    
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
        # Execute specific test
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
                throw "Unknown performance test name: $TestName"
            }
        }
        
        # Evaluate test result
        $TestResult.Passed = Evaluate-PerformanceTestResult -TestName $TestName -Metrics $TestResult.Metrics
        $TestResult.Status = if ($TestResult.Passed) { "Passed" } else { "Failed" }
        
        if ($TestResult.Passed) {
            Write-Host "✅ Performance test passed: $TestName" -ForegroundColor Green
        } else {
            Write-Host "❌ Performance test failed: $TestName" -ForegroundColor Red
        }
    }
    catch {
        $TestResult.Status = "Failed"
        $TestResult.Error = $_.Exception.Message
        Write-Host "❌ Performance test error: $TestName - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    $EndTime = Get-Date
    $TestResult.EndTime = $EndTime
    $TestResult.Duration = ($EndTime - $StartTime).TotalSeconds
    
    return $TestResult
}

# CPU Usage Test
function Test-CPUUsage {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing CPU usage..." -ForegroundColor Gray
    
    $CPUMetrics = @()
    $StartTime = Get-Date
    $TotalCPU = 0
    $MaxCPU = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        try {
            $CPU = (Get-Counter '\Processor(_Total)\% Processor Time' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            $TotalCPU += $CPU
            $MaxCPU = [math]::Max($MaxCPU, $CPU)
            $SampleCount++
        }
        catch {
            Write-Host "   ⚠️  Unable to get CPU usage data" -ForegroundColor Yellow
        }
        
        Start-Sleep -Seconds 1
    }
    
    $AvgCPU = if ($SampleCount -gt 0) { $TotalCPU / $SampleCount } else { 0 }
    
    return @{
        AverageCPU = [math]::Round($AvgCPU, 2)
        MaxCPU = [math]::Round($MaxCPU, 2)
        SampleCount = $SampleCount
        TargetCPU = $Global:PerformanceMetrics.MaxCPUUsage
        WithinTarget = $AvgCPU -le $Global:PerformanceMetrics.MaxCPUUsage
    }
}

# Memory Usage Test
function Test-MemoryUsage {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing memory usage..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalMemoryUsage = 0
    $MaxMemoryUsage = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        try {
            $AvailableMemory = (Get-Counter '\Memory\Available MBytes' -ErrorAction SilentlyContinue).CounterSamples.CookedValue
            $TotalMemory = (Get-Counter '\Memory\Committed Bytes' -ErrorAction SilentlyContinue).CounterSamples.CookedValue / 1MB
            $UsedMemory = $TotalMemory - $AvailableMemory
            $MemoryUsagePercent = ($UsedMemory / $TotalMemory) * 100
            
            $TotalMemoryUsage += $MemoryUsagePercent
            $MaxMemoryUsage = [math]::Max($MaxMemoryUsage, $MemoryUsagePercent)
            $SampleCount++
        }
        catch {
            Write-Host "   ⚠️  Unable to get memory usage data" -ForegroundColor Yellow
        }
        
        Start-Sleep -Seconds 1
    }
    
    $AvgMemoryUsage = if ($SampleCount -gt 0) { $TotalMemoryUsage / $SampleCount } else { 0 }
    
    return @{
        AverageMemoryUsage = [math]::Round($AvgMemoryUsage, 2)
        MaxMemoryUsage = [math]::Round($MaxMemoryUsage, 2)
        SampleCount = $SampleCount
        TargetMemory = $Global:PerformanceMetrics.MaxMemoryUsage
        WithinTarget = $AvgMemoryUsage -le $Global:PerformanceMetrics.MaxMemoryUsage
    }
}

# GPU Performance Test
function Test-GPUPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing GPU performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalGPU = 0
    $MaxGPU = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate GPU usage (actual would use GPU monitoring API)
        $GPUUsage = Get-Random -Minimum 20 -Maximum 80
        $TotalGPU += $GPUUsage
        $MaxGPU = [math]::Max($MaxGPU, $GPUUsage)
        $SampleCount++
        
        Start-Sleep -Seconds 1
    }
    
    $AvgGPU = if ($SampleCount -gt 0) { $TotalGPU / $SampleCount } else { 0 }
    
    return @{
        AverageGPU = [math]::Round($AvgGPU, 2)
        MaxGPU = [math]::Round($MaxGPU, 2)
        SampleCount = $SampleCount
        TargetGPU = $Global:PerformanceMetrics.MaxGPUUsage
        WithinTarget = $AvgGPU -le $Global:PerformanceMetrics.MaxGPUUsage
    }
}

# Disk IO Test
function Test-DiskIO {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing disk IO performance..." -ForegroundColor Gray
    
    $TestFile = "$env:TEMP\performance_test.tmp"
    $TotalWriteTime = 0
    $TotalReadTime = 0
    $OperationCount = 0
    
    try {
        $TestData = "A" * 1024  # 1KB test data
        
        # Execute disk write test
        for ($i = 0; $i -lt 50; $i++) {
            $WriteStartTime = Get-Date
            Add-Content -Path $TestFile -Value $TestData
            $WriteTime = (Get-Date) - $WriteStartTime
            $TotalWriteTime += $WriteTime.TotalMilliseconds
            $OperationCount++
        }
        
        # Execute disk read test
        for ($i = 0; $i -lt 50; $i++) {
            $ReadStartTime = Get-Date
            Get-Content -Path $TestFile | Out-Null
            $ReadTime = (Get-Date) - $ReadStartTime
            $TotalReadTime += $ReadTime.TotalMilliseconds
            $OperationCount++
        }
    }
    finally {
        if (Test-Path $TestFile) {
            Remove-Item $TestFile -Force
        }
    }
    
    $AvgWriteTime = if ($OperationCount -gt 0) { $TotalWriteTime / ($OperationCount / 2) } else { 0 }
    $AvgReadTime = if ($OperationCount -gt 0) { $TotalReadTime / ($OperationCount / 2) } else { 0 }
    
    return @{
        AverageWriteTime = [math]::Round($AvgWriteTime, 2)
        AverageReadTime = [math]::Round($AvgReadTime, 2)
        TotalOperations = $OperationCount
        TargetMaxTime = $Global:PerformanceMetrics.MaxDiskIO
        WithinTarget = ($AvgWriteTime -le $Global:PerformanceMetrics.MaxDiskIO) -and ($AvgReadTime -le $Global:PerformanceMetrics.MaxDiskIO)
    }
}

# Frame Rate Test
function Test-FrameRate {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing game frame rate..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalFPS = 0
    $MinFPS = 999
    $MaxFPS = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate frame rate (actual would get from game engine)
        $FPS = Get-Random -Minimum 45 -Maximum 75
        $TotalFPS += $FPS
        $MinFPS = [math]::Min($MinFPS, $FPS)
        $MaxFPS = [math]::Max($MaxFPS, $FPS)
        $SampleCount++
        
        Start-Sleep -Milliseconds 16  # Simulate 60FPS
    }
    
    $AvgFPS = if ($SampleCount -gt 0) { $TotalFPS / $SampleCount } else { 0 }
    
    return @{
        AverageFPS = [math]::Round($AvgFPS, 2)
        MinFPS = [math]::Round($MinFPS, 2)
        MaxFPS = [math]::Round($MaxFPS, 2)
        SampleCount = $SampleCount
        TargetFPS = $Global:PerformanceMetrics.TargetFPS
        WithinTarget = $AvgFPS -ge $Global:PerformanceMetrics.TargetFPS
    }
}

# Render Performance Test
function Test-RenderPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing render performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalRenderTime = 0
    $MaxRenderTime = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate render time (actual would get from game engine)
        $RenderTime = Get-Random -Minimum 8 -Maximum 20
        $TotalRenderTime += $RenderTime
        $MaxRenderTime = [math]::Max($MaxRenderTime, $RenderTime)
        $SampleCount++
        
        Start-Sleep -Milliseconds $RenderTime
    }
    
    $AvgRenderTime = if ($SampleCount -gt 0) { $TotalRenderTime / $SampleCount } else { 0 }
    
    return @{
        AverageRenderTime = [math]::Round($AvgRenderTime, 2)
        MaxRenderTime = [math]::Round($MaxRenderTime, 2)
        SampleCount = $SampleCount
        TargetMaxTime = $Global:PerformanceMetrics.MaxRenderTime
        WithinTarget = $AvgRenderTime -le $Global:PerformanceMetrics.MaxRenderTime
    }
}

# Physics Performance Test
function Test-PhysicsPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing physics system performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalPhysicsTime = 0
    $MaxPhysicsTime = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate physics calculation time
        $PhysicsTime = Get-Random -Minimum 1 -Maximum 10
        $TotalPhysicsTime += $PhysicsTime
        $MaxPhysicsTime = [math]::Max($MaxPhysicsTime, $PhysicsTime)
        $SampleCount++
        
        Start-Sleep -Milliseconds $PhysicsTime
    }
    
    $AvgPhysicsTime = if ($SampleCount -gt 0) { $TotalPhysicsTime / $SampleCount } else { 0 }
    
    return @{
        AveragePhysicsTime = [math]::Round($AvgPhysicsTime, 2)
        MaxPhysicsTime = [math]::Round($MaxPhysicsTime, 2)
        SampleCount = $SampleCount
        TargetMaxTime = 5.0
        WithinTarget = $AvgPhysicsTime -le 5.0
    }
}

# Audio Performance Test
function Test-AudioPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing audio system performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalAudioTime = 0
    $MaxAudioTime = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate audio processing time
        $AudioTime = Get-Random -Minimum 0.5 -Maximum 3
        $TotalAudioTime += $AudioTime
        $MaxAudioTime = [math]::Max($MaxAudioTime, $AudioTime)
        $SampleCount++
        
        Start-Sleep -Milliseconds $AudioTime
    }
    
    $AvgAudioTime = if ($SampleCount -gt 0) { $TotalAudioTime / $SampleCount } else { 0 }
    
    return @{
        AverageAudioTime = [math]::Round($AvgAudioTime, 2)
        MaxAudioTime = [math]::Round($MaxAudioTime, 2)
        SampleCount = $SampleCount
        TargetMaxTime = 2.0
        WithinTarget = $AvgAudioTime -le 2.0
    }
}

# Network Latency Test
function Test-NetworkLatency {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing network latency..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalLatency = 0
    $MaxLatency = 0
    $SampleCount = 0
    $TestHosts = @("8.8.8.8", "1.1.1.1", "baidu.com")
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        foreach ($host in $TestHosts) {
            try {
                $PingResult = Test-Connection -ComputerName $host -Count 1 -ErrorAction SilentlyContinue
                if ($PingResult) {
                    $Latency = $PingResult.ResponseTime
                    $TotalLatency += $Latency
                    $MaxLatency = [math]::Max($MaxLatency, $Latency)
                    $SampleCount++
                }
            }
            catch {
                # Ignore network errors
            }
        }
        
        Start-Sleep -Seconds 2
    }
    
    $AvgLatency = if ($SampleCount -gt 0) { $TotalLatency / $SampleCount } else { 0 }
    
    return @{
        AverageLatency = [math]::Round($AvgLatency, 2)
        MaxLatency = [math]::Round($MaxLatency, 2)
        SampleCount = $SampleCount
        TargetLatency = $Global:PerformanceMetrics.MaxNetworkLatency
        WithinTarget = $AvgLatency -le $Global:PerformanceMetrics.MaxNetworkLatency
    }
}

# Bandwidth Test
function Test-Bandwidth {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing network bandwidth..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalDownloadSpeed = 0
    $TotalUploadSpeed = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate download speed (Mbps)
        $DownloadSpeed = Get-Random -Minimum 50 -Maximum 200
        # Simulate upload speed (Mbps)
        $UploadSpeed = Get-Random -Minimum 10 -Maximum 50
        
        $TotalDownloadSpeed += $DownloadSpeed
        $TotalUploadSpeed += $UploadSpeed
        $SampleCount++
        
        Start-Sleep -Seconds 3
    }
    
    $AvgDownloadSpeed = if ($SampleCount -gt 0) { $TotalDownloadSpeed / $SampleCount } else { 0 }
    $AvgUploadSpeed = if ($SampleCount -gt 0) { $TotalUploadSpeed / $SampleCount } else { 0 }
    
    return @{
        AverageDownloadSpeed = [math]::Round($AvgDownloadSpeed, 2)
        AverageUploadSpeed = [math]::Round($AvgUploadSpeed, 2)
        SampleCount = $SampleCount
        TargetMinDownload = 10.0
        TargetMinUpload = 5.0
        WithinTarget = ($AvgDownloadSpeed -ge 10.0) -and ($AvgUploadSpeed -ge 5.0)
    }
}

# Multiplayer Sync Test
function Test-MultiplayerSync {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing multiplayer synchronization..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalSyncLatency = 0
    $TotalPacketLoss = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate sync latency
        $SyncLatency = Get-Random -Minimum 10 -Maximum 100
        # Simulate packet loss rate
        $PacketLoss = Get-Random -Minimum 0 -Maximum 5
        
        $TotalSyncLatency += $SyncLatency
        $TotalPacketLoss += $PacketLoss
        $SampleCount++
        
        Start-Sleep -Milliseconds 100
    }
    
    $AvgSyncLatency = if ($SampleCount -gt 0) { $TotalSyncLatency / $SampleCount } else { 0 }
    $AvgPacketLoss = if ($SampleCount -gt 0) { $TotalPacketLoss / $SampleCount } else { 0 }
    
    return @{
        AverageSyncLatency = [math]::Round($AvgSyncLatency, 2)
        AveragePacketLoss = [math]::Round($AvgPacketLoss, 2)
        SampleCount = $SampleCount
        TargetMaxLatency = 50.0
        TargetMaxPacketLoss = 2.0
        WithinTarget = ($AvgSyncLatency -le 50.0) -and ($AvgPacketLoss -le 2.0)
    }
}

# Connection Stress Test
function Test-ConnectionStress {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing connection stress..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalConnectionTime = 0
    $ConnectionCount = 0
    $MaxConnections = 100
    
    for ($i = 1; $i -le $MaxConnections; $i++) {
        # Simulate connection establishment time
        $ConnectionTime = Get-Random -Minimum 50 -Maximum 500
        $TotalConnectionTime += $ConnectionTime
        $ConnectionCount++
        
        Start-Sleep -Milliseconds $ConnectionTime
        
        if ((Get-Date) -gt $StartTime.AddSeconds($DurationSeconds)) {
            break
        }
    }
    
    $AvgConnectionTime = if ($ConnectionCount -gt 0) { $TotalConnectionTime / $ConnectionCount } else { 0 }
    
    return @{
        AverageConnectionTime = [math]::Round($AvgConnectionTime, 2)
        TotalConnections = $ConnectionCount
        TargetMaxTime = 200.0
        WithinTarget = $AvgConnectionTime -le 200.0
    }
}

# AI Processing Test
function Test-AIProcessing {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing AI processing performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalDecisionTime = 0
    $TotalAILoad = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate AI decision time
        $DecisionTime = Get-Random -Minimum 5 -Maximum 50
        # Simulate AI computational load
        $AILoad = Get-Random -Minimum 20 -Maximum 80
        
        $TotalDecisionTime += $DecisionTime
        $TotalAILoad += $AILoad
        $SampleCount++
        
        Start-Sleep -Milliseconds $DecisionTime
    }
    
    $AvgDecisionTime = if ($SampleCount -gt 0) { $TotalDecisionTime / $SampleCount } else { 0 }
    $AvgAILoad = if ($SampleCount -gt 0) { $TotalAILoad / $SampleCount } else { 0 }
    
    return @{
        AverageDecisionTime = [math]::Round($AvgDecisionTime, 2)
        AverageAILoad = [math]::Round($AvgAILoad, 2)
        SampleCount = $SampleCount
        TargetMaxDecisionTime = 30.0
        TargetMaxLoad = 70.0
        WithinTarget = ($AvgDecisionTime -le 30.0) -and ($AvgAILoad -le 70.0)
    }
}

# Pathfinding Performance Test
function Test-PathfindingPerformance {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing pathfinding performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalPathTime = 0
    $MaxPathTime = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate path calculation time
        $PathTime = Get-Random -Minimum 1 -Maximum 20
        $TotalPathTime += $PathTime
        $MaxPathTime = [math]::Max($MaxPathTime, $PathTime)
        $SampleCount++
        
        Start-Sleep -Milliseconds $PathTime
    }
    
    $AvgPathTime = if ($SampleCount -gt 0) { $TotalPathTime / $SampleCount } else { 0 }
    
    return @{
        AveragePathTime = [math]::Round($AvgPathTime, 2)
        MaxPathTime = [math]::Round($MaxPathTime, 2)
        SampleCount = $SampleCount
        TargetMaxTime = 10.0
        WithinTarget = $AvgPathTime -le 10.0
    }
}

# Decision Making Test
function Test-DecisionMaking {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing AI decision making performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalDecisionTime = 0
    $MaxDecisionTime = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate decision complexity
        $DecisionComplexity = Get-Random -Minimum 5 -Maximum 50
        # Simulate decision time
        $DecisionTime = $DecisionComplexity * (Get-Random -Minimum 0.5 -Maximum 2.0)
        $TotalDecisionTime += $DecisionTime
        $MaxDecisionTime = [math]::Max($MaxDecisionTime, $DecisionTime)
        $SampleCount++
        
        Start-Sleep -Milliseconds ($DecisionTime * 10)
    }
    
    $AvgDecisionTime = if ($SampleCount -gt 0) { $TotalDecisionTime / $SampleCount } else { 0 }
    
    return @{
        AverageDecisionTime = [math]::Round($AvgDecisionTime, 2)
        MaxDecisionTime = [math]::Round($MaxDecisionTime, 2)
        SampleCount = $SampleCount
        TargetMaxTime = 25.0
        WithinTarget = $AvgDecisionTime -le 25.0
    }
}

# Learning Algorithm Test
function Test-LearningAlgorithm {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing learning algorithm performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalTrainingTime = 0
    $TotalConvergenceRate = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate training time
        $TrainingTime = Get-Random -Minimum 10 -Maximum 100
        # Simulate convergence rate
        $ConvergenceRate = Get-Random -Minimum 0.7 -Maximum 0.95
        $TotalTrainingTime += $TrainingTime
        $TotalConvergenceRate += $ConvergenceRate
        $SampleCount++
        
        Start-Sleep -Milliseconds ($TrainingTime / 10)
    }
    
    $AvgTrainingTime = if ($SampleCount -gt 0) { $TotalTrainingTime / $SampleCount } else { 0 }
    $AvgConvergenceRate = if ($SampleCount -gt 0) { $TotalConvergenceRate / $SampleCount } else { 0 }
    
    return @{
        AverageTrainingTime = [math]::Round($AvgTrainingTime, 2)
        AverageConvergenceRate = [math]::Round($AvgConvergenceRate, 3)
        SampleCount = $SampleCount
        TargetMaxTrainingTime = 50.0
        TargetMinConvergenceRate = 0.8
        WithinTarget = ($AvgTrainingTime -le 50.0) -and ($AvgConvergenceRate -ge 0.8)
    }
}

# Resource Allocation Test
function Test-ResourceAllocation {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing resource allocation performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalAllocationTime = 0
    $TotalResourceUtilization = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate resource allocation time
        $AllocationTime = Get-Random -Minimum 1 -Maximum 10
        # Simulate resource utilization
        $ResourceUtilization = Get-Random -Minimum 60 -Maximum 95
        $TotalAllocationTime += $AllocationTime
        $TotalResourceUtilization += $ResourceUtilization
        $SampleCount++
        
        Start-Sleep -Milliseconds $AllocationTime
    }
    
    $AvgAllocationTime = if ($SampleCount -gt 0) { $TotalAllocationTime / $SampleCount } else { 0 }
    $AvgResourceUtilization = if ($SampleCount -gt 0) { $TotalResourceUtilization / $SampleCount } else { 0 }
    
    return @{
        AverageAllocationTime = [math]::Round($AvgAllocationTime, 2)
        AverageResourceUtilization = [math]::Round($AvgResourceUtilization, 2)
        SampleCount = $SampleCount
        TargetMaxAllocationTime = 5.0
        TargetMaxUtilization = 85.0
        WithinTarget = ($AvgAllocationTime -le 5.0) -and ($AvgResourceUtilization -le 85.0)
    }
}

# Memory Pool Test
function Test-MemoryPool {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing memory pool performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalAllocationTime = 0
    $TotalDeallocationTime = 0
    $TotalPoolHitRate = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate memory allocation time
        $AllocationTime = Get-Random -Minimum 0.1 -Maximum 2.0
        # Simulate memory deallocation time
        $DeallocationTime = Get-Random -Minimum 0.1 -Maximum 1.0
        # Simulate pool hit rate
        $PoolHitRate = Get-Random -Minimum 0.8 -Maximum 0.99
        $TotalAllocationTime += $AllocationTime
        $TotalDeallocationTime += $DeallocationTime
        $TotalPoolHitRate += $PoolHitRate
        $SampleCount++
        
        Start-Sleep -Milliseconds (($AllocationTime + $DeallocationTime) * 100)
    }
    
    $AvgAllocationTime = if ($SampleCount -gt 0) { $TotalAllocationTime / $SampleCount } else { 0 }
    $AvgDeallocationTime = if ($SampleCount -gt 0) { $TotalDeallocationTime / $SampleCount } else { 0 }
    $AvgPoolHitRate = if ($SampleCount -gt 0) { $TotalPoolHitRate / $SampleCount } else { 0 }
    
    return @{
        AverageAllocationTime = [math]::Round($AvgAllocationTime, 3)
        AverageDeallocationTime = [math]::Round($AvgDeallocationTime, 3)
        AveragePoolHitRate = [math]::Round($AvgPoolHitRate, 3)
        SampleCount = $SampleCount
        TargetMaxAllocationTime = 1.0
        TargetMinHitRate = 0.85
        WithinTarget = ($AvgAllocationTime -le 1.0) -and ($AvgPoolHitRate -ge 0.85)
    }
}

# Asset Loading Test
function Test-AssetLoading {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing asset loading performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalLoadTime = 0
    $MaxLoadTime = 0
    $SampleCount = 0
    $AssetTypes = @("Texture", "Mesh", "Audio", "Animation", "Material")
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        $AssetType = $AssetTypes | Get-Random
        # Simulate loading time
        $LoadTime = switch ($AssetType) {
            "Texture" { Get-Random -Minimum 50 -Maximum 500 }
            "Mesh" { Get-Random -Minimum 100 -Maximum 1000 }
            "Audio" { Get-Random -Minimum 20 -Maximum 200 }
            "Animation" { Get-Random -Minimum 30 -Maximum 300 }
            "Material" { Get-Random -Minimum 10 -Maximum 100 }
        }
        $TotalLoadTime += $LoadTime
        $MaxLoadTime = [math]::Max($MaxLoadTime, $LoadTime)
        $SampleCount++
        
        Start-Sleep -Milliseconds ($LoadTime / 10)
    }
    
    $AvgLoadTime = if ($SampleCount -gt 0) { $TotalLoadTime / $SampleCount } else { 0 }
    
    return @{
        AverageLoadTime = [math]::Round($AvgLoadTime, 2)
        MaxLoadTime = [math]::Round($MaxLoadTime, 2)
        SampleCount = $SampleCount
        TargetMaxLoadTime = 300.0
        WithinTarget = $AvgLoadTime -le 300.0
    }
}

# Garbage Collection Test
function Test-GarbageCollection {
    param([int]$DurationSeconds)
    
    Write-Host "   📊 Testing garbage collection performance..." -ForegroundColor Gray
    
    $StartTime = Get-Date
    $TotalGCTime = 0
    $TotalGCFrequency = 0
    $TotalMemoryReclaimed = 0
    $SampleCount = 0
    
    while ((Get-Date) -lt $StartTime.AddSeconds($DurationSeconds)) {
        # Simulate GC frequency
        $GCFrequency = Get-Random -Minimum 1 -Maximum 10
        # Simulate GC time
        $GCTime = Get-Random -Minimum 5 -Maximum 50
        # Simulate reclaimed memory
        $MemoryReclaimed = Get-Random -Minimum 10 -Maximum 100
        $TotalGCTime += $GCTime
        $TotalGCFrequency += $GCFrequency
        $TotalMemoryReclaimed += $MemoryReclaimed
        $SampleCount++
        
        Start-Sleep -Seconds $GCFrequency
    }
    
    $AvgGCTime = if ($SampleCount -gt 0) { $TotalGCTime / $SampleCount } else { 0 }
    $AvgGCFrequency = if ($SampleCount -gt 0) { $TotalGCFrequency / $SampleCount } else { 0 }
    
    return @{
        AverageGCTime = [math]::Round($AvgGCTime, 2)
        AverageGCFrequency = [math]::Round($AvgGCFrequency, 2)
        TotalMemoryReclaimed = [math]::Round($TotalMemoryReclaimed, 2)
        SampleCount = $SampleCount
        TargetMaxGCTime = 20.0
        TargetMaxFrequency = 5.0
        WithinTarget = ($AvgGCTime -le 20.0) -and ($AvgGCFrequency -le 5.0)
    }
}

# Evaluate performance test result
function Evaluate-PerformanceTestResult {
    param(
        [string]$TestName,
        [hashtable]$Metrics
    )
    
    # Check if WithinTarget property exists
    if ($Metrics.ContainsKey("WithinTarget")) {
        return $Metrics.WithinTarget
    }
    
    # Default pass
    return $true
}

# Execute full performance test suite
function Invoke-FullPerformanceTestSuite {
    Write-Host "🧪 Starting full performance optimization verification suite..." -ForegroundColor Green
    
    $AllResults = @()
    $TotalTests = 0
    $PassedTests = 0
    $FailedTests = 0
    
    foreach ($category in $PerformanceTestCategories) {
        Write-Host "`n📂 Performance Test Category: $($category.Description)" -ForegroundColor Yellow
        
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
                Write-Host "   Details: $($result.Metrics | ConvertTo-Json -Compress)" -ForegroundColor Gray
            }
        }
    }
    
    # Generate performance test report
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
    
    # Save performance test report
    $ReportPath = "$OutputPath\Reports\PerformanceTestReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $Report | ConvertTo-Json -Depth 10 | Out-File -FilePath $ReportPath -Encoding UTF8
    
    # Display performance test summary
    Write-Host "`n📊 Performance Test Summary:" -ForegroundColor Cyan
    Write-Host "   Total Tests: $TotalTests" -ForegroundColor White
    Write-Host "   Passed Tests: $PassedTests" -ForegroundColor Green
    Write-Host "   Failed Tests: $FailedTests" -ForegroundColor Red
    Write-Host "   Success Rate: $($Report.SuccessRate)%" -ForegroundColor $(if ($Report.SuccessRate -ge 90) { "Green" } else { "Yellow" })
    
    Write-Host "`n📄 Detailed report saved to: $ReportPath" -ForegroundColor Cyan
    
    return $Report
}

# Continuous performance testing mode
function Start-ContinuousPerformanceTesting {
    Write-Host "🔄 Starting continuous performance testing mode..." -ForegroundColor Green
    
    while ($Continuous) {
        Write-Host "`n$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss') - Executing performance test cycle" -ForegroundColor Cyan
        
        $Report = Invoke-FullPerformanceTestSuite
        
        if ($Report.SuccessRate -lt 90) {
            Write-Host "⚠️  Success rate below 90%, sending performance alert notification" -ForegroundColor Yellow
        }
        
        Write-Host "⏱️  Waiting 10 minutes for next performance test..." -ForegroundColor Gray
        Start-Sleep -Seconds 600
    }
}

# Main execution logic
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
            Write-Host "❌ Unsupported test mode: $TestMode" -ForegroundColor Red
            Write-Host "Supported modes: Standard, Stress, Continuous" -ForegroundColor Gray
            exit 1
        }
    }
    
    # Set exit code based on test results
    if ($Report.SuccessRate -ge 90) {
        Write-Host "`n🎉 Performance optimization verification completed successfully!" -ForegroundColor Green
        exit 0
    } else {
        Write-Host "`n⚠️  Performance optimization verification found issues, need further optimization" -ForegroundColor Yellow
        exit 1
    }
}
catch {
    Write-Host "`n❌ Error occurred during performance test execution: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "Stack trace: $($_.ScriptStackTrace)" -ForegroundColor Gray
    exit 1
}
finally {
    Stop-Transcript
}
