# Comprehensive fix for MingGoRTS compilation errors
Write-Host "Starting comprehensive compilation error fixes..."

# Get all header files that might have syntax errors
$headerFiles = @(
    "c:\HW\MingGoRTS\Source\MingGoRTS\Public\*.h",
    "c:\HW\MingGoRTS\Plugins\*\Source\*\Public\*.h"
)

$fixedCount = 0
$totalFiles = 0

foreach ($pattern in $headerFiles) {
    $files = Get-ChildItem -Path (Split-Path $pattern)[0] -Filter $pattern.Split('\')[-1] -Recurse -File
    foreach ($file in $files) {
        $totalFiles++
        $filePath = $file.FullName
        Write-Host "Processing: $filePath"
        
        try {
            $content = Get-Content $filePath -Raw -Encoding UTF8
            $originalContent = $content
            
            # Fix 1: Replace } with ) in function declarations
            $content = $content -replace '\}\s*$', ')'
            $content = $content -replace 'void\s+(\w+)\s*\(\s*\}\s*$', 'void $1();'
            $content = $content -replace 'bool\s+(\w+)\s*\(\s*\}\s*$', 'bool $1();'
            $content = $content -replace 'FString\s+(\w+)\s*\(\s*\}\s*$', 'FString $1();'
            $content = $content -replace 'int\s+(\w+)\s*\(\s*\}\s*$', 'int $1();'
            $content = $content -replace 'float\s+(\w+)\s*\(\s*\}\s*$', 'float $1();'
            
            # Fix 2: Fix unterminated character constants
            $content = $content -replace "'\s*$", "';"
            
            # Fix 3: Fix delegate declarations
            $content = $content -replace 'DECLARE_DYNAMIC_MULTICAST_DELEGATE.*\{\s*$', 'DECLARE_DYNAMIC_MULTICAST_DELEGATE'
            
            # Fix 4: Add missing closing braces for classes
            if ($content -match 'class\s+\w+\s*:\s*public\s+\w+\s*\{') {
                $openBraces = ($content | Select-String -Pattern '\{' -AllMatches).Count
                $closeBraces = ($content | Select-String -Pattern '\}' -AllMatches).Count
                $braceDiff = $openBraces - $closeBraces
                
                if ($braceDiff -gt 1) {
                    $content += "`n};"
                    Write-Host "  Added missing closing brace"
                }
            }
            
            # Only write if content changed
            if ($content -ne $originalContent) {
                Set-Content $filePath -Value $content -Encoding UTF8 -NoNewline
                $fixedCount++
                Write-Host "  FIXED: $filePath" -ForegroundColor Green
            } else {
                Write-Host "  OK: $filePath" -ForegroundColor Gray
            }
        } catch {
            Write-Host "  ERROR processing $filePath : $($_.Exception.Message)" -ForegroundColor Red
        }
    }
}

Write-Host ""
Write-Host "================================"
Write-Host "COMPREHENSIVE FIX SUMMARY"
Write-Host "================================"
Write-Host "Total files processed: $totalFiles"
Write-Host "Files fixed: $fixedCount"
Write-Host "Files OK: $($totalFiles - $fixedCount)"
Write-Host "================================"

if ($fixedCount -gt 0) {
    Write-Host "Compilation fixes completed! Try building again." -ForegroundColor Green
} else {
    Write-Host "No fixes needed or all files already OK." -ForegroundColor Yellow
}
