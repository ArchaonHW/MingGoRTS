# MingGoRTS 自动化素材生成与部署系统
# Auto-Generate & Deploy Assets to Unreal Engine
# 功能：自动生成素材（音乐/美术）并自动部署到UE项目中

param(
    [string]$AssetType = "All",           # Music, Art, All
    [string]$Faction = "Beiyang",         # 指定势力（用于美术资源）
    [switch]$SkipGeneration = $false,     # 跳过生成，仅部署
    [switch]$AutoImport = $false,          # 自动导入UE
    [switch]$Silent = $false,             # 静默模式
    [switch]$GenerateMaterials,         # 为纹理生成材质
    [int]$MaxRetries = 3
)

$ErrorActionPreference = "Stop"

# ============================================
# 配置路径
# ============================================
$ProjectRoot = "C:\HW\MingGoRTS"
$ToolsPath = "$ProjectRoot\Tools"
$AIScriptsPath = "$ToolsPath\ai"
$AutomationPath = "$ToolsPath\automation"
$ContentDir = "$ProjectRoot\Content"
$SourceAssetsDir = "$ProjectRoot\SourceAssets"
$GeneratedDir = "$ProjectRoot\Generated"
$LogDir = "$ProjectRoot\Logs\AutoDeploy"

# 确保目录存在
@($LogDir, $GeneratedDir, "$SourceAssetsDir\Music", "$SourceAssetsDir\Art", "$SourceAssetsDir\UI") | ForEach-Object {
    if (!(Test-Path $_)) { New-Item -ItemType Directory -Path $_ -Force | Out-Null }
}

# ============================================
# 日志系统
# ============================================
$Script:LogBuffer = @()
function Write-Log {
    param(
        [string]$Message,
        [string]$Level = "INFO",
        [string]$Category = ""
    )
    $Timestamp = Get-Date -Format "HH:mm:ss"
    $Prefix = if ($Category) { "[$Category] " } else { "" }
    $LogLine = "[$Timestamp] [$Level] $Prefix$Message"
    
    $Color = switch($Level) {
        "ERROR" { "Red" }
        "WARN"  { "Yellow" }
        "INFO"  { "Cyan" }
        "SUCCESS" { "Green" }
        "DEBUG" { "Gray" }
        default { "White" }
    }
    
    Write-Host $LogLine -ForegroundColor $Color
    Add-Content "$LogDir\AutoDeploy_$(Get-Date -Format 'yyyyMMdd').log" $LogLine
}

# ============================================
# 阶段1: 生成素材
# ============================================
function Invoke-AssetGeneration {
    param([string]$Type)
    
    Write-Log "=== 阶段1: 生成素材 ($Type) ===" "INFO" "GENERATE"
    
    $GeneratedFiles = @()
    
    switch ($Type) {
        "Music" { $GeneratedFiles += Invoke-MusicGeneration }
        "Art"   { $GeneratedFiles += Invoke-ArtGeneration }
        "All"   { 
            $GeneratedFiles += Invoke-MusicGeneration
            $GeneratedFiles += Invoke-ArtGeneration
        }
    }
    
    Write-Log "素材生成完成，共 $($GeneratedFiles.Count) 个文件" "SUCCESS" "GENERATE"
    return $GeneratedFiles
}

# 生成音乐
function Invoke-MusicGeneration {
    Write-Log "开始生成音乐素材..." "INFO" "MUSIC"
    
    $MusicScript = "$AIScriptsPath\auto_generate.ps1"
    if (!(Test-Path $MusicScript)) {
        Write-Log "音乐生成脚本未找到: $MusicScript" "ERROR" "MUSIC"
        return @()
    }
    
    try {
        # 执行音乐生成脚本（简化参数）
        $Arguments = @()
        if ($Silent) { $Arguments += "-Silent" }
        if ($MaxRetries -ne 3) { $Arguments += "-MaxRetries"; $Arguments += $MaxRetries }
        
        & $MusicScript @Arguments 2>&1 | Out-Null
        $ExitCode = $LASTEXITCODE
        
        if ($ExitCode -eq 0) {
            Write-Log "音乐生成成功" "SUCCESS" "MUSIC"
            
            # 收集生成的文件
            $MusicDir = "$ContentDir\Audio\Generated\Music"
            if (Test-Path $MusicDir) {
                $Files = Get-ChildItem -Path $MusicDir -Filter "*.wav" -Recurse | 
                    Select-Object -ExpandProperty FullName
                Write-Log "找到 $($Files.Count) 个音乐文件" "INFO" "MUSIC"
                return $Files
            }
        } else {
            Write-Log "音乐生成失败 (退出码: $ExitCode)" "ERROR" "MUSIC"
        }
    }
    catch {
        Write-Log "音乐生成异常: $_" "ERROR" "MUSIC"
    }
    
    return @()
}

# 生成美术资源 (使用epic_asset_generator)
function Invoke-ArtGeneration {
    Write-Log "开始生成美术素材 ($Faction)..." "INFO" "ART"
    
    $ArtScript = "$AIScriptsPath\epic_assets_generate.ps1"
    if (!(Test-Path $ArtScript)) {
        Write-Log "美术生成脚本未找到: $ArtScript" "ERROR" "ART"
        return @()
    }
    
    try {
        # 设置环境变量指定势力
        $env:MING_FACTION = $Faction
        
        # 执行美术生成（简化参数）
        $Arguments = @("-All")
        if ($MaxRetries -ne 3) { $Arguments += "-MaxRetries"; $Arguments += $MaxRetries }
        
        & $ArtScript @Arguments 2>&1 | Out-Null
        $ExitCode = $LASTEXITCODE
        
        if ($ExitCode -eq 0) {
            Write-Log "$Faction 美术资源生成成功" "SUCCESS" "ART"
            
            # 收集生成的文件
            $ArtOutputDir = "$ContentDir\Art\Generated"
            if (Test-Path $ArtOutputDir) {
                $Files = Get-ChildItem -Path $ArtOutputDir -File -Recurse | 
                    Where-Object { $_.Extension -in @('.png', '.jpg', '.fbx') } |
                    Select-Object -ExpandProperty FullName
                Write-Log "找到 $($Files.Count) 个美术文件" "INFO" "ART"
                return $Files
            }
        } else {
            Write-Log "美术生成失败 (退出码: $ExitCode)" "ERROR" "ART"
        }
    }
    catch {
        Write-Log "美术生成异常: $_" "ERROR" "ART"
    }
    
    return @()
}

# ============================================
# 阶段2: 部署素材到UE
# ============================================
function Invoke-AssetDeployment {
    param([array]$FilesToDeploy)
    
    Write-Log "=== 阶段2: 部署素材到UE ===" "INFO" "DEPLOY"
    
    if ($FilesToDeploy.Count -eq 0) {
        Write-Log "没有需要部署的文件" "WARN" "DEPLOY"
        return
    }
    
    $DeployScript = "$AutomationPath\DeployUEAssets.ps1"
    if (!(Test-Path $DeployScript)) {
        Write-Log "部署脚本未找到: $DeployScript" "ERROR" "DEPLOY"
        return
    }
    
    # 分类整理文件
    $CategorizedFiles = @{}
    foreach ($File in $FilesToDeploy) {
        $Category = Get-FileCategory -FilePath $File
        if (!$CategorizedFiles[$Category]) {
            $CategorizedFiles[$Category] = @()
        }
        $CategorizedFiles[$Category] += $File
    }
    
    # 显示部署计划
    Write-Log "部署计划:" "INFO" "DEPLOY"
    foreach ($Cat in $CategorizedFiles.Keys) {
        Write-Log "  $Cat`: $($CategorizedFiles[$Cat].Count) 个文件" "DEBUG" "DEPLOY"
    }
    
    # 执行部署
    $SuccessCount = 0
    $FailCount = 0
    
    foreach ($Category in $CategorizedFiles.Keys) {
        $Files = $CategorizedFiles[$Category]
        Write-Log "部署 $Category 类别 ($($Files.Count) 个文件)..." "INFO" "DEPLOY"
        
        # 创建临时目录
        $TempDir = "$GeneratedDir\$Category"
        New-Item -ItemType Directory -Path $TempDir -Force | Out-Null
        
        # 复制文件到临时目录
        foreach ($File in $Files) {
            $Dest = Join-Path $TempDir (Split-Path $File -Leaf)
            Copy-Item $File $Dest -Force
        }
        
        # 调用部署脚本
        try {
            & $DeployScript -SourcePath $TempDir -TargetCategory $Category -BatchMode -GenerateMaterials:$GenerateMaterials
            $SuccessCount += $Files.Count
        }
        catch {
            Write-Log "部署失败: $_" "ERROR" "DEPLOY"
            $FailCount += $Files.Count
        }
    }
    
    Write-Log "部署完成: 成功 $SuccessCount, 失败 $FailCount" "SUCCESS" "DEPLOY"
}

# 判断文件类别
function Get-FileCategory {
    param([string]$FilePath)
    
    $FileName = (Split-Path $FilePath -Leaf).ToLower()
    $Ext = [System.IO.Path]::GetExtension($FilePath).ToLower()
    
    # 根据文件名判断
    switch -Regex ($FileName) {
        "music|audio|sound|bgm|sfx" { return "Audio" }
        "ui|icon|button|hud|panel|menu" { return "UI" }
        "vfx|effect|particle|niagara" { return "VFX" }
        "character|npc|player|hero|unit" { return "Characters" }
        "building|architecture|structure|house" { return "Environment" }
        "weapon|item|prop|equipment|armor" { return "Art" }
    }
    
    # 根据扩展名判断
    switch ($Ext) {
        ".mp3" { return "Audio" }
        ".ogg" { return "Audio" }
        ".fbx" { return "Art" }
        ".obj" { return "Art" }
        ".png" { return "Art" }
        ".jpg" { return "Art" }
        ".tga" { return "Art" }
        default { return "Art" }
    }
}

# ============================================
# 阶段3: UE自动导入
# ============================================
function Invoke-UEAutoImport {
    Write-Log "=== 阶段3: UE自动导入 ===" "INFO" "IMPORT"
    
    $UEEditorPath = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
    $UProjectPath = "$ProjectRoot\MingGoRTS.uproject"
    
    if (!(Test-Path $UEEditorPath)) {
        Write-Log "UE编辑器未找到: $UEEditorPath" "ERROR" "IMPORT"
        return $false
    }
    
    if (!(Test-Path $UProjectPath)) {
        Write-Log "项目文件未找到: $UProjectPath" "ERROR" "IMPORT"
        return $false
    }
    
    # 导入参数
    $ImportArgs = @(
        "$UProjectPath"
        "-run=ImportAssets"
        "-importpath=$ContentDir"
        "-destpath=/Game"
        "-importsettings=ImportAll"
        "-unattended"
        "-nosplash"
        "-stdout"
    )
    
    try {
        Write-Log "启动UE自动导入..." "INFO" "IMPORT"
        $Process = Start-Process -FilePath $UEEditorPath -ArgumentList $ImportArgs -Wait -PassThru -WindowStyle Hidden
        
        if ($Process.ExitCode -eq 0) {
            Write-Log "UE导入成功" "SUCCESS" "IMPORT"
            return $true
        } else {
            Write-Log "UE导入失败 (退出码: $($Process.ExitCode))" "ERROR" "IMPORT"
            return $false
        }
    }
    catch {
        Write-Log "UE导入异常: $_" "ERROR" "IMPORT"
        return $false
    }
}

# ============================================
# 报告生成
# ============================================
function New-DeploymentReport {
    param(
        [array]$GeneratedFiles,
        [datetime]$StartTime,
        [string]$Status = "Success"
    )
    
    $EndTime = Get-Date
    $Duration = $EndTime - $StartTime
    
    $Report = @{
        Timestamp = $EndTime.ToString("yyyy-MM-dd HH:mm:ss")
        Duration = "{0:mm\:ss}" -f $Duration
        AssetType = $AssetType
        Faction = $Faction
        Status = $Status
        GeneratedFiles = @{
            Count = $GeneratedFiles.Count
            Files = $GeneratedFiles | ForEach-Object { Split-Path $_ -Leaf }
        }
        LogLocation = "$LogDir\AutoDeploy_$(Get-Date -Format 'yyyyMMdd').log"
    }
    
    $ReportPath = "$LogDir\Report_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
    $Report | ConvertTo-Json -Depth 3 | Out-File -FilePath $ReportPath -Encoding UTF8
    
    Write-Log "报告已保存: $ReportPath" "INFO" "REPORT"
    
    # 显示摘要
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "    自动化部署完成摘要" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "状态: $Status" -ForegroundColor $(if ($Status -eq "Success") { "Green" } else { "Red" })
    Write-Host "耗时: $($Report.Duration)" -ForegroundColor Gray
    Write-Host "生成文件数: $($GeneratedFiles.Count)" -ForegroundColor Gray
    Write-Host "日志位置: $($Report.LogLocation)" -ForegroundColor Gray
    Write-Host "========================================`n" -ForegroundColor Cyan
}

# ============================================
# 主执行流程
# ============================================
function Main {
    $StartTime = Get-Date
    
    Write-Host "`n========================================" -ForegroundColor Magenta
    Write-Host "    MingGoRTS 自动化素材生成与部署" -ForegroundColor Magenta
    Write-Host "========================================`n" -ForegroundColor Magenta
    
    Write-Log "配置: AssetType=$AssetType, Faction=$Faction, AutoImport=$AutoImport" "INFO"
    
    try {
        # 阶段1: 生成素材
        $GeneratedFiles = @()
        if (-not $SkipGeneration) {
            $GeneratedFiles = Invoke-AssetGeneration -Type $AssetType
        } else {
            Write-Log "跳过生成阶段，仅执行部署" "WARN"
            # 查找现有文件
            $ExistingDirs = @(
                "$ContentDir\Audio\Generated",
                "$ContentDir\Art\Generated"
            )
            foreach ($Dir in $ExistingDirs) {
                if (Test-Path $Dir) {
                    $Files = Get-ChildItem -Path $Dir -File -Recurse | Select-Object -ExpandProperty FullName
                    $GeneratedFiles += $Files
                }
            }
            Write-Log "找到 $($GeneratedFiles.Count) 个现有文件" "INFO"
        }
        
        # 阶段2: 部署素材
        if ($GeneratedFiles.Count -gt 0) {
            Invoke-AssetDeployment -FilesToDeploy $GeneratedFiles
        }
        
        # 阶段3: UE自动导入（可选）
        if ($AutoImport -and $GeneratedFiles.Count -gt 0) {
            Invoke-UEAutoImport
        }
        
        # 生成报告
        New-DeploymentReport -GeneratedFiles $GeneratedFiles -StartTime $StartTime -Status "Success"
        
        Write-Host "`n[OK] 自动化流程完成!" -ForegroundColor Green
        exit 0
    }
    catch {
        Write-Log "执行异常: $_" "ERROR"
        New-DeploymentReport -GeneratedFiles @() -StartTime $StartTime -Status "Failed"
        exit 1
    }
}

# 执行主函数
Main 
