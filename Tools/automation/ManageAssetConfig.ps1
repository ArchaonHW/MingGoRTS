# MingGoRTS 素材部署配置管理器
# 管理UE素材部署的配置文件和規則

param(
    [string]$Action = "Show",
    [string]$ConfigName = "",
    [string]$ConfigValue = "",
    [string]$ImportPath = ""
)

$ProjectRoot = "C:\HW\MingGoRTS"
$ConfigFile = "$ProjectRoot\Tools\automation\AssetDeploymentConfig.json"
$TemplateDir = "$ProjectRoot\Tools\automation\Templates"

# 默認配置
$DefaultConfig = @{
    UEVersion = "5.7"
    AutoImport = $true
    GenerateMaterials = $true
    GenerateLODs = $true
    CompressTextures = $true
    MaxTextureSize = 2048
    DefaultTextureFormat = "TGA"
    AssetCategories = @{
        Textures = @{
            Extensions = @(".png", ".jpg", ".jpeg", ".tga", ".bmp", ".psd")
            TargetPath = "Content/Art"
            ImportSettings = @{
                Compression = "Default"
                MipGenSettings = "FromTextureGroup"
                LODGroup = "TEXTUREGROUP_World"
            }
        }
        Models = @{
            Extensions = @(".fbx", ".obj", ".3ds", ".dae")
            TargetPath = "Content/Art"
            ImportSettings = @{
                ConvertSceneUnit = $true
                ForceFrontXAxis = $false
                ConvertScene = $true
            }
        }
        Audio = @{
            Extensions = @(".wav", ".mp3", ".ogg", ".flac")
            TargetPath = "Content/Audio"
            ImportSettings = @{
                CompressionQuality = 40
                Streaming = $true
            }
        }
        UI = @{
            Extensions = @(".png", ".jpg")
            TargetPath = "Content/UI"
            ImportSettings = @{
                Compression = "UserInterface2D"
                LODGroup = "TEXTUREGROUP_UI"
            }
        }
    }
    ImportRules = @(
        @{
            Pattern = "*_N.*"
            Description = "法線貼圖"
            Settings = @{ CompressionSettings = "Normalmap" }
        }
        @{
            Pattern = "*_ORM.*"
            Description = "ORM貼圖 (Occlusion, Roughness, Metallic)"
            Settings = @{ CompressionSettings = "Masks" }
        }
        @{
            Pattern = "*_D.*|*_Albedo.*|*_BaseColor.*"
            Description = "基礎顏色/漫反射"
            Settings = @{ sRGB = $true }
        }
        @{
            Pattern = "*_E.*|*_Emissive.*"
            Description = "自發光"
            Settings = @{ CompressionSettings = "VectorDisplacementMap" }
        }
    )
    NamingConventions = @{
        Textures = "T_{Name}_{Type}"
        Materials = "M_{Name}"
        StaticMeshes = "SM_{Name}"
        SkeletalMeshes = "SK_{Name}"
        Animations = "A_{Name}"
        Sounds = "S_{Name}"
        Blueprints = "BP_{Name}"
        UI = "UI_{Name}"
    }
}

# 加載配置
function Load-Config {
    if (Test-Path $ConfigFile) {
        try {
            $Content = Get-Content $ConfigFile -Raw
            return $Content | ConvertFrom-Json
        }
        catch {
            Write-Host "配置檔案載入失敗，使用預設值" -ForegroundColor Yellow
            return $DefaultConfig
        }
    }
    return $DefaultConfig
}

# 保存配置
function Save-Config {
    param($Config)
    $Config | ConvertTo-Json -Depth 10 | Out-File -FilePath $ConfigFile -Encoding UTF8
    Write-Host "✓ 配置已保存: $ConfigFile" -ForegroundColor Green
}

# 顯示當前配置
function Show-Config {
    $Config = Load-Config
    
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "    UE素材部署配置" -ForegroundColor Cyan
    Write-Host "========================================`n" -ForegroundColor Cyan
    
    Write-Host "UE版本: $($Config.UEVersion)" -ForegroundColor White
    Write-Host "自動導入: $($Config.AutoImport)" -ForegroundColor White
    Write-Host "生成材質: $($Config.GenerateMaterials)" -ForegroundColor White
    Write-Host "最大紋理尺寸: $($Config.MaxTextureSize)" -ForegroundColor White
    
    Write-Host "`n素材分類設定:" -ForegroundColor Yellow
    foreach ($Cat in $Config.AssetCategories.PSObject.Properties) {
        Write-Host "  $($Cat.Name):" -ForegroundColor Green
        Write-Host "    擴展名: $($Cat.Value.Extensions -join ', ')" -ForegroundColor Gray
        Write-Host "    目標路徑: $($Cat.Value.TargetPath)" -ForegroundColor Gray
    }
    
    Write-Host "`n導入規則:" -ForegroundColor Yellow
    foreach ($Rule in $Config.ImportRules) {
        Write-Host "  模式: $($Rule.Pattern)" -ForegroundColor Green
        Write-Host "    描述: $($Rule.Description)" -ForegroundColor Gray
    }
    
    Write-Host "`n命名規範:" -ForegroundColor Yellow
    foreach ($Conv in $Config.NamingConventions.PSObject.Properties) {
        Write-Host "  $($Conv.Name): $($Conv.Value)" -ForegroundColor Gray
    }
    
    Write-Host "`n========================================`n" -ForegroundColor Cyan
}

# 設定配置項
function Set-ConfigItem {
    param($Name, $Value)
    
    $Config = Load-Config
    
    # 處理嵌套路徑 (如: AssetCategories.Textures.Extensions)
    $PathParts = $Name -split '\.'
    $Target = $Config
    
    for ($i = 0; $i -lt $PathParts.Count - 1; $i++) {
        $Target = $Target.$($PathParts[$i])
    }
    
    $LastKey = $PathParts[-1]
    
    # 嘗試轉換為適當的類型
    if ($Value -eq "true" -or $Value -eq "false") {
        $Value = [bool]::Parse($Value)
    } elseif ($Value -match "^\d+$") {
        $Value = [int]::Parse($Value)
    }
    
    $Target.$LastKey = $Value
    Save-Config -Config $Config
    Write-Host "✓ 設定已更新: $Name = $Value" -ForegroundColor Green
}

# 重置配置
function Reset-Config {
    Save-Config -Config $DefaultConfig
    Write-Host "✓ 配置已重置為預設值" -ForegroundColor Green
}

# 導入外部配置
function Import-Config {
    param($Path)
    
    if (!(Test-Path $Path)) {
        Write-Host "✗ 配置文件未找到: $Path" -ForegroundColor Red
        return
    }
    
    try {
        $Content = Get-Content $Path -Raw
        $NewConfig = $Content | ConvertFrom-Json
        Save-Config -Config $NewConfig
        Write-Host "✓ 配置已從 $Path 導入" -ForegroundColor Green
    }
    catch {
        Write-Host "✗ 配置導入失敗: $_" -ForegroundColor Red
    }
}

# 導出配置
function Export-Config {
    param($Path)
    
    $Config = Load-Config
    $Config | ConvertTo-Json -Depth 10 | Out-File -FilePath $Path -Encoding UTF8
    Write-Host "✓ 配置已導出到: $Path" -ForegroundColor Green
}

# 驗證配置
function Test-Config {
    $Config = Load-Config
    $Valid = $true
    
    Write-Host "驗證配置..." -ForegroundColor Yellow
    
    # 檢查必要欄位
    if (!$Config.UEVersion) {
        Write-Host "✗ 缺少 UEVersion 設定" -ForegroundColor Red
        $Valid = $false
    }
    
    if (!$Config.AssetCategories) {
        Write-Host "✗ 缺少 AssetCategories 設定" -ForegroundColor Red
        $Valid = $false
    }
    
    # 檢查路徑有效性
    $UEPath = "C:\Program Files\Epic Games\UE_$($Config.UEVersion)\Engine\Binaries\Win64\UnrealEditor.exe"
    if (!(Test-Path $UEPath)) {
        Write-Host "⚠ UE路徑可能無效: $UEPath" -ForegroundColor Yellow
    }
    
    if ($Valid) {
        Write-Host "✓ 配置驗證通過" -ForegroundColor Green
    }
    
    return $Valid
}

# 創建部署模板
function New-DeploymentTemplate {
    $Template = @{
        TemplateName = "MingGoRTS資產部署"
        Description = "標準UE資產部署模板"
        Steps = @(
            "掃描源目錄素材"
            "分類和驗證素材"
            "轉換紋理格式"
            "複製到Content目錄"
            "生成材質實例"
            "執行UE導入"
            "生成部署報告"
        )
        Filters = @{
            Include = @("*.png", "*.jpg", "*.fbx", "*.wav")
            Exclude = @("*.tmp", "*.bak", "*~*")
        }
        Processing = @{
            Textures = @{
                GenerateMips = $true
                Compress = $true
                MaxSize = 2048
            }
            Models = @{
                GenerateLODs = $true
                Collision = $true
            }
        }
    }
    
    if (!(Test-Path $TemplateDir)) {
        New-Item -ItemType Directory -Path $TemplateDir -Force | Out-Null
    }
    
    $TemplatePath = "$TemplateDir\StandardDeployment.json"
    $Template | ConvertTo-Json -Depth 5 | Out-File -FilePath $TemplatePath -Encoding UTF8
    Write-Host "✓ 模板已創建: $TemplatePath" -ForegroundColor Green
}

# 主執行
switch ($Action.ToLower()) {
    "show" { Show-Config }
    "set" { 
        if ($ConfigName -and $ConfigValue) {
            Set-ConfigItem -Name $ConfigName -Value $ConfigValue
        } else {
            Write-Host "用法: ManageAssetConfig.ps1 -Action Set -ConfigName <名稱> -ConfigValue <值>" -ForegroundColor Yellow
        }
    }
    "reset" { Reset-Config }
    "import" { 
        if ($ImportPath) {
            Import-Config -Path $ImportPath
        } else {
            Write-Host "請指定 -ImportPath 參數" -ForegroundColor Yellow
        }
    }
    "export" { 
        $ExportPath = if ($ImportPath) { $ImportPath } else { "$ProjectRoot\AssetDeploymentConfig_Export.json" }
        Export-Config -Path $ExportPath
    }
    "test" { Test-Config }
    "template" { New-DeploymentTemplate }
    default { 
        Write-Host "未知操作: $Action" -ForegroundColor Red
        Write-Host "可用操作: Show, Set, Reset, Import, Export, Test, Template" -ForegroundColor Yellow
    }
}
