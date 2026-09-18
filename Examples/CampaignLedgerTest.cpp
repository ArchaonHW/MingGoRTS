// CampaignLedgerTest - N-4 敵將處置與聲望持久化無頭測試
#include "Gameplay/CampaignLedger.h"

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
    printf("=== Campaign Ledger Tests (N-4) ===\n\n");

    // ---- [1] 處置記錄 ----
    printf("[1] 處置記錄\n");
    CampaignLedger ledger;
    ledger.RecordDisposition("geluoke", "格洛克", 1,
                             GeneralDisposition::Subdued);
    ledger.RecordDisposition("mayer", "梅爾", 2,
                             GeneralDisposition::Slain);
    Check(ledger.Records().size() == 2, "兩筆處置");
    const auto* g = ledger.Find("geluoke");
    Check(g && g->disposition == GeneralDisposition::Subdued,
          "格洛克收服");
    Check(ledger.Find("nobody") == nullptr, "查無此人");

    // 同 id 覆寫（處置修正）
    ledger.RecordDisposition("geluoke", "格洛克", 3,
                             GeneralDisposition::Defected);
    Check(ledger.Records().size() == 2, "覆寫不增筆");
    Check(ledger.Find("geluoke")->disposition ==
              GeneralDisposition::Defected,
          "處置更新為變節");

    // ---- [2] 稱號軌跡 ----
    printf("\n[2] 稱號軌跡\n");
    ledger.EarnTitle("斷橋軍師", 1);
    ledger.EarnTitle("斷橋之筆", 4);
    Check(ledger.Titles().size() == 2, "兩段稱號");
    Check(ledger.CurrentTitle() == "斷橋之筆", "現行稱號最新");

    // ---- [3] 章節推進 ----
    printf("\n[3] 章節\n");
    ledger.AdvanceChapter(3);
    ledger.AdvanceChapter(2); // 倒退不縮
    Check(ledger.Chapter() == 3, "章節單調前進");

    // ---- [4] 結局引用 ----
    printf("\n[4] 結局證詞\n");
    auto cites = ledger.EndingCitations();
    Check(cites.size() == 4, "處置 2 + 稱號 2 = 4 行");
    Check(cites[0].find("格洛克") != std::string::npos &&
              cites[0].find("Defected") != std::string::npos,
          "證詞含處置");
    Check(cites[2].find("斷橋軍師") != std::string::npos,
          "證詞含稱號");

    // ---- [5] 名稱往返 ----
    printf("\n[5] 名稱解析\n");
    GeneralDisposition d;
    Check(DispositionFromName("Subdued", d) &&
              d == GeneralDisposition::Subdued,
          "Subdued 解析");
    Check(DispositionFromName("Retired", d) &&
              d == GeneralDisposition::Retired,
          "Retired 解析");
    Check(!DispositionFromName("Banished", d), "未知處置拒絕");

    // ---- [6] 序列化跨場 ----
    printf("\n[6] 持久化\n");
    std::string json = ledger.ToJson();
    Check(json.find("potato.campaign_ledger/1") != std::string::npos,
          "schema 標記");
    CampaignLedger ledger2;
    Check(ledger2.FromJson(json), "回讀");
    Check(ledger2.Chapter() == 3, "章節保存");
    Check(ledger2.Records().size() == 2, "處置保存");
    Check(ledger2.CurrentTitle() == "斷橋之筆", "稱號保存");
    Check(ledger2.Find("mayer")->disposition ==
              GeneralDisposition::Slain,
          "處置內容一致");
    Check(!ledger2.FromJson("{bad"), "壞 JSON 拒絕");
    Check(!ledger2.FromJson("{\"schema\":\"potato.other/1\"}"),
          "錯 schema 拒絕");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
