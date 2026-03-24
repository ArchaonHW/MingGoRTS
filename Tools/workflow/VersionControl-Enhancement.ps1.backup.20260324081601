#!/usr/bin/env pwsh

<#
.SYNOPSIS
    Version Control Enhancement - Version & Changelog Management
    版控增強 - 版本號與變更日誌管理

.DESCRIPTION
    提供版本號自動遞增和 CHANGELOG.md 自動生成功能
    支持根據 commit 類型自動 bump 版本號 (Semantic Versioning)

.PARAMETER BumpType
    版本遞增類型: major|minor|patch|auto
    
.PARAMETER GenerateChangelog
    是否生成 CHANGELOG.md

.EXAMPLE
    .\VersionControl-Enhancement.ps1 -BumpType auto -GenerateChangelog
#>

[CmdletBinding()]
param(
    [Parameter()]
    [ValidateSet("major", "minor", "patch", "auto")]
    [string]$BumpType = "auto",
    
    [Parameter()]
    [switch]$GenerateChangelog,
    
    [Parameter()]
    [string]$ProjectRoot = "$PWD",
    
    [Parameter()]
    [string]$ChangelogPath = "$PWD\CHANGELOG.md"
)

$ErrorActionPreference = "Stop"

# ============================================
# 版本管理功能
# ============================================

function Get-CurrentVersion {
    param([string]$ProjectPath)
    
    $VersionSources = @(
        @{ Path = "$ProjectPath\version.txt"; Type = "Plain" },
        @{ Path = "$ProjectPath\package.json"; Type = "JSON"; Field = "version" },
        @{ Path = "$ProjectPath\.github\version.json"; Type = "JSON"; Field = "version" },
        @{ Path = "$ProjectPath\MingGoRTS.uproject"; Type = "UE5"; Pattern = '"Version":\s*(\d+)' }
    )
    
    foreach ($Source in $VersionSources) {
        if (Test-Path $Source.Path) {
            $Content = Get-Content -Path $Source.Path -Raw -ErrorAction SilentlyContinue
            
            switch ($Source.Type) {
                "Plain" { 
                    if ($Content -match '(\d+\.\d+\.\d+)') {
                        return $matches[1]
                    }
                }
                "JSON" {
                    try {
                        $Json = $Content | ConvertFrom-Json
                        return $Json.($Source.Field)
                    } catch {}
                }
                "UE5" {
                    if ($Content -match $Source.Pattern) {
                        $VersionNum = [int]$matches[1]
                        $Major = [math]::Floor($VersionNum / 10000)
                        $Minor = [math]::Floor(($VersionNum % 10000) / 100)
                        $Patch = $VersionNum % 100
                        return "$Major.$Minor.$Patch"
                    }
                }
            }
        }
    }
    
    # 默認版本
    return "0.1.0"
}

function Invoke-VersionBump {
    param(
        [string]$CurrentVersion,
        [string]$BumpType,
        [array]$Commits = @()
    )
    
    $Parts = $CurrentVersion -split '\.'
    if ($Parts.Count -ne 3) {
        $Parts = @(0, 1, 0)
    }
    
    [int]$Major = $Parts[0]
    [int]$Minor = $Parts[1]
    [int]$Patch = $Parts[2]
    
    # 自動檢測 bump 類型
    if ($BumpType -eq "auto" -and $Commits.Count -gt 0) {
        $HasBreaking = $Commits | Where-Object { $_ -match "BREAKING CHANGE|^[a-z]+\(!\)" }
        $HasFeature = $Commits | Where-Object { $_ -match "^feat[(:]" }
        
        if ($HasBreaking) {
            $BumpType = "major"
        }
        elseif ($HasFeature) {
            $BumpType = "minor"
        }
        else {
            $BumpType = "patch"
        }
    }
    
    switch ($BumpType) {
        "major" {
            $Major++
            $Minor = 0
            $Patch = 0
        }
        "minor" {
            $Minor++
            $Patch = 0
        }
        "patch" {
            $Patch++
        }
    }
    
    return @{
        OldVersion = $CurrentVersion
        NewVersion = "$Major.$Minor.$Patch"
        BumpType = $BumpType
    }
}

function Save-Version {
    param(
        [string]$ProjectPath,
        [string]$Version
    )
    
    # 保存到 version.txt
    $VersionFile = Join-Path $ProjectPath "version.txt"
    $Version | Set-Content -Path $VersionFile
    
    # 保存到 .github/version.json
    $GitHubVersionFile = Join-Path $ProjectPath ".github\version.json"
    if (-not (Test-Path (Split-Path -Parent $GitHubVersionFile))) {
        New-Item -ItemType Directory -Path (Split-Path -Parent $GitHubVersionFile) -Force | Out-Null
    }
    
    @{ version = $Version; updatedAt = (Get-Date -Format "yyyy-MM-dd HH:mm:ss") } | 
        ConvertTo-Json | Set-Content -Path $GitHubVersionFile
}

# ============================================
# CHANGELOG 生成功能
# ============================================

function Get-CommitHistory {
    param(
        [string]$ProjectPath,
        [string]$SinceVersion = $null
    )
    
    $Format = '%H|%s|%b|%an|%ad'
    
    if ($SinceVersion) {
        $LogOutput = & git -C $ProjectPath log "${SinceVersion}..HEAD" --pretty=format:$Format --date=short 2>$null
    } else {
        $LogOutput = & git -C $ProjectPath log --pretty=format:$Format --date=short -50 2>$null
    }
    
    $Commits = @()
    foreach ($Line in $LogOutput -split "`n") {
        $Parts = $Line -split '\|'
        if ($Parts.Count -ge 5) {
            $Commits += @{
                Hash = $Parts[0].Substring(0, 7)
                Subject = $Parts[1]
                Body = $Parts[2]
                Author = $Parts[3]
                Date = $Parts[4]
                Type = Invoke-ParseCommitType -Message $Parts[1]
                Scope = Invoke-ParseCommitScope -Message $Parts[1]
            }
        }
    }
    
    return $Commits
}

function Invoke-ParseCommitType {
    param([string]$Message)
    
    $Types = @("feat", "fix", "docs", "style", "refactor", "test", "chore", "perf", "ci", "build", "revert")
    
    foreach ($Type in $Types) {
        if ($Message -match "^$Type[(:\[]") {
            return $Type
        }
    }
    
    return "other"
}

function Invoke-ParseCommitScope {
    param([string]$Message)
    
    if ($Message -match "^\w+\(([^)]+)\)") {
        return $matches[1]
    }
    
    return ""
}

function New-Changelog {
    param(
        [string]$ProjectPath,
        [string]$ChangelogFile,
        [string]$Version,
        [array]$Commits
    )
    
    $Date = Get-Date -Format "yyyy-MM-dd"
    
    $Changelog = @"
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [$Version] - $Date

"@
    
    # 分類提交
    $Categories = @{
        "Added" = @($Commits | Where-Object { $_.Type -eq "feat" })
        "Fixed" = @($Commits | Where-Object { $_.Type -eq "fix" })
        "Changed" = @($Commits | Where-Object { $_.Type -in @("refactor", "perf") })
        "Documentation" = @($Commits | Where-Object { $_.Type -eq "docs" })
        "Tests" = @($Commits | Where-Object { $_.Type -eq "test" })
        "Build/CI" = @($Commits | Where-Object { $_.Type -in @("build", "ci") })
        "Other" = @($Commits | Where-Object { $_.Type -in @("chore", "style", "revert", "other") })
    }
    
    foreach ($Category in $Categories.GetEnumerator()) {
        if ($Category.Value.Count -gt 0) {
            $Changelog += "### $($Category.Key)`n`n"
            foreach ($Commit in $Category.Value) {
                $Scope = if ($Commit.Scope) { "**[$($Commit.Scope)]** " } else { "" }
                $Changelog += "- $Scope$($Commit.Subject) ([$($Commit.Hash)](https://github.com/ArchaonHW/MingGoRTS/commit/$($Commit.Hash)))`n"
            }
            $Changelog += "`n"
        }
    }
    
    # 如果存在舊的 CHANGELOG，保留歷史版本
    if (Test-Path $ChangelogFile) {
        $OldContent = Get-Content -Path $ChangelogFile -Raw
        if ($OldContent -match "## \[\d+\.\d+\.\d+\]") {
            $OldVersions = $OldContent.Substring($OldContent.IndexOf("## ["))
            # 移除最舊的新版本（因為我們剛剛添加了它）
            $OldVersions = $OldVersions -replace "(?s)## \[$Version\].*?(?=## \[|$)", ""
            $Changelog += $OldVersions.Trim()
        }
    }
    
    $Changelog | Set-Content -Path $ChangelogFile -Encoding UTF8
    return $ChangelogFile
}

# ============================================
# Git Tag 管理
# ============================================

function New-GitTag {
    param(
        [string]$ProjectPath,
        [string]$Version,
        [string]$CommitHash = "HEAD"
    )
    
    $TagName = "v$Version"
    
    # 檢查標籤是否已存在
    $ExistingTag = & git -C $ProjectPath tag -l $TagName 2>$null
    if ($ExistingTag) {
        return @{ Success = $false; Message = "Tag $TagName already exists" }
    }
    
    # 創建標籤
    $TagMessage = "Release $TagName`n`nGenerated automatically by Version Control System"
    & git -C $ProjectPath tag -a $TagName -m $TagMessage $CommitHash 2>$null
    
    if ($LASTEXITCODE -eq 0) {
        return @{ Success = $true; Message = "Created tag $TagName" }
    } else {
        return @{ Success = $false; Message = "Failed to create tag" }
    }
}

function Push-ToRemote {
    param(
        [string]$ProjectPath,
        [string]$Branch = "main",
        [switch]$PushTags
    )
    
    # 檢查遠程倉庫
    $RemoteUrl = & git -C $ProjectPath remote get-url origin 2>$null
    if ($LASTEXITCODE -ne 0 -or -not $RemoteUrl) {
        return @{ Success = $false; Message = "No remote repository configured" }
    }
    
    Write-Host "Pushing to: $RemoteUrl" -ForegroundColor Cyan
    
    # 推送提交
    & git -C $ProjectPath push origin $Branch 2>$null
    if ($LASTEXITCODE -ne 0) {
        return @{ Success = $false; Message = "Failed to push to remote" }
    }
    
    # 推送標籤
    if ($PushTags) {
        & git -C $ProjectPath push origin --tags 2>$null
        if ($LASTEXITCODE -ne 0) {
            return @{ Success = $true; Message = "Pushed commits but failed to push tags" }
        }
    }
    
    return @{ Success = $true; Message = "Successfully pushed to remote" }
}

# ============================================
# 主執行流程
# ============================================

function Main {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Version Control Enhancement System" -ForegroundColor Cyan
    Write-Host "版控增強系統 - 版本號與變更日誌管理" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    
    # 檢測 Git 倉庫
    $GitPath = Join-Path $ProjectRoot ".git"
    if (-not (Test-Path $GitPath)) {
        Write-Error "Not a git repository. Please run this from a git project."
        return
    }
    
    # 獲取當前版本
    $CurrentVersion = Get-CurrentVersion -ProjectPath $ProjectRoot
    Write-Host "`nCurrent Version: $CurrentVersion" -ForegroundColor Green
    
    # 獲取提交歷史
    $Commits = Get-CommitHistory -ProjectPath $ProjectRoot -SinceVersion "v$CurrentVersion"
    Write-Host "Commits since last version: $($Commits.Count)" -ForegroundColor Gray
    
    if ($Commits.Count -eq 0) {
        Write-Host "`nNo new commits found. Exiting." -ForegroundColor Yellow
        return
    }
    
    # 計算新版本
    $VersionInfo = Invoke-VersionBump -CurrentVersion $CurrentVersion -BumpType $BumpType -Commits ($Commits | ForEach-Object { $_.Subject })
    Write-Host "`nVersion Bump: $($VersionInfo.OldVersion) -> $($VersionInfo.NewVersion) ($($VersionInfo.BumpType))" -ForegroundColor Green
    
    # 保存新版本
    Save-Version -ProjectPath $ProjectRoot -Version $VersionInfo.NewVersion
    Write-Host "✓ Version file updated" -ForegroundColor Green
    
    # 生成 CHANGELOG
    if ($GenerateChangelog) {
        $ChangelogFile = New-Changelog -ProjectPath $ProjectRoot -ChangelogFile $ChangelogPath -Version $VersionInfo.NewVersion -Commits $Commits
        Write-Host "✓ CHANGELOG generated: $ChangelogFile" -ForegroundColor Green
        
        # 添加 CHANGELOG 到 Git
        & git -C $ProjectRoot add $ChangelogPath
        & git -C $ProjectRoot add "version.txt"
        & git -C $ProjectRoot add ".github/version.json"
        
        $CommitMsg = "chore(release): bump version to $($VersionInfo.NewVersion) [skip ci]"
        & git -C $ProjectRoot commit -m $CommitMsg
        Write-Host "✓ Version bump committed" -ForegroundColor Green
    }
    
    # 創建 Git Tag
    $TagResult = New-GitTag -ProjectPath $ProjectRoot -Version $VersionInfo.NewVersion
    if ($TagResult.Success) {
        Write-Host "✓ $($TagResult.Message)" -ForegroundColor Green
    } else {
        Write-Host "⚠ $($TagResult.Message)" -ForegroundColor Yellow
    }
    
    # 推送到遠程
    $PushResult = Push-ToRemote -ProjectPath $ProjectRoot -PushTags:$GenerateChangelog
    if ($PushResult.Success) {
        Write-Host "✓ $($PushResult.Message)" -ForegroundColor Green
    } else {
        Write-Host "⚠ $($PushResult.Message)" -ForegroundColor Yellow
    }
    
    Write-Host "`n========================================" -ForegroundColor Cyan
    Write-Host "Version Control Enhancement Complete!" -ForegroundColor Green
    Write-Host "New Version: $($VersionInfo.NewVersion)" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Cyan
}

# 執行
Main
