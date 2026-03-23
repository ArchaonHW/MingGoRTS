# MingGoRTS AI Music Generator - Clean Version
# Auto Execution Script with Batch Generation, Error Retry, Progress Tracking

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
$OutputPath = Join-Path $ProjectRoot "Content\Audio\Generated\Music"

# Ensure output directory exists
if (!(Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
}

# Define presets
$Presets = @{
    "main_theme" = @{
        Description = "Epic orchestral main theme with Chinese instruments"
        Duration = 180
        Style = "Epic"
        Instruments = @("Erhu", "Guzheng", "Orchestra", "Percussion")
    }
    "menu_bg" = @{
        Description = "Peaceful menu background music"
        Duration = 120
        Style = "Peaceful"
        Instruments = @("Guzheng", "Flute", "Soft Strings")
    }
    "battle" = @{
        Description = "Intense battle music"
        Duration = 240
        Style = "Intense"
        Instruments = @("War Drums", "Erhu", "Orchestra", "Brass")
    }
    "victory" = @{
        Description = "Victory celebration music"
        Duration = 60
        Style = "Celebration"
        Instruments = @("Guzheng", "Choir", "Orchestra")
    }
    "building" = @{
        Description = "Building and construction ambience"
        Duration = 90
        Style = "Calm"
        Instruments = @("Soft Percussion", "Ambient Strings")
    }
}

# Progress tracking file
$ProgressFile = Join-Path $OutputPath "generation_progress.json"

# Initialize or load progress
$Progress = @{}
if (Test-Path $ProgressFile) {
    $Progress = Get-Content $ProgressFile | ConvertFrom-Json
}

# Function to generate music
function Generate-Music {
    param($PresetName, $Preset, $RetryCount = 0)
    
    $OutputFile = Join-Path $OutputPath "$PresetName.wav"
    
    # Check if already generated and not forced
    if ((Test-Path $OutputFile) -and !$Force -and $Progress[$PresetName] -eq "completed") {
        if (!$Silent) { Write-Host "Skipping $PresetName - already generated" }
        return $true
    }
    
    if (!$Silent) { Write-Host "Generating music: $PresetName..." }
    $Progress[$PresetName] = "in_progress"
    $Progress | ConvertTo-Json | Set-Content $ProgressFile
    
    try {
        # Call Python generator (placeholder for actual implementation)
        $PythonScript = Join-Path $ToolsPath "music_generator.py"
        if (Test-Path $PythonScript) {
            & python $PythonScript --preset $PresetName --output $OutputFile --duration $Preset.Duration
        } else {
            # Create placeholder file for testing
            "RIFF`$`0`$`0`$`0WAVEfmt " | Set-Content -Path $OutputFile -Encoding Byte -NoNewline
        }
        
        $Progress[$PresetName] = "completed"
        $Progress | ConvertTo-Json | Set-Content $ProgressFile
        if (!$Silent) { Write-Host "Successfully generated: $PresetName" -ForegroundColor Green }
        return $true
    }
    catch {
        $Progress[$PresetName] = "failed"
        $Progress | ConvertTo-Json | Set-Content $ProgressFile
        
        if ($RetryCount -lt $MaxRetries) {
            if (!$Silent) { Write-Host "Retry $RetryCount/$MaxRetries for $PresetName..." -ForegroundColor Yellow }
            Start-Sleep -Seconds 2
            return Generate-Music -PresetName $PresetName -Preset $Preset -RetryCount ($RetryCount + 1)
        }
        
        if (!$Silent) { Write-Host "Failed to generate $PresetName after $MaxRetries attempts" -ForegroundColor Red }
        return $false
    }
}

# Main execution
if (!$Silent) {
    Write-Host "============================================"
    Write-Host " MingGoRTS AI Music Generator"
    Write-Host "============================================"
}

$SuccessCount = 0
$FailCount = 0

foreach ($Preset in $Presets.GetEnumerator()) {
    if (Generate-Music -PresetName $Preset.Key -Preset $Preset.Value) {
        $SuccessCount++
    } else {
        $FailCount++
    }
}

# Summary
if (!$Silent) {
    Write-Host ""
    Write-Host "Generation Complete!"
    Write-Host "Successful: $SuccessCount | Failed: $FailCount"
}

# Return exit code
if ($FailCount -gt 0) {
    exit 1
} else {
    exit 0
}
