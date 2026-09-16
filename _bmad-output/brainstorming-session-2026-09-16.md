---
title: 'Game Brainstorming Session'
date: '2026-09-16'
author: 'potat'
version: '1.0'
stepsCompleted: [1, 2]
status: 'in-progress'
---

# Game Brainstorming Session

## Session Info

- **Date:** 2026-09-16
- **Facilitator:** Game Designer Agent
- **Participant:** potat
- **Seed direction:** 回合制 RPG × 即時戰略戰鬥模式（hybrid）

---

_Ideas will be captured as we progress through the session._

## Brainstorming Approach

**Selected Mode:** YOLO（全技法驅動）

**Techniques Available:**
Genre Mashup / Core Loop / MDA / Player Fantasy Mining / Verbs Before Nouns /
Failure State Design / Progression Curve / Emotion Targeting / What-If /
Remix Existing Game / Constraint Box / Emergence Engineering /
Ludonarrative Harmony / Reward Schedule Architecture / Morphological Analysis /
SCAMPER / Economy Balancing / Toy Before Game / Anti-Game Design /
First Principles / Social Dynamics / Spectator Design

**Focus Areas:**
- 回合制 RPG × 即時戰略戰鬥的縫合切點（何時回合、何時即時、誰控制什麼）
- 玩家動詞：「佈署」vs「介入」vs「微操」的光譜定位
- RPG 成長（角色/裝備/羈絆）如何映射到 RTS 單位與戰場
- 失敗設計：回合玩家 vs 即時玩家的容錯期待落差
- 引擎可行性：MingGoRTS/PotatoEngine 現有 ECS、事件、物理、輸入能撐什麼玩法

**Ideation 回合規劃（Party Mode 產出）：**
1. Core Loop 切分 —— 回合層產出「命令」還是「資源」？
2. 玩家動詞 —— 指揮官視角 vs 微操視角的主從
3. 失敗與犧牲 —— 有名有姓的單位戰損作為敘事代價
4. Mashup 狂想 —— What-If / Remix 打開邊界
5. 原型收斂 —— 最小可行戰鬥原型定義

**Party Mode / Advanced Elicitation 產生的設計洞察：**
- 切點光譜：全事前規劃（自走棋端）← Gambit/腳本式（潛在甜區）→ 全微操（純 RTS 端）
- 核心二分：回合層產出「命令」vs 產出「資源」
- 三拍情緒循環：掌控（回合運籌）→ 懸疑/心痛（即時執行）→ 哀悼與重建（戰後回合）
- 突變株點子：FF12 Gambit × RTS＝玩家寫 AI 腳本打仗的程式對戰 RPG
- 失敗機制構想：即時劣勢彈回回合層做「斷後犧牲」決策——機制即主題

**Advanced Elicitation 第二輪洞察：**
- 形態四軸矩陣（設計空間地圖，256 種組合）：
  - 誰下指令：玩家本人 / AI 腳本 / NPC 副官（玩家只控英雄）
  - 時間流動：永遠即時 / 指令階段暫停 / 限次子彈時間 / 慢速即時
  - 什麼持續：角色養成 / 傷亡名單 / 裝備領地 / **敵方學習你的戰術**
  - 失敗落地：讀檔 / 永久死亡 / 撤退保殘部 / **戰敗推進劇情**
- 第一性原理推論：回合層 = 深思區，只放需要 deliberation 的決策
  （戰前情報、編成、doctrine 撰寫、外交取捨；即時做也沒差的不放）
- 反設計突變株：戰敗解鎖敘事線（流亡/被俘營救/游擊）——輸也是分岔點
- Doctrine 排程作為玩具本體（Opus Magnum 式時間軸編排），
  天生可觀賞可分享——「匯出 doctrine 給朋友挑戰」
- Gambit 式路線的最大缺口：RPG 玩家「想微操但做不到」的初期沮喪期

**Advanced Elicitation 第三輪洞察（系統層 × 引擎現實）：**
- 回合資源經濟四幣種：情報 / 指令槽 / 士氣 / 指揮點數(CP)
  —— 情報與指令槽互斥（多偵查就少寫腳本）製造核心取捨
- 成長綁動詞：升級解鎖腳本表達力而非數值
  （單條件 → 邏輯鏈 → 跨隊聯動觸發 → 頂點解鎖 CP 即時介入）
  build 多樣性 = 指揮風格，不是屬性配點
- 引擎順勢形狀（PotatoEngine 現況反推）：
  - 無尋路 → 小隊為最小單位，squad-level flow field 即可
  - EventBus 現成 → doctrine 觸發器天然映射（OnHealthBelow30 → 動作）
  - 物理碰撞現成 → 地形互動玩法（推落石、點燃油漬、衝鋒擊飛）
- SCAMPER 突變：指令卡組化 / 腳本過熱（寫太多降精度）/
  戰後繳獲敵方 doctrine / 部署反轉（敵人先看你的陣再出牌）
- 非同步 doctrine 對戰：上傳防守腳本給朋友攻打 = 單機獲得 metagame 壽命
