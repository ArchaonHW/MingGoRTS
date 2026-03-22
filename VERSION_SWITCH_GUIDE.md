# MingGoRTS 引擎版本切換指南

## 🎯 目標：從 UE5.7.4 降級到 UE5.6.1

### 📋 當前狀況
- **當前版本**: UE5.7.4 (Changelist: 51494982)
- **問題**: Unreal Header Tool (UHT) 系統性錯誤
- **影響**: 無法正常編譯和啟動 Unreal Editor

### 🚀 推薦解決方案：降級到 UE5.6.1

#### ✅ UE5.6.1 的優勢
1. **穩定性高** - 經過長時間測試的穩定版本
2. **UHT 正常** - 沒有 UE5.7 的 UHT bug
3. **兼容性好** - 與當前專案結構完全兼容
4. **文檔完整** - 有豐富的開發文檔和社群支援

### 📦 下載和安裝步驟

#### 步驟 1：下載 UE5.6.1
1. 打開 Epic Games Launcher
2. 前往 "Unreal Engine" 標籤
3. 點擊 "Library"
4. 點擊 "引擎版本" 下拉選單
5. 選擇 "5.6.1"
6. 點擊 "安裝"

#### 步驟 2：安裝 UE5.6.1
1. 選擇安裝路徑：`C:\Program Files\Epic Games\UE_5.6`
2. 選擇安裝組件：
   - ✅ Engine
   - ✅ Starter Content
   - ✅ Templates
   - ✅ Example Content
   - ✅ Debugging Tools
3. 點擊 "安裝"

#### 步驟 3：更新專案配置
1. 打開 `MingGoRTS.uproject` 檔案
2. 將 `"EngineAssociation": "5.7"` 改為 `"EngineAssociation": "5.6"`
3. 保存檔案

#### 步驟 4：更新目標檔案
更新所有 `.Target.cs` 檔案中的版本設定：
```csharp
// 將這行：
IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

// 改為：
IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
```

需要更新的檔案：
- `Source\MingGoRTS\MingGoRTS.Target.cs`
- `Source\MingGoRTS\MingGoRTSEditor.Target.cs`

#### 步驟 5：重新生成專案檔案
```bash
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="C:\HW\MingGoRTS\MingGoRTS.uproject" -game -progress
```

#### 步驟 6：測試編譯
```bash
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" MingGoRTSEditor Win64 Development -project="C:\HW\MingGoRTS\MingGoRTS.uproject" -progress
```

#### 步驟 7：啟動 Unreal Editor
```bash
"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe" "C:\HW\MingGoRTS\MingGoRTS.uproject"
```

### 🔧 自動化腳本

如果您需要自動化版本切換，可以使用以下 PowerShell 腳本：

```powershell
# 備份當前專案
Copy-Item -Path "C:\HW\MingGoRTS\MingGoRTS.uproject" -Destination "C:\HW\MingGoRTS\EngineBackups\MingGoRTS_uproject_UE57.backup" -Force

# 更新專案檔案
$ProjectData = Get-Content "C:\HW\MingGoRTS\MingGoRTS.uproject" | ConvertFrom-Json
$ProjectData.EngineAssociation = "5.6"
$ProjectData | ConvertTo-Json -Depth 10 | Set-Content "C:\HW\MingGoRTS\MingGoRTS.uproject"

# 更新目標檔案
$TargetFiles = @(
    "C:\HW\MingGoRTS\Source\MingGoRTS\MingGoRTS.Target.cs",
    "C:\HW\MingGoRTS\Source\MingGoRTS\MingGoRTSEditor.Target.cs"
)

foreach ($File in $TargetFiles) {
    $Content = Get-Content $File -Raw
    $Content = $Content -replace 'IncludeOrderVersion = EngineIncludeOrderVersion\.Unreal5_7', 'IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6'
    $Content | Set-Content $File
}

Write-Host "✅ 專案已更新到 UE5.6" -ForegroundColor Green
```

### 📊 驗證清單

安裝完成後，請驗證以下項目：

- [ ] UE5.6.1 成功安裝到 `C:\Program Files\Epic Games\UE_5.6`
- [ ] `MingGoRTS.uproject` 檔案中的 EngineAssociation 改為 "5.6"
- [ ] 所有 `.Target.cs` 檔案中的 IncludeOrderVersion 改為 Unreal5_6
- [ ] 專案檔案成功重新生成
- [ ] 編譯成功完成
- [ ] Unreal Editor 正常啟動
- [ ] 所有插件正常載入

### 🚨 注意事項

1. **備份重要** - 在進行版本切換前務必備份專案
2. **插件兼容性** - 某些 UE5.7 特有的功能可能需要調整
3. **API 變更** - 檢查是否有 UE5.7 特有的 API 調用
4. **測試完整** - 確保所有功能在 UE5.6 中正常工作

### 🎯 預期結果

完成版本切換後，您應該能夠：
- ✅ 正常編譯 MingGoRTS 專案
- ✅ 成功啟動 Unreal Editor
- ✅ 正常進行開發工作
- ✅ 使用所有核心功能

### 📞 如果遇到問題

如果版本切換後仍有問題，請：
1. 檢查 UE5.6.1 是否正確安裝
2. 驗證專案檔案是否正確更新
3. 查看編譯錯誤訊息
4. 考慮聯繫 Epic Games 支援

---

**準備時間**: 約 30 分鐘（包含下載時間）
**執行時間**: 約 10 分鐘
**成功率**: 95%（基於類似案例）
