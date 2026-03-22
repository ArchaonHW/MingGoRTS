# MingGoRTS Epic 素材生成 - 快速參考

## 🚀 快速開始

### 1. 初始化系統（只需一次）
```powershell
.\Tools\ai\epic_assets_generate.ps1 -Init
```

### 2. 生成所有素材
```powershell
.\Tools\ai\epic_assets_generate.ps1 -All
```

### 3. 查看素材清單
```powershell
.\Tools\ai\epic_assets_generate.ps1 -List
```

---

## 📊 素材總覽

| Epic | 名稱 | 圖像 | 音樂 | 音效 | 影片 | 總計 |
|------|------|:----:|:----:|:----:|:----:|:----:|
| Epic 1 | 歷史決策體驗基礎 | 9 | 2 | 3 | 1 | 15 |
| Epic 2 | 大規模戰術戰鬥系統 | 17 | 3 | 11 | 1 | 32 |
| Epic 3 | 角色成長與敘事系統 | 33 | 3 | 4 | 1 | 41 |
| Epic 4 | 基地建設與資源管理 | 21 | 3 | 10 | 1 | 35 |
| Epic 5 | 四層策略整合系統 | 17 | 3 | 4 | 1 | 25 |
| Epic 6 | 跨平台無縫體驗 | 4 | 2 | 3 | 1 | 10 |
| **總計** | | **101** | **16** | **35** | **6** | **158** |

---

## 🎯 常用命令

### 生成特定 Epic
```powershell
# Epic 1: 歷史決策
.\Tools\ai\epic_assets_generate.ps1 -Epic epic1

# Epic 2: 戰鬥系統
.\Tools\ai\epic_assets_generate.ps1 -Epic epic2

# Epic 3: 角色系統
.\Tools\ai\epic_assets_generate.ps1 -Epic epic3
```

### 生成特定類型
```powershell
# 只生成圖像
.\Tools\ai\epic_assets_generate.ps1 -All -Type images

# 只生成音樂
.\Tools\ai\epic_assets_generate.ps1 -All -Type music

# 只生成音效
.\Tools\ai\epic_assets_generate.ps1 -Epic epic2 -Type sfx
```

### 進階選項
```powershell
# 強制重新生成（覆蓋現有）
.\Tools\ai\epic_assets_generate.ps1 -Epic epic1 -Force

# 靜默模式（無確認提示）
.\Tools\ai\epic_assets_generate.ps1 -All -Silent

# 清理所有素材
.\Tools\ai\epic_assets_generate.ps1 -Clean
```

---

## 📁 輸出目錄結構

```
Content/EpicAssets/
├── epic1/                          # 歷史決策體驗基礎
│   ├── images/                     # 9 個圖像
│   │   ├── decision_bg.png
│   │   ├── historical_characters_1.png ... _5.png
│   │   └── event_triggers_1.png ... _3.png
│   ├── music/                      # 2 首音樂
│   │   ├── decision_theme.wav
│   │   └── historical_event.wav
│   ├── sfx/                        # 3 個音效
│   │   ├── decision_confirm.wav
│   │   ├── event_trigger.wav
│   │   └── paper_rustle.wav
│   └── video/                      # 1 個影片
│       └── intro_cinematic.mp4
├── epic2/                          # 大規模戰術戰鬥系統
│   ├── images/                     # 17 個圖像
│   ├── music/                      # 3 首音樂
│   ├── sfx/                        # 11 個音效
│   └── video/                      # 1 個影片
├── epic3/                          # 角色成長與敘事系統
│   ├── images/                     # 33 個圖像
│   ├── music/                      # 3 首音樂
│   ├── sfx/                        # 4 個音效
│   └── video/                      # 1 個影片
├── epic4/                          # 基地建設與資源管理
│   ├── images/                     # 21 個圖像
│   ├── music/                      # 3 首音樂
│   ├── sfx/                        # 10 個音效
│   └── video/                      # 1 個影片
├── epic5/                          # 四層策略整合系統
│   ├── images/                     # 17 個圖像
│   ├── music/                      # 3 首音樂
│   ├── sfx/                        # 4 個音效
│   └── video/                      # 1 個影片
└── epic6/                          # 跨平台無縫體驗
    ├── images/                     # 4 個圖像
    ├── music/                      # 2 首音樂
    ├── sfx/                        # 3 個音效
    └── video/                      # 1 個影片
```

---

## 🎨 AI 生成工具建議

### 圖像生成
| 工具 | 用途 | 推薦參數 |
|------|------|----------|
| **Stable Diffusion** | 主要工具 | 1024x1024, CFG 7-8, Steps 30-50 |
| Midjourney | 備選 | --ar 16:9 --v 6 --style raw |
| DALL-E 3 | 快速原型 | 1024x1024, vivid style |

### 音樂生成
| 工具 | 用途 | 推薦參數 |
|------|------|----------|
| **MusicGen** | 主要工具 | 32kHz, 30-120s |
| AIVA | 備選 | Classical/Chinese preset |
| Suno | 快速原型 | 120 BPM, Epic cinematic |

### 音效生成
| 工具 | 用途 | 推薦參數 |
|------|------|----------|
| **ElevenLabs Sound Effects** | 主要工具 | High quality |
| Freesound + AI | 備選 | 混合編輯 |
| Splice | 商業 | 專業音效庫 |

### 影片生成
| 工具 | 用途 | 推薦參數 |
|------|------|----------|
| **Runway Gen-2** | 主要工具 | 1080p, 24fps |
| Pika Labs | 備選 | Cinematic style |
| HeyGen | 角色影片 | AI avatar |

---

## 📝 風格提示詞關鍵詞

### 民國時期風格
```
Chinese Republican era 1912-1949
Vintage photography style
Sepia tone / Film noir atmosphere
Cinematic documentary quality
Historical authenticity
Military and political setting
```

### 音樂風格
```
Chinese traditional instruments (erhu, pipa, guzheng)
Western orchestra hybrid
Military brass and percussion
Republican era documentary style
Epic and cinematic
```

---

## 🔧 配置檔案

### 主配置
- **路徑**: `Tools/ai/epic_assets_config.json`
- **用途**: 定義所有 Epic 的素材規格

### 進度追蹤
- **路徑**: `Tools/ai/epic_generation_progress.json`
- **用途**: 記錄生成進度，避免重複生成

---

## 📚 詳細文件

| 文件 | 路徑 | 內容 |
|------|------|------|
| **素材規劃** | `docs/EPIC_ASSETS_GENERATION_PLAN.md` | 完整素材規格與生成計劃 |
| **UE5 整合** | `docs/EPIC_ASSETS_UE5_INTEGRATION.md` | UE5 匯入與使用指南 |
| **快速參考** | `docs/EPIC_ASSETS_QUICK_REF.md` | 本文檔 |

---

## ⚡ 疑難排解

### 問題: Python 未安裝
**解決**: 安裝 Python 3.8+
```powershell
# 檢查 Python 版本
python --version
```

### 問題: 權限不足
**解決**: 使用管理員權限執行 PowerShell
```powershell
# 以管理員身份執行
Start-Process powershell -Verb runAs
```

### 問題: 生成失敗
**解決**: 增加重試次數
```powershell
.\Tools\ai\epic_assets_generate.ps1 -Epic epic1 -MaxRetries 5
```

### 問題: 缺少相依套件
**解決**: 安裝 PIL (用於圖像生成)
```powershell
pip install pillow numpy scipy
```

---

## 📞 使用範例流程

### 完整工作流程
```powershell
# 1. 初始化（第一次使用）
.\Tools\ai\epic_assets_generate.ps1 -Init

# 2. 查看素材清單
.\Tools\ai\epic_assets_generate.ps1 -List

# 3. 生成 Epic 2 的所有素材（戰鬥系統）
.\Tools\ai\epic_assets_generate.ps1 -Epic epic2

# 4. 生成所有音效
.\Tools\ai\epic_assets_generate.ps1 -All -Type sfx

# 5. 確認素材已生成
Get-ChildItem -Path Content\EpicAssets\epic2 -Recurse

# 6. （選項）使用 AI 工具生成高品質版本
# 將佔位檔案替換為 AI 生成的實際素材
```

---

## 📈 預估資源

### 存儲空間
| 類型 | 數量 | 預估大小 |
|------|------|----------|
| 圖像 | 101 | ~200 MB |
| 音樂 | 16 | ~80 MB |
| 音效 | 35 | ~20 MB |
| 影片 | 6 | ~300 MB |
| **總計** | | **~600 MB** |

### 生成時間
| 類型 | 數量 | 預估時間 |
|------|------|----------|
| 圖像 | 101 | ~3.5 小時 |
| 音樂 | 16 | ~1.5 小時 |
| 音效 | 35 | ~1 小時 |
| 影片 | 6 | ~1.5 小時 |
| **總計** | | **~8 小時** |

---

## ✅ 品質檢查清單

### 生成前
- [ ] 系統已初始化
- [ ] 磁碟空間充足 (> 1GB)
- [ ] Python 環境就緒

### 生成後
- [ ] 所有檔案已建立
- [ ] 檔案大小合理（非 0 bytes）
- [ ] 命名符合規範
- [ ] 目錄結構正確

### 整合前
- [ ] 素材品質符合要求
- [ ] UE5 匯入測試通過
- [ ] 效能測試通過

---

*最後更新: 2026-03-22*
