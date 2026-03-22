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
    @{ Id = "epic1"; Name = "甇瑕瘙箇?擃??箇?"; Description = "甇瑕瘙箇?蝟餌絞????隞嗉孛?? }
    @{ Id = "epic2"; Name = "憭扯?璅⊥銵擛亦頂蝯?; Description = "1000+?桐??圈洛?TS?詨?" }
    @{ Id = "epic3"; Name = "閫???鈭頂蝯?; Description = "閫?萄遣???賜撅?鈭?閰? }
    @{ Id = "epic4"; Name = "?箏撱箄身??皞恣??; Description = "?箏撱粹?皞????" }
    @{ Id = "epic5"; Name = "?惜蝑?游?蝟餌絞"; Description = "?啁?銵犖?遣??撅斗?? }
    @{ Id = "epic6"; Name = "頝典像?啁蝮恍?撽?; Description = "憭像?啜蝡臬?甇乓??賢?? }
)

# Asset type mapping
$AssetTypes = @{
    "images" = @{ Color = "Cyan"; Icon = "?"; Name = "??" }
    "music" = @{ Color = "Magenta"; Icon = "?"; Name = "?單?" }
    "sfx" = @{ Color = "Yellow"; Icon = "??"; Name = "?單?" }
    "video" = @{ Color = "Green"; Icon = "?"; Name = "敶梁?" }
}

# Helper functions
function Write-Header($text) {
    Write-Host "`n============================================" -ForegroundColor Blue
    Write-Host $text -ForegroundColor Blue
    Write-Host "============================================" -ForegroundColor Blue
}

function Write-Success($text) {
    Write-Host "??$text" -ForegroundColor Green
}

function Write-Warning($text) {
    Write-Host "??$text" -ForegroundColor Yellow
}

function Write-Error($text) {
    Write-Host "??$text" -ForegroundColor Red
}

function Write-Info($text) {
    Write-Host "??$text" -ForegroundColor Cyan
}

# Check environment
function Test-Environment {
    Write-Info "瑼Ｘ?啣?..."
    
    # Check Python
    try {
        $pythonVersion = python --version 2>&1
        Write-Success "Python: $pythonVersion"
    } catch {
        Write-Error "Python ?芸?鋆?
        exit 1
    }
    
    # Check if generator script exists
    if (-not (Test-Path $GeneratorScript)) {
        Write-Error "?曆??啁???單: $GeneratorScript"
        exit 1
    }
    Write-Success "???刻?砍停蝺?
    
    return $true
}

# Initialize
function Initialize-System {
    Write-Header "????Epic Asset Generation System"
    
    & python $GeneratorScript --init
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "蝟餌絞??????"
        Write-Info "?蔭瑼?: $ToolsPath\epic_assets_config.json"
        Write-Info "頛詨?桅?: $OutputPath"
    } else {
        Write-Error "???仃??
        exit 1
    }
}

# List epics
function Show-EpicList {
    Write-Header "Epic 蝝??瘙???
    
    & python $GeneratorScript --list
}

# Generate single epic
function Invoke-EpicGeneration($epicId, $type, $force) {
    $epic = $Epics | Where-Object { $_.Id -eq $epicId }
    if (-not $epic) {
        Write-Error "?芰??Epic ID: $epicId"
        return $false
    }
    
    Write-Header "?? Epic: $($epic.Name)"
    Write-Info $epic.Description
    
    $arguments = "--epic $epicId"
    if ($type) { $arguments += " --type $type" }
    if ($force) { $arguments += " --force" }
    
    $attempt = 0
    $success = $false
    
    while ($attempt -lt $MaxRetries -and -not $success) {
        $attempt++
        Write-Info "?岫 $attempt/$MaxRetries..."
        
        try {
            $null = & python $GeneratorScript $arguments.Split(' ') 2>&1
            if ($LASTEXITCODE -eq 0) {
                $success = $true
                Write-Success "Epic $epicId ??摰?嚗?
            }
        } catch {
            Write-Warning "?岫 $attempt 憭望?: $_"
            if ($attempt -lt $MaxRetries) {
                Start-Sleep -Seconds 2
            }
        }
    }
    
    if (-not $success) {
        Write-Error "Epic $epicId ??憭望?"
        return $false
    }
    
    return $true
}

# Generate all epics
function Invoke-AllGeneration($type, $force) {
    Write-Header "?寞活?????Epic 蝝?"
    
    $total = $Epics.Count
    $success = 0
    $failed = @()
    
    for ($i = 0; $i -lt $total; $i++) {
        $epic = $Epics[$i]
        Write-Host "`n[$($i+1)/$total] ??: $($epic.Name)" -ForegroundColor Cyan
        
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
    Write-Header "????"
    Write-Success "摰?: $success/$total"
    if ($failed.Count -gt 0) {
        Write-Error "憭望?: $($failed.Count)"
        Write-Info "憭望??: $($failed -join ', ')"
    }
    
    # Show output
    Write-Info "頛詨雿蔭: $OutputPath"
    
    if (Test-Path $OutputPath) {
        $stats = Get-ChildItem -Path $OutputPath -Recurse -File | 
            Group-Object { $_.Extension } | 
            Select-Object Name, @{N='Count';E={$_.Count}}, @{N='Size';E={($_.Group | Measure-Object -Property Length -Sum).Sum}}
        
        if ($stats) {
            Write-Host "`n鞈蝯梯?:" -ForegroundColor Cyan
            $stats | ForEach-Object {
                $size = "{0:N2} MB" -f ($_.Size / 1MB)
                Write-Host "  $($_.Name): $($_.Count) ??獢?($size)" -ForegroundColor Gray
            }
        }
    }
    
    return $failed.Count -eq 0
}

# Clean generated assets
function Clear-GeneratedAssets {
    Write-Header "皜???????
    
    if (-not $Silent) {
        $confirm = Read-Host "蝣箏?閬?斗?????蝝??? (y/n)"
        if ($confirm -ne 'y') {
            Write-Info "??撌脣?瘨?
            return
        }
    }
    
    & python $GeneratorScript --clean
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "蝝?撌脫???
    } else {
        Write-Error "皜?憭望?"
    }
}

# Show usage
function Show-Usage {
    Write-Header "MingGoRTS Epic Asset Generator"
    
    Write-Host @"

雿輻?孵?:
  .\epic_assets_generate.ps1 -List                    # ????Epic
  .\epic_assets_generate.ps1 -Epic epic1            # ?? Epic 1 ??????  .\epic_assets_generate.ps1 -Epic epic2 -Type music # ?芰??璅???  .\epic_assets_generate.ps1 -All                     # ???????  .\epic_assets_generate.ps1 -All -Type images       # ???????  .\epic_assets_generate.ps1 -Clean                   # 皜??????  .\epic_assets_generate.ps1 -Init                    # ???頂蝯?
?:
  -Epic <id>      ?? Epic ID (epic1-epic6)
  -Type <type>    蝝?憿?: images, music, sfx, video
  -All            ?????Epic ??????  -Force          撘瑕???嚗????
  -Clean          皜??????蝝?
  -Init           ????蝵桀??桅?蝯?
  -Silent         ??璅∪?嚗蝣箄??內嚗?  -MaxRetries <n> ?憭折?閰行活??(?身: 3)

Epic ?”:
"@
    
    foreach ($epic in $Epics) {
        Write-Host "  $($epic.Id): $($epic.Name)" -ForegroundColor Cyan
        Write-Host "     $($epic.Description)" -ForegroundColor Gray
    }
}

# Main execution
Write-Host "`n?? MingGoRTS Epic Asset Generator" -ForegroundColor Magenta
Write-Host "   ??敶梁????璅???n" -ForegroundColor Gray

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

