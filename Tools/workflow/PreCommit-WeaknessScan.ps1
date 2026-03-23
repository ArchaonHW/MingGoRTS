# Pre-Commit Weakness Scan Hook
# 在每次提交前自動執行弱點掃描

param(
    [string]$ProjectPath = "F:\MingGoRTS",
    [switch]$SkipScan = $false,
    [switch]$AutoFix = $false
)

$ErrorActionPreference = "Stop"
$LogFile = "$ProjectPath\Logs\PreCommitScan_$(Get-Date -Format 'yyyyMMdd_HHmmss').log"

# 確保日誌目錄存在
$LogDir = "$ProjectPath\Logs"
if (!(Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
}

function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logEntry = "[$timestamp] [$Level] $Message"
    Write-Host $logEntry -ForegroundColor $(
        switch($Level) {
            "ERROR" { "Red" }
            "WARN"  { "Yellow" }
            "SUCCESS" { "Green" }
            default { "White" }
        }
    )
    Add-Content -Path $LogFile -Value $logEntry
}

Write-Log "=== 提交前弱點掃描開始 ==="

if ($SkipScan) {
    Write-Log "跳過掃描 (SkipScan = true)" "WARN"
    exit 0
}

# 1. 執行語法檢查
Write-Log "步驟 1: 執行語法檢查..."
try {
    $SyntaxIssues = 0
    $CppFiles = Get-ChildItem -Path "$ProjectPath\Source" -Recurse -Filter "*.cpp" -ErrorAction SilentlyContinue
    foreach ($File in $CppFiles) {
        $Content = Get-Content -Path $File.FullName -Raw -ErrorAction SilentlyContinue
        if ($Content -match "\{\s*\}$" -or $Content -match ";;") {
            $SyntaxIssues++
            Write-Log "  發現語法問題: $($File.Name)" "WARN"
        }
    }
    Write-Log "語法檢查完成，發現 $SyntaxIssues 個問題"
}
catch {
    Write-Log "語法檢查失敗: $($_.Exception.Message)" "ERROR"
}

# 2. 檢查編碼問題
Write-Log "步驟 2: 檢查編碼問題..."
try {
    $EncodingIssues = 0
    foreach ($File in $CppFiles) {
        $Content = Get-Content -Path $File.FullName -Raw -ErrorAction SilentlyContinue
        if ($Content -match "[使本成設置正軍的輸]" -or $Content -match "Min成|Gene本ated|En成ine") {
            $EncodingIssues++
            Write-Log "  發現編碼問題: $($File.Name)" "WARN"
        }
    }
    Write-Log "編碼檢查完成，發現 $EncodingIssues 個問題"
}
catch {
    Write-Log "編碼檢查失敗: $($_.Exception.Message)" "ERROR"
}

# 3. 檢查頭文件引用
Write-Log "步驟 3: 檢查頭文件引用..."
try {
    $HeaderIssues = 0
    $HeaderFiles = Get-ChildItem -Path "$ProjectPath\Source" -Recurse -Filter "*.h" -ErrorAction SilentlyContinue
    foreach ($File in $HeaderFiles) {
        $Content = Get-Content -Path $File.FullName -Raw -ErrorAction SilentlyContinue
        if ($Content -match "#include.*\.h" -and !($Content -match "#pragma once")) {
            $HeaderIssues++
            Write-Log "  缺少 #pragma once: $($File.Name)" "WARN"
        }
    }
    Write-Log "頭文件檢查完成，發現 $HeaderIssues 個問題"
}
catch {
    Write-Log "頭文件檢查失敗: $($_.Exception.Message)" "ERROR"
}

# 4. 檢查文件大小
Write-Log "步驟 4: 檢查文件大小..."
try {
    $LargeFiles = 0
    foreach ($File in $CppFiles) {
        if ($File.Length -gt 100KB) {
            $LargeFiles++
            Write-Log "  大文件警告: $($File.Name) ($([math]::Round($File.Length/1KB, 2)) KB)" "WARN"
        }
    }
    Write-Log "文件大小檢查完成，發現 $LargeFiles 個大文件"
}
catch {
    Write-Log "文件大小檢查失敗: $($_.Exception.Message)" "ERROR"
}

# 5. 統計結果
$TotalIssues = $SyntaxIssues + $EncodingIssues + $HeaderIssues

Write-Log ""
Write-Log "=== 掃描結果摘要 ==="
Write-Log "語法問題: $SyntaxIssues"
Write-Log "編碼問題: $EncodingIssues"
Write-Log "頭文件問題: $HeaderIssues"
Write-Log "大文件警告: $LargeFiles"
Write-Log "總問題數: $TotalIssues"

if ($TotalIssues -eq 0) {
    Write-Log "✅ 沒有發現弱點，可以安全提交" "SUCCESS"
    exit 0
}
elseif ($TotalIssues -lt 10) {
    Write-Log "⚠️ 發現少量問題，建議修復後再提交" "WARN"
    if ($AutoFix) {
        Write-Log "嘗試自動修復..."
        # 這裡可以添加自動修復邏輯
    }
    exit 0
}
else {
    Write-Log "❌ 發現嚴重問題，請修復後再提交" "ERROR"
    exit 1
}
