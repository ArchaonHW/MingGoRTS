# Visual Studio 2022 銝???鋆??Write-Host "=== Visual Studio 2022 Community 摰? ===" -ForegroundColor Green

# ?萄遣銝??桅?
$downloadPath = "C:\VS2022_Download"
if (!(Test-Path $downloadPath)) {
    New-Item -ItemType Directory -Path $downloadPath -Force
    Write-Host "???萄遣銝??桅?: $downloadPath" -ForegroundColor Green
}

Set-Location $downloadPath

# 銝? Visual Studio 2022 Community
Write-Host "`n甇?銝? Visual Studio 2022 Community..." -ForegroundColor Yellow
$downloadUrl = "https://aka.ms/vs/17/release/vs_community.exe"
$installerPath = Join-Path $downloadPath "vs_community.exe"

try {
    Invoke-WebRequest -Uri $downloadUrl -OutFile $installerPath -UseBasicParsing
    Write-Host "??銝?摰?: $installerPath" -ForegroundColor Green
} catch {
    Write-Host "??銝?憭望?: $_" -ForegroundColor Red
    Write-Host "隢???頛? $downloadUrl" -ForegroundColor Yellow
    exit 1
}

# 摰??賊?
Write-Host "`n隢??鋆??" -ForegroundColor Cyan
Write-Host "1. ?芸?摰??刻蝯辣 (?刻)" -ForegroundColor White
Write-Host "2. ???芸?蝢拙?鋆? -ForegroundColor White
Write-Host "3. ??頛?摰?" -ForegroundColor White

$choice = Read-Host "`n隢??(1-3)"

switch ($choice) {
    "1" {
        Write-Host "`n甇??芸?摰??刻蝯辣..." -ForegroundColor Yellow
        $arguments = @(
            "--quiet",
            "--wait", 
            "--add", "Microsoft.VisualStudio.Workload.Game",
            "--add", "Microsoft.VisualStudio.Workload.ManagedDesktop",
            "--add", "Microsoft.VisualStudio.Workload.NativeDesktop",
            "--includeRecommended"
        )
        
        Start-Process -FilePath $installerPath -ArgumentList $arguments -Wait
        Write-Host "???芸?摰?摰?" -ForegroundColor Green
    }
    "2" {
        Write-Host "`n???芸?蝢拙?鋆?.." -ForegroundColor Yellow
        Start-Process -FilePath $installerPath -Wait
        Write-Host "???芸?蝢拙?鋆??? -ForegroundColor Green
    }
    "3" {
        Write-Host "`n??頛????典隞交???銵?$installerPath ?脰?摰?" -ForegroundColor Yellow
    }
    default {
        Write-Host "`n?⊥??豢?嚗??摰儔摰?..." -ForegroundColor Yellow
        Start-Process -FilePath $installerPath -Wait
    }
}

Write-Host "`n=== 摰???摰? ===" -ForegroundColor Green
Write-Host "摰?摰?敺????餉" -ForegroundColor Cyan

# 撽?摰?
Write-Host "`n?臬閬?霅?鋆? (Y/N)" -ForegroundColor Yellow
$verify = Read-Host

if ($verify -eq "Y" -or $verify -eq "y") {
    $vsPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
    if (Test-Path $vsPath) {
        Write-Host "??Visual Studio 2022 Community 撌脫迤蝣箏?鋆? -ForegroundColor Green
        $versionInfo = (Get-Item $vsPath).VersionInfo
        Write-Host "?: $($versionInfo.FileVersion)" -ForegroundColor Gray
        Write-Host "摰?頝臬?: $vsPath" -ForegroundColor Gray
    } else {
        Write-Host "??Visual Studio 2022 Community ?芣?? -ForegroundColor Red
        Write-Host "隢Ⅱ隤?鋆?血??? -ForegroundColor Yellow
    }
}
