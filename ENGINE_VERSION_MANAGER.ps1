# MingGoRTS 引擎版本管理器
# 專業級版本切換工具

param(
    [string]$TargetVersion,
    [switch]$ListVersions,
    [switch]$Backup,
    [switch]$Restore
)

$EngineRoot = "C:\Program Files\Epic Games"
$ProjectRoot = "C:\HW\MingGoRTS"
$BackupPath = "$ProjectRoot\EngineBackups"

function Write-Status {
    param($Message, $Status = "INFO")
    $Colors = @{
        "SUCCESS" = "Green"
        "ERROR" = "Red"
        "WARNING" = "Yellow"
        "INFO" = "Cyan"
    }
    Write-Host $Message -ForegroundColor $Colors[$Status]
}

function Get-InstalledVersions {
    Write-Status "🔍 檢查已安裝的 Unreal Engine 版本..." "INFO"
    
    $Versions = @()
    $EngineDirs = Get-ChildItem -Path $EngineRoot | Where-Object { $_.Name -match "UE_" }
    
    foreach ($Dir in $EngineDirs) {
        $VersionFile = "$($Dir.FullName)\Engine\Build\Build.version"
        if (Test-Path $VersionFile) {
            $VersionInfo = Get-Content $VersionFile | ConvertFrom-Json
            $Versions += [PSCustomObject]@{
                Name = $Dir.Name
                Path = $Dir.FullName
                Major = $VersionInfo.MajorVersion
                Minor = $VersionInfo.MinorVersion
                Patch = $VersionInfo.PatchVersion
                Changelist = $VersionInfo.Changelist
                IsPromoted = $VersionInfo.IsPromotedBuild
                InstallDate = $Dir.LastWriteTime
            }
        }
    }
    
    return $Versions
}

function Show-VersionList {
    $Versions = Get-InstalledVersions
    
    Write-Host "📊 已安裝的 Unreal Engine 版本:" "INFO"
    Write-Host "======================================" "INFO"
    
    foreach ($Version in $Versions) {
        $Status = if ($Version.IsPromoted) { "🟢" } else { "🟡" }
        Write-Host "$Status $($Version.Name) - $($Version.Major).$($Version.Minor).$($Version.Patch) (Changelist: $($Version.Changelist))" "INFO"
        Write-Host "   路徑: $($Version.Path)" "Gray"
        Write-Host "   安裝日期: $($Version.InstallDate)" "Gray"
        Write-Host ""
    }
    
    if ($Versions.Count -eq 0) {
        Write-Status "❌ 未找到已安裝的 Unreal Engine 版本" "ERROR"
    }
}

function Backup-Project {
    Write-Status "💾 備份專案配置..." "INFO"
    
    if (-not (Test-Path $BackupPath)) {
        New-Item -ItemType Directory -Path $BackupPath -Force | Out-Null
    }
    
    $BackupName = "Backup_$(Get-Date -Format 'yyyyMMdd_HHmmss')"
    $BackupDir = "$BackupPath\$BackupName"
    
    # 備份關鍵檔案
    $FilesToBackup = @(
        "$ProjectRoot\MingGoRTS.uproject",
        "$ProjectRoot\Source",
        "$ProjectRoot\Plugins"
    )
    
    New-Item -ItemType Directory -Path $BackupDir -Force | Out-Null
    
    foreach ($File in $FilesToBackup) {
        if (Test-Path $File) {
            $TargetPath = "$BackupDir\$(Split-Path $File -Leaf)"
            Copy-Item -Path $File -Destination $TargetPath -Recurse -Force
            Write-Status "  備份: $(Split-Path $File -Leaf)" "INFO"
        }
    }
    
    Write-Status "✅ 專案已備份到: $BackupName" "SUCCESS"
    return $BackupName
}

function Update-ProjectVersion {
    param([string]$NewVersion)
    
    Write-Status "🔄 更新專案到 Unreal Engine $NewVersion..." "INFO"
    
    # 更新 .uproject 檔案
    $ProjectFile = "$ProjectRoot\MingGoRTS.uproject"
    if (Test-Path $ProjectFile) {
        $ProjectData = Get-Content $ProjectFile | ConvertFrom-Json
        $ProjectData.EngineAssociation = $NewVersion
        $ProjectData | ConvertTo-Json -Depth 10 | Set-Content $ProjectFile
        Write-Status "  更新專案檔案: MingGoRTS.uproject" "INFO"
    }
    
    # 更新所有 .Target.cs 檔案
    $TargetFiles = Get-ChildItem -Path "$ProjectRoot\Source" -Recurse -Filter "*.Target.cs"
    foreach ($File in $TargetFiles) {
        $Content = Get-Content $File -Raw
        $Content = $Content -replace 'IncludeOrderVersion = EngineIncludeOrderVersion\.Unreal5_\d+', "IncludeOrderVersion = EngineIncludeOrderVersion.Unreal$($NewVersion.Replace('.', '_'))"
        $Content | Set-Content $File
        Write-Status "  更新目標檔案: $($File.Name)" "INFO"
    }
    
    Write-Status "✅ 專案版本更新完成" "SUCCESS"
}

function Test-NewVersion {
    param([string]$TargetVersion)
    
    Write-Status "🧪 測試新版本兼容性..." "INFO"
    
    $EnginePath = "$EngineRoot\UE_$TargetVersion"
    $UBTPath = "$EnginePath\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
    
    if (-not (Test-Path $UBTPath)) {
        Write-Status "❌ Unreal Engine $TargetVersion 未安裝" "ERROR"
        return $false
    }
    
    # 測試編譯
    $Process = Start-Process -FilePath $UBTPath -ArgumentList "-projectfiles", "-project=`"$ProjectRoot\MingGoRTS.uproject`"", "-game", "-progress" -Wait -PassThru
    
    if ($Process.ExitCode -eq 0) {
        Write-Status "✅ 版本兼容性測試通過" "SUCCESS"
        return $true
    } else {
        Write-Status "❌ 版本兼容性測試失敗" "ERROR"
        return $false
    }
}

function Restore-Project {
    Write-Status "🔄 恢復專案備份..." "INFO"
    
    if (-not (Test-Path $BackupPath)) {
        Write-Status "❌ 找不到備份目錄" "ERROR"
        return
    }
    
    $Backups = Get-ChildItem -Path $BackupPath | Sort-Object LastWriteTime -Descending
    
    if ($Backups.Count -eq 0) {
        Write-Status "❌ 找不到備份檔案" "ERROR"
        return
    }
    
    $LatestBackup = $Backups[0]
    Write-Status "  恢復備份: $($LatestBackup.Name)" "INFO"
    
    # 恢復檔案
    Copy-Item -Path "$($LatestBackup.FullName)\*" -Destination $ProjectRoot -Recurse -Force
    
    Write-Status "✅ 專案恢復完成" "SUCCESS"
}

# 主要執行流程
try {
    Write-Status "🚀 MingGoRTS 引擎版本管理器" "INFO"
    Write-Status "================================" "INFO"
    
    if ($ListVersions) {
        Show-VersionList
        return
    }
    
    if ($Backup) {
        Backup-Project
        return
    }
    
    if ($Restore) {
        Restore-Project
        return
    }
    
    if ($TargetVersion) {
        # 備份當前專案
        $BackupName = Backup-Project
        
        # 更新版本
        Update-ProjectVersion -NewVersion $TargetVersion
        
        # 測試兼容性
        if (Test-NewVersion -TargetVersion $TargetVersion) {
            Write-Status "🎉 版本切換成功！" "SUCCESS"
            Write-Status "💡 建議重新啟動 Unreal Editor" "INFO"
        } else {
            Write-Status "❌ 版本切換失敗，正在恢復..." "ERROR"
            Restore-Project
            Write-Status "🔄 已恢復到原始版本" "WARNING"
        }
    } else {
        Write-Status "❌ 請指定目標版本或使用 -ListVersions" "ERROR"
        Write-Status "用法示例:" "INFO"
        Write-Status "  .\ENGINE_VERSION_MANAGER.ps1 -ListVersions" "INFO"
        Write-Status "  .\ENGINE_VERSION_MANAGER.ps1 -TargetVersion '5.6'" "INFO"
        Write-Status "  .\ENGINE_VERSION_MANAGER.ps1 -Backup" "INFO"
        Write-Status "  .\ENGINE_VERSION_MANAGER.ps1 -Restore" "INFO"
    }
    
} catch {
    Write-Status "❌ 執行過程中發生錯誤: $($_.Exception.Message)" "ERROR"
    exit 1
}
