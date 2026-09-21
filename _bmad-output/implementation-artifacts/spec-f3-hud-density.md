---
title: 'F-3 HUD 密度三檔（minimal/standard/verbose）——特徵閘門 + 設定持久化'
type: 'feature'
created: '2026-09-22'
status: 'in-review'
context:
  - '{project-root}/_bmad-output/implementation-artifacts/spec-f2-ui-scale-pipeline.md'
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/Examples/UISettings.h'
---

## Intent

**Problem:** 執行層 HUD 常駐資訊 12+ 項（階段/CP/情報/時間/未揭露敵軍/
選取明細/操作提示/敵將檔案/全軍列表/戰況卷軸/小地圖），全部玩家被迫
全收。epics F.3 + EXPERIENCE 要求資訊密度玩家可調。

**Approach:** `HUDDensity`（Minimal/Standard/Verbose）+ `HUDFeature`
枚舉的純函數閘門 `HUDShown(feature, density)`——面板層級裁撤，
不是逐行裁撤；門檻矩陣集中在 `Examples/HUDDensity.h`（header-only
同 UITheme 慣例）。`UISettings::Data` 加 `hudDensity`（可選欄位，
舊檔缺省 Standard）；設定頁加三檔 combo，沿用 F-2 dirty+落盤管線。

## Boundaries & Constraints

**Always:**
- 門檻矩陣單一來源：`HUDShown()` 純函數，headless 可測
- 關鍵情報三檔恆在：階段/CP/戰鬥時鐘、全軍面板（士氣/潰逃條是
  雙編碼關鍵情報也是 CP 介入控制面）、結算/戰後視窗
- 密度只裁「資訊冗餘」不裁「控制面」——玩家仍可下令/介入
- `hudDensity` 為 potato.settings/1 可選欄位：舊檔無欄→Standard；
  越界值鉗回 0..2（ClampScale 同規）
- banned C 函式禁令；header-only；不引第三方庫

**Never:**
- 不在 Minimal 裁撤控制面（全軍介入按鈕/下令/暫停恆可用）
- 不改既有面板的內部排版——只在 Begin 外層加閘門
- 不裁結算/戰敗視窗（戰果是敘事必要節點）

## Design

**`Examples/HUDDensity.h`（新，header-only）：**
```cpp
enum class HUDDensity : int { Minimal = 0, Standard = 1, Verbose = 2 };
enum class HUDFeature {
    EventLog,      // 戰況卷軸
    Minimap,       // 小地圖
    EnemyDossier,  // 敵將檔案
    SquadDetail,   // 斷橋指揮內選取小隊明細
    HelpHints,     // 操作提示行
    FogIntel,      // 未揭露敵軍計數/霧情報（verbose 專屬）
    ExecStats,     // 執行率/帳本計數列（verbose 專屬）
};
bool HUDShown(HUDFeature f, HUDDensity d);
HUDDensity HUDDensityFromInt(int);   // 越界鉗回
const char* HUDDensityName(HUDDensity);  // 精簡/標準/詳盡
```

矩陣：Minimal 全 false（面板只剩恆在件）；Standard 開前五項
（EventLog~HelpHints）；Verbose 全開。

**`UISettings::Data` 增補：** `int hudDensity = 1`（Standard 序數）；
Save 寫 `"hudDensity"`，Load 用 `HUDDensityFromInt` 鉗制。

**`DuanqiaoPlayable.cpp` 閘門點：**
- 斷橋指揮：階段/CP/時間恆在；`未揭露敵軍` 行 → FogIntel；
  選取明細 → SquadDetail；操作提示三行 → HelpHints
- 敵將檔案 → EnemyDossier；戰況 → EventLog；小地圖 → Minimap
- 全軍/作戰計畫/結算 → 無閘（控制面與敘事節點）
- 設定頁：HUD 密度 combo，變更即 dirty 落盤（沿用 F-2 管線）

## Tasks

- [ ] `Examples/HUDDensity.h`（枚舉 + HUDShown 矩陣 + FromInt/Name）
- [ ] `UISettings.h`：`hudDensity` 欄位 + 存取 + 鉗制
- [ ] `DuanqiaoPlayable.cpp`：閘門點 + 設定 combo
- [ ] `Examples/HUDDensityTest.cpp` + CMakeLists 註冊

## I/O 矩陣（測試）

| 輸入 | 預期 |
|---|---|
| Minimal | 全部可裁特徵 false；恆在件無閘門 |
| Standard | EventLog/Minimap/Dossier/SquadDetail/HelpHints 全 true |
| Verbose | 全特徵 true |
| FromInt(-1/0/1/2/99) | 鉗回 0..2 |
| settings 存取 hudDensity | roundtrip 一致 |
| 舊檔無 hudDensity 欄 | 載入 → Standard(1) |
| 檔案 hudDensity=99 | 鉗回 Verbose(2) |

## Verification

- `HUDDensityTest` 全綠（MinGW+MSVC）
- `DuanqiaoPlayable` 雙工具鏈建置通過
- banned 函式掃描無新增命中
