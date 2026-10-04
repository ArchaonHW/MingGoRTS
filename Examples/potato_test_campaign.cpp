// Story 3.1 — CampaignState facade: potato.campaign/1 round-trip.
#include "Campaign/State/CampaignState.h"
#include "Gameplay/Json/JsonValue.h"

#include <cstdio>

using namespace Potato;
using Potato::Gameplay::JsonValue;
using Potato::Campaign::CampaignState;
using Potato::Campaign::Ledger;
using Potato::Campaign::Posting;
using Potato::Campaign::Leg;
using Potato::Campaign::Account;
using Potato::Campaign::RosterEntry;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        std::printf("[PASS] %s\n", name);
    } else {
        std::printf("[FAIL] %s\n", name);
        ++failures;
    }
}

static Posting Burn() {
    Posting p;
    p.credit = {Account::Materiel, 40};
    p.debit = {Account::PopularSupport, 15};
    p.memo = "burned village";
    return p;
}

static CampaignState SampleState() {
    CampaignState st;
    st.GetChapter().current = 2;
    st.GetChapter().unlocked = {true, true, true, false};
    st.GetChapter().resolved = {true, true, false, false};
    RosterEntry a{"右翼第三隊", 3, 40, false};
    RosterEntry b{"左翼第一隊", 1, 90, true};
    st.GetRoster() = {a, b};
    st.GetLedger().Post(Burn());
    Posting f = Burn();
    f.memo = "forged report";
    const auto r = st.GetLedger().Forge(f);
    st.GetLedger().SetSuspect(r.value);
    return st;
}

int main() {
    // --- round-trip ---
    {
        const CampaignState st = SampleState();
        auto jr = st.ToJson();
        Check(jr.ok(), "ToJson succeeds");
        const JsonValue doc = jr.value;
        auto back = CampaignState::FromJson(doc);
        Check(back.ok(), "FromJson accepts own emit");
        if (back.ok()) {
            const CampaignState& s2 = back.value;
            Check(s2.GetChapter().current == 2 &&
                      s2.GetChapter().unlocked.size() == 4 &&
                      s2.GetChapter().unlocked[2] &&
                      !s2.GetChapter().resolved[2],
                  "chapter progress round-trips");
            Check(s2.GetRoster().size() == 2 &&
                      s2.GetRoster()[0].name == "右翼第三隊" &&
                      s2.GetRoster()[0].veterancy == 3 &&
                      s2.GetRoster()[0].casualties == 40 &&
                      !s2.GetRoster()[0].dead &&
                      s2.GetRoster()[1].dead,
                  "roster round-trips (incl. CJK name)");
            Check(s2.GetLedger().Size() == 2 &&
                      s2.GetLedger().Verify() == nullptr &&
                      s2.GetLedger().SuspectEntries().size() == 1,
                  "ledger embeds intact (chain + suspect)");
            Check(s2.GetLedger().Balance(Account::Materiel) == 80 &&
                      s2.GetLedger().Balance(Account::PopularSupport) ==
                          -30,
                  "balances round-trip through embed");
        }
    }

    // --- rejections (no partial state escapes) ---
    {
        const CampaignState st = SampleState();
        const JsonValue good = st.ToJson().value;

        auto badSchema = good.Members();
        badSchema["schema"] = JsonValue::String("potato.campaign/2");
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badSchema)))
                   .ok(),
              "bad campaign schema rejected");

        auto badChapter = good.Members();
        badChapter["chapter"] = JsonValue::Int(5);
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badChapter)))
                   .ok(),
              "non-object chapter rejected");

        auto badCur = good.Members();
        JsonValue::Object ch = badCur["chapter"].Members();
        ch["current"] = JsonValue::Int(-1);
        badCur["chapter"] = JsonValue::MakeObject(std::move(ch));
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badCur)))
                   .ok(),
              "negative chapter.current rejected");

        auto badRoster = good.Members();
        badRoster["roster"] = JsonValue::String("nope");
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badRoster)))
                   .ok(),
              "non-array roster rejected");

        // Broken embedded ledger -> whole doc rejected.
        auto badLedger = good.Members();
        JsonValue::Object led = badLedger["ledger"].Members();
        JsonValue::Array ents = led["entries"].Items();
        JsonValue::Object e0 = ents[0].Members();
        e0["memo"] = JsonValue::String("tampered");
        ents[0] = JsonValue::MakeObject(std::move(e0));
        led["entries"] = JsonValue::MakeArray(std::move(ents));
        badLedger["ledger"] = JsonValue::MakeObject(std::move(led));
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badLedger)))
                   .ok(),
              "tampered embedded ledger rejected");

        // Roster counter bounds.
        auto badVet = good.Members();
        JsonValue::Array ro = badVet["roster"].Items();
        JsonValue::Object r0 = ro[0].Members();
        r0["veterancy"] = JsonValue::Int(-1);
        ro[0] = JsonValue::MakeObject(std::move(r0));
        badVet["roster"] = JsonValue::MakeArray(std::move(ro));
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badVet)))
                   .ok(),
              "negative veterancy rejected");

        // Duplicate roster names rejected.
        auto dup = good.Members();
        ro = dup["roster"].Items();
        JsonValue::Object r1 = ro[1].Members();
        r1["name"] = ro[0]["name"];
        ro[1] = JsonValue::MakeObject(std::move(r1));
        dup["roster"] = JsonValue::MakeArray(std::move(ro));
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(dup)))
                   .ok(),
              "duplicate roster name rejected");

        // Parallel chapter vectors must agree.
        auto badCh = good.Members();
        ch = badCh["chapter"].Members();
        ch["resolved"] = JsonValue::MakeArray({JsonValue::Bool(true)});
        badCh["chapter"] = JsonValue::MakeObject(std::move(ch));
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badCh)))
                   .ok(),
              "mismatched chapter vectors rejected");

        // current outside the chapter space.
        auto badIdx = good.Members();
        ch = badIdx["chapter"].Members();
        ch["current"] = JsonValue::Int(9); // > unlocked.size()==4
        badIdx["chapter"] = JsonValue::MakeObject(std::move(ch));
        Check(!CampaignState::FromJson(
                  JsonValue::MakeObject(std::move(badIdx)))
                   .ok(),
              "current beyond chapter space rejected");
    }

    // --- edges ---
    {
        CampaignState empty; // all defaults
        auto jr = empty.ToJson();
        auto back = CampaignState::FromJson(jr.value);
        Check(back.ok() && back.value.GetRoster().empty() &&
                  back.value.GetChapter().current == 0 &&
                  back.value.GetLedger().Size() == 0,
              "empty campaign round-trips");

        // Deterministic emit: serialize twice, byte-identical.
        Check(jr.value.Emit() == empty.ToJson().value.Emit(),
              "emit is deterministic");

        // Full text pipeline: Emit -> Parse -> FromJson.
        auto parsed = JsonValue::Parse(jr.value.Emit());
        Check(parsed.ok() &&
                  CampaignState::FromJson(parsed.value).ok(),
              "emit->parse->load pipeline round-trips");

        // The REAL save path on full state: CJK names and the
        // embedded chain travel Emit->Parse->FromJson.
        auto text = SampleState().ToJson().value.Emit();
        auto full = JsonValue::Parse(text);
        Check(full.ok(), "full state parses");
        auto fullBack = CampaignState::FromJson(full.value);
        Check(fullBack.ok() &&
                  fullBack.value.GetRoster()[0].name ==
                      "右翼第三隊" &&
                  fullBack.value.GetLedger().Verify() == nullptr &&
                  fullBack.value.GetLedger().Size() == 2,
              "full-state text pipeline round-trips");

        // Campaign-complete sentinel: current == vector size.
        CampaignState done;
        done.GetChapter().current = 2;
        done.GetChapter().unlocked = {true, true};
        done.GetChapter().resolved = {true, true};
        Check(CampaignState::FromJson(done.ToJson().value).ok(),
              "campaign-complete sentinel round-trips");
    }

    std::printf(failures ? "CAMPAIGN TESTS FAILED: %d\n"
                         : "CAMPAIGN TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
