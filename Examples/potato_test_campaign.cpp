// Story 3.1 — CampaignState facade: potato.campaign/1 round-trip.
// Story 3.2 — SaveSystem: atomic tmp->rename saves, gated loads.
#include "Campaign/Chapters/ChapterLibrary.h"
#include "Campaign/Save/SaveSystem.h"
#include "Campaign/State/CampaignState.h"
#include "Gameplay/Json/JsonValue.h"

#include <cstdio>
#include <filesystem>
#include <fstream>

using namespace Potato;
using Potato::Gameplay::JsonValue;
using Potato::Campaign::CampaignState;
using Potato::Campaign::Ledger;
using Potato::Campaign::Posting;
using Potato::Campaign::Leg;
using Potato::Campaign::Account;
using Potato::Campaign::RosterEntry;
using Potato::Campaign::SaveSystem;
using Potato::Campaign::ChapterLibrary;

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

    // ===== Story 3.2 — SaveSystem =====
    {
        namespace fs = std::filesystem;
        const fs::path dir =
            fs::temp_directory_path() / "potato_test_saves";
        fs::remove_all(dir); // clean slate
        SaveSystem saves(dir);

        // Save -> load round-trip through real files.
        {
            const CampaignState st = SampleState();
            const auto w = saves.Save(0, st);
            Check(w.ok() && fs::exists(w.value),
                  "save writes the slot file");
            auto back = saves.Load(0);
            Check(back.ok() &&
                      back.value.GetRoster()[0].name ==
                          "右翼第三隊" &&
                      back.value.GetLedger().Size() == 2 &&
                      back.value.GetLedger().Verify() == nullptr,
                  "save->load round-trips full state");
            Check(saves.ListSlots() == std::vector<int>{0},
                  "ListSlots reports written slot");
        }

        // Overwrite: second save replaces.
        {
            CampaignState st;
            st.GetChapter().current = 1;
            st.GetChapter().unlocked = {true, true};
            st.GetChapter().resolved = {true, false};
            Check(saves.Save(0, st).ok(), "overwrite save writes");
            auto back = saves.Load(0);
            Check(back.ok() &&
                      back.value.GetChapter().current == 1 &&
                      back.value.GetRoster().empty(),
                  "overwrite replaces prior save");
        }

        // Crash simulation: a stale .tmp must not shadow the
        // committed .json.
        {
            const fs::path tmp = SaveSystem::TmpPath(dir, 0);
            std::ofstream junk(tmp, std::ios::binary | std::ios::trunc);
            junk << "{\"schema\":\"potato.campaign/1\",\"chapte";
            junk.close();
            auto back = saves.Load(0);
            Check(back.ok() && back.value.GetChapter().current == 1,
                  "stale .tmp never shadows committed save");
            fs::remove(tmp);
        }

        // Corrupt committed file -> typed reject; next Load still
        // fails (nothing mutated) but a later good Save works.
        {
            const fs::path dst = SaveSystem::SlotPath(dir, 1);
            std::ofstream bad(dst,
                              std::ios::binary | std::ios::trunc);
            bad << "{\"schema\":\"potato.campaign/99\"}";
            bad.close();
            auto r = saves.Load(1);
            Check(!r.ok() && r.error == "schema" && !r.reason.empty(),
                  "bad schema rejected with reason class");
            auto r2 = saves.Load(1);
            Check(!r2.ok(), "rejected file stays rejected");
        }

        // Malformed committed .json -> "parse" class rejection.
        {
            const fs::path dst = SaveSystem::SlotPath(dir, 2);
            std::ofstream bad(dst,
                              std::ios::binary | std::ios::trunc);
            bad << "{\"schema\":\"potato.campaign/1\",\"chapter\":";
            bad.close();
            const auto r = saves.Load(2);
            Check(!r.ok() && r.error == "parse",
                  "torn committed file rejected as parse error");
        }

        // Missing file + slot bounds.
        {
            Check(!saves.Load(5).ok(), "missing slot rejected");
            Check(!saves.Save(-1, SampleState()).ok() &&
                      !saves.Load(-1).ok() &&
                      !saves.Save(8, SampleState()).ok(),
                  "slot bounds enforced before I/O");
        }

        fs::remove_all(dir);
    }

    // ===== Story 3.3 — ChapterLibrary =====
    {
        namespace fs = std::filesystem;
        const fs::path cdir =
            fs::temp_directory_path() / "potato_test_chapters";
        fs::remove_all(cdir);
        fs::create_directories(cdir);
        const auto write = [&cdir](const char* name,
                                   const std::string& text) {
            std::ofstream f(cdir / name,
                            std::ios::binary | std::ios::trunc);
            f << text;
        };
        const auto write_s = [](const fs::path& d,
                                const char* name,
                                const std::string& text) {
            std::ofstream f(d / name,
                            std::ios::binary | std::ios::trunc);
            f << text;
        };
        const auto chapter = [](const char* id, int idx,
                                bool combat = true) {
            return std::string(
                       "{\"schema\":\"potato.chapter/1\","
                       "\"id\":\"") +
                   id + "\",\"index\":" + std::to_string(idx) +
                   ",\"title\":\"t\",\"map\":\"m.json\",\"combat\":" +
                   (combat ? "true" : "false") + "}";
        };

        write("02_second.json", chapter("ch_b", 1));
        write("01_first.json", chapter("ch_a", 0));
        write("03_talk.json", chapter("ch_c", 2, false));
        write("bad_schema.json",
              "{\"schema\":\"potato.chapter/2\",\"id\":\"x\"}");
        write("bad_field.json",
              "{\"schema\":\"potato.chapter/1\",\"id\":\"y\"}");
        write("dup_id.json", chapter("ch_a", 3));
        write("dup_index.json", chapter("ch_d", 0));
        write("not_json.txt",
              "{\"schema\":\"potato.chapter/1\"}"); // skipped ext

        ChapterLibrary lib;
        const auto res = ChapterLibrary::Load(cdir, lib);
        Check(res.ok, "library load succeeds");
        Check(lib.Size() == 3, "valid chapters registered");
        Check(res.rejected.size() == 4,
              "bad schema/field/dup-id/dup-index each rejected");

        // Registered in index order regardless of filename.
        Check(lib.Chapters()[0].id == "ch_a" &&
                  lib.Chapters()[1].id == "ch_b" &&
                  lib.Chapters()[2].id == "ch_c",
              "library ordered by index");
        Check(lib.Find("ch_b") != nullptr &&
                  lib.Find("nope") == nullptr,
              "Find by id works");
        Check(lib.AtIndex(2) != nullptr &&
                  !lib.AtIndex(2)->combat &&
                  lib.AtIndex(5) == nullptr,
              "AtIndex + zero-combat flag");

        // Empty readable dir -> empty library, ok.
        {
            const fs::path empty =
                fs::temp_directory_path() / "potato_test_empty_ch";
            fs::remove_all(empty);
            fs::create_directories(empty);
            ChapterLibrary e;
            const auto r = ChapterLibrary::Load(empty, e);
            Check(r.ok && e.Size() == 0,
                  "empty dir yields empty library");
            fs::remove_all(empty);
        }

        // Missing dir -> io failure.
        {
            ChapterLibrary x;
            const auto r = ChapterLibrary::Load(
                cdir / "does_not_exist", x);
            Check(!r.ok && r.error == "io",
                  "unreadable dir fails the load");
        }

        // Sparse indexes: legal; chapter space is [0, MaxIndex()+1).
        {
            const fs::path sdir =
                fs::temp_directory_path() / "potato_test_sparse_ch";
            fs::remove_all(sdir);
            fs::create_directories(sdir);
            write_s(sdir, "a.json", chapter("s_a", 0));
            write_s(sdir, "b.json", chapter("s_b", 5));
            ChapterLibrary sl;
            const auto r = ChapterLibrary::Load(sdir, sl);
            Check(r.ok && sl.Size() == 2 && sl.MaxIndex() == 5 &&
                      sl.AtIndex(3) == nullptr &&
                      sl.AtIndex(5)->id == "s_b",
                  "sparse indexes: gap is dead space, not error");
            fs::remove_all(sdir);
        }

        fs::remove_all(cdir);
    }

    std::printf(failures ? "CAMPAIGN TESTS FAILED: %d\n"
                         : "CAMPAIGN TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
