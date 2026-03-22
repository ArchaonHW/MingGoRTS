# MingGoRTS UE素材自動化部署系統
# 自動化部署和管理Unreal Engine素材資源

param(
    [string]$SourcePath = "",
    [string]$AssetType = "All",
    [string]$TargetCategory = "",
    [switch]$AutoImport = $false,
    [switch]$GenerateMaterials = $false,
    [switch]$BatchMode = $false,
    [string]$UEVersion = "5.7"
)

# 配置路徑
$ProjectRoot = "C:\HW\MingGoRTS"
$ContentDir = "$ProjectRoot\Content"
$SourceDir = "$ProjectRoot\SourceAssets"
$LogDir = "$ProjectRoot\Logs\AssetDeployment"
$ConfigDir = "$ProjectRoot\Config"
$UEEditorPath = "C:\Program Files\Epic Games\UE_$UEVersion\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"

# 素材類型定義
$AssetTypes = @{
    "Textures" = @("*.png", "*.jpg", "*.jpeg", "*.tga", "*.bmp", "*.psd")
    "Models" = @("*.fbx", "*.obj", "*.3ds", "*.dae", "*.stl")
    "Audio" = @("*.wav", "*.mp3", "*.ogg", "*.flac")
    "Materials" = @("*.uasset")
    "Blueprints" = @("*.uasset")
    "Animations" = @("*.fbx", "*.anim")
    "UI" = @("*.png", "*.jpg", "*.svg", "*.uasset")
    "VFX" = @("*.uasset", "*.niagara")
}

# 目標分類目錄
$CategoryPaths = @{
    "Art" = "$ContentDir\Art"
    "Audio" = "$ContentDir\Audio"
    "Blueprints" = "$ContentDir\Blueprints"
    "Characters" = "$ContentDir\Characters"
    "Environment" = "$ContentDir\Environment"
    "UI" = "$ContentDir\UI"
    "VFX" = "$ContentDir\VFX"
    "Maps" = "$ContentDir\Maps"
}

# 確保日誌目錄存在
if (!(Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
}

# 日誌函數
function Write-AssetLog {
    param(
        [string]$Message,
        [string]$Level = "INFO",
        [string]$AssetName = ""
    )
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $AssetPrefix = if ($AssetName) { "[$AssetName] " } else { "" }
    $LogMessage = "[$Timestamp] [$Level] $AssetPrefix$Message"
    
    $Color = switch($Level) {
        "ERROR" { "Red" }
        "WARN"  { "Yellow" }
        "INFO"  { "Green" }
        "DEBUG" { "Gray" }
        "SUCCESS" { "Cyan" }
        default { "White" }
    }
    
    Write-Host $LogMessage -ForegroundColor $Color
    Add-Content "$LogDir\AssetDeployment_$(Get-Date -Format 'yyyyMMdd').log" $LogMessage
}

# 初始化部署環境
function Initialize-DeploymentEnvironment {
    Write-AssetLog "初始化UE素材部署環境..." "INFO"
    
    # 檢查UE編輯器
    if (!(Test-Path $UEEditorPath)) {
        Write-AssetLog "UE編輯器未找到: $UEEditorPath" "ERROR"
        return $false
    }
    Write-AssetLog "✓ UE編輯器已找到" "INFO"
    
    # 創建素材目錄
    foreach ($Category in $CategoryPaths.Values) {
        if (!(Test-Path $Category)) {
            New-Item -ItemType Directory -Path $Category -Force | Out-Null
            Write-AssetLog "創建目錄: $Category" "DEBUG"
        }
    }
    
    # 創建源素材目錄
    if (!(Test-Path $SourceDir)) {
        New-Item -ItemType Directory -Path $SourceDir -Force | Out-Null
    }
    
    Write-AssetLog "✓ 部署環境初始化完成" "SUCCESS"
    return $true
}

# 掃描素材文件
function Scan-AssetFiles {
    param([string]$ScanPath, [string]$TypeFilter = "All")
    
    Write-AssetLog "掃描素材文件: $ScanPath" "INFO"
    
    if (!(Test-Path $ScanPath)) {
        Write-AssetLog "掃描路徑不存在: $ScanPath" "ERROR"
        return @()
    }
    
    $Assets = @()
    
    if ($TypeFilter -eq "All") {
        $Extensions = $AssetTypes.Values | ForEach-Object { $_ } | Select-Object -Unique
    } else {
        $Extensions = $AssetTypes[$TypeFilter]
    }
    
    foreach ($Ext in $Extensions) {
        $Files = Get-ChildItem -Path $ScanPath -Filter $Ext -Recurse -File
        foreach ($File in $Files) {
            $AssetInfo = @{
                Name = $File.BaseName
                FullPath = $File.FullName
                Extension = $File.Extension
                Size = $File.Length
                Modified = $File.LastWriteTime
                Category = Get-AssetCategory -FileName $File.Name -Extension $File.Extension
            }
            $Assets += $AssetInfo
        }
    }
    
    Write-AssetLog "找到 $($Assets.Count) 個素材文件" "INFO"
    return $Assets
}

# 判斷素材分類
function Get-AssetCategory {
    param([string]$FileName, [string]$Extension)
    
    $LowerName = $FileName.ToLower()
    $LowerExt = $Extension.ToLower()
    
    # 根據文件名關鍵詞判斷
    if ($LowerName -match "character|npc|player|hero|unit") { return "Characters" }
    if ($LowerName -match "ui|hud|menu|button|icon|panel") { return "UI" }
    if ($LowerName -match "sound|music|audio|sfx|bgm|voice") { return "Audio" }
    if ($LowerName -match "effect|particle|vfx|niagara|smoke|fire") { return "VFX" }
    if ($LowerName -match "building|architecture|structure|house") { return "Environment" }
    if ($LowerName -match "terrain|landscape|ground|tree|rock|foliage") { return "Environment" }
    if ($LowerName -match "weapon|item|prop|equipment|armor") { return "Art" }
    if ($LowerName -match "animation|anim|motion") { return "Art" }
    if ($LowerName -match "bp_|blueprint") { return "Blueprints" }
    if ($LowerName -match "map|level|world") { return "Maps" }
    
    # 根據擴展名判斷
    switch ($LowerExt) {
        ".wav" { return "Audio" }
        ".mp3" { return "Audio" }
        ".ogg" { return "Audio" }
        ".fbx" { return "Art" }
        ".obj" { return "Art" }
        ".png" { return "UI" }
        ".jpg" { return "Art" }
        ".uasset" { return "Blueprints" }
        default { return "Art" }
    }
}

# 轉換圖片格式為UE友好格式
function Convert-ToUETexture {
    param([string]$SourceFile, [string]$OutputPath)
    
    Write-AssetLog "轉換紋理: $(Split-Path $SourceFile -Leaf)" "DEBUG"
    
    try {
        # 檢查是否為2的冪次方尺寸
        Add-Type -AssemblyName System.Drawing
        $Image = [System.Drawing.Image]::FromFile($SourceFile)
        $Width = $Image.Width
        $Height = $Image.Height
        $Image.Dispose()
        
        # 檢查尺寸是否為2的冪次方
        $IsPowerOfTwo = {
            param([int]$n)
            return ($n -band ($n - 1)) -eq 0
        }
        
        $WidthOK = & $IsPowerOfTwo $Width
        $HeightOK = & $IsPowerOfTwo $Height
        
        if (-not ($WidthOK -and $HeightOK)) {
            Write-AssetLog "警告: 紋理尺寸非2的冪次方 (${Width}x${Height}) - $(Split-Path $SourceFile -Leaf)" "WARN"
        }
        
        # 複製到目標位置（UE會自動導入）
        Copy-Item $SourceFile $OutputPath -Force
        return $true
    }
    catch {
        Write-AssetLog "轉換失敗: $_" "ERROR"
        return $false
    }
}

# 準備FBX模型導入
function Prepare-FBXImport {
    param([string]$SourceFile, [string]$OutputPath)
    
    Write-AssetLog "準備FBX導入: $(Split-Path $SourceFile -Leaf)" "DEBUG"
    
    try {
        # 檢查FBX文件
        $Content = Get-Content $SourceFile -Raw -ErrorAction Stop
        
        # 基本驗證
        if ($Content -match "FBX") {
            Copy-Item $SourceFile $OutputPath -Force
            Write-AssetLog "✓ FBX文件已準備: $(Split-Path $SourceFile -Leaf)" "SUCCESS"
            return $true
        } else {
            Write-AssetLog "無效的FBX文件: $(Split-Path $SourceFile -Leaf)" "ERROR"
            return $false
        }
    }
    catch {
        Write-AssetLog "準備FBX失敗: $_" "ERROR"
        return $false
    }
}

# 生成材質實例
function Generate-MaterialInstance {
    param([string]$TexturePath, [string]$MaterialType = "Default")
    
    Write-AssetLog "生成材質實例: $(Split-Path $TexturePath -Leaf)" "DEBUG"
    
    $TextureName = [System.IO.Path]::GetFileNameWithoutExtension($TexturePath)
    $MaterialName = "M_${TextureName}"
    $MaterialDir = [System.IO.Path]::GetDirectoryName($TexturePath)
    $MaterialPath = Join-Path $MaterialDir "$MaterialName.uasset"
    
    # 材質實例內容（簡化版本）
    $MaterialContent = @"
{
    "MaterialName": "$MaterialName",
    "ParentMaterial": "/Game/Materials/${MaterialType}_Master",
    "TextureParameters": {
        "BaseColor": "$TexturePath"
    },
    "ScalarParameters": {
        "Roughness": 0.5,
        "Metallic": 0.0
    }
}
"@
    
    # 保存材質配置（實際導入需要UE編輯器）
    $ConfigPath = "$MaterialPath.json"
    $MaterialContent | Out-File -FilePath $ConfigPath -Encoding UTF8
    
    Write-AssetLog "材質配置已生成: $MaterialName" "INFO"
    return $MaterialPath
}

# 執行UE自動導入
function Invoke-UEAutoImport {
    param([string]$ImportPath, [string]$DestinationPath)
    
    Write-AssetLog "執行UE自動導入..." "INFO"
    
    $UProjectPath = "$ProjectRoot\MingGoRTS.uproject"
    
    if (!(Test-Path $UProjectPath)) {
        Write-AssetLog "項目文件未找到: $UProjectPath" "ERROR"
        return $false
    }
    
    # 構建導入命令
    $ImportArgs = @(
        "$UProjectPath"
        "-run=ImportAssets"
        "-importpath=`"$ImportPath`""
        "-destpath=`"$DestinationPath`""
        "-importsettings=`"ImportAll`""
        "-unattended"
        "-nosplash"
        "-stdout"
    )
    
    try {
        $Process = Start-Process -FilePath $UEEditorPath -ArgumentList $ImportArgs -Wait -PassThru -WindowStyle Hidden
        
        if ($Process.ExitCode -eq 0) {
            Write-AssetLog "✓ UE導入成功" "SUCCESS"
            return $true
        } else {
            Write-AssetLog "UE導入失敗，退出碼: $($Process.ExitCode)" "ERROR"
            return $false
        }
    }
    catch {
        Write-AssetLog "UE導入異常: $_" "ERROR"
        return $false
    }
}

# 部署單個素材
function Deploy-SingleAsset {
    param([hashtable]$AssetInfo, [string]$TargetCategoryOverride = "")
    
    $AssetName = $AssetInfo.Name
    $SourcePath = $AssetInfo.FullPath
    $Category = if ($TargetCategoryOverride) { $TargetCategoryOverride } else { $AssetInfo.Category }
    
    Write-AssetLog "開始部署素材: $AssetName" "INFO" $AssetName
    
    # 確保目標目錄存在
    $TargetDir = $CategoryPaths[$Category]
    if (!(Test-Path $TargetDir)) {
        New-Item -ItemType Directory -Path $TargetDir -Force | Out-Null
    }
    
    $TargetPath = Join-Path $TargetDir ($AssetName + $AssetInfo.Extension)
    
    # 根據類型處理
    $Success = $false
    switch ($AssetInfo.Extension.ToLower()) {
        { $_ -in @(".png", ".jpg", ".jpeg", ".tga") } {
            $Success = Convert-ToUETexture -SourceFile $SourcePath -OutputPath $TargetPath
            
            if ($Success -and $GenerateMaterials) {
                Generate-MaterialInstance -TexturePath $TargetPath
            }
        }
        { $_ -in @(".fbx", ".obj") } {
            $Success = Prepare-FBXImport -SourceFile $SourcePath -OutputPath $TargetPath
        }
        { $_ -in @(".wav", ".mp3", ".ogg") } {
            Copy-Item $SourcePath $TargetPath -Force
            $Success = $true
        }
        default {
            Copy-Item $SourcePath $TargetPath -Force
            $Success = $true
        }
    }
    
    if ($Success) {
        Write-AssetLog "✓ 素材部署成功: $AssetName -> $Category" "SUCCESS" $AssetName
    } else {
        Write-AssetLog "✗ 素材部署失敗: $AssetName" "ERROR" $AssetName
    }
    
    return $Success
}

# 批量部署素材
function Deploy-AssetsBatch {
    param([string]$SourcePath, [string]$AssetType = "All", [string]$Category = "")
    
    Write-AssetLog "開始批量部署素材..." "INFO"
    Write-AssetLog "源路徑: $SourcePath" "DEBUG"
    Write-AssetLog "素材類型: $AssetType" "DEBUG"
    
    # 掃描素材
    $Assets = Scan-AssetFiles -ScanPath $SourcePath -TypeFilter $AssetType
    
    if ($Assets.Count -eq 0) {
        Write-AssetLog "未找到素材文件" "WARN"
        return
    }
    
    $SuccessCount = 0
    $FailCount = 0
    
    foreach ($Asset in $Assets) {
        if (Deploy-SingleAsset -AssetInfo $Asset -TargetCategoryOverride $Category) {
            $SuccessCount++
        } else {
            $FailCount++
        }
    }
    
    Write-AssetLog "批量部署完成: 成功 $SuccessCount, 失敗 $FailCount" "INFO"
    
    # 如果需要自動導入到UE
    if ($AutoImport) {
        Write-AssetLog "啟動UE自動導入..." "INFO"
        foreach ($Cat in $CategoryPaths.Keys) {
            Invoke-UEAutoImport -ImportPath $CategoryPaths[$Cat] -DestinationPath "/Game/$Cat"
        }
    }
}

# 生成素材清單報告
function Generate-AssetReport {
    param([string]$OutputPath = "$LogDir\AssetReport_$(Get-Date -Format 'yyyyMMdd_HHmmss').json")
    
    Write-AssetLog "生成素材清單報告..." "INFO"
    
    $Report = @{
        GeneratedAt = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
        ProjectRoot = $ProjectRoot
        Categories = @{}
        TotalAssets = 0
        TotalSize = 0
    }
    
    foreach ($Category in $CategoryPaths.Keys) {
        $Path = $CategoryPaths[$Category]
        if (Test-Path $Path) {
            $Files = Get-ChildItem -Path $Path -File -Recurse
            $CategorySize = ($Files | Measure-Object -Property Length -Sum).Sum
            
            $Report.Categories[$Category] = @{
                Count = $Files.Count
                Size = $CategorySize
                SizeHuman = Format-Size -Size $CategorySize
                Assets = $Files | Select-Object Name, Extension, Length, LastWriteTime
            }
            
            $Report.TotalAssets += $Files.Count
            $Report.TotalSize += $CategorySize
        }
    }
    
    $Report.TotalSizeHuman = Format-Size -Size $Report.TotalSize
    
    # 保存報告
    $Report | ConvertTo-Json -Depth 3 | Out-File -FilePath $OutputPath -Encoding UTF8
    Write-AssetLog "報告已保存: $OutputPath" "SUCCESS"
    
    return $Report
}

# 格式化文件大小
function Format-Size {
    param([long]$Size)
    
    $Units = @("B", "KB", "MB", "GB", "TB")
    $UnitIndex = 0
    $Value = [double]$Size
    
    while ($Value -ge 1024 -and $UnitIndex -lt $Units.Count - 1) {
        $Value /= 1024
        $UnitIndex++
    }
    
    return "{0:N2} {1}" -f $Value, $Units[$UnitIndex]
}

# 清理舊素材
function Clear-OldAssets {
    param([int]$DaysOld = 30)
    
    Write-AssetLog "清理超過 $DaysOld 天的舊素材..." "INFO"
    
    $CutoffDate = (Get-Date).AddDays(-$DaysOld)
    $RemovedCount = 0
    
    foreach ($Category in $CategoryPaths.Values) {
        if (Test-Path $Category) {
            $OldFiles = Get-ChildItem -Path $Category -File -Recurse | 
                Where-Object { $_.LastWriteTime -lt $CutoffDate -and $_.Name -notlike "*_Master*" }
            
            foreach ($File in $OldFiles) {
                Remove-Item $File.FullName -Force
                Write-AssetLog "已刪除: $($File.FullName)" "DEBUG"
                $RemovedCount++
            }
        }
    }
    
    Write-AssetLog "清理完成: 刪除 $RemovedCount 個舊文件" "INFO"
}

# 主執行流程
function Main {
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "    MingGoRTS UE素材自動化部署系統" -ForegroundColor Cyan
    Write-Host "========================================`n" -ForegroundColor Cyan
    
    # 初始化環境
    if (!(Initialize-DeploymentEnvironment)) {
        Write-AssetLog "環境初始化失敗，退出" "ERROR"
        exit 1
    }
    
    # 處理命令行參數
    if ($BatchMode) {
        Write-AssetLog "批量模式: 處理源目錄所有素材" "INFO"
        
        $SourceToProcess = if ($SourcePath) { $SourcePath } else { $SourceDir }
        Deploy-AssetsBatch -SourcePath $SourceToProcess -AssetType $AssetType -Category $TargetCategory
        
        # 生成報告
        Generate-AssetReport
        
    } elseif ($SourcePath -and (Test-Path $SourcePath)) {
        # 單個文件或目錄處理
        $Item = Get-Item $SourcePath
        
        if ($Item.PSIsContainer) {
            Deploy-AssetsBatch -SourcePath $SourcePath -AssetType $AssetType -Category $TargetCategory
        } else {
            $AssetInfo = @{
                Name = $Item.BaseName
                FullPath = $Item.FullName
                Extension = $Item.Extension
                Size = $Item.Length
                Modified = $Item.LastWriteTime
                Category = Get-AssetCategory -FileName $Item.Name -Extension $Item.Extension
            }
            Deploy-SingleAsset -AssetInfo $AssetInfo -TargetCategoryOverride $TargetCategory
        }
        
        # 生成報告
        Generate-AssetReport
        
    } else {
        Write-AssetLog "請指定有效的素材路徑或使用 -BatchMode 參數" "WARN"
        Write-AssetLog "使用示例:" "INFO"
        Write-AssetLog "  .\DeployUEAssets.ps1 -SourcePath 'C:\Assets\texture.png' -TargetCategory 'UI'" "INFO"
        Write-AssetLog "  .\DeployUEAssets.ps1 -BatchMode -SourcePath 'C:\Assets' -AutoImport" "INFO"
        
        # 顯示當前素材統計
        Write-Host "`n當前項目素材統計:" -ForegroundColor Yellow
        $Report = Generate-AssetReport -OutputPath "$LogDir\CurrentAssets_Report.json"
        
        foreach ($Cat in $Report.Categories.Keys) {
            $Info = $Report.Categories[$Cat]
            Write-Host "  $Cat`: $($Info.Count) 個文件, $($Info.SizeHuman)" -ForegroundColor Gray
        }
        Write-Host "`n總計: $($Report.TotalAssets) 個文件, $($Report.TotalSizeHuman)" -ForegroundColor Green
    }
    
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "    素材部署流程完成" -ForegroundColor Cyan
    Write-Host "========================================`n" -ForegroundColor Cyan
}

# 執行主函數
Main
