# AI驅動開頭動畫使用指南

## 概述

本指南介紹如何使用AI技術製作MingGoRTS的民國史詩風格開頭動畫，集成Stable Diffusion圖像生成和AIVA音樂生成。

---

## 🎯 系統架構

### 核心組件
- **AICinematicManager** - AI動畫管理器
- **Stable Diffusion API** - 圖像生成
- **AIVA API** - 音樂生成
- **配置系統** - 動畫參數配置

### 工作流程
```
1. 載入配置文件
2. 並行生成所有場景素材
3. 處理生成的資產
4. 組合最終動畫
5. 輸出多格式版本
```

---

## 🛠️ 環境設置

### 1. Stable Diffusion環境
```bash
# 安裝Stable Diffusion WebUI
git clone https://github.com/AUTOMATIC1111/stable-diffusion-webui.git
cd stable-diffusion-webui

# 安裝依賴
pip install -r requirements.txt

# 啟動服務
python launch.py --api --listen
```

### 2. API配置
```json
{
  "stable_diffusion": {
    "endpoint": "http://localhost:7860/sdapi/v1/txt2img",
    "model": "sd-v1-5",
    "width": 1920,
    "height": 1080,
    "steps": 30,
    "cfg_scale": 7.5
  }
}
```

### 3. AIVA設置
```bash
# 獲取AIVA API密鑰
# 1. 註冊AIVA帳戶
# 2. 生成API密鑰
# 3. 配置到配置文件中
```

---

## 🎨 場景生成詳細說明

### 場景1: 序幕 (0-5秒)
#### AI提示詞
```
圖像提示詞:
"1911 Chinese Republic era, traditional Chinese calligraphy, ink wash painting style, historical document, faded paper texture, cinematic lighting, 8K, masterpiece"

音樂提示詞:
"Traditional guzheng melody, subtle and elegant, historical atmosphere, Chinese classical music, gentle introduction"
```

#### 生成參數
- 解析度: 1920x1080
- 採樣步數: 30
- CFG Scale: 7.5
- 採樣器: DPM++ 2M Karras

### 場景2: Logo展示 (5-10秒)
#### AI提示詞
```
圖像提示詞:
"Potato Studio logo, modern minimalist design, revolutionary symbols, Chinese traditional elements, golden glow effect, professional gaming studio logo, clean vector style, 8K, high quality"

音樂提示詞:
"Modern electronic beat mixed with traditional Chinese instruments, energetic and professional, gaming studio introduction music"
```

#### 生成參數
- 解析度: 1920x1080
- 採樣步數: 25
- CFG Scale: 8.0
- 採樣器: DPM++ 2M Karras

### 場景3: 歷史場景 (10-20秒)
#### AI提示詞
```
圖像提示詞:
"1911 Chinese Republic historical scene, five-color flag waving, revolutionary soldiers marching, traditional Chinese architecture, crowd celebrating freedom, epic cinematic lighting, dramatic atmosphere, 8K, historical accuracy"

音樂提示詞:
"Epic orchestral music with Chinese instruments, revolutionary spirit, dramatic and inspiring, historical battle music, guzheng and erhu leading"
```

#### 生成參數
- 解析度: 1920x1080
- 採樣步數: 35
- CFG Scale: 7.0
- 採樣器: DPM++ 2M Karras

### 場景4: 標題場景 (20-25秒)
#### AI提示詞
```
圖像提示詞:
"MingGoRTS title screen, Chinese historical RTS game, epic battle scene, traditional Chinese calligraphy, revolutionary theme, modern gaming aesthetic, cinematic composition, 8K, AAA game quality"

音樂提示詞:
"Epic main theme, full orchestra with Chinese instruments, powerful and memorable, RTS game music, heroic and dramatic, climactic finale"
```

#### 生成參數
- 解析度: 1920x1080
- 採樣步數: 40
- CFG Scale: 6.5
- 採樣器: DPM++ 2M Karras

---

## 🔧 使用方法

### 1. 在UE5中設置
```cpp
// 在遊戲實例中添加AI動畫管理器
AICinematicManager* CinematicManager = GetGameInstance()->GetSubsystem<AICinematicManager>();

// 配置動畫參數
CinematicManager->CinematicTitle = TEXT("MingGoRTS - 民國無雙");
CinematicManager->TotalDuration = 25.0f;
CinematicManager->bUseAIGeneration = true;

// 開始動畫
CinematicManager->StartCinematic();
```

### 2. 在藍圖中使用
```
1. 創建Actor
2. 添加AICinematicManager組件
3. 配置屬性面板
4. 調用StartCinematic函數
```

### 3. 自定義場景
```cpp
// 自定義提示詞
FString CustomPrompt = TEXT("Your custom prompt here");
CinematicManager->StableDiffusionPrompt = CustomPrompt;

// 生成特定場景
CinematicManager->GenerateIntroBackground();
CinematicManager->GenerateLogoAnimation();
CinematicManager->GenerateHistoryScene();
CinematicManager->GenerateTitleScene();
```

---

## 📊 性能優化

### 1. 並行生成
- 同時生成多個場景
- 限制並發請求數量
- 智能隊列管理

### 2. 資產緩存
- 本地緩存生成的資產
- 避免重複生成
- 版本控制管理

### 3. 漸進式載入
- 預載入下一場景
- 流暢的場景轉換
- 內存使用優化

---

## 🎯 質量控制

### 1. 提示詞優化
```
正向提示詞結構:
[主體] + [風格] + [細節] + [技術參數]

負向提示詞:
"blurry, low quality, distorted, text, watermark, signature, ugly, bad anatomy"
```

### 2. 參數調整
```
高質量設置:
- Steps: 30-40
- CFG Scale: 6.5-8.0
- Sampler: DPM++ 2M Karras
- Resolution: 1920x1080
```

### 3. 後期處理
```
圖像處理:
- 色彩校正
- 銳利化
- 雜點去除
- 尺寸標準化

音頻處理:
- 音量標準化
- 動態範圍壓縮
- 均衡器調整
- 母帶處理
```

---

## 🚀 故障排除

### 常見問題

#### 1. API連接失敗
```
問題: 無法連接到Stable Diffusion API
解決方案:
- 檢查API服務是否運行
- 驗證端點URL是否正確
- 檢查防火牆設置
```

#### 2. 生成質量不佳
```
問題: 生成的圖像質量不符合預期
解決方案:
- 調整提示詞
- 修改CFG Scale
- 增加採樣步數
- 更換模型
```

#### 3. 生成速度慢
```
問題: AI生成過程緩慢
解決方案:
- 使用更強的GPU
- 降低解析度
- 減少採樣步數
- 啟用並行生成
```

### 調試工具
```cpp
// 啟用詳細日誌
UE_LOG(LogTemp, Warning, TEXT("AI Cinematic Debug: %s"), *DebugInfo);

// 檢查生成狀態
bool bIsGenerating = CinematicManager->IsCinematicPlaying();
int32 PendingCount = CinematicManager->GetPendingEventCount();

// 監控性能
float GenerationTime = GetWorld()->GetDeltaSeconds();
```

---

## 📋 最佳實踐

### 1. 提示詞工程
- 使用具體描述
- 包含風格關鍵詞
- 添加質量修飾詞
- 使用負向提示詞

### 2. 工作流程優化
- 預生成常用資產
- 使用版本控制
- 自動化測試流程
- 建立質量標準

### 3. 資產管理
- 統一命名規範
- 分類存儲結構
- 備份重要資產
- 文檔化生成過程

---

## 🎯 預期效果

### 視覺效果
- 民國歷史氛圍
- 專業Logo展示
- 流暢場景轉換
- 高質量視覺效果

### 音頻效果
- 傳統中國音樂
- 現代電子元素
- 情感豐富的配樂
- 專業音頻質量

### 整體效果
- 25秒完整動畫
- 4個主要場景
- AI生成的高質量素材
- 專業遊戲開頭動畫

---

**使用AI技術製作的開頭動畫將為MingGoRTS營造獨特的視覺體驗！**
