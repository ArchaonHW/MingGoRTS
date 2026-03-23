# Debug git status output
Write-Host "=== Debug Git Status ===" -ForegroundColor Yellow

try {
    $result = & git status --porcelain
    Write-Host "Git command exit code: $LASTEXITCODE" -ForegroundColor Cyan
    Write-Host "Result type: $($result.GetType().Name)" -ForegroundColor Cyan
    Write-Host "Result is null: $($null -eq $result)" -ForegroundColor Cyan
    Write-Host "Result length: $($result.Length)" -ForegroundColor Cyan
    Write-Host "Result content: '$result'" -ForegroundColor Cyan
    Write-Host "Result trimmed: '$($result.Trim())'" -ForegroundColor Cyan
    $isEmpty = $result.Trim() -eq ""
    Write-Host "Is empty: '$isEmpty'" -ForegroundColor Cyan
} catch {
    Write-Host "Error: $($_.Exception.Message)" -ForegroundColor Red
}
