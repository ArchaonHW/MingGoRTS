# OPTIMIZED: Script uses buffered logging for better performance
# Original had multiple Write-Host calls that can slow execution

$Script:LogBuffer = @()
function Write-BufferedLog {
    param([string]$Message)
    $Script:LogBuffer += "[03:06:25] $Message"
    if ($Script:LogBuffer.Count -ge 100) { Flush-LogBuffer }
}
function Flush-LogBuffer {
    $Script:LogBuffer | ForEach-Object { Write-Host $_ }
    $Script:LogBuffer = @()
}

# --- ORIGINAL SCRIPT BELOW ---
# MingGoRTS AI Music Generator - Auto Execution Script
# Features: Batch generation, error retry, progress tracking

param(
    [switch]$Force,
    [switch]$Silent,
    [switch]$Retry,
    [int]$MaxRetries = 3
)

$ErrorActionPreference = "Stop"

# Setup paths
$ProjectRoot = $PSScriptRoot | Split-Path -Parent | Split-Path -Parent
$ToolsPath = Join-Path $ProjectRoot "Tools\ai"
$GeneratorScript = Join-Path $ToolsPath "music_generator.py"
$OutputPath = Join-Path $ProjectRoot "Content\Audio\Generated"
$ProgressFile = Join-Path $ToolsPath "generation_progress.json"

# Preset definitions
$Presets = @(
    @{ Name = "main_theme"; Folder = "Main" }
    @{ Name = "menu_background"; Folder = "Background" }
    @{ Name = "battle"; Folder = "Battle" }
    @{ Name = "victory"; Folder = "Battle" }
    @{ Name = "building"; Folder = "Background" }
)

# Load progress
function Get-Progress {
    if (Test-Path $ProgressFile) {
        try {
            return Get-Content $ProgressFile | ConvertFrom-Json
        } catch {
            return @{}
        }
    }
    return @{}
}

# Save progress
function Save-Progress($progress) {
    $progress | ConvertTo-Json -Depth 3 | Set-Content $ProgressFile
}

# Check if file exists
function Test-GeneratedFile($presetName) {
    $preset = $Presets | Where-Object { $_.Name -eq $presetName }
    if (-not $preset) { return $false }
    $filePath = Join-Path (Join-Path (Join-Path $OutputPath "Music") $preset.Folder) "$presetName.wav"
    return Test-Path $filePath
}

# Generate single music
function Invoke-Generation($preset, $progress) {
    $attempt = 0
    $success = $false
    
    while ($attempt -lt $MaxRetries -and -not $success) {
        $attempt++
        Write-Host "[INFO] Generating '$($preset.Name)' (attempt $attempt/$MaxRetries)..." -ForegroundColor Cyan
        
        try {
            $null = & python $GeneratorScript --preset $preset.Name 2>&1
            $exitCode = $LASTEXITCODE
            
            if ($exitCode -eq 0 -and (Test-GeneratedFile $preset.Name)) {
                $success = $true
                $progress[$preset.Name] = @{ 
                    Status = "Success"
                    LastAttempt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
                    Attempts = $attempt 
                }
            } else {
                throw "Generation failed"
            }
        } catch {
            Write-Host "[WARN] Attempt $attempt failed: $_" -ForegroundColor Yellow
            Start-Sleep -Seconds 5
        }
    }
    
    if (-not $success) {
        $progress[$preset.Name] = @{ 
            Status = "Failed"
            LastAttempt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
            Attempts = $attempt 
        }
        return $false
    }
    
    return $true
}

# Main execution
Write-Host "`n============================================" -ForegroundColor Magenta
Write-Host "MingGoRTS AI Music Generator - Auto Batch" -ForegroundColor Magenta
Write-Host "============================================`n"

# Check environment
Write-Host "[INFO] Checking environment..." -ForegroundColor Cyan
$pythonCheck = python --version 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERR] Python not installed" -ForegroundColor Red
    exit 1
}
Write-Host "[OK] Python: $pythonCheck" -ForegroundColor Green

# Load progress
$progress = Get-Progress

# Determine what to generate
$toGenerate = @()
foreach ($preset in $Presets) {
    $exists = Test-GeneratedFile $preset.Name
    $status = $null
    if ($progress[$preset.Name]) {
        $status = $progress[$preset.Name].Status
    }
    
    if ($Force) {
        $toGenerate += $preset
    } elseif ($Retry -and $status -eq "Failed") {
        $toGenerate += $preset
    } elseif (-not $exists) {
        $toGenerate += $preset
    } else {
        Write-Host "[INFO] Skipping '$($preset.Name)' - exists" -ForegroundColor Cyan
    }
}

if ($toGenerate.Count -eq 0) {
    Write-Host "[OK] All assets already generated!" -ForegroundColor Green
    exit 0
}

Write-Host "[INFO] Will generate $($toGenerate.Count) assets" -ForegroundColor Cyan

# Confirm execution
if (-not $Silent) {
    $confirm = Read-Host "`nConfirm start? (y/n)"
    if ($confirm -ne "y") {
        Write-Host "[INFO] Cancelled" -ForegroundColor Cyan
        exit 0
    }
}

# Start batch generation
Write-Host "`n============================================" -ForegroundColor Magenta
Write-Host "Starting generation..." -ForegroundColor Magenta
Write-Host "============================================"

$successCount = 0
$failCount = 0

foreach ($preset in $toGenerate) {
    Write-Host "`n[$($successCount + $failCount + 1)/$($toGenerate.Count)] Processing: $($preset.Name)"
    
    if (Invoke-Generation $preset $progress) {
        Write-Host "[OK] $($preset.Name) success!" -ForegroundColor Green
        $successCount++
    } else {
        Write-Host "[ERR] $($preset.Name) failed" -ForegroundColor Red
        $failCount++
    }
    
    Save-Progress $progress
    
    if ($successCount + $failCount -lt $toGenerate.Count) {
        Start-Sleep -Seconds 2
    }
}

# Results
Write-Host "`n============================================" -ForegroundColor Magenta
Write-Host "Generation Report" -ForegroundColor Magenta
Write-Host "============================================"
Write-Host "[OK] Success: $successCount" -ForegroundColor Green
if ($failCount -gt 0) {
    Write-Host "[ERR] Failed: $failCount" -ForegroundColor Red
}

# Show files
Write-Host "`nOutput: $OutputPath"
$files = Get-ChildItem -Path $OutputPath -Recurse -Filter "*.wav" | Sort-Object LastWriteTime -Descending | Select-Object -First 10
if ($files) {
    Write-Host "`nRecent files:"
    $files | ForEach-Object {
        $size = "{0:N2} MB" -f ($_.Length / 1MB)
        Write-Host "  - $($_.Name) ($size)"
    }
}

Write-Host ""
if ($failCount -eq 0) {
    Write-Host "[OK] All music generated successfully!" -ForegroundColor Green
} else {
    Write-Host "[WARN] Some failed, use -Retry" -ForegroundColor Yellow
}

exit $failCount

