// LedgerAssuranceTest - L-8 查證式史官無頭測試
// 覆蓋 spec-l8 I/O 矩陣：seed 確定性、自引種、三態結論
// （相符/存疑×偽帳·疑帳/拒絕）、全覆蓋抽驗、小帳簿、戰報文句
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

// 剝除第 idx 筆 entry 的 "prov" 段 → 斷鏈（沿用 L-7 篡改手法）
static std::string StripProv(std::string json, int idx) {
    size_t pos = json.find("\"entries\":[");
    if (pos == std::string::npos) return json;
    for (int i = 0; i <= idx; ++i) {
        pos = json.find('{', pos);
        if (pos == std::string::npos) return json;
        ++pos;
    }
    const size_t bound = json.find("\"prev\":", pos);
    const size_t pv = json.find("\"prov\":{", pos);
    if (pv == std::string::npos || pv > bound) return json;
    const size_t pvEnd = json.find("},", pv);
    return json.erase(pv, pvEnd - pv + 2);
}

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

// 乾淨帳簿：三來源分錄
static LedgerChain CleanChain() {
    LedgerChain c;
    c.Append(LedgerEntry::BattleVictory(120, 1, "斷橋之捷"));
    c.Append(LedgerEntry::Recruitment(30, 1));
    c.Append(LedgerEntry::Negotiation(80, 1, "渭水之盟"));
    c.Append(ManualEntry(15, 2, "佚名舊帳"));
    return c;
}

// 含 malformed 偽帳的帳簿（鏈合法、會計不成立）
static LedgerChain ForgedChain() {
    LedgerChain c = CleanChain();
    LedgerEntry fake;
    fake.debit = LedgerAccount::Martial;
    fake.credit = LedgerAccount::Martial;
    fake.amount = 50;
    fake.prov.source = EntrySource::Myth;
    c.InjectForgery(fake);
    return c;
}

int main() {
    printf("=== Ledger Assurance Tests (L-8) ===\n\n");

    const LedgerChain clean = CleanChain();

    // ---- [1] 確定性 ----
    printf("[1] 抽驗確定性\n");
    {
        const LedgerAssurance a1 = AssureLedger(clean, 42, 3);
        const LedgerAssurance a2 = AssureLedger(clean, 42, 3);
        Check(a1.verdict == a2.verdict && a1.sampled == a2.sampled &&
                  a1.sampleBad == a2.sampleBad,
              "同帳簿同 seed → 同結論");
        const LedgerAssurance a3 = AssureLedger(clean, 7, 3);
        Check(a3.verdict == AssuranceVerdict::Assured,
              "不同 seed 同樣合法（帳本體乾淨）");
    }

    // ---- [2] 自引種 ----
    printf("\n[2] seed==0 自引種\n");
    {
        const LedgerAssurance s1 = AssureLedger(clean, 0, 2);
        const LedgerAssurance s2 = AssureLedger(clean, 0, 2);
        Check(s1.sampled == s2.sampled && s1.verdict == s2.verdict,
              "自引種同帳簿恆同樣本");
        Check(s1.sampled == 2 && s1.sampleSize == 2, "抽驗筆數正確");
    }

    // ---- [3] 相符 ----
    printf("\n[3] 乾淨帳簿 → Assured\n");
    {
        const LedgerAssurance a = AssureLedger(clean, 0, 3);
        Check(a.verdict == AssuranceVerdict::Assured, "verdict=相符");
        Check(a.sampled == a.sampleSize && a.sampleBad == -1,
              "抽驗全過");
        Check(a.brokenAt == -1 && a.unsoundAt == -1 &&
                  a.suspectCount == 0,
              "無斷點無偽帳無疑帳");
    }

    // ---- [4] 存疑（偽帳）----
    printf("\n[4] 偽帳帳簿 → Qualified\n");
    {
        const LedgerChain forged = ForgedChain();
        const LedgerAssurance a = AssureLedger(forged, 0, 5);
        Check(forged.Verify() == -1, "偽帳鏈仍合法");
        Check(a.verdict == AssuranceVerdict::Qualified,
              "verdict=存疑（鏈合法但 malformed）");
        Check(a.unsoundAt == 4, "偽帳定位第 5 筆");
    }

    // ---- [5] 存疑（疑帳標記）----
    printf("\n[5] 標記疑帳 → Qualified\n");
    {
        LedgerChain marked = CleanChain();
        marked.MarkSuspect(1);
        const LedgerAssurance a = AssureLedger(marked, 0, 3);
        Check(a.verdict == AssuranceVerdict::Qualified,
              "verdict=存疑（疑帳在場）");
        Check(a.suspectCount == 1 && a.unsoundAt == -1 &&
                  a.brokenAt == -1,
              "純疑帳：鏈與健全皆過");
    }

    // ---- [6] 拒絕（斷鏈）----
    printf("\n[6] 斷鏈帳簿 → Adverse\n");
    {
        LedgerChain tampered;
        Check(tampered.FromJson(StripProv(clean.ToJson(), 0)),
              "篡改檔載入");
        const LedgerAssurance a = AssureLedger(tampered, 0, 3);
        Check(a.verdict == AssuranceVerdict::Adverse, "verdict=拒絕");
        Check(a.brokenAt == 0, "斷點在第 1 筆");
    }

    // ---- [7] 全覆蓋抽驗 ----
    printf("\n[7] sampleN >= size 全覆蓋\n");
    {
        const LedgerAssurance a = AssureLedger(clean, 0, 99);
        Check(a.sampled == 4 && a.sampleSize == 4,
              "全覆蓋：每筆皆抽驗通過");
        // 抽驗樣本本身抓到斷鏈：篡改中間筆 hash 後全覆蓋必命中
        std::string json = clean.ToJson();
        const size_t hp = json.rfind("\"hash\":\"");
        json.replace(hp + 8, 16, "0000000000000000");
        LedgerChain broken;
        broken.FromJson(json);
        const LedgerAssurance b = AssureLedger(broken, 0, 99);
        Check(b.sampleBad == 3, "全覆蓋抽驗命中末筆篡改");
        Check(b.verdict == AssuranceVerdict::Adverse, "verdict=拒絕");
    }

    // ---- [8] 戰報文句三態 ----
    printf("\n[8] 查帳段確信結論句\n");
    {
        HistorianInput in;
        in.ledger = &clean;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "史官具結：帳目相符"), "相符結論句");
        Check(Has(r.text, "抽驗 3 筆皆符"), "抽驗筆數入句");

        const LedgerChain forged = ForgedChain();
        HistorianInput qin;
        qin.ledger = &forged;
        HistorianReport qr = ComposeHistorianReport(qin);
        Check(Has(qr.text, "史官存疑"), "存疑結論句");
        Check(Has(qr.text, "不予具結"), "存疑不具結");
        Check(Has(qr.text, "借貸不成立"), "存疑理由：偽帳");

        LedgerChain tampered;
        tampered.FromJson(StripProv(clean.ToJson(), 0));
        HistorianInput ain;
        ain.ledger = &tampered;
        HistorianReport ar = ComposeHistorianReport(ain);
        Check(Has(ar.text, "拒絕具結"), "拒絕結論句");
        Check(Has(ar.text, "不予採信"), "拒絕採信用語");

        // assuranceSample<=0 → 關閉結論句（維持舊句式）
        HistorianInput off;
        off.ledger = &clean;
        off.assuranceSample = 0;
        HistorianReport orpt = ComposeHistorianReport(off);
        Check(!Has(orpt.text, "史官具結"), "sample<=0 關閉結論句");
        Check(Has(orpt.text, "登錄字號"), "L-6 字號仍在");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
