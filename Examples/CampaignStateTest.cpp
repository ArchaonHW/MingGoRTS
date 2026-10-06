// CampaignStateTest - E-7 戰役框架無頭測試
// 驗收：全欄位存讀 roundtrip、壞檔/異版拒絕不丟現況、
//       tmp+rename 原子寫、預留段佔位、章節推進同步
#include "Campaign/CampaignState.h"
#include "Gameplay/Squad.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

using namespace Potato::Campaign;
using namespace Potato::Gameplay;
using Potato::Vector2;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

static std::string ReadAll(const std::string& path) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static bool FileExists(const std::string& path) {
    std::ifstream f(path);
    return f.good();
}

int main() {
    const std::string kPath = "campaign_state_test.json";
    printf("=== Campaign State Tests (E-7) ===\n\n");

    // [1] 章節推進同步帳本
    printf("[1] 章節推進\n");
    CampaignState cs;
    cs.AdvanceChapter(1, 3, "duanqiao");
    Check(cs.chapter.arc == 1 && cs.chapter.chapter == 3,
          "AdvanceChapter 寫入 ChapterState");
    Check(cs.chapter.chapterId == "duanqiao", "章節 id 記錄");
    Check(cs.Ledger().Chapter() == 3, "帳本章節序同步");
    cs.chapter.frontsTaken.push_back("bridgehead");
    cs.chapter.frontsTaken.push_back("ford");

    // [2] 填充各子系統
    printf("\n[2] 子系統填充\n");
    cs.Camp().DepositLoot(12);
    VeteranUnit vu;
    vu.squadName = "前鋒";
    vu.templateId = "infantry_company";
    vu.unitClass = UnitClass::Infantry;
    vu.members = 22;
    vu.wounded = 5;
    vu.maxMembers = 30;
    vu.captainName = "老周";
    vu.relics.push_back("斷刃");
    cs.Camp().EnrollUnit(vu);
    Check(cs.Camp().GetLoot() == 12 && cs.Camp().GetUnits().size() == 1,
          "RefitCamp 填充");

    Squad sq("前鋒", 0, Vector2(2, 2), 22);
    cs.NamedRoster().Enroll(&sq, "老周", "captain", "斷刃");
    Check(cs.NamedRoster().GetEntries().size() == 1, "Roster 填充");

    cs.Ledger().RecordDisposition("gen_shi", "石守信", 3,
                                  GeneralDisposition::Subdued);
    cs.Ledger().RecordDisposition("gen_zhang", "張員外", 3,
                                  GeneralDisposition::Slain);
    cs.Ledger().EarnTitle("斷橋軍師", 3);
    Check(cs.Ledger().Records().size() == 2, "處置記帳");
    Check(cs.Ledger().CurrentTitle() == "斷橋軍師", "稱號記帳");

    // [3] 存檔 + 欄位檢視
    printf("\n[3] 存檔\n");
    Check(cs.SaveToFile(kPath), "SaveToFile 成功");
    Check(!FileExists(kPath + ".tmp"), "tmp 已 rename 不殘留");
    std::string doc = ReadAll(kPath);
    Check(doc.find("\"potato.campaign/1\"") != std::string::npos,
          "schema 寫入");
    Check(doc.find("\"governance\"") != std::string::npos &&
          doc.find("\"god_stance\"") != std::string::npos &&
          doc.find("\"intel_ledger\"") != std::string::npos,
          "E-5/E-6/E-11 預留段佔位");

    // [4] 全欄位 roundtrip
    printf("\n[4] 讀檔 roundtrip\n");
    CampaignState back;
    Check(back.LoadFromFile(kPath), "LoadFromFile 成功");
    Check(back.chapter.arc == 1 && back.chapter.chapter == 3 &&
          back.chapter.chapterId == "duanqiao", "章節還原");
    Check(back.chapter.frontsTaken.size() == 2 &&
          back.chapter.frontsTaken[0] == "bridgehead", "fronts 還原");
    Check(back.Camp().GetLoot() == 12, "營地戰利品還原");
    const auto& units = back.Camp().GetUnits();
    Check(units.size() == 1 && units[0].members == 22 &&
          units[0].wounded == 5 && units[0].maxMembers == 30 &&
          units[0].captainName == "老周" &&
          units[0].relics.size() == 1 && units[0].relics[0] == "斷刃",
          "老兵單位全欄位還原");
    const auto& entries = back.NamedRoster().GetEntries();
    Check(entries.size() == 1 && entries[0].name == "老周" &&
          entries[0].rank == "captain" && entries[0].alive &&
          entries[0].relic == "斷刃", "名冊還原");
    const auto* rec = back.Ledger().Find("gen_shi");
    Check(rec != nullptr &&
          rec->disposition == GeneralDisposition::Subdued &&
          rec->chapter == 3 && rec->generalName == "石守信",
          "處置記錄還原");
    Check(back.Ledger().CurrentTitle() == "斷橋軍師", "稱號還原");
    Check(back.Ledger().Chapter() == 3, "帳本章節序還原");

    // [5] 壞檔/異版拒絕——且現況不丟
    printf("\n[5] 壞檔拒絕\n");
    {
        std::ofstream f("campaign_bad_schema.json");
        f << "{\"schema\":\"potato.campaign/99\"}";
    }
    CampaignState probe;
    probe.AdvanceChapter(2, 1, "probe");
    Check(!probe.LoadFromFile("campaign_bad_schema.json"),
          "異版 schema 拒絕");
    Check(probe.chapter.chapterId == "probe", "拒絕後現況保留");

    {
        std::ofstream f("campaign_corrupt.json");
        f << "{not valid json [[[";
    }
    Check(!probe.LoadFromFile("campaign_corrupt.json"),
          "損毀 JSON 拒絕");
    Check(probe.chapter.chapterId == "probe", "損毀拒絕後現況保留");

    Check(!probe.LoadFromFile("campaign_nonexistent.json"),
          "不存在檔案拒絕");

    // 子文件損毀：schema 對但 refit_camp 壞
    {
        std::ofstream f("campaign_bad_section.json");
        f << "{\"schema\":\"potato.campaign/1\","
             "\"refit_camp\":{\"schema\":\"wrong/1\"},"
             "\"roster\":{\"entries\":[]}}";
    }
    Check(!probe.LoadFromFile("campaign_bad_section.json"),
          "子文件損毀拒絕");
    Check(probe.chapter.chapterId == "probe", "子文件拒絕後現況保留");

    // [6] 原子寫：殘留 tmp 不影響後續讀檔
    printf("\n[6] tmp 殘留容忍\n");
    {
        std::ofstream f(kPath + ".tmp");
        f << "{\"garbage\":true}";
    }
    CampaignState again;
    Check(again.LoadFromFile(kPath), "tmp 殘留下主檔仍可讀");
    Check(again.Camp().GetLoot() == 12, "殘留 tmp 不污染載入");

    // 清理
    std::remove(kPath.c_str());
    std::remove((kPath + ".tmp").c_str());
    std::remove("campaign_bad_schema.json");
    std::remove("campaign_corrupt.json");
    std::remove("campaign_bad_section.json");

    printf("\n=== %d PASS / %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
