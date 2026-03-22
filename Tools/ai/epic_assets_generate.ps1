# MingGoRTS Epic Asset Generator - PowerShell Execution Script
# Features: Batch generation, error handling, progress tracking, video/image/music/SFX

param(
    [string]$Epic = "",
    [string]$Type = "",
    [switch]$All,
    [switch]$List,
    [switch]$Force,
    [switch]$Clean,
    [switch]$Init,
    [switch]$Silent,
    [int]$MaxRetries = 3
)

$ErrorActionPreference = "Stop"

# Setup paths
$ProjectRoot = $PSScriptRoot | Split-Path -Parent | Split-Path -Parent
$ToolsPath = Join-Path $ProjectRoot "Tools\ai"
$GeneratorScript = Join-Path $ToolsPath "epic_asset_generator.py"
$OutputPath = Join-Path $ProjectRoot "Content\EpicAssets"
$ProgressFile = Join-Path $ToolsPath "epic_generation_progress.json"

# Epic definitions
$Epics = @(
    @{ Id = "epic1"; Name = "歷史決策體驗基礎"; Description = "歷史決策系統、動態事件觸發" }
    @{ Id = "epic2"; Name = "大規模戰術戰鬥系統"; Description = "1000+單位戰鬥、RTS核心" }
    @{ Id = "epic3"; Name = "角色成長與敘事系統"; Description = "角色創建、技能發展、敘事對話" }
    @{ Id = "epic4"; Name = "基地建設與資源管理"; Description = "基地建造、資源採集、科技研發" }
    @{ Id = "epic5"; Name = "四層策略整合系統"; Description = "戰略、戰術、個人、建造四層整合" }
    @{ Id = "epic6"; Name = "跨平台無縫體驗"; Description = "多平台、雲端同步、效能優化" }
)

# Asset type mapping
$AssetTypes = @{
    "images" = @{ Color = "Cyan"; Icon = "🖼"; Name = "圖像" }
    "music" = @{ Color = "Magenta"; Icon = "🎵"; Name = "音樂" }
    "sfx" = @{ Color = "Yellow"; Icon = "🔊"; Name = "音效" }
    "video" = @{ Color = "Green"; Icon = "🎬"; Name = "影片" }
}

# Helper functions
function Write-Header($text) {
    Write-Host "`n============================================" -ForegroundColor Blue
    Write-Host $text -ForegroundColor Blue
    Write-Host "============================================" -ForegroundColor Blue
}

function Write-Success($text) {
    Write-Host "✓ $text" -ForegroundColor Green
}

function Write-Warning($text) {
    Write-Host "⚠ $text" -ForegroundColor Yellow
}

function Write-Error($text) {
    Write-Host "✗ $text" -ForegroundColor Red
}

function Write-Info($text) {
    Write-Host "ℹ $text" -ForegroundColor Cyan
}

# Check environment
function Test-Environment {
    Write-Info "檢查環境..."
    
    # Check Python
    try {
        $pythonVersion = python --version 2>&1
        Write-Success "Python: $pythonVersion"
    } catch {
        Write-Error "Python 未安裝"
        exit 1
    }
    
    # Check if generator script exists
    if (-not (Test-Path $GeneratorScript)) {
        Write-Error "找不到生成器腳本: $GeneratorScript"
        exit 1
    }
    Write-Success "生成器腳本就緒"
    
    return $true
}

# Initialize
function Initialize-System {
    Write-Header "初始化 Epic Asset Generation System"
    
    & python $GeneratorScript --init
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "系統初始化完成！"
        Write-Info "配置檔案: $ToolsPath\epic_assets_config.json"
        Write-Info "輸出目錄: $OutputPath"
    } else {
        Write-Error "初始化失敗"
        exit 1
    }
}

# List epics
function Show-EpicList {
    Write-Header "Epic 素材需求清單"
    
    & python $GeneratorScript --list
}

# Generate single epic
function Invoke-EpicGeneration($epicId, $type, $force) {
    $epic = $Epics | Where-Object { $_.Id -eq $epicId }
    if (-not $epic) {
        Write-Error "未知的 Epic ID: $epicId"
        return $false
    }
    
    Write-Header "生成 Epic: $($epic.Name)"
    Write-Info $epic.Description
    
    $arguments = "--epic $epicId"
    if ($type) { $arguments += " --type $type" }
    if ($force) { $arguments += " --force" }
    
    $attempt = 0
    $success = $false
    
    while ($attempt -lt $MaxRetries -and -not $success) {
        $attempt++
        Write-Info "嘗試 $attempt/$MaxRetries..."
        
        try {
            $null = & python $GeneratorScript $arguments.Split(' ') 2>&1
            if ($LASTEXITCODE -eq 0) {
                $success = $true
                Write-Success "Epic $epicId 生成完成！"
            }
        } catch {
            Write-Warning "嘗試 $attempt 失敗: $_"
            if ($attempt -lt $MaxRetries) {
                Start-Sleep -Seconds 2
            }
        }
    }
    
    if (-not $success) {
        Write-Error "Epic $epicId 生成失敗"
        return $false
    }
    
    return $true
}

# Generate all epics
function Invoke-AllGeneration($type, $force) {
    Write-Header "批次生成所有 Epic 素材"
    
    $total = $Epics.Count
    $success = 0
    $failed = @()
    
    for ($i = 0; $i -lt $total; $i++) {
        $epic = $Epics[$i]
        Write-Host "`n[$($i+1)/$total] 處理: $($epic.Name)" -ForegroundColor Cyan
        
        if (Invoke-EpicGeneration $epic.Id $type $force) {
            $success++
        } else {
            $failed += $epic.Id
        }
        
        if ($i -lt $total - 1) {
            Start-Sleep -Seconds 1
        }
    }
    
    # Summary
    Write-Header "生成摘要"
    Write-Success "完成: $success/$total"
    if ($failed.Count -gt 0) {
        Write-Error "失敗: $($failed.Count)"
        Write-Info "失敗項目: $($failed -join ', ')"
    }
    
    # Show output
    Write-Info "輸出位置: $OutputPath"
    
    if (Test-Path $OutputPath) {
        $stats = Get-ChildItem -Path $OutputPath -Recurse -File | 
            Group-Object { $_.Extension } | 
            Select-Object Name, @{N='Count';E={$_.Count}}, @{N='Size';E={($_.Group | Measure-Object -Property Length -Sum).Sum}}
        
        if ($stats) {
            Write-Host "`n資產統計:" -ForegroundColor Cyan
            $stats | ForEach-Object {
                $size = "{0:N2} MB" -f ($_.Size / 1MB)
                Write-Host "  $($_.Name): $($_.Count) 個檔案 ($size)" -ForegroundColor Gray
            }
        }
    }
    
    return $failed.Count -eq 0
}

# Clean generated assets
function Clear-GeneratedAssets {
    Write-Header "清理生成的素材"
    
    if (-not $Silent) {
        $confirm = Read-Host "確定要刪除所有生成的素材嗎？ (y/n)"
        if ($confirm -ne 'y') {
            Write-Info "操作已取消"
            return
        }
    }
    
    & python $GeneratorScript --clean
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "素材已清理"
    } else {
        Write-Error "清理失敗"
    }
}

# Show usage
function Show-Usage {
    Write-Header "MingGoRTS Epic Asset Generator"
    
    Write-Host @"

使用方式:
  .\epic_assets_generate.ps1 -List                    # 列出所有 Epic
  .\epic_assets_generate.ps1 -Epic epic1            # 生成 Epic 1 的所有素材
  .\epic_assets_generate.ps1 -Epic epic2 -Type music # 只生成音樂素材
  .\epic_assets_generate.ps1 -All                     # 生成所有素材
  .\epic_assets_generate.ps1 -All -Type images       # 生成所有圖像
  .\epic_assets_generate.ps1 -Clean                   # 清理所有素材
  .\epic_assets_generate.ps1 -Init                    # 初始化系統

參數:
  -Epic <id>      指定 Epic ID (epic1-epic6)
  -Type <type>    素材類型: images, music, sfx, video
  -All            生成所有 Epic 的所有素材
  -Force          強制重新生成（覆蓋現有）
  -Clean          清理所有生成的素材
  -Init           初始化配置和目錄結構
  -Silent         靜默模式（無確認提示）
  -MaxRetries <n> 最大重試次數 (預設: 3)

Epic 列表:
"@
    
    foreach ($epic in $Epics) {
        Write-Host "  $($epic.Id): $($epic.Name)" -ForegroundColor Cyan
        Write-Host "     $($epic.Description)" -ForegroundColor Gray
    }
}

# Main execution
Write-Host "`n🚀 MingGoRTS Epic Asset Generator" -ForegroundColor Magenta
Write-Host "   生成影片、圖像、音樂、音效素材`n" -ForegroundColor Gray

# Check environment first
Test-Environment | Out-Null

# Handle parameters
if ($Init) {
    Initialize-System
} elseif ($List) {
    Show-EpicList
} elseif ($Clean) {
    Clear-GeneratedAssets
} elseif ($All) {
    $success = Invoke-AllGeneration $Type $Force
    exit ($success ? 0 : 1)
} elseif ($Epic) {
    $success = Invoke-EpicGeneration $Epic $Type $Force
    exit ($success ? 0 : 1)
} else {
    Show-Usage
    Show-EpicList
}

exit 0
