# Decision Log — MingGoRTS GDD

| Date | Decision | Rationale |
|---|---|---|
| 2026-09-17 | Create mode; source input = existing game brief | Brief already encodes genre/pillars/MVP — discovery focuses on what's new or open |
| 2026-09-17 | Narrative complexity target: Heavy | User selected in narrative pre-check; GDD must leave room for commander-biography campaign structure |
| 2026-09-17 | Design values: 至聖者無戰 / 治平者無勝 | From 《至聖者指揮學》《治平者管理學》 — already partially mechanized (SageOutcome::SubdueWithoutWar / GovernedPeace) |
| 2026-09-17 | Game type = strategy primary + tactics secondary | RTS 即時執行 + 小隊戰術為主軸；RPG 元素（名冊/傳記）為內容層不拉成主類型 |
| 2026-09-17 | 至聖者/治平者 = 獨立 pillar（無戰而勝）+ 結局軸 | 五級結局梯已實作；戰場與戰役皆由民心/秩序/歸附驅動 |
| 2026-09-17 | 民心/秩序 = 雙層並行 | 場內即時反應（村莊/佔領/投降）+ 戰役持久累積（治績） |
| 2026-09-17 | 無戰 = 可不戰通關 | 指定治平章節提供外交/威懾/收編路線；最高評價層 |
| 2026-09-17 | 神話層 = 民心放大器 | 安撫神祇/平息怨靈直接轉化民心秩序——治平者操作場域 |
| 2026-09-17 | Express 模式起草 | brief 已解決大部分面，只補哲學結構決策 |
| 2026-09-17 | 對帳+學科校驗後 patch 一輪 | 補回 Grok/4→7 槽位/戰鬥時長/急停鈕/配樂機制/戰神附身；沙盒模擬器移出 v1.0 out-of-scope（brief 定為必需工具） |
| 2026-09-17 | 墮落 = 單向棘輪（by design） | 無還原路徑——暴行線是設計好的陷阱；投降/收屍是唯一反制 |
| 2026-09-17 | 治績 = 民心+秩序的戰役複合指標 | 併入資源表，不成為第四條未命名累計器 |
| 2026-09-17 | Draw 為戰鬥層結果，映射入結局梯 | 對齊實作：BattleOutcome::Draw → SageOutcome 解析為 GovernedPeace/Defeat |
| 2026-09-17 | 無戰而勝升為 pillar 2（取代 brief 的 History×Myth pillar 位） | 使用者明確要求以至聖者/治平者為結構骨架；History×Myth 降為內容結構層仍 load-bearing |
