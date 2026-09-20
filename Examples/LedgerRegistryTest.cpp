// LedgerRegistryTest - L-6 帳簿登錄編號無頭測試
// 覆蓋 spec-l6 I/O 矩陣：確定性、內容敏感、roundtrip、舊檔降級、
// 空鏈編號、查帳段字號、竄改 registryId 不強制驗（重算為準）
#include "Gameplay/HistorianReport.h"
#include "Gameplay/Ledger.h"

#include <cctype>
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

// 取出 JSON 中 "registryId" 的值（找不到回空字串）
static std::string ExtractRegistryId(const std::string& json) {
    const size_t p = json.find("\"registryId\":\"");
    if (p == std::string::npos) return "";
    const size_t v = p + 14;
    const size_t e = json.find('"', v);
    return json.substr(v, e - v);
}

// 剝除 registryId 欄位（模擬舊檔）
static std::string StripRegistryId(std::string json) {
    const size_t p = json.find(",\"registryId\":\"");
    if (p == std::string::npos) return json;
    const size_t e = json.find('"', p + 15);
    if (e == std::string::npos) return json;
    return json.erase(p, e - p + 1); // 含前導逗號與尾引號
}

// 格式檢查：XXXX-XXXX（8 位大寫 hex、第 5 字元為 '-'）
static bool IsRegistryFormat(const std::string& id) {
    if (id.size() != 9 || id[4] != '-') {
        return false;
    }
    for (size_t i = 0; i < id.size(); ++i) {
        if (i == 4) continue;
        if (!std::isxdigit(static_cast<unsigned char>(id[i])) ||
            std::islower(static_cast<unsigned char>(id[i]))) {
            return false;
        }
    }
    return true;
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

int main() {
    printf("=== Ledger Registry ID Tests (L-6) ===\n\n");

    // ---- [1] 確定性 + 格式 ----
    printf("[1] 確定性與格式\n");
    LedgerChain chain;
    {
        chain.Append(LedgerEntry::BattleVictory(120, 1, "斷橋之捷"));
        chain.Append(LedgerEntry::Recruitment(30, 1));
        Check(chain.RegistryId() == chain.RegistryId(), "重複呼叫恆相同");
        Check(IsRegistryFormat(chain.RegistryId()),
              "格式為 XXXX-XXXX 大寫 hex");
    }

    // ---- [2] 內容敏感 ----
    printf("\n[2] 內容敏感\n");
    {
        LedgerChain other;
        other.Append(LedgerEntry::BattleVictory(121, 1, "斷橋之捷"));
        other.Append(LedgerEntry::Recruitment(30, 1));
        Check(chain.RegistryId() != other.RegistryId(),
              "金額改一筆 → 編號不同");
        LedgerChain provDiff;
        LedgerEntry e = LedgerEntry::BattleVictory(120, 1, "斷橋之捷");
        e.prov.tick = 7;
        e.prov.eventId = "tampered-prov";
        provDiff.Append(e);
        provDiff.Append(LedgerEntry::Recruitment(30, 1));
        Check(chain.RegistryId() != provDiff.RegistryId(),
              "prov 改動 → 編號不同");
    }

    // ---- [3] roundtrip ----
    printf("\n[3] 持久化 roundtrip\n");
    {
        const std::string json = chain.ToJson();
        const std::string written = ExtractRegistryId(json);
        Check(written == chain.RegistryId(), "寫出值即 RegistryId()");
        LedgerChain back;
        Check(back.FromJson(json), "回讀");
        Check(back.Verify() == -1, "回讀後鏈完整");
        Check(back.RegistryId() == written, "回讀後重算一致");
    }

    // ---- [4] 舊檔降級 ----
    printf("\n[4] 舊檔降級\n");
    {
        const std::string legacy = StripRegistryId(chain.ToJson());
        Check(!Has(legacy, "registryId"), "欄位已剝除");
        LedgerChain t;
        Check(t.FromJson(legacy), "舊檔載入");
        Check(t.Verify() == -1, "舊檔鏈完整");
        Check(t.RegistryId() == chain.RegistryId(),
              "無欄位照常派生同編號");
    }

    // ---- [5] 空鏈 ----
    printf("\n[5] 空鏈編號\n");
    {
        LedgerChain e1, e2;
        Check(IsRegistryFormat(e1.RegistryId()), "空鏈也有合法格式編號");
        Check(e1.RegistryId() == e2.RegistryId(),
              "空鏈編號確定（創世根派生）");
        Check(e1.RegistryId() != chain.RegistryId(),
              "空鏈與有帳鏈編號不同");
    }

    // ---- [6] 竄改 registryId 不強制驗 ----
    printf("\n[6] 竄改 registryId 欄位\n");
    {
        std::string json = chain.ToJson();
        const std::string real = ExtractRegistryId(json);
        // 改寫欄位值為偽造字號
        const size_t p = json.find("\"registryId\":\"");
        json.replace(p + 14, 9, "DEAD-BEEF");
        LedgerChain t;
        Check(t.FromJson(json), "竄改檔仍載入（欄位非防線）");
        Check(t.Verify() == -1, "鏈本身仍合法");
        Check(t.RegistryId() == real && t.RegistryId() != "DEAD-BEEF",
              "編號恆重算、不吃檔案值");
    }

    // ---- [7] 查帳段登錄字號 ----
    printf("\n[7] 查帳段登錄字號\n");
    {
        HistorianInput in;
        in.ledger = &chain;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "登錄字號"), "查帳段含登錄字號");
        Check(Has(r.text, chain.RegistryId().c_str()),
              "字號值即 RegistryId()");
        // 存疑分支（不健全帳簿）也要帶字號
        LedgerChain bad;
        bad.Append(ManualEntry(10, 1, "舊帳"));
        LedgerEntry fake;
        fake.debit = LedgerAccount::Martial;
        fake.credit = LedgerAccount::Martial;
        fake.amount = 50;
        bad.InjectForgery(fake);
        HistorianInput bin;
        bin.ledger = &bad;
        HistorianReport br = ComposeHistorianReport(bin);
        Check(Has(br.text, "登錄字號"), "存疑分支亦帶字號");
        Check(Has(br.text, bad.RegistryId().c_str()),
              "存疑字號值正確");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
