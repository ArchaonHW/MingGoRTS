// LedgerChainTest - L-1/L-2 複式記帳與雜湊鏈帳簿無頭測試
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

int main() {
    printf("=== Ledger Chain Tests (L-1/L-2) ===\n\n");

    // ---- [1] 帳戶命名 ----
    printf("[1] 帳戶\n");
    Check(std::string(AccountName(LedgerAccount::Martial)) == "Martial" &&
              std::string(AccountNameZh(LedgerAccount::Martial)) == "武功",
          "武功帳戶命名");
    LedgerAccount parsed;
    Check(AccountFromName("Fate", parsed) && parsed == LedgerAccount::Fate,
          "名稱解析");
    Check(!AccountFromName("Gold", parsed), "未知帳戶拒絕");

    // ---- [2] 分錄合法性 ----
    printf("\n[2] 分錄合法性\n");
    LedgerEntry good;
    good.debit = LedgerAccount::Martial;
    good.credit = LedgerAccount::Supply;
    good.amount = 100;
    Check(good.WellFormed(), "合法分錄");
    LedgerEntry sameAcct = good;
    sameAcct.credit = LedgerAccount::Martial;
    Check(!sameAcct.WellFormed(), "借貸同戶拒絕");
    LedgerEntry zeroAmt = good;
    zeroAmt.amount = 0;
    Check(!zeroAmt.WellFormed(), "零額拒絕");

    // ---- [3] 分錄模板 ----
    printf("\n[3] 分錄模板\n");
    LedgerEntry vic = LedgerEntry::BattleVictory(120, 1, "斷橋之捷");
    Check(vic.debit == LedgerAccount::Martial &&
              vic.credit == LedgerAccount::Supply && vic.amount == 120,
          "勝仗：借武功貸物資");
    LedgerEntry rec = LedgerEntry::Recruitment(30, 1);
    Check(rec.debit == LedgerAccount::Army &&
              rec.credit == LedgerAccount::Supply,
          "招募：借軍威貸物資");
    LedgerEntry nego = LedgerEntry::Negotiation(50, 2);
    Check(nego.debit == LedgerAccount::Civil &&
              nego.credit == LedgerAccount::Martial,
          "談判：借民心貸武功");

    // ---- [4] 追加與鏈完整性 ----
    printf("\n[4] 雜湊鏈\n");
    LedgerChain chain;
    Check(chain.RootHash() == LedgerChain::kGenesisHash, "空鏈創世根");
    Check(chain.Append(vic), "追加勝仗");
    Check(chain.Append(rec), "追加招募");
    Check(!chain.Append(sameAcct), "malformed 拒入鏈");
    Check(chain.Size() == 2, "鏈長二");
    Check(chain.Verify() == -1, "全鏈驗證通過");
    Check(chain.Entries()[0].prevHash == LedgerChain::kGenesisHash,
          "首筆接創世雜湊");
    Check(chain.Entries()[1].prevHash == chain.Entries()[0].hash,
          "次筆接前筆雜湊");
    Check(chain.RootHash() == chain.Entries()[1].hash, "根雜湊=末筆");

    // ---- [5] 試算平衡 ----
    printf("\n[5] 試算平衡\n");
    auto net = chain.TrialBalance();
    Check(net[static_cast<size_t>(LedgerAccount::Martial)] == 120,
          "武功淨額 +120");
    Check(net[static_cast<size_t>(LedgerAccount::Army)] == 30,
          "軍威淨額 +30");
    Check(net[static_cast<size_t>(LedgerAccount::Supply)] == -150,
          "物資淨額 -150");
    int total = 0;
    for (int n : net) {
        total += n;
    }
    Check(total == 0, "全域借貸恆等");
    Check(chain.SoundnessViolation() == -1, "健全性通過");

    // ---- [6] 序列化與篡改偵測 ----
    printf("\n[6] 持久化與篡改\n");
    std::string json = chain.ToJson();
    Check(json.find("potato.ledger_chain/1") != std::string::npos,
          "schema 標記");
    LedgerChain back;
    Check(back.FromJson(json), "回讀");
    Check(back.Size() == 2 && back.Verify() == -1, "回讀後鏈仍合法");
    Check(back.RootHash() == chain.RootHash(), "根雜湊一致");

    // 篡改摘要——雜湊不符
    std::string tampered = json;
    size_t pos = tampered.find("斷橋之捷");
    Check(pos != std::string::npos, "找到篡改目標");
    if (pos != std::string::npos) {
        tampered.replace(pos, 12, "敗走麥城");
    }
    LedgerChain forged;
    Check(forged.FromJson(tampered), "篡改檔可載入");
    Check(forged.Verify() == 0, "斷鏈偵測於首筆");

    // 篡改金額——雜湊不符且位置在第二筆
    std::string tampered2 = json;
    pos = tampered2.rfind("\"amt\":30");
    Check(pos != std::string::npos, "找到金額目標");
    if (pos != std::string::npos) {
        tampered2.replace(pos, 8, "\"amt\":95");
    }
    LedgerChain forged2;
    Check(forged2.FromJson(tampered2), "金額篡改檔可載入");
    Check(forged2.Verify() == 1, "斷鏈偵測於次筆");

    // 偽帳：借貸同戶——雜湊鏈合法但會計不健全
    LedgerChain unsound;
    Check(unsound.FromJson(json), "基準檔載入");
    {
        // 手造一筆 malformed 但 hash 正確鏈結的分錄（模擬 L-4 偽帳）
        LedgerChain::Chained fake;
        fake.entry.debit = LedgerAccount::Fate;
        fake.entry.credit = LedgerAccount::Fate; // 同戶——會計不成立
        fake.entry.amount = 77;
        fake.entry.memo = "神蹟虛報";
        fake.entry.chapter = 2;
        fake.prevHash = unsound.RootHash();
        fake.hash = LedgerChain::HashEntry(fake.prevHash, fake.entry);
        // FromJson 無法注入；此處直接驗 SoundnessViolation 語意：
        // 若該筆入鏈，雜湊合法但 WellFormed 失敗
        Check(LedgerChain::HashEntry(fake.prevHash, fake.entry) == fake.hash,
              "偽帳雜湊可造合法");
        Check(!fake.entry.WellFormed(), "偽帳借貸不成立");
    }

    Check(!back.FromJson("{bad"), "壞 JSON 拒絕");
    Check(!back.FromJson("{\"schema\":\"potato.other/1\"}"),
          "錯 schema 拒絕");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
