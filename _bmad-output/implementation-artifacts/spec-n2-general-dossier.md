---
title: 'N-2 敵將判詞（聽聞態，非真相態）'
type: 'feature'
created: '2026-09-18'
status: 'draft'
baseline_commit: 'ee1fc5b'
context:
  - _bmad-output/implementation-artifacts/auto-dev-queue.yaml (id: N-2)
  - spec: deps EnemyGeneral 三軸(done) / 敵情霧觀測(done)
---

## Objective

敵將檔案面板加「判詞」行——**傳聞值**，系統性低估（固定可學習偏差）；
偵查/觀測可驗證或推翻；高狡詐敵將能放假判詞進傳聞。
帳本寫聽聞，不寫真相——「據報」是敘事層的核心修辭。

## Approach

**1. `Gameplay/GeneralDossier.h/.cpp`（新，純邏輯可測）**

```cpp
struct HearsayEntry {
    std::string generalName;
    std::string verdict;       // 判詞一句（傳聞體）
    float estAggression;       // 聽聞估值三軸（有系統性偏差）
    float estDiscipline;
    float estCunning;
    bool verified = false;     // 觀測驗證後為真相
    bool planted = false;      // 高狡詐者放假判詞（驗證後才揭曉）
};
```

- `Hear(const EnemyGeneral&)` — 建傳聞檔案：
  - 估值偏差寫死：`est = true × 0.8`（系統性低估，玩家可學習修正）
  - 判詞由**顯性軸**產生：aggression≥70「其人好戰鋒不可當」、
    discipline≥70「持重守紀難以誘之」、cunning≥70「多詐善變
    不可盡信其形」、皆中平「資質中庸無足深慮」
  - **cunning ≥ 70 放假判詞**：判詞改寫最弱軸的反向描述
    （如實為狡詐 85/侵略 30 → 判詞「其人怯戰」），planted=true，
    估值照寫偏差（謊在文字不在數）
- `Verify(const EnemyGeneral&)` — 觀測驗證：三軸回真值、
  verified=true；planted 檔案驗證時 verdict 附加「（前判有詐）」
- `Reveal(...)`/`Entries()` 讀取介面

**2. `DuanqiaoPlayable` 最小接線**：主 HUD 旁「敵將檔案」窗格——
開局 `Hear(glock)` 顯示判詞+聽聞三軸；玩家觀測到格洛克親衛的雲
（fog Observe 命中 entity id）→ `Verify` → 面板分層呈現
「傳聞」與「實情」。檔案熱區，僅加一個小窗。

**3. `Examples/GeneralDossierTest.cpp`** 入 POTATO_TESTS：
低估偏差固定規則、三軸判詞對應、狡詐放假判詞（cunning≥70
verdict 與真實主軸不符 + planted 旗標）、Verify 回真值 +
「前判有詐」標記、中庸軸判詞。

## Out of scope

- 假判詞反向欺敵（放假情報進敵方傳聞）——敵軍無 meta 層
- 判詞影響玩法數值（純情報呈現層）
- dossier UX 規格書之外的完整面板（只做最小窗格）

## Acceptance

- [ ] 檔案顯示判詞（聽聞）與驗證後真相分層
- [ ] 低估偏差為固定規則（×0.8），觀測後更新
- [ ] 狡詐≥70 放假判詞有測試覆蓋
- [ ] GeneralDossierTest 全綠；MSVC + MinGW；無回歸

## Files

- new: `Gameplay/GeneralDossier.h`, `Gameplay/GeneralDossier.cpp`,
  `Examples/GeneralDossierTest.cpp`
- edit: `Examples/DuanqiaoPlayable.cpp`（敵將檔案窗格）、
  `CMakeLists.txt`
