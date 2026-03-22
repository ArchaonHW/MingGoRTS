# MingGoRTS AI Music Generator PowerShell Script
# 本地 AI 音樂生成腳本 - PowerShell 版本
# 使用方式: .\generate_music.ps1 [選項]

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

# 顏色輸出函數
function Write-ColorOutput($ForegroundColor) {
    $fc = $host.UI.RawUI.ForegroundColor
    $host.UI.RawUI.ForegroundColor = $ForegroundColor
    if ($args) {
        Write-Output $args
    }
    $host.UI.RawUI.ForegroundColor = $fc
}

function Write-Success($message) {
    Write-ColorOutput Green "✓ $message"
}

function Write-Info($message) {
    Write-ColorOutput Cyan "ℹ $message"
}

function Write-Warning($message) {
    Write-ColorOutput Yellow "⚠ $message"
}

function Write-Error($message) {
    Write-ColorOutput Red "✗ $message"
}

# 顯示標題
Write-Output ""
Write-ColorOutput Magenta "🎵 MingGoRTS AI Music Generator"
Write-ColorOutput Magenta "=============================="
Write-Output ""

# 檢查 Python
Write-Info "檢查 Python 環境..."
$pythonVersion = python --version 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Error "Python 未安裝或未加入 PATH"
    exit 1
}
Write-Success "Python 版本: $pythonVersion"

# 設定路徑
$ProjectRoot = $PSScriptRoot | Split-Path -Parent
$ToolsPath = Join-Path $ProjectRoot "Tools\ai"
$GeneratorScript = Join-Path $ToolsPath "music_generator.py"

# 檢查腳本存在
if (-not (Test-Path $GeneratorScript)) {
    Write-Error "找不到音樂生成腳本: $GeneratorScript"
    exit 1
}

# 顯示說明
if ($Help) {
    Write-Output "使用方法:"
    Write-Output "  .\generate_music.ps1 -Preset <風格名>     # 使用預設風格生成"
    Write-Output "  .\generate_music.ps1 -Prompt <提示詞>     # 自定義提示詞"
    Write-Output "  .\generate_music.ps1 -All                  # 生成所有預設"
    Write-Output "  .\generate_music.ps1 -List                # 列出所有預設"
    Write-Output ""
    Write-Output "參數:"
    Write-Output "  -Preset    預設風格名稱 (main_theme, battle, victory, building, menu_background)"
    Write-Output "  -Prompt    自定義提示詞文字"
    Write-Output "  -Duration   音樂長度秒數 (預設: 10)"
    Write-Output "  -Output     輸出檔名 (預設: generated)"
    Write-Output "  -All        批量生成所有預設音樂"
    Write-Output "  -List       顯示所有可用預設"
    Write-Output "  -Help       顯示此說明"
    Write-Output ""
    Write-Output "範例:"
    Write-Output "  .\generate_music.ps1 -Preset main_theme"
    Write-Output "  .\generate_music.ps1 -Prompt 'Epic battle music' -Duration 30"
    Write-Output "  .\generate_music.ps1 -All"
    exit 0
}

# 切換到專案根目錄
Set-Location $ProjectRoot
Write-Info "工作目錄: $ProjectRoot"

# 建立指令
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
    # 預設：顯示說明
    Write-Output ""
    Write-Warning "未指定生成模式，顯示使用說明..."
    Write-Output ""
    & $GeneratorScript @("--list")
    Write-Output ""
    Write-Info "使用方法:"
    Write-Output "  1. 生成單個預設: .\generate_music.ps1 -Preset main_theme"
    Write-Output "  2. 批量生成所有: .\generate_music.ps1 -All"
    Write-Output "  3. 自定義生成:   .\generate_music.ps1 -Prompt 'your prompt' -Duration 30"
    Write-Output "  4. 顯示說明:     .\generate_music.ps1 -Help"
    Write-Output ""
    
    # 詢問是否生成範例
    $response = Read-Host "是否生成範例音樂 'main_theme'? (y/n)"
    if ($response -eq "y" -or $response -eq "Y") {
        $arguments += "--preset"
        $arguments += "main_theme"
    } else {
        exit 0
    }
}

# 執行生成
Write-Output ""
Write-Info "開始生成音樂..."
Write-Output "執行: python $GeneratorScript $arguments"
Write-Output ""

try {
    & python $GeneratorScript @arguments
    
    if ($LASTEXITCODE -eq 0) {
        Write-Output ""
        Write-Success "音樂生成完成！"
        
        # 顯示輸出位置
        $OutputPath = Join-Path $ProjectRoot "Content\Audio\Generated"
        if (Test-Path $OutputPath) {
            Write-Info "輸出位置: $OutputPath"
            
            # 列出最近生成的檔案
            $recentFiles = Get-ChildItem -Path $OutputPath -Recurse -Filter "*.wav" | 
                           Sort-Object LastWriteTime -Descending | 
                           Select-Object -First 5
            
            if ($recentFiles) {
                Write-Output ""
                Write-Info "最近生成的檔案:"
                $recentFiles | ForEach-Object {
                    $size = "{0:N2} MB" -f ($_.Length / 1MB)
                    Write-Output "  • $($_.Name) ($size)"
                }
            }
        }
    } else {
        Write-Error "音樂生成失敗 (Exit Code: $LASTEXITCODE)"
    }
} catch {
    Write-Error "執行失敗: $_"
    exit 1
}

Write-Output ""
Write-ColorOutput Magenta "=============================="
