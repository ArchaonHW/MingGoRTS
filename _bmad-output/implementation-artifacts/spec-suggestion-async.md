# Spec: 智慧建議分析移出 render thread（B-1）

status: done
source: deferred-work.md / auto-dev-queue.yaml B-1

## Intent

`IDEGUI.cpp` 去抖動 500ms 後在 UI 執行緒同步呼叫
`IntelligentSuggestionSystem::GenerateSuggestions`（全檔 regex 掃描 ~8KB），
可能造成每個分析週期卡一幀。把分析搬到背景 worker，UI 執行緒只做
「投遞快照 → 輪詢結果」。

## Approach

在 `IntelligentSuggestionSystem` 加非同步入口，UI 端最小改動：

1. `IntelligentSuggestionSystem` 新增：
   - `SubmitAnalysis(code, filePath, line, col)` → 回傳 job id（`uint64_t`）
   - `PollResult(uint64_t& doneId, std::vector<Suggestion>& out)` → 有結果回 true
   - 內部：單一 worker thread + `std::queue`（`mutex`+`cv`），
     新 job 進來時丟棄未完成的舊 job（**supersede**，不排積壓）
   - `Shutdown()`/`~` join worker，cv 喚醒退出
2. `IDEGUI.cpp` 去抖動區塊改為：
   - 滿 500ms → `SubmitAnalysis`（快照 `std::string(tab.buffer)` 已存在）記住 job id
   - 每幀 `PollResult` → 有結果且 job id 為最新才套用；
     套用前的 dismissed 過濾 / signature 比較 / `state.currentSuggestions` 寫入邏輯不變
3. `GenerateSuggestions` 同步入口保留（測試與其他呼叫端仍用）

## Always

- worker 只碰快照資料，不讀 `tab`/`state`；結果交還只在 UI 執行緒套用
- 無資料競爭：job 佇列、result 槽都走 mutex；`Suggestion` 值語義拷貝
- `Shutdown` 可重入且 join 成功；worker 不洩漏 thread
- supersede：連續打字只跑最新快照，不追舊結果
- 不改 `external/`；不引新依賴（`std::thread`/`condition_variable` 即可）

## Never

- 不改 `GenerateSuggestions` 的同步行為與回傳內容
- 不在 worker 內碰 ImGui / OpenGL / `state`
- 不引入 thread pool 框架

## Acceptance

1. 打字連續觸發 → 只有最後一次快照的分析結果被套用（stale 結果被丟棄）
2. UI 執行緒無 `GenerateSuggestions` 直接呼叫（grep 驗證 IDEGUI.cpp）
3. MSVC Debug 建置 0 錯誤；`DevAssistantSmoke` 及 ctest 全量不回歸
4. 新增/更新測試：Submit→Poll 成功路徑、supersede、Shutdown 時有 pending job 不死結
   （走現有 headless 測試模式，target 進 `POTATO_TESTS`）

## Verify

```
cmake --build build --config Debug
cd build && ctest -C Debug
grep -n "GenerateSuggestions" MingGoRTS_IDE/GUI/IDEGUI.cpp   # 應無直接呼叫
```

## Out of scope

- 分析結果的增量/取消式細粒度（整檔重掃即可）
- 多 worker 並行
