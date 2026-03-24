#!/usr/bin/env pwsh

<#
.SYNOPSIS
    Version Control Health Dashboard
    版控健康度儀表板

.DESCRIPTION
    生成版控健康度報告，包含提交頻率、文件變更熱力圖、審查週期等指標

.PARAMETER OutputPath
    報告輸出路徑

.PARAMETER DaysToAnalyze
    分析天數（默認30天）

.EXAMPLE
    .\VersionControl-HealthDashboard.ps1 -DaysToAnalyze 30
#>

[CmdletBinding()]
param(
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$OutputPath = "$PWD\Reports\VersionControlHealth",
    
    [Parameter()]
    [int]$DaysToAnalyze = 30
)

$ErrorActionPreference = "Stop"

# ============================================
# Git 數據收集
# ============================================

function Get-GitStatistics {
    param(
        [string]$ProjectPath,
        [int]$Days
    )
    
    $SinceDate = (Get-Date).AddDays(-$Days).ToString("yyyy-MM-dd")
    
    $Stats = @{
        TotalCommits = 0
        UniqueAuthors = @()
        FilesChanged = @()
        Insertions = 0
        Deletions = 0
        CommitFrequency = @()
        DailyCommits = @{}
        HourlyDistribution = @{}
        DayOfWeekDistribution = @{}
    }
    
    # 獲取提交歷史（包含統計）
    $LogFormat = '%H|%an|%ad|%s'
    $LogOutput = & git -C $ProjectPath log --since=$SinceDate --pretty=format:$LogFormat --date=short --numstat 2>$null
    
    $CurrentCommit = $null
    $Lines = $LogOutput -split "`n"
    
    for ($i = 0; $i -lt $Lines.Count; $i++) {
        $Line = $Lines[$i]
        
        # 檢查是否是提交行
        if ($Line -match '^([a-f0-9]+)\|([^|]+)\|([^|]+)\|(.+)$') {
            $CurrentCommit = @{
                Hash = $matches[1].Substring(0, 7)
                Author = $matches[2]
                Date = $matches[3]
                Subject = $matches[4]
                Insertions = 0
                Deletions = 0
                FilesChanged = @()
            }
            
            $Stats.TotalCommits++
            if ($Stats.UniqueAuthors -notcontains $CurrentCommit.Author) {
                $Stats.UniqueAuthors += $CurrentCommit.Author
            }
            
            # 每日提交統計
            if (-not $Stats.DailyCommits.ContainsKey($CurrentCommit.Date)) {
                $Stats.DailyCommits[$CurrentCommit.Date] = 0
            }
            $Stats.DailyCommits[$CurrentCommit.Date]++
            
            # 星期幾分佈
            $DayOfWeek = [DateTime]::Parse($CurrentCommit.Date).DayOfWeek.ToString()
            if (-not $Stats.DayOfWeekDistribution.ContainsKey($DayOfWeek)) {
                $Stats.DayOfWeekDistribution[$DayOfWeek] = 0
            }
            $Stats.DayOfWeekDistribution[$DayOfWeek]++
        }
        # 檢查是否是 numstat 行（文件變更統計）
        elseif ($CurrentCommit -and $Line -match '^(\d+)\s+(\d+)\s+(.+)$') {
            $Ins = [int]$matches[1]
            $Del = [int]$matches[2]
            $File = $matches[3]
            
            $CurrentCommit.Insertions += $Ins
            $CurrentCommit.Deletions += $Del
            $CurrentCommit.FilesChanged += $File
            
            $Stats.Insertions += $Ins
            $Stats.Deletions += $Del
            
            if ($Stats.FilesChanged -notcontains $File) {
                $Stats.FilesChanged += $File
            }
        }
        # 空行表示提交結束
        elseif ($CurrentCommit -and $Line -eq '' -and $CurrentCommit.Insertions -gt 0) {
            $Stats.CommitFrequency += $CurrentCommit
            $CurrentCommit = $null
        }
    }
    
    return $Stats
}

function Get-FileChangeHeatmap {
    param(
        [string]$ProjectPath,
        [int]$Days
    )
    
    $SinceDate = (Get-Date).AddDays(-$Days).ToString("yyyy-MM-dd")
    $FileStats = @{}
    
    # 獲取每個文件的變更次數
    $LogOutput = & git -C $ProjectPath log --since=$SinceDate --name-only --pretty=format: 2>$null
    $Files = $LogOutput -split "`n" | Where-Object { $_ -match '\.(h|cpp|cs|json|md|yml|yaml|ini|uproject)$' }
    
    foreach ($File in $Files) {
        $File = $File.Trim()
        if ($File) {
            if (-not $FileStats.ContainsKey($File)) {
                $FileStats[$File] = 0
            }
            $FileStats[$File]++
        }
    }
    
    # 排序並獲取前 20
    $TopFiles = $FileStats.GetEnumerator() | 
        Sort-Object Value -Descending | 
        Select-Object -First 20
    
    return $TopFiles
}

function Get-CodeReviewMetrics {
    param(
        [string]$ProjectPath,
        [int]$Days
    )
    
    # 這裡可以擴展為從 GitHub API 獲取 PR 數據
    # 目前使用本地 Git 數據估算
    
    $SinceDate = (Get-Date).AddDays(-$Days).ToString("yyyy-MM-dd")
    
    # 獲取合併提交（估算審查週期）
    $MergeCommits = & git -C $ProjectPath log --since=$SinceDate --merges --pretty=format:'%H|%an|%ad|%s' --date=short 2>$null
    
    $ReviewMetrics = @{
        MergeCount = 0
        AverageCommitsPerMerge = 0
        FastForwards = 0
    }
    
    if ($MergeCommits) {
        $MergeList = $MergeCommits -split "`n"
        $ReviewMetrics.MergeCount = $MergeList.Count
        
        # 估算每次合併包含的提交數
        $TotalCommitsInMerges = 0
        foreach ($Merge in $MergeList) {
            if ($Merge -match '^([a-f0-9]+)\|') {
                $Hash = $matches[1]
                $ParentCount = (& git -C $ProjectPath cat-file -p $Hash 2>$null | Select-String "^parent").Count
                if ($ParentCount -eq 1) {
                    $ReviewMetrics.FastForwards++
                } elseif ($ParentCount -eq 2) {
                    # 估算提交數（簡化計算）
                    $CommitsInMerge = (& git -C $ProjectPath rev-list --count "$Hash^..$Hash" 2>$null)
                    $TotalCommitsInMerges += [int]$CommitsInMerge
                }
            }
        }
        
        if ($ReviewMetrics.MergeCount -gt 0) {
            $ReviewMetrics.AverageCommitsPerMerge = [math]::Round($TotalCommitsInMerges / $ReviewMetrics.MergeCount, 2)
        }
    }
    
    return $ReviewMetrics
}

function Get-TechnicalDebtIndicators {
    param([string]$ProjectPath)
    
    $Indicators = @{
        TODOCount = 0
        FIXMECount = 0
        HACKCount = 0
        LargeFiles = @()
        OldBranches = @()
    }
    
    # 掃描 TODO/FIXME/HACK 註釋
    $SourceFiles = Get-ChildItem -Path "$ProjectPath\Source", "$ProjectPath\Plugins" -Recurse -Include "*.h", "*.cpp" -File -ErrorAction SilentlyContinue
    
    foreach ($File in $SourceFiles) {
        $Content = Get-Content -Path $File.FullName -Raw -ErrorAction SilentlyContinue
        
        $Indicators.TODOCount += ([regex]::Matches($Content, "TODO|BMAD-TODO", [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)).Count
        $Indicators.FIXMECount += ([regex]::Matches($Content, "FIXME", [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)).Count
        $Indicators.HACKCount += ([regex]::Matches($Content, "HACK|XXX", [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)).Count
        
        # 大文件檢測（>500行）
        $LineCount = ($Content -split "`n").Count
        if ($LineCount -gt 500) {
            $Indicators.LargeFiles += @{
                File = $File.FullName.Substring($ProjectPath.Length + 1)
                Lines = $LineCount
            }
        }
    }
    
    # 檢查舊分支
    $Branches = & git -C $ProjectPath branch -a --format='%(refname:short)|%(committerdate:short)' 2>$null
    $CutoffDate = (Get-Date).AddDays(-60)
    
    foreach ($Branch in $Branches) {
        if ($Branch -match '^([^|]+)\|(\d{4}-\d{2}-\d{2})$') {
            $BranchName = $matches[1]
            $LastCommit = [DateTime]::Parse($matches[2])
            
            if ($LastCommit -lt $CutoffDate -and $BranchName -notmatch 'main|master|develop|HEAD') {
                $Indicators.OldBranches += @{
                    Branch = $BranchName
                    LastCommit = $matches[2]
                    DaysAgo = [math]::Round(((Get-Date) - $LastCommit).TotalDays)
                }
            }
        }
    }
    
    return $Indicators
}

# ============================================
# 報告生成
# ============================================

function New-HealthDashboard {
    param(
        [hashtable]$GitStats,
        [array]$FileHeatmap,
        [hashtable]$ReviewMetrics,
        [hashtable]$TechDebt,
        [string]$ReportFile,
        [int]$AnalysisDays
    )
    
    $ReportDate = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $EndDate = Get-Date -Format "yyyy-MM-dd"
    $StartDate = (Get-Date).AddDays(-$AnalysisDays).ToString("yyyy-MM-dd")
    
    # 計算關鍵指標
    $CommitsPerDay = if ($GitStats.TotalCommits -gt 0) { 
        [math]::Round($GitStats.TotalCommits / $AnalysisDays, 2) 
    } else { 0 }
    
    $ChangeRatio = if ($GitStats.Deletions -gt 0) {
        [math]::Round($GitStats.Insertions / $GitStats.Deletions, 2)
    } else { 0 }
    
    # 生成健康度評分
    $HealthScore = 100
    if ($TechDebt.TODOCount -gt 50) { $HealthScore -= 10 }
    if ($TechDebt.FIXMECount -gt 20) { $HealthScore -= 10 }
    if ($TechDebt.HACKCount -gt 10) { $HealthScore -= 15 }
    if ($CommitsPerDay -lt 0.5) { $HealthScore -= 10 }
    if ($TechDebt.OldBranches.Count -gt 5) { $HealthScore -= 10 }
    
    $HealthScore = [math]::Max(0, [math]::Min(100, $HealthScore))
    
    # 健康度等級
    $HealthLevel = switch ($HealthScore) {
        { $_ -ge 90 } { "🟢 優秀" }
        { $_ -ge 70 } { "🟡 良好" }
        { $_ -ge 50 } { "🟠 一般" }
        default { "🔴 需改進" }
    }
    
    $Html = @"
<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MingGoRTS Version Control Health Dashboard</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { 
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: #0d1117;
            color: #c9d1d9;
            line-height: 1.6;
            padding: 20px;
        }
        .container { max-width: 1400px; margin: 0 auto; }
        .header {
            text-align: center;
            padding: 30px 0;
            border-bottom: 1px solid #30363d;
            margin-bottom: 30px;
        }
        .header h1 { 
            font-size: 2.5em; 
            margin-bottom: 10px;
            background: linear-gradient(135deg, #58a6ff, #238636);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }
        .header p { color: #8b949e; }
        
        .metrics-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        .metric-card {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 12px;
            padding: 20px;
            transition: transform 0.2s;
        }
        .metric-card:hover { transform: translateY(-5px); }
        .metric-label { color: #8b949e; font-size: 0.9em; text-transform: uppercase; }
        .metric-value { 
            font-size: 2.5em; 
            font-weight: bold; 
            margin: 10px 0;
            color: #58a6ff;
        }
        .metric-trend { font-size: 0.85em; color: #238636; }
        
        .health-score {
            text-align: center;
            padding: 40px;
            background: linear-gradient(135deg, #161b22, #1f2937);
            border-radius: 16px;
            margin-bottom: 30px;
        }
        .score-circle {
            width: 200px;
            height: 200px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 4em;
            font-weight: bold;
            margin: 0 auto 20px;
            background: conic-gradient(
                $(if ($HealthScore -ge 70) { "#238636" } elseif ($HealthScore -ge 50) { "#d29922" } else { "#da3633" }) $HealthScore%, 
                #30363d 0
            );
            color: white;
            position: relative;
        }
        .score-circle::before {
            content: '';
            position: absolute;
            width: 160px;
            height: 160px;
            background: #0d1117;
            border-radius: 50%;
        }
        .score-value {
            position: relative;
            z-index: 1;
        }
        .health-level {
            font-size: 1.5em;
            margin-top: 10px;
        }
        
        .section {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 12px;
            padding: 25px;
            margin-bottom: 20px;
        }
        .section h2 {
            color: #58a6ff;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 1px solid #30363d;
        }
        
        .heatmap {
            display: grid;
            gap: 8px;
        }
        .heatmap-row {
            display: flex;
            align-items: center;
            gap: 10px;
        }
        .heatmap-file {
            width: 300px;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
            font-family: monospace;
            font-size: 0.9em;
        }
        .heatmap-bar {
            flex: 1;
            height: 24px;
            background: #238636;
            border-radius: 4px;
            display: flex;
            align-items: center;
            justify-content: flex-end;
            padding-right: 10px;
            color: white;
            font-weight: bold;
            font-size: 0.85em;
        }
        
        .debt-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 15px;
        }
        .debt-item {
            text-align: center;
            padding: 20px;
            background: #0d1117;
            border-radius: 8px;
        }
        .debt-count {
            font-size: 2.5em;
            font-weight: bold;
            margin-bottom: 5px;
        }
        .debt-label { color: #8b949e; font-size: 0.9em; }
        .debt-todo { color: #58a6ff; }
        .debt-fixme { color: #d29922; }
        .debt-hack { color: #da3633; }
        
        table {
            width: 100%;
            border-collapse: collapse;
            font-size: 0.9em;
        }
        th, td {
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #30363d;
        }
        th {
            color: #8b949e;
            font-weight: 600;
            text-transform: uppercase;
            font-size: 0.85em;
        }
        
        .alert {
            background: rgba(218, 54, 51, 0.1);
            border-left: 4px solid #da3633;
            padding: 15px;
            margin: 10px 0;
            border-radius: 0 8px 8px 0;
        }
        .alert-title { color: #da3633; font-weight: bold; margin-bottom: 5px; }
        
        .footer {
            text-align: center;
            padding: 30px;
            color: #8b949e;
            border-top: 1px solid #30363d;
            margin-top: 30px;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>📊 Version Control Health Dashboard</h1>
            <p>分析期間: $StartDate ~ $EndDate | 生成時間: $ReportDate</p>
        </div>
        
        <div class="health-score">
            <div class="score-circle">
                <span class="score-value">$HealthScore</span>
            </div>
            <div class="health-level">$HealthLevel</div>
            <p style="color: #8b949e; margin-top: 10px;">版控健康度評分 (0-100)</p>
        </div>
        
        <div class="metrics-grid">
            <div class="metric-card">
                <div class="metric-label">總提交數</div>
                <div class="metric-value">$($GitStats.TotalCommits)</div>
                <div class="metric-trend">平均每天 $CommitsPerDay 次提交</div>
            </div>
            <div class="metric-card">
                <div class="metric-label">活躍貢獻者</div>
                <div class="metric-value">$($GitStats.UniqueAuthors.Count)</div>
                <div class="metric-trend">過去 $AnalysisDays 天內</div>
            </div>
            <div class="metric-card">
                <div class="metric-label">代碼變更</div>
                <div class="metric-value">+$($GitStats.Insertions)/-$($GitStats.Deletions)</div>
                <div class="metric-trend">增刪比: $ChangeRatio</div>
            </div>
            <div class="metric-card">
                <div class="metric-label">變更文件</div>
                <div class="metric-value">$($GitStats.FilesChanged.Count)</div>
                <div class="metric-trend">唯一文件數</div>
            </div>
        </div>
        
        <div class="section">
            <h2>🔥 文件變更熱力圖 (Top 20)</h2>
            <div class="heatmap">
"@

    $MaxCount = if ($FileHeatmap.Count -gt 0) { $FileHeatmap[0].Value } else { 1 }
    
    foreach ($File in $FileHeatmap) {
        $Width = [math]::Round(($File.Value / $MaxCount) * 100)
        $Html += @"
                <div class="heatmap-row">
                    <div class="heatmap-file">$($File.Key)</div>
                    <div class="heatmap-bar" style="width: $Width%;">$($File.Value)</div>
                </div>
"@
    }
    
    $Html += @"
            </div>
        </div>
        
        <div class="section">
            <h2>📈 提交頻率分佈</h2>
            <div class="debt-grid">
                <div class="debt-item">
                    <div class="debt-count" style="color: #58a6ff;">$($GitStats.DayOfWeekDistribution['Monday'] + $GitStats.DayOfWeekDistribution['Tuesday'] + $GitStats.DayOfWeekDistribution['Wednesday'] + $GitStats.DayOfWeekDistribution['Thursday'] + $GitStats.DayOfWeekDistribution['Friday'])</div>
                    <div class="debt-label">工作日提交</div>
                </div>
                <div class="debt-item">
                    <div class="debt-count" style="color: #d29922;">$($GitStats.DayOfWeekDistribution['Saturday'] + $GitStats.DayOfWeekDistribution['Sunday'])</div>
                    <div class="debt-label">週末提交</div>
                </div>
                <div class="debt-item">
                    <div class="debt-count" style="color: #238636;">$ReviewMetrics.MergeCount</div>
                    <div class="debt-label">合併次數</div>
                </div>
            </div>
        </div>
        
        <div class="section">
            <h2>💳 技術債務指標</h2>
            <div class="debt-grid">
                <div class="debt-item">
                    <div class="debt-count debt-todo">$($TechDebt.TODOCount)</div>
                    <div class="debt-label">TODO 註釋</div>
                </div>
                <div class="debt-item">
                    <div class="debt-count debt-fixme">$($TechDebt.FIXMECount)</div>
                    <div class="debt-label">FIXME 註釋</div>
                </div>
                <div class="debt-item">
                    <div class="debt-count debt-hack">$($TechDebt.HACKCount)</div>
                    <div class="debt-label">HACK/XXX 註釋</div>
                </div>
            </div>
"@

    if ($TechDebt.LargeFiles.Count -gt 0) {
        $Html += @"
            <div class="alert" style="margin-top: 20px;">
                <div class="alert-title">⚠️ 大文件警告</div>
                <p>發現 $($TechDebt.LargeFiles.Count) 個超過 500 行的文件，建議考慮重構：</p>
                <ul style="margin-top: 10px; margin-left: 20px;">
"@
        foreach ($File in $TechDebt.LargeFiles | Select-Object -First 5) {
            $Html += "<li>$($File.File) ($($File.Lines) 行)</li>`n"
        }
        $Html += @"
                </ul>
            </div>
"@
    }
    
    if ($TechDebt.OldBranches.Count -gt 0) {
        $Html += @"
        <div class="section">
            <h2>🌿 過期分支 (60+ 天未更新)</h2>
            <table>
                <thead>
                    <tr>
                        <th>分支名稱</th>
                        <th>最後提交</th>
                        <th>天數</th>
                        <th>建議</th>
                    </tr>
                </thead>
                <tbody>
"@
        foreach ($Branch in $TechDebt.OldBranches | Select-Object -First 10) {
            $Suggestion = if ($Branch.DaysAgo -gt 90) { "考慮刪除" } else { "考慮合併或更新" }
            $Html += @"
                    <tr>
                        <td>$($Branch.Branch)</td>
                        <td>$($Branch.LastCommit)</td>
                        <td>$($Branch.DaysAgo) 天</td>
                        <td>$Suggestion</td>
                    </tr>
"@
        }
        $Html += @"
                </tbody>
            </table>
        </div>
"@
    }
    
    $Html += @"
        
        <div class="footer">
            <p>Generated by MingGoRTS Version Control Health Dashboard</p>
            <p style="font-size: 0.85em; margin-top: 10px;">
                <a href="https://semver.org">Semantic Versioning</a> | 
                <a href="https://conventionalcommits.org">Conventional Commits</a>
            </p>
        </div>
    </div>
</body>
</html>
"@
    
    $Html | Set-Content -Path $ReportFile -Encoding UTF8
    return $ReportFile
}

# ============================================
# 主執行流程
# ============================================

function Main {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Version Control Health Dashboard" -ForegroundColor Cyan
    Write-Host "版控健康度儀表板生成器" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    
    # 檢查 Git 倉庫
    $GitPath = Join-Path $ProjectRoot ".git"
    if (-not (Test-Path $GitPath)) {
        Write-Error "Not a git repository."
        return
    }
    
    # 確保輸出目錄存在
    if (-not (Test-Path $OutputPath)) {
        New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    }
    
    Write-Host "`n📊 分析過去 $DaysToAnalyze 天的數據..." -ForegroundColor Yellow
    
    # 收集數據
    Write-Host "  → 收集 Git 統計數據..." -ForegroundColor Gray
    $GitStats = Get-GitStatistics -ProjectPath $ProjectRoot -Days $DaysToAnalyze
    
    Write-Host "  → 生成文件變更熱力圖..." -ForegroundColor Gray
    $FileHeatmap = Get-FileChangeHeatmap -ProjectPath $ProjectRoot -Days $DaysToAnalyze
    
    Write-Host "  → 計算代碼審查指標..." -ForegroundColor Gray
    $ReviewMetrics = Get-CodeReviewMetrics -ProjectPath $ProjectRoot -Days $DaysToAnalyze
    
    Write-Host "  → 分析技術債務..." -ForegroundColor Gray
    $TechDebt = Get-TechnicalDebtIndicators -ProjectPath $ProjectRoot
    
    # 生成報告
    $ReportFile = Join-Path $OutputPath "HealthDashboard_$(Get-Date -Format 'yyyyMMdd_HHmmss').html"
    Write-Host "`n📄 生成儀表板報告..." -ForegroundColor Yellow
    
    $GeneratedFile = New-HealthDashboard `
        -GitStats $GitStats `
        -FileHeatmap $FileHeatmap `
        -ReviewMetrics $ReviewMetrics `
        -TechDebt $TechDebt `
        -ReportFile $ReportFile `
        -AnalysisDays $DaysToAnalyze
    
    # 輸出摘要
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "📊 分析摘要" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  總提交數: $($GitStats.TotalCommits)" -ForegroundColor White
    Write-Host "  活躍貢獻者: $($GitStats.UniqueAuthors.Count) 人" -ForegroundColor White
    Write-Host "  代碼變更: +$($GitStats.Insertions)/-$($GitStats.Deletions)" -ForegroundColor White
    Write-Host "  技術債務: $($TechDebt.TODOCount) TODO, $($TechDebt.FIXMECount) FIXME, $($TechDebt.HACKCount) HACK" -ForegroundColor White
    Write-Host "  過期分支: $($TechDebt.OldBranches.Count) 個" -ForegroundColor $(if ($TechDebt.OldBranches.Count -gt 5) { "Red" } else { "White" })
    Write-Host "`n  📄 報告已生成: $GeneratedFile" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
}

# 執行
Main
