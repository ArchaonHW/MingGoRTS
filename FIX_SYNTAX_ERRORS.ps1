# PowerShell script to fix common syntax errors in header files

# Function to fix unterminated character constants
function Repair-UnterminatedCharacterConstants {
    param($FilePath)
    
    $content = Get-Content $FilePath -Raw
    $originalContent = $content
    
    # Fix unterminated character constants (single quotes without closing)
    $content = $content -replace '(\w+)''\s*$', '$1'
    $content = $content -replace '(\w+)''\s*//', '$1 //'
    $content = $content -replace '(\w+)''\s*,', '$1,'
    $content = $content -replace '(\w+)''\s*\}', '$1}'
    
    # Fix malformed enum entries
    $content = $content -replace '(\w+),\s*//\s*[^\}]*\}', '$1 // $2 }'
    $content = $content -replace '(\w+),\s*//\s*[^,]*$', '$1 // $2'
    
    # Fix missing closing braces in enums
    $content = $content -replace '(\w+),\s*//.*\};', '$1 // $2' + [Environment]::NewLine + '};'
    
    if ($content -ne $originalContent) {
        Set-Content $FilePath $content -NoNewline
        Write-Host "Fixed: $FilePath" -ForegroundColor Green
    }
}

# Function to fix UENUM syntax errors
function Repair-UENUMSyntax {
    param($FilePath)
    
    $content = Get-Content $FilePath -Raw
    $originalContent = $content
    
    # Fix UENUM entries with misplaced API macros
    $content = $content -replace '(\w+),\s*//.*API.*', '$1 // $2'
    $content = $content -replace '(\w+)\s+API.*', '$1'
    
    # Fix UENUM entries with class/struct keywords
    $content = $content -replace '(\w+)\s+class\s*,', '$1,'
    $content = $content -replace '(\w+)\s+struct\s*,', '$1,'
    $content = $content -replace '(\w+)\s+USTRUCT\s*,', '$1,'
    $content = $content -replace '(\w+)\s+UENUM\s*,', '$1,'
    
    if ($content -ne $originalContent) {
        Set-Content $FilePath $content -NoNewline
        Write-Host "Fixed UENUM: $FilePath" -ForegroundColor Yellow
    }
}

# Function to fix missing closing braces
function Repair-MissingBraces {
    param($FilePath)
    
    $content = Get-Content $FilePath -Raw
    $originalContent = $content
    
    # Fix missing closing braces in enums
    $enumBlocks = [regex]::Matches($content, 'UENUM\s*\([^)]*\)\s*[^{]*\{([^}]*)')
    foreach ($match in $enumBlocks) {
        $enumContent = $match.Groups[1].Value
        if (-not $enumContent.EndsWith('}')) {
            $fixedEnum = $enumContent.TrimEnd() + [Environment]::NewLine + '};'
            $content = $content.Replace($match.Value, $match.Value.Replace($enumContent, $fixedEnum))
        }
    }
    
    # Fix EOF without closing brace
    if ($content.Contains('{') -and ($content.Split('{').Length - 1) -gt ($content.Split('}').Length - 1)) {
        $missingBraces = ($content.Split('{').Length - 1) - ($content.Split('}').Length - 1)
        $content += [Environment]::NewLine + ('}' * $missingBraces)
        Write-Host "Added $missingBraces closing braces to: $FilePath" -ForegroundColor Cyan
    }
    
    if ($content -ne $originalContent) {
        Set-Content $FilePath $content -NoNewline
    }
}

# Function to fix include order issues
function Repair-IncludeOrder {
    param($FilePath)
    
    $content = Get-Content $FilePath -Raw
    $originalContent = $content
    
    # Move .generated.h includes to the top
    $generatedInclude = [regex]::Match($content, '#include\s+"[^"]*\.generated\.h"')
    if ($generatedInclude.Success) {
        $otherIncludes = [regex]::Matches($content, '#include\s+"[^"]*(?<!\.generated)"')
        $content = $generatedInclude.Value + [Environment]::NewLine + [Environment]::NewLine
        foreach ($include in $otherIncludes) {
            $content += $include.Value + [Environment]::NewLine
        }
        $content = $content.TrimEnd()
    }
    
    if ($content -ne $originalContent) {
        Set-Content $FilePath $content -NoNewline
        Write-Host "Fixed includes: $FilePath" -ForegroundColor Magenta
    }
}

# Find all header files
$headerFiles = Get-ChildItem -Path "C:\HW\MingGoRTS" -Recurse -Filter "*.h" | Where-Object { $_.FullName -like "*.h" }

Write-Host "Found $($headerFiles.Count) header files to fix..." -ForegroundColor White

foreach ($file in $headerFiles) {
    Write-Host "Processing: $($file.FullName)" -ForegroundColor Gray
    
    try {
        Repair-UnterminatedCharacterConstants -FilePath $file.FullName
        Repair-UENUMSyntax -FilePath $file.FullName
        Repair-MissingBraces -FilePath $file.FullName
        Repair-IncludeOrder -FilePath $file.FullName
    }
    catch {
        Write-Host "Error processing $($file.FullName): $($_.Exception.Message)" -ForegroundColor Red
    }
}

Write-Host "Syntax error fixing completed!" -ForegroundColor Green
