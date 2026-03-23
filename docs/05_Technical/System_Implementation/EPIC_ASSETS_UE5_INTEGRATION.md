# MingGoRTS Epic 素材 UE5 整合指南

## 概述

本文檔說明如何將 Epic Asset Generator 生成的素材整合到 Unreal Engine 5 專案中。

## 目錄結構對應

### 素材輸出目錄
```
Content/EpicAssets/
├── epic1/                    # 歷史決策體驗基礎
│   ├── images/              # 貼圖、背景
│   ├── music/               # 音樂曲目
│   ├── sfx/                 # 音效
│   └── video/               # 影片
├── epic2/                   # 大規模戰術戰鬥系統
...
```

### UE5 Content Browser 結構
```
Content/
├── EpicAssets/             # 自動匯入目錄
│   ├── Epic1_Decision/     # 歷史決策素材
│   ├── Epic2_Battle/       # 戰鬥系統素材
│   ├── Epic3_Character/    # 角色系統素材
│   ├── Epic4_BaseBuilding/  # 基地建設素材
│   ├── Epic5_Strategy/     # 策略整合素材
│   └── Epic6_CrossPlatform/# 跨平台素材
```

## 素材類型與 UE5 資產類型對應

| 生成類型 | UE5 資產類型 | 匯入路徑 |
|----------|--------------|----------|
| images (.png/.jpg) | Texture2D | Content/EpicAssets/[Epic]/Textures/ |
| images (UI sprites) | Sprite/Texture2D | Content/EpicAssets/[Epic]/UI/ |
| music (.wav) | Sound Wave → Sound Cue | Content/EpicAssets/[Epic]/Audio/Music/ |
| sfx (.wav) | Sound Wave → Sound Cue | Content/EpicAssets/[Epic]/Audio/SFX/ |
| video (.mp4) | Media Source → Media Texture | Content/EpicAssets/[Epic]/Video/ |

## 自動匯入設定

### 1. 啟用 Editor Utility Blueprint

建立 `EUW_ImportEpicAssets` Editor Utility Widget:

```cpp
// 在 Content/EpicAssets/ 建立 Editor Utility Blueprint
// 命名: EUW_EpicAssetImporter

UFUNCTION(BlueprintCallable)
void ImportEpicAssets(FString EpicId, FString AssetType);

UFUNCTION(BlueprintCallable)
void BatchImportAllAssets();

UFUNCTION(BlueprintCallable)
void SetupAssetMetadata(UObject* Asset, FString EpicId, FString Category);
```

### 2. 匯入設定 JSON

建立 `Config/AssetImportSettings.json`:

```json
{
  "importSettings": {
    "texture": {
      "compression": "BC7",
      "lodGroup": "TEXTUREGROUP_UI",
      "srgb": true
    },
    "audio": {
      "compression": "BinkAudio",
      "sampleRate": 44100,
      "channels": "Auto"
    },
    "video": {
      "format": "H264",
      "preload": false
    }
  },
  "epicMappings": {
    "epic1": {
      "displayName": "Epic1_Decision",
      "plugin": "MingPersonal"
    },
    "epic2": {
      "displayName": "Epic2_Battle",
      "plugin": "MingTactical"
    },
    "epic3": {
      "displayName": "Epic3_Character",
      "plugin": "MingPersonal"
    },
    "epic4": {
      "displayName": "Epic4_BaseBuilding",
      "plugin": "MingBuilding"
    },
    "epic5": {
      "displayName": "Epic5_Strategy",
      "plugin": "MingStrategic"
    },
    "epic6": {
      "displayName": "Epic6_CrossPlatform",
      "plugin": "MingCore"
    }
  }
}
```

## 素材使用指南

### Epic 1: 歷史決策體驗基礎

#### 圖像素材
```cpp
// 在 MingPersonal Plugin 中使用
// 決策背景
UTexture2D* DecisionBackground = LoadObject<UTexture2D>(
    nullptr, 
    TEXT("/Game/EpicAssets/Epic1_Decision/Textures/decision_bg")
);

// 角色肖像
UTexture2D* CharacterPortrait = LoadObject<UTexture2D>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic1_Decision/Textures/historical_characters_01")
);
```

#### 音樂素材
```cpp
// 使用 Sound Cue
USoundCue* DecisionTheme = LoadObject<USoundCue>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic1_Decision/Audio/Music/decision_theme_Cue")
);

// 播放
UGameplayStatics::PlaySound2D(GetWorld(), DecisionTheme);
```

#### 音效素材
```cpp
// 使用 Sound Cue 帶有 Attenuation
USoundCue* DecisionConfirm = LoadObject<USoundCue>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic1_Decision/Audio/SFX/decision_confirm_Cue")
);

// 3D 音效
UGameplayStatics::PlaySoundAtLocation(
    GetWorld(), 
    DecisionConfirm, 
    PlayerLocation
);
```

### Epic 2: 大規模戰術戰鬥系統

#### 圖像素材
```cpp
// 戰場背景
UTexture2D* BattlefieldBG = LoadObject<UTexture2D>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic2_Battle/Textures/battlefield_tactical")
);

// 單位精靈
UPaperSprite* UnitSprite = LoadObject<UPaperSprite>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic2_Battle/Sprites/unit_sprites_01")
);
```

#### 音效素材
```cpp
// 戰鬥音效帶有隨機變體
USoundCue* Gunfire = LoadObject<USoundCue>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic2_Battle/Audio/SFX/gunfire_rifle_Cue")
);

// 在 Sound Cue 中設定 Random 節點使用 gunfire_rifle_1, _2, _3
```

### Epic 3: 角色成長與敘事系統

#### 圖像素材
```cpp
// 技能圖示
UTexture2D* SkillIcon = LoadObject<UTexture2D>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic3_Character/Icons/skill_icons_01")
);

// 在 UMG 中使用
SkillButton->SetBrushFromTexture(SkillIcon);
```

### Epic 4: 基地建設與資源管理

#### 圖像素材
```cpp
// 建築精靈（使用 Paper2D）
UPaperSprite* BuildingSprite = LoadObject<UPaperSprite>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic4_BaseBuilding/Sprites/building_sprites_01")
);

// 資源圖示
UTexture2D* ResourceIcon = LoadObject<UTexture2D>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic4_BaseBuilding/Icons/resource_icons_food")
);
```

### Epic 5: 四層策略整合系統

#### 圖像素材
```cpp
// 戰略地圖
UTexture2D* StrategicMap = LoadObject<UTexture2D>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic5_Strategy/Textures/strategic_map")
);

// 在 UMG 中使用
MapImage->SetBrushFromTexture(StrategicMap);
```

### Epic 6: 跨平台無縫體驗

#### 影片素材
```cpp
// 媒體播放器
UMediaPlayer* MediaPlayer = NewObject<UMediaPlayer>(this);
UMediaSource* VideoSource = LoadObject<UMediaSource>(
    nullptr,
    TEXT("/Game/EpicAssets/Epic6_CrossPlatform/Video/cross_platform_demo")
);

MediaPlayer->OpenSource(VideoSource);
MediaPlayer->Play();
```

## 自動化匯入腳本

### Python 匯入腳本

建立 `Tools/ue5/import_epic_assets.py`:

```python
import unreal
import json
import os

def import_texture(file_path, destination_path):
    """匯入貼圖"""
    task = unreal.AssetImportTask()
    task.set_editor_property('filename', file_path)
    task.set_editor_property('destination_path', destination_path)
    task.set_editor_property('replace_existing', True)
    task.set_editor_property('automated', True)
    
    # 貼圖設定
    texture_fact = unreal.TextureFactory()
    task.set_editor_property('factory', texture_fact)
    
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    return task

def import_audio(file_path, destination_path):
    """匯入音訊"""
    task = unreal.AssetImportTask()
    task.set_editor_property('filename', file_path)
    task.set_editor_property('destination_path', destination_path)
    task.set_editor_property('replace_existing', True)
    
    # 音訊設定
    sound_fact = unreal.SoundFactory()
    task.set_editor_property('factory', sound_fact)
    
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    return task

def create_sound_cue(sound_wave_path, destination_path, name):
    """建立 Sound Cue"""
    sound_wave = unreal.EditorAssetLibrary.load_asset(sound_wave_path)
    
    # 建立 Sound Cue
    factory = unreal.SoundCueFactoryNew()
    cue = factory.factory_create_new(
        destination_path + '/' + name,
        None,
        unreal.SoundCue
    )
    
    # 連接 Sound Wave 到 Output
    # 這裡需要使用 Blueprint 編輯 API 或手動設定
    
    return cue

def import_epic_assets(epic_id, project_root):
    """匯入特定 Epic 的所有素材"""
    epic_path = os.path.join(project_root, 'Content', 'EpicAssets', epic_id)
    
    if not os.path.exists(epic_path):
        unreal.log_warning(f"Epic path not found: {epic_path}")
        return
    
    # 匯入圖像
    images_path = os.path.join(epic_path, 'images')
    if os.path.exists(images_path):
        for file in os.listdir(images_path):
            if file.endswith(('.png', '.jpg', '.jpeg')):
                import_texture(
                    os.path.join(images_path, file),
                    f'/Game/EpicAssets/{epic_id}/Textures'
                )
    
    # 匯入音樂
    music_path = os.path.join(epic_path, 'music')
    if os.path.exists(music_path):
        for file in os.listdir(music_path):
            if file.endswith('.wav'):
                import_audio(
                    os.path.join(music_path, file),
                    f'/Game/EpicAssets/{epic_id}/Audio/Music'
                )
    
    # 匯入音效
    sfx_path = os.path.join(epic_path, 'sfx')
    if os.path.exists(sfx_path):
        for file in os.listdir(sfx_path):
            if file.endswith('.wav'):
                import_audio(
                    os.path.join(sfx_path, file),
                    f'/Game/EpicAssets/{epic_id}/Audio/SFX'
                )

# 執行
project_root = "C:/HW/MingGoRTS"
for epic in ['epic1', 'epic2', 'epic3', 'epic4', 'epic5', 'epic6']:
    import_epic_assets(epic, project_root)
```

## 使用 Editor Utility Widget

### 建立 EUW_EpicAssetManager

在 Content/EpicAssets/ 建立 Editor Utility Widget:

```cpp
// Blueprint 邏輯

// 事件: 生成素材
Event Generate Assets:
    - 執行 Python: Tools/ai/epic_asset_generator.py --epic [SelectedEpic]
    - 顯示進度條
    
// 事件: 匯入素材
Event Import Assets:
    - 執行 Python: Tools/ue5/import_epic_assets.py
    - 重新整理 Content Browser
    
// 事件: 建立 Sound Cue
Event Create Sound Cues:
    - 遍歷所有 Sound Wave
    - 為每個建立對應的 Sound Cue
    - 設定 Attenuation（如果是 SFX）
```

## 效能最佳化建議

### 圖像素材
1. **壓縮設定**
   - UI 元素: BC7, sRGB
   - 精靈圖: BC7 或 DXT5 (需要 Alpha)
   - 背景: BC7, sRGB

2. **LOD 設定**
   - 使用 Texture Groups
   - UI: TEXTUREGROUP_UI
   - World: TEXTUREGROUP_World

3. **Streaming**
   - 大型背景使用 Texture Streaming
   - 設定適當的 Mip 層級

### 音訊素材
1. **壓縮格式**
   - 音樂: BinkAudio (壓縮比高)
   - SFX: ADPCM (低延遲)

2. **載入策略**
   - 音樂: Streaming
   - UI SFX: Preload
   - 3D SFX: On Demand

3. **Attenuation**
   - 3D 音效使用 Spherical Attenuation
   - UI 音效使用 2D (無 Attenuation)

### 影片素材
1. **播放設定**
   - 使用 Media Framework
   - 設定適當的 Buffer 大小
   - 支援硬體解碼

2. **記憶體管理**
   - 非使用時釋放 Media Texture
   - 使用 Media Player 的 Close 方法

## 測試檢查清單

### 素材匯入測試
- [ ] 所有圖像正確顯示
- [ ] 音訊正確播放
- [ ] Sound Cue 設定正確
- [ ] 影片正常播放

### 功能測試
- [ ] UI 中使用圖像素材
- [ ] 遊戲中播放音樂
- [ ] 觸發音效正確
- [ ] 影片播放器工作正常

### 效能測試
- [ ] 記憶體使用合理
- [ ] 載入時間可接受
- [ ] 無記憶體洩漏

## 常見問題

### Q: 素材匯入後顯示錯誤顏色
A: 檢查 Texture 的 sRGB 設定，UI 素材應該啟用 sRGB。

### Q: 音效音量不一致
A: 在 Sound Cue 中使用 Modulator 節點，或在 Sound Wave 中標準化音量。

### Q: 影片播放卡頓
A: 使用 Media Framework 的 Streaming 模式，或降低影片解析度。

### Q: 自動匯入失敗
A: 檢查檔案路徑是否正確，並確保 UE5 Editor 已開啟。

## 附錄

### 快捷命令參考

```powershell
# 生成所有素材
python Tools/ai/epic_asset_generator.py --all

# 匯入到 UE5 (在 Editor 中執行 Python)
py Tools/ue5/import_epic_assets.py

# 批次生成特定 Epic
python Tools/ai/epic_asset_generator.py --epic epic2 --type music

# 清理並重新生成
python Tools/ai/epic_asset_generator.py --clean
python Tools/ai/epic_asset_generator.py --all --force
```

---

*文件生成時間: 2026-03-22*
*版本: 1.0*
*適用 UE5 版本: 5.4+*
