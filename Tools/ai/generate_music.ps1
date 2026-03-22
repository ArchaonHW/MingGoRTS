# MingGoRTS AI Music Generator PowerShell Script
# ?砍 AI ?單????單 - PowerShell ?
# 雿輻?孵?: .\generate_music.ps1 [?賊?]

param(
    [string]$Preset = "",
    [string]$Prompt = "",
    [int]$Duration = 10,
    [string]$Output = "generated",
    [switch]$All,
    [switch]$List,
    [switch]$Help
)

$ErrorActionPreference = "Stop"

# 憿頛詨?賣
function Write-ColorOutput($ForegroundColor) {
    $fc = $host.UI.RawUI.ForegroundColor
    $host.UI.RawUI.ForegroundColor = $ForegroundColor
    if ($args) {
        Write-Output $args
    }
    $host.UI.RawUI.ForegroundColor = $fc
}

function Write-Success($message) {
    Write-ColorOutput Green "??$message"
}

function Write-Info($message) {
    Write-ColorOutput Cyan "??$message"
}

function Write-Warning($message) {
    Write-ColorOutput Yellow "??$message"
}

function Write-Error($message) {
    Write-ColorOutput Red "??$message"
}

# 憿舐內璅?
Write-Output ""
Write-ColorOutput Magenta "? MingGoRTS AI Music Generator"
Write-ColorOutput Magenta "=============================="
Write-Output ""

# 瑼Ｘ Python
Write-Info "瑼Ｘ Python ?啣?..."
$pythonVersion = python --version 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Error "Python ?芸?鋆??芸???PATH"
    exit 1
}
Write-Success "Python ?: $pythonVersion"

# 閮剖?頝臬?
$ProjectRoot = $PSScriptRoot | Split-Path -Parent
$ToolsPath = Join-Path $ProjectRoot "Tools\ai"
$GeneratorScript = Join-Path $ToolsPath "music_generator.py"

# 瑼Ｘ?單摮
if (-not (Test-Path $GeneratorScript)) {
    Write-Error "?曆??圈璅???? $GeneratorScript"
    exit 1
}

# 憿舐內隤芣?
if ($Help) {
    Write-Output "雿輻?寞?:"
    Write-Output "  .\generate_music.ps1 -Preset <憸冽??     # 雿輻?身憸冽??"
    Write-Output "  .\generate_music.ps1 -Prompt <?內閰?     # ?芸?蝢拇?蝷箄?"
    Write-Output "  .\generate_music.ps1 -All                  # ?????閮?
    Write-Output "  .\generate_music.ps1 -List                # ????閮?
    Write-Output ""
    Write-Output "?:"
    Write-Output "  -Preset    ?身憸冽?迂 (main_theme, battle, victory, building, menu_background)"
    Write-Output "  -Prompt    ?芸?蝢拇?蝷箄???"
    Write-Output "  -Duration   ?單??瑕漲蝘 (?身: 10)"
    Write-Output "  -Output     頛詨瑼? (?身: generated)"
    Write-Output "  -All        ?寥??????閮剝璅?
    Write-Output "  -List       憿舐內???券?閮?
    Write-Output "  -Help       憿舐內甇方牧??
    Write-Output ""
    Write-Output "蝭?:"
    Write-Output "  .\generate_music.ps1 -Preset main_theme"
    Write-Output "  .\generate_music.ps1 -Prompt 'Epic battle music' -Duration 30"
    Write-Output "  .\generate_music.ps1 -All"
    exit 0
}

# ???啣?獢?桅?
Set-Location $ProjectRoot
Write-Info "撌乩??桅?: $ProjectRoot"

# 撱箇??誘
$arguments = @()

if ($List) {
    $arguments += "--list"
}
elseif ($All) {
    $arguments += "--all"
}
elseif ($Preset) {
    $arguments += "--preset"
    $arguments += $Preset
}
elseif ($Prompt) {
    $arguments += "--prompt"
    $arguments += "`"$Prompt`""
    $arguments += "--duration"
    $arguments += $Duration
    $arguments += "--output"
    $arguments += $Output
}
else {
    # ?身嚗＊蝷箄牧??    Write-Output ""
    Write-Warning "?芣?摰??芋撘?憿舐內雿輻隤芣?..."
    Write-Output ""
    & $GeneratorScript @("--list")
    Write-Output ""
    Write-Info "雿輻?寞?:"
    Write-Output "  1. ???桀?閮? .\generate_music.ps1 -Preset main_theme"
    Write-Output "  2. ?寥?????? .\generate_music.ps1 -All"
    Write-Output "  3. ?芸?蝢拍???   .\generate_music.ps1 -Prompt 'your prompt' -Duration 30"
    Write-Output "  4. 憿舐內隤芣?:     .\generate_music.ps1 -Help"
    Write-Output ""
    
    # 閰Ｗ??臬??蝭?
    $response = Read-Host "?臬??蝭??單? 'main_theme'? (y/n)"
    if ($response -eq "y" -or $response -eq "Y") {
        $arguments += "--preset"
        $arguments += "main_theme"
    } else {
        exit 0
    }
}

# ?瑁???
Write-Output ""
Write-Info "?????單?..."
Write-Output "?瑁?: python $GeneratorScript $arguments"
Write-Output ""

try {
    & python $GeneratorScript @arguments
    
    if ($LASTEXITCODE -eq 0) {
        Write-Output ""
        Write-Success "?單???摰?嚗?
        
        # 憿舐內頛詨雿蔭
        $OutputPath = Join-Path $ProjectRoot "Content\Audio\Generated"
        if (Test-Path $OutputPath) {
            Write-Info "頛詨雿蔭: $OutputPath"
            
            # ??餈???瑼?
            $recentFiles = Get-ChildItem -Path $OutputPath -Recurse -Filter "*.wav" | 
                           Sort-Object LastWriteTime -Descending | 
                           Select-Object -First 5
            
            if ($recentFiles) {
                Write-Output ""
                Write-Info "?餈???瑼?:"
                $recentFiles | ForEach-Object {
                    $size = "{0:N2} MB" -f ($_.Length / 1MB)
                    Write-Output "  ??$($_.Name) ($size)"
                }
            }
        }
    } else {
        Write-Error "?單???憭望? (Exit Code: $LASTEXITCODE)"
    }
} catch {
    Write-Error "?瑁?憭望?: $_"
    exit 1
}

Write-Output ""
Write-ColorOutput Magenta "=============================="

