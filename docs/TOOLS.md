# TOOLS.md - 本地筆記

## 當前狀態

### ✅ **UE5.7.4 專用文件已創建**
- ✅ `Tools\build\GenerateProject_5.7.4.bat` - 生成項目文件
- ✅ `Tools\build\Build_5.7.4.bat` - 編譯項目
- ✅ `Tools\ue5\UE5.7_Check.bat` - UE5 檢查腳本
- ✅ `Tools\ue5\QuickStart_UE5.7.4.bat` - 快速啟動腳本
- ✅ `QUICKSTART_UE5.7.4.md` - UE5.7.4 專用指南
- ✅ `MingGoRTS.uproject` (EngineAssociation: "5.7")

### 📁 **目錄結構更新**
- ✅ `Tools/build/` - 構建工具目錄
- ✅ `Tools/ue5/` - UE5 專用工具目錄
- ✅ `Tools/test/` - 測試腳本目錄
- ✅ `Tools/ai/` - AI 工具目錄

### 🎯 **下一步操作**
1. **執行**: `Tools\build\GenerateProject_5.7.4.bat`
2. **檢查**: 生成 `.sln` 文件
3. **編譯**: 使用 Visual Studio 或 `Tools\build\Build_5.7.4.bat`
4. **啟動**: 雙擊 `MingGoRTS.uproject` 或使用 `Tools\ue5\QuickStart_UE5.7.4.bat`

### ⚠️ **注意事項**
- UE5.7.4 需要 Visual Studio 2019/2022 與 C++ 開發工具
- 首次編譯可能需要 10-30 分鐘
- 如果出錯，檢查 `Tools\ue5\UE5.7_Check.bat` 的輸出

---

## 常用命令

```cmd
# 生成項目文件
cd C:\HW\MingGoRTS
Tools\build\GenerateProject_5.7.4.bat

# 編譯項目
Tools\build\Build_5.7.4.bat

# 檢查 UE5 安裝
Tools\ue5\UE5.7_Check.bat

# 快速啟動
Tools\ue5\QuickStart_UE5.7.4.bat
```

## 文件變更摘要

| 文件 | 類型 | 目的 |
|------|------|------|
| `MingGoRTS.uproject` | 項目文件 | 更新 EngineAssociation 為 "5.7" |
| `Tools\build\GenerateProject_5.7.4.bat` | 構建腳本 | UE5.7.4 專用生成腳本 |
| `Tools\build\Build_5.7.4.bat` | 構建腳本 | UE5.7.4 專用編譯腳本 |
| `Tools\ue5\UE5.7_Check.bat` | 檢查腳本 | UE5.7.4 安裝檢查 |
| `Tools\ue5\QuickStart_UE5.7.4.bat` | 啟動腳本 | 一鍵啟動流程 |
| `QUICKSTART_UE5.7.4.md` | 文檔 | UE5.7.4 專用指南 |

---

## 下一步優先級

1. **執行 UE5.7.4 檢查** - 確認 UE5 安裝
2. **生成項目文件** - 創建 Visual Studio 項目
3. **編譯項目** - 構建 C++ 代碼
4. **創建藍圖** - 創建 BP_MingWarGameMode
5. **創建地圖** - 創建 TestMap.umap
6. **測試功能** - 驗證單位選擇和移動

---

*狀態：UE5.7.4 專用文件創建完成，等待執行*