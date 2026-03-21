Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MingWar-RTS Debug Tool" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "This tool will check:"
Write-Host "- UE5 installation"
Write-Host "- Project structure"
Write-Host "- C++ source files"
Write-Host "- Build configuration"
Write-Host ""
Pause

cd C:\HW\MingGoRTS

Write-Host ""
Write-Host "[1] Checking UE5 installation..." -ForegroundColor Yellow
Write-Host ""
$ue5Path = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe"
if (Test-Path $ue5Path) {
    Write-Host "[OK] UE5 Editor found" -ForegroundColor Green
} else {
    Write-Host "[ERROR] UE5 Editor not found!" -ForegroundColor Red
}

$ubtPath = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool.exe"
if (Test-Path $ubtPath) {
    Write-Host "[OK] UnrealBuildTool found" -ForegroundColor Green
} else {
    Write-Host "[ERROR] UnrealBuildTool not found!" -ForegroundColor Red
}

Write-Host ""
Write-Host "[2] Checking project structure..." -ForegroundColor Yellow
Write-Host ""
if (Test-Path "MingWarRTS.uproject") {
    Write-Host "[OK] MingWarRTS.uproject exists" -ForegroundColor Green
    $content = Get-Content "MingWarRTS.uproject" -Raw
    if ($content -match "MingWarRTS") {
        Write-Host "[OK] Project name correct" -ForegroundColor Green
    } else {
        Write-Host "[WARN] Project name mismatch" -ForegroundColor Yellow
    }
} else {
    Write-Host "[ERROR] MingWarRTS.uproject missing!" -ForegroundColor Red
}

if (Test-Path "Source\MingWarRTS") {
    Write-Host "[OK] Source\MingWarRTS directory exists" -ForegroundColor Green
} else {
    Write-Host "[ERROR] Source\MingWarRTS missing!" -ForegroundColor Red
}

Write-Host ""
Write-Host "[3] Checking C++ source files..." -ForegroundColor Yellow
Write-Host ""
$requiredFiles = @(
    "Source\MingWarRTS\Public\MingWarRTS.h",
    "Source\MingWarRTS\Public\RTSUnit.h",
    "Source\MingWarRTS\Public\RTSBuilding.h",
    "Source\MingWarRTS\Public\RTSPlayerController.h",
    "Source\MingWarRTS\Public\ResourceManager.h",
    "Source\MingWarRTS\Private\MingWarRTS.cpp",
    "Source\MingWarRTS\Private\RTSUnit.cpp",
    "Source\MingWarRTS\Private\RTSBuilding.cpp",
    "Source\MingWarRTS\Private\RTSPlayerController.cpp",
    "Source\MingWarRTS\Private\ResourceManager.cpp"
)

foreach ($file in $requiredFiles) {
    if (Test-Path $file) {
        Write-Host "[OK] $($file.Split('\')[-1])" -ForegroundColor Green
    } else {
        Write-Host "[ERROR] $($file.Split('\')[-1]) missing!" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "[4] Checking Build.cs..." -ForegroundColor Yellow
Write-Host ""
$buildCs = "Source\MingWarRTS\MingWarRTS.Build.cs"
if (Test-Path $buildCs) {
    Write-Host "[OK] MingWarRTS.Build.cs exists" -ForegroundColor Green
    Write-Host "Content:"
    Get-Content $buildCs
} else {
    Write-Host "[ERROR] MingWarRTS.Build.cs missing!" -ForegroundColor Red
}

Write-Host ""
Write-Host "[5] Test compile with UBT (dry run)..." -ForegroundColor Yellow
Write-Host ""
Write-Host "This will attempt to generate project files without building."
Write-Host ""

if (Test-Path $ubtPath) {
    & $ubtPath -projectfiles -project="MingWarRTS.uproject" -game -progress 2>&1 |
        Select-String -Pattern "error|warning|success|failed" -CaseSensitive:$false |
        ForEach-Object { $_.Line }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Debug complete!" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Fix any errors shown above"
Write-Host "2. Double-click MingWarRTS.uproject to open in UE5"
Write-Host "3. Wait for compilation"
Write-Host ""
Pause