Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MingWar-RTS Absolute Path Fix" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Change to project directory
$projectDir = "C:\HW\MingGoRTS"
cd $projectDir

Write-Host "Project Directory: $projectDir"
Write-Host ""

Write-Host "Step 1: Cleaning build files..." -ForegroundColor Yellow
# Clean old build files
$pathsToRemove = @("Intermediate", "Saved", "Binaries")
foreach ($path in $pathsToRemove) {
    $fullPath = Join-Path $projectDir $path
    if (Test-Path $fullPath) {
        Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $fullPath
        Write-Host "  Removed: $path"
    } else {
        Write-Host "  Not found: $path (skipping)"
    }
}
Write-Host "Cleaned." -ForegroundColor Green
Write-Host ""

Write-Host "Step 2: Verifying project structure..." -ForegroundColor Yellow
$requiredFiles = @(
    "MingWarRTS.uproject",
    "Source\MingWarRTS\Public\MingWarRTS.h",
    "Source\MingWarRTS\Private\MingWarRTS.cpp",
    "Source\MingWarRTS\MingWarRTS.Build.cs"
)

$allExist = $true
foreach ($file in $requiredFiles) {
    $fullPath = Join-Path $projectDir $file
    if (Test-Path $fullPath) {
        Write-Host "  [OK] $file" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
        $allExist = $false
    }
}

if (-not $allExist) {
    Write-Host ""
    Write-Host "ERROR: Some required files are missing!" -ForegroundColor Red
    Write-Host "Cannot proceed. Please check project structure."
    pause
    exit 1
}
Write-Host ""

Write-Host "Step 3: Creating professional Build.cs..." -ForegroundColor Yellow
$buildCsPath = "Source\MingWarRTS\MingWarRTS.Build.cs"
$buildCsContent = @'
using UnrealBuildTool;

public class MingWarRTS : ModuleRules
{
    public MingWarRTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrecompileForTargets = PrecompileTargetsType.Any;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Engine",
            "CoreUObject",
            "Slate",
            "SlateCore"
        });
    }
}
'@

Set-Content -Path $buildCsPath -Value $buildCsContent -Encoding UTF8
Write-Host "Build.cs created at: $buildCsPath" -ForegroundColor Green
Write-Host ""

Write-Host "Step 4: Generating project files with ABSOLUTE PATHS..." -ForegroundColor Yellow
Write-Host ""

$ubtPath = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$absProjectPath = Join-Path $projectDir "MingWarRTS.uproject"

Write-Host "UBT Path: $ubtPath"
Write-Host "Project: $absProjectPath"
Write-Host ""

if (Test-Path $ubtPath) {
    Write-Host "Attempt 1: Using UnrealBuildTool with absolute path..." -ForegroundColor Cyan
    & $ubtPath -projectfiles -project=$absProjectPath -game -progress
    if ($LASTEXITCODE -eq 0) {
        Write-Host "SUCCESS with UBT!" -ForegroundColor Green
        goto SUCCESS
    } else {
        Write-Host "UBT returned exit code: $LASTEXITCODE" -ForegroundColor Yellow
    }
} else {
    Write-Host "WARNING: UBT not found at: $ubtPath" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Attempt 2: Using Build.bat with absolute project path..." -ForegroundColor Cyan
$buildBat = "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat"
if (Test-Path $buildBat) {
    & $buildBat MingWarRTS Win64 Development -Project="$absProjectPath" -Progress
    if ($LASTEXITCODE -eq 0) {
        Write-Host "SUCCESS with Build.bat!" -ForegroundColor Green
        goto SUCCESS
    } else {
        Write-Host "Build.bat returned exit code: $LASTEXITCODE" -ForegroundColor Yellow
    }
} else {
    Write-Host "WARNING: Build.bat not found at: $buildBat" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Attempt 3: Let UE5 Editor handle it..." -ForegroundColor Cyan
Write-Host "Please open the project directly in UE5 Editor:"
Write-Host "  $absProjectPath"
Write-Host ""
Write-Host "UE5 will automatically generate project files." -ForegroundColor Green

Write-Host ""
Write-Host "========================================" -ForegroundColor Red
Write-Host "ERROR: All automated methods failed!" -ForegroundColor Red
Write-Host "========================================" -ForegroundColor Red
Write-Host ""
Write-Host "MANUAL SOLUTION:"
Write-Host "1. Close UE5 Editor completely"
Write-Host "2. Open File Explorer and navigate to:"
Write-Host "   $projectDir"
Write-Host "3. Double-click MingWarRTS.uproject"
Write-Host "4. When prompted, click 'Yes' to rebuild modules"
Write-Host "5. Wait 10-30 minutes for compilation"
Write-Host ""
Write-Host "If this still fails, check:"
Write-Host "- Visual Studio C++ is installed (Desktop development with C++)"
Write-Host "- Windows SDK is installed"
Write-Host "- UE5 5.7.4 installation is complete"
Write-Host ""
pause
exit 1

:SUCCESS
Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "SUCCESS: Project files generated!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Check for MingWarRTS.sln in: $projectDir"
Write-Host "2. Open MingWarRTS.sln in Visual Studio"
Write-Host "3. Build the project (Development | Win64)"
Write-Host "4. Double-click MingWarRTS.uproject to open in UE5"
Write-Host ""
Write-Host "OR (simpler):"
Write-Host "Just double-click MingWarRTS.uproject and UE5 will compile automatically."
Write-Host ""
pause