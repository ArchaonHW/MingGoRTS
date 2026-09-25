// TutorialFlowTest —— G-5 教學章節無頭測試：
// tutor 欄位解析/正典序列/亂序拒絕/可跳過/pack 文案/序列驗證。

#include "Campaign/ChapterLibrary.h"
#include "Campaign/TutorialFlow.h"
#include "Gameplay/NarrativePack.h"

#include <cstdio>
#include <filesystem>
#include <string>

using namespace Potato;
using namespace Potato::Campaign;
using namespace Potato::Gameplay;

static int failures = 0;
static int checks = 0;

static void Check(bool cond, const char* name) {
    ++checks;
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", name);
    }
}

static bool Has(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

int main() {
    // ---- 章節定義解析 ----
    {
        ChapterDef def;
        Check(def.LoadFromFile("assets/campaign/tutorial_01.json"),
              "教學章定義載入");
        Check(def.tutor && def.tutorSteps.size() == 4,
              "tutor flag + 四步解析");
        Check(def.tutorSteps[0] == "pick_card" &&
                  def.tutorSteps[3] == "cp_intervene",
              "步序保持檔案順序");
        Check(def.next == "duanqiao", "教學章接續斷橋");
    }
    {
        // tutor:true 布林形 → 正典序列
        ChapterDef def;
        Check(def.LoadFromString(
                  R"({"schema":"potato.campaign_chapter/1","id":"t",
"name":"t","tutor":true})") && def.tutor &&
                  def.tutorSteps.empty(),
              "tutor:true 解析（steps 留空走正典）");
        // 無 tutor 欄 → 一般章節
        ChapterDef plain;
        Check(plain.LoadFromString(
                  R"({"schema":"potato.campaign_chapter/1","id":"p",
"name":"p"})") && !plain.tutor,
              "缺 tutor 欄=一般章節");
        // 壞 steps → 警告
        ChapterDef bad;
        Check(bad.LoadFromString(
                  R"({"schema":"potato.campaign_chapter/1","id":"b",
"name":"b","tutor":{"steps":[123,"ok_step"]}})") &&
                  bad.tutor && bad.tutorSteps.size() == 1 &&
                  !bad.warnings.empty(),
              "壞步記警告略過");
    }

    // ---- 序列驗證 ----
    {
        Check(TutorialFlow::Validate(
                  {"pick_card", "deploy", "watch_trigger",
                   "cp_intervene"})
                  .empty(),
              "正典全序合法");
        Check(TutorialFlow::Validate({"pick_card", "deploy"}).empty(),
              "正典子序列合法");
        Check(!TutorialFlow::Validate({}).empty(), "空序列非法");
        Check(!TutorialFlow::Validate({"bogus_step"}).empty(),
              "未知步非法");
        Check(!TutorialFlow::Validate({"deploy", "pick_card"})
                   .empty(),
              "亂序非法");
        Check(!TutorialFlow::Validate({"deploy", "deploy"}).empty(),
              "重複步非法");
    }

    // ---- 流程狀態機 ----
    {
        ChapterDef def;
        def.LoadFromFile("assets/campaign/tutorial_01.json");
        TutorialFlow flow;
        flow.Begin(def);
        Check(flow.Active() && flow.Current() == "pick_card" &&
                  flow.StepIndex() == 0 && flow.StepCount() == 4,
              "Begin 進入首步");
        Check(!flow.Advance("deploy"), "亂序回報拒絕");
        Check(flow.Current() == "pick_card", "亂序不動狀態");
        Check(flow.Advance("pick_card") &&
                  flow.Current() == "deploy",
              "依序推進");
        Check(flow.Advance("deploy") && flow.Advance("watch_trigger") &&
                  flow.Advance("cp_intervene"),
              "走完四步");
        Check(flow.Done() && flow.Current().empty() &&
                  flow.Prompt(nullptr).empty(),
              "完成後進正常流程");
        Check(!flow.Advance("pick_card"), "完成後回報拒絕");
    }
    {
        // 可跳過：任何步驟中 Skip → Done
        ChapterDef def;
        def.LoadFromFile("assets/campaign/tutorial_01.json");
        TutorialFlow flow;
        flow.Begin(def);
        flow.Advance("pick_card");
        flow.Skip();
        Check(flow.Done(), "中途跳過完成");
    }
    {
        // 非教學章 → 直接 Done
        ChapterDef plain;
        plain.LoadFromString(
            R"({"schema":"potato.campaign_chapter/1","id":"p","name":"p"})");
        TutorialFlow flow;
        flow.Begin(plain);
        Check(flow.Done(), "一般章節不引導");
    }
    {
        // 壞自訂序列 → 不引導（不卡死玩家）
        ChapterDef def;
        def.LoadFromString(
            R"({"schema":"potato.campaign_chapter/1","id":"x",
"name":"x","tutor":{"steps":["deploy","pick_card"]}})");
        TutorialFlow flow;
        flow.Begin(def);
        Check(flow.Done(), "壞序列降級不引導");
    }

    // ---- 文案自 NarrativePack 取 ----
    {
        NarrativePack pack;
        std::string dir = "assets/narrative";
        if (!std::filesystem::is_directory(dir))
            dir = "../../assets/narrative";
        pack.LoadDir(dir);
        ChapterDef def;
        def.LoadFromFile("assets/campaign/tutorial_01.json");
        TutorialFlow flow;
        flow.Begin(def);
        const std::string p0 = flow.Prompt(&pack);
        Check(Has(p0, "doctrine") && Has(p0, "軍令"),
              "pack 文案入引導");
        flow.Advance("pick_card");
        Check(Has(flow.Prompt(&pack), "部署"), "第二步文案");
        // 無 pack → 內建回退不空
        Check(!flow.Prompt(nullptr).empty(), "無 pack 回退");
        // 查無 id 的回退路徑（人工造未知步不可達——用 pack 缺 id 驗）
        const std::string miss =
            pack.Frag("tutorial.step.nonexistent", "回退", {});
        Check(miss == "回退", "缺 id 回退");
    }

    std::printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
