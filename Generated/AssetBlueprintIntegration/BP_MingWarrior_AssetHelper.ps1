# Asset-Blueprint Integration Helper Script
# 蝝????誨蝣潮??航??抵??# Generated: 03/23/2026 02:58:27

param(
    [string]$IntegrationConfigPath = "C:\HW\MingGoRTS\Tools\build\..\..\Generated\AssetBlueprintIntegration\BP_MingWarrior_AssetRefConfig_20260323_025826.json",
    [string]$Operation = "Validate", # Validate, Load, Bind, Status
    [string]$AssetName = ""
)

function Show-IntegrationStatus {
    param([string]$ConfigPath)
    
    if (Test-Path $ConfigPath) {
        $config = Get-Content $ConfigPath | ConvertFrom-Json
        Write-Host "=== 鞈??????| Asset-Blueprint Integration Status ==="
        Write-Host "Integration ID: $($config.IntegrationID)"
        Write-Host "Asset Type: $($config.AssetType)"
        Write-Host "Blueprint: $($config.BlueprintName)"
        Write-Host "Generated: $($config.GeneratedAt)"
        Write-Host ""
        Write-Host "Asset References:"
        foreach ($ref in $config.AssetReferences) {
            $status = if (Test-Path "$ProjectPath\Content\$($ref.AssetPath -replace '/','\\').uasset") { "??摮 | Exists" } else { "??蝻箏仃 | Missing" }
            Write-Host "  [$($ref.Type)] $($ref.AssetName) (Priority: $($ref.LoadPriority)) - $status"
        }
    } else {
        Write-Error "Configuration file not found: $ConfigPath"
    }
}

function Validate-AssetPaths {
    param([string]$ConfigPath)
    
    $config = Get-Content $ConfigPath | ConvertFrom-Json
    $missing = @()
    $existing = @()
    
    foreach ($ref in $config.AssetReferences) {
        $assetFile = "$ProjectPath\Content\$($ref.AssetPath -replace '/','\\').uasset"
        if (Test-Path $assetFile) {
            $existing += $ref.AssetName
        } else {
            $missing += $ref.AssetName
        }
    }
    
    Write-Host "=== 鞈頝臬?撽? | Asset Path Validation ==="
    Write-Host "?暹?鞈 | Existing: $($existing.Count)"
    Write-Host "蝻箏仃鞈 | Missing: $($missing.Count)"
    
    if ($missing.Count -gt 0) {
        Write-Host ""
        Write-Host "Missing assets:" -ForegroundColor Red
        $missing | ForEach-Object { Write-Host "  - $_" -ForegroundColor Red }
    }
    
    return $missing.Count -eq 0
}

# Main execution
switch ($Operation) {
    "Status" { Show-IntegrationStatus $IntegrationConfigPath }
    "Validate" { Validate-AssetPaths $IntegrationConfigPath }
    default { Show-IntegrationStatus $IntegrationConfigPath }
}
