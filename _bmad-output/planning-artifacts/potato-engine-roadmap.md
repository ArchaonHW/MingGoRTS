# Potato Engine 開發需求規劃與任務清單

> 日期: 2026-09-17 · 基線: 全模組編譯通過、ctest 16+、MinGW+MSVC 雙 toolchain、Linux CI Build Check 綠
> 定位: RTS 遊戲引擎 (MingGoRTS) — 斷橋原型 T-1~T-8 已落地
> 更新: 2026-09-17 第二版 — T-4~T-8 完成、JsonParser/SceneRenderer 落地

## 現況盤點

| 子系統 | 狀態 |
|---|---|
| MathUtils | ✅ Vector/Matrix/Quaternion/Frustum,測試覆蓋 |
| Physics | ✅ spatial hash broadphase + SAT OBB/sphere + 材質衝量 + CollisionDetection.h,測試擴充至碰撞回調/kinematic/零質量邊界 |
| Scene | ✅ 場景圖 + 階層式 frustum culling,測試覆蓋 |
| Rendering | ✅ SceneRenderer + RenderableComponent 已串接場景圖(E1-1);仍缺 instancing/陰影 |
| Core/ECS/Events/FS/Log/Memory/Platform/Time | ✅ 全部建置+測試通過 |
| Serialization | ✅ JsonParser.h 完整實作(E3-1);replay/roster/map 都走它 |
| Resources | ⚠️ cache/loader 框架在,無 async/hot-reload |
| Audio | ⚠️ OpenAL 抽象層,多處簡化實作 |
| Input | ✅ GLFW 鍵鼠+手把輪詢 + headless keyStates 回退 + 完整鍵碼映射;無 action mapping |
| Security | ✅ WinTrust/hash pinning,紅隊全過 |
| Gameplay | ✅ **斷橋 T-1~T-8 全部落地**:FlowField/Squad/Doctrine(冷卻)/BattleController/BattlePlanner/BattleMap/EnemyGeneral/BattleResources/BattleRecorder/Roster/BattleSceneSync;三個無頭 demo 全過 |
| 基建 | ✅ CMake 34+ 目標、ctest 註冊表、靜態 runtime、雙 toolchain、CI workflow 修正(X11/submodule/lint 權限) |

## 需求 → Epic 拆解

### E1 · 渲染管線產品化 (P0)
RTS 要畫出數百單位,目前 renderer 與場景圖脫鉤是最大缺口。

| # | Story | 說明 | 依賴 |
|---|---|---|---|
| ~~E1-1~~ | ~~RenderableComponent + SceneRenderer~~ | ✅ 已完成 — `Rendering/SceneRenderer.*` + `RenderableComponent.h` | — |
| E1-2 | Instanced rendering | 同 mesh 多實例一次 draw(glDrawElementsInstanced),RTS 人海必備 | E1-1 |
| E1-3 | 陰影貼圖 | Directional shadow map,RTS 俯視視角一張 cascade 就夠 | E1-1 |
| E1-4 | Debug draw | 畫包圍球/AABB/frustum,除錯剔除與物理 | E1-1 |

### E2 · 物理補強 (P0→P1)
| # | Story | 說明 | 依賴 |
|---|---|---|---|
| E2-1 | Raycast 精確化 | OBB/sphere ray 測試 — **RTS 點選單位的核心**,目前 Raycast 只有簡化版 | — |
| E2-2 | Contact manifold | 多接觸點 box-box,解決箱子堆疊抖動 | E1 無關 |
| E2-3 | Trigger volume | 只報事件不解析 — 區域偵測/技能範圍 | — |
| E2-4 | CCD (speculative/TOI) | 高速物體防穿隧 | E2-1 |

### E3 · Serialization 實作 (P0)
| # | Story | 說明 | 依賴 |
|---|---|---|---|
| ~~E3-1~~ | ~~JSON parser~~ | ✅ 已完成 — `Serialization/JsonParser.h` 全功能解析,map/replay/roster 已用 | — |
| E3-2 | SceneNodeData/GameObjectData 往返測試 | serialize→deserialize round-trip | E3-1 |
| E3-3 | 版本號+欄位容錯 | 存檔相容性 | E3-1 |

### E4 · Gameplay 串接 (P1)
| # | Story | 說明 | 依賴 |
|---|---|---|---|
| E4-1 | Squad↔SceneNode 綁定 | 🔄 進行中 — `BattleSceneSync` 已起步 | E1-1, E2-1 |
| ~~E4-2~~ | ~~FlowField 尋路實測~~ | ✅ 已完成 — 斷橋地圖河面阻擋+單渡口通行驗證通過 | — |
| E4-3 | 可玩 demo 場景 | 點選→移動→交戰最小循環 | E4-1, E4-2 |

### E5 · 資源/基建 (P1→P2)
| # | Story | 說明 | 依賴 |
|---|---|---|---|
| E5-1 | Async loading | ResourceManager 目前同步阻塞 | — |
| E5-2 | Input action mapping | 鍵位重綁定、抽象 action 層 | — |
| E5-3 | Audio 實作補完 | OpenAL buffer/source 實際接線 | — |
| E5-4 | Linux CI 驗證 | AGENTS.md 註明 CI 跑 Linux g++,確認本輪改動在 Linux 過 | — |

## 建議 Sprint 順序（已依完成度重排）

```
Sprint A (已完成):   E3-1 JSON parser → E1-1 SceneRenderer → E4-2 FlowField 實測
Sprint B (下一步):   E2-1 raycast 點選 → E4-1 Squad↔Scene 綁定 → E4-3 可玩 demo
Sprint C (視覺):     E1-2 instancing → E1-4 debug draw → E2-3 trigger volume
Sprint D (補強):     E1-3 陰影 → E2-2 contact manifold → E5-1 async loading
Sprint E (收尾):     E3-2 round-trip → E3-3 版本容錯 → E5-2 input mapping → E5-3 audio
```

斷橋 prototype 專用軌（與 epic 並行）：T-9~T-12 遊戲 UI 層——
卡槽欄/卡編輯器（回合層）、狀態條/CP 鈕/訊息流（即時層）、史官筆戰報+名冊+回放時間軸（戰後）、膠囊體單位。

## 風險與注意事項

- ~~Deserialize stub~~ 已解決（JsonParser 實作完成）
- ~~renderer/scene 脫鉤~~ 已解決（SceneRenderer 消費 CollectVisibleNodes）
- **新風險 — 潰逃不等於陣亡**：目前士氣歸零先於兵力歸零，名冊的「陣亡」只在全滅時觸發；
  T-E 教學關的「老周必死」需要「追殺潰兵」或獨立殲滅判定機制
- RTS 的 **raycast 點選**(E2-1)是讓斷橋從 console 走向可玩圖形版的最後一塊拼圖
- 變更需雙 toolchain 驗證(MinGW + MSVC/CI Linux g++),並守 AGENTS.md: 不碰 external/、conventional commits、PR 流程
