---
title: 'C-2 治理源實裝（民心/秩序戰場來源 + 戰役累加器）'
type: 'feature'
created: '2026-09-19'
status: 'in-progress'
route: 'queue-pick'
baseline_revision: '290e7ff'
---

## Intent

Epic A 剩餘：戰場事件成為民心/秩序的來源（村莊佔領、受降、
護輜、暴行），戰役層 `Campaign/Governance` 跨章節累加，
低秩序觸發動亂事件。分層守衛：Gameplay 不認識 Campaign；
事件走 recorder 事件流（record-is-truth）。

## Boundaries

**In scope:**

- `Gameplay/GovernanceEvent.h`：四事件 enum + delta 表 +
  `EventName`——戰場側唯一真相定義
- `Gameplay/BattleController`：自動偵測
  - 新潰逃敵隊 → `SurrenderAccepted`（每隊一次，吃 routEmitted 慣例）
  - 潰逃中敵隊損員 → `Atrocity`（每隊一次）
  - `RecordGovernanceEvent(ev)` 公開 API：村莊/護輜由有地圖知識的
    呼叫端（DuanqiaoPlayable）餵入；`GetGovernanceEvents()` 回事件計數
  - 事件同時 Emit 進 recorder 事件流
- `Gameplay/SageCommand::RecordGovernanceEvent`：同一 delta 表作用
  戰場民心/秩序（SageCommand 使用者即刻受益）
- `Campaign/Governance.h/.cpp`：戰役累加器——`Accumulate(events)`
  折進 campaign 民心/秩序（戰場 delta × 折半率），`UnrestLevel()`
  0-3 + `PollUnrestEvents()` 動亂事件字串（跨級才發不重複），
  ToJson/FromJson
- `CampaignState`：`Gov()` 掛載 + `governance` 段序列化（替換空佔位）
- `DuanqiaoPlayable`：village/convoy interactable 佔領偵測（每點每場一次）、
  章節邊界 `Gov().Accumulate` + 動亂事件入結算報告
- `assets/maps/duanqiao.json`：+village/convoy interactable 各一
- `Examples/GovernanceTest.cpp` 入 POTATO_TESTS

**Out of scope:**

- 村莊/護輜的專屬玩法機制（佔領進度、護送路線）——v1 佔領即觸發
- 動亂的玩法後果（叛軍生成等）——v1 只出事件
- SageCommand 接進 DuanqiaoPlayable（獨立整合項）

## I/O Contract

- 敵隊新潰逃 → SurrenderAccepted×1 入事件計數 + recorder 字串
- 潰逃隊續掉員 → Atrocity×1（同隊不再重發）
- village/convoy interactable 內有我軍隊 → 各事件一次
- 章節邊界 Accumulate：campaign 民心/秩序依 delta 表×0.5 移動、夾 [0,100]
- campaign 秩序跌破 30/20/10 → 動亂事件逐級發出（不回頭重發）
- 存檔 governance 段 roundtrip 保留數值與動亂級

## Acceptance Criteria

- [ ] 四事件皆能被偵測/記錄且各自只發一次
- [ ] Accumulate 正負向移動正確、夾值正確
- [ ] 動亂級跨檻發事件、平級不發
- [ ] GovernanceTest 全綠入 ctest；MSVC + MinGW；無回歸

## Verification

- `cmake --build build --config Release --target GovernanceTest`
- `cd build && ctest -C Release -R "Governance|CampaignState|SageCommand"`
- `cmake --build build-mingw --target GovernanceTest`
