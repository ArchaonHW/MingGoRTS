---
title: 'technical research: latest-engine-techniques'
type: 'technical'
topic: 'latest-engine-techniques'
decision: 'which 2025-2026 techniques to adopt into Potato Engine (MingGoRTS)'
source: 'run'
status: final
preset: 'standard'
validation: 'normal'
created: '2026-09-17'
updated: '2026-09-17'
---

# technical research: latest-engine-techniques

**Decision this research serves:** which 2025–2026 techniques are worth adopting into Potato Engine — a C++20 game engine (OpenGL renderer, custom physics, ECS, offline AI codegen in IDE, user-mode anti-tamper security subsystem).

---

## Executive Summary

四個維度各一輪掃描完成（standard preset、4 個平行 researcher、每維度 ~8 來源）。核心結論：**每個維度都有明確的「高回報/低複雜度」切入點，而且多數直接對齊專案現有結構**。

最值得先做的五件事（按回報/成本排序）：

1. **Frame/scratch arena + `std::pmr`**（架構）— C++17 標準庫就有，~100 行，消除熱迴圈 malloc；Frostbite 到小型引擎的收斂做法 [46][47][48]
2. **GPU-driven culling + MultiDrawIndirect**（渲染）— 現有 CPU frustum culling 的直接演化；GL 4.3 compute+SSBO+MDI 全核心功能，有可複製的 GL 參考實作 [1][2][3]
3. **`GetRuntimeAttestationReport` 輪詢**（安全）— 文件化 Win32 API，Secure Kernel 簽章的驅動清單+code-integrity 報告，MS 文件明確點名 anti-cheat 用途；不需自帶 driver 就能偵測 BYOVD/HVCI-off 狀態 [49]
4. **GBNF/JSON-schema 約束 LLM 輸出**（AI/IDE）— 保留現有模板系統，把 LLM fallback 輸出約束成結構化 JSON（patch spec），保證可解析 [21][22]
5. **Clustered forward lighting**（渲染）— forward renderer 的正典升級路徑；Godot 4 主渲染器即 clustered forward，兩個 compute pass 就夠 [8][9]

**重大負面發現**：kernel-assisted VAD 隱藏與 indirect-syscall+stack-spoofing 讓現有 VAD 掃描/RIP 稽核存在已知盲區 [53][56]；XeSS/FSR4 對 OpenGL 完全無路可走 [13]；ETW-TI 第三方拿不到（需 ELAM+PPL-AM）[51]。

---

## 維度一：渲染（OpenGL 3.3/4.x → 現代化）

### GPU-driven rendering — 可行且為首選

完整的 GPU-driven pipeline（compute frustum culling、Hi-Z occlusion、LOD 選擇、compaction 進 `glMultiDrawElementsIndirect`）全部落在 GL 4.3–4.6 **核心功能**內（compute shader、SSBO、indirect draw）[1]。NVIDIA GTC 2015 就示範過 GL 上的 GPU culling+Hi-Z；`NV_command_list` 雖是 NVIDIA 限定，但 compute+MDI 路線完全可移植 [1]。

可複製的參考實作：**azdo** 用 OpenGL 實作了 instancing + compute Hi-Z occlusion（screen-space AABB vs hierarchical-z mipmap）+ atomic-counter LOD + 單次 MDI 提交，採用與 Niagara 相同的 two-phase temporal-coherence 架構 [2]。一個 GL 4.6 voxel renderer 回報單一 MDI draw call/帧、culling dispatch ~181µs [3]。

**注意（已標記矛盾）**：Imagination 白皮書指出 indirect+compute 在 GL 可運作但 Vulkan/DX12 上效能更可預測——GL 的 `glMemoryBarrier` 正確性要自己負責，driver 排程不透明 [4]。不構成阻擋，但效能驗證要以實測為準。

### Bindless textures — 真實可用但受 extension 限制

`GL_ARB_bindless_texture` 從未進 core，但在 NVIDIA/AMD 獨顯廣泛支援 [5]。關鍵約束：無 `NV_gpu_shader5` 時 sampler handle 必須是 dynamically uniform——`gl_DrawID` 經 flat varying 傳遞可保持此性質，AMD driver bug 已於 21.12.1 修復 [5]。Bindless+MDI 是設計上的搭配：每幀標記 resident、64-bit handle 存 SSBO、以 `gl_DrawID` 索引 [6][7]。**Intel GPU 支援度本輪未驗證**（見開放問題）。

### Clustered / Forward+ — forward renderer 的正典升級

Clustered shading 把 view frustum 切成 3D grid，compute pass 指派光源，fragment 查表；對 forward pipeline「侵入極小」且可擴展到上千光源，有多個完整 GL 4.3/4.5 實作（grid-build + light-cull 兩個 `glDispatchCompute`）[8]。**最有力的 landscape 證據：Godot 4 主渲染器明確是「clustered forward」**（選它而非 deferred 是為了彈性+原生 MSAA）[9]。

### Frame graph — 只採一半

Frame graph（Frostbite 推廣、UE RDG）的 DAG pass 宣告好處在 transient resource 重用、dead-pass 消除、pipeline 模組化 [10]。但最大賣點「自動 barrier 求解」在 GL 上沒有意義——沒有 explicit layout barrier 可自動化。**建議只採 pass-declaration + transient-target pooling 那一半** [10]。

### Upscaler — FSR 3.1 是 OpenGL 的天花板

- FSR 3.1 官方文件列出 GLSL 4.50 shader target（需 `GL_EXT_samplerless_texture_functions`、`GL_EXT_shader_image_load_formatted`），FSR3Unity 證明可在 OpenGL Core 跑——但官方 API backend 只有 DX12+Vulkan，需自寫 backend 或改寫 shader 來源 [12]
- FSR 2 有完整社群 GL port（FidelityFX-FSR2-OpenGL），port 工作清單明確 [11]
- **FSR 4 = RDNA4 + DX12 + signed binary，無 GL/VK；XeSS 只支援 DX11/12/Vulkan——對純 GL 引擎完全封死** [13]
- 所有 temporal upscaler 都需要 depth + motion vectors + camera jitter——這些前置要先做

### Meshlet / visibility buffer / VT — 全部可行但建議延後

- GL mesh shader 只有 `GL_NV_mesh_shader`（NV 限定）；可移植路線 = meshoptimizer 離線建 meshlet + compute cull（frustum + backface cone）+ 每 meshlet 一個 indirect draw [14]
- Visibility buffer 正在擴散（The Forge "Forward++"、I3D 2026）[15]，但 **Cinevva 2026 post-mortem 實測 vis-buffer 在其場景不比 forward 快**——「教科書答案是錯的」；只在重 overdraw/貴 material 時划算，先 benchmark [16]
- `ARB_sparse_texture` 提供 page-commitment + residency feedback（VT 的原語），但 page-table/streaming policy 全要自己寫，高成本 [17]

### 渲染採用排序

| 順位 | 技術 | 理由 |
|---|---|---|
| 1 | GPU-driven culling + MDI | 現有 frustum culling 的直接演化；順便建立 compute shader 能力（目前引擎沒有，這是 on-ramp）|
| 2 | Clustered forward lighting | 正典升級、兩個 compute pass |
| 3 | Bindless textures | 小改動、解鎖 MDI 內 per-draw material |
| 4 | FSR 3.1 custom backend / FSR2 GL port | 唯一可行 upscaler；先做 motion vectors+jitter |
| 5 | Lightweight render graph | 只採 pass 宣告+transient pooling |
| 延後 | Meshlet、vis-buffer、sparse VT | 場景依賴，先 benchmark |

---

## 維度二：AI/IDE code generation

### 本地模型版圖（2025–2026）

- **Qwen3-Coder 三層**：480B-A35B（256K ctx，開源 agentic coding SOTA）；30B-A3B-Instruct（Apache-2.0，262K ctx）；Qwen3-Coder-Next 80B/3B（官方定位「coding agents 與本地開發」，有官方 GGUF）[18]
- **DeepSeek-Coder-V2-Lite 16B**（2.4B active）：RTX 4090 上 >100 tok/s、~10GB Q4、商用授權乾淨 [19]
- **Codestral 22B**：MNPL 非生產授權——**出貨產品不可用** [20]
- **StarCoder2-3B**（OpenRAIL-M）：~180 tok/s，標準 autocomplete 級選擇 [20]

### Structured / constrained decoding

- llama.cpp 內建 **GBNF grammar-constrained sampling**（`--grammar`/`--grammar-file`、server `grammar` 欄位、JSON-Schema→GBNF 自動轉換、llguidance backend）[21]
- **XGrammar** 是 vLLM/SGLang/TRT-LLM/MLC 的預設 backend，保證 100% 結構正確、JSON 近零開銷 [22]
- Grammar 約束已能對真實程式語言保證語法有效性（constrained-diffusion 展示 LLaDA 產出 C++ 時拒絕非法 token；PSC 論文回報複雜語言 grammar 的 mask 計算快 700×）[23]
- **重要警告**：過嚴的 grammar 會扭曲輸出分佈、降低品質（CRANE ICML'25、GAD NeurIPS'24）——**不要把完整 C++ grammar 套上去**；應約束成「結構化 JSON/AST spec」，程式碼本體仍由模板系統生成 [24]
- llama.cpp server 有 `/infill` FIM endpoint；**FIM 要用 base Coder 模型，instruct 版會幻覺** [25]

### Codebase RAG — 收斂答案是 hybrid

- Anthropic 官方：Claude Code **刻意不用 embedding index**——agentic grep + LSP symbol 導航；embedding RAG 在活躍 codebase 上的核心失敗模式是 staleness [27]
- 反證：Cursor 量到加 semantic search 後 agent 準確率 +12.5% [28]
- 收斂架構：tree-sitter symbol chunking + dense+BM25 混合 + RRF + rerank [26]——**grep 查 identifier、embedding 查意圖、LSP/KG 查結構**
- nomic-embed-code（7B，Apache-2.0）GGUF 可直接跑 llama.cpp；**但訓練語言清單不含 C++——採用前必須先驗證 C++ 檢索品質** [29]

### Agentic loop 模式

- MCP 是收斂的整合介面（VS Code/Cursor/Claude Code 都載 stdio MCP server，spec 2025-11-25）[30]
- 工具面要窄且型別化：`search_symbol`/`find_tests`/`get_build_errors`，回傳 path+excerpt+confidence，避免「read everything」工具 [30]
- 把 IDE 能力經 MCP 暴露給 agent（JetBrains MCP 模式）可提供 symbol 級精準導航，取代純文字 grep——與 Anthropic 的 LSP 建議同構 [32]
- Loop runtime 收斂於：**外部 verifier 決定 DONE（agent 不自我宣告）**、每輪重注入 standing orders、stuck 時 escalation、branch 隔離+人核 merge [31]

### AI/IDE 採用排序

| 順位 | 技術 | 對現有系統的關係 |
|---|---|---|
| 1 | llama.cpp + GGUF tiered（base 模型做 /infill、30B-A3B 做 chat） | 單一 runtime 補齊現有 LLM fallback 的本地部署路徑 |
| 2 | GBNF/JSON-schema 約束輸出成 patch spec | 保留模板系統，LLM 只產結構化輸入——避開 grammar 品質陷阱 [24] |
| 3 | Hybrid retrieval：現有 KnowledgeGraph 當 symbol 層 + grep + 可選 embedding | KG 已經等價於 LSP surface，補 grep/embedding 即可 |
| 4 | MCP-shaped 窄型別工具介面 | 內部先用這個形狀，之後接 MCP wire format 免費 |
| 5 | External-verifier loop（build/test 結果當 DONE 檢查） | 現有 build infra 直接可用；這是 2026 年「agent vs chat client」的分界 |

---

## 維度三：引擎架構

### ECS — 業界收斂於 hybrid storage，無贏家通吃

- 2025 年 CGVC 同儕審查研究各建一個 sparse-set 與 archetype 量化比較：sparse-set 的 add/remove 便宜但迭代擴展差；archetype 靠 cache 效率贏大規模迭代但 composition 變更貴 [33]
- **Bevy 讓每個 component type 自己選 Table（columnar SoA，預設）或 SparseSet**；flecs 也加了 sparse components（明言受 EnTT 啟發）[34][35]
- flecs 作者自己的 FAQ：EnTT 贏 add/remove 與單 component 查詢，flecs 贏多 component 查詢/批量建刪——** workload 決定，沒有絕對更快**（med confidence，作者自述有立場）[35]
- flecs 的差異化：relationships（`ChildOf` pair）與 observers（OnAdd/OnRemove/OnSet 反應式回呼）[35]
- EnTT 維持極簡，observers 是可選 mixin；已出貨 Minecraft Legends / D2R / CoD Vanguard [36]

### Job system — work-stealing 即可，fiber 不必要

- Naughty Dog fiber 系統：job 在 fiber context 跑、atomic counter 上中途 yield、無 work stealing——**選 fiber 是為了 console 上的 API 易用性而非速度** [37]
- enkiTS 是參考級「小而產線驗證」scheduler（Avoyd 出貨、Mastering Graphics Programming 採用）[38]
- 2025 年論文：<1000 行 C++ 的 work-stealing pool 效能與 Taskflow 相當——**自製極小 scheduler 是可行路線**（med confidence）[39]
- `std::execution`（P2300）已進 C++26 draft，可經 NVIDIA stdexec 用，但對小引擎是 heavyweight [40]

### Data-oriented physics — 直接可移植的技術

- **Box2D v3 的關鍵技術是 greedy graph coloring + wide SIMD**：contact constraint 著色使同色內一個 body 只出現一次→每色 4/8-wide SoA 求解無 race；greedy+bitset 就夠，增刪時增量更新 [41]（已驗證：官方 post 附 benchmark，SSE2 ~2× scalar、AVX2 再快 14%）
- Box2D v3 整體 >2× v2.4 單執行緒，主因 data-oriented layout+SIMD [42]
- Wide-vs-narrow 指引：**把 work unit（contact point、edge pair）放 SIMD lane，不要把 xyz 放一個 register**；SAT edge-edge 只在高 edge count 時值得 SIMD [43]
- Jolt 的 `CROSS_PLATFORM_DETERMINISTIC`：純 float 達成跨架構/編譯器 bit-identical，代價 ~8%——但 **RTS lockstep 實務仍偏好 fixed-point**（Bannermen/Photon、ferrets）[44][45]

### Memory — 收斂的三層模型

- Frostbite ScopeStack：linear allocator 但依依賴序跑 dtor；動機是主機固定記憶體下 fragmentation 會擋出貨 [46]
- `std::pmr::monotonic_buffer_resource`（C++17）：bump alloc、free 是 no-op、`release()` 一次全放；可用 stack buffer 播種達成零 heap；**非執行緒安全——正好符合單執行緒引擎** [47]
- 三層收斂：frame arena（每幀 reset）/ persistent arena（level 生命週期）/ pooled generational handles [48]

### 架構採用排序

| 順位 | 技術 | 理由 |
|---|---|---|
| 1 | Frame/scratch arena + std::pmr | ~100 行、殺熱迴圈 malloc、零新依賴 |
| 2 | ECS per-component storage 選擇（dense SoA vs sparse） | flecs/Bevy/EnTT 的收斂答案；若現有 ECS 是純 sparse-set 是最便宜的大迭代收益 |
| 3 | 小型 work-stealing scheduler（enkiTS 或自製 <1k LOC） | fiber 的中途 yield 用不到；work-stealing deque+parallel_for 拿大部分多核收益 |
| 4 | Contact solver 寬 SIMD + graph coloring | 直接從 Box2D v3 移植；只在 pair count 高時值得 |
| 5 | Fixed-tick + deterministic ordering + checksum | 僅在 RTS lockstep 為需求時；Jolt 證明 float 可行但 fixed-point 是 RTS 驗證路線 |

---

## 維度四：Anti-cheat / 安全

### 攻擊面現況 — 現有防禦存在已知盲區

- **Indirect syscall + return-address/call-stack spoofing 可繞過 RIP 稽核**（DoomSyscalls：解析 SSN+syscall gadget、推 spoofed return address）[53]
- `.pdata`-aware stack spoofing 讓 unwind-based 檢查也失效（SindriKit 1.3.0）[54]
- **Kernel-assisted VAD 隱藏是真實的**：driver unlink VAD+翻 PTE 後 `NtQueryVirtualMemory` 掃描全盲——現有 `CheckHiddenModules` 對此類無效 [56]
- 記憶體類型偽裝：private **RX**（非寫入）shellcode 繞過 RWX 掃描；pagefile-backed `MEM_MAPPED` 繞過「mapped 須有檔案路徑」檢查；`LdrLoadDll` hollowing 會填 PEB 使「無 LDR entry」啟發失效 [57]
- ETW blinding：patch `EtwEventWrite*`/`NtTraceEvent` 可靜音 user-mode ETW（kernel ETW-TI 不受影響）；偵測 artefact 是對 ntdll 頁面的 `NtProtectVirtualMemory` [55]
- HVCI 可被繞過（BusterCall PFN-swap）；KVC 在 boot 階段靜默關閉 HVCI/DSE 再還原旗標——**「HVCI 已啟用」的 registry/UI 狀態不可信，attestation 必須來自 Secure Kernel** [58]

### 新防禦能力 — 不需自帶 driver 的路徑

- **`GetRuntimeAttestationReport`（sysinfoapi.h，已驗證：MS Learn 文件 ms.date 2026-01）**：VTL0 user-mode 取回 Secure Kernel 簽章報告（載入的 driver 清單+code integrity）；MS 文件明確點名「enforcing anti-cheat policies in gaming」。需 TPM2/SecureBoot/VBS/HVCI/IOMMU、test-signing/debug 關閉；**報告只在 HVCI 啟用時能生成** [49]
- **CET user-mode shadow stack 已出貨**：`PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY`（compat/strict 模式、`SetProcessDynamicEnforcedCetCompatibleRanges` 給 JIT）。注入的 cheat code 不是 CETCOMPAT→strict 模式下 RET-spoofing/ROP 直接 `#CP` fault [50]
- **`NtSetInformationProcess(ProcessInstrumentationCallback=0x28)`**：純 user-mode 攔截每個 syscall return——檢查 syscall 來源是否在 ntdll/win32u 內（抓 direct/indirect syscall）。SentinelOne 在用；**但攻擊者可用 NULL 註銷它→必須把自己的 callback 存活當完整性項目來心跳** [52]
- **ETW-TI 第三方拿不到**（需 ELAM cert+PPL-AM，MVI partnership+NDA）——不要圍繞它設計 [51]

### 商用 AC 對標

- Vanguard：pre-boot 安全姿態強制（SecureBoot/TPM/IOMMU/HVCI）→ driver 可 on-demand；**VAN:Restriction 抓到過「主機板回報 IOMMU 已啟但實際 init 失敗」——教訓：驗證 IOMMU 真的列舉到，不要只信 policy flag** [60]
- BattlEye 據信掃 PCI config space 簽章；EAC 可 kernel 或 usermode 部署；Ricochet 雲端 attestation [59]
- DMA cheat（2026 現況）：**部分可偵測**——PCIe config-space 比對 known-bad baseline、Xilinx IP 指紋、pre-boot IOMMU gap（med-high，cheat-adjacent 來源）[59]
- Heartbeat 反竄改模式（Raven AC）：signed payload+server nonce+rolling counter+adaptive cadence+server 端 inter-arrival 分析 [61]
- VM/speedhack：`RDTSC-CPUID-RDTSC` 時序基線（BattlEye/EAC 級產品在用，hypervisor 可模擬→要配 cache side-channel）；EPT-hook 可用 read-latency 異常偵測；多時鐘交叉驗證（rdtsc/QPC/GetTickCount64/dspTime）[62][63]

### 安全採用排序

| 順位 | 技術 | 理由 |
|---|---|---|
| 1 | `GetRuntimeAttestationReport` 輪詢 | 零 driver 取得 Secure Kernel 真相；抓 BYOVD/KVC/HVCI-off |
| 2 | CET shadow stack（`/CETCOMPAT` + strict policy） | 硬體級擋 RET-spoof/非 CETCOMPAT 注入碼 |
| 3 | ProcessInstrumentationCallback + 自我心跳 | user-mode syscall-origin 稽核；必須防被 NULL 註銷 |
| 4 | 擴充掃描面：RX-private、pathless MEM_MAPPED、EtwEventWrite* 進 hook 掃描 | 補現有 RWX-only/PEB-only 盲區 |
| 5 | PCI config 枚舉 + IOMMU-actually-present + 多時鐘心跳 | DMA/VM/speedhack 訊號；holder 白名單維持必要 |

---

## 矛盾與可信度標記

| 矛盾 | 兩方證據 | 處理 |
|---|---|---|
| GL compute+indirect 效能可預測性 | VoxelMVP 實測快 [3] vs imgtec 指出 VK/DX12 才 optimal [4] | 採用但要求實測 benchmark，不預設達到 VK 水準 |
| Visibility buffer 是否值得 | The Forge/I3D 2026 擴散中 [15] vs Cinevva 實測不比 forward 快 [16] | 標記場景依賴，列為延後+benchmark-first |
| Embedding RAG vs agentic search | Anthropic 不用 embedding [27] vs Cursor +12.5% [28] | 收斂解是 hybrid——兩者並非互斥 |
| Sparse-set vs archetype 快慢 | flecs FAQ 自述各有勝場 [35] vs CGVC25 量化比較 [33] | 一致：workload 決定；對本引擎 workload 需自測 |

**低可信度標記**（med 或 cheat-adjacent 來源）：flecs FAQ 的勝場歸因（作者立場）、CSIT <1k-LOC scheduler 效能宣稱、Cursor +12.5%（secondary 轉述）、DMA 偵測細節（lystic.dev/hwidchange）、Raven heartbeat 模式（商用 vendor 自述）、Devstral-2 SWE-bench 數字。

**本輪親自驗證**（lead spot-check）：`GetRuntimeAttestationReport` MS Learn 頁面存在且明載 anti-cheat 用途與 HVCI 需求 [49]；Box2D graph-coloring/wide-SIMD 技術細節與 benchmark 數字 [41]；llama.cpp GBNF grammar 文件與 JSON-schema 轉換 [21]。Khronos bindless wiki 頁面 403 無法取得——[5] 標記為 unverified（同 digest 有 SO 旁證，且非首要建議的承重 claim）。

---

## 建議整合路線圖（Roadmap）

依「先做低成本高回報、且能建立後續技術地基」排序：

**Phase 0（立即，零依賴）**
- Frame/scratch arena + `std::pmr`（架構）
- `GetRuntimeAttestationReport` 加入 SecuritySystem 監控週期（安全；Win11 only，需 runtime feature-detect）
- IDE：把 LLM fallback 輸出改為 GBNF/JSON-schema 約束的 patch spec（AI）

**Phase 1（建立 compute shader 地基）**
- GL 4.3 compute shader + SSBO 基礎設施（渲染 on-ramp）
- GPU-driven culling 原型（azdo 式 two-phase）對照現有 CPU culling benchmark
- ECS storage 分層：熱迭代 component 走 dense SoA

**Phase 2（擴張）**
- Clustered forward lighting
- 小型 work-stealing scheduler
- ProcessInstrumentationCallback + CET strict mode（安全 Phase 2）
- Hybrid retrieval：KG + grep +（驗證 C++ 品質後）nomic embedding

**Phase 3（場景驗證後）**
- FSR 3.1 custom backend（前置：motion vectors+jitter）
- Meshlet via compute+MDI、bindless textures
- PCI config/IOMMU/多時鐘心跳（安全 Phase 3）

**明確不採用**：fiber scheduler（無需求）、ETW-TI 依賴設計（拿不到）、XeSS/FSR4（無 GL 路徑）、kernel driver 自研（超出合法部署邊界）、完整 C++ grammar 約束（品質反效果）。

---

## Open Questions

1. **Intel GPU 的 `ARB_bindless_texture` 支援度**——若目標硬體含 Intel iGPU 需先確認（rendering shortlist #3 的承重前提）
2. **`glMultiDrawElementsIndirectCountARB`/`GL_ARB_indirect_parameters` 各 vendor 覆蓋率**——影響 GPU-driven 實作選型
3. **nomic-embed-code 的 C++ 檢索品質**——訓練語言清單不含 C++，需用本 repo 建 benchmark 驗證
4. **Qwen3-Coder-Next 的授權**——HF card license 欄位本輪未取到
5. **`GetRuntimeAttestationReport` 的最低 OS build**——文件頁很新（2026-01），實機可用版本需驗證
6. **本引擎 workload 的 sparse-vs-archetype 實測**——兩份來源都警告要量自己的 workload

## Follow-up Leads

- `niagara`（zeux）— GPU culling/meshlet/LOD 參考架構，culling kernel 可直譯 GL compute+MDI
- `azdo`（potato3d）— 最接近可直接複製的 GL GPU-culling 實作
- `FidelityFX-FSR2-OpenGL`（JuanDiegoMontoya）— 完成的 GL backend 可研究/fork
- flecs「Building an ECS」系列 part 6 — sparse components 細節
- Box3D 公告（2026-06）— wide-SIMD solver 泛化到 3D
- DART PR #2504 — allocator benchmark suite
- Elastic Defend ETW-callstack 文、0xflux EDR 系列 — 可移植的 usermode 偵測啟發
- llama.cpp `llama.vim` — FIM-in-editor 最小整合參考
- CRANE（ICML'25）— 若 grammar 約束損害品質時的 reasoning-preserving 技術

---

## Source Table

| # | Source | Publisher | Pub date | Accessed | Class |
|---|---|---|---|---|---|
| 1 | GPU-Driven Large Scene Rendering in OpenGL (S5135) | NVIDIA/GTC (huihoo mirror) | 2015 | 2026-09-17 | pattern |
| 2 | azdo | GitHub (potato3d) | ~2024 | 2026-09-17 | pattern |
| 3 | VoxelMVP | GitHub (SSeanPP) | 2024–25 | 2026-09-17 | performance |
| 4 | GPU-Controlled Rendering whitepaper | Imagination | n/a | 2026-09-17 | pattern |
| 5 | Bindless Texture wiki + SO 70377014 | Khronos / SO | ongoing/2021 | 2026-09-17 | pattern |
| 6 | OpenGL bindless rendering | ktstephano blog | ~2023 | 2026-09-17 | pattern |
| 7 | GL_NV_bindless_multi_draw_indirect | NVIDIA | 2013 | 2026-09-17 | version |
| 8 | clustered-shading + Olsson et al. preprint | GitHub DaveH355 / Chalmers HPG | ~2023 / 2012 | 2026-09-17 | pattern |
| 9 | Internal rendering architecture | Godot docs | current | 2026-09-17 | landscape |
| 10 | Frame graph docs + render-graphs-101 | Babylon.js / yoo.be | current / 2026-09 | 2026-09-17 | pattern |
| 11 | FidelityFX-FSR2-OpenGL + porting blog | GitHub / J.D. Montoya | ~2023-24 | 2026-09-17 | pattern |
| 12 | FSR3.1 SDK docs + FSR3Unity | AMD GPUOpen / GitHub | 2024–25 | 2026-09-17 | version |
| 13 | FSR4 release + XeSS + OptiScaler bridge | GPUOpen / Intel / VideoCardz | 2025 / 2025-26 / 2026-02 | 2026-09-17 | version |
| 14 | NV_mesh_shader spec + niagara + chaoticbob | Khronos / zeux / chaoticbob | 2019 / ongoing / 2024 | 2026-09-17 | pattern |
| 15 | The Forge v1.57 + SCHUETZ-2026-CURAST | Confetti / TU Wien I3D | 2024-05 / 2026 | 2026-09-17 | landscape |
| 16 | Visibility buffer post-mortem | Cinevva | 2026-05 | 2026-09-17 | performance |
| 17 | ARB_sparse_texture / sparse_texture2 | Khronos registry | 2015-era | 2026-09-17 | pattern |
| 18 | Qwen3-Coder repo + HF cards | Qwen Team | 2025–26 | 2026-09-17 | landscape |
| 19 | DeepSeek-Coder-V2 + runlocalai | DeepSeek-AI / RunLocalAI | 2024–25 | 2026-09-17 | version |
| 20 | Best local coding models guide | LLM Configurator | 2026 | 2026-09-17 | landscape |
| 21 | llama.cpp grammars/README + deepwiki | llama.cpp project | 2025 | 2026-09-17 | pattern |
| 22 | XGrammar + arXiv 2411.15100 | MLC-AI | 2024–25 | 2026-09-17 | performance |
| 23 | constrained-diffusion.ai + arXiv 2608.03065 | research | 2025–26 | 2026-09-17 | pattern |
| 24 | CRANE (ICML'25) + GAD (NeurIPS'24) | PMLR / NeurIPS | 2024–25 | 2026-09-17 | pattern |
| 25 | llama.cpp server /infill + PR10724 | llama.cpp | 2024–25 | 2026-09-17 | pattern |
| 26 | RAG for codebase architecture | llmversus.com | 2025 | 2026-09-17 | pattern |
| 27 | How Claude Code works in large codebases | Anthropic | 2025–26 | 2026-09-17 | pattern |
| 28 | Codebase RAG (Cursor +12.5%) | samuelfaj.com | 2025 | 2026-09-17 | performance |
| 29 | nomic-embed-code-GGUF + nomic.ai + CodeRankEmbed | Nomic AI / AgentiX-E | 2025 | 2026-09-17 | landscape |
| 30 | MCP surfaces + narrow tools essay | npm / samuelfaj.com | 2025–26 | 2026-09-17 | pattern |
| 31 | AI_Loop_MCP + forloop-mcp | GitHub | 2025–26 | 2026-09-17 | pattern |
| 32 | JetBrains MCP pattern | Medium/CodeX | 2025 | 2026-09-17 | pattern |
| 33 | cgvc25-ecs-comparison | StaffsUniGames / CGVC | 2025 | 2026-09-17 | performance |
| 34 | StorageType enum docs | docs.rs / Bevy | current | 2026-09-17 | pattern |
| 35 | flecs storage series + FAQ + relationships/observers | S. Mertens / flecs | 2024–25 / current | 2026-09-17 | pattern |
| 36 | EnTT entity docs | EnTT | current | 2026-09-17 | landscape |
| 37 | Parallelizing the Naughty Dog Engine | GDC 2015 (Gyrling) | 2015 | 2026-09-17 | pattern |
| 38 | enkiTS | enkisoftware | current | 2026-09-17 | landscape |
| 39 | Work-stealing pool paper | CSIT 2025-1-9 | 2025 | 2026-09-17 | performance |
| 40 | C++26 async/parallel + cppreference execution | StreamHPC / cppreference | 2026-06 | 2026-09-17 | version |
| 41 | SIMD Matters | box2d.org (Catto) | 2024-08 | 2026-09-17 | pattern |
| 42 | Releasing Box2D 3.0 | box2d.org | 2024-08 | 2026-09-17 | performance |
| 43 | SIMD for Collision | box2d.org | 2026-07 | 2026-09-17 | pattern |
| 44 | Jolt CROSS_PLATFORM_DETERMINISTIC docs | Jolt (Rouwe) | current | 2026-09-17 | performance |
| 45 | Bannermen lockstep + ferrets | Photon / GitHub | n/a | 2026-09-17 | pattern |
| 46 | ScopeStack | EA / Frostbite | ~2010-13 | 2026-09-17 | pattern |
| 47 | monotonic_buffer_resource | cppreference / libstdc++ | current | 2026-09-17 | pattern |
| 48 | Meridian memory-model + myth arena | GitHub | current | 2026-09-17 | pattern |
| 49 | GetRuntimeAttestationReport | Microsoft Learn | 2026-01 | 2026-09-17 | version |
| 50 | CET user shadow stack policy + HSP + CFI survey | MS Learn / techcommunity / paragmali | current/2026 | 2026-09-17 | version |
| 51 | EtwTiViewer + meekolab + benjitrapp ETW-TI | GitHub / research blogs | 2026-06 | 2026-09-17 | version |
| 52 | ProcessInstrumentationCallback (winternl + cirosec + oblivion) | research blogs | n/a | 2026-09-17 | pattern |
| 53 | DoomSyscalls | GitHub PoC | 2025-era | 2026-09-17 | pattern |
| 54 | SindriKit 1.3.0 writeup | sibouzitoun.tech | n/a | 2026-09-17 | pattern |
| 55 | ETW patching | maldev docs | n/a | 2026-09-17 | pattern |
| 56 | Kernel_VADInjector | GitHub | n/a | 2026-09-17 | pattern |
| 57 | Masking Malicious Memory Artifacts III | forrest-orr.net | n/a | 2026-09-17 | pattern |
| 58 | BusterCall + KVC + CVE-2025-8061 | GitHub / sibouzitoun | 2025 | 2026-09-17 | pattern |
| 59 | DMA detection vectors + hwidchange | dma.lystic.dev / hwidchange | 2026 | 2026-09-17 | landscape |
| 60 | Vanguard on-demand + motherboard update | Riot Games | n/a | 2026-09-17 | landscape |
| 61 | Heartbeat docs | ravenac.net | n/a | 2026-09-17 | pattern |
| 62 | anti-emulation + RDTSC + kernel-anticheats + zebonastic | secret.club / kstrlworks / meekolab | 2020–26 | 2026-09-17 | pattern |
| 63 | ACTK SpeedHackDetector docs | codestage | current | 2026-09-17 | pattern |
