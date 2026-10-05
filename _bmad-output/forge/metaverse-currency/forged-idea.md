---
idea: metaverse-currency
status: hardened
date: 2026-10-05
---

# Forged Idea — 鏈上存證 + NFT 收藏層 (Epic 11 輸入)

## 定案 (Locks)

- **幣種**:虛擬代幣發行於測試網(Sepolia 或本地 Anvil)。真實區塊鏈技術棧、幣值為零、零法規風險。
- **鏈架構**:混合式 — 遊戲內帳本延伸出本地鏈原語(Merkle tree、鑄幣規則),Merkle root 定期錨定上測試網,實現「鏈上存證」。
- **鑄幣觸發(雙軌)**:① BattleRecorder 重放驗證通過的章節結算鑄幣(proof-of-battle);② 稀有成就額外鑄幣(零戰鬥通關、偵破偽帳、四聲部結局)。
- **代幣用途**:鑄造 NFT 收藏品 — 章回箋插畫、本草圖鑑、名將檔案。「元宇宙」在此 = 玩家持有的鏈上收藏/身份層,不是共享持久世界。
- **整合方式**:外部 relayer — 遊戲只輸出 versioned JSON mint-claim + Merkle root;獨立腳本(Node/Python/Foundry cast)負責讀 claim、調合約、上鏈。引擎零網路依賴(已確認引擎無 socket/HTTP 層),符合 GDD「工具走 versioned-JSON 邊界」先例。
- **落地**:新增 Epic 11,走 correct-course → epic → sprint 正規流程。

## 否決 (Kills)

- **主網真錢 P2E** — 單機遊戲無交易對手、無經濟體支撐幣值;發幣+流動性有成本與法規風險;與 GDD「非商業個人專案」直接衝突。
- **引擎內建 RPC client** — 引擎無網路層,私鑰/簽名放進 C++ 引擎風險高、工作量大,收益不成比例。
- **玩家間交易資產** — 單機無交易對手,需多人/社群才有意義;v1.0 排除多人。

## 邊界 (Boundaries)

- 代幣**不**是五帳戶之一 — 是帳本外的「元貨幣」,保護 deterministic sim 不被網路污染。
- 鏈上互動**只**發生在 aftermath/章節邊界,tick path 零網路、零檔案 I/O 原則不變。
- 私鑰永不進 repo;relayer 從環境變數/本地設定讀 dev wallet。

## 存活弱點 (Accepted Risks)

- 全 client-side 無法真防作弊 — 玩家可自己產 claim 叫 relayer 鑄幣。可接受:測試網幣值為零,防作弊叙事(replay 驗證)是展示重點而非安全保證。
- 一人開發、Epic 6/10 進行中 — Epic 11 規模須控制,建議先最小切片(本地鏈 + claim + relayer 打測試網)。
