# MingGoRTS Epic Asset Generator - Clean Version
# Generates epic assets for factions using AI

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
$OutputPath = Join-Path $ProjectRoot "Content\Art\Generated"
$LogPath = Join-Path $ProjectRoot "Logs\AssetGeneration"

# Ensure directories exist
if (!(Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
}
if (!(Test-Path $LogPath)) {
    New-Item -ItemType Directory -Path $LogPath -Force | Out-Null
}

# Epic definitions for Beiyang Faction
$Epics = @(
    @{ Id = "epic1"; Name = "北洋戰爭機器"; Description = "北洋軍的蒸汽坦克與重型火砲部隊"; AssetType = "units" }
    @{ Id = "epic2"; Name = "帝國裝甲師"; Description = "1000輛坦克組成的無敵裝甲師"; AssetType = "units" }
    @{ Id = "epic3"; Name = "蒸汽動力核心"; Description = "蒸汽動力核心與能量轉換系統"; AssetType = "buildings" }
    @{ Id = "epic4"; Name = "軍械製造廠"; Description = "軍械製造廠內部運作與生產線"; AssetType = "buildings" }
    @{ Id = "epic5"; Name = "蒸汽坦克"; Description = "蒸汽坦克設計與戰場運用"; AssetType = "units" }
    @{ Id = "epic6"; Name = "北洋要塞"; Description = "北洋要塞防禦工事與兵力配置"; AssetType = "buildings" }
    @{ Id = "epic7"; Name = "帝國兵工廠"; Description = "帝國最大兵工廠全景"; AssetType = "buildings" }
    @{ Id = "epic8"; Name = "蒸汽機甲"; Description = "蒸汽機甲內部結構與操作"; AssetType = "units" }
    @{ Id = "epic9"; Name = "飛艇編隊"; Description = "飛艇編隊組成與戰術運用"; AssetType = "units" }
    @{ Id = "epic10"; Name = "皇帝座艦"; Description = "皇帝座艦與皇家艦隊"; AssetType = "units" }
)

# Helper functions
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogEntry = "[$Timestamp] [$Level] $Message"
    if (!$Silent) {
        Write-Host $LogEntry
    }
    $LogEntry | Add-Content -Path (Join-Path $LogPath "epic_generation.log")
}

function Get-Epic {
    param([string]$Id)
    return $Epics | Where-Object { $_.Id -eq $Id }
}

function Generate-EpicAsset {
    param($Epic, $Retry = 0)
    
    Write-Log "Generating epic asset: $($Epic.Name)" "INFO"
    
    $OutputDir = Join-Path $OutputPath $Epic.AssetType
    if (!(Test-Path $OutputDir)) {
        New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
    }
    
    # Create placeholder file
    $OutputFile = Join-Path $OutputDir "$($Epic.Id)_$($Epic.Name.Replace(' ', '_')).png"
    
    try {
        # Simulate generation (placeholder)
        # In real implementation, this would call Python generator
        $Bytes = [byte[]]::new(100)
        [System.IO.File]::WriteAllBytes($OutputFile, $Bytes)
        
        Write-Log "Successfully generated: $($Epic.Name)" "SUCCESS"
        return $true
    }
    catch {
        if ($Retry -lt $MaxRetries) {
            Write-Log "Retrying $($Epic.Name)... ($Retry/$MaxRetries)" "WARN"
            Start-Sleep -Seconds 1
            return Generate-EpicAsset -Epic $Epic -Retry ($Retry + 1)
        }
        Write-Log "Failed to generate: $($Epic.Name)" "ERROR"
        return $false
    }
}

# Main execution
if ($List) {
    $Epics | ForEach-Object {
        Write-Host "$($_.Id): $($_.Name) - $($_.Description)"
    }
    exit 0
}

if ($Clean) {
    Write-Log "Cleaning output directory..." "INFO"
    if (Test-Path $OutputPath) {
        Remove-Item -Path $OutputPath -Recurse -Force
        New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    }
    Write-Log "Clean complete" "SUCCESS"
    exit 0
}

$ToGenerate = @()

if ($All) {
    $ToGenerate = $Epics
}
elseif ($Epic) {
    $Selected = Get-Epic -Id $Epic
    if ($Selected) {
        $ToGenerate = @($Selected)
    } else {
        Write-Log "Epic not found: $Epic" "ERROR"
        exit 1
    }
}
else {
    Write-Log "No epic specified. Use -All or -Epic <id>" "ERROR"
    exit 1
}

# Generate assets
$Success = 0
$Failed = 0

foreach ($EpicItem in $ToGenerate) {
    if (Generate-EpicAsset -Epic $EpicItem) {
        $Success++
    } else {
        $Failed++
    }
}

Write-Log "Generation complete: $Success success, $Failed failed" "INFO"

if ($Failed -gt 0) {
    exit 1
} else {
    exit 0
}
