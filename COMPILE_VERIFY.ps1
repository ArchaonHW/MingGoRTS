# MingGoRTS CulturalAdaptationSystem Compile Verification
# PowerShell Script

$EnginePath = "C:\Program Files\Epic Games\UE_5.4"
$UBT = "$EnginePath\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$ProjectPath = "C:\HW\MingGoRTS\MingGoRTS.uproject"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MingGoRTS CulturalAdaptationSystem Compile Verification" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check engine path
Write-Host "[Step 1/3] Verifying Engine Path..." -ForegroundColor Yellow
if (-not (Test-Path $UBT)) {
    Write-Host "[ERROR] UnrealBuildTool not found at: $UBT" -ForegroundColor Red
    Write-Host "Please verify UE5.4 installation." -ForegroundColor Red
    exit 1
}
Write-Host "Engine path verified!" -ForegroundColor Green
Write-Host ""

# Start compilation
Write-Host "[Step 2/3] Starting Compilation..." -ForegroundColor Yellow
Write-Host "Command: UnrealBuildTool.exe MingGoRTS Win64 Development" -ForegroundColor Gray
Write-Host "This may take 2-5 minutes..." -ForegroundColor Gray
Write-Host ""

try {
    $process = Start-Process -FilePath $UBT -ArgumentList "MingGoRTS", "Win64", "Development", "-Project=$ProjectPath" -WorkingDirectory "C:\HW\MingGoRTS" -Wait -PassThru -NoNewWindow
    $exitCode = $process.ExitCode
} catch {
    Write-Host "[ERROR] Failed to start compilation: $_" -ForegroundColor Red
    exit 1
}

# Check result
Write-Host ""
Write-Host "[Step 3/3] Compilation Result" -ForegroundColor Yellow
Write-Host "Exit Code: $exitCode" -ForegroundColor Gray
Write-Host ""

if ($exitCode -eq 0) {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "SUCCESS: Compilation Verified!" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "All modifications compile correctly:" -ForegroundColor White
    Write-Host "  - Thread safety (FCriticalSection)" -ForegroundColor Green
    Write-Host "  - LRU cache implementation" -ForegroundColor Green
    Write-Host "  - Cache statistics tracking" -ForegroundColor Green
    Write-Host "  - Enum validation" -ForegroundColor Green
    Write-Host ""
    Write-Host "Next Step: Run unit tests MingRTSLocalizationSystemTest" -ForegroundColor Cyan
    Write-Host ""
} else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "FAILED: Compilation Errors Found" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please check:" -ForegroundColor Yellow
    Write-Host "  1. Include statements in header file" -ForegroundColor White
    Write-Host "  2. Syntax in .cpp implementations" -ForegroundColor White
    Write-Host "  3. Missing dependencies" -ForegroundColor White
    Write-Host ""
}

Read-Host "Press Enter to continue"
exit $exitCode
