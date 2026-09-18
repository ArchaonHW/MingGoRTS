# U-1 垂直切片 Demo（DuanqiaoPlayable → 可散布遊戲 demo）

- id: U-1
- size: L
- source: 2026-09-18 評估「B 垂直切片」決策；UX spec ux-MingGoRTS-2026-09-18（phasing: tactical-sim + war-map 先行）

## 目標

把 `DuanqiaoPlayable` 從 dev 工具包成「像遊戲」的可散布 demo：
標題殼 → 規劃 → 執行 → 戰後 → 回主選單；字體/資產自包含；
ImGui 主題系統落地 tactical-sim + war-map 兩套（token 表驅動）。

## 範圍

1. **`assets/fonts/`** — NotoSansTC-Regular/Bold、NotoSerifTC-Regular（OFL，附 LICENSE）。
   msjh.ttc 是微軟財產不可散布，全面替換。
2. **`Examples/DemoAssets.h`**（header-only 共用）— `ResolveAssetPath(rel)`：
   exe 目錄起向上探 ≤5 層找 `assets/`；找不到回退 cwd。包裝時 assets/ 放 exe 旁即自動命中。
3. **`Examples/UITheme.h/.cpp`** —
   `enum class ThemeId { TacticalSim, WarMap }`；
   `ApplyTheme(ImGuiStyle&, ThemeId)` 照 DESIGN.md token 表設 WindowBg/Text/Button/FrameBg/邊框/圓角；
   `ThemeFont(ThemeId)` 回 Sans/Serif；`ThemeCloudColor(ThemeId)` 回 3D 雲色
   （tactical #35d0c5 / war-map #6a4a8c）。
4. **`DuanqiaoPlayable`** —
   - 殼狀態：`Title → Battle → Title`；標題頁按鈕：開戰 / 操作說明 / 設定 / 離開。
     設定頁：主題下拉 + UI 縮放 slider（100–150%，FontGlobalScale + ScaleAllSizes）。
   - 字體：resolver 找 `assets/fonts/*.otf`，AddFontFromFileTTF + MergeMode 掛
     `GetGlyphRangesChineseFull()`；主題切換時 PushFont 對應字體。
     找不到字體檔 → 退回系統字體嘗試 → 再退 ImGui 預設字體（英文介面仍可用）。
   - 戰後結算窗加「返回主選單」（重置 battle → 回 Title）與既有「再戰」並列。
   - 標題頁同時是退出點：視窗關閉或「離開」→ 結束程式。
5. **`tools/package_demo.bat`** — 複製 MinGW 版 exe + `assets/`（maps+fonts）→
   `dist/MingGoRTS-Demo/`；附 README.txt（操作說明）。MinGW 靜態連結自包含，
   不需 vcredist。

## 不做（延後）

- 音效（miniaudio milestone）、command-room/ink-chronicle 主題、
  minimap、完整設定頁（語言/密度/急停選項）、教學併入主流程
  （DuanqiaoTutorial 維持獨立 exe）、安裝程式。

## 驗收

- `dist/MingGoRTS-Demo/DuanqiaoPlayable.exe` 在乾淨目錄（無 repo）雙擊可玩全流程
- 主題切換即時生效且不位移 HUD 錨點
- CJK 文字無方塊字（字體自包含，不依賴系統字體）
- MSVC + MinGW 建置過；既有測試無回歸（DemoAssets/UITheme 無需新測試，
  但 ResolveAssetPath 可加 headless 斷言——放 Examples 內隨 demo 驗證）
