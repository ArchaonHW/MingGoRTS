// LedgerProvenanceTest - L-7 產生軌跡無頭測試
// 覆蓋 spec-l7 I/O 矩陣：模板帶源、prov 入 hash、剝除/補登斷鏈、
// 舊檔降級、roundtrip、查帳來源統計、InjectForgery 相容
#include "Gameplay/HistorianReport.h"
#include "Gameplay/Ledger.h"

#include <cstdio>
#include <string>

using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s\n", name);
    }
}

static bool Has(const std::string& hay, const char* needle) {
    return hay.find(needle) != std::string::npos;
}

// 第 idx 筆 entry 物件在 JSON 中的內容起點（'{' 之後）
static size_t EntryPos(const std::string& json, int idx) {
    size_t pos = json.find("\"entries\":[");
    if (pos == std::string::npos) return pos;
    for (int i = 0; i <= idx; ++i) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) return pos;
        ++pos;
    }
    return pos;
}

// 剝除第 idx 筆的 "prov":{...}, 段（篡改手法：刪除產生軌跡）
static std::string StripProv(std::string json, int idx) {
    const size_t e = EntryPos(json, idx);
    if (e == std::string::npos) return json;
    const size_t bound = json.find("\"prev\":", e);
    const size_t pv = json.find("\"prov\":{", e);
    if (pv == std::string::npos || pv > bound) return json;
    const size_t pvEnd = json.find("},", pv);
    if (pvEnd == std::string::npos) return json;
    return json.erase(pv, pvEnd - pv + 2); // 含尾逗號
}

// 對第 idx 筆補登 prov（篡改手法：事後捏造來源），插於 "prev" 前
static std::string InjectProv(std::string json, int idx,
                              const char* provJson) {
    const size_t e = EntryPos(json, idx);
    if (e == std::string::npos) return json;
    const size_t p = json.find("\"prev\":", e);
    if (p == std::string::npos) return json;
    return json.insert(p, provJson);
}

// 舊格式分錄：prov 全預設，canon/hash 與 L-7 之前逐位相同
static LedgerEntry ManualEntry(int amount, int chapter,
                               const std::string& memo) {
    LedgerEntry e;
    e.debit = LedgerAccount::Martial;
    e.credit = LedgerAccount::Supply;
    e.amount = amount;
    e.chapter = chapter;
    e.memo = memo;
    return e;
}

int main() {
    printf("=== Ledger Provenance Tests (L-7) ===\n\n");

    // ---- [1] 模板帶源 ----
    printf("[1] 模板預設來源\n");
    {
        Check(LedgerEntry::BattleVictory(120, 1).prov.source ==
                  EntrySource::Battle,
              "BattleVictory → Battle");
        Check(LedgerEntry::Recruitment(30, 1).prov.source ==
                  EntrySource::Refit,
              "Recruitment → Refit");
        Check(LedgerEntry::Negotiation(80, 1).prov.source ==
                  EntrySource::Negotiation,
              "Negotiation → Negotiation");
        const LedgerEntry& b = LedgerEntry::BattleVictory(1, 1);
        Check(b.prov.tick == 0 && b.prov.eventId.empty(),
              "tick/eventId 預設");
    }

    // ---- [2] prov 入 hash ----
    printf("\n[2] prov 納入雜湊\n");
    {
        LedgerEntry a = LedgerEntry::BattleVictory(120, 1, "斷橋");
        LedgerEntry b = a;
        b.prov.tick = 42;
        b.prov.eventId = "ev-xyz";
        Check(LedgerChain::HashEntry(LedgerChain::kGenesisHash, a) !=
                  LedgerChain::HashEntry(LedgerChain::kGenesisHash, b),
              "同 entry 不同 prov → 雜湊不同");
        LedgerEntry c = a;
        c.prov.source = EntrySource::Myth;
        Check(LedgerChain::HashEntry(LedgerChain::kGenesisHash, a) !=
                  LedgerChain::HashEntry(LedgerChain::kGenesisHash, c),
              "不同 source → 雜湊不同");
        // 相容性核心：失考分錄與帶源分錄同額同註，雜湊必須不同
        LedgerEntry d = ManualEntry(120, 1, "斷橋");
        Check(LedgerChain::HashEntry(LedgerChain::kGenesisHash, a) !=
                  LedgerChain::HashEntry(LedgerChain::kGenesisHash, d),
              "帶源模板 vs 失考同額分錄 → 雜湊不同");
    }

    // ---- [3] 帶 prov 上鏈 + 預設 prov 不寫欄位 ----
    printf("\n[3] 帶 prov 上鏈\n");
    LedgerChain chain;
    {
        LedgerEntry e1 = LedgerEntry::BattleVictory(120, 1, "斷橋之捷");
        e1.prov.tick = 42;
        e1.prov.eventId = "duanqiao-win";
        Check(chain.Append(e1), "帶 prov 分錄可 Append");
        Check(chain.Append(LedgerEntry::Recruitment(30, 1)),
              "模板分錄 Append");
        Check(chain.Verify() == -1, "Verify 通過");
        Check(Has(chain.ToJson(), "\"prov\":{"), "非預設 prov 寫入檔案");
        LedgerChain plain;
        plain.Append(ManualEntry(10, 1, "舊帳"));
        Check(!Has(plain.ToJson(), "\"prov\""), "預設 prov 不寫欄位");
    }

    // ---- [4] roundtrip ----
    printf("\n[4] 持久化 roundtrip\n");
    {
        LedgerChain back;
        Check(back.FromJson(chain.ToJson()), "回讀");
        Check(back.Verify() == -1, "回讀後鏈完整");
        const LedgerEntry& e0 = back.Entries()[0].entry;
        Check(e0.prov.source == EntrySource::Battle &&
                  e0.prov.tick == 42 &&
                  e0.prov.eventId == "duanqiao-win",
              "prov 三欄位保留");
    }

    // ---- [5] 剝除 prov → 斷鏈 ----
    printf("\n[5] 剝除 prov 篡改\n");
    {
        const std::string tampered = StripProv(chain.ToJson(), 0);
        Check(tampered != chain.ToJson(), "prov 段被移除");
        LedgerChain t;
        Check(t.FromJson(tampered), "篡改檔可讀入");
        Check(t.Verify() == 0, "Verify 斷於第 1 筆");
    }

    // ---- [6] 補登 prov → 斷鏈 ----
    printf("\n[6] 補登 prov 篡改\n");
    {
        LedgerChain plain;
        plain.Append(ManualEntry(10, 1, "舊帳")); // 預設 prov → 舊格式
        const std::string tampered =
            InjectProv(plain.ToJson(), 0, "\"prov\":{\"src\":\"Myth\",\"t\":7},");
        Check(tampered != plain.ToJson(), "prov 段被注入");
        LedgerChain t;
        Check(t.FromJson(tampered), "補登檔可讀入");
        Check(t.Verify() == 0, "補登後斷於第 1 筆");
    }

    // ---- [7] 舊檔降級 ----
    printf("\n[7] 舊檔降級\n");
    {
        LedgerChain ref;
        ref.Append(ManualEntry(120, 1, "斷橋")); // 預設 prov = 舊格式檔
        Check(!Has(ref.ToJson(), "\"prov\""), "舊格式無 prov 欄位");
        LedgerChain t;
        Check(t.FromJson(ref.ToJson()), "舊檔載入");
        Check(t.Verify() == -1, "舊檔鏈完整（hash 相容）");
        Check(t.Entries()[0].entry.prov.IsDefault(), "prov 降級為失考");
    }

    // ---- [8] 查帳段來源統計 ----
    printf("\n[8] 史官查帳來源統計\n");
    {
        HistorianInput in;
        in.ledger = &chain;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "戰陣"), "統計含戰陣");
        Check(Has(r.text, "營務"), "統計含營務");
        Check(Has(r.text, "戰陣 1") && Has(r.text, "營務 1"),
              "各來源筆數在場");
    }

    // ---- [9] InjectForgery 相容 ----
    printf("\n[9] 偽帳通道相容\n");
    {
        LedgerChain f;
        f.Append(ManualEntry(10, 1, "舊帳"));
        LedgerEntry fake;
        fake.debit = LedgerAccount::Martial;
        fake.credit = LedgerAccount::Martial;
        fake.amount = 50;
        fake.prov.source = EntrySource::Myth;
        fake.prov.eventId = "forged-tidings";
        Check(f.InjectForgery(fake) == 1, "帶 prov 偽帳可注入");
        Check(f.Verify() == -1, "偽帳鏈仍合法");
        Check(f.SoundnessViolation() == 1, "會計不健全揭露");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
