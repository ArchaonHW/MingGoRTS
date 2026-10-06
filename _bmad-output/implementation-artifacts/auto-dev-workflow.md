# 智能自動開發工作流

潛在項目 → 可驗證 commit 的循環。佇列定義在 `auto-dev-queue.yaml`。

## 迴圈（每個 item 一次迭代）

```
1. PICK      從佇列取 status: queued 的最高優先項（B → C → A，同層按 id）
2. SPEC      跑 gds-quick-dev：產 spec-*.md，寫死 acceptance → 使用者核准
3. IMPLEMENT 實作；遵守 AGENTS.md 全部 policy
4. VERIFY    cmake --build（MSVC Debug）→ ctest 全量 → MinGW 編譯相關 target
             + banned 函式掃描（gets|strcpy|strcat|sprintf|vsprintf|scanf）
5. REVIEW    spec 有 review 階段就跑（blind/edge-case/acceptance 三層）
6. COMMIT    conventional commits；佇列 item 標 status: done + commit hash
7. REPORT    一行摘要 + 驗證結果；取下一項或停下
```

## 硬性檢查（任一不過 → 寫 blocked 結果檔並停）

- 開始前工作樹必須 clean（`git status --porcelain` 空）
- 使用者必須核准 spec（不核准 → item 回 queued，不強行實作）
- 不改 `external/`、`build/`；產出物只落在 `output/` 或 `_bmad-output/`
- 驗證失敗 → 修到過或標 blocked，不留半套

## 狀態機

```
queued → specced → implementing → verifying → done
              ↘ blocked（原因寫在 item.note）
```

## 升級規則（何時停下問人）

- spec 核准被拒兩次 → 升級，不自動重試
- 同一 item 驗證失敗三次 → 標 blocked + 寫原因，跳下一項
- 碰 `external/` 需求、新依賴、破壞性操作 → 一律先問
- 佇列清空 → 報告總結，等新任務

## 目前佇列快照（2026-09-17）

| id | 項目 | size | 狀態 |
|----|------|------|------|
| C-1 | SynthDataDemo depth pass | M | queued |
| C-2 | SynthDataDemo instance-seg pass | M | queued |
| A-1 | T-9 回合層 UI（卡槽+編輯器+圖釘） | XL | queued |
| A-2 | T-10 即時層 HUD | L | queued |
| A-3 | T-11 戰後層（戰報+名冊+回放軸） | L | queued |
| A-4 | T-12 單位膠囊體/色塊 | M | queued |
| B-1 | IDE 建議分析移出 render thread | M | queued |
| B-2 | brainstorming 檔案提交決策 | S | blocked（使用者檔案） |
