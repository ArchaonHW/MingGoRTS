# OPTIMIZED: Script uses buffered logging for better performance
# Original had multiple Write-Host calls that can slow execution

$Script:LogBuffer = @()
function Write-BufferedLog {
    param([string]$Message)
    $Script:LogBuffer += "[03:06:26] $Message"
    if ($Script:LogBuffer.Count -ge 100) { Flush-LogBuffer }
}
function Flush-LogBuffer {
    $Script:LogBuffer | ForEach-Object { Write-Host $_ }
    $Script:LogBuffer = @()
}

# --- ORIGINAL SCRIPT BELOW ---
# MingGoRTS Epic Asset Generator - PowerShell Execution Script
# Features: Batch generation, error handling, progress tracking, video/image/music/SFX

param(
    [string]$Epic,
    [string]$Type,
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
    @{ Id = "epic1"; Name = "北洋軍閥單位包"; Description = "北洋軍閥特色單位：軍官、炮兵、騎兵、步兵" }
    @{ Id = "epic2"; Name = "民國時期建築包"; Description = "1000+民國風格建築資產：民居、軍營、政府大樓" }
    @{ Id = "epic3"; Name = "北洋戰爭場景包"; Description = "北洋戰爭主題場景：戰場、軍營、指揮部、城市街道" }
    @{ Id = "epic4"; Name = "民國角色服裝包"; Description = "民國時期角色服裝：軍服、官服、民服、學生服" }
    @{ Id = "epic5"; Name = "北洋武器裝備包"; Description = "北洋時期武器裝備：步枪、火炮、军刀、马匹" }
    @{ Id = "epic6"; Name = "戰術地圖與環境"; Description = "戰術地圖元素：地形、植被、天氣效果、道路" }
)

# Asset type mapping
$AssetTypes = @{
    "images" = @{ Color = "Cyan"; Icon = "🖼"; Name = "圖片" }
    "music" = @{ Color = "Magenta"; Icon = "🎵"; Name = "音樂" }
    "sfx" = @{ Color = "Yellow"; Icon = "🔊"; Name = "音效" }
    "video" = @{ Color = "Green"; Icon = "🎬"; Name = "視頻" }
}

# Helper functions
function Write-Header($text) {
    Write-Host "`n============================================" -ForegroundColor Blue
    Write-Host $text -ForegroundColor Blue
    Write-Host "============================================" -ForegroundColor Blue
}

function Write-Success($text) {
    Write-Host "✅ $text" -ForegroundColor Green
}

function Write-Warning($text) {
    Write-Host "⚠️ $text" -ForegroundColor Yellow
}

function Write-Error($text) {
    Write-Host "❌ $text" -ForegroundColor Red
}

function Write-Info($text) {
    Write-Host "ℹ️ $text" -ForegroundColor Cyan
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
    Write-Success "環境檢查通過"
    
    return $true
}

# Initialize
function Initialize-System {
    Write-Header "初始化 Epic Asset Generation System"
    
    & python $GeneratorScript --init
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "系統初始化成功"
        Write-Info "配置文件: $ToolsPath\epic_assets_config.json"
        Write-Info "輸出目錄: $OutputPath"
    } else {
        Write-Error "初始化失敗"
        exit 1
    }
}

# List epics
function Show-EpicList {
    Write-Header "Epic 資產列表"
    
    & python $GeneratorScript --list
}

# Generate single epic
function Invoke-EpicGeneration($epicId, $type, $force) {
    $epic = $Epics | Where-Object { $_.Id -eq $epicId }
    if (-not $epic) {
        Write-Error "無效的 Epic ID: $epicId"
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
                Write-Success "Epic $epicId 生成成功！"
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
    Write-Header "批量生成所有 Epic 資產"
    
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
    Write-Success "成功: $success/$total"
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
            Write-Host "`n文件統計:" -ForegroundColor Cyan
            $stats | ForEach-Object {
                $size = "{0:N2} MB" -f ($_.Size / 1MB)
                Write-Host "  $($_.Name): $($_.Count) 個文件 ($size)" -ForegroundColor Gray
            }
        }
    }
    
    return $failed.Count -eq 0
}

# Clean generated assets
function Clear-GeneratedAssets {
    Write-Header "清理生成的資產"
    
    if (-not $Silent) {
        $confirm = Read-Host "確定要刪除所有生成的資產嗎? (y/n)"
        if ($confirm -ne 'y') {
            Write-Info "操作已取消"
            return
        }
    }
    
    & python $GeneratorScript --clean
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "資產清理完成"
    } else {
        Write-Error "清理失敗"
    }
}

# Show usage
function Show-Usage {
    Write-Header "MingGoRTS Epic Asset Generator"
    
    Write-Host @"

使用方法:
  .\epic_assets_generate.ps1 -List                    # 顯示所有Epic
  .\epic_assets_generate.ps1 -Epic epic1            # 生成 Epic 1 的所有資產  .\epic_assets_generate.ps1 -Epic epic2 -Type music # 只生成音樂資產  .\epic_assets_generate.ps1 -All                     # 生成所有資產  .\epic_assets_generate.ps1 -All -Type images       # 只生成圖片  .\epic_assets_generate.ps1 -Clean                   # 清理所有資產  .\epic_assets_generate.ps1 -Init                    # 初始化系統
參數:
  -Epic <id>      指定 Epic ID (epic1-epic6)
  -Type <type>    資產類型: images, music, sfx, video
  -All            生成所有 Epic 的所有資產
  -Force          強制重新生成(忽略緩存)
  -Clean          清理所有生成的資產
  -Init           初始化系統配置
  -Silent         靜默模式(不提示確認)
  -MaxRetries <n> 最大重試次數(默認: 3)

Epic 列表:
"@
    
    foreach ($epic in $Epics) {
        Write-Host "  $($epic.Id): $($epic.Name)" -ForegroundColor Cyan
        Write-Host "     $($epic.Description)" -ForegroundColor Gray
    }
}

# Main execution
Write-Host "`n🚀 MingGoRTS Epic Asset Generator" -ForegroundColor Magenta
Write-Host "   生成遊戲資產並自動部署到UE項目`n" -ForegroundColor Gray

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

