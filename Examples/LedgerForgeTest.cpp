// LedgerForgeTest - L-4 偽帳注入與疑帳標記無頭測試
// 對手軍師攻擊面：hash 鏈合法但會計不成立的分錄可注入、
// 可被 SoundnessViolation 揭露、可被玩家標記並持久
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

// 對手風格偽帳：「彼軍自潰，不戰而勝」——武功自貸武功，
// 雜湊可算合法但借貸同戶，會計不成立
static LedgerEntry RivalForgery(int amount, int chapter,
                                const std::string& memo) {
    LedgerEntry e;
    e.debit = LedgerAccount::Martial;
    e.credit = LedgerAccount::Martial;
    e.amount = amount;
    e.memo = memo;
    e.chapter = chapter;
    return e;
}

int main() {
    printf("=== Ledger Forge Tests (L-4) ===\n\n");

    LedgerChain chain;
    chain.Append(LedgerEntry::BattleVictory(120, 1, "斷橋之捷"));
    chain.Append(LedgerEntry::Recruitment(30, 1, "募鄉勇"));

    // ---- [1] Append 拒收偽帳，InjectForgery 專用通道 ----
    printf("[1] 注入通道\n");
    const LedgerEntry fake = RivalForgery(50, 1, "彼軍自潰，不戰而勝");
    Check(!chain.Append(fake), "Append 拒收 malformed");
    Check(chain.Size() == 2, "鏈未被污染");
    const int idx = chain.InjectForgery(fake);
    Check(idx == 2, "InjectForgery 回索引 2");
    Check(chain.Size() == 3, "偽帳入鏈");

    // ---- [2] 雜湊合法、會計不成立 ----
    printf("\n[2] 偵測面\n");
    Check(chain.Verify() == -1, "雜湊鏈仍合法");
    Check(chain.SoundnessViolation() == 2, "健全性揭露於第三筆");
    // 注入後 TrialBalance 全域仍守恆（同戶自貸自借抵銷）——
    // 偽帳的會計異常靠 WellFormed 揭露，不靠淨額
    int total = 0;
    for (int n : chain.TrialBalance()) {
        total += n;
    }
    Check(total == 0, "試算全域仍守恆");

    // ---- [3] 戰報整合：偽帳鏈產借貸不符存疑句 ----
    printf("\n[3] 戰報整合\n");
    {
        HistorianInput in;
        in.ledger = &chain;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "借貸不符"), "戰報產借貸不符");
        Check(Has(r.text, "第 3 筆"), "回報偽帳位置");
    }

    // ---- [4] 疑帳標記 ----
    printf("\n[4] 疑帳標記\n");
    Check(chain.MarkSuspect(2), "標記偽帳索引");
    Check(chain.IsSuspect(2), "IsSuspect 命中");
    Check(!chain.IsSuspect(0), "合法筆未標記");
    Check(!chain.MarkSuspect(2), "重複標記拒絕");
    Check(!chain.MarkSuspect(99), "越界索引拒絕");
    Check(chain.SuspectCount() == 1, "疑帳計數 1");
    Check(chain.Verify() == -1, "標記不動鏈雜湊");

    // ---- [5] 持久化：suspect 陣列隨存檔走 ----
    printf("\n[5] 持久化\n");
    {
        const std::string json = chain.ToJson();
        Check(Has(json, "\"suspect\":[2]"), "suspect 陣列入檔");
        LedgerChain back;
        Check(back.FromJson(json), "回讀");
        Check(back.IsSuspect(2), "疑帳標記保存");
        Check(back.Size() == 3, "鏈長保存");
        Check(back.SoundnessViolation() == 2, "偽帳仍可揭露");
    }
    {
        // 無 suspect 的檔案回讀 → 空標記（向後相容）
        LedgerChain clean;
        clean.Append(LedgerEntry::BattleVictory(10, 1));
        LedgerChain back;
        Check(back.FromJson(clean.ToJson()), "無標記檔回讀");
        Check(back.SuspectCount() == 0, "預設無疑帳");
        Check(clean.ToJson().find("suspect") == std::string::npos,
              "空標記不寫欄位");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
