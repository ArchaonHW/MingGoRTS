---
title: 'E-7 戰役框架（章節容器 + CampaignState facade）'
type: 'feature'
created: '2026-09-18'
status: 'draft'
baseline_commit: '2a57c19'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (N-4/N-6 依賴)
  - _bmad-output/game-architecture.md §Campaign State / §Save/Load
---

## Objective

戰役層骨架：`Campaign/` 新目錄，`CampaignState` facade 聚合
跨章節狀態，單一 `potato.campaign/1` 檔在章節邊界原子寫入。
解鎖 N-4（敵將處置持久化）與 N-6（章節慣例文字層）。

## Approach

**1. `Campaign/ChapterState.h`（header-only 小結構）**

```cpp
struct ChapterState {
    int arc = 0;              // 弧（軍閥/北伐/抗戰/內戰）
    int chapter = 0;          // 章節序
    std::string chapterId;    // "duanqiao" 等內容 id
    std::vector<std::string> frontsTaken; // 已收服戰線 id
};
```

**2. `Campaign/Dispositions.h`**——敵將處置與稱號（N-4 資料模型先落）

```cpp
enum class Disposition { Unknown, Subdued, Executed, Defected,
                         Retired, AtLarge };
struct DispositionRecord {
    std::string generalName;
    Disposition state = Disposition::Unknown;
    int recordedAtChapter = -1; // 記帳章節
    std::vector<std::string> titles; // 稱號軌跡（「斷橋軍師」等）
};
```

**3. `Campaign/CampaignState.h/.cpp`——facade**

- 持有 `RefitCamp`（實體）、`Roster`（實體）、`ChapterState`、
  `Dispositions` 清單、稱號帳（`TitleTrack`：我軍稱號字串列）
- `SaveToFile/LoadFromFile`：`potato.campaign/1` JSON，
  **tmp+rename 原子寫**，schema 不符/壞檔 `Log::Error` + false
  （架構§Error Handling：拒絕大聲、不丟資料）
- `RefitCamp`/`Roster` 各補 `ToJson()/FromJson()` 產生
  `JsonValue` 子物件，既有檔案 API 改包一層（不破壞現行用法）
- 預留空段 `governance`/`god_stance`/`intel_ledger` 進 schema——
  E-5/E-6/E-11 後續填充，載入時缺段不報錯

**4. `Examples/CampaignStateTest.cpp`** 入 POTATO_TESTS：
存讀檔 roundtrip（章節/fronts/處置/稱號/RefitCamp 兵力傷兵/
Roster 生死）、schema 拒絕、原子寫（讀檔中 tmp 檔殘留不影響）、
處置記帳查詢。

**5. DuanqiaoPlayable 最小接線**：殼層外 `CampaignState campaign`
持有整補營（`camp` 改為 `campaign.Camp()` 引用）——檔案熱區
僅一行換引用來源，其餘接線等敘事內容進場再做。

## Out of scope

- 戰役地圖 UI / 章節選擇流程（等敘事內容與 E-11）
- Governance/GodStance/IntelLedger 實作（E-5/E-6 各自 spec）
- 戰中存檔（架構寫死章節邊界才存）
- 多存檔槽位

## Acceptance

- [ ] `potato.campaign/1` 存讀檔 roundtrip 全欄位保存
- [ ] 原子寫（tmp+rename）；壞檔拒絕大聲不丟資料
- [ ] RefitCamp/Roster 狀態經 campaign 檔跨章節持續
- [ ] Dispositions/TitleTrack 模型就位（N-4 直接消費）
- [ ] CampaignStateTest 全綠；MSVC + MinGW；無回歸

## Files

- new: `Campaign/CampaignState.h/.cpp`, `Campaign/ChapterState.h`,
  `Campaign/Dispositions.h`, `Examples/CampaignStateTest.cpp`
- edit: `Gameplay/RefitCamp.h/.cpp`（ToJson/FromJson）、
  `Gameplay/Roster.h/.cpp`（ToJson/FromJson）、
  `Examples/DuanqiaoPlayable.cpp`（camp → campaign.Camp()）、
  `CMakeLists.txt`
