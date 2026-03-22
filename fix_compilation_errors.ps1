# Fix compilation errors in MingGoRTS
# This script fixes common syntax errors

Write-Host "Starting compilation error fixes..."

# Files that need fixing based on build errors
$filesToFix = @(
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\Replay\MingRTSReplayManager.h",
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\SelfLearning\MingRTSSelfLearningSystemTest.h",
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\Mod\MingRTSModSystem.h",
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\Localization\MingRTSLocalizationSubsystem.h",
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\Replay\MingRTSReplaySystem.h",
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\QuickStart\MingRTSQuickStart.h"
)

foreach ($file in $filesToFix) {
    if (Test-Path $file) {
        Write-Host "Checking file: $file"
        $content = Get-Content $file -Raw
        
        # Fix common syntax errors
        $content = $content -replace '\}\s*$', ');'
        $content = $content -replace 'void\s+(\w+)\s*\(\s*\}\s*$', 'void $1();'
        $content = $content -replace 'bool\s+(\w+)\s*\(\s*\}\s*$', 'bool $1();'
        $content = $content -replace 'FString\s+(\w+)\s*\(\s*\}\s*$', 'FString $1();'
        $content = $content -replace 'int\s+(\w+)\s*\(\s*\}\s*$', 'int $1();'
        
        # Fix unterminated character constants
        $content = $content -replace "'\s*$'", "';"
        
        # Fix delegate declarations
        $content = $content -replace 'DECLARE_DYNAMIC_MULTICAST_DELEGATE.*\{\s*$', 'DECLARE_DYNAMIC_MULTICAST_DELEGATE'
        
        Set-Content $file -Value $content -NoNewline
        Write-Host "Fixed: $file"
    }
}

Write-Host "Compilation error fixes completed!"
