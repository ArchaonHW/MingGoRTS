# MingGoRTS Sprint 1 素材製作指南

## 📋 製作清單

### 🎨 視覺素材 (優先級：高)

#### 必需素材
- [ ] **T_TitleCard.png** - 標題卡片背景
- [ ] **T_MingGoRTS_Logo.png** - 遊戲 Logo
- [ ] **T_GameTitle_Final.png** - 最終遊戲標題
- [ ] **UI_Skip_Button_Normal.png** - 跳過按鈕
- [ ] **UI_Skip_Button_Hover.png** - 跳過按鈕懸停

#### 歷史素材
- [ ] **T_1911_Revolution.jpg** - 辛亥革命
- [ ] **T_Republic_Established.jpg** - 民國建立
- [ ] **T_Warlord_Period.jpg** - 軍閥時期
- [ ] **T_Japanese_Invasion.jpg** - 日軍入侵

#### 粒子效果
- [ ] **P_Sparkle.png** - 閃光粒子
- [ ] **P_Smoke.png** - 煙霧粒子
- [ ] **P_Glow.png** - 光暈效果

### 🎵 音效素材 (優先級：中)

#### 背景音樂
- [ ] **IntroMusic_MainTheme.wav** - 主題音樂 (12秒)
- [ ] **HistoryBackground_Ambience.wav** - 歷史背景音效 (5秒)
- [ ] **GameTitle_Fanfare.wav** - 遊戲標題音效 (4秒)

#### 互動音效
- [ ] **UI_Click.wav** - 按鈕點擊音效
- [ ] **UI_Hover.wav** - 按鈕懸停音效
- [ ] **Transition_Swoosh.wav** - 轉場音效

### 🎮 UI 元素 (優先級：高)

#### Widget 藍圖
- [ ] **WBP_IntroWidget.uasset** - 開頭動畫 Widget
- [ ] **WBP_MainMenu.uasset** - 主選單 Widget
- [ ] **WBP_GameHUD.uasset** - 遊戲 HUD Widget

#### 材質
- [ ] **M_IntroFade.uasset** - 淡入淡出材質
- [ ] **M_TitleGlow.uasset** - 標題光暈材質
- [ ] **M_ButtonHighlight.uasset** - 按鈕高亮材質

## 🎨 視覺素材製作指南

### 工具推薦
- **圖像編輯**: Adobe Photoshop / GIMP
- **向量設計**: Adobe Illustrator / Inkscape
- **3D 建模**: Blender / 3ds Max
- **紋理製作**: Substance Painter

### 製作步驟

#### 1. 標題卡片背景製作
```
1. 創建 1920x1080 畫布
2. 填充米黃色背景 (#F5E6D3)
3. 添加羊皮紙紋理
4. 繪製中國傳統雲紋邊框
5. 添加四角龍紋裝飾
6. 應用老化效果
7. 導出為 PNG 格式
```

#### 2. 遊戲 Logo 製作
```
1. 創建 800x400 透明畫布
2. 設計 "MingGoRTS" 英文標題
3. 添加 "民國歷史" 中文副標題
4. 應用金色漸變效果
5. 添加陰影和光暈
6. 導出為 PNG 格式 (支援透明)
```

#### 3. 歷史照片處理
```
1. 收集歷史照片素材
2. 調整解析度至 1920x1080
3. 應用褐色調處理
4. 添加粒子效果
5. 調整對比度和亮度
6. 導出為 JPEG 格式
```

### 色彩標準
- **主色**: 中國紅 #DE2910
- **輔色**: 金色 #FFD700
- **背景**: 深褐色 #3E2723
- **文字**: 米白色 #F5F5DC

### 字體標準
- **中文標題**: 楷體 / 隸體
- **中文內文**: 宋體
- **英文標題**: Times New Roman
- **英文內文**: Arial

## 🎵 音效素材製作指南

### 工具推薦
- **音頻編輯**: Audacity / Adobe Audition
- **音樂製作**: FL Studio / Ableton Live
- **音效庫**: Freesound.org / Adobe Audition Library

### 製作步驟

#### 1. 主題音樂製作
```
1. 設定 12 秒時長
2. 選擇 120 BPM 節奏
3. 使用軍鼓作為主節奏
4. 添加銅管樂器主旋律
5. 加入小號輝煌音色
6. 混音和母帶處理
7. 導出為 WAV 格式 (44.1kHz, 16-bit)
```

#### 2. 歷史背景音效
```
1. 收集歷史音效素材
2. 添加舊照片翻頁聲
3. 混合歷史紀錄音效
4. 調整音量平衡
5. 添加環境氛圍
6. 導出為 OGG 格式
```

### 音效規格
- **採樣率**: 44.1 kHz
- **位元深度**: 16-bit
- **聲道**: 立體聲
- **音量**: -6dB (主音樂), -12dB (音效)

## 🎮 UI 元素製作指南

### 工具推薦
- **UI 設計**: Unreal Editor UMG
- **圖像資產**: Photoshop / GIMP
- **動畫**: Unreal Editor Sequencer

### 製作步驟

#### 1. 開頭動畫 Widget
```
1. 創建 Widget 藍圖
2. 添加 Image 組件 (IntroImage)
3. 添加 TextBlock 組件 (IntroText)
4. 添加 TextBlock 組件 (SubtitleText)
5. 添加 Button 組件 (SkipButton)
6. 綁定事件和動畫
7. 測試所有功能
```

#### 2. 材質製作
```
1. 創建 Material 實例
2. 設置參數 (Opacity, Color, etc.)
3. 添加紋理節點
4. 創建動畫參數
5. 測試視覺效果
```

## 📁 檔案組織

### 資料夾結構
```
Content/Intro/
├── Textures/
│   ├── T_TitleCard.png
│   ├── T_MingGoRTS_Logo.png
│   ├── T_GameTitle_Final.png
│   └── T_Historical_*.jpg
├── Materials/
│   ├── M_IntroFade.uasset
│   ├── M_TitleGlow.uasset
│   └── M_ButtonHighlight.uasset
├── UI/
│   ├── WBP_IntroWidget.uasset
│   ├── WBP_MainMenu.uasset
│   └── WBP_GameHUD.uasset
├── Audio/
│   ├── IntroMusic_MainTheme.wav
│   ├── HistoryBackground_Ambience.wav
│   └── GameTitle_Fanfare.wav
└── Particles/
    ├── P_Sparkle.png
    ├── P_Smoke.png
    └── P_Glow.png
```

### 命名規範
- **紋理**: T_[描述].png/jpg
- **材質**: M_[描述].uasset
- **Widget**: WBP_[描述].uasset
- **音效**: [類型]_[描述].wav/ogg
- **粒子**: P_[描述].png

## 🎯 品質標準

### 視覺品質
- **解析度**: 最少 1920x1080
- **色彩深度**: 24-bit
- **檔案格式**: PNG (透明), JPEG (不透明)
- **檔案大小**: < 5MB (單個檔案)

### 音效品質
- **採樣率**: 44.1 kHz
- **位元深度**: 16-bit
- **檔案格式**: WAV (無損), OGG (壓縮)
- **檔案大小**: < 10MB (單個檔案)

### 效能標準
- **載入時間**: < 2秒
- **記憶體佔用**: < 200MB
- **GPU 使用率**: < 80%
- **幀率**: 穩定 60 FPS

## 🔄 測試流程

### 功能測試
1. **載入測試**
   - [ ] 所有素材正常載入
   - [ ] 無載入錯誤
   - [ ] 載入時間符合標準

2. **顯示測試**
   - [ ] 圖像正常顯示
   - [ ] 色彩正確
   - [ ] 解析度適配

3. **音效測試**
   - [ ] 音效正常播放
   - [ ] 音量平衡
   - [ ] 無雜音或破音

### 兼容性測試
1. **解析度測試**
   - [ ] 1920x1080
   - [ ] 2560x1440
   - [ ] 3840x2160

2. **平台測試**
   - [ ] Windows
   - [ ] Android (未來)
   - [ ] iOS (未來)

## 📅 時間規劃

### 第一週 (視覺素材)
- **第1-2天**: 標題卡片和 Logo 製作
- **第3-4天**: 歷史照片處理
- **第5天**: UI 元素和按鈕

### 第二週 (音效素材)
- **第1-2天**: 主題音樂製作
- **第3天**: 歷史背景音效
- **第4-5天**: UI 音效和轉場音效

### 第三週 (整合測試)
- **第1-2天**: Widget 開發
- **第3-4天**: 動畫整合
- **第5天**: 最終測試和優化

## 🎯 成功指標

### 技術指標
- [ ] 所有素材載入成功
- [ ] 動畫播放流暢
- [ ] 音效同步播放
- [ ] 跳過功能正常

### 用戶體驗指標
- [ ] 視覺效果出色
- [ ] 音效品質良好
- [ ] 載入時間快速
- [ ] 互動反應及時

### 專案指標
- [ ] 按時完成
- [ ] 預算控制
- [ ] 品質達標
- [ ] 文檔完整

## 🚀 後續擴展

### Sprint 2 準備
- 遊戲關卡素材
- 角色模型和動畫
- 建築和環境素材
- 更多音效和音樂

### 長期規劃
- 多語言支援
- 高解析度素材
- VR/AR 支援
- 雲端素材管理

---

**📝 製作完成後，請更新此文檔並標記完成項目。**

**🎯 目標：創建一個專業級的開頭動畫，展現民國歷史背景和遊戲特色！**
