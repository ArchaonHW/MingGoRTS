---
title: 'F-2 UI scale 真管線（75–150%）——絕對重建 + 全 token 縮放 + 設定持久化'
type: 'feature'
created: '2026-09-21'
status: 'done'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/spec-u1-vertical-slice-demo.md'
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
---

## Intent

**Problem:** U-1 的 uiScale 是粗版——`ScaleAllSizes(ratio)` 比值累乘
（重複調整會漂移），`Apply(theme)` 會把已縮放的 style 打回基底但
appliedScale 不重置（換主題後尺寸失控）；面板尺寸/錨點偏移多處寫死
像素不經 scale；設定不持久化。

**Approach:** 三層補完：(1) `UITheme::ApplyScaled` 從基底 token
絕對重建（Apply + ScaleAllSizes(scale)）——同參數恆同結果；
(2) `UITheme::Px(base, scale)` 把面板/子窗/按鈕的寫死尺寸全走
scale 因子，錨點位置改用 `ww - Px(w+8)` 形式保持右緣貼齊；
(3) `UISettings`（potato.settings/1）持久化 theme+uiScale，
tmp+rename 原子寫、拖曳中不落盤（deactivated 後存）。

## Boundaries & Constraints

**Always:**
- scale 範圍 0.75–1.5，`ClampScale` 單一守衛（滑桿與檔案載入共用）
- 樣式重建必須確定性：ApplyScaled 先 Apply(theme) 再
  ScaleAllSizes(scale)——不動存量比值
- 錨點不動（UX-DR4）：左上 (8,8) 類絕對錨不乘 scale；
  右緣面板用 `ww - Px(width+8)` 讓 8px 邊距也走 scale
- 設定存 `DemoAssets::ExeDir()/saves/settings.json`（與 campaign.json
  同目錄慣例）；缺檔=預設值不報錯、壞檔拒絕載入不動現況
- banned C 函式禁令；不引第三方庫；ImGui 現有依賴範圍內

**Ask First:**
- IDE 主程式（MingGoRTS_IDE）的 uiScale 接線——本項只做
  DuanqiaoPlayable 垂直切片；IDE 共用留待獨立項

**Never:**
- 不改四主題 token 色值
- 不在 UITheme.h 引 Campaign/Gameplay 型別（theme 存 int 序數）

## Design

**`Examples/UITheme.h` 增補：**
```cpp
inline constexpr float kScaleMin = 0.75f, kScaleMax = 1.5f;
inline float ClampScale(float s);
// F-2 絕對重建：Apply 基底 → ScaleAllSizes(scale)，無累乘漂移
inline void ApplyScaled(ImGuiStyle& s, Id id, float scale);
// 面板/子窗寫死尺寸的 scale 閘門
inline float Px(float base, float scale) { return base * ClampScale(scale); }
```

**`Examples/UISettings.h`（新，header-only 同 UITheme 慣例）：**
```cpp
namespace UISettings {
struct Data { int theme = 0; float uiScale = 1.0f; };
bool Load(const std::string& path, Data& out); // 缺檔/壞檔→false
bool Save(const std::string& path, const Data& d); // tmp+rename
}
```
schema：`{"schema":"potato.settings/1","theme":0,"uiScale":1.0}`

**`DuanqiaoPlayable.cpp` 接線：**
- 啟動：`UISettings::Load` → theme/uiScale 初始值
- TitleFrame 樣式段：`theme!=appliedTheme || uiScale!=appliedScale`
  → `ApplyScaled`（修掉換主題不打回 scale 的現存 bug）
- 滑桿 0.75–1.5；theme/scale 變更標 dirty，`!IsAnyItemActive()`
  時落盤（拖曳中不寫檔）
- 全部寫死尺寸過 `Px()`：標題窗 430、開戰鈕 34、章節圖 660×420、
  BeginChild 150/230/300、右上面板 300/308、狀態列 264/272、
  戰報 430×182/190、直書標題偏移 245

## I/O & Edge-Case Matrix

| Scenario | Input / State | Expected Output / Behavior | Error Handling |
|----------|--------------|---------------------------|----------------|
| 絕對重建 | Apply 後 ApplyScaled(1.5) ×2 次 | 兩次 WindowPadding 完全一致 | N/A |
| 主題+縮放 | Apply(theme) 後 ApplyScaled | 尺寸=基底×scale 不殘留舊值 | N/A |
| 邊界鉗制 | ClampScale(0.5/2.0) | 0.75 / 1.5 | 鉗制 |
| 面板尺寸 | Px(300, 1.5) | 450 | N/A |
| 設定 roundtrip | Save→Load | theme/scale 逐值相符 | N/A |
| 檔案值越界 | 檔案 uiScale=3.0 | Load 後鉗回 1.5 | 鉗制 |
| 缺檔 | 無 settings.json | Load=false，預設值不變 | 降級 |
| 壞檔 | 非 JSON/schema 不符 | Load=false，現況不動 | 拒絕 |
| 拖曳中 | slider 拖動未放 | 不落盤；放開才寫 | 節流 |

## Code Map

- `Examples/UITheme.h` — ClampScale/ApplyScaled/Px
- `Examples/UISettings.h` — 新檔，potato.settings/1
- `Examples/DuanqiaoPlayable.cpp` — 載入/儲存/ApplyScaled/Px 全尺寸
- `Examples/UIScaleTest.cpp` — 無頭測試
- `CMakeLists.txt` — 註冊

## Tasks & Acceptance

**Execution:**
- [x] `Examples/UITheme.h` — ClampScale/ApplyScaled/Px
- [x] `Examples/UISettings.h` — 新檔
- [x] `Examples/DuanqiaoPlayable.cpp` — 接線
- [x] `Examples/UIScaleTest.cpp` + CMakeLists 註冊

**Acceptance Criteria:**
- Given 重複切換 scale/theme，when ApplyScaled 兩次同參數，then
  style 尺寸逐位一致（無漂移）
- Given 寫死面板尺寸，when scale=1.5，then 全部經 Px 放大且
  右緣錨點仍貼齊（ww - Px(w+8)）
- Given 修改設定後放開控制項，when 下一幀，then
  saves/settings.json 寫入且重啟可還原
- Given 壞/缺設定檔，when Load，then 回 false 且現況不動

## Verification

- `cmake --build build --config Release --target UIScaleTest`（MSVC）
- `ctest -R "UIScale|UITheme"` — 全綠（UIThemeTest 不迴歸）
- MinGW 編譯 + 執行 — 通過
- DuanqiaoPlayable MinGW/MSVC 建置 — 通過（UI 層無 headless 驗）
- banned 函式掃描 — 無新增命中
