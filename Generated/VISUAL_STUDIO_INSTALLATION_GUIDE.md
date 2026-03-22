# Visual Studio 2022 安裝指南 - MingGoRTS 開發環境

## 🚀 快速安裝步驟

### 方法 1: 直接下載安裝 (推薦)

#### 步驟 1: 下載 Visual Studio 2022 Community
**下載連結**: https://aka.ms/vs/17/release/vs_community.exe

1. 點擊上述連結或訪問：https://visualstudio.microsoft.com/zh-hans/downloads/
2. 找到 "Visual Studio 2022 Community" 
3. 點擊 "下載 Community" 按鈕

#### 步驟 2: 運行安裝程序
1. 雙擊下載的 `vs_community.exe` 文件
2. 等待安裝程序啟動

#### 步驟 3: 選擇工作負載
在安裝程序中，**必須勾選**以下工作負載：

```
✅ 使用 C++ 的遊戲開發 (Game development with C++)
✅ .NET 桌面開發 (.NET desktop development)
```

#### 步驟 4: 選擇組件
在右側面板中，確保勾選：

```
必須組件：
✅ Windows 10/11 SDK (最新版本)
✅ C++ MFC 用於最新 v143 生成工具的 x86/x64 生成工具
✅ 用於 Windows 的 C++ CMake 工具

推薦組件：
✅ Git for Windows
✅ IntelliSense
✅ C++ 分析工具
```

#### 步驟 5: 安裝
1. 點擊 "安裝" 按鈕
2. 等待安裝完成（可能需要 30-60 分鐘）
3. 安裝完成後重啟電腦

### 方法 2: 使用腳本安裝

#### 自動安裝腳本
```powershell
# 創建安裝腳本
$script = @"
@echo off
cd /d "C:\VS2022_Download"
vs_community.exe --quiet --wait --add Microsoft.VisualStudio.Workload.Game --add Microsoft.VisualStudio.Workload.ManagedDesktop --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended
"@

$script | Out-File -FilePath "install_vs.bat" -Encoding ASCII
.\install_vs.bat
```

## 🔧 安裝後驗證

### 驗證腳本
```powershell
# 檢查安裝
$vsPath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
if (Test-Path $vsPath) {
    Write-Host "✓ Visual Studio 2022 已安裝" -ForegroundColor Green
    $version = (Get-Item $vsPath).VersionInfo.FileVersion
    Write-Host "版本: $version" -ForegroundColor Gray
} else {
    Write-Host "✗ Visual Studio 2022 未找到" -ForegroundColor Red
}
```

### 手動驗證
1. 按 `Win + R`，輸入 `devenv.exe`
2. 如果 Visual Studio 2022 啟動，表示安裝成功
3. 或在開始菜單中搜索 "Visual Studio 2022"

## 🎯 MingGoRTS 項目配置

### 打開項目
1. 啟動 Visual Studio 2022
2. 選擇 "打開項目或解決方案"
3. 瀏覽到：`c:\HW\MingGoRTS\MingGoRTS.sln`
4. 雙擊打開

### 編譯測試
1. 打開解決方案後，按 `Ctrl+Shift+B` 編譯
2. 或選擇 "生成" → "生成解決方案"
3. 確保編譯成功無錯誤

### 推薦擴展
在 Visual Studio 中安裝以下擴展：

```
必須擴展：
✅ GitHub Extension for Visual Studio
✅ Visual Studio IntelliCode

推薦擴展：
✅ Visual Assist X (增強 C++ 開發)
✅ C++ Clang power tools
✅ C++ Core Guidelines Checkers
```

## ⚠️ 常見問題解決

### 問題 1: 下載速度慢
**解決方案**：
- 使用 VPN 或更換網絡
- 選擇離線安裝包下載

### 問題 2: 安裝失敗
**解決方案**：
- 確保有足夠磁碟空間（至少 20GB）
- 暫時關閉防毒軟體
- 以管理員身份運行安裝程序

### 問題 3: 編譯錯誤
**解決方案**：
- 確保安裝了 Windows 10/11 SDK
- 檢查 C++ 工具鏈是否正確安裝
- 清理解決方案並重新編譯

### 問題 4: IntelliSense 不工作
**解決方案**：
- 重啟 Visual Studio
- 清除 IntelliSense 緩存：工具 → 選項 → 文本編輯器 → C/C++ → 高級 → 禁用 IntelliSense → 重新啟用

## 📋 系統需求

### 最低要求
- **作業系統**: Windows 10 版本 1909 或更高
- **記憶體**: 8GB RAM（推薦 16GB）
- **磁碟空間**: 20GB 可用空間
- **處理器**: 1.8 GHz 或更快的處理器

### 推薦配置
- **作業系統**: Windows 11
- **記憶體**: 16GB+ RAM
- **磁碟空間**: 50GB+ SSD
- **處理器**: 多核處理器 3.0GHz+

## 🎉 安裝完成後

### 下一步
1. **驗證安裝**：運行驗證腳本
2. **打開項目**：載入 MingGoRTS.sln
3. **編譯測試**：確保項目能正常編譯
4. **配置設置**：調整個人偏好設置
5. **安裝擴展**：安裝推薦的 Visual Studio 擴展

### 開發環境就緒
完成以上步驟後，您的 Visual Studio 2022 開發環境就配置完成，可以開始 MingGoRTS 項目的開發工作了！

## 📞 技術支援

如果遇到問題，可以：
1. 查看 Visual Studio 官方文檔
2. 搜索錯誤訊息尋求解決方案
3. 檢查 MingGoRTS 項目的編譯要求
4. 聯繫項目技術支援

---

**最後更新**: 2026-03-23  
**適用版本**: Visual Studio 2022 Community  
**項目**: MingGoRTS
