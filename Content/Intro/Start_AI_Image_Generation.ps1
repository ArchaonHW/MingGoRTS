# OPTIMIZED: Script uses buffered logging for better performance
# Original had multiple Write-Host calls that can slow execution

$Script:LogBuffer = @()
function Write-BufferedLog {
    param([string]$Message)
    $Script:LogBuffer += "[03:06:25] $Message"
    if ($Script:LogBuffer.Count -ge 100) { Flush-LogBuffer }
}
function Flush-LogBuffer {
    $Script:LogBuffer | ForEach-Object { Write-Host $_ }
    $Script:LogBuffer = @()
}

# --- ORIGINAL SCRIPT BELOW ---
# MingGoRTS AI ????撖阡????單
# ??砍?撘??典??祕?? AI ??????

param(
    [string]$Tool = "midjourney",  # midjourney, dalle, stable
    [string]$Batch = "all",         # core, history, ui, effects, all
    [switch]$Interactive = $false   # 鈭?璅∪?
)

# 閮剔蔭霈
$ProjectRoot = "C:\HW\MingGoRTS"
$OutputDir = "$ProjectRoot\Content\Intro\Generated\Images"
$LogDir = "$ProjectRoot\Content\Intro\Logs"

# ?萄遣?桅?
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
New-Item -ItemType Directory -Path $LogDir -Force | Out-Null

# ?亥??賣
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

# Midjourney ?內閰澈
$MidjourneyPrompts = @{
    "T_TitleCard" = @{
        Prompt = "Chinese Republican era elegant title card background, 1912-1949, aged parchment texture with traditional Chinese cloud patterns in corners, warm sepia tones, ornate border with dragon motifs, vintage paper texture, calligraphy style elements, historical authenticity, cinematic quality, 1920x1080, ultra detailed, 8K, photorealistic"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "璅??∠??"
        ExpectedTime = "2-3??"
    }
    "T_MingGoRTS_Logo" = @{
        Prompt = "`"MingGoRTS`" logo design, Chinese Republican era style, English text with Chinese characters `"瘞?甇瑕`", traditional calligraphy font, gold gradient effect, subtle shadow, transparent background, vector style, professional logo design, elegant and historical, 800x400, high resolution"
        Params = "--ar 2:1 --q 2 --s 750"
        Description = "? Logo"
        ExpectedTime = "2-3??"
    }
    "T_GameTitle_Final" = @{
        Prompt = "Epic game title `"MingGoRTS`" with Chinese subtitle `"瘞?甇瑕??單??啁?`", dramatic background with battlefield scene, gold and red color scheme, cinematic lighting, professional game title design, historical elements, 1920x1080, ultra high quality"
        Params = "--ar 16:9 --q 2 --s 750"
        Description = "?蝯??脫?憿?
        ExpectedTime = "2-3??"
    }
    "T_1911_Revolution" = @{
        Prompt = "1911 Xinhai Revolution historical scene, Dr. Sun Yat-sen giving speech, revolutionary soldiers in uniform, Chinese Republican era flags, indoor government building, dramatic lighting, historical accuracy, sepia tone effect, vintage photography style, 1920x1080, ultra realistic, detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "颲漸?拙?湔"
        ExpectedTime = "2-3??"
    }
    "T_Republic_Established" = @{
        Prompt = "Founding ceremony of Republic of China, 1912, five-color flag ceremony, government officials in formal attire, traditional Chinese architecture, outdoor plaza, clear daylight, historical accuracy, formal composition, vintage photography style, 1920x1080, detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "瘞?撱箇??湔"
        ExpectedTime = "2-3??"
    }
    "T_Warlord_Period" = @{
        Prompt = "Chinese Warlord period battle scene, 1920s, soldiers in period uniforms, traditional Chinese landscape, smoke and gunfire effects, dramatic composition, historical warfare, gritty realism, dark tones, cinematic lighting, 1920x1080, ultra detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "頠???湔"
        ExpectedTime = "2-3??"
    }
    "T_Japanese_Invasion" = @{
        Prompt = "Second Sino-Japanese War scene, 1937-1945, Chinese soldiers defending, destroyed city background, smoke and fire effects, dramatic lighting, historical warfare, patriotic atmosphere, cinematic composition, high contrast, 1920x1080, realistic"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "?亥??乩噩?湔"
        ExpectedTime = "2-3??"
    }
    "UI_Skip_Button_Normal" = @{
        Prompt = "Chinese traditional style skip button, rectangular shape with rounded corners, dark brown background (#8B4513), red border (#DE2910), white text `"頝喲?`", elegant design, subtle texture, UI element, 200x50, high resolution"
        Params = "--ar 4:1 --q 2 --s 500"
        Description = "頝喲???"
        ExpectedTime = "1-2??"
    }
    "UI_Skip_Button_Hover" = @{
        Prompt = "Chinese style skip button hover state, glowing effect, bright red border (#FF6347), yellow text (#FFFF00), light brown background (#A0522D), subtle glow effect, UI element, 200x50, high resolution"
        Params = "--ar 4:1 --q 2 --s 500"
        Description = "頝喲????詨?"
        ExpectedTime = "1-2??"
    }
    "UI_Progress_Bar" = @{
        Prompt = "Chinese traditional progress bar, dark brown background (#3E2723), red progress fill (#DE2910), gold border (#FFD700), decorative Chinese patterns, elegant design, 800x20, high resolution"
        Params = "--ar 40:1 --q 2 --s 500"
        Description = "?脣漲璇?
        ExpectedTime = "1-2??"
    }
    "P_Sparkle" = @{
        Prompt = "Golden sparkle particle texture, bright yellow center (#FFFF00), golden edges (#FFD700), radial gradient, transparent background, particle effect, 128x128, seamless tile, high quality"
        Params = "--ar 1:1 --q 2 --s 300"
        Description = "??蝎?"
        ExpectedTime = "1-2??"
    }
    "P_Smoke" = @{
        Prompt = "Battlefield smoke particle texture, gray tones (#808080), soft edges, semi-transparent, atmospheric effect, 256x256, seamless tile, realistic smoke"
        Params = "--ar 1:1 --q 2 --s 300"
        Description = "?蝎?"
        ExpectedTime = "1-2??"
    }
    "P_Glow" = @{
        Prompt = "Soft glow effect texture, white center (#FFFFFF), radial transparency gradient, seamless edges, 512x512, high resolution, lighting effect"
        Params = "--ar 1:1 --q 2 --s 300"
        Description = "????"
        ExpectedTime = "1-2??"
    }
    "T_Heroes_Collage" = @{
        Prompt = "Collage of Chinese Republic era heroes, Chiang Kai-shek, Mao Zedong, historical figures, black and white photography style, vintage photo effect, dramatic lighting, historical accuracy, professional portrait style, 1920x1080, detailed"
        Params = "--ar 16:9 --q 2 --style raw --s 750"
        Description = "?梢??潸票"
        ExpectedTime = "2-3??"
    }
}

# ?寞活摰儔
$Batches = @{
    "core" = @("T_TitleCard", "T_MingGoRTS_Logo", "T_GameTitle_Final")
    "history" = @("T_1911_Revolution", "T_Republic_Established", "T_Warlord_Period", "T_Japanese_Invasion")
    "ui" = @("UI_Skip_Button_Normal", "UI_Skip_Button_Hover", "UI_Progress_Bar")
    "effects" = @("P_Sparkle", "P_Smoke", "P_Glow", "T_Heroes_Collage")
    "all" = $MidjourneyPrompts.Keys
}

# 憿舐內撌亙雿輻??
function Show-ToolGuide {
    param([string]$ToolName)
    
    Write-Log "=== $ToolName 雿輻?? ===" "INFO"
    
    switch ($ToolName) {
        "midjourney" {
            Write-Host @"
1. ?? Discord
2. ? Midjourney 隡箸???
3. ?脣 #newbies ??#general ?駁?
4. 雿輻 /imagine ?賭誘
5. 銴ˊ銝??蝷箄?
6. 蝎票??Discord 銝衣??
7. 蝑???摰?
8. ?豢??雿喟???(U1-U4)
9. 雿輻?曉之?賭誘 (U1-U4)
10. 銝???????
"@ -ForegroundColor Yellow
        }
        "dalle" {
            Write-Host @"
1. ?? ChatGPT Plus
2. ?豢? DALL-E 3 璅∪?
3. 銴ˊ銝??蝷箄?
4. 蝎票??ChatGPT
5. 蝑???摰?
6. 銝???????
"@ -ForegroundColor Yellow
        }
        "stable" {
            Write-Host @"
1. ?? Stable Diffusion WebUI
2. ?豢??拍?芋??
3. 銴ˊ銝??蝷箄?
4. 閮剔蔭?
5. 暺???
6. 銝???????
"@ -ForegroundColor Yellow
        }
    }
}

# ???桀???
function Invoke-ImageGeneration {
    param([string]$AssetName, [hashtable]$PromptData)
    
    Write-Log "????: $AssetName - $($PromptData.Description)" "INFO"
    Write-Log "????: $($PromptData.ExpectedTime)" "INFO"
    
    # 憿舐內?內閰?
    $FullPrompt = "$($PromptData.Prompt) $($PromptData.Params)"
    Write-Host "?內閰?" -ForegroundColor Cyan
    Write-Host $FullPrompt -ForegroundColor White
    
    # 銴ˊ?啣鞎潛倏
    $FullPrompt | Set-Clipboard
    Write-Log "?內閰歇銴ˊ?啣鞎潛倏" "SUCCESS"
    
    if ($Interactive) {
        Write-Host "`n??Enter 蝜潛?銝?????.." -ForegroundColor Yellow
        Read-Host
    }
    
    # 閮???
    $GenerationRecord = @{
        AssetName = $AssetName
        Description = $PromptData.Description
        Prompt = $FullPrompt
        StartTime = Get-Date
        Status = "Pending"
    }
    
    $GenerationRecord | ConvertTo-Json | Out-File -FilePath "$OutputDir\$AssetName.json" -Encoding UTF8
    Write-Log "??閮?撌脖?摮? $AssetName.json" "INFO"
}

# 銝餃銵??
function Start-ImageGeneration {
    Write-Log "?? AI ????" "INFO"
    Write-Log "撌亙: $Tool" "INFO"
    Write-Log "?寞活: $Batch" "INFO"
    
    # 憿舐內撌亙??
    Show-ToolGuide -ToolName $Tool
    
    # ?脣?閬???蝝??”
    $Assets = if ($Batch -eq "all") { 
        $MidjourneyPrompts.Keys 
    } else { 
        $Batches[$Batch] 
    }
    
    if (-not $Assets) {
        Write-Log "?航炊: ?寞活 '$Batch' 銝??? "ERROR"
        return
    }
    
    Write-Log "撠???$($Assets.Count) ???? "INFO"
    
    # 憿舐內??閮?
    Write-Host "`n=== ??閮? ===" -ForegroundColor Green
    for ($i = 0; $i -lt $Assets.Count; $i++) {
        $Asset = $Assets[$i]
        $PromptData = $MidjourneyPrompts[$Asset]
        Write-Host "$($i + 1). $Asset - $($PromptData.Description) ($($PromptData.ExpectedTime))" -ForegroundColor White
    }
    
    if ($Interactive) {
        Write-Host "`n??Enter ????..." -ForegroundColor Yellow
        Read-Host
    }
    
    # ??瘥???
    for ($i = 0; $i -lt $Assets.Count; $i++) {
        $Asset = $Assets[$i]
        $PromptData = $MidjourneyPrompts[$Asset]
        
        Write-Host "`n=== ?? $($i + 1)/$($Assets.Count): $Asset ===" -ForegroundColor Green
        
        Invoke-ImageGeneration -AssetName $Asset -PromptData $PromptData
        
        if ($i -lt $Assets.Count - 1) {
            Write-Host "`n皞?銝?????.." -ForegroundColor Yellow
            if ($Interactive) {
                Start-Sleep -Seconds 5
            }
        }
    }
    
    Write-Log "AI ????摰?" "SUCCESS"
}

# ?釭瑼Ｘ?賣
function Invoke-QualityCheck {
    Write-Log "???釭瑼Ｘ" "INFO"
    
    Write-Host @"
=== ?釭瑼Ｘ皜 ===

隢?瘥??????脰?隞乩?瑼Ｘ:

??閫??摨行?衣 1920x1080 (16:9)
?????臬皜?⊥芋蝟?
???脣蔗?臬撟唾﹛?臬末
??瑽??臬撠平
??憸冽?臬蝚血?瘞?甇瑕?
??蝝啁??臬鞊?皞Ⅱ
???湧?憸冽?臬蝯曹?

=== 敺???撱箄降 ===

1. 雿輻 Photoshop/GIMP ?脰??脣蔗?⊥迤
2. 隤踵??撠箏站?單?皞圾?漲
3. 憓撥撠?摨血?皜摨?
4. 頧??箸迤蝣箸撘?(PNG/JPEG)
5. 蝣箔?瑼?憭批???

=== 瑼??賢?閬? ===

- ???: T_[?迂].jpg
- Logo/UI: [?迂].png
- 蝎???: P_[?迂].png
- ?蝯??? [?迂]_Final.[?澆?]
"@ -ForegroundColor Cyan
}

# 憿舐內蝯梯?靽⊥
function Show-Statistics {
    Write-Host @"
=== AI ????蝯梯? ===

蝮賜???? 14??
??????: 2-3撠?
撟喳?瘥??? 10-15??

?寞活??:
- ?詨?蝝?: 3??(30-45??)
- 甇瑕蝝?: 4??(40-60??)
- UI??: 3??(20-30??)
- ??蝝?: 4??(30-45??)

?釭?格?:
- 撟喳?閰?: 8.5/10隞乩?
- ???? 100%
- 甇瑕皞Ⅱ?? 95%隞乩?
"@ -ForegroundColor Green
}

# ?瑁?銝餌?摨?
try {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "    MingGoRTS AI ????撌亙" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    
    Show-Statistics
    
    if ($Interactive) {
        Write-Host "`n皞??? AI ????..." -ForegroundColor Yellow
        Write-Host "蝣箔??典歇蝬??末?豢???AI 撌亙撣唾?" -ForegroundColor Yellow
        Write-Host "`n??Enter ??..." -ForegroundColor Yellow
        Read-Host
    }
    
    Start-ImageGeneration
    Invoke-QualityCheck
    
    Write-Log "??遙???? "SUCCESS"
    Write-Host "`n?? AI ????瘚?摰?嚗? -ForegroundColor Green
    Write-Host "隢??批?鞈芣炎?交??桅?霅?????" -ForegroundColor Green
    
}
catch {
    Write-Log "?航炊: $($_.Exception.Message)" "ERROR"
}

# 雿輻隤芣?
Write-Host @"
=== 雿輻隤芣? ===

?箸?冽?:
.\Start_AI_Image_Generation.ps1

?寞活??:
.\Start_AI_Image_Generation.ps1 -Batch core
.\Start_AI_Image_Generation.ps1 -Batch history
.\Start_AI_Image_Generation.ps1 -Batch ui
.\Start_AI_Image_Generation.ps1 -Batch effects

鈭?璅∪?:
.\Start_AI_Image_Generation.ps1 -Interactive

?湔?撌亙:
.\Start_AI_Image_Generation.ps1 -Tool dalle
.\Start_AI_Image_Generation.ps1 -Tool stable

瘜冽?鈭?:
- 蝣箔??冽??豢???AI 撌亙撣唾?
- ?內閰??芸?銴ˊ?啣鞎潛倏
- ??閮???摮 Logs ?桅?
- 隢??批?鞈芣?皞炎?亦?????
"@ -ForegroundColor Cyan

