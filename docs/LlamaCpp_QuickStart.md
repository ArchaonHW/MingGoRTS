# llama.cpp 本地模型啟動指南

## 🚀 快速開始

### 1. 環境準備

**系統需求：**
- Windows 10/11 (64-bit) / Linux / macOS
- CMake 3.16+
- C++17 編譯器 (MSVC 2019+ / GCC 9+ / Clang 10+)
- 4GB+ 可用 RAM (取決於模型大小)
- (可選) NVIDIA GPU 用於 CUDA 加速

**安裝依賴：**

```bash
# Windows - 使用 Chocolatey
choco install cmake git

# 或使用 vcpkg
vcpkg install cmake

# Linux
sudo apt-get install cmake git build-essential

# macOS
brew install cmake git
```

---

## 2. 下載 llama.cpp 源碼

```bash
cd f:\MingGoRTS\Source\ThirdParty\llama

# 克隆 llama.cpp (穩定版本 b3473)
git clone --depth 1 --branch b3473 https://github.com/ggerganov/llama.cpp.git

cd llama.cpp
```

---

## 3. 下載免費模型

### 推薦模型（程式碼生成）

| 模型 | 量化級別 | VRAM | 下載 |
|------|---------|------|------|
| CodeLlama-7B | Q4_K_M | ~4GB | [下載](https://huggingface.co/TheBloke/CodeLlama-7B-Instruct-GGUF) |
| DeepSeek-Coder-6.7B | Q4_K_M | ~4GB | [下載](https://huggingface.co/TheBloke/deepseek-coder-6.7b-instruct-GGUF) |
| Mistral-7B-Instruct | Q4_K_M | ~4GB | [下載](https://huggingface.co/TheBloke/Mistral-7B-Instruct-v0.2-GGUF) |
| Phi-2 | Q4_K_M | ~1.6GB | [下載](https://huggingface.co/TheBloke/phi-2-GGUF) |

### 自動下載腳本

創建 `download_models.ps1`：

```powershell
# models/download_models.ps1
$ModelsDir = "models"
New-Item -ItemType Directory -Force -Path $ModelsDir

$Models = @(
    @{
        Name = "codellama-7b-instruct.Q4_K_M.gguf"
        Url = "https://huggingface.co/TheBloke/CodeLlama-7B-Instruct-GGUF/resolve/main/codellama-7b-instruct.Q4_K_M.gguf"
    },
    @{
        Name = "mistral-7b-instruct-v0.2.Q4_K_M.gguf"
        Url = "https://huggingface.co/TheBloke/Mistral-7B-Instruct-v0.2-GGUF/resolve/main/mistral-7b-instruct-v0.2.Q4_K_M.gguf"
    }
)

foreach ($Model in $Models) {
    $OutputPath = Join-Path $ModelsDir $Model.Name
    if (-not (Test-Path $OutputPath)) {
        Write-Host "下載 $($Model.Name)..."
        curl -L -o $OutputPath $Model.Url
    } else {
        Write-Host "$($Model.Name) 已存在，跳過下載"
    }
}

Write-Host "模型下載完成！"
```

執行：
```powershell
cd f:\MingGoRTS
.\download_models.ps1
```

---

## 4. 編譯專案

### Windows (Visual Studio)

```bash
cd f:\MingGoRTS\Game
mkdir build
cd build

# CPU 版本 (無 GPU)
cmake .. -DENABLE_LLAMA_CPP=ON -DLLAMA_CUDA=OFF -DLLAMA_AVX2=ON

# CUDA 版本 (需 NVIDIA GPU)
cmake .. -DENABLE_LLAMA_CPP=ON -DLLAMA_CUDA=ON

# 編譯
cmake --build . --config Release --parallel
```

### Linux / macOS

```bash
cd Game
mkdir build && cd build

cmake .. -DENABLE_LLAMA_CPP=ON
make -j$(nproc)
```

---

## 5. 配置本地 AI Agent

### 5.1 創建配置檔

創建 `Config/LocalLLM.json`：

```json
{
    "Provider": "LlamaCpp",
    "ModelPath": "models/codellama-7b-instruct.Q4_K_M.gguf",
    "UseGPU": true,
    "GPUDeviceId": 0,
    "DefaultParams": {
        "Temperature": 0.7,
        "TopP": 0.9,
        "MaxTokens": 2048,
        "RepeatPenalty": 1.1
    }
}
```

### 5.2 啟動代碼範例

```cpp
// Main.cpp - 本地 AI 啟動
#include "AI/MingLocalAIAgent.h"
#include "AI/MingLocalLLMInterface.h"

void StartLocalAIAgent()
{
    // 1. 創建 LLM 接口
    ULocalLLMInterface* LLM = NewObject<ULocalLLMInterface>();
    
    // 2. 配置本地模型
    FLocalLLMConfig Config;
    Config.Provider = ELocalLLMProvider::LlamaCpp;
    Config.ModelPath = TEXT("models/codellama-7b-instruct.Q4_K_M.gguf");
    Config.bUseGPU = true;
    Config.Temperature = 0.7f;
    Config.TopP = 0.9f;
    Config.MaxTokens = 2048;
    
    // 3. 初始化
    LLM->Initialize(Config);
    
    // 4. 載入模型 (需要幾秒鐘)
    UE_LOG(LogTemp, Log, TEXT("正在載入本地模型..."));
    if (LLM->LoadModel(Config.ModelPath))
    {
        UE_LOG(LogTemp, Log, TEXT("模型載入成功！"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("模型載入失敗！"));
        return;
    }
    
    // 5. 創建 AI Agent
    FLocalAIAgentConfig AgentConfig;
    AgentConfig.AgentType = ELocalAIAgentType::CodeAssistant;
    AgentConfig.AgentName = FMingOptimizedString(TEXT("本地代碼助手"));
    
    ULocalAIAgent* Agent = NewObject<ULocalAIAgent>();
    Agent->Initialize(AgentConfig);
    
    // 6. 測試推理
    UE_LOG(LogTemp, Log, TEXT("測試本地推理..."));
    FString TestPrompt = TEXT("生成一個 UE5 C++ 組件，實現角色跳躍功能");
    
    FLLMInferenceResult Result = LLM->GenerateSync(TestPrompt, FLLMInferenceParams());
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("生成結果:\n%s"), *Result.GeneratedText);
        UE_LOG(LogTemp, Log, TEXT("生成速度: %.1f tokens/秒"), Result.TokensPerSecond);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("推理失敗: %s"), *Result.ErrorMessage);
    }
}
```

---

## 6. 執行專案

### 運行可執行文件

```bash
cd f:\MingGoRTS\Game\build\Release

# 運行遊戲主程序
.\MingGoRTSGame.exe

# 或運行 AI Agent 啟動器
.\AIAgentLauncher.exe
```

### 驗證運行

成功啟動後，控制台應顯示：

```
[LocalLLM] Initialized with provider: LlamaCpp
[LocalLLM] LlamaCpp wrapper created
[LocalLLM] Loading GGUF model from: models/codellama-7b-instruct.Q4_K_M.gguf (GPU layers: -1)
[LocalLLM] Model loaded successfully:
  - Name: CodeLlama-7B-Instruct
  - Vocab Size: 32000
  - Context Size: 4096
  - Load Time: 2450.50 ms
```

---

## 7. 常見問題排查

### 問題 1: 模型載入失敗

```
Error: Failed to load model from: models/xxx.gguf
```

**解決方案：**
1. 檢查模型文件是否存在
2. 驗證模型文件完整性 (MD5)
3. 確認模型格式為 GGUF

### 問題 2: CUDA 初始化失敗

```
CUDA error: no CUDA-capable device is detected
```

**解決方案：**
```bash
# 切換到 CPU 模式
cmake .. -DENABLE_LLAMA_CPP=ON -DLLAMA_CUDA=OFF
```

### 問題 3: 記憶體不足

```
ggml_new_tensor_impl: not enough space in the context's memory pool
```

**解決方案：**
1. 使用更小的模型 (Phi-2 1.6GB)
2. 增加虛擬記憶體
3. 減少 GPU 層數：
   ```cpp
   Config.bUseGPU = false;  // CPU 模式
   // 或
   // 在 LlamaCppWrapper.cpp 中減少 GpuLayers
   ```

### 問題 4: 編譯錯誤

```
error: 'llama_sampling_sample' was not declared
```

**解決方案：**
```bash
# 更新到正確版本的 llama.cpp
cd f:\MingGoRTS\Source\ThirdParty\llama\llama.cpp
git fetch --tags
git checkout b3473  # 使用穩定版本
```

---

## 8. 性能優化建議

### GPU 加速

| GPU | 建議量化級別 | 預期速度 |
|-----|------------|---------|
| RTX 3060 12GB | Q4_K_M | 50-80 tokens/s |
| RTX 4070 12GB | Q5_K_M | 80-120 tokens/s |
| GTX 1660 6GB | Q4_0 | 20-40 tokens/s |
| 僅 CPU | Q4_0 | 5-15 tokens/s |

### 優化參數

```cpp
// 減少記憶體佔用
FLLMInferenceParams Params;
Params.MaxTokens = 512;        // 減少最大生成长度
Params.TopK = 20;              // 限制採樣範圍
Params.RepeatLastN = 32;       // 減少重複懲罰範圍

// 提高速度
Params.Temperature = 0.3f;     // 降低隨機性可略微加速
```

---

## 9. 下一步

- [AI Agent 開發指南](LocalAIAgent_Development.md)
- [模型微調指南](ModelFineTuning.md)
- [性能調優指南](PerformanceTuning.md)

---

**文檔版本**: v1.0.0  
**更新日期**: 2026-03-24  
**狀態**: 已驗證可運行
