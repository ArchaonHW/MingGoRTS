# Spec: L 層 — 帳本機械化（區塊鏈與會計概念導入）

status: draft
created: 2026-09-19
source: 使用者「加入區塊鏈與會計系統概念」

## 設計公理對位

本作核心公理「帳本與筆」——記錄是完整帳、書寫是節選帳、回放即審計。
區塊鏈與會計不是外加系統，而是把這條公理**機械化**：

| 概念 | 遊戲映射 | 現有地基 |
|---|---|---|
| 複式記帳 | 每筆分錄借貸必相等——戰功的借方必有代價的貸方 | CampaignLedger / Roster 傷亡 / BattleResources |
| 帳戶分類 | 武功・民心・天命・軍威・物資 五帳戶 | 治理軸 + GodStance + 補給 |
| Hash 鏈 append-only | 帳目逐筆鏈結，篡改舊帳即鏈驗證失敗 | CampaignLedger 記錄流 |
| Merkle root | 回放檔雜湊根——回放驗證 = 審計證明 | BattleRecorder / potato.battle_replay/1 |
| 試算平衡 | 章節邊界 balance check——借貸不平 = 帳目有誤 | AdvanceChapter |
| 偽帳稽核 | N-3 對手軍師/神話滲透可注入假帳；驗證揭露 | N-3 反制牌組 + D-1 滲透 + N-5 MythLog |

**範圍守衛**：取機制不取炒作——無共識演算法、無代幣經濟、
無網路層。單機遊戲的「區塊鏈」= append-only 雜湊鏈帳簿 +
Merkle 驗證，全部 headless 可測。

## 工作項

### L-1 複式記帳分錄層（M）
- `Gameplay/LedgerEntry`：{debit帳戶, credit帳戶, 金額, 摘要, 章節}
- 五帳戶：martial(武功)/civil(民心)/fate(天命)/army(軍威)/supply(物資)
- 戰役事件自動產生平衡分錄：勝仗 = 借武功 / 貸物資+兵力（傷亡折算）
- 整補營招募 = 借軍威 / 貸物資；無戰談判 = 借民心 / 貸武功（未竟之功）
- 驗收：任意事件序列後各帳戶借貸總和恆相等（trial balance = 0）

### L-2 雜湊鏈 append-only 帳簿（M）
- `Gameplay/LedgerChain`：entry = {prevHash, payload, hash}
  （FNV-1a 或簡易 SHA 自實作——不引第三方庫）
- CampaignLedger 所有記錄改走鏈式追加；`Verify()` 全鏈重算
- 篡改任一歷史 entry → Verify() 回報第一個斷鏈位置
- 驗收：append/verify/篡改偵測/存檔 roundtrip 全 headless

### L-3 試算表與查帳戰報（M）
- 章節邊界產生 trial balance 摘要入戰報：
  「本章記帳 N 筆，借貸相符」或「借貸不符——史官存疑」
- HistorianReport 加 audit 段：不平時產生「墨跡未乾」式懸疑句
- 驗收：平衡章節產正常句、注入不平衡產存疑句

### L-4 偽帳機制（L，依賴 D-1 + N-3）
- 對手軍師讀到玩家慣用 pattern 後，可注入一筆偽造分錄
  （如「彼軍自潰」假戰功）；神話滲透 ≥2 級時偽帳機率上升
- 偽帳不立即破鏈——hash 仍合法，但**試算不平衡**暴露它
- 玩家可在查帳 UI 標記疑帳 → 下一章 N-3 反制失效
- 驗收：注入偽帳 → 試算存疑 → 標記後反制解除，全程入 MythLog

### L-5 回放 Merkle 根（S，依賴 L-2 hash 實作）
- potato.battle_replay/1 加 rootHash 欄位（事件流 Merkle root）
- 載入回放時驗證——驗證通過的回放才准進史官摘錄
- 驗收：合法回放通過、篡改事件被拒、舊版無 hash 回放降級警告

## 敘事加成

- 「借貸必相等」給了「戰功有價」一個機械表達——贏一場仗帳上
  一定有東西被扣掉，戰報可以說「此役之勝，以 N 人命為貸」
- 偽帳 = 對手軍師攻擊你的**記憶系統**——比攻擊單位更「對手軍師」
- 滲透視效（D-5）配合：偽帳暴露時帳本頁面墨跡滲開
