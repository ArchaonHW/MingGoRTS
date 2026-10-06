# Evolution: 武將名冊（Card Gallery）— 立繪上屏

## 目標
IDE GUI 新增「武將名冊」面板：掃描 `assets/cards/**/*.json`，
左列卡片清單、右側顯示立繪 PNG + 卡面 metadata（name/epithet/rarity/
faction/stats/art 路徑）。立繪經 ImageCodec 解碼 → GL texture →
ImGui::Image 上屏。

## Boundary Map

### New
- `IDEGUI::RenderCardGallery()` — 面板本體（清單 + 詳情 + 立繪）
- Card texture cache：`std::unordered_map<std::string, GLuint>`，lazy
  decode+upload（GL context 在 render pass 內有效）
- `showCardGallery` state flag + View menu item
- 資產根目錄解析：`art` 欄位相對 `assets/`（如 `cards/art/roc_x.png`
  → `assets/cards/art/x.png`）；沿用 `EnemyGeneral::LoadFromFile`
  的前綴 fallback 慣例（"", "../", "../../", "../../../"）

### Modified
- `MingGoRTS_IDE/GUI/IDEGUI.h` — flag + cache member + method decl
- `MingGoRTS_IDE/GUI/IDEGUI.cpp` — View menu item + panel dispatch
- `CMakeLists.txt` — MingGoRTS_IDE_GUI 加 `Rendering/ImageCodec.cpp`
  與 `Logging/Logger.cpp`（ImageCodec 的 Logger 依賴；不連整支
  PotatoEngine，避免把 glfw/engine 全域拖進 IDE target）

### Untouched（必須維持現狀）
- `PotatoEngine`/`Gameplay` lib 邊界（IDE 不反向依賴）
- `EnemyGeneral`/`Roster` 資料模型（art 欄位已就緒）
- `PortraitBaker`/`PortraitRenderer` 產線
- 既有所有面板行為
- card JSON schema（`potato.character_card/1`）

## Integration Points
| 點 | 說明 | 風險 |
|---|---|---|
| GL texture 上傳 | render pass 內 lazy `glTexImage2D`；用 imgui_impl_opengl3_loader.h 內建 loader | 低 |
| 圖片解碼 | `ImageCodec::DecodeImage` → RGBA8 | 低 |
| 卡片列舉 | `std::filesystem` 遞迴 `assets/cards/**.json` + JsonParser.h（header-only） | 低 |
| 工作目錄 | IDE exe 可能跑在 build/bin/Release；asset 根用前綴 fallback | 中 |
| Texture 生命周期 | 面板關閉仍保留 cache；程序結束前 glDeleteTextures | 低 |
| 缺圖降級 | art 欄位空或檔案不存在 → 顯示 placeholder 文字 | 低 |

## Step 2: Impact Analysis

### 依賴圖
- `Main.cpp`（render loop）→ 新增 `ideGUI.RenderCardGallery()` 呼叫；
  `ideGUI.Shutdown()` 在 `ImGui_ImplOpenGL3_Shutdown` 之前 →
  GL context 仍有效，可安全 `glDeleteTextures`
- `IDEGUI.cpp` → View menu + panel 實作；`IDEGUIState` 加
  `showCardGallery` flag（bool 追加，不破壞既有 layout）
- GL 函式：IDE target 未定義 `GLFW_INCLUDE_NONE` → glfw3.h 帶系統
  gl.h，`glGenTextures/glTexImage2D/glBindTexture`（GL 1.1）直接可用，
  不需 glad
- PNG 解碼：`ImageCodec::DecodeImage`（需 `Rendering/ImageCodec.cpp`
  + `Logging/Logger.cpp` 進 IDE target；JsonParser.h 為 header-only）
- 資產根：`core->GetConfig().workspacePath + "/assets"`
  （Main.cpp 已設 `C:\HWC\MingGoRTS`；fallback 前綴保底）

### Breaking-change 風險
- IDEGUIState 追加 bool——純增量，無 ABI/行為變更
- Main.cpp render loop 加一行呼叫——既有面板順序不變
- CMake 加兩個源檔——不動 link 結構
- 無既有介面/資料形狀變更；card JSON 為唯讀消費

### 風險評估
| Risk | Likelihood | Impact | Mitigation |
|---|---|---|---|
| Logger.cpp 獨立編譯失敗（隱含依賴） | 低 | 中 | 建置驗證；失敗則改連 PotatoEngine |
| 每帧重解碼 PNG | 中 | 低 | texture cache 只上傳一次 |
| 立繪檔案缺失 | 中 | 低 | placeholder 降級顯示 |
| texture 洩漏 | 低 | 低 | Shutdown() 統一 glDeleteTextures |
| IDE GL 3.0 compat vs 核心 profile | 低 | 低 | 只用 GL 1.1 texture API |

## Step 3: Implementation Plan（含潛在需求發現）

### 發現的潛在需求
- 缺圖偵測：art/ 28 張圖 vs cards/roc/ 10 張 JSON，需 placeholder + 紅標
- 改卡後重 bake：面板內「重新產生立繪」按鈕呼叫 PortraitBaker
- `art` 空欄位 fallback：`cards/art/<id>.png` 慣例猜路徑
- 熱重載：重 bake 後 invalidate 單張 texture
- I18N：新 key 需動 5 語言 map → 比照既有 literal menu item 跳過

### 增量
- Inc 1：`RenderCardGallery()` 面板 + texture cache + 缺圖降級
- Inc 2：「重新產生立繪」按鈕 + texture invalidate
- 延後：Roster tab、VRM 3D 預覽、I18N key

### 變更檔案
- `IDEGUI.h`：`showCardGallery` flag、`cardTexCache`/`cardList` members、
  `RenderCardGallery()`/`EnsureCardTexture()`/`InvalidateCardTexture()` decl
- `IDEGUI.cpp`：View menu item、panel 實作、Shutdown 釋放 texture
- `Main.cpp`：render loop 加一行 `RenderCardGallery()`
- `CMakeLists.txt`：IDE target +`Rendering/ImageCodec.cpp`+`Logging/Logger.cpp`
