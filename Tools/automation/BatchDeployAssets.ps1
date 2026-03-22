# MingGoRTS 素材批次部署腳本
# 專門處理大量素材的自動化導入

param(
    [Parameter(Mandatory=$true)]
    [string]$SourceDirectory,
    
    [string]$DestinationCategory = "Auto",
    [switch]$WatchMode = $false,
    [int]$WatchInterval = 30,
    [switch]$DryRun = $false,
    [string]$Filter = "*.*",
    [switch]$PreserveStructure = $false
)

$ProjectRoot = "C:\HW\MingGoRTS"
$ContentDir = "$ProjectRoot\Content"
$LogDir = "$ProjectRoot\Logs\BatchDeployment"
$ProcessingDir = "$ProjectRoot\Temp\AssetProcessing"

# 確保目錄存在
if (!(Test-Path $LogDir)) { New-Item -ItemType Directory -Path $LogDir -Force | Out-Null }
if (!(Test-Path $ProcessingDir)) { New-Item -ItemType Directory -Path $ProcessingDir -Force | Out-Null }

# 日誌函數
function Write-BatchLog {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    
    $Color = switch($Level) {
        "ERROR" { "Red" }
        "WARN" { "Yellow" }
        "SUCCESS" { "Green" }
        default { "White" }
    }
    
    Write-Host $LogEntry -ForegroundColor $Color
    Add-Content "$LogDir\BatchDeploy_$(Get-Date -Format 'yyyyMMdd').log" $LogEntry
}

# 批次部署統計
$BatchStats = @{
    StartTime = Get-Date
    TotalFiles = 0
    Processed = 0
    Successful = 0
    Failed = 0
    Skipped = 0
    ByCategory = @{}
    Errors = @()
}

# 獲取素材類別
function Get-AssetCategoryByContent {
    param([string]$FilePath, [string]$FileName)
    
    $Extension = [System.IO.Path]::GetExtension($FilePath).ToLower()
    $BaseName = [System.IO.Path]::GetFileNameWithoutExtension($FilePath)
    
    # 根據內容和命名分析
    $LowerName = $FileName.ToLower()
    
    # 紋理檢測
    if ($Extension -in @(".png", ".jpg", ".jpeg", ".tga", ".bmp")) {
        if ($LowerName -match "_n$|_normal|_norm") { return "Art/Textures/Normals" }
        if ($LowerName -match "_orm$|_mask") { return "Art/Textures/Masks" }
        if ($LowerName -match "ui_|hud_|icon_") { return "UI/Textures" }
        if ($LowerName -match "sky_|background_") { return "Environment/Sky" }
        return "Art/Textures"
    }
    
    # 模型檢測
    if ($Extension -in @(".fbx", ".obj", ".3ds")) {
        if ($LowerName -match "char_|character_") { return "Characters/Meshes" }
        if ($LowerName -match "prop_|item_") { return "Art/Props" }
        if ($LowerName -match "building_|arch_") { return "Environment/Buildings" }
        if ($LowerName -match "terrain_|landscape_") { return "Environment/Terrain" }
        return "Art/Meshes"
    }
    
    # 音頻檢測
    if ($Extension -in @(".wav", ".mp3", ".ogg", ".flac")) {
        if ($LowerName -match "bgm_|music_") { return "Audio/Music" }
        if ($LowerName -match "sfx_|effect_") { return "Audio/SFX" }
        if ($LowerName -match "voice_|dialog_") { return "Audio/Voice" }
        return "Audio"
    }
    
    # 動畫檢測
    if ($Extension -eq ".fbx" -and ($LowerName -match "anim_|animation_|a_")) {
        return "Art/Animations"
    }
    
    return "Art/Misc"
}

# 處理單個文件
function Process-AssetFile {
    param([string]$FilePath)
    
    $FileName = [System.IO.Path]::GetFileName($FilePath)
    $BatchStats.TotalFiles++
    
    Write-BatchLog "處理: $FileName" "INFO"
    
    # 確定目標類別
    $TargetCategory = if ($DestinationCategory -eq "Auto") {
        Get-AssetCategoryByContent -FilePath $FilePath -FileName $FileName
    } else {
        $DestinationCategory
    }
    
    # 構建目標路徑
    $TargetDir = Join-Path $ContentDir $TargetCategory
    if (!(Test-Path $TargetDir)) {
        New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
    }
    
    # 保持目錄結構
    if ($PreserveStructure) {
        $RelativePath = $FilePath.Substring($SourceDirectory.Length).TrimStart('\', '/')
        $RelativeDir = [System.IO.Path]::GetDirectoryName($RelativePath)
        if ($RelativeDir) {
            $TargetDir = Join-Path $TargetDir $RelativeDir
            if (!(Test-Path $TargetDir)) {
                New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
            }
        }
    }
    
    $TargetPath = Join-Path $TargetDir $FileName
    
    # 檢查是否需要更新
    if (Test-Path $TargetPath) {
        $SourceTime = (Get-Item $FilePath).LastWriteTime
        $TargetTime = (Get-Item $TargetPath).LastWriteTime
        
        if ($SourceTime -le $TargetTime) {
            Write-BatchLog "跳過 (未變更): $FileName" "DEBUG"
            $BatchStats.Skipped++
            return $true
        }
    }
    
    # 模擬運行模式
    if ($DryRun) {
        Write-BatchLog "[模擬] 將複製: $FileName -> $TargetCategory" "INFO"
        $BatchStats.Successful++
        return $true
    }
    
    # 執行複製
    try {
        Copy-Item $FilePath $TargetPath -Force
        
        # 記錄統計
        if (!$BatchStats.ByCategory[$TargetCategory]) {
            $BatchStats.ByCategory[$TargetCategory] = 0
        }
        $BatchStats.ByCategory[$TargetCategory]++
        
        $BatchStats.Successful++
        Write-BatchLog "✓ 成功: $FileName" "SUCCESS"
        return $true
    }
    catch {
        $BatchStats.Failed++
        $BatchStats.Errors += "Failed to copy $FileName : $_"
        Write-BatchLog "✗ 失敗: $FileName - $_" "ERROR"
        return $false
    }
}

# 掃描目錄
function Scan-Directory {
    param([string]$Path, [string]$FileFilter = "*.*")
    
    Write-BatchLog "掃描目錄: $Path" "INFO"
    
    if (!(Test-Path $Path)) {
        Write-BatchLog "目錄不存在: $Path" "ERROR"
        return @()
    }
    
    $Files = Get-ChildItem -Path $Path -Filter $FileFilter -File -Recurse
    Write-BatchLog "找到 $($Files.Count) 個文件" "INFO"
    
    return $Files
}

# 執行批次部署
function Start-BatchDeployment {
    Write-BatchLog "開始批次部署..." "INFO"
    Write-BatchLog "源目錄: $SourceDirectory" "INFO"
    Write-BatchLog "目標分類: $DestinationCategory" "INFO"
    
    $Files = Scan-Directory -Path $SourceDirectory -FileFilter $Filter
    
    if ($Files.Count -eq 0) {
        Write-BatchLog "沒有找到素材文件" "WARN"
        return
    }
    
    $BatchStats.TotalFiles = $Files.Count
    $counter = 0
    
    foreach ($File in $Files) {
        $counter++
        $percent = [math]::Round(($counter / $Files.Count) * 100, 1)
        Write-Progress -Activity "部署素材" -Status "$counter / $($Files.Count) ($percent%)" -PercentComplete $percent -CurrentOperation $File.Name
        
        Process-AssetFile -FilePath $File.FullName
        $BatchStats.Processed++
    }
    
    Write-Progress -Activity "部署素材" -Completed
    
    # 生成報告
    Generate-BatchReport
}

# 監視模式
function Start-WatchMode {
    Write-BatchLog "啟動監視模式 (間隔: $WatchInterval 秒)" "INFO"
    Write-BatchLog "監視目錄: $SourceDirectory" "INFO"
    Write-BatchLog "按 Ctrl+C 停止監視" "WARN"
    
    $ProcessedFiles = @{}
    
    while ($true) {
        try {
            $Files = Get-ChildItem -Path $SourceDirectory -File -Recurse
            
            foreach ($File in $Files) {
                $FileKey = $File.FullName
                $FileHash = "$($File.LastWriteTime.Ticks)_$($File.Length)"
                
                # 檢查是否已處理過
                if ($ProcessedFiles[$FileKey] -eq $FileHash) {
                    continue
                }
                
                Write-BatchLog "檢測到新/變更文件: $($File.Name)" "INFO"
                Process-AssetFile -FilePath $File.FullName
                $ProcessedFiles[$FileKey] = $FileHash
            }
            
            Write-BatchLog "監視中... 下次檢查: $WatchInterval 秒後" "DEBUG"
            Start-Sleep -Seconds $WatchInterval
        }
        catch {
            Write-BatchLog "監視異常: $_" "ERROR"
            Start-Sleep -Seconds 5
        }
    }
}

# 生成批次報告
function Generate-BatchReport {
    $EndTime = Get-Date
    $Duration = $EndTime - $BatchStats.StartTime
    
    $Report = @"
========================================
批次部署報告
========================================
開始時間: $($BatchStats.StartTime)
結束時間: $EndTime
耗時: $($Duration.ToString('hh\:mm\:ss'))

統計:
  總文件數: $($BatchStats.TotalFiles)
  已處理: $($BatchStats.Processed)
  成功: $($BatchStats.Successful)
  失敗: $($BatchStats.Failed)
  跳過: $($BatchStats.Skipped)

分類統計:
"@
    
    foreach ($Cat in $BatchStats.ByCategory.Keys) {
        $Report += "  $Cat`: $($BatchStats.ByCategory[$Cat])`n"
    }
    
    if ($BatchStats.Errors.Count -gt 0) {
        $Report += "`n錯誤列表:`n"
        foreach ($Error in $BatchStats.Errors) {
            $Report += "  - $Error`n"
        }
    }
    
    $Report += "========================================`n"
    
    # 輸出到控制台
    Write-Host $Report -ForegroundColor Cyan
    
    # 保存到文件
    $ReportPath = "$LogDir\BatchReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').txt"
    $Report | Out-File -FilePath $ReportPath -Encoding UTF8
    Write-BatchLog "報告已保存: $ReportPath" "INFO"
}

# 主入口
Write-Host "`n========================================" -ForegroundColor Green
Write-Host "   MingGoRTS 素材批次部署系統" -ForegroundColor Green
Write-Host "========================================`n" -ForegroundColor Green

# 驗證源目錄
if (!(Test-Path $SourceDirectory)) {
    Write-BatchLog "錯誤: 源目錄不存在 - $SourceDirectory" "ERROR"
    exit 1
}

# 執行
if ($WatchMode) {
    Start-WatchMode
} else {
    Start-BatchDeployment
}

Write-Host "`n========================================" -ForegroundColor Green
Write-Host "   批次部署完成" -ForegroundColor Green
Write-Host "========================================`n" -ForegroundColor Green
