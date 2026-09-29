#include "Gameplay/Squad/Squad.h"
#include "Gameplay/Json/JsonValue.h"

#include <cstdio>
#include <fstream>
#include <string>

namespace {

int failures = 0;
void Check(bool cond, const char* name) {
    if (cond) {
        std::printf("[PASS] %s\n", name);
    } else {
        std::printf("[FAIL] %s\n", name);
        ++failures;
    }
}

using namespace Potato::Gameplay;

const char* SQUAD_DOC = R"json({
    "schema": "potato.squad/1",
    "squads": [
        {"id": "sq_vanguard", "name": "鋒矢營", "unit": "infantry",
         "hp": 100, "attack": 10, "speed": "medium", "cost": 40},
        {"id": "sq_riders", "unit": "cavalry",
         "hp": 80, "attack": 14, "speed": "fast", "cost": 60},
        {"id": "sq_watch", "unit": "scouts",
         "hp": 40, "attack": 4, "speed": "very_fast", "cohesion": 60, "cost": 30}
    ]
})json";

} // namespace

int main() {
    // --- Template library + instantiate ---
    auto doc = JsonValue::Parse(SQUAD_DOC);
    Check(doc.ok(), "fixture parses");
    auto lib = SquadTemplateLibrary::FromJson(doc.value);
    Check(lib.ok() && lib.value.Count() == 3, "potato.squad/1 library loads");

    const SquadTemplate* vg = lib.value.Find("sq_vanguard");
    Check(vg != nullptr && vg->name == "鋒矢營" && vg->unit == UnitType::Infantry &&
          vg->hp == 100 && vg->attack == 10 && vg->speed == Speed::Medium &&
          vg->cost == 40 && vg->cohesion == 100, "template fields decoded");
    Check(lib.value.Find("nope") == nullptr, "Find miss -> nullptr");

    Squad sq = Squad::Instantiate(*vg, 3);
    Check(sq.id == "sq_vanguard" && sq.name == "鋒矢營" && sq.hp == 100 &&
          sq.maxHp == 100 && sq.cohesion == 100 && sq.regionIndex == 3 &&
          sq.state == SquadState::Holding, "instantiate -> Holding at spawn");

    // --- Speed math (constexpr table) ---
    Check(TicksForEdge(SpeedMilli(Speed::Slow)) == 66, "slow = 66 ticks/edge");
    Check(TicksForEdge(SpeedMilli(Speed::Medium)) == 40, "medium = 40 ticks/edge");
    Check(TicksForEdge(SpeedMilli(Speed::Fast)) == 26, "fast = 26 ticks/edge");
    Check(TicksForEdge(SpeedMilli(Speed::VeryFast)) == 20, "very_fast = 20 ticks/edge");

    // --- Transition table spot-checks ---
    Check(CanTransition(SquadState::Holding, SquadEvent::MoveOrder), "table: Holding->Moving");
    Check(!CanTransition(SquadState::Moving, SquadEvent::MoveOrder), "table: no re-order mid-move");
    Check(CanTransition(SquadState::Moving, SquadEvent::Arrived), "table: Moving->Holding");
    Check(CanTransition(SquadState::Holding, SquadEvent::CohesionBreak) &&
          CanTransition(SquadState::Moving, SquadEvent::CohesionBreak), "table: rout from Holding|Moving");
    Check(!CanTransition(SquadState::Routing, SquadEvent::CohesionBreak), "table: no double-rout");
    Check(CanTransition(SquadState::Routing, SquadEvent::RetreatComplete), "table: Routing->Routed");
    Check(CanTransition(SquadState::Routing, SquadEvent::HpZero), "table: killed while routing");
    Check(!CanTransition(SquadState::Routed, SquadEvent::MoveOrder) &&
          !CanTransition(SquadState::Destroyed, SquadEvent::HpZero), "table: terminal states");

    // --- Movement timing (medium = 40 ticks/edge) ---
    Check(sq.IssueMove(4), "IssueMove from Holding");
    Check(!sq.IssueMove(5), "IssueMove rejected while Moving");
    for (int i = 0; i < 39; ++i) sq.TickMove();
    Check(sq.state == SquadState::Moving && sq.regionIndex == 3 &&
          sq.edgeProgress == 39, "still moving at tick 39");
    sq.TickMove();
    Check(sq.state == SquadState::Holding && sq.regionIndex == 4 &&
          sq.edgeTarget == ~std::size_t{0}, "arrived at tick 40");
    sq.TickMove();
    Check(sq.state == SquadState::Holding && sq.regionIndex == 4,
          "TickMove no-op while Holding");

    // --- Cohesion: rout boundary ---
    {
        Squad s2 = Squad::Instantiate(lib.value.At(1), 0);
        s2.ApplyHit(0, 80); // 100-80 = 20, NOT < 20
        Check(s2.cohesion == 20 && s2.state == SquadState::Holding,
              "cohesion == 20 holds (threshold is < 20)");
        s2.ApplyHit(0, 1); // 19 < 20
        Check(s2.cohesion == 19 && s2.state == SquadState::Routing,
              "cohesion 19 -> Routing");
    }

    // --- Rout cancels an in-flight move ---
    {
        Squad s3 = Squad::Instantiate(lib.value.At(1), 0);
        Check(s3.IssueMove(1), "move issued");
        for (int i = 0; i < 10; ++i) s3.TickMove();
        s3.ApplyHit(0, 90);
        Check(s3.state == SquadState::Routing && s3.regionIndex == 0 &&
              s3.edgeTarget == ~std::size_t{0} && s3.edgeProgress == 0,
              "rout cancels move, squad stays in region");
        s3.TickMove();
        Check(s3.state == SquadState::Routing, "routing squad does not auto-move");
    }

    // --- Destroyed precedence over rout ---
    {
        Squad s4 = Squad::Instantiate(lib.value.At(2), 0);
        s4.ApplyHit(40, 90); // hp 0 AND cohesion < 20
        Check(s4.hp == 0 && s4.state == SquadState::Destroyed,
              "HpZero beats CohesionBreak -> Destroyed");
        Check(!s4.IsEffective(), "Destroyed is not effective");
    }

    // --- Clamps + heal/restore ---
    {
        Squad s5 = Squad::Instantiate(*vg, 0);
        s5.ApplyHit(150, 0);
        Check(s5.hp == 0 && s5.state == SquadState::Destroyed, "hp clamps at 0");
        Squad s6 = Squad::Instantiate(*vg, 0);
        s6.ApplyHit(30, 30);
        s6.Heal(10);
        s6.RestoreCohesion(500);
        Check(s6.hp == 80 && s6.cohesion == 100, "heal/restore clamped");
    }

    // --- Validation rejects ---
    auto reject = [](const char* json, const char* name) {
        auto d = JsonValue::Parse(json);
        if (!d.ok()) { Check(false, name); return; }
        auto m = SquadTemplateLibrary::FromJson(d.value);
        Check(!m.ok() && !m.error.empty() && !m.reason.empty(), name);
    };
    reject("[]", "non-object root rejected");
    reject(R"({"squads":[]})", "empty squads rejected");
    reject(R"({"squads":[{"id":"a","unit":"infantry","hp":10,"speed":"medium"},
                          {"id":"a","unit":"cavalry","hp":10,"speed":"fast"}]})",
           "duplicate squad id rejected");
    reject(R"({"squads":[{"id":"a","unit":"dragon","hp":10,"speed":"fast"}]})",
           "unknown unit token rejected");
    reject(R"({"squads":[{"id":"a","unit":"infantry","hp":10,"speed":"warp"}]})",
           "unknown speed token rejected");
    reject(R"({"squads":[{"id":"a","unit":"infantry","hp":0,"speed":"fast"}]})",
           "hp < 1 rejected");
    reject(R"({"squads":[{"id":"a","unit":"infantry","speed":"fast"}]})",
           "missing hp rejected");
    reject(R"({"squads":[{"id":"a","unit":"infantry","hp":10,"speed":"fast","cohesion":150}]})",
           "cohesion > 100 rejected");
    reject(R"({"schema":"potato.squad/2","squads":[{"id":"a","unit":"infantry","hp":10,"speed":"fast"}]})",
           "schema tag re-checked in FromJson");

    // --- File path via Load ---
    {
        const char* tmp = "potato_test_squad.fixture.tmp.json";
        {
            std::ofstream out(tmp, std::ios::binary);
            Check(out.good(), "fixture file opens");
            out << SQUAD_DOC;
        }
        auto loaded = SquadTemplateLibrary::Load(tmp);
        Check(loaded.ok() && loaded.value.Count() == 3, "Load(path) with schema gate");
        std::remove(tmp);
    }

    if (failures == 0) {
        std::puts("SQUAD TESTS PASS");
        return 0;
    }
    std::printf("%d CHECK(S) FAILED\n", failures);
    return 1;
}
