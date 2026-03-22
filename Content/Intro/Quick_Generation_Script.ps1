# MingGoRTS AI 素材快速生成腳本
# 使用 PowerShell 自動化 AI 素材生成流程

param(
    [string]$Tool = "midjourney",  # midjourney, dalle, suno
    [string]$Batch = "core",       # core, history, ui, effects
    [switch]$Test = $false         # 測試模式
)

# 設置變數
$ProjectRoot = "C:\HW\MingGoRTS"
$OutputDir = "$ProjectRoot\Content\Intro\Generated"
$LogDir = "$ProjectRoot\Content\Intro\Logs"

# 創建目錄
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
New-Item -ItemType Directory -Path $LogDir -Force | Out-Null

# 日誌函數
function Write-Log {
    param([string]$Message)
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    Write-Host "[$Timestamp] $Message" -ForegroundColor Green
    Add-Content -Path "$LogDir\Generation_$(Get-Date -Format 'yyyyMMdd').log" -Value "[$Timestamp] $Message"
}

# Midjourney 提示詞庫
$MidjourneyPrompts = @{
    "T_TitleCard" = "Chinese Republican era elegant title card background, 1912-1949, aged parchment texture with traditional Chinese cloud patterns in corners, warm sepia tones, ornate border with dragon motifs, vintage paper texture, calligraphy style elements, historical authenticity, cinematic quality, 1920x1080, ultra detailed, 8K, photorealistic --ar 16:9 --q 2 --style raw"
    "T_MingGoRTS_Logo" = "'MingGoRTS' logo design, Chinese Republican era style, English text with Chinese characters '民國歷史', traditional calligraphy font, gold gradient effect, subtle shadow, transparent background, vector style, professional logo design, elegant and historical, 800x400, high resolution --ar 2:1 --q 2"
    "T_GameTitle_Final" = "Epic game title 'MingGoRTS' with Chinese subtitle '民國歷史背景即時戰略遊戲', dramatic background with battlefield scene, gold and red color scheme, cinematic lighting, professional game title design, historical elements, 1920x1080, ultra high quality --ar 16:9 --q 2"
    "T_1911_Revolution" = "1911 Xinhai Revolution historical scene, Dr. Sun Yat-sen giving speech, revolutionary soldiers in uniform, Chinese Republican era flags, indoor government building, dramatic lighting, historical accuracy, sepia tone effect, vintage photography style, 1920x1080, ultra realistic, detailed --ar 16:9 --q 2"
    "T_Republic_Established" = "Founding ceremony of Republic of China, 1912, five-color flag ceremony, government officials in formal attire, traditional Chinese architecture, outdoor plaza, clear daylight, historical accuracy, formal composition, vintage photography style, 1920x1080, detailed --ar 16:9 --q 2"
    "T_Warlord_Period" = "Chinese Warlord period battle scene, 1920s, soldiers in period uniforms, traditional Chinese landscape, smoke and gunfire effects, dramatic composition, historical warfare, gritty realism, dark tones, cinematic lighting, 1920x1080, ultra detailed --ar 16:9 --q 2"
    "T_Japanese_Invasion" = "Second Sino-Japanese War scene, 1937-1945, Chinese soldiers defending, destroyed city background, smoke and fire effects, dramatic lighting, historical warfare, patriotic atmosphere, cinematic composition, high contrast, 1920x1080, realistic --ar 16:9 --q 2"
}

# DALL-E 3 提示詞庫
$DallePrompts = @{
    "T_TitleCard" = "Chinese Republican era elegant title card background, 1912-1949, aged parchment texture with traditional Chinese cloud patterns in corners, warm sepia tones, ornate border with dragon motifs, vintage paper texture, calligraphy style elements, historical authenticity, cinematic quality, 1920x1080, ultra detailed, 8K, photorealistic"
    "T_MingGoRTS_Logo" = "'MingGoRTS' logo design, Chinese Republican era style, English text with Chinese characters '民國歷史', traditional calligraphy font, gold gradient effect, subtle shadow, transparent background, vector style, professional logo design, elegant and historical, 800x400, high resolution"
    "T_GameTitle_Final" = "Epic game title 'MingGoRTS' with Chinese subtitle '民國歷史背景即時戰略遊戲', dramatic background with battlefield scene, gold and red color scheme, cinematic lighting, professional game title design, historical elements, 1920x1080, ultra high quality"
}

# Suno AI 音效提示詞庫
$SunoPrompts = @{
    "IntroMusic_MainTheme" = "Chinese Republican era military theme music, 12 seconds duration, 120 BPM, majestic brass fanfare, traditional Chinese instruments mixed with Western military band, solemn and patriotic atmosphere, dramatic orchestration, historical authenticity, high quality audio"
    "GameTitle_Fanfare" = "Chinese Republican era fanfare, 4 seconds, powerful trumpet calls, military brass section, dramatic crescendo, heroic and glorious, traditional Chinese musical elements, cinematic impact, professional fanfare composition"
    "HistoryBackground_Ambience" = "Chinese Republican era historical background ambience, 5 seconds, vintage photograph page turning sounds, historical narration voice texture, subtle wind sounds, nostalgic atmosphere, aged audio quality, documentary style"
    "UI_Click" = "Traditional Chinese UI click sound, 0.2 seconds, bamboo clapper sound, wooden fish percussion, clean and crisp, traditional Chinese audio aesthetic, gentle and elegant, high quality recording"
    "UI_Hover" = "Chinese traditional UI hover sound, 0.1 seconds, gentle bell chime, silk string pluck, soft and elegant, traditional Chinese sound design, subtle and refined, high quality"
    "Transition_Swoosh" = "Smooth transition swoosh sound, 0.5 seconds, gentle wind whoosh, cinematic transition effect, fluid and elegant, professional sound design, high quality audio"
}

# 批次定義
$Batches = @{
    "core" = @("T_TitleCard", "T_MingGoRTS_Logo", "T_GameTitle_Final", "IntroMusic_MainTheme", "GameTitle_Fanfare")
    "history" = @("T_1911_Revolution", "T_Republic_Established", "T_Warlord_Period", "T_Japanese_Invasion", "HistoryBackground_Ambience")
    "ui" = @("UI_Click", "UI_Hover", "Transition_Swoosh")
    "effects" = @("P_Sparkle", "P_Smoke", "P_Glow", "T_Heroes_Collage")
}

# 生成函數
function Invoke-MidjourneyGeneration {
    param([string]$Prompt, [string]$Name)
    
    Write-Log "開始 Midjourney 生成: $Name"
    
    if ($Test) {
        Write-Log "測試模式: $Prompt"
        return
    }
    
    # 這裡需要實際的 Midjourney API 調用
    # 目前為模擬實現
    $Prompt | Out-File -FilePath "$OutputDir\midjourney_$Name.txt" -Encoding UTF8
    Write-Log "Midjourney 提示詞已保存: $Name"
    
    # 模擬等待時間
    Start-Sleep -Seconds 30
    
    Write-Log "Midjourney 生成完成: $Name"
}

function Invoke-DalleGeneration {
    param([string]$Prompt, [string]$Name)
    
    Write-Log "開始 DALL-E 3 生成: $Name"
    
    if ($Test) {
        Write-Log "測試模式: $Prompt"
        return
    }
    
    # 這裡需要實際的 DALL-E API 調用
    # 目前為模擬實現
    $Prompt | Out-File -FilePath "$OutputDir\dalle_$Name.txt" -Encoding UTF8
    Write-Log "DALL-E 提示詞已保存: $Name"
    
    # 模擬等待時間
    Start-Sleep -Seconds 20
    
    Write-Log "DALL-E 生成完成: $Name"
}

function Invoke-SunoGeneration {
    param([string]$Prompt, [string]$Name)
    
    Write-Log "開始 Suno AI 生成: $Name"
    
    if ($Test) {
        Write-Log "測試模式: $Prompt"
        return
    }
    
    # 這裡需要實際的 Suno AI API 調用
    # 目前為模擬實現
    $Prompt | Out-File -FilePath "$OutputDir\suno_$Name.txt" -Encoding UTF8
    Write-Log "Suno AI 提示詞已保存: $Name"
    
    # 模擬等待時間
    Start-Sleep -Seconds 45
    
    Write-Log "Suno AI 生成完成: $Name"
}

# 主執行函數
function Start-AIGeneration {
    Write-Log "開始 AI 素材生成"
    Write-Log "工具: $Tool"
    Write-Log "批次: $Batch"
    
    # 獲取當前批次的素材列表
    $Assets = $Batches[$Batch]
    if (-not $Assets) {
        Write-Log "錯誤: 批次 '$Batch' 不存在"
        return
    }
    
    Write-Log "將生成 $($Assets.Count) 個素材"
    
    # 根據工具選擇提示詞庫
    switch ($Tool) {
        "midjourney" {
            $Prompts = $MidjourneyPrompts
        }
        "dalle" {
            $Prompts = $DallePrompts
        }
        "suno" {
            $Prompts = $SunoPrompts
        }
        default {
            Write-Log "錯誤: 不支援的工具 '$Tool'"
            return
        }
    }
    
    # 生成每個素材
    foreach ($Asset in $Assets) {
        $Prompt = $Prompts[$Asset]
        if (-not $Prompt) {
            Write-Log "警告: 素材 '$Asset' 的提示詞不存在"
            continue
        }
        
        switch ($Tool) {
            "midjourney" {
                Invoke-MidjourneyGeneration -Prompt $Prompt -Name $Asset
            }
            "dalle" {
                Invoke-DalleGeneration -Prompt $Prompt -Name $Asset
            }
            "suno" {
                Invoke-SunoGeneration -Prompt $Prompt -Name $Asset
            }
        }
        
        # 批次間隔
        if (-not $Test) {
            Start-Sleep -Seconds 10
        }
    }
    
    Write-Log "AI 素材生成完成"
}

# 品質檢查函數
function Invoke-QualityCheck {
    Write-Log "開始品質檢查"
    
    $GeneratedFiles = Get-ChildItem -Path $OutputDir -Filter "*.png" -ErrorAction SilentlyContinue
    
    foreach ($File in $GeneratedFiles) {
        $Size = Get-ImageSize -Path $File.FullName
        Write-Log "檢查檔案: $($File.Name) - 尺寸: $($Size.Width)x$($Size.Height)"
    }
    
    Write-Log "品質檢查完成"
}

# 獲取圖片尺寸函數 (模擬)
function Get-ImageSize {
    param([string]$Path)
    return @{ Width = 1920; Height = 1080 }
}

# 執行主程序
try {
    Start-AIGeneration
    
    if (-not $Test) {
        Invoke-QualityCheck
    }
    
    Write-Log "所有任務完成"
}
catch {
    Write-Log "錯誤: $($_.Exception.Message)"
}

# 使用說明
Write-Host @"
使用說明:
1. 測試模式: .\Quick_Generation_Script.ps1 -Test
2. 生成核心批次: .\Quick_Generation_Script.ps1 -Tool midjourney -Batch core
3. 生成歷史批次: .\Quick_Generation_Script.ps1 -Tool dalle -Batch history
4. 生成音效批次: .\Quick_Generation_Script.ps1 -Tool suno -Batch ui

支援的工具:
- midjourney: 圖像生成
- dalle: 圖像生成
- suno: 音效生成

支援的批次:
- core: 核心素材
- history: 歷史素材
- ui: UI 元素
- effects: 效果素材
"@ -ForegroundColor Cyan
