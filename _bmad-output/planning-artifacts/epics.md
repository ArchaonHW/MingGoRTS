---
stepsCompleted: [1, 2, 3, 4]
workflowType: 'epics'
lastStep: 4
status: 'complete'
completedAt: '2026-03-21T22:45:00Z'
inputDocuments: ["c:\\HW\\MingGoRTS\\_bmad-output\\gdd.md", "c:\\HW\\MingGoRTS\\_bmad-output\\planning-artifacts\\architecture.md"]
---

# MingGoRTS - Epic Breakdown

## Overview

This document provides the complete epic and story breakdown for MingGoRTS, decomposing the requirements from the GDD, and Architecture requirements into implementable stories.

## Requirements Inventory

### Functional Requirements

FR1: 實現四層策略系統（戰略、戰術、個人、建造層面）
FR2: 支持1000+單位同時戰鬥的大規模RTS戰鬥系統
FR3: 基於真實民國歷史的決策後果體驗系統
FR4: 角色成長和技能發展系統（騎馬與砍殺風格）
FR5: 基地建設和資源管理系統（Elin風格）
FR6: 動態歷史事件觸發和決策系統
FR7: 多兵種協同作戰系統
FR8: 外交和政治聯盟系統
FR9: 科技樹研發系統
FR10: 跨平台適配（PC、Android、iOS）
FR11: AI輔助美術資產生成系統
FR12: 敘事整合和對話系統
FR13: 勝利/失敗條件系統
FR14: 難度漸進系統（初級、中級、高級）
FR15: 歷史人物扮演和互動系統

### NonFunctional Requirements

NFR1: 60 FPS性能目標（支持1000+單位戰鬥）
NFR2: 跨平台兼容性（Windows、Android、iOS）
NFR3: 模組化插件架構支持獨立開發
NFR4: 事件驅動通訊確保組件間鬆散耦合
NFR5: AI資產管道集成（Stable Diffusion、AIVA）
NFR6: 服務器驗證和客戶端預測網絡架構
NFR7: 歷史真實性和教育價值
NFR8: 安全性和反作弊系統

### Additional Requirements

- 起始模板實施: UE5.4+模組化插件架構
- MingCore事件系統: 層次間通訊基礎
- 四層插件結構: MingStrategic、MingTactical、MingPersonal、MingBuilding
- 性能優化: 二進制戰術數據和事件批處理
- 測試框架: 單元測試、集成測試、性能測試
- CI/CD管道: 平台特定打包和部署

### UX Design Requirements

UX-DR1: 歷史風格的UI設計系統
UX-DR2: 多層次界面導航（戰略地圖、戰役地圖、角色界面、建造界面）
UX-DR3: 觸控和鼠標雙重輸入支持
UX-DR4: 歷史信息展示和教育內容整合
UX-DR5: 大規模戰鬥的可視化清晰度
UX-DR6: 決策後果的視覺反饋系統

### FR Coverage Map

FR1: Epic 5 - 四層策略系統整合
FR2: Epic 2 - 大規模戰鬥核心
FR3: Epic 1 - 歷史決策後果體驗
FR4: Epic 3 - 角色技能發展
FR5: Epic 4 - 基地建設核心
FR6: Epic 1 - 動態歷史事件
FR7: Epic 2 - 多兵種協同作戰
FR8: Epic 5 - 外交政治聯盟
FR9: Epic 4 - 科技樹研發
FR10: Epic 6 - 跨平台適配
FR11: Epic 5 - AI資產生成
FR12: Epic 3 - 敘事對話系統
FR13: Epic 1 - 勝利失敗條件
FR14: Epic 2 - 難度漸進系統
FR15: Epic 3 - 歷史人物互動

## Epic List

### Epic 1: 歷史決策體驗基礎
用戶可以體驗基於真實民國歷史的決策後果
**FRs covered:** FR3, FR6, FR13, FR15

### Epic 2: 大規模戰術戰鬥系統
用戶可以指揮1000+單位進行RTS戰鬥
**FRs covered:** FR2, FR7, FR14

### Epic 3: 角色成長與敘事系統
用戶可以發展角色技能並參與歷史敘事
**FRs covered:** FR4, FR12, FR15

### Epic 4: 基地建設與資源管理
用戶可以建造基地並管理資源
**FRs covered:** FR5, FR9

### Epic 5: 四層策略整合系統
用戶可以在戰略、戰術、個人、建造四個層面進行決策
**FRs covered:** FR1, FR8, FR11

### Epic 6: 跨平台無縫體驗
用戶可以在任何設備上繼續他們的民國史詩旅程，無縫切換平台而不丟失進度
**FRs covered:** FR10 (跨平台適配), NFR2 (跨平台兼容性), NFR3 (模組化架構支持多平台部署)
**User Value:** 玩家可以在PC上進行深度策略規劃，在移動設備上隨時管理資源和快速戰鬥，所有進度實時同步

## Epic 1: 歷史決策體驗基礎

用戶可以體驗基於真實民國歷史的決策後果

### Story 1.1: 歷史決策系統基礎

As a 民國時期的軍官,
I want 在關鍵歷史時刻做出決策,
So that 影響歷史的進程並體驗決策的重量。

**Acceptance Criteria:**

**Given** 玩家處於1920年的民國時期背景
**When** 遊戲呈現第一個歷史決策點（如選擇效忠哪個軍閥）
**Then** 系統顯示決策選項和歷史背景信息
**And** 玩家的選擇被記錄並影響後續遊戲進程
**And** 系統提供決策權重的視覺反饋

### Story 1.2: 動態歷史事件觸發器

As a 民國時期的軍官,
I want 遊戲能根據我的決策動態觸發歷史事件,
So that 體驗真實歷史進程的連鎖反應。

**Acceptance Criteria:**

**Given** 玩家已做出歷史決策
**When** 遊戲時間推進到關鍵歷史節點
**Then** 系統觸發相應的歷史事件（如北伐、抗日戰爭等）
**And** 事件的觸發條件基於玩家之前的決策路徑
**And** 事件提供多個選擇分支
**And** 事件結果影響後續的歷史進程

### Story 1.3: 決策後果計算系統

As a 民國時期的軍官,
I want 看到我的決策如何影響歷史進程,
So that 理解每個選擇的歷史重量和後果。

**Acceptance Criteria:**

**Given** 玩家已做出關鍵歷史決策
**When** 系統計算決策的歷史影響
**Then** 顯示決策對不同歷史路徑的影響程度
**And** 提供歷史知識和教育內容說明後果
**And** 記錄決策對角色聲望、勢力範圍的影響
**And** 更新後續事件的觸發條件

### Story 1.4: 勝利/失敗條件判定

As a 民國時期的軍官,
I want 遊戲根據我的歷史決策路徑判定勝負,
So that 體驗不同歷史結局的成就感或遺憾。

**Acceptance Criteria:**

**Given** 玩家已完成一系列歷史決策
**When** 遊戲達到關鍵歷史節點（如統一中國、失敗流亡等）
**Then** 系統評估玩家的歷史決策路徑
**And** 判定勝利、失敗或持續進行的狀態
**And** 提供歷史結局的描述和評價
**And** 記錄最終的歷史地位和成就

### Story 1.5: 歷史人物互動系統

As a 民國時期的軍官,
I want 與歷史人物進行對話和互動,
So that 體驗真實的民國政治和軍事環境。

**Acceptance Criteria:**

**Given** 玩家處於歷史決策情境中
**When** 遇到歷史人物（如蔣介石、毛澤東、軍閥等）
**Then** 系統提供基於真實歷史的對話選項
**And** 對話影響歷史事件和決策路徑
**And** 可以建立或破壞政治聯盟關係
**And** 記錄人物關係歷史和互動結果

## Epic 2: 大規模戰術戰鬥系統

用戶可以指揮1000+單位進行RTS戰鬥

### Story 2.1: 大規模戰鬥核心引擎

As a 民國時期的指揮官,
I want 在戰場上指揮大量部隊進行實時戰鬥,
So that 體驗大規模戰爭的震撼和策略深度。

**Acceptance Criteria:**

**Given** 戰鬥場景已初始化
**When** 遊戲開始加載戰鬥單位
**Then** 系統能同時渲染和處理1000+單位
**And** 保持60 FPS性能目標
**And** 支持大規模地形和複雜環境
**And** 實現高效的單位更新和渲染優化

### Story 2.2: 單位控制系統

As a 民國時期的指揮官,
I want 選擇和控制多個戰鬥單位,
So that 有效指揮部隊執行戰術任務。

**Acceptance Criteria:**

**Given** 戰鬥單位已生成在戰場上
**When** 玩家使用鼠標或觸控選擇單位
**Then** 支持框選、Ctrl點選等多種選擇方式
**And** 顯示選中單位的狀態和信息
**And** 可以下達移動、攻擊、防守等指令
**And** 支持編隊和群組控制

### Story 2.3: 多兵種協同作戰

As a 民國時期的指揮官,
I want 不同兵種之間進行協同作戰,
So that 發揮各兵種優勢取得戰鬥勝利。

**Acceptance Criteria:**

**Given** 戰場上存在多種兵種單位
**When** 玩家下達協同作戰指令
**Then** 步兵、騎兵、砲兵等兵種執行各自戰術角色
**And** 兵種之間形成有效的配合（如騎兵包抄、步兵正面、砲兵支援）
**And** 兵種克制關係影響戰鬥結果
**And** 提供兵種優勢和劣勢的視覺反饋

### Story 2.4: 戰鬥AI系統

As a 民國時期的指揮官,
I want 單位能自動執行戰鬥和尋找路徑,
So that 專注於整體戰術而無需微觀管理每個單位。

**Acceptance Criteria:**

**Given** 戰鬥單位在戰場上
**When** 單位需要移動或攻擊
**Then** AI自動尋找最佳路徑避開障礙
**And** 自動選擇攻擊目標並執行戰鬥
**And** 根據地形和敵情調整戰術行為
**And** AI性能優化支持1000+單位同時運算

### Story 2.5: 難度漸進系統

As a 民國時期的指揮官,
I want 選擇適合自己水平的難度,
So that 從初學者逐步成長為戰略大師。

**Acceptance Criteria:**

**Given** 玩家開始新遊戲或戰鬥
**When** 選擇難度級別（初級、中級、高級）
**Then** 系統調整AI的智能程度和反應速度
**And** 調整敵方單位的數量和強度
**And** 提供適當的教程和提示（初級）或挑戰（高級）
**And** 玩家可以在遊戲過程中調整難度

## Epic 3: 角色成長與敘事系統

用戶可以發展角色技能並參與歷史敘事

### Story 3.1: 角色創建和基礎屬性

As a 民國時期的軍官,
I want 創建並自定義我的角色,
So that 以獨特的身份開始歷史旅程。

**Acceptance Criteria:**

**Given** 玩家開始新遊戲
**When** 進入角色創建界面
**Then** 可以選擇角色名稱和背景
**And** 設定基礎屬性（統帥、智謀、勇武等）
**And** 選擇初始技能和專長
**And** 角色創建信息保存到遊戲進度

### Story 3.2: 技能發展系統

As a 民國時期的軍官,
I want 通過經驗和成就發展新技能,
So that 提升角色的能力和戰場表現。

**Acceptance Criteria:**

**Given** 角色參與戰鬥或完成任務
**When** 獲得經驗值達到升級條件
**Then** 系統提示技能點數可用
**And** 玩家可以選擇升級現有技能或學習新技能
**And** 技能提升影響角色在戰鬥和策略中的表現
**And** 技能發展路徑影響角色的專長方向（指揮官、戰術家、外交官等）

### Story 3.3: 敘事對話系統

As a 民國時期的軍官,
I want 與NPC進行豐富的對話互動,
So that 深入了解歷史背景和人物關係。

**Acceptance Criteria:**

**Given** 玩家與NPC（歷史人物或虛構角色）相遇
**When** 啟動對話系統
**Then** 提供多個對話選項反映不同態度
**And** 對話內容基於真實歷史背景和人物性格
**And** 對話結果影響人物關係和後續事件
**And** 記錄重要對話內容供玩家回顧

### Story 3.4: 歷史角色扮演

As a 民國時期的軍官,
I want 扮演特定的歷史人物角色,
So that 體驗真實歷史人物的視角和抉擇。

**Acceptance Criteria:**

**Given** 玩家選擇扮演特定歷史人物（如某個軍閥或將領）
**When** 進入角色扮演模式
**Then** 角色屬性和背景基於真實歷史人物
**And** 面臨該歷史人物真實經歷的決策情境
**And** 可以體驗該人物的歷史成就和失敗
**And** 角色扮演結果影響整體歷史進程

### Story 3.5: 角色關係網絡

As a 民國時期的軍官,
I want 建立和維護與其他角色的關係,
So that 形成政治聯盟或製造敵對勢力。

**Acceptance Criteria:**

**Given** 玩家在遊戲過程中與多個角色互動
**When** 完成互動（對話、交易、戰鬥等）
**Then** 系統更新角色關係值（友好、中立、敵對）
**And** 顯示關係網絡圖展示所有人物關係
**And** 關係影響政治聯盟的可能性和條件
**And** 敵對關係可能觸發衝突或戰爭事件

## Epic 4: 基地建設與資源管理

用戶可以建造基地並管理資源

### Story 4.1: 基地建造系統

As a 民國時期的軍官,
I want 在領地上建造各種功能建築,
So that 建立穩固的後勤和指揮基地。

**Acceptance Criteria:**

**Given** 玩家擁有可建造的土地
**When** 進入建造模式
**Then** 顯示可建造的建築類型（兵營、工廠、倉庫等）
**And** 可以選擇建築位置並預覽建造效果
**And** 消耗相應資源完成建造
**And** 建築提供特定功能（訓練部隊、生產物資等）

### Story 4.2: 資源採集和管理

As a 民國時期的軍官,
I want 採集和管理各種資源,
So that 支持軍隊和基地的運作。

**Acceptance Criteria:**

**Given** 玩家擁有資源產出點或採集設施
**When** 資源採集週期完成
**Then** 自動獲得相應資源（糧食、金錢、原材料等）
**And** 資源顯示在管理界面
**And** 可以分配資源到不同用途（軍隊、建設、研發等）
**And** 資源不足時顯示警告和影響

### Story 4.3: 建築升級系統

As a 民國時期的軍官,
I want 升級現有建築提高效率和功能,
So that 適應不斷發展的戰爭需求。

**Acceptance Criteria:**

**Given** 玩家擁有可升級的建築
**When** 選擇建築並查看升級選項
**Then** 顯示升級所需的資源和條件
**And** 升級後建築效率提升或解鎖新功能
**And** 建築可以專業化發展（如兵營→步兵營→精銳步兵營）
**And** 升級進度可視化顯示

### Story 4.4: 科技樹研發

As a 民國時期的軍官,
I want 研究新技術提升部隊和設施能力,
So that 在技術上領先敵人。

**Acceptance Criteria:**

**Given** 玩家擁有研發設施（如研究所、大學等）
**When** 進入科技研發界面
**Then** 顯示可研究的科技樹（軍事、經濟、外交等分支）
**And** 每個科技顯示所需時間和資源
**And** 研究完成的科技立即生效
**And** 某些科技需要先決條件（前置科技、特定建築等）

### Story 4.5: 生產製造系統

As a 民國時期的軍官,
I want 在基地生產武器、裝備和物資,
So that 裝備部隊並支持戰爭需求。

**Acceptance Criteria:**

**Given** 玩家擁有生產設施（兵工廠、製衣廠等）
**When** 下達生產指令
**Then** 設施開始生產選定的物品（武器、彈藥、制服等）
**And** 生產需要時間和原材料
**And** 生產完成後物品進入倉庫
**And** 可以優化生產線提高效率

## Epic 5: 四層策略整合系統

用戶可以在戰略、戰術、個人、建造四個層面進行決策

### Story 5.1: 戰略層界面和決策

As a 民國時期的指揮官,
I want 在戰略地圖上管理整體勢力和外交,
So that 制定長期的國家發展和戰爭策略。

**Acceptance Criteria:**

**Given** 玩家進入戰略層視圖
**When** 查看全國地圖和勢力分布
**Then** 顯示所有勢力的領土、軍隊和資源狀態
**And** 可以下達戰略指令（宣戰、締結同盟、貿易等）
**And** 戰略決策影響長期的遊戲進程
**And** 接收戰略層的事件和情報報告

### Story 5.2: 戰術層戰役指揮

As a 民國時期的指揮官,
I want 在戰役地圖上指揮具體戰鬥,
So that 贏得關鍵戰役推進戰略目標。

**Acceptance Criteria:**

**Given** 戰略層發起戰爭或遭遇敵軍
**When** 進入戰術層戰役地圖
**Then** 顯示戰場地形和雙方軍隊部署
**And** 可以實時指揮部隊進行戰術機動和戰鬥
**And** 戰役結果影響戰略層的領土和資源變化
**And** 支持1000+單位的大規模戰役

### Story 5.3: 個人層角色體驗

As a 民國時期的指揮官,
I want 親身參與關鍵戰鬥和事件,
So that 體驗戰爭的個人視角和英雄時刻。

**Acceptance Criteria:**

**Given** 玩家選擇進入個人層模式
**When** 控制角色在戰場上戰鬥或參與事件
**Then** 切換到第三人稱或第一人稱視角
**And** 直接控制角色進行戰鬥、對話和互動
**And** 個人層表現影響角色聲望和部隊士氣
**And** 可以隨時切換回戰術層進行整體指揮

### Story 5.4: 建造層基地管理

As a 民國時期的指揮官,
I want 詳細管理基地建設和資源生產,
So that 確保穩固的後勤支持前線作戰。

**Acceptance Criteria:**

**Given** 玩家進入建造層視圖
**When** 查看和管理基地設施
**Then** 顯示詳細的建築、資源和生產信息
**And** 可以規劃建築佈局、升級設施、調整生產
**And** 建造層決策影響戰略層的資源供應
**And** 可以查看多個基地並進行資源調配

### Story 5.5: 四層切換和整合

As a 民國時期的指揮官,
I want 在四個層次之間無縫切換,
So that 根據需要靈活調整管理粒度。

**Acceptance Criteria:**

**Given** 玩家在任何層次進行遊戲
**When** 選擇切換到其他層次
**Then** 層次切換流暢無縫，保持遊戲狀態
**And** 每個層次的信息和決策同步到其他層次
**And** 提供層次切換的快捷方式和提示
**And** 高層決策自動在下層執行，下層結果反饋到高層

## Epic 6: 跨平台無縫體驗

用戶可以在任何設備上繼續他們的民國史詩旅程，無縫切換平台而不丟失進度

**價值主張：** 玩家可以在PC上進行深度策略規劃，在移動設備上隨時管理資源和快速戰鬥，所有進度實時同步。無論是在家中的大屏幕體驗史詩戰役，還是在通勤途中處理政務，玩家都能隨時繼續他們的歷史征程。

### Story 6.1: 多平台帳號系統

As a 民國時期的指揮官,
I want 在任何設備上登入我的帳號繼續遊戲,
So that 不會因為更換設備而丟失歷史進程。

**Acceptance Criteria:**

**Given** 玩家已在某個設備上開始遊戲
**When** 在新設備上登入同一帳號
**Then** 自動同步所有遊戲進度（勢力狀態、角色發展、歷史決策等）
**And** 顯示同步完成的確認信息
**And** 可以立即繼續之前的遊戲狀態

### Story 6.2: 平台適配界面

As a 民國時期的指揮官,
I want 界面根據當前設備自動適配輸入方式,
So that 無論使用鼠標鍵盤還是觸控都能流暢操作。

**Acceptance Criteria:**

**Given** 玩家在不同設備上運行遊戲
**When** 進入遊戲界面
**Then** 自動檢測輸入設備並調整UI佈局
**And** PC上顯示鼠標懸停提示和快捷鍵
**And** 移動設備上顯示觸控優化的按鈕和手势支持
**And** 核心遊戲體驗在所有平台保持一致

### Story 6.3: 雲端存檔同步

As a 民國時期的指揮官,
I want 我的遊戲存檔自動備份到雲端,
So that 即使設備損壞也不會失去歷史征程。

**Acceptance Criteria:**

**Given** 玩家進行遊戲操作
**When** 達到存檔點或手動存檔
**Then** 自動上傳存檔到雲端服務器
**And** 顯示存檔同步狀態
**And** 可以查看和管理多個存檔版本
**And** 支持離線模式，網絡恢復後自動同步

### Story 6.4: 跨平台性能優化

As a 民國時期的指揮官,
I want 無論在哪個設備上都能享受流暢的史詩戰役,
So that 大規模戰鬥不會因為設備限制而體驗下降。

**Acceptance Criteria:**

**Given** 玩家在不同性能設備上進行大規模戰鬥
**When** 進入1000+單位的戰役
**Then** 系統自動調整渲染質量保持流暢度
**And** PC上提供最高畫質選項
**And** 移動設備上智能降低特效但保持核心體驗
**And** 戰鬥邏輯和結果在所有平台完全一致

### Story 6.5: 平台特定功能利用

As a 民國時期的指揮官,
I want 充分利用當前設備的獨特功能,
So that 獲得最佳的歷史沉浸體驗。

**Acceptance Criteria:**

**Given** 玩家在特定設備上遊戲
**When** 使用設備的獨特功能
**Then** PC上支持多顯示器策略地圖擴展
**And** 移動設備支持震動反饋增強戰鬥沉浸感
**And** 平板設備支持手寫筆進行地圖標註
**And** 所有平台特定功能都不影響核心遊戲平衡
