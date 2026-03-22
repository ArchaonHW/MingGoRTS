# MingGoRTS AI 蝝?敹恍????
# 雿輻 PowerShell ?芸???AI 蝝???瘚?

param(
    [string]$Tool = "midjourney",  # midjourney, dalle, suno
    [string]$Batch = "core",       # core, history, ui, effects
    [switch]$Test = $false         # 皜祈岫璅∪?
)

# 閮剔蔭霈
$ProjectRoot = "C:\HW\MingGoRTS"
$OutputDir = "$ProjectRoot\Content\Intro\Generated"
$LogDir = "$ProjectRoot\Content\Intro\Logs"

# ?萄遣?桅?
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
New-Item -ItemType Directory -Path $LogDir -Force | Out-Null

# ?亥??賣
function Write-Log {
    param([string]$Message)
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    Write-Host "[$Timestamp] $Message" -ForegroundColor Green
    Add-Content -Path "$LogDir\Generation_$(Get-Date -Format 'yyyyMMdd').log" -Value "[$Timestamp] $Message"
}

# Midjourney ?內閰澈
$MidjourneyPrompts = @{
    "T_TitleCard" = "Chinese Republican era elegant title card background, 1912-1949, aged parchment texture with traditional Chinese cloud patterns in corners, warm sepia tones, ornate border with dragon motifs, vintage paper texture, calligraphy style elements, historical authenticity, cinematic quality, 1920x1080, ultra detailed, 8K, photorealistic --ar 16:9 --q 2 --style raw"
    "T_MingGoRTS_Logo" = "'MingGoRTS' logo design, Chinese Republican era style, English text with Chinese characters '瘞?甇瑕', traditional calligraphy font, gold gradient effect, subtle shadow, transparent background, vector style, professional logo design, elegant and historical, 800x400, high resolution --ar 2:1 --q 2"
    "T_GameTitle_Final" = "Epic game title 'MingGoRTS' with Chinese subtitle '瘞?甇瑕??單??啁?', dramatic background with battlefield scene, gold and red color scheme, cinematic lighting, professional game title design, historical elements, 1920x1080, ultra high quality --ar 16:9 --q 2"
    "T_1911_Revolution" = "1911 Xinhai Revolution historical scene, Dr. Sun Yat-sen giving speech, revolutionary soldiers in uniform, Chinese Republican era flags, indoor government building, dramatic lighting, historical accuracy, sepia tone effect, vintage photography style, 1920x1080, ultra realistic, detailed --ar 16:9 --q 2"
    "T_Republic_Established" = "Founding ceremony of Republic of China, 1912, five-color flag ceremony, government officials in formal attire, traditional Chinese architecture, outdoor plaza, clear daylight, historical accuracy, formal composition, vintage photography style, 1920x1080, detailed --ar 16:9 --q 2"
    "T_Warlord_Period" = "Chinese Warlord period battle scene, 1920s, soldiers in period uniforms, traditional Chinese landscape, smoke and gunfire effects, dramatic composition, historical warfare, gritty realism, dark tones, cinematic lighting, 1920x1080, ultra detailed --ar 16:9 --q 2"
    "T_Japanese_Invasion" = "Second Sino-Japanese War scene, 1937-1945, Chinese soldiers defending, destroyed city background, smoke and fire effects, dramatic lighting, historical warfare, patriotic atmosphere, cinematic composition, high contrast, 1920x1080, realistic --ar 16:9 --q 2"
}

# DALL-E 3 ?內閰澈
$DallePrompts = @{
    "T_TitleCard" = "Chinese Republican era elegant title card background, 1912-1949, aged parchment texture with traditional Chinese cloud patterns in corners, warm sepia tones, ornate border with dragon motifs, vintage paper texture, calligraphy style elements, historical authenticity, cinematic quality, 1920x1080, ultra detailed, 8K, photorealistic"
    "T_MingGoRTS_Logo" = "'MingGoRTS' logo design, Chinese Republican era style, English text with Chinese characters '瘞?甇瑕', traditional calligraphy font, gold gradient effect, subtle shadow, transparent background, vector style, professional logo design, elegant and historical, 800x400, high resolution"
    "T_GameTitle_Final" = "Epic game title 'MingGoRTS' with Chinese subtitle '瘞?甇瑕??單??啁?', dramatic background with battlefield scene, gold and red color scheme, cinematic lighting, professional game title design, historical elements, 1920x1080, ultra high quality"
}

# Suno AI ?單??內閰澈
$SunoPrompts = @{
    "IntroMusic_MainTheme" = "Chinese Republican era military theme music, 12 seconds duration, 120 BPM, majestic brass fanfare, traditional Chinese instruments mixed with Western military band, solemn and patriotic atmosphere, dramatic orchestration, historical authenticity, high quality audio"
    "GameTitle_Fanfare" = "Chinese Republican era fanfare, 4 seconds, powerful trumpet calls, military brass section, dramatic crescendo, heroic and glorious, traditional Chinese musical elements, cinematic impact, professional fanfare composition"
    "HistoryBackground_Ambience" = "Chinese Republican era historical background ambience, 5 seconds, vintage photograph page turning sounds, historical narration voice texture, subtle wind sounds, nostalgic atmosphere, aged audio quality, documentary style"
    "UI_Click" = "Traditional Chinese UI click sound, 0.2 seconds, bamboo clapper sound, wooden fish percussion, clean and crisp, traditional Chinese audio aesthetic, gentle and elegant, high quality recording"
    "UI_Hover" = "Chinese traditional UI hover sound, 0.1 seconds, gentle bell chime, silk string pluck, soft and elegant, traditional Chinese sound design, subtle and refined, high quality"
    "Transition_Swoosh" = "Smooth transition swoosh sound, 0.5 seconds, gentle wind whoosh, cinematic transition effect, fluid and elegant, professional sound design, high quality audio"
}

# ?寞活摰儔
$Batches = @{
    "core" = @("T_TitleCard", "T_MingGoRTS_Logo", "T_GameTitle_Final", "IntroMusic_MainTheme", "GameTitle_Fanfare")
    "history" = @("T_1911_Revolution", "T_Republic_Established", "T_Warlord_Period", "T_Japanese_Invasion", "HistoryBackground_Ambience")
    "ui" = @("UI_Click", "UI_Hover", "Transition_Swoosh")
    "effects" = @("P_Sparkle", "P_Smoke", "P_Glow", "T_Heroes_Collage")
}

# ???賣
function Invoke-MidjourneyGeneration {
    param([string]$Prompt, [string]$Name)
    
    Write-Log "?? Midjourney ??: $Name"
    
    if ($Test) {
        Write-Log "皜祈岫璅∪?: $Prompt"
        return
    }
    
    # ?ㄐ?閬祕?? Midjourney API 隤輻
    # ?桀??箸芋?砍祕??
    $Prompt | Out-File -FilePath "$OutputDir\midjourney_$Name.txt" -Encoding UTF8
    Write-Log "Midjourney ?內閰歇靽?: $Name"
    
    # 璅⊥蝑???
    Start-Sleep -Seconds 30
    
    Write-Log "Midjourney ??摰?: $Name"
}

function Invoke-DalleGeneration {
    param([string]$Prompt, [string]$Name)
    
    Write-Log "?? DALL-E 3 ??: $Name"
    
    if ($Test) {
        Write-Log "皜祈岫璅∪?: $Prompt"
        return
    }
    
    # ?ㄐ?閬祕?? DALL-E API 隤輻
    # ?桀??箸芋?砍祕??
    $Prompt | Out-File -FilePath "$OutputDir\dalle_$Name.txt" -Encoding UTF8
    Write-Log "DALL-E ?內閰歇靽?: $Name"
    
    # 璅⊥蝑???
    Start-Sleep -Seconds 20
    
    Write-Log "DALL-E ??摰?: $Name"
}

function Invoke-SunoGeneration {
    param([string]$Prompt, [string]$Name)
    
    Write-Log "?? Suno AI ??: $Name"
    
    if ($Test) {
        Write-Log "皜祈岫璅∪?: $Prompt"
        return
    }
    
    # ?ㄐ?閬祕?? Suno AI API 隤輻
    # ?桀??箸芋?砍祕??
    $Prompt | Out-File -FilePath "$OutputDir\suno_$Name.txt" -Encoding UTF8
    Write-Log "Suno AI ?內閰歇靽?: $Name"
    
    # 璅⊥蝑???
    Start-Sleep -Seconds 45
    
    Write-Log "Suno AI ??摰?: $Name"
}

# 銝餃銵??
function Start-AIGeneration {
    Write-Log "?? AI 蝝???"
    Write-Log "撌亙: $Tool"
    Write-Log "?寞活: $Batch"
    
    # ?脣??嗅??寞活????銵?
    $Assets = $Batches[$Batch]
    if (-not $Assets) {
        Write-Log "?航炊: ?寞活 '$Batch' 銝???
        return
    }
    
    Write-Log "撠???$($Assets.Count) ????
    
    # ?寞?撌亙?豢??內閰澈
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
            Write-Log "?航炊: 銝?渡?撌亙 '$Tool'"
            return
        }
    }
    
    # ??瘥???
    foreach ($Asset in $Assets) {
        $Prompt = $Prompts[$Asset]
        if (-not $Prompt) {
            Write-Log "霅血?: 蝝? '$Asset' ??蝷箄?銝???
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
        
        # ?寞活??
        if (-not $Test) {
            Start-Sleep -Seconds 10
        }
    }
    
    Write-Log "AI 蝝???摰?"
}

# ?釭瑼Ｘ?賣
function Invoke-QualityCheck {
    Write-Log "???釭瑼Ｘ"
    
    $GeneratedFiles = Get-ChildItem -Path $OutputDir -Filter "*.png" -ErrorAction SilentlyContinue
    
    foreach ($File in $GeneratedFiles) {
        $Size = Get-ImageSize -Path $File.FullName
        Write-Log "瑼Ｘ瑼?: $($File.Name) - 撠箏站: $($Size.Width)x$($Size.Height)"
    }
    
    Write-Log "?釭瑼Ｘ摰?"
}

# ?脣???撠箏站?賣 (璅⊥)
function Get-ImageSize {
    param([string]$Path)
    return @{ Width = 1920; Height = 1080 }
}

# ?瑁?銝餌?摨?
try {
    Start-AIGeneration
    
    if (-not $Test) {
        Invoke-QualityCheck
    }
    
    Write-Log "??遙????
}
catch {
    Write-Log "?航炊: $($_.Exception.Message)"
}

# 雿輻隤芣?
Write-Host @"
雿輻隤芣?:
1. 皜祈岫璅∪?: .\Quick_Generation_Script.ps1 -Test
2. ???詨??寞活: .\Quick_Generation_Script.ps1 -Tool midjourney -Batch core
3. ??甇瑕?寞活: .\Quick_Generation_Script.ps1 -Tool dalle -Batch history
4. ???單??寞活: .\Quick_Generation_Script.ps1 -Tool suno -Batch ui

?舀?極??
- midjourney: ????
- dalle: ????
- suno: ?單???

?舀?甈?
- core: ?詨?蝝?
- history: 甇瑕蝝?
- ui: UI ??
- effects: ??蝝?
"@ -ForegroundColor Cyan

