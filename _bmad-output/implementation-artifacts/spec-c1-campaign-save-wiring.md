---
title: 'C-1 戰役存檔接線（章節邊界存讀 + 首章定義）'
type: 'feature'
created: '2026-09-19'
status: 'done'
route: 'queue-pick'
baseline_revision: '60ac7df'
---

## Intent

E-7 骨架（`CampaignState` facade + `potato.campaign/1`）已存在，
但 demo 只建構物件、從不存讀——跨執行緒進度不持久。
C-1 把存檔接進 DuanqiaoPlayable 的章節邊界，並補上
`assets/campaign/` 首章定義檔，讓戰役第一次真的能「存檔再戰」。

## Boundaries

**In scope:**

- `assets/campaign/` 新目錄 + `potato.campaign_chapter/1` schema：
  `{schema,id,name,arc,chapter,map,enemy_deck}`；首檔 `duanqiao.json`
- `Campaign/ChapterLibrary.h/.cpp`：依 SquadTemplateLibrary 慣例的
  `LoadFromFile`/`LoadDir`/`Find(id)`（schema 嚴驗、壞檔跳過+警告）
- `DuanqiaoPlayable` 接線：
  - 殼層啟動：`campaign.LoadFromFile(kSavePath)`——檔不存在視為新戰役
    （不報錯），存在則還原整補營/名冊/帳本/章節
  - 章節定義載入：由 `assets/campaign/` 取當前章 `ChapterDef`
  - 戰後結算（既有 `battle.GetOutcome() != Ongoing` 區塊）：
    `AdvanceChapter`（依章節定義推進）+ `SaveToFile(kSavePath)`
- 存檔路徑：`saves/campaign.json`（相對執行目錄，目錄不存在則建）

**Out of scope:**

- 多存檔槽位 / 戰中存檔（架構寫死章節邊界才存）
- 章節選擇 UI / 戰役地圖（E-7 後續）
- Governance/GodStance 子系統（C-2/E-6）

## I/O Contract

- 首次啟動（無存檔）→ 新戰役，章節=duanqiao 定義檔
- 戰鬥結束（任一 outcome）→ 寫出合法 `potato.campaign/1`；
  勝利時 `AdvanceChapter` 推進章節序
- 重啟 → 整補營兵力/戰利品、名冊生死、帳本、章節全數還原
- 存檔壞/schema 錯 → `LoadFromFile` false 且現況不動（先驗證再置換）
- 章節定義檔壞 → 跳過+警告，不擋啟動

## Acceptance Criteria

- [ ] `assets/campaign/duanqiao.json` 可被 ChapterLibrary 載入
- [ ] 戰後寫出合法 campaign 檔；重啟還原整補營/章節狀態
- [ ] headless 測試入 ctest；MSVC + MinGW 建置通過；無回歸

## Verification

- `cmake --build build --config Release --target ChapterLibraryTest`
- `cd build && ctest -C Release -R "ChapterLibrary|CampaignState"`
- `cmake --build build-mingw --target ChapterLibraryTest`
