#include "Campaign/CampaignFlow.h"
#include "Campaign/Checkpoint.h"
#include "Campaign/JsonWriter.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/SquadTemplate.h"
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace Potato;
using namespace Potato::Campaign;
using namespace Potato::Gameplay;
static int failed = 0, passed = 0;
static void Check(bool ok, const char *label) {
    std::cout << (ok ? "PASS " : "FAIL ") << label << "\n";
    if (ok)
        ++passed;
    else
        ++failed;
}
static bool Battle(CampaignState &state, const CampaignFlow &flow, std::string &error, bool victory,
                   int loss = 0, bool routedGeneral = false,
                   const std::string &checkpointPath = {}) {
    const auto *chapter = flow.Current(state);
    BattleController b(24, 16, 1);
    Roster roster;
    auto friendly = state.Camp().Deploy(b, 0, chapter->friendlyDeployment);
    for (auto *sq : friendly) {
        const auto *unit = state.Camp().FindUnit(sq->GetName());
        roster.Enroll(sq, unit->captainName, "captain",
                      unit->relics.empty() ? "" : unit->relics.front());
    }
    for (size_t i = 0; i < chapter->enemies.size(); ++i) {
        const auto &e = chapter->enemies[i];
        auto *sq = b.CreateSquad(e.name, 1, e.position, e.members);
        roster.Enroll(sq, i == 0 ? chapter->enemyGeneralName : e.name, "captain",
                      i == 0 ? "將軍印信" : "");
    }
    b.BeginExecution();
    if (loss && !friendly.empty())
        friendly.front()->ApplyCasualties(loss);
    for (const auto &sq : b.GetSquads())
        if (sq->GetTeam() == (victory ? 1 : 0)) {
            if (routedGeneral && sq->GetName() == chapter->enemies.front().name)
                sq->AdjustMorale(-2);
            else
                sq->ApplyCasualties(10000);
        }
    b.Update(.1f);
    roster.Update(b);
    if (!checkpointPath.empty())
        return CommitCampaign(state, flow, checkpointPath, error, [&](auto &s, auto &e) {
            return flow.CompleteBattle(s, b, roster, e);
        });
    return flow.CompleteBattle(state, b, roster, error);
}
static int Strength(const CampaignState &s) {
    int count = 0;
    for (const auto &u : s.Camp().GetUnits())
        count += u.members;
    return count;
}
int main() {
    std::cout.setf(std::ios::unitbuf);
    auto original = std::filesystem::current_path();
    std::filesystem::current_path(POTATO_SOURCE_DIR);
    ChapterLibrary library;
    std::string error;
    Check(library.LoadFromFile("assets/campaign/chapters.json", error),
          "load seven authored chapters");
    if (library.Chapters().size() != 7)
        return 1;
    CampaignFlow flow(library);
    CampaignState s;
    Check(flow.StartNew(s, error), "new campaign");
    Check(s.Camp().GetUnits().size() == 5, "five initial units");
    Check(flow.Validate(s, error), "new progress validates");
    Check(flow.Choose(s, "mercy", error), "mercy choice");
    int strength = Strength(s);
    Check(Battle(s, flow, error, true, 5, true), "chapter one battle settlement");
    Check(Strength(s) == strength - 5 && s.progress.lastReport.TotalLost(0) == 5,
          "real casualties persisted");
    Check(s.Ledger().Find("duanqiao_general")->disposition == GeneralDisposition::Retired,
          "routed living general is not declared slain");
    int balance = s.Camp().GetLoot();
    auto before = WriteJson(s.progress.ToJson());
    Check(!Battle(s, flow, error, true), "settlement rejects duplicate");
    Check(balance == s.Camp().GetLoot() && before == WriteJson(s.progress.ToJson()),
          "duplicate leaves result intact");
    const auto save =
        std::filesystem::temp_directory_path() / "minggo_campaign_flow_acceptance.json";
    Check(s.SaveToFile(save.string()), "save aftermath");
    CampaignState loaded;
    Check(loaded.LoadFromFile(save.string()) && flow.Validate(loaded, error),
          "load aftermath checkpoint");
    Check(Strength(loaded) == Strength(s) && loaded.progress.stage == CampaignStage::Aftermath,
          "loaded army and phase unchanged");
    Check(flow.Advance(loaded, error) && flow.Current(loaded)->id == "ash_ledger",
          "advance second chapter");
    Check(loaded.Camp().GetUnits().front().wounded == 3, "wounded carried to chapter two");
    Check(flow.Choose(loaded, "rescue", error), "rescue choice");
    balance = loaded.Camp().GetLoot();
    Check(!flow.Choose(loaded, "pursue", error) && balance == loaded.Camp().GetLoot(),
          "choice cannot repeatedly pay or grant supply");
    Check(loaded.SaveToFile(save.string()), "overwrite save atomically");
    CampaignState checkpoint;
    Check(checkpoint.LoadFromFile(save.string()) && flow.Validate(checkpoint, error) &&
              flow.SelectedChoice(checkpoint)->id == "rescue",
          "prebattle checkpoint resumes choice");
    Check(Battle(checkpoint, flow, error, true), "rescue chapter victory");
    Check(flow.Advance(checkpoint, error), "advance third chapter");
    Check(flow.CanResolvePeacefully(checkpoint, error), "previous decisions unlock peace");
    Check(flow.Choose(checkpoint, "negotiate", error), "choose negotiation");
    strength = Strength(checkpoint);
    balance = checkpoint.Camp().GetLoot();
    Check(flow.CompletePeace(checkpoint, error), "complete without combat");
    Check(Strength(checkpoint) == strength && balance == checkpoint.Camp().GetLoot() &&
              checkpoint.progress.lastReport.casualties.empty(),
          "peace creates no casualties or loot");
    Check(!flow.CompletePeace(checkpoint, error), "peace settlement exactly once");
    for (int number = 4; number <= 7; ++number) {
        Check(flow.Advance(checkpoint, error), "advance authored chapter");
        const auto *c = flow.Current(checkpoint);
        Check(c->number == number, "chapter number sequence");
        Check(flow.Choose(checkpoint, c->choices.back().id, error), "later chapter decision");
        Check(Battle(checkpoint, flow, error, true), "later chapter battle");
    }
    Check(flow.Advance(checkpoint, error) && checkpoint.progress.stage == CampaignStage::Complete,
          "terminal seven chapter state");
    Check(flow.Validate(checkpoint, error) && !flow.Advance(checkpoint, error),
          "terminal state cannot advance eighth chapter");
    CampaignState combat;
    flow.StartNew(combat, error);
    flow.Choose(combat, "pursue", error);
    Check(Battle(combat, flow, error, true) && flow.Advance(combat, error),
          "pursuit first chapter");
    flow.Choose(combat, "pursue", error);
    Check(Battle(combat, flow, error, true) && flow.Advance(combat, error),
          "pursuit second chapter");
    Check(!flow.CanResolvePeacefully(combat, error) && !flow.Choose(combat, "negotiate", error),
          "wrong decisions lock peaceful route");
    Check(flow.Choose(combat, "attack", error) && Battle(combat, flow, error, true),
          "third chapter combat route");
    for (int number = 4; number <= 7; ++number) {
        Check(flow.Advance(combat, error), "combat route advances later chapter");
        Check(flow.Choose(combat, flow.Current(combat)->choices.back().id, error) &&
                  Battle(combat, flow, error, true),
              "combat route completes later battle");
    }
    Check(flow.Advance(combat, error) && flow.Validate(combat, error) &&
              combat.progress.stage == CampaignStage::Complete,
          "combat route completes all seven");
    CampaignState defeated;
    flow.StartNew(defeated, error);
    flow.Choose(defeated, "mercy", error);
    Check(Battle(defeated, flow, error, false), "defeat settlement");
    auto history = defeated.NamedRoster().DeadCount();
    Check(history == 5 && Strength(defeated) == 0 && defeated.Camp().Inventory().empty(),
          "defeat preserves dead captains and grants no relics");
    Check(flow.Advance(defeated, error) && Strength(defeated) == 24 &&
              defeated.NamedRoster().DeadCount() == history,
          "new reserves do not revive veterans");
    Check(s.SaveToFile(save.string()), "restore known save");
    std::ifstream input(save);
    std::string data((std::istreambuf_iterator<char>(input)), {});
    input.close();
    JsonValue root;
    JsonValue::ParseOk(data, root);
    root.objectValue["refit_camp"].objectValue["loot"] = JsonValue::Number(-1);
    {
        std::ofstream f(save);
        f << WriteJson(root);
    }
    before = WriteJson(loaded.progress.ToJson());
    balance = loaded.Camp().GetLoot();
    Check(!loaded.LoadFromFile(save.string()) && balance == loaded.Camp().GetLoot() &&
              before == WriteJson(loaded.progress.ToJson()),
          "negative balance corrupt save rejected without mutation");
    root.objectValue["refit_camp"].objectValue["loot"] = JsonValue::Number(10);
    root.objectValue["chapter"].objectValue["chapter"] = JsonValue::Number(1.5);
    {
        std::ofstream f(save);
        f << WriteJson(root);
    }
    Check(!loaded.LoadFromFile(save.string()), "fractional chapter rejected");
    auto invalid = loaded;
    invalid.progress.choices["unknown"] = "bad";
    Check(!flow.Validate(invalid, error), "unknown chapter choice rejected");
    Check(s.SaveToFile(save.string()), "write atomic reference");
    auto blocker = save;
    blocker += ".tmp";
    std::filesystem::create_directory(blocker);
    Check(!s.SaveToFile(save.string()), "save write failure reported");
    CampaignState intact;
    Check(intact.LoadFromFile(save.string()) && flow.Validate(intact, error),
          "failed save preserves previous file");
    std::filesystem::remove(blocker);
    CampaignState transaction;
    flow.StartNew(transaction, error);
    flow.Choose(transaction, "mercy", error);
    const auto originalProgress = WriteJson(transaction.progress.ToJson());
    const int originalLoot = transaction.Camp().GetLoot();
    std::filesystem::create_directory(blocker);
    Check(!Battle(transaction, flow, error, true, 5, false, save.string()) &&
              WriteJson(transaction.progress.ToJson()) == originalProgress &&
              transaction.Camp().GetLoot() == originalLoot && Strength(transaction) == 98,
          "failed settlement write rolls back army loot and progression");
    std::filesystem::remove(blocker);
    Check(Battle(transaction, flow, error, true, 5, false, save.string()) &&
              Strength(transaction) == 93 && transaction.progress.cumulativeDead == 2,
          "settlement retry commits one real casualty report");
    balance = transaction.Camp().GetLoot();
    Check(!Battle(transaction, flow, error, true, 5, false, save.string()) &&
              balance == transaction.Camp().GetLoot() && Strength(transaction) == 93,
          "saved settlement retry cannot double pay or injure");
    auto carry = s;
    flow.Advance(carry, error);
    flow.Choose(carry, "pursue", error);
    Check(Battle(carry, flow, error, true, 25), "eliminate previously wounded veteran");
    const auto *medical = carry.Camp().FindUnit("傷員接收 · 前鋒");
    Check(medical && medical->wounded == 3 && medical->members == 0 &&
              !carry.Camp().FindUnit("前鋒"),
          "off-field wounded preserved under a new receiving officer");
    bool originalDead = false;
    for (const auto &e : carry.NamedRoster().GetEntries())
        if (e.name == "周鐵槍")
            originalDead = !e.alive;
    carry.Camp().HealWounded(3);
    Check(originalDead && carry.Camp().FindUnit("傷員接收 · 前鋒")->members == 3,
          "medical recovery never revives dead captain");
    auto full = s;
    auto fullCamp = full.Camp().ToJson();
    fullCamp.objectValue["units"].arrayValue.clear();
    for (int i = 0; i < 12; ++i) {
        auto u = s.Camp().ToJson()["units"][0];
        u.objectValue["squad_name"] = JsonValue::String("depleted" + std::to_string(i));
        u.objectValue["captain"] = JsonValue::String("officer" + std::to_string(i));
        u.objectValue["members"] = JsonValue::Number(1);
        u.objectValue["wounded"] = JsonValue::Number(0);
        fullCamp.objectValue["units"].arrayValue.push_back(u);
    }
    Check(full.Camp().FromJson(fullCamp) && flow.Advance(full, error) && flow.Validate(full, error),
          "full depleted camp still advances with distinct reserves");
    auto unknown = s;
    auto badCamp = unknown.Camp().ToJson();
    badCamp.objectValue["units"].arrayValue[0].objectValue["template_id"] =
        JsonValue::String("missing-template");
    unknown.Camp().FromJson(badCamp);
    Check(!flow.Validate(unknown, error), "unknown template id rejected before deployment");
    Roster pointerless;
    Check(pointerless.FromJson(s.NamedRoster().ToJson()), "load history roster");
    BattleController empty(24, 16, 1);
    pointerless.Update(empty);
    Check(pointerless.GetEntries().size() == s.NamedRoster().GetEntries().size(),
          "loaded roster update safely ignores missing pointers");
    RefitCamp zero;
    VeteranUnit unit;
    unit.squadName = "傷兵隊";
    unit.maxMembers = 5;
    unit.wounded = 5;
    zero.EnrollUnit(unit);
    Check(zero.Deploy(empty, 0, {}).empty(), "zero strength not deployed");
    RefitCamp cavalryCamp;
    VeteranUnit cavalry;
    cavalry.squadName = "傷騎";
    cavalry.captainName = "騎官";
    cavalry.unitClass = UnitClass::Cavalry;
    cavalry.templateId = "cavalry-template";
    cavalry.members = 6;
    cavalry.wounded = 3;
    cavalry.maxMembers = 9;
    cavalry.relics = {"舊旗"};
    cavalryCamp.EnrollUnit(cavalry);
    PostBattleReport killed;
    killed.casualties.push_back({"傷騎", 0, 6, 0, 6, true});
    cavalryCamp.Absorb(killed, pointerless, 0);
    const auto *receiving = cavalryCamp.FindUnit("傷員接收 · 傷騎");
    Check(receiving && receiving->unitClass == UnitClass::Cavalry &&
              receiving->templateId == "cavalry-template" && receiving->relics == cavalry.relics,
          "medical transfer preserves class template and memorial relic custody");
    std::filesystem::remove(save);
    std::filesystem::current_path(original);
    std::cout << passed << " passed / " << failed << " failed\n";
    return failed ? 1 : 0;
}
