# Potato Engine 開發需求規劃與任務清單

> 日期: 2026-09-17 · 基線: 全模組編譯通過、ctest 12/12、MinGW+MSVC 雙 toolchain
> 定位: RTS 遊戲引擎 (MingGoRTS)

## 現況盤點

| 子系統 | 狀態 |
|---|---|
| MathUtils | ✅ Vector/Matrix/Quaternion/Frustum,測試覆蓋 |
| Physics | ✅ spatial hash broadphase + SAT OBB/sphere + 材質衝量,9/9 測試 |
| Scene | ✅ 場景圖 + 階層式 frustum culling,測試覆蓋 |
| Rendering | ⚠️ OpenGL3.3 renderer/Shader/Camera/Lighting/ModelLoader 可用,但**未與場景圖串接**,無 instancing/陰影 |
| Core/ECS/Events/FS/Log/Memory/Platform/Time | ✅ 全部建置+測試通過 |
| Serialization | ⚠️ Serialize 有 JSON escape;**Deserialize 是 stub(只印字串回 true)** |
| Resources | ⚠️ cache/loader 框架在,無 async/hot-reload |
| Audio | ⚠️ OpenAL 抽象層,多處簡化實作 |
| Input | ✅ GLFW 鍵鼠+手把輪詢;無 action mapping/重綁定 |
| Security | ✅ WinTrust/hash pinning,紅隊 39/0 |
| Gameplay | ⚠️ BattleController/Doctrine/FlowField/Squad 存在但未與渲染/物理串接 |
| 基建 | ✅ CMake 34 目標、ctest、靜態 runtime、雙 toolchain |

## 需求 → Epic 拆解

### E1 · 渲染管線產品化 (P0)
RTS 要畫出數百單位,目前 renderer 與場景圖脫鉤是最大缺口。

| # | Story | 說明 | 依賴 |
|---|---|---|---|
| E1-1 | RenderableComponent + SceneRenderer | SceneNode 掛 mesh/material,場景遍歷→frustum cull→提交 draw | — |
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
| E3-1 | JSON parser | Deserialize 目前回傳 true 不解析 — 存檔系統實質是空的 | — |
| E3-2 | SceneNodeData/GameObjectData 往返測試 | serialize→deserialize round-trip | E3-1 |
| E3-3 | 版本號+欄位容錯 | 存檔相容性 | E3-1 |

### E4 · Gameplay 串接 (P1)
| # | Story | 說明 | 依賴 |
|---|---|---|---|
| E4-1 | Squad↔SceneNode 綁定 | 單位在場景中有實體+選取圈 | E1-1, E2-1 |
| E4-2 | FlowField 尋路實測 | RTS 群體移動的核心 — 驗證 FlowField 在真實 grid 上運作 | — |
| E4-3 | 可玩 demo 場景 | 點選→移動→交戰最小循環 | E4-1, E4-2 |

### E5 · 資源/基建 (P1→P2)
| # | Story | 說明 | 依賴 |
|---|---|---|---|
| E5-1 | Async loading | ResourceManager 目前同步阻塞 | — |
| E5-2 | Input action mapping | 鍵位重綁定、抽象 action 層 | — |
| E5-3 | Audio 實作補完 | OpenAL buffer/source 實際接線 | — |
| E5-4 | Linux CI 驗證 | AGENTS.md 註明 CI 跑 Linux g++,確認本輪改動在 Linux 過 | — |

## 建議 Sprint 順序

```
Sprint A (P0 基礎):  E3-1+E3-2 → E2-1 → E1-1
   理由: 存檔/點選/畫面三個「能用」的最低門檻,彼此獨立可並行
Sprint B (P0 視覺):  E1-2 → E1-4 → E2-3
Sprint C (P1 遊戲):  E4-2 → E4-1 → E4-3 (可玩 demo)
Sprint D (P1 補強):  E1-3 → E2-2 → E5-1
Sprint E (P2 收尾):  E3-3 → E5-2 → E5-3 → E5-4
```

## 風險與注意事項

- **Deserialize stub 是沉默的錯誤**:`return true` 不解析,遊戲載入會「成功但資料是空的」——E3-1 優先
- **renderer/scene 脫鉤**:CollectVisibleNodes 已備好,但沒有任何東西消費它——E1-1 是讓引擎「真的能畫東西」的關鍵
- RTS 的 **raycast 點選**(E2-1)與 **FlowField 群體尋路**(E4-2)是 MingGoRTS 類型定義功能,優先度高於一般 polishing
- 變更需雙 toolchain 驗證(MinGW + MSVC/CI Linux g++),並守 AGENTS.md: 不碰 external/、conventional commits、PR 流程
