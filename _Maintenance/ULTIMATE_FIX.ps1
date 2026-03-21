Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MingWar-RTS Ultimate Fix" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Change to project directory
cd C:\HW\MingGoRTS

Write-Host "Step 1: Cleaning build files..." -ForegroundColor Yellow
# Clean old build files
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue Intermediate, Saved, Binaries
Write-Host "Cleaned." -ForegroundColor Green
Write-Host ""

Write-Host "Step 2: Creating professional Build.cs..." -ForegroundColor Yellow
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

Set-Content -Path "Source\MingWarRTS\MingWarRTS.Build.cs" -Value $buildCsContent -Encoding UTF8
Write-Host "Build.cs created." -ForegroundColor Green
Write-Host ""

Write-Host "Step 3: Generating project files..." -ForegroundColor Yellow
Write-Host ""

$ubtPath = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
$projectPath = "MingWarRTS.uproject"

if (Test-Path $ubtPath) {
    Write-Host "Using UnrealBuildTool at: $ubtPath"
    & $ubtPath -projectfiles -project=$projectPath -game -progress
    if ($LASTEXITCODE -eq 0) {
        goto SUCCESS
    }
}

Write-Host "UBT failed, trying Build.bat..." -ForegroundColor Yellow
$buildBat = "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat"
if (Test-Path $buildBat) {
    & $buildBat MingWarRTS Win64 Development -Project="MingWarRTS.uproject" -Progress
    if ($LASTEXITCODE -eq 0) {
        goto SUCCESS
    }
}

echo.
echo ========================================
echo ERROR: All methods failed!
echo ========================================
echo.
echo Please try:
echo 1. Open MingWarRTS.uproject in UE5 Editor
echo 2. Click 'Fix' button
echo 3. Check Visual Studio C++ installation
echo.
pause
exit 1

:SUCCESS
echo.
echo ========================================
echo SUCCESS: Project files generated!
echo ========================================
echo.
echo Next steps:
echo 1. Check for MingWarRTS.sln file
echo 2. Open in Visual Studio
echo 3. Build the project
echo 4. Double-click MingWarRTS.uproject to open in UE5
echo.
echo If you still see errors, try:
echo - 'Fix' button in UE5 Editor
echo - 'Generate Visual Studio project files'
echo.
pause