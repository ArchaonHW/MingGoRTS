// LedgerAuditTest - L-3 查帳戰報無頭測試
// HistorianReport 消費 LedgerChain：平衡→借貸相符、斷鏈/偽帳→墨跡未乾
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

int main() {
    printf("=== Ledger Audit Report Tests (L-3) ===\n\n");

    // ---- [1] 無帳簿不書查帳段 ----
    printf("[1] 無帳簿\n");
    {
        HistorianInput in;
        HistorianReport r = ComposeHistorianReport(in);
        Check(!Has(r.text, "記帳"), "無 ledger 無查帳段");
    }
    {
        LedgerChain empty;
        HistorianInput in;
        in.ledger = &empty;
        HistorianReport r = ComposeHistorianReport(in);
        Check(!Has(r.text, "記帳"), "空鏈無查帳段");
    }

    // ---- [2] 平衡帳→正常句 ----
    printf("\n[2] 試算平衡\n");
    LedgerChain chain;
    chain.Append(LedgerEntry::BattleVictory(120, 1, "斷橋之捷"));
    chain.Append(LedgerEntry::Recruitment(30, 1, "募鄉勇"));
    {
        HistorianInput in;
        in.ledger = &chain;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "本章記帳 2 筆"), "記帳筆數");
        Check(Has(r.text, "借貸相符"), "平衡正常句");
        Check(Has(r.text, "武功增 120"), "武功淨額入句");
        Check(Has(r.text, "軍威增 30"), "軍威淨額入句");
        Check(Has(r.text, "物資損 150"), "物資淨額入句");
        Check(!Has(r.text, "存疑"), "平衡無存疑");
    }

    // ---- [3] 查帳段位於省略計數之前 ----
    printf("\n[3] 段落順序\n");
    {
        HistorianInput in;
        in.ledger = &chain;
        HistorianReport r = ComposeHistorianReport(in);
        const size_t audit = r.text.find("記帳");
        const size_t omit = r.text.find("本報告省略");
        Check(audit != std::string::npos && omit != std::string::npos &&
                  audit < omit,
              "查帳段在審計欄位前");
    }

    // ---- [4] 斷鏈→帳目遭篡存疑句 ----
    printf("\n[4] 斷鏈偵測\n");
    {
        std::string json = chain.ToJson();
        const size_t pos = json.find("斷橋之捷");
        Check(pos != std::string::npos, "找到篡改目標");
        if (pos != std::string::npos) {
            json.replace(pos, 12, "敗走麥城"); // 等長 12-byte 置換
        }
        LedgerChain forged;
        Check(forged.FromJson(json), "篡改帳載入");
        HistorianInput in;
        in.ledger = &forged;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "墨跡未乾"), "斷鏈產存疑句");
        Check(Has(r.text, "帳目遭篡"), "斷鏈用語");
        Check(Has(r.text, "第 1 筆"), "回報斷鏈位置");
        Check(!Has(r.text, "借貸相符"), "斷鏈不產正常句");
    }

    // ---- [5] 偽帳：雜湊合法但借貸不成立 → 借貸不符存疑句 ----
    printf("\n[5] 偽帳偵測\n");
    {
        // 手造一筆 Fate→Fate 同戶分錄，hash 鏈結合法——
        // 模擬 L-4 對手注入的會計不成立偽帳
        LedgerEntry fake;
        fake.debit = LedgerAccount::Fate;
        fake.credit = LedgerAccount::Fate;
        fake.amount = 77;
        fake.memo = "神蹟虛報";
        fake.chapter = 2;
        const uint64_t prev = chain.RootHash();
        const uint64_t h = LedgerChain::HashEntry(prev, fake);
        // 原鏈 JSON 去掉結尾 "]}"，追加偽帳——保留前兩筆合法鏈結
        std::string frag = chain.ToJson();
        frag.resize(frag.size() - 2);
        char tail[256];
        std::snprintf(tail, sizeof(tail),
                      ",{\"d\":\"Fate\",\"c\":\"Fate\",\"amt\":77,\"ch\":2,"
                      "\"memo\":\"神蹟虛報\",\"prev\":\"%016llx\",\"hash\":\"%016llx\"}]}",
                      (unsigned long long)prev, (unsigned long long)h);
        frag += tail;
        LedgerChain poisoned;
        Check(poisoned.FromJson(frag), "偽帳鏈載入");
        Check(poisoned.Size() == 3, "三筆入鏈");
        Check(poisoned.Verify() == -1, "偽帳雜湊鏈合法");
        Check(poisoned.SoundnessViolation() == 2, "偽帳於第三筆");
        HistorianInput in;
        in.ledger = &poisoned;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "借貸不符"), "偽帳產借貸不符句");
        Check(Has(r.text, "墨跡未乾"), "偽帳產墨跡未乾");
        Check(Has(r.text, "第 3 筆"), "回報偽帳位置");
        Check(!Has(r.text, "帳目遭篡"), "偽帳不誣為遭篡");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
