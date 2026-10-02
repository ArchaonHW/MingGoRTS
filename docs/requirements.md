# Requirements Document

## Introduction

MingGoRTS（民國史詩 RTS）是一款回合制 RPG 與即時戰略混合的戰術遊戲，執行於自研 PotatoEngine（C++20）。玩家不直接控制單位，而是透過編寫 doctrine 卡片（trigger → condition → action）來指揮戰鬥。遊戲圍繞兩大哲學軸：「至聖者無戰」（不戰而屈人之兵）與「治平者無勝」（治理優於軍事勝利）。

本規格書定義 MingGoRTS 專案的核心使用者需求，涵蓋遊戲機制、引擎架構、戰役系統、雙層世界、AI 系統與開發工具。

## Glossary

- **System**: MingGoRTS 遊戲系統整體
- **PotatoEngine**: 自研 C++20 遊戲引擎
- **Doctrine**: 玩家編寫的戰術卡片（trigger → condition → action → modifier）
- **BattleController**: 戰鬥控制器，管理戰鬥狀態機（Planning → Execution → Aftermath）
- **Squad**: 具名小隊，遊戲中的基本作戰單位
- **QuantumFog**: 機率雲霧系統，處理戰場不確定性
- **Roster**: 名冊系統，追蹤小隊狀態與傷亡
- **RefitCamp**: 整補營地，處理醫治、招募與掠奪
- **BattleRecorder**: 戰鬥錄製回放系統
- **Ledger**: 複式記帳系統（武功/民心/天命/軍威/物資）
- **GovernanceField**: 治理追蹤系統（民心、秩序、墮落）
- **Campaign**: 戰役系統，管理跨章節持久狀態
- **ChapterLibrary**: 章節定義庫
- **MythLog**: 神話事件記錄系統
- **HistorianReport**: 史官戰報組裝器
- **IDE**: MingGoRTS 整合開發環境
- **Player**: 遊戲玩家
- **Developer**: 遊戲開發者

## Requirements

### 需求 1：Doctrine 卡片系統

**使用者故事：** 作為玩家，我想要編寫 doctrine 卡片來指揮戰鬥，以便實現「以筆代兵」的策略規劃。

#### 驗收標準

1. THE System SHALL 解譯 doctrine 卡片結構，包含 trigger、condition、action 與 modifier 四個組件
2. WHEN 玩家定義 trigger 條件，THE System SHALL 監控戰場狀態並在觸發條件滿足時執行卡片
3. WHEN 玩家定義 condition 條件，THE System SHALL 評估條件真假並決定是否執行 action
4. WHEN 玩家定義 action 指令，THE System SHALL 執行對應的單位行為（移動、攻擊、防守等）
5. WHERE 玩家定義 modifier，THE System SHALL 修改 action 的執行參數（速度、範圍、優先級等）
6. THE System SHALL 維護每張 doctrine 卡片的冷卻時間，防止重複觸發
7. WHEN doctrine 卡片處於冷卻中，THE System SHALL 顯示剩餘冷卻時間並禁止觸發
8. THE System SHALL 支援至少 20 種不同的 trigger 類型（敵人接近、生命值低於閾值、時間經過等）
9. THE System SHALL 支援至少 15 種不同的 condition 類型（敵人數量、地形類型、友軍距離等）
10. THE System SHALL 支援至少 25 種不同的 action 類型（進攻、撤退、包抄、守備、支援等）

### 需求 2：戰鬥狀態機

**使用者故事：** 作為玩家，我想要在三個階段中管理戰鬥流程，以便清晰地規劃、執行與檢討戰術。

#### 驗收標準

1. THE BattleController SHALL 實作三拍狀態機，包含 Planning、Execution 與 Aftermath 階段
2. WHEN 戰鬥開始，THE BattleController SHALL 進入 Planning 階段
3. WHILE 處於 Planning 階段，THE System SHALL 允許玩家編排 doctrine 卡片與部署小隊
4. WHEN 玩家確認規劃完成，THE BattleController SHALL 轉換至 Execution 階段
5. WHILE 處於 Execution 階段，THE System SHALL 執行雙方的 doctrine 腳本並更新戰場狀態
6. WHILE 處於 Execution 階段，THE System SHALL 允許玩家使用稀少的指揮點（CP）介入戰場
7. WHEN 戰鬥結束條件滿足（勝利、失敗或時間耗盡），THE BattleController SHALL 轉換至 Aftermath 階段
8. WHILE 處於 Aftermath 階段，THE System SHALL 計算戰果、更新 Roster、記錄戰報並處理敵將投降
9. THE BattleController SHALL 記錄每個階段的轉換時間戳與觸發原因
10. THE BattleController SHALL 在狀態轉換時發送事件通知，供其他系統訂閱

### 需求 3：量子霧霾系統

**使用者故事：** 作為玩家，我想要面對戰場不確定性，以便體驗真實的情報戰與決策風險。

#### 驗收標準

1. THE QuantumFog SHALL 維護機率雲霧，表示玩家對敵方狀態的不確定性
2. THE QuantumFog SHALL 實作疊加態，允許敵方單位同時處於多個可能位置
3. WHEN 玩家探測特定區域，THE QuantumFog SHALL 執行觀測，減少該區域的不確定性
4. THE QuantumFog SHALL 實作衰減機制，隨時間增加未觀測區域的不確定性
5. THE QuantumFog SHALL 實作糾纏機制，連結相關單位的狀態（如護衛與將領）
6. THE QuantumFog SHALL 支援人格先驗，根據敵將性格調整機率分布
7. WHEN 玩家獲得新情報，THE QuantumFog SHALL 更新機率雲霧並重新計算可能狀態
8. THE QuantumFog SHALL 提供視覺化接口，顯示機率雲霧的密度與確定度
9. THE QuantumFog SHALL 影響 AI 決策，根據不確定性調整戰術選擇
10. THE QuantumFog SHALL 將觀測歷史記錄至 BattleRecorder，供回放驗證

### 需求 4：名冊與整補系統

**使用者故事：** 作為玩家，我想要追蹤具名小隊的狀態並在戰鬥間整補，以便建立長期的戰術規劃與情感連結。

#### 驗收標準

1. THE Roster SHALL 維護所有具名小隊的名冊，包含名稱、兵種、經驗值、士氣與傷亡狀態
2. WHEN 小隊參與戰鬥，THE Roster SHALL 記錄傷亡人數與損失裝備
3. WHEN 戰鬥結束，THE Roster SHALL 持久化小隊狀態至存檔
4. THE RefitCamp SHALL 提供醫治功能，恢復受傷小隊的戰力
5. THE RefitCamp SHALL 提供招募功能，補充小隊的兵員至滿編
6. THE RefitCamp SHALL 提供掠奪功能，從戰利品中獲取裝備與物資
7. WHEN 小隊完全損失，THE Roster SHALL 標記該小隊為「陣亡」並從可用名冊中移除
8. THE Roster SHALL 追蹤小隊的戰鬥歷史，包含參與戰役數、擊殺數與生存時間
9. THE RefitCamp SHALL 計算整補成本，根據小隊損失程度與物資存量
10. THE RefitCamp SHALL 顯示整補預估時間，根據醫治複雜度與招募難度

### 需求 5：戰鬥錄製與回放

**使用者故事：** 作為玩家，我想要錄製與回放戰鬥過程，以便分析戰術決策與學習改進。

#### 驗收標準

1. THE BattleRecorder SHALL 錄製所有戰鬥事件，包含單位移動、攻擊、技能使用與狀態變化
2. THE BattleRecorder SHALL 實作 record-is-truth 原則，以錄製資料為真實來源
3. THE BattleRecorder SHALL 計算 rootHash（完整性根），用於檢測錄製檔篡改
4. WHEN 載入回放檔案，THE BattleRecorder SHALL 驗證 rootHash 並拒絕被篡改的檔案
5. WHEN 回放檔案版本過舊，THE BattleRecorder SHALL 發出降級警告並嘗試相容載入
6. THE BattleRecorder SHALL 支援暫停、快進與倒退回放控制
7. THE BattleRecorder SHALL 支援時間軸跳躍，直接跳至指定時間點
8. THE BattleRecorder SHALL 支援視角切換，檢視雙方視角與完整視角
9. THE BattleRecorder SHALL 記錄 doctrine 觸發事件與執行結果，供戰術分析
10. THE BattleRecorder SHALL 將回放檔案序列化為 JSON 格式（potato.battle_recording/1 schema）

### 需求 6：複式記帳與帳本鏈

**使用者故事：** 作為玩家，我想要透過複式記帳系統追蹤戰役資源與榮譽，以便理解戰役經濟與防範帳本作弊。

#### 驗收標準

1. THE Ledger SHALL 實作五帳戶複式記帳系統：武功、民心、天命、軍威、物資
2. THE Ledger SHALL 確保借貸必相等，每筆交易的借方總和等於貸方總和
3. THE Ledger SHALL 實作 FNV-1a 雜湊鏈，每筆記錄包含前一筆記錄的雜湊值
4. THE Ledger SHALL 提供 Verify 方法，檢測雜湊鏈斷鏈並標記可疑記錄
5. THE Ledger SHALL 提供 SoundnessViolation 方法，偵測借貸不等的偽帳
6. THE Ledger SHALL 提供 InjectForgery 通道，允許對手注入偽帳作為遊戲機制
7. WHEN 偵測到偽帳，THE Ledger SHALL 標記為疑帳並在持久化時保留 suspect 欄位
8. THE Ledger SHALL 採用 append-only 設計，禁止修改或刪除已記錄的帳目
9. THE Ledger SHALL 序列化為 JSON 格式（potato.ledger_chain/1 schema）
10. THE HistorianReport SHALL 讀取 LedgerChain，揭露借貸不符並生成查帳段落

### 需求 7：治理系統

**使用者故事：** 作為玩家，我想要追蹤民心、秩序與墮落，以便實現「治平者無勝」的治理導向玩法。

#### 驗收標準

1. THE GovernanceField SHALL 追蹤戰場治理事件，包含村莊佔領、焚村標記、護輜抵達與劫輜
2. WHEN 玩家佔領村莊，THE GovernanceField SHALL 增加民心值
3. WHEN 玩家焚燬村莊，THE GovernanceField SHALL 減少民心值並增加墮落值
4. WHEN 玩家護送物資抵達，THE GovernanceField SHALL 增加秩序值
5. WHEN 玩家劫掠物資，THE GovernanceField SHALL 減少秩序值並增加墮落值
6. THE Campaign Governance SHALL 累計民心、秩序與墮落跨多場戰鬥
7. THE Governance SHALL 實作墮落 ratchet 機制，墮落值只增不減
8. WHEN 墮落值超過閾值，THE System SHALL 觸發動亂事件
9. THE GovernanceField SHALL 記錄治理事件至 HistorianReport，生成治理段落
10. THE Campaign SHALL 根據治理指標影響結局，高民心與秩序解鎖和平結局

### 需求 8：戰役持久化系統

**使用者故事：** 作為玩家，我想要在多個章節間保留進度與狀態，以便體驗連貫的戰役敘事。

#### 驗收標準

1. THE Campaign SHALL 維護跨章節持久狀態，包含 Roster、Governance、敵將處置與稱號
2. THE Campaign SHALL 提供 CampaignState facade，聚合各子存儲的狀態
3. THE ChapterLibrary SHALL 載入章節定義包，包含地圖、敵將、事件與目標
4. WHEN 玩家完成章節，THE Campaign SHALL 儲存進度至 potato.campaign/1 存檔
5. THE Campaign SHALL 實作 tmp + rename 原子寫入，防止存檔損壞
6. WHEN 載入存檔，THE Campaign SHALL 驗證 schema 版本並拒絕不相容版本
7. THE Campaign SHALL 支援多個存檔槽，至少 10 個自動存檔與 10 個手動存檔
8. THE Campaign SHALL 記錄存檔時間戳、遊戲版本與章節進度
9. WHEN 偵測到壞 schema 或損壞存檔，THE Campaign SHALL 保留現況並提示玩家
10. THE Campaign SHALL 提供匯出功能，將存檔匯出為可分享的 JSON 檔案

### 需求 9：章節管理系統

**使用者故事：** 作為玩家，我想要遊玩不同的章節與劇情分支，以便探索多樣化的戰役體驗。

#### 驗收標準

1. THE ChapterLibrary SHALL 定義至少 7 個可玩章節，每個章節包含獨立故事與目標
2. THE ChapterLibrary SHALL 載入章節定義從 assets/campaign/ 目錄
3. WHEN 玩家選擇章節，THE System SHALL 載入對應的地圖、敵將與事件配置
4. THE ChapterConventions SHALL 實作章回體例，包含題詞、敵將判詞、欲知後事與結局四聲部
5. THE System SHALL 根據玩家選擇與戰役狀態，分支至不同劇情路徑
6. WHERE 章節支援無戰路徑，THE System SHALL 提供談判、嚇阻或顛覆選項
7. THE ChapterLibrary SHALL 定義章節前置條件，限制章節解鎖順序
8. WHEN 玩家滿足前置條件，THE ChapterLibrary SHALL 解鎖新章節
9. THE ChapterLibrary SHALL 提供章節摘要，顯示劇情背景、目標與難度
10. THE System SHALL 在章節間顯示過場動畫或文字敘事，串連劇情

### 需求 10：神話層系統

**使用者故事：** 作為玩家，我想要與神話實體互動，以便體驗雙層世界（歷史層與神話層）的獨特玩法。

#### 驗收標準

1. THE System SHALL 維護神話層，與歷史層同時執行於同一張地圖
2. THE MythLog SHALL 記錄神話事件，包含土地神、狐仙、戰神附體與怨靈鬼軍
3. WHEN 玩家安撫神社，THE System SHALL 增加天命值並提升下場戰鬥的民心加成
4. WHEN 玩家觸怒神祇，THE System SHALL 觸發神話懲罰事件
5. THE System SHALL 實作滲透狀態機（0–3 級），表示神話層對歷史層的影響強度
6. WHEN 滲透等級提升，THE System SHALL 增強神話事件的效果與頻率
7. THE System SHALL 提供神話實體視覺呈現，區別於歷史層單位
8. THE MythLog SHALL 序列化為 JSON 格式（potato.myth_log/1 schema）
9. THE System SHALL 提供滲透掛鉤，允許神話事件修改歷史層戰況
10. THE System SHALL 將神話事件整合至 HistorianReport，生成神話段落

### 需求 11：史官戰報系統

**使用者故事：** 作為玩家，我想要閱讀詳細的戰報，以便回顧戰鬥過程與決策影響。

#### 驗收標準

1. THE HistorianReport SHALL 組裝完整戰報，包含戰役概述、關鍵事件、傷亡統計與治理評估
2. THE HistorianReport SHALL 實作省略計數恆在場機制，記錄省略項目數量
3. WHEN 戰報過長，THE HistorianReport SHALL 省略次要事件並標註「本報告省略 N 項」
4. THE HistorianReport SHALL 包含查帳段落，讀取 LedgerChain 並揭露借貸不符
5. THE HistorianReport SHALL 包含治理段落，總結民心、秩序與墮落變化
6. THE HistorianReport SHALL 包含神話段落，列出神話事件與天命消耗
7. THE HistorianReport SHALL 採用章回體例，包含題詞與欲知後事預告
8. THE HistorianReport SHALL 支援多種輸出格式（文字、HTML、Markdown）
9. THE HistorianReport SHALL 顯示關鍵決策點，標註玩家介入與 doctrine 觸發
10. THE HistorianReport SHALL 提供戰報匯出功能，儲存為獨立檔案

### 需求 12：敵將系統

**使用者故事：** 作為玩家，我想要與具備獨特人格的敵將對戰，以便體驗多樣化的對手策略。

#### 驗收標準

1. THE System SHALL 定義敵將屬性，包含名稱、人格、戰術偏好與歷史背景
2. THE GeneralDossier SHALL 提供敵將判詞，採用聽聞態（第三人稱傳聞）描述
3. THE RivalDeck SHALL 統計玩家慣用 trigger 類型，預寫反制牌組
4. WHEN 敵將學習玩家戰術，THE RivalDeck SHALL 調整 doctrine 卡片配置
5. THE CampaignLedger SHALL 記錄敵將處置（斬首、俘虜、釋放、招降）
6. WHEN 玩家招降敵將，THE System SHALL 將敵將加入可用將領名冊
7. THE System SHALL 根據敵將人格，調整 QuantumFog 的先驗機率
8. THE GeneralDossier SHALL 更新敵將資訊，隨戰役進展揭露更多細節
9. THE System SHALL 為每位敵將定義至少 5 張專屬 doctrine 卡片
10. THE CampaignLedger SHALL 序列化敵將處置記錄為 JSON 格式（potato.campaign_ledger/1 schema）

### 需求 13：小隊模板系統

**使用者故事：** 作為玩家，我想要使用預定義的小隊模板快速部署，以便簡化戰鬥準備流程。

#### 驗收標準

1. THE SquadTemplate SHALL 載入巢狀 JSON 模板，定義小隊組成與裝備
2. THE SquadTemplate SHALL 實作 BudgetedBuild，根據預算限制實例化小隊
3. WHEN 預算不足，THE SquadTemplate SHALL 跳過超預算項目並記錄 skipReasons
4. THE SquadTemplate SHALL 對齊 skipReasons 至每個跳過項目，標註 unknown_id 或 over_budget
5. THE System SHALL 提供至少 10 種預定義小隊模板（步兵、騎兵、炮兵、偵查等）
6. THE SquadTemplate SHALL 支援自定義模板，玩家可儲存常用配置
7. THE SquadTemplate SHALL 驗證模板完整性，偵測未知單位 ID 或裝備 ID
8. THE SquadTemplate SHALL 計算模板成本，顯示實例化所需物資
9. THE System SHALL 在部署階段顯示可用模板清單，支援預覽與選擇
10. THE SquadTemplate SHALL 序列化為 JSON 格式，存放於 assets/squads/ 目錄

### 需求 14：計畫箭頭與量子感知

**使用者故事：** 作為玩家，我想要在計畫階段繪製移動路徑，並根據情報確定度調整策略。

#### 驗收標準

1. THE BattlePlan SHALL 支援繪製計畫箭頭，指示小隊預期移動路徑
2. THE BattlePlan SHALL 整合 QuantumFog，根據情報確定度即時縮放箭頭視覺
3. WHEN 情報確定度高，THE BattlePlan SHALL 顯示清晰完整的計畫箭頭
4. WHEN 情報確定度低，THE BattlePlan SHALL 顯示虛線或半透明的計畫箭頭
5. THE BattlePlan SHALL 提供量子感知加成，探測後提升相關區域的計畫精度
6. THE BattlePlan SHALL 支援多小隊協同計畫，顯示交叉路徑與會合點
7. THE BattlePlan SHALL 顯示預估到達時間，根據地形與小隊速度計算
8. THE BattlePlan SHALL 標註危險區域，根據 QuantumFog 推測敵方可能位置
9. THE System SHALL 在 Execution 階段比對實際路徑與計畫路徑，顯示偏差
10. THE BattlePlan SHALL 將計畫資料序列化至 BattleRecorder，供回放分析

### 需求 15：地圖生成器

**使用者故事：** 作為玩家，我想要遊玩程序生成的地圖，以便獲得可重複遊玩的體驗。

#### 驗收標準

1. THE MapGenerator SHALL 生成戰鬥地圖，包含地形、村莊、道路與障礙物
2. THE MapGenerator SHALL 支援多種地形類型（平原、森林、山地、河流、城鎮）
3. THE MapGenerator SHALL 確保地圖可玩性，保證雙方起始點之間存在可通行路徑
4. THE MapGenerator SHALL 生成戰略要點（橋樑、渡口、高地、補給站）
5. THE MapGenerator SHALL 根據章節設定調整地圖特徵（規模、地形分布、要點密度）
6. THE MapGenerator SHALL 支援種子參數，相同種子生成相同地圖
7. THE MapGenerator SHALL 生成地圖知識層，標註村莊、神社與糧道
8. THE MapGenerator SHALL 驗證生成結果，拒絕不合法的地圖（孤島、無法通行等）
9. THE MapGenerator SHALL 序列化地圖資料為 JSON 格式（potato.map/1 schema）
10. THE System SHALL 提供地圖預覽功能，顯示縮略圖與戰略要點

---

## PotatoEngine 引擎架構需求

### 需求 16：引擎核心系統

**使用者故事：** 作為開發者，我想要穩定可靠的引擎核心，以便構建遊戲功能。

#### 驗收標準

1. THE PotatoEngine SHALL 實作單例模式，提供全域唯一引擎實例
2. THE PotatoEngine SHALL 提供 Initialize 方法，接受 EngineConfig 配置參數
3. THE PotatoEngine SHALL 實作主循環（RunMainLoop），以固定時間步驟更新遊戲狀態
4. THE PotatoEngine SHALL 提供 Shutdown 方法，安全釋放所有子系統資源
5. THE PotatoEngine SHALL 管理子系統生命週期，包含 Renderer、Physics、Audio、Input、GUI、ResourceManager、AI
6. THE PotatoEngine SHALL 實作性能監控（PerformanceMetrics），追蹤 FPS、幀時間、CPU 與記憶體使用率
7. THE PotatoEngine SHALL 支援時間縮放（TimeScale），允許慢動作或加速模式
8. THE PotatoEngine SHALL 提供 Pause 與 Resume 功能，暫停遊戲邏輯更新
9. THE PotatoEngine SHALL 採用 C++20 標準，禁用不安全 C 函式（gets、strcpy、sprintf 等）
10. THE PotatoEngine SHALL 編譯通過 MSVC 與 MinGW，確保跨編譯器相容性

### 需求 17：事件系統

**使用者故事：** 作為開發者，我想要解耦的事件系統，以便模組間通訊不產生循環依賴。

#### 驗收標準

1. THE Event_System SHALL 定義 Event 結構，包含事件類型、時間戳與資料負載
2. THE Event_System SHALL 提供 RegisterEvent 方法，允許模組訂閱事件類型
3. THE Event_System SHALL 提供 EmitEvent 方法，廣播事件至所有訂閱者
4. THE Event_System SHALL 實作線程安全，使用 mutex 保護事件佇列
5. THE Event_System SHALL 支援至少 20 種內建引擎事件（ResourceLoaded、SceneChanged、InputReceived 等）
6. THE Event_System SHALL 支援自定義事件類型，允許遊戲層擴充
7. THE Event_System SHALL 實作優先級佇列，確保高優先級事件優先處理
8. THE Event_System SHALL 提供 UnregisterEvent 方法，允許模組取消訂閱
9. THE Event_System SHALL 限制事件負載大小至 1KB，防範記憶體濫用
10. THE Event_System SHALL 記錄事件處理時間，用於性能分析

### 需求 18：資源管理系統

**使用者故事：** 作為開發者，我想要統一的資源管理，以便高效載入與釋放遊戲資源。

#### 驗收標準

1. THE ResourceManager SHALL 提供 LoadResource 方法，接受檔案路徑與資源類型
2. THE ResourceManager SHALL 返回 ResourceHandle，作為資源的唯一識別符
3. THE ResourceManager SHALL 實作資源快取，避免重複載入相同資源
4. THE ResourceManager SHALL 提供 UnloadResource 方法，釋放指定資源的記憶體
5. THE ResourceManager SHALL 提供 IsResourceLoaded 方法，查詢資源載入狀態
6. THE ResourceManager SHALL 提供 ClearResourceCache 方法，釋放所有快取資源
7. THE ResourceManager SHALL 發送 ResourceLoaded 與 ResourceUnloaded 事件
8. THE ResourceManager SHALL 支援異步載入，避免阻塞主執行緒
9. THE ResourceManager SHALL 追蹤資源引用計數，當引用歸零時自動釋放
10. THE ResourceManager SHALL 支援資源熱重載，允許執行期間更新資源

### 需求 19：場景管理系統

**使用者故事：** 作為開發者，我想要管理多個場景，以便組織遊戲關卡與介面。

#### 驗收標準

1. THE Scene_System SHALL 提供 CreateScene 方法，建立新場景並返回 SceneHandle
2. THE Scene_System SHALL 提供 LoadScene 方法，從檔案載入場景定義
3. THE Scene_System SHALL 提供 UnloadScene 方法，卸載場景並釋放資源
4. THE Scene_System SHALL 提供 SwitchScene 方法，切換至指定場景
5. THE Scene_System SHALL 管理場景生命週期（OnLoad、OnUnload、OnActivate、OnDeactivate）
6. THE Scene_System SHALL 發送 SceneLoaded、SceneUnloaded 與 SceneChanged 事件
7. THE Scene_System SHALL 支援多場景共存，允許背景場景與前景 UI 並存
8. THE Scene_System SHALL 序列化場景資料為 JSON 格式（potato.scene/1 schema）
9. THE Scene_System SHALL 驗證場景完整性，拒絕載入損壞的場景檔案
10. THE Scene_System SHALL 追蹤活動場景，提供 GetActiveScene 方法

### 需求 20：渲染系統

**使用者故事：** 作為開發者，我想要靈活的渲染系統，以便支援 2D 與 3D 圖形。

#### 驗收標準

1. THE PotatoRenderer SHALL 提供抽象介面，支援多種渲染後端（OpenGL、DirectX、Vulkan）
2. THE PotatoRenderer SHALL 提供 Initialize 方法，接受 RenderConfig 配置參數
3. THE PotatoRenderer SHALL 提供 BeginFrame、EndFrame 與 Present 方法，控制幀生命週期
4. THE PotatoRenderer SHALL 提供 DrawMesh 方法，渲染 3D 網格
5. THE PotatoRenderer SHALL 提供 DrawTexture 方法，渲染 2D 紋理與精靈
6. THE PotatoRenderer SHALL 提供 SetViewport、SetClearColor 與 SetShader 方法
7. THE PotatoRenderer SHALL 支援 Sprite Atlas，批次渲染減少 draw call
8. THE PotatoRenderer SHALL 追蹤渲染統計（draw calls、三角形數、紋理切換次數）
9. THE PotatoRenderer SHALL 支援多視口渲染，允許分割畫面與小地圖
10. THE PotatoRenderer SHALL 實作 VSync 控制，防止畫面撕裂

### 需求 21：物理系統

**使用者故事：** 作為開發者，我想要物理模擬，以便實現單位碰撞與地形互動。

#### 驗收標準

1. THE PotatoPhysics SHALL 整合 Bullet Physics 引擎
2. THE PotatoPhysics SHALL 提供 CreateRigidBody 方法，建立剛體物件
3. THE PotatoPhysics SHALL 提供碰撞形狀（Box、Sphere、Capsule、Mesh）
4. THE PotatoPhysics SHALL 提供 SetGravity 與 GetGravity 方法
5. THE PotatoPhysics SHALL 實作碰撞偵測，發送 CollisionEnter 與 CollisionExit 事件
6. THE PotatoPhysics SHALL 支援剛體屬性（質量、摩擦力、彈性、阻尼）
7. THE PotatoPhysics SHALL 提供 Raycast 方法，射線檢測場景物件
8. THE PotatoPhysics SHALL 支援觸發器（Trigger），偵測物件進入區域
9. THE PotatoPhysics SHALL 提供物理材質，定義表面摩擦與彈性
10. THE PotatoPhysics SHALL 允許禁用物理模擬，支援純 2D 或回合制遊戲

### 需求 22：音訊系統

**使用者故事：** 作為開發者，我想要播放音效與音樂，以便增強遊戲沉浸感。

#### 驗收標準

1. THE PotatoAudio SHALL 整合 OpenAL Soft 音訊函式庫
2. THE PotatoAudio SHALL 提供 LoadSound 方法，載入音效檔案（WAV、OGG、MP3）
3. THE PotatoAudio SHALL 提供 PlaySound 與 StopSound 方法
4. THE PotatoAudio SHALL 提供 LoadMusic 方法，載入背景音樂
5. THE PotatoAudio SHALL 提供 PlayMusic 與 StopMusic 方法，支援循環播放
6. THE PotatoAudio SHALL 提供 SetMasterVolume 方法，調整全域音量
7. THE PotatoAudio SHALL 支援音效分組（SFX、Music、Voice），獨立音量控制
8. THE PotatoAudio SHALL 支援 3D 音訊，根據音源位置與聽者位置計算音量與聲道
9. THE PotatoAudio SHALL 實作音訊資源池，限制同時播放音效數量
10. THE PotatoAudio SHALL 提供淡入淡出功能，平滑音樂過渡

### 需求 23：輸入系統

**使用者故事：** 作為開發者，我想要處理多種輸入設備，以便支援鍵盤、滑鼠與手把。

#### 驗收標準

1. THE PotatoInput SHALL 整合 GLFW 輸入處理
2. THE PotatoInput SHALL 提供 IsKeyPressed 與 IsKeyJustPressed 方法
3. THE PotatoInput SHALL 提供 IsMouseButtonPressed 方法
4. THE PotatoInput SHALL 提供 GetMousePosition 與 GetMouseDelta 方法
5. THE PotatoInput SHALL 支援遊戲手把輸入，提供 IsGamepadConnected 方法
6. THE PotatoInput SHALL 提供 GetGamepadAxis 方法，讀取類比搖桿數值
7. THE PotatoInput SHALL 實作輸入映射，允許自定義按鍵綁定
8. THE PotatoInput SHALL 發送 InputReceived 事件，通知輸入變化
9. THE PotatoInput SHALL 支援輸入記錄與回放，用於測試與 demo
10. THE PotatoInput SHALL 提供防抖處理，避免按鈕誤觸

### 需求 24：GUI 系統

**使用者故事：** 作為開發者，我想要整合 ImGui，以便快速建立除錯與編輯器介面。

#### 驗收標準

1. THE PotatoGUI SHALL 整合 Dear ImGui 函式庫
2. THE PotatoGUI SHALL 提供 BeginFrame 與 EndFrame 方法，控制 GUI 幀生命週期
3. THE PotatoGUI SHALL 提供 Render 方法，渲染 GUI 至畫面
4. THE PotatoGUI SHALL 支援 ImGui 完整控件集（Button、Slider、Text、Input、Tree 等）
5. THE PotatoGUI SHALL 支援可停靠面板（Docking），允許自定義佈局
6. THE PotatoGUI SHALL 支援多視窗，允許彈出式編輯器視窗
7. THE PotatoGUI SHALL 提供主題系統，支援 Dark、Light 與 High Contrast 主題
8. THE PotatoGUI SHALL 整合至 PotatoInput，自動處理 GUI 輸入攔截
9. THE PotatoGUI SHALL 支援中日韓字體，正確顯示非 ASCII 文字
10. THE PotatoGUI SHALL 提供 GUI 序列化，儲存與載入佈局設定

### 需求 25：序列化系統

**使用者故事：** 作為開發者，我想要統一的序列化介面，以便儲存與載入遊戲資料。

#### 驗收標準

1. THE Serialization_System SHALL 提供 JsonParser，解析與生成 JSON 資料
2. THE Serialization_System SHALL 使用 Potato::JsonValue 作為統一 JSON 型別，禁止第三方 JSON 函式庫
3. THE Serialization_System SHALL 支援 JSON schema 版本化，格式為 potato.<name>/<version>
4. THE Serialization_System SHALL 驗證 schema 版本，拒絕載入不相容版本
5. THE Serialization_System SHALL 實作 tmp + rename 原子寫入，防止存檔損壞
6. THE Serialization_System SHALL 提供 Serialize 與 Deserialize 模板方法，支援自定義型別
7. THE Serialization_System SHALL 支援巢狀結構，正確處理陣列與物件
8. THE Serialization_System SHALL 提供錯誤報告，指出解析失敗的行號與原因
9. THE Serialization_System SHALL 支援 UTF-8 編碼，正確處理中日韓文字
10. THE Serialization_System SHALL 提供 Pretty Print 功能，格式化 JSON 輸出便於人工閱讀

---

## AI 與機器學習需求

### 需求 26：AI Agent 系統

**使用者故事：** 作為開發者，我想要 AI Agent 框架，以便實現敵方戰術 AI 與開發輔助。

#### 驗收標準

1. THE AIAgentSystem SHALL 支援 10 種代理類型（Developer、Designer、Analyst、Tester、Debugger、Researcher、Multimodal、Planner、Communicator、ToolUser）
2. THE AIAgentSystem SHALL 實作感知系統，支援 6 種感知類型（視覺、聽覺、戰術、資源、社交、時間）
3. THE AIAgentSystem SHALL 實作記憶系統，支援 5 種記憶類型（短期、長期、工作、情節、語義）
4. THE AIAgentSystem SHALL 實作記憶重要性評分，根據重要性與可及性排序記憶
5. THE AIAgentSystem SHALL 實作時間衰減，自動清理過期記憶
6. THE AIAgentSystem SHALL 實作協作系統，支援哈希特長匹配與負載平衡任務分配
7. THE AIAgentSystem SHALL 實作計畫系統，支援任務分解、依賴管理與資源分配
8. THE AIAgentSystem SHALL 提供 IntelligenceMetrics，追蹤代理的智慧指標
9. THE AIAgentSystem SHALL 支援多代理並行執行，處理至少 10 個同時活動的代理
10. THE AIAgentSystem SHALL 提供線程安全保證，支援多執行緒環境

### 需求 27：神經網路模組

**使用者故事：** 作為開發者，我想要訓練神經網路，以便實現學習型 AI 行為。

#### 驗收標準

1. THE NeuralNetwork SHALL 實作多層感知器（MLP），支援任意層數與神經元數量
2. THE NeuralNetwork SHALL 支援 5 種激活函數（Sigmoid、ReLU、Tanh、Leaky ReLU、Softmax）
3. THE NeuralNetwork SHALL 支援 3 種損失函數（MSE、Cross Entropy、Binary Cross Entropy）
4. THE NeuralNetwork SHALL 實作反向傳播演算法，訓練網路權重
5. THE NeuralNetwork SHALL 支援 mini-batch 訓練，提升訓練效率
6. THE NeuralNetwork SHALL 提供序列化功能，儲存與載入訓練完成的模型
7. THE NeuralNetwork SHALL 提供 ConvLayer，實作卷積神經網路（CNN）
8. THE NeuralNetwork SHALL 提供 LSTMLayer，實作長短期記憶網路（LSTM）
9. THE NeuralNetwork SHALL 提供 AttentionLayer，實作注意力機制
10. THE NeuralNetwork SHALL 追蹤訓練指標（損失值、準確率、訓練時間）

### 需求 28：強化學習模組

**使用者故事：** 作為開發者，我想要訓練強化學習代理，以便實現自適應敵方 AI。

#### 驗收標準

1. THE ReinforcementLearning SHALL 實作 Q-Learning 演算法
2. THE ReinforcementLearning SHALL 實作 Deep Q-Network（DQN），整合神經網路與經驗回放
3. THE ReinforcementLearning SHALL 實作 Policy Gradient 演算法
4. THE ReinforcementLearning SHALL 實作 Actor-Critic 演算法
5. THE ReinforcementLearning SHALL 實作 Multi-Armed Bandit 演算法
6. THE ReinforcementLearning SHALL 提供 RLEnvironment 介面，定義狀態、動作與獎勵
7. THE ReinforcementLearning SHALL 提供 GridWorld 參考實作，供測試使用
8. THE ReinforcementLearning SHALL 支援 3 種探索策略（ε-greedy、UCB、Boltzmann）
9. THE ReinforcementLearning SHALL 實作獎勵塑形，加速學習收斂
10. THE ReinforcementLearning SHALL 提供課程學習，逐步增加任務難度

### 需求 29：自然語言處理模組

**使用者故事：** 作為開發者，我想要處理自然語言，以便實現對話系統與文本分析。

#### 驗收標準

1. THE NaturalLanguageProcessing SHALL 提供 Tokenizer，分詞中英文文本
2. THE NaturalLanguageProcessing SHALL 提供 POSTagger，標註詞性
3. THE NaturalLanguageProcessing SHALL 提供 NER（Named Entity Recognition），識別實體
4. THE NaturalLanguageProcessing SHALL 提供 SentimentAnalyzer，分析情感傾向
5. THE NaturalLanguageProcessing SHALL 提供 IntentRecognizer，識別使用者意圖
6. THE NaturalLanguageProcessing SHALL 提供 TextEmbedding，將文本轉換為向量
7. THE NaturalLanguageProcessing SHALL 提供 TextGenerator，生成文本回應
8. THE NaturalLanguageProcessing SHALL 提供 TextSummarizer，摘要長文本
9. THE NaturalLanguageProcessing SHALL 提供 QuestionAnswering，回答基於上下文的問題
10. THE NaturalLanguageProcessing SHALL 提供 NLPPipeline，串接多個 NLP 組件

### 需求 30：LLM 整合模組

**使用者故事：** 作為開發者，我想要整合大型語言模型，以便實現智慧對話與代碼生成。

#### 驗收標準

1. THE LLMIntegration SHALL 支援 OpenAI API（GPT-4、GPT-3.5）
2. THE LLMIntegration SHALL 支援 Anthropic API（Claude 3）
3. THE LLMIntegration SHALL 支援本地模型（Ollama、llama.cpp）
4. THE LLMIntegration SHALL 提供統一 API，抽象不同供應商的差異
5. THE LLMIntegration SHALL 支援串流聊天，即時返回生成內容
6. THE LLMIntegration SHALL 支援工具調用（Function Calling），整合外部工具
7. THE LLMIntegration SHALL 提供 Embedding API，生成文本向量
8. THE LLMIntegration SHALL 實作自動降級，當主模型不可用時切換至備用模型
9. THE LLMIntegration SHALL 實作速率限制，防止超出 API 配額
10. THE LLMIntegration SHALL 實作響應快取，減少重複請求成本

### 需求 31：RAG 系統

**使用者故事：** 作為開發者，我想要檢索增強生成系統，以便提升 LLM 回答準確性。

#### 驗收標準

1. THE RAGSystem SHALL 實作內存向量資料庫，儲存文檔 Embedding
2. THE RAGSystem SHALL 支援文檔分塊，將長文檔切分為可檢索的片段
3. THE RAGSystem SHALL 實作餘弦相似度搜尋，檢索相關文檔
4. THE RAGSystem SHALL 實作混合檢索，結合關鍵字與向量搜尋
5. THE RAGSystem SHALL 實作 BM25 演算法，提升關鍵字檢索準確性
6. THE RAGSystem SHALL 提供文檔重排序，根據相關性調整結果順序
7. THE RAGSystem SHALL 支援知識庫管理，匯入與匯出文檔集合
8. THE RAGSystem SHALL 整合 LLMIntegration，自動生成 Embedding
9. THE RAGSystem SHALL 提供檢索統計，追蹤查詢數、命中率與回應時間
10. THE RAGSystem SHALL 支援多語言檢索，正確處理中英日韓文本

### 需求 32：Agent Chain 系統

**使用者故事：** 作為開發者，我想要編排多個 Agent，以便實現複雜的多步驟任務。

#### 驗收標準

1. THE AgentChain SHALL 支援 Sequential Chain，依序執行多個 Agent
2. THE AgentChain SHALL 支援 Parallel Chain，並行執行多個 Agent
3. THE AgentChain SHALL 支援 Router Chain，根據條件選擇執行路徑
4. THE AgentChain SHALL 支援 Loop Chain，重複執行直到條件滿足
5. THE AgentChain SHALL 支援 Map-Reduce Chain，分散處理後聚合結果
6. THE AgentChain SHALL 實作 Agent Graph（DAG），支援複雜依賴關係
7. THE AgentChain SHALL 執行拓撲排序，確保依賴順序正確
8. THE AgentChain SHALL 偵測循環依賴，拒絕無效的 DAG
9. THE AgentChain SHALL 生成 DOT 格式圖形，視覺化 Agent 鏈結構
10. THE AgentChain SHALL 提供 ChainBuilder 流式介面，簡化鏈建構

### 需求 33：Tool Framework

**使用者故事：** 作為開發者，我想要工具框架，以便擴充 Agent 能力。

#### 驗收標準

1. THE ToolFramework SHALL 提供工具註冊機制，動態新增工具
2. THE ToolFramework SHALL 提供工具發現機制，列出可用工具與參數
3. THE ToolFramework SHALL 實作參數驗證，確保工具輸入合法
4. THE ToolFramework SHALL 實作安全執行，限制工具執行時間與資源
5. THE ToolFramework SHALL 提供至少 14 種內建工具（文件操作、Web 搜尋、代碼執行、系統查詢、數據處理等）
6. THE ToolFramework SHALL 支援工具鏈，依序或條件執行多個工具
7. THE ToolFramework SHALL 整合 LLMIntegration，支援 LLM 工具調用
8. THE ToolFramework SHALL 記錄工具執行歷史，用於除錯與審計
9. THE ToolFramework SHALL 實作工具權限系統，限制敏感工具使用
10. THE ToolFramework SHALL 提供工具模板，簡化自定義工具開發

### 需求 34：智能開發系統

**使用者故事：** 作為開發者，我想要 AI 輔助開發，以便自動生成與優化代碼。

#### 驗收標準

1. THE IntelligentDevelopmentSystem SHALL 支援代碼生成，根據描述生成多語言代碼
2. THE IntelligentDevelopmentSystem SHALL 支援代碼分析，計算圈複雜度與可維護性指數
3. THE IntelligentDevelopmentSystem SHALL 支援重構建議，提供前後對比與置信度
4. THE IntelligentDevelopmentSystem SHALL 支援測試生成，自動產生單元測試
5. THE IntelligentDevelopmentSystem SHALL 支援文檔自動化，生成 API 文檔與註釋
6. THE IntelligentDevelopmentSystem SHALL 支援 Bug 修復，分析錯誤並提供修正建議
7. THE IntelligentDevelopmentSystem SHALL 支援性能優化，識別效能瓶頸並建議改進
8. THE IntelligentDevelopmentSystem SHALL 支援依賴分析，檢測循環依賴與未使用模組
9. THE IntelligentDevelopmentSystem SHALL 提供 DevelopmentAssistant，互動式問答與代碼解釋
10. THE IntelligentDevelopmentSystem SHALL 實作反饋學習，根據開發者回饋調整建議

---

## MingGoRTS IDE 需求

### 需求 35：IDE 核心架構

**使用者故事：** 作為開發者，我想要整合開發環境，以便高效開發 MingGoRTS 內容。

#### 驗收標準

1. THE MingGoRTS_IDE SHALL 提供 IDECore，管理專案、編輯器、導航器與建置器
2. THE MingGoRTS_IDE SHALL 提供 IDEProject，管理專案結構與設定
3. THE MingGoRTS_IDE SHALL 提供 IDEEditor，支援多標籤頁代碼編輯
4. THE MingGoRTS_IDE SHALL 提供 IDENavigator，瀏覽專案檔案樹
5. THE MingGoRTS_IDE SHALL 提供 IDEBuilder，整合 CMake 建置系統
6. THE MingGoRTS_IDE SHALL 整合 AI Agent，提供智慧代碼補全與分析
7. THE MingGoRTS_IDE SHALL 提供 5 種專門化遊戲開發代理（EngineCode、Asset、LevelDesign、Performance、Build）
8. THE MingGoRTS_IDE SHALL 感知 PotatoEngine API，提供引擎特定建議
9. THE MingGoRTS_IDE SHALL 理解資產類型（Texture、Model、Sound），提供資產管理
10. THE MingGoRTS_IDE SHALL 提供 CMake 優化建議，分析建置配置

### 需求 36：IDE GUI

**使用者故事：** 作為開發者，我想要圖形介面，以便視覺化操作 IDE 功能。

#### 驗收標準

1. THE IDE_GUI SHALL 基於 ImGui、GLFW 與 OpenGL 3.0 實作
2. THE IDE_GUI SHALL 提供完整菜單欄（File、Edit、View、Build、AI Agent、Help）
3. THE IDE_GUI SHALL 提供 6 個可停靠面板（文件瀏覽器、代碼編輯器、AI Agent 交互、終端、輸出日誌、屬性）
4. THE IDE_GUI SHALL 支援標籤頁編輯器，顯示多個開啟檔案
5. THE IDE_GUI SHALL 顯示修改標記（*），標示未儲存檔案
6. THE IDE_GUI SHALL 提供工具欄，快速存取常用功能（New、Open、Save、Build 等）
7. THE IDE_GUI SHALL 提供狀態欄，顯示光標位置、編碼、檔案狀態與建置進度
8. THE IDE_GUI SHALL 提供搜尋與替換面板，支援大小寫與全字匹配
9. THE IDE_GUI SHALL 採用深色主題，支援 Light 與 High Contrast 主題切換
10. THE IDE_GUI SHALL 支援面板快捷鍵（Ctrl+1~5 切換面板、Ctrl+, 設定、Ctrl+? 幫助）

### 需求 37：IDE 進階功能

**使用者故事：** 作為開發者，我想要除錯與版本控制整合，以便完整的開發工作流程。

#### 驗收標準

1. THE IDE_GUI SHALL 提供調試器面板，支援 F5/F10/F11/Shift+F11 調試控制
2. THE IDE_GUI SHALL 支援斷點管理，設定、啟用與禁用斷點
3. THE IDE_GUI SHALL 提供表達式求值，檢視變數值
4. THE IDE_GUI SHALL 提供 Git 面板，支援 status、pull、push 與 commit
5. THE IDE_GUI SHALL 顯示分支管理，切換與建立分支
6. THE IDE_GUI SHALL 顯示暫存與修改檔案列表
7. THE IDE_GUI SHALL 提供自動完成，根據上下文建議代碼片段
8. THE IDE_GUI SHALL 提供代碼分析面板，顯示 error、warning 與 info
9. THE IDE_GUI SHALL 支援自動修復，點擊應用建議修正
10. THE IDE_GUI SHALL 提供語法檢查，整合診斷至分析面板

### 需求 38：IDE 國際化

**使用者故事：** 作為開發者，我想要多語言支援，以便全球開發者使用 IDE。

#### 驗收標準

1. THE IDE_I18N SHALL 支援 5 種語言（English、繁體中文、簡體中文、日文、韓文）
2. THE IDE_I18N SHALL 提供 TranslationKey 枚舉，定義至少 80 個翻譯鍵
3. THE IDE_I18N SHALL 提供 I18NManager，管理語言切換與翻譯查詢
4. THE IDE_I18N SHALL 提供 T() 宏，簡化翻譯鍵使用
5. THE IDE_I18N SHALL 覆蓋主要介面 100%，包含菜單、面板、按鈕與狀態消息
6. THE IDE_I18N SHALL 支援實時語言切換，無需重啟 IDE
7. THE IDE_I18N SHALL 提供至少 400 條翻譯條目
8. THE IDE_I18N SHALL 使用 ASCII 兼容源碼，避免編譯器編碼警告
9. THE IDE_I18N SHALL 提供翻譯編輯器，允許新增與修改翻譯
10. THE IDE_I18N SHALL 支援翻譯匯入匯出，便於社群貢獻

### 需求 39：IDE 智能建議系統

**使用者故事：** 作為開發者，我想要智能建議，以便提升代碼質量與開發速度。

#### 驗收標準

1. THE IntelligentSuggestionSystem SHALL 提供 8 種建議類型（CodeCompletion、Refactoring、Optimization、BugFix、BestPractice、Documentation、TestGeneration、Architectural）
2. THE IntelligentSuggestionSystem SHALL 使用神經網路排序建議，根據相關性
3. THE IntelligentSuggestionSystem SHALL 使用 NLP 分析上下文，提升建議準確性
4. THE IntelligentSuggestionSystem SHALL 使用 Embedding 執行語義分析
5. THE IntelligentSuggestionSystem SHALL 實作反饋學習，根據接受率調整建議
6. THE IntelligentSuggestionSystem SHALL 提供實時建議，輸入時自動觸發
7. THE IntelligentSuggestionSystem SHALL 實作防抖處理，避免頻繁觸發
8. THE IntelligentSuggestionSystem SHALL 提供建議面板，顯示類型、置信度與詳情
9. THE IntelligentSuggestionSystem SHALL 支援 Apply 與 Dismiss 操作
10. THE IntelligentSuggestionSystem SHALL 達成性能目標：建議生成 <100ms、分析 <50ms、UI 60 FPS

### 需求 40：IDE 模板系統

**使用者故事：** 作為開發者，我想要檔案模板，以便快速建立常用檔案結構。

#### 驗收標準

1. THE IDE_Templates SHALL 提供至少 7 種檔案模板（C++ Class、Header、Source、Lua、Python、JSON、Markdown）
2. THE IDE_Templates SHALL 支援自定義模板，允許開發者新增專案特定模板
3. THE IDE_Templates SHALL 提供模板變數替換，自動填入檔案名稱、日期、作者等
4. THE IDE_Templates SHALL 顯示模板預覽，允許確認後建立
5. THE IDE_Templates SHALL 整合至 File 選單與右鍵選單
6. THE IDE_Templates SHALL 支援多檔案模板，一次建立相關檔案（如 .h 與 .cpp）
7. THE IDE_Templates SHALL 提供模板編輯器，視覺化編輯模板內容
8. THE IDE_Templates SHALL 支援模板分類，組織模板為引擎、遊戲、測試等類別
9. THE IDE_Templates SHALL 驗證模板語法，防止無效模板
10. THE IDE_Templates SHALL 提供模板匯入匯出，分享模板至社群

---

## 非功能性需求

### 需求 41：效能需求

**使用者故事：** 作為玩家，我想要流暢的遊戲體驗，以便享受遊戲而不受卡頓干擾。

#### 驗收標準

1. THE System SHALL 維持至少 60 FPS 於 1920x1080 解析度下，包含至少 100 個活動單位
2. THE System SHALL 載入戰鬥場景於 5 秒內
3. THE System SHALL 載入存檔於 3 秒內
4. THE System SHALL 回應玩家輸入於 100 毫秒內
5. THE System SHALL 處理 doctrine 觸發於 50 毫秒內
6. THE System SHALL 限制記憶體使用至 4GB 以下
7. THE System SHALL 支援至少 200 個同時存在的小隊
8. THE QuantumFog SHALL 更新機率雲霧於 16 毫秒內（每幀）
9. THE BattleRecorder SHALL 錄製事件不影響遊戲效能，開銷 <5%
10. THE System SHALL 支援熱重載資源，更新時間 <1 秒

### 需求 42：可測試性需求

**使用者故事：** 作為開發者，我想要全面的測試覆蓋，以便確保代碼品質與正確性。

#### 驗收標準

1. THE System SHALL 支援 headless 測試，無需 OpenGL context
2. THE System SHALL 提供至少 85 個可執行測試檔案，涵蓋引擎與遊戲層
3. THE System SHALL 整合 CTest，支援自動化測試執行
4. THE System SHALL 要求新功能必須附帶單元測試
5. THE System SHALL 支援模擬測試（Mock），隔離外部依賴
6. THE System SHALL 記錄測試覆蓋率，目標 >80%
7. THE System SHALL 提供測試輸出檔案驗證，檢查 replay/roster JSON 正確性
8. THE System SHALL 執行回歸測試，防止功能退化
9. THE System SHALL 支援性能測試，追蹤效能指標變化
10. THE System SHALL 提供測試報告，匯出為 HTML 或 Markdown

### 需求 43：安全性需求

**使用者故事：** 作為開發者，我想要安全的代碼實踐，以便防範常見漏洞與攻擊。

#### 驗收標準

1. THE System SHALL 禁用不安全 C 函式（gets、strcpy、strcat、sprintf、vsprintf、scanf）
2. THE System SHALL 使用安全替代函式（strncpy、snprintf、strncat）
3. THE System SHALL 驗證所有外部輸入，包含檔案路徑、JSON 資料與使用者指令
4. THE System SHALL 限制檔案存取至專案目錄內，防範路徑遍歷攻擊
5. THE System SHALL 驗證 JSON schema 版本，拒絕不相符或未知版本
6. THE System SHALL 實作 rootHash 驗證，偵測回放檔篡改
7. THE System SHALL 限制 Ledger 操作為 append-only，防止歷史篡改
8. THE System SHALL 隔離 AI Agent 執行環境，限制檔案與系統存取
9. THE System SHALL 記錄安全事件（非法存取、驗證失敗），用於審計
10. THE System SHALL 執行定期安全稽核，使用靜態分析工具掃描漏洞

### 需求 44：可維護性需求

**使用者故事：** 作為開發者，我想要清晰的代碼結構，以便長期維護與擴充專案。

#### 驗收標準

1. THE System SHALL 採用分層架構（PotatoEngine ← Gameplay ← Campaign ← Examples）
2. THE System SHALL 確保依賴方向單向，禁止循環依賴
3. THE System SHALL 要求所有公開 API 包含文檔註釋
4. THE System SHALL 限制檔案長度至 1000 行以內
5. THE System SHALL 限制函式複雜度，圈複雜度 <15
6. THE System SHALL 採用一致的命名規範（PascalCase 類別、camelCase 變數、UPPER_CASE 常數）
7. THE System SHALL 提供架構文檔，說明各模組職責與互動
8. THE System SHALL 使用 Conventional Commits 規範，標準化提交訊息
9. THE System SHALL 執行代碼審查，所有 PR 需至少一位審查者批准
10. THE System SHALL 記錄重大變更於 CHANGELOG.md

### 需求 45：相容性需求

**使用者故事：** 作為開發者，我想要跨平台支援，以便在多種環境中建置與測試。

#### 驗收標準

1. THE System SHALL 編譯通過 Windows（MSVC 與 MinGW）
2. THE System SHALL 編譯通過 Linux（GCC）
3. THE System SHALL 支援 CMake ≥ 3.15
4. THE System SHALL 支援 C++20 標準
5. THE System SHALL 驗證雙編譯器相容性，本地建置需同時通過 MSVC 與 MinGW
6. THE System SHALL 使用 if(WIN32 AND NOT MSVC) 守衛 MinGW 專用連結
7. THE System SHALL 禁止修改 external/ 既有內容，新第三方庫僅以新增子目錄方式引入
8. THE System SHALL 使用 FetchContent 自動拉取依賴（如 GLFW）
9. THE System SHALL 避免硬編碼路徑，使用相對路徑與環境變數
10. THE System SHALL 執行持續整合（CI），自動建置與測試多平台

### 需求 46：文檔需求

**使用者故事：** 作為使用者與開發者，我想要完整的文檔，以便理解系統功能與開發指南。

#### 驗收標準

1. THE System SHALL 提供 README.md，說明專案概述、建置步驟與目錄結構
2. THE System SHALL 提供 GDD（遊戲設計文檔），詳述遊戲機制與哲學
3. THE System SHALL 提供技術架構文檔，說明分層設計與模組職責
4. THE System SHALL 提供 API 文檔，涵蓋所有公開介面
5. THE System SHALL 提供使用者指南，教導玩家如何遊玩與編寫 doctrine
6. THE System SHALL 提供開發者指南，說明如何擴充引擎與遊戲功能
7. THE System SHALL 提供貢獻指南（CONTRIBUTING.md），定義開發規範與 PR 流程
8. THE System SHALL 提供安全政策（SECURITY.md），說明漏洞回報流程
9. THE System SHALL 提供變更日誌（CHANGELOG.md），記錄版本歷史與重大變更
10. THE System SHALL 提供授權文件（LICENSE），明確開源授權條款

---

## 總結

本需求規格書定義了 MingGoRTS 專案的 46 項核心需求，涵蓋：

- **遊戲機制**（Doctrine、戰鬥、量子霧霾、名冊、錄製、帳本、治理、戰役、章節、神話、戰報、敵將、模板、計畫、地圖）
- **引擎架構**（核心、事件、資源、場景、渲染、物理、音訊、輸入、GUI、序列化）
- **AI 系統**（Agent、神經網路、強化學習、NLP、LLM 整合、RAG、Agent Chain、Tool Framework、智能開發）
- **開發工具**（IDE 核心、GUI、進階功能、國際化、智能建議、模板）
- **非功能性**（效能、可測試性、安全性、可維護性、相容性、文檔）

所有需求均遵循 EARS 模式與 INCOSE 質量規則，確保清晰、可測試與完整。

---

**文件版本：** 1.0  
**建立日期：** 2026-09-XX  
**Schema：** potato.requirements/1
