# 至聖者角色生成腳本
# 用於快速生成和配置至聖者角色

param(
    [Parameter(Mandatory=$true)]
    [string]$CharacterName,
    
    [Parameter(Mandatory=$true)]
    [ValidateSet("Sage", "DemonKing", "PseudoSage")]
    [string]$CharacterType,
    
    [int]$Level = 1,
    
    [string]$OutputPath = "c:\HW\MingGoRTS\Generated\Characters"
)

Write-Host "=== 至聖者角色生成器 ===" -ForegroundColor Green
Write-Host "角色名稱: $CharacterName" -ForegroundColor Cyan
Write-Host "角色類型: $CharacterType" -ForegroundColor Cyan
Write-Host "初始等級: $Level" -ForegroundColor Cyan

# 檢查輸出路徑
if (-not (Test-Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath -Force | Out-Null
    Write-Host "✓ 創建輸出目錄: $OutputPath" -ForegroundColor Green
}

# 角色類型配置
$CharacterConfigs = @{
    "Sage" = @{
        DisplayName = "聖者"
        Description = "至聖者能用而能停，正邪皆兵而不墮。正逆策略切換無懲罰，是最平衡的指揮者。"
        InitialSpiritualPower = 120
        MaxSpiritualPower = 120
        InitialWisdom = 10
        AuraRange = 600
        Skills = @("基本冥想", "靈力感知", "陰陽調和", "聖者光環")
        Attributes = @{
            SpiritualPower = 1.2
            Wisdom = 1.2
            Charisma = 1.0
            Constitution = 0.8
            Luck = 0.8
        }
        Recommendations = @(
            "平衡使用正逆策略",
            "專注於陰陽五行和諧",
            "建立廣泛的關係網絡",
            "適合外交和解決衝突"
        )
    }
    "DemonKing" = @{
        DisplayName = "魔王"
        Description = "魔王能用邪而不能停，逆策效果+50%但無法停止。一旦踏上邪道，便無法回頭。"
        InitialSpiritualPower = 80
        MaxSpiritualPower = 80
        InitialWisdom = 5
        AuraRange = 400
        Skills = @("基本冥想", "靈力感知", "逆策強化", "魔王威懾")
        Attributes = @{
            SpiritualPower = 1.0
            Wisdom = 0.8
            Charisma = 0.6
            Constitution = 1.4
            Luck = 1.2
        }
        Recommendations = @(
            "積極使用逆策獲得強大效果",
            "接受墮落風險追求極限力量",
            "專注於戰鬥和征服",
            "建立恐懼為基礎的統治"
        )
    }
    "PseudoSage" = @{
        DisplayName = "偽聖者"
        Description = "偽聖者不能用而假裝不用，正策+20%，外交+30%，但無法使用逆策。表面清高，實則無能。"
        InitialSpiritualPower = 100
        MaxSpiritualPower = 100
        InitialWisdom = 8
        AuraRange = 500
        Skills = @("基本冥想", "靈力感知", "外交強化", "正策專精")
        Attributes = @{
            SpiritualPower = 1.1
            Wisdom = 1.4
            Charisma = 1.3
            Constitution = 0.9
            Luck = 1.0
        }
        Recommendations = @(
            "專注於正策和外交",
            "建立強大的聯盟網絡",
            "發展經濟和文化",
            "避免直接衝突"
        )
    }
}

# 獲取角色配置
$Config = $CharacterConfigs[$CharacterType]
if (-not $Config) {
    Write-Host "錯誤：無效的角色類型 $CharacterType" -ForegroundColor Red
    exit 1
}

Write-Host "`n=== 角色信息 ===" -ForegroundColor Yellow
Write-Host "顯示名稱: $($Config.DisplayName)" -ForegroundColor White
Write-Host "描述: $($Config.Description)" -ForegroundColor White
Write-Host "初始靈力: $($Config.InitialSpiritualPower)/$($Config.MaxSpiritualPower)" -ForegroundColor White
Write-Host "初始智慧: $($Config.InitialWisdom)" -ForegroundColor White
Write-Host "光環範圍: $($Config.AuraRange)" -ForegroundColor White

# 生成角色數據
$CharacterData = @{
    Name = $CharacterName
    Type = $CharacterType
    DisplayName = $Config.DisplayName
    Description = $Config.Description
    Level = $Level
    Experience = 0
    SpiritualPower = $Config.InitialSpiritualPower
    MaxSpiritualPower = $Config.MaxSpiritualPower
    Wisdom = $Config.InitialWisdom
    AuraRange = $Config.AuraRange
    Skills = $Config.Skills
    Attributes = $Config.Attributes
    Recommendations = $Config.Recommendations
    FallValue = 0
    IsFallen = $false
    Relationships = @{}
    Reputation = @{}
    Achievements = @()
    History = @("角色 $CharacterName 誕生了")
    CreatedDate = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
}

# 計算等級加成
if ($Level -gt 1) {
    $LevelBonus = ($Level - 1) * 20
    $CharacterData.SpiritualPower += $LevelBonus
    $CharacterData.MaxSpiritualPower += $LevelBonus
    $CharacterData.Wisdom += ($Level - 1) * 2
    $CharacterData.AuraRange += ($Level - 1) * 50
    Write-Host "✓ 應用等級加成 (等級 $Level)" -ForegroundColor Green
}

# 生成角色文件
$CharacterJson = $CharacterData | ConvertTo-Json -Depth 10
$CharacterFile = Join-Path $OutputPath "$CharacterName.json"
$CharacterJson | Out-File -FilePath $CharacterFile -Encoding UTF8
Write-Host "✓ 角色文件已保存: $CharacterFile" -ForegroundColor Green

# 生成角色配置文件
$ConfigContent = @"
// 角色配置文件 - $CharacterName
// 生成時間: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')

#pragma once

#include "CoreMinimal.h"
#include "SageCommand/MingSageCharacterSystem.h"

// $CharacterName 角色配置
namespace $CharacterName`Config
{
    // 基本信息
    const FString CharacterName = TEXT("$CharacterName");
    const ESageCharacterType CharacterType = ESageCharacterType::$CharacterType;
    const int32 InitialLevel = $Level;
    
    // 初始屬性
    const float InitialSpiritualPower = $($Config.InitialSpiritPower);
    const float MaxSpiritualPower = $($Config.MaxSpiritualPower);
    const int32 InitialWisdom = $($Config.InitialWisdom);
    const float AuraRange = $($Config.AuraRange);
    
    // 初始技能
    const TArray<FString> InitialSkills = {
"@

foreach ($Skill in $Config.Skills) {
    $ConfigContent += "        TEXT(`"$Skill`"),`n"
}

$ConfigContent += @"
    };
    
    // 屬性倍率
    const struct Attributes {
        float SpiritualPower = $($Config.Attributes.SpiritualPower);
        float Wisdom = $($Config.Attributes.Wisdom);
        float Charisma = $($Config.Attributes.Charisma);
        float Constitution = $($Config.Attributes.Constitution);
        float Luck = $($Config.Attributes.Luck);
    } AttributeMultipliers;
}
"@

$ConfigFile = Join-Path $OutputPath "$CharacterName`Config.h"
$ConfigContent | Out-File -FilePath $ConfigFile -Encoding UTF8
Write-Host "✓ 配置文件已保存: $ConfigFile" -ForegroundColor Green

# 生成角色建議文件
$RecommendationsContent = @"
# $CharacterName 角色建議指南

## 角色概述
**名稱**: $CharacterName  
**類型**: $($Config.DisplayName)  
**等級**: $Level  
**創建時間**: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')

## 角色描述
$($Config.Description)

## 初始屬性
- **靈力**: $($Config.InitialSpiritualPower)/$($Config.MaxSpiritualPower)
- **智慧**: $($Config.InitialWisdom)
- **光環範圍**: $($Config.AuraRange)
- **技能**: $($Config.Skills -join ', ')

## 發展建議
"@

for ($i = 0; $i -lt $Config.Recommendations.Count; $i++) {
    $RecommendationsContent += "$($i + 1). $($Config.Recommendations[$i])`n"
}

$RecommendationsContent += @"

## 等級發展路徑

### 等級 $Level-10
- 學習並掌握初始技能
- 建立基本的關係網絡
- 熟悉角色機制和玩法

### 等級 11-25
- 解鎖進階技能
- 擴大影響範圍
- 建立穩固的聯盟關係

### 等級 26-50
- 掌握高級能力
- 成為區域強權
- 引導重要決策

### 等級 51+
- 成為傳說級人物
- 創造永恆傳承
- 引導世界走向

## 技能發展建議

### 優先技能
1. **基本冥想** - 必需的基礎技能
2. **靈力感知** - 重要的輔助技能
3. **陰陽調和** - 核心平衡技能
4. **角色特色技能** - 根據角色類型選擇

### 技能組合推薦
- **防守型**: 基本冥想 + 靈力感知 + 防護技能
- **攻擊型**: 靈力感知 + 攻擊技能 + 強化技能
- **平衡型**: 基本冥想 + 陰陽調和 + 靈活技能

## 關係建立建議

### 初期關係目標
- 建立至少 3-5 個友好關係
- 避免過早樹敵
- 專注於互惠互利的關係

### 中期關係目標
- 發展 2-3 個核心盟友
- 建立穩固的聯盟網絡
- 平衡各方關係

### 長期關係目標
- 成為關係網絡的中心
- 建立永恆的友誼
- 引導關係走向

## 注意事項

1. **墮落風險**: 特別注意墮落值的變化
2. **策略平衡**: 根據角色類型調整策略
3. **資源管理**: 合理管理靈力和資源
4. **時機把握**: 把握關鍵時機做出決策

## 成功指標

- **等級達成**: 按時達成目標等級
- **技能掌握**: 掌握所有必要技能
- **關係建立**: 建立穩固的關係網絡
- **成就解鎖**: 解鎖相關成就

---

*此建議指南基於角色初始配置生成，請根據實際遊戲情況調整策略*
"@

$RecommendationsFile = Join-Path $OutputPath "$CharacterName`Recommendations.md"
$RecommendationsContent | Out-File -FilePath $RecommendationsFile -Encoding UTF8
Write-Host "✓ 建議文件已保存: $RecommendationsFile" -ForegroundColor Green

# 生成角色統計文件
$StatsContent = @{
    CharacterName = $CharacterName
    CharacterType = $CharacterType
    DisplayName = $Config.DisplayName
    Level = $Level
    CreatedDate = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    InitialStats = @{
        SpiritualPower = $CharacterData.SpiritualPower
        MaxSpiritualPower = $CharacterData.MaxSpiritualPower
        Wisdom = $CharacterData.Wisdom
        AuraRange = $CharacterData.AuraRange
    }
    Skills = $Config.Skills
    Attributes = $Config.Attributes
    Recommendations = $Config.Recommendations
    PowerRating = [math]::Round(($CharacterData.SpiritualPower + $CharacterData.Wisdom * 10 + $Config.Skills.Count * 5) * 1.0, 2)
}

$StatsJson = $StatsContent | ConvertTo-Json -Depth 10
$StatsFile = Join-Path $OutputPath "$CharacterName`Stats.json"
$StatsJson | Out-File -FilePath $StatsFile -Encoding UTF8
Write-Host "✓ 統計文件已保存: $StatsFile" -ForegroundColor Green

# 生成摘要報告
Write-Host "`n=== 角色生成摘要 ===" -ForegroundColor Yellow
Write-Host "角色名稱: $CharacterName" -ForegroundColor White
Write-Host "角色類型: $($Config.DisplayName)" -ForegroundColor White
Write-Host "初始等級: $Level" -ForegroundColor White
Write-Host "能力評分: $($StatsContent.PowerRating)" -ForegroundColor White
Write-Host "技能數量: $($Config.Skills.Count)" -ForegroundColor White
Write-Host "建議數量: $($Config.Recommendations.Count)" -ForegroundColor White

Write-Host "`n=== 生成文件 ===" -ForegroundColor Yellow
Write-Host "角色數據: $CharacterFile" -ForegroundColor Gray
Write-Host "配置文件: $ConfigFile" -ForegroundColor Gray
Write-Host "建議指南: $RecommendationsFile" -ForegroundColor Gray
Write-Host "統計數據: $StatsFile" -ForegroundColor Gray

Write-Host "`n=== 使用建議 ===" -ForegroundColor Yellow
Write-Host "1. 將配置文件集成到 UE5 項目中" -ForegroundColor White
Write-Host "2. 參考建議指南制定發展策略" -ForegroundColor White
Write-Host "3. 根據統計數據調整遊戲平衡" -ForegroundColor White
Write-Host "4. 定期更新角色數據文件" -ForegroundColor White

Write-Host "`n✓ 角色 $CharacterName 生成完成！" -ForegroundColor Green
Write-Host "祝您遊戲愉快！" -ForegroundColor Cyan

# 可選：打開輸出目錄
$OpenFolder = Read-Host "`n是否打開輸出目錄？(Y/N)"
if ($OpenFolder -eq "Y" -or $OpenFolder -eq "y") {
    Start-Process "explorer.exe" -ArgumentList $OutputPath
}
