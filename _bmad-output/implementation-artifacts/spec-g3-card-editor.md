---
title: 'G-3 敵將編輯器——CardEditor 可編輯存回半邊 + doctrine 交叉引用驗證'
type: 'feature'
created: '2026-09-25'
status: 'done'
baseline_commit: 'c6a450b'
context:
  - '{project-root}/_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md'
  - '{project-root}/Gameplay/CardValidator.h'
  - '{project-root}/Gameplay/DoctrineLibrary.h'
  - '{project-root}/Gameplay/EnemyGeneral.h'
  - '{project-root}/Serialization/JsonParser.h'
  - '{project-root}/Campaign/JsonWriter.h'
  - '{project-root}/Examples/CardValidator.cpp'
---

<frozen-after-approval reason="human-owned intent — do not modify unless human renegotiates">

## Intent

**Problem:** epics Story G.3 要求「三軸人格/signature/對話碎片
可編輯並驗證存回」。`729824d` 已落地 CardValidator（掃描/
診斷半邊,CLI + headless 測試齊），但**沒有任何寫回路徑**——
設計者仍得手改 JSON 再祈禱語義正確。另有一條隱性缺口：
G-2 引入 `signatureDoctrineId` 欄位指向 `assets/doctrine`
卡池 id，CardValidator 目前**不驗證這條跨檔引用**——寫錯 id
不報錯，掠奪解鎖靜默失敗（正是 AC 點名的「unknown doctrine
id → 拒存 + 明確錯誤」情境）。

**Approach:** 兩件補齊 G-3——
(1) `Gameplay::CardEditor`：載入卡檔成可變 JsonValue DOM，
    提供型別化編輯操作（人格軸/簽名卡/signatureDoctrineId/
    文字碎片/cards[] 槽位），`Save()` 前先跑 CardValidator
    全套驗證（含 doctrine 交叉引用）——**有 error 即拒存**，
    合法才 tmp+rename 原子寫回（與 CampaignState 存檔同慣例）。
    輸出走 `WriteJsonValue`（物件鍵排序輸出，diff 可審）。
(2) `CardValidator` 加 doctrine 目錄交叉驗證：`ValidateDir`
    /`ValidateString` 增 `doctrineDir` 參數（空=跳過，保舊
    行為），`signatureDoctrineId`/`cards[].doctrineId` 不在
    DoctrineLibrary 池內 → error。CLI 加 `--doctrine-dir`。

**對話碎片界定：** `potato.character_card/1` schema 無獨立
dialogue 欄——卡上最接近的自由文字是 `lore`/`epithet`/
`courtesyName`/`historicalBattle`；敵將語音碎片屬
NarrativePack（另一檔案型別，其 editor 屬 C epic 範疇）。
本 story 以通用 `SetText(field, value)` 覆蓋卡片文字欄，
epics 的「對話碎片」按此詮釋。

## Boundaries & Constraints

**Always:**
- `WriteJsonValue`（Campaign/JsonWriter.h）序列化——物件鍵
  排序，產出 deterministic 可 diff
- 存檔走 tmp+rename（原子寫回），失敗不留半成品檔
- Save 前必跑完整驗證；error → 拒存並回傳 CardReport
- 編輯操作不回寫記憶體以外的狀態；每個 op 立即型別檢查
  （unknown trigger/action → op 本身失敗，不等 Save）

**Never:**
- 不引入第三方 JSON 庫（JsonValue DOM 已可變，`objectValue`
  為 public 成員可直接改）
- 不改 `potato.character_card/1` schema——編輯器只消費既有
  欄位，不新增必填欄
- 不做 ImGui 面板——AC「ImGui 面板或 CLI 驗證器」為擇一，
  CLI 已滿足 headless/CI 需求；面板屬 F epic 呈現層後續
- 不改 CardValidator 既有 issue 分級語義（error/warn 維持）

**Ask First:**
- cards[] 槽位編輯範圍：提案含 AddRule/RemoveRule（完整
  編輯能力）——若只要最小 AC，可縮到「signature + 人格軸 +
  文字」三類
- CLI 語法：提案 `--set field=value` 通用 + `--set-signature`
  專用旗標

## Design

### CardEditor（Gameplay/CardEditor.h/.cpp）

```cpp
class CardEditor {
public:
    bool Load(const std::string& path);            // 檔案→DOM
    bool Parse(const std::string& json);           // 字串→DOM
    void BindDoctrineDir(const std::string& dir);  // 交叉驗證來源

    // 型別化編輯（失敗回 false + LastError()）
    bool SetPersonality(const std::string& axis, float v); // 夾 0..100
    bool SetSignature(const std::string& name,
                      const std::string& trigger,
                      const std::string& action,
                      float threshold);
    bool SetSignatureDoctrineId(const std::string& id);    // 綁池時驗存在
    bool SetText(const std::string& field,
                 const std::string& value);                // lore 等文字欄
    bool AddCardRule(const std::string& name,
                     const std::string& trigger,
                     const std::string& action,
                     float threshold, int priority, float cooldown);
    bool RemoveCardRule(int index);

    CardReport Validate() const;                  // 跑 CardValidator 全套
    bool Save();                                  // 覆寫原檔（tmp+rename）
    bool Save(const std::string& path);           // 另存（同樣驗證守門）
    const std::string& LastError() const;
    const JsonValue& Root() const;                // 唯讀 DOM（測試對帳）
};
```

- `SetPersonality`：axis 限 aggression/discipline/cunning，
  v 夾 0..100（與 validator 的 warn 語義對齊——編輯器主動
  夾取，不產出越界值）
- `SetSignature`/`AddCardRule`：trigger/action 先過
  `CardValidator::TriggerNames()/ActionNames()` 白名單，
  未知值 op 直接失敗（fail fast，不靠 Save 才炸）
- `SetSignatureDoctrineId`：BindDoctrineDir 已綁且 id 非空
  → 必須在池內（DoctrineLibrary::Find）；未綁池時僅記錄，
  交叉檢查延後到 Validate()——與 CLI 的 --doctrine-dir
  是否給定行為一致
- `SetText` 白名單：name/epithet/courtesyName/faction/role/
  lore/historicalBattle/art（防亂寫結構欄位如 schema/id）
- `Save()`：空路徑（Parse 來的 DOM）→ 回 false；寫出前
  `Validate().Ok()` 守門；WriteJsonValue → tmp → rename

### CardValidator 擴充

- `ValidateString/ValidateFile/ValidateDir` 增最後參數
  `doctrineDir = ""`；非空時建 DoctrineLibrary::LoadDir 一次
  （Dir 層只建一次不重複掃），`signatureDoctrineId` 與
  `cards[].doctrineId` 非空且 `Find()==nullptr` → error
  「未知 doctrine id」
- 池目錄不存在/全壞 → 每卡 warn「doctrine 池不可用」一次，
  不誤報 error（池本身壞不是卡的錯）

### CLI（Examples/CardEditor.cpp）

```
CardEditor <card.json> [--assets-root assets] [--doctrine-dir assets/doctrine]
           [--set field=value]... [--set-personality axis=v]...
           [--set-signature "name|trigger|action|threshold"]
           [--set-signature-id id] [--add-rule "name|trigger|action|th|pri|cd"]
           [--remove-rule idx] [--dry-run] [--out path]
```

- 無 `--set*` 操作時 = validate-only 模式（同 CardValidator 但單卡）
- `--dry-run`：套操作+驗證+印結果，不寫檔
- 任一 op 失敗或驗證有 error → exit 1，明確錯誤訊息
- 成功 exit 0 印 diff 摘要（改了哪些欄位）

## Acceptance Criteria

1. `SetPersonality("aggression", 75)` 後 `Save()`，重新
   `EnemyGeneral::LoadFromFile` 讀到 aggression=75——
   編輯→存回→載入整鏈可驗
2. `SetSignatureDoctrineId("nonexistent")` + doctrineDir 綁定
   → `Validate()` 報 error，`Save()` 拒存且原檔不變
3. `SetSignature` 餵未知 trigger → op 回 false，DOM 不改
4. 對 `assets/cards/` 全目錄現有卡跑 `CardEditor --dry-run`
   → 全數 validate OK（既有卡不能被我們自己的驗證器拒）
5. `--set lore=新敘述` → 存回後 JSON 含新值、其餘欄位不變
   （round-trip 保真：portrait/stats/cards 等未觸及欄位逐值
   相等——WriteJsonValue 鍵序變動可接受，語義相等即可）
6. tmp+rename：Save 中途模擬失敗（目錄唯讀）→ 原檔完整
7. CLI：`--doctrine-dir` 指向壞池目錄 → warn 不擋；指向
   好池 + 壞 id → error 擋存

## Tasks

- [ ] `Gameplay/CardEditor.h/.cpp`（DOM 編輯 + 驗證守門 + 原子寫回）
- [ ] `CardValidator` 加 `doctrineDir` 交叉驗證參數
- [ ] `Examples/CardEditor.cpp` CLI
- [ ] `Examples/CardEditorTest.cpp` headless 測試（AC 1-7）
- [ ] `Examples/CardValidator.cpp` CLI 補 `--doctrine-dir` 旗標
- [ ] CMakeLists 註冊 CardEditor target + CardEditorTest 入 POTATO_TESTS
- [ ] MSVC + MinGW 建置、全量 CTest、banned 掃描
- [ ] 佇列補登 g-3、sprint-status → done、spec → done + Review Log

## Suggested Review Order

1. `Gameplay/CardEditor.h` — API 契約與白名單
2. `Gameplay/CardEditor.cpp` — DOM 改寫與原子 Save
3. `Gameplay/CardValidator.cpp` — doctrine 交叉驗證段
4. `Examples/CardEditorTest.cpp` — AC 覆蓋
</frozen-after-approval>

## Review Log

- **2026-09-25 落地**——三筆組成完整故事：
  - `729824d`：CardValidator 診斷半邊（平行 session，先於本 spec）
  - `4d679f6`：CardValidator::FixCard 自動修 + CLI --fix
    （平行 session；本次的 doctrineDir 交叉驗證 hunk 因其
    path-limited commit 吃工作區整檔而一併入帳）
  - `9903e89`：CardEditor 型別化編輯 + Save 驗證守門 +
    Serialization/JsonWriter.h 消費端遷移收尾
- **平行交織紀錄**：兩 session 同改 CardValidator.h/.cpp——
  對方 --fix 寫回與本方 CardEditor 互補（自動修 vs 手動編輯），
  FixResult 與 CardEditor 共用 CardValidator 驗證核心不衝突。
  CMakeLists 採 hash-object staged 法切離平行 hunk。
- **JsonWriter 下移**：CardEditor 在 Gameplay 層，不能依賴
  Campaign::WriteJson——寫出器屬引擎級序列化基建，故移到
  Serialization/ 收斂 namespace 為 Potato（見檔頭註記）。
  四個消費端 include 同步遷移、舊路徑刪除。
- **驗證**：MSVC+MinGW 雙綠；CardEditorTest 31 格；
  CardValidatorTest（含平行 session 的 fix 格）全綠；
  全量 CTest 107/107；banned 掃描乾淨；CLI 實掃
  assets/cards 31 卡 0 error（doctrineDir 交叉驗證啟用下
  三張 signatureDoctrineId 全解析）。
- **偏離 spec**：cards[].doctrineId 檢查保留（schema 尚無
  消費者，屬未來相容）；「對話碎片」按 lore/epithet/
  courtesyName/historicalBattle 詮釋（card schema 無
  dialogue 欄）。
