# MingGoRTS AI 圖像生成實際操作腳本
# 這個腳本將引導您完成實際的 AI 圖像生成過程

param(
    [string]$Tool = "midjourney",  # midjourney, dalle, stable
    [string]$Batch = "all",         # core, history, ui, effects, all
    [switch]$Interactive = $false   # 互動模式
)

# 設置變數
$ProjectRoot = "C:\HW\MingGoRTS"
$OutputDir = "$ProjectRoot\Content\Intro\Generated\Images"
$LogDir = "$ProjectRoot\Content\Intro\Logs"

# 創建目錄
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
New-Item -ItemType Directory -Path $LogDir -Force | Out-Null

# 日誌函數
function Write-Log {
    param([string]$Message, [string]$Level = "INFO")
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $Color = switch ($Level) {
        "INFO" { "Green" }
        "WARNING" { "Yellow" }
        "ERROR" { "Red" }
        "SUCCESS" { "Cyan" }
        default { "White" }
    }
    Write-Host "[$Timestamp] [$Level] $Message" -ForegroundColor $Color
    Add-Content -Path "$LogDir\ImageGeneration_$(Get-Date -Format 'yyyyMMdd').log" -Value "[$Timestamp] [$Level] $Message"
}

# Midjourney 提示詞庫
$MidjourneyPrompts = @{
    "T_TitleCard" = @{
        Prompt = "Chinese Republican era elegant title card background, 1912-1949, aged parchment texture with traditional Chinese cloud patterns in corners, warm sepia tones, ornate border with dragon motifs, vintage paper texture, calligraphy style elements, historical authenticity, cinematic quality, 1920x1080, ultra detailed, 8K, photorealistic"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "標題卡片背景"
        ExpectedTime = "2-3分鐘"
    }
    "T_MingGoRTS_Logo" = @{
        Prompt = "`"MingGoRTS`" logo design, Chinese Republican era style, English text with Chinese characters `"民國歷史`", traditional calligraphy font, gold gradient effect, subtle shadow, transparent background, vector style, professional logo design, elegant and historical, 800x400, high resolution"
        Params = "--ar 2:1 --q 2 --s 750"
        Description = "遊戲 Logo"
        ExpectedTime = "2-3分鐘"
    }
    "T_GameTitle_Final" = @{
        Prompt = "Epic game title `"MingGoRTS`" with Chinese subtitle `"民國歷史背景即時戰略遊戲`", dramatic background with battlefield scene, gold and red color scheme, cinematic lighting, professional game title design, historical elements, 1920x1080, ultra high quality"
        Params = "--ar 16:9 --q 2 --s 750"
        Description = "最終遊戲標題"
        ExpectedTime = "2-3分鐘"
    }
    "T_1911_Revolution" = @{
        Prompt = "1911 Xinhai Revolution historical scene, Dr. Sun Yat-sen giving speech, revolutionary soldiers in uniform, Chinese Republican era flags, indoor government building, dramatic lighting, historical accuracy, sepia tone effect, vintage photography style, 1920x1080, ultra realistic, detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "辛亥革命場景"
        ExpectedTime = "2-3分鐘"
    }
    "T_Republic_Established" = @{
        Prompt = "Founding ceremony of Republic of China, 1912, five-color flag ceremony, government officials in formal attire, traditional Chinese architecture, outdoor plaza, clear daylight, historical accuracy, formal composition, vintage photography style, 1920x1080, detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "民國建立場景"
        ExpectedTime = "2-3分鐘"
    }
    "T_Warlord_Period" = @{
        Prompt = "Chinese Warlord period battle scene, 1920s, soldiers in period uniforms, traditional Chinese landscape, smoke and gunfire effects, dramatic composition, historical warfare, gritty realism, dark tones, cinematic lighting, 1920x1080, ultra detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "軍閥時期場景"
        ExpectedTime = "2-3分鐘"
    }
    "T_Japanese_Invasion" = @{
        Prompt = "Second Sino-Japanese War scene, 1937-1945, Chinese soldiers defending, destroyed city background, smoke and fire effects, dramatic lighting, historical warfare, patriotic atmosphere, cinematic composition, high contrast, 1920x1080, realistic"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "日軍入侵場景"
        ExpectedTime = "2-3分鐘"
    }
    "UI_Skip_Button_Normal" = @{
        Prompt = "Chinese traditional style skip button, rectangular shape with rounded corners, dark brown background (#8B4513), red border (#DE2910), white text `"跳過`", elegant design, subtle texture, UI element, 200x50, high resolution"
        Params = "--ar 4:1 --q 2 --s 500"
        Description = "跳過按鈕"
        ExpectedTime = "1-2分鐘"
    }
    "UI_Skip_Button_Hover" = @{
        Prompt = "Chinese style skip button hover state, glowing effect, bright red border (#FF6347), yellow text (#FFFF00), light brown background (#A0522D), subtle glow effect, UI element, 200x50, high resolution"
        Params = "--ar 4:1 --q 2 --s 500"
        Description = "跳過按鈕懸停"
        ExpectedTime = "1-2分鐘"
    }
    "UI_Progress_Bar" = @{
        Prompt = "Chinese traditional progress bar, dark brown background (#3E2723), red progress fill (#DE2910), gold border (#FFD700), decorative Chinese patterns, elegant design, 800x20, high resolution"
        Params = "--ar 40:1 --q 2 --s 500"
        Description = "進度條"
        ExpectedTime = "1-2分鐘"
    }
    "P_Sparkle" = @{
        Prompt = "Golden sparkle particle texture, bright yellow center (#FFFF00), golden edges (#FFD700), radial gradient, transparent background, particle effect, 128x128, seamless tile, high quality"
        Params = "--ar 1:1 --q 2 --s 300"
        Description = "閃光粒子"
        ExpectedTime = "1-2分鐘"
    }
    "P_Smoke" = @{
        Prompt = "Battlefield smoke particle texture, gray tones (#808080), soft edges, semi-transparent, atmospheric effect, 256x256, seamless tile, realistic smoke"
        Params = "--ar 1:1 --q 2 --s 300"
        Description = "煙霧粒子"
        ExpectedTime = "1-2分鐘"
    }
    "P_Glow" = @{
        Prompt = "Soft glow effect texture, white center (#FFFFFF), radial transparency gradient, seamless edges, 512x512, high resolution, lighting effect"
        Params = "--ar 1:1 --q 2 --s 300"
        Description = "光暈效果"
        ExpectedTime = "1-2分鐘"
    }
    "T_Heroes_Collage" = @{
        Prompt = "Collage of Chinese Republic era heroes, Chiang Kai-shek, Mao Zedong, historical figures, black and white photography style, vintage photo effect, dramatic lighting, historical accuracy, professional portrait style, 1920x1080, detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "英雄拼貼"
        ExpectedTime = "2-3分鐘"
    }
}

# 批次定義
$Batches = @{
    "core" = @("T_TitleCard", "T_MingGoRTS_Logo", "T_GameTitle_Final")
    "history" = @("T_1911_Revolution", "T_Republic_Established", "T_Warlord_Period", "T_Japanese_Invasion")
    "ui" = @("UI_Skip_Button_Normal", "UI_Skip_Button_Hover", "UI_Progress_Bar")
    "effects" = @("P_Sparkle", "P_Smoke", "P_Glow", "T_Heroes_Collage")
    "all" = $MidjourneyPrompts.Keys
}

# 顯示工具使用指南
function Show-ToolGuide {
    param([string]$ToolName)
    
    Write-Log "=== $ToolName 使用指南 ===" "INFO"
    
    switch ($ToolName) {
        "midjourney" {
            Write-Host @"
1. 開啟 Discord
2. 加入 Midjourney 伺服器
3. 進入 #newbies 或 #general 頻道
4. 使用 /imagine 命令
5. 複製下面的提示詞
6. 粘貼到 Discord 並發送
7. 等待生成完成
8. 選擇最佳版本 (U1-U4)
9. 使用放大命令 (U1-U4)
10. 下載生成的圖像
"@ -ForegroundColor Yellow
        }
        "dalle" {
            Write-Host @"
1. 開啟 ChatGPT Plus
2. 選擇 DALL-E 3 模型
3. 複製下面的提示詞
4. 粘貼到 ChatGPT
5. 等待生成完成
6. 下載生成的圖像
"@ -ForegroundColor Yellow
        }
        "stable" {
            Write-Host @"
1. 開啟 Stable Diffusion WebUI
2. 選擇適當的模型
3. 複製下面的提示詞
4. 設置參數
5. 點擊生成
6. 下載生成的圖像
"@ -ForegroundColor Yellow
        }
    }
}

# 生成單個素材
function Invoke-ImageGeneration {
    param([string]$AssetName, [hashtable]$PromptData)
    
    Write-Log "開始生成: $AssetName - $($PromptData.Description)" "INFO"
    Write-Log "預期時間: $($PromptData.ExpectedTime)" "INFO"
    
    # 顯示提示詞
    $FullPrompt = "$($PromptData.Prompt) $($PromptData.Params)"
    Write-Host "提示詞:" -ForegroundColor Cyan
    Write-Host $FullPrompt -ForegroundColor White
    
    # 複製到剪貼簿
    $FullPrompt | Set-Clipboard
    Write-Log "提示詞已複製到剪貼簿" "SUCCESS"
    
    if ($Interactive) {
        Write-Host "`n按 Enter 繼續下一個素材..." -ForegroundColor Yellow
        Read-Host
    }
    
    # 記錄生成
    $GenerationRecord = @{
        AssetName = $AssetName
        Description = $PromptData.Description
        Prompt = $FullPrompt
        StartTime = Get-Date
        Status = "Pending"
    }
    
    $GenerationRecord | ConvertTo-Json | Out-File -FilePath "$OutputDir\$AssetName.json" -Encoding UTF8
    Write-Log "生成記錄已保存: $AssetName.json" "INFO"
}

# 主執行函數
function Start-ImageGeneration {
    Write-Log "開始 AI 圖像生成" "INFO"
    Write-Log "工具: $Tool" "INFO"
    Write-Log "批次: $Batch" "INFO"
    
    # 顯示工具指南
    Show-ToolGuide -ToolName $Tool
    
    # 獲取要生成的素材列表
    $Assets = if ($Batch -eq "all") { 
        $MidjourneyPrompts.Keys 
    } else { 
        $Batches[$Batch] 
    }
    
    if (-not $Assets) {
        Write-Log "錯誤: 批次 '$Batch' 不存在" "ERROR"
        return
    }
    
    Write-Log "將生成 $($Assets.Count) 個素材" "INFO"
    
    # 顯示生成計劃
    Write-Host "`n=== 生成計劃 ===" -ForegroundColor Green
    for ($i = 0; $i -lt $Assets.Count; $i++) {
        $Asset = $Assets[$i]
        $PromptData = $MidjourneyPrompts[$Asset]
        Write-Host "$($i + 1). $Asset - $($PromptData.Description) ($($PromptData.ExpectedTime))" -ForegroundColor White
    }
    
    if ($Interactive) {
        Write-Host "`n按 Enter 開始生成..." -ForegroundColor Yellow
        Read-Host
    }
    
    # 生成每個素材
    for ($i = 0; $i -lt $Assets.Count; $i++) {
        $Asset = $Assets[$i]
        $PromptData = $MidjourneyPrompts[$Asset]
        
        Write-Host "`n=== 生成 $($i + 1)/$($Assets.Count): $Asset ===" -ForegroundColor Green
        
        Invoke-ImageGeneration -AssetName $Asset -PromptData $PromptData
        
        if ($i -lt $Assets.Count - 1) {
            Write-Host "`n準備下一個素材..." -ForegroundColor Yellow
            if ($Interactive) {
                Start-Sleep -Seconds 5
            }
        }
    }
    
    Write-Log "AI 圖像生成完成" "SUCCESS"
}

# 品質檢查函數
function Invoke-QualityCheck {
    Write-Log "開始品質檢查" "INFO"
    
    Write-Host @"
=== 品質檢查清單 ===

請對每個生成的圖像進行以下檢查:

□ 解析度是否為 1920x1080 (16:9)
□ 圖像是否清晰無模糊
□ 色彩是否平衡良好
□ 構圖是否專業
□ 風格是否符合民國歷史背景
□ 細節是否豐富準確
□ 整體風格是否統一

=== 後期處理建議 ===

1. 使用 Photoshop/GIMP 進行色彩校正
2. 調整圖像尺寸至標準解析度
3. 增強對比度和清晰度
4. 轉換為正確格式 (PNG/JPEG)
5. 確保檔案大小合理

=== 檔案命名規範 ===

- 背景圖片: T_[名稱].jpg
- Logo/UI: [名稱].png
- 粒子效果: P_[名稱].png
- 最終版本: [名稱]_Final.[格式]
"@ -ForegroundColor Cyan
}

# 顯示統計信息
function Show-Statistics {
    Write-Host @"
=== AI 圖像生成統計 ===

總素材數量: 14個
預期生成時間: 2-3小時
平均每個素材: 10-15分鐘

批次分佈:
- 核心素材: 3個 (30-45分鐘)
- 歷史素材: 4個 (40-60分鐘)
- UI元素: 3個 (20-30分鐘)
- 效果素材: 4個 (30-45分鐘)

品質目標:
- 平均評分: 8.5/10以上
- 成功率: 100%
- 歷史準確性: 95%以上
"@ -ForegroundColor Green
}

# 執行主程序
try {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "    MingGoRTS AI 圖像生成工具" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    
    Show-Statistics
    
    if ($Interactive) {
        Write-Host "`n準備開始 AI 圖像生成..." -ForegroundColor Yellow
        Write-Host "確保您已經準備好相應的 AI 工具帳號" -ForegroundColor Yellow
        Write-Host "`n按 Enter 開始..." -ForegroundColor Yellow
        Read-Host
    }
    
    Start-ImageGeneration
    Invoke-QualityCheck
    
    Write-Log "所有任務完成" "SUCCESS"
    Write-Host "`n🎉 AI 圖像生成流程完成！" -ForegroundColor Green
    Write-Host "請按照品質檢查清單驗證生成的圖像" -ForegroundColor Green
    
}
catch {
    Write-Log "錯誤: $($_.Exception.Message)" "ERROR"
}

# 使用說明
Write-Host @"
=== 使用說明 ===

基本用法:
.\Start_AI_Image_Generation.ps1

批次生成:
.\Start_AI_Image_Generation.ps1 -Batch core
.\Start_AI_Image_Generation.ps1 -Batch history
.\Start_AI_Image_Generation.ps1 -Batch ui
.\Start_AI_Image_Generation.ps1 -Batch effects

互動模式:
.\Start_AI_Image_Generation.ps1 -Interactive

更換工具:
.\Start_AI_Image_Generation.ps1 -Tool dalle
.\Start_AI_Image_Generation.ps1 -Tool stable

注意事項:
- 確保您有相應的 AI 工具帳號
- 提示詞會自動複製到剪貼簿
- 生成記錄會保存在 Logs 目錄
- 請按照品質標準檢查生成的圖像
"@ -ForegroundColor Cyan
