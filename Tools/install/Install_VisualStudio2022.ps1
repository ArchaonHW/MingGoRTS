# Visual Studio 2022 摰??單
# MingGoRTS ??啣?閮剔蔭

Write-Host "=== Visual Studio 2022 摰??? ===" -ForegroundColor Green

# 瑼Ｘ蝟餌絞?瘙?Write-Host "`n1. 瑼Ｘ蝟餌絞?瘙?.." -ForegroundColor Yellow

$systemInfo = Get-ComputerInfo
$osVersion = $systemInfo.WindowsProductName
$totalMemory = [math]::Round($systemInfo.TotalPhysicalMemory / 1GB, 1)

Write-Host "雿平蝟餌絞: $osVersion" -ForegroundColor White
Write-Host "閮擃? ${totalMemory}GB" -ForegroundColor White

# 瑼Ｘ?臬撌脣?鋆?Visual Studio
Write-Host "`n2. 瑼Ｘ?暹? Visual Studio 摰?..." -ForegroundColor Yellow

$vsInstances = Get-CimInstance -ClassName MSFT_VSInstance -Namespace root/cimv2/vs
if ($vsInstances) {
    Write-Host "?潛撌脣?鋆? Visual Studio 撖虫?:" -ForegroundColor Green
    foreach ($instance in $vsInstances) {
        Write-Host "  - $($instance.DisplayName) $($instance.Version)" -ForegroundColor Cyan
    }
} else {
    Write-Host "?芰?曉歇摰???Visual Studio" -ForegroundColor Red
}

# 銝??賊?
Write-Host "`n3. Visual Studio 2022 摰??賊?:" -ForegroundColor Yellow

Write-Host @"
?賊? A: Visual Studio 2022 Community (?祥)
- ?拙??犖???皞??柴飛銵?蝛?- 摰?嚗?平?
- ?刻蝯?MingGoRTS ?

?賊? B: Visual Studio 2022 Professional
- ?拙?撠平???撠???
- 憿?????
?賊? C: Visual Studio 2022 Enterprise
- ?拙?憭批?隡平??????- ?摰???"@ -ForegroundColor White

# ?萄遣銝??單
Write-Host "`n4. ?萄遣銝???鋆??.." -ForegroundColor Yellow

$installerScript = @'
@echo off
echo === Visual Studio 2022 Community 摰???===
echo.

REM ?萄遣銝??桅?
if not exist "C:\VS2022_Download" mkdir "C:\VS2022_Download"
cd /d "C:\VS2022_Download"

echo 甇?銝? Visual Studio 2022 Community...
powershell -Command "Invoke-WebRequest -Uri 'https://aka.ms/vs/17/release/vs_community.exe' -OutFile 'vs_community.exe'"

echo 銝?摰?嚗?echo.
echo 摰??賊?嚗?echo 1. ?芸?摰??刻蝯辣嚗?佗?
echo 2. ?芸?蝢拙?鋆?echo 3. ??頛?摰?
echo.
set /p choice="隢??(1-3): "

if "%choice%"=="1" goto auto_install
if "%choice%"=="2" goto custom_install
if "%choice%"=="3" goto download_only

:auto_install
echo 甇??芸?摰??刻蝯辣...
vs_community.exe --quiet --wait --add Microsoft.VisualStudio.Workload.Game --add Microsoft.VisualStudio.Workload.ManagedDesktop --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended
goto end

:custom_install
echo ???芸?蝢拙?鋆?..
vs_community.exe
goto end

:download_only
echo ??頛????典隞交???銵?vs_community.exe ?脰?摰?
goto end

:end
echo.
echo 摰???摰?嚗?pause
'@

$installerScript | Out-File -FilePath "c:\HW\MingGoRTS\Tools\install\Install_VS2022.bat" -Encoding ASCII

Write-Host "??摰??單撌脣撱? c:\HW\MingGoRTS\Tools\install\Install_VS2022.bat" -ForegroundColor Green

# ?刻?極雿?頛?Write-Host "`n5. MingGoRTS ?刻?極雿?頛?" -ForegroundColor Yellow

Write-Host @"
敹?摰??極雿?頛?
??雿輻 C++ ???脤???(Game development with C++)
??.NET 獢? (.NET desktop development)

?刻摰???隞塚?
??Windows 10/11 SDK (??啁???
??C++ MFC ?冽???v143 ??撌亙??x86/x64 ??撌亙
???冽 Windows ??C++ CMake 撌亙
??Git for Windows
??Intellisense
??C++ ??撌亙

?舫蝯辣嚗???Unity ?? (憒??閬?Unity ?舀?)
??Python ? (憒??閬?砍???
"@ -ForegroundColor Cyan

# ??摰???
Write-Host "`n6. ??摰?甇仿?:" -ForegroundColor Yellow

Write-Host @"
甇仿? 1: 閮芸? Visual Studio 摰雯
https://visualstudio.microsoft.com/zh-hans/downloads/

甇仿? 2: 銝? Visual Studio 2022 Community
暺? "Community 2022" 銝???

甇仿? 3: ??摰?蝔?
??銝???vs_community.exe ?辣

甇仿? 4: ?豢?撌乩?鞎?
?暸嚗?- 雿輻 C++ ???脤???- .NET 獢?

甇仿? 5: 摰?蝯辣
?典?湧?蹂葉蝣箔??暸?刻蝯辣

甇仿? 6: ??摰?
暺? "摰?" ??嚗?敺?鋆???
甇仿? 7: ???餉
摰?摰?敺????"@ -ForegroundColor White

# 撽?摰?
Write-Host "`n7. 撽?摰?:" -ForegroundColor Yellow

$verifyScript = @'
# 撽? Visual Studio 2022 摰?
Write-Host "撽? Visual Studio 2022 摰?..." -ForegroundColor Yellow

# 瑼Ｘ devenv.exe
$vsPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
if (Test-Path $vsPath) {
    Write-Host "??Visual Studio 2022 Community 撌脫迤蝣箏?鋆? -ForegroundColor Green
    Write-Host "摰?頝臬?: $vsPath" -ForegroundColor Gray
    
    # 瑼Ｘ?
    $versionInfo = (Get-Item $vsPath).VersionInfo
    Write-Host "?: $($versionInfo.FileVersion)" -ForegroundColor Gray
    
    # 瑼Ｘ C++ 蝯辣
    $vcPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC"
    if (Test-Path $vcPath) {
        Write-Host "??C++ 撌亙?歇摰?" -ForegroundColor Green
    } else {
        Write-Host "??C++ 撌亙??曉" -ForegroundColor Red
    }
    
    # 瑼Ｘ .NET 蝯辣
    $dotnetPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\Roslyn"
    if (Test-Path $dotnetPath) {
        Write-Host "??.NET 撌亙?歇摰?" -ForegroundColor Green
    } else {
        Write-Host "??.NET 撌亙??曉" -ForegroundColor Red
    }
    
} else {
    Write-Host "??Visual Studio 2022 Community ?芣?? -ForegroundColor Red
    Write-Host "隢Ⅱ隤?鋆?血??? -ForegroundColor Yellow
}
'@

$verifyScript | Out-File -FilePath "c:\HW\MingGoRTS\Tools\install\Verify_VS2022.ps1" -Encoding UTF8

Write-Host "??撽??單撌脣撱? c:\HW\MingGoRTS\Tools\install\Verify_VS2022.ps1" -ForegroundColor Green

# 敹恍????Write-Host "`n8. 敹恍????" -ForegroundColor Yellow

Write-Host @"
?賊? 1: ?芸?摰? (?刻)
??: c:\HW\MingGoRTS\Tools\install\Install_VS2022.bat

?賊? 2: ??銝?
閮芸?: https://visualstudio.microsoft.com/zh-hans/downloads/

?賊? 3: ?湔銝????
Community ?? https://aka.ms/vs/17/release/vs_community.exe
Professional ?? https://aka.ms/vs/17/release/vs_professional.exe
Enterprise ?? https://aka.ms/vs/17/release/vs_enterprise.exe
"@ -ForegroundColor Cyan

# 摰?敺?蝵?Write-Host "`n9. 摰?敺?蝵桀遣霅?" -ForegroundColor Yellow

Write-Host @"
1. ?? Visual Studio 2022
2. ?駁? Microsoft 撣單 (?舫)
3. 摰??游??刻嚗?   - Visual Assist X (憓撥 C++ ?)
   - GitHub Extension for Visual Studio
   - C++ Clang power tools
4. ?蔭閮剔蔭嚗?   - 撌亙 ???賊? ???蝺刻摩????C/C++
   - ? IntelliSense
   - ?蔭?澆????5. 皜祈岫蝺刻陌嚗?   - ?? MingGoRTS.sln
   - ?岫蝺刻陌?
"@ -ForegroundColor White

Write-Host "`n=== 摰???摰? ===" -ForegroundColor Green
Write-Host "隢??鋆撘蒂?甇仿??脰?摰??? -ForegroundColor Cyan
Write-Host "摰?摰?敺?銵?霅?祉Ⅱ隤?鋆??? -ForegroundColor Yellow
