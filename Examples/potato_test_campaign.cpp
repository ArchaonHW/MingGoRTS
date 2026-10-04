// Story 3.1 — CampaignState facade: potato.campaign/1 round-trip.
// Story 3.2 — SaveSystem: atomic tmp->rename saves, gated loads.
#include "Campaign/Chapters/ChapterLibrary.h"
#include "Campaign/Chapters/Progression.h"
#include "Campaign/Roster/RefitCamp.h"
#include "Campaign/Roster/Roster.h"
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
using Potato::Campaign::AftermathRow;
using Potato::Campaign::ApplyAftermath;
using Potato::Campaign::Enlist;
using Potato::Campaign::BandCosts;
using Potato::Campaign::Deploy;
using Potato::Campaign::Heal;
using Potato::Campaign::Recruit;
using Potato::Campaign::Plunder;

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

    // ===== Story 3.4 — Chapter progression =====
    {
        namespace fs = std::filesystem;
        const fs::path pdir =
            fs::temp_directory_path() / "potato_test_prog_ch";
        fs::remove_all(pdir);
        fs::create_directories(pdir);
        const auto wch = [](const fs::path& dir, const char* name,
                            const std::string& text) {
            std::ofstream f(dir / name,
                            std::ios::binary | std::ios::trunc);
            f << text;
        };
        const auto chapter = [](const char* id, int idx) {
            return std::string(
                       "{\"schema\":\"potato.chapter/1\","
                       "\"id\":\"") +
                   id + "\",\"index\":" + std::to_string(idx) +
                   ",\"title\":\"t\",\"map\":\"m.json\",\"combat\":true}";
        };

        // Dense run: chapters at 0,1,2.
        wch(pdir, "a.json", chapter("p0", 0));
        wch(pdir, "b.json", chapter("p1", 1));
        wch(pdir, "c.json", chapter("p2", 2));
        ChapterLibrary lib;
        Check(ChapterLibrary::Load(pdir, lib).ok,
              "progression library loads");

        CampaignState st;
        st.GetLedger().Post(Burn());
        st.GetRoster().push_back(
            RosterEntry{"敢死隊", 0, 0, false});
        Check(InitializeProgress(st, lib).ok(),
              "InitializeProgress succeeds");
        const auto& cp0 = st.GetChapter();
        Check(cp0.current == 0 && cp0.unlocked.size() == 3 &&
                  cp0.unlocked[0] && !cp0.unlocked[1],
              "init: first chapter current+unlocked");

        // Advance: resolve ch0 -> unlock ch1.
        Check(ResolveAndAdvance(st, lib).ok(),
              "resolve advances");
        Check(st.GetChapter().resolved[0] &&
                  st.GetChapter().current == 1 &&
                  st.GetChapter().unlocked[1],
              "next chapter unlocked, prior resolved");

        // Carry-forward: roster+ledger are the same state.
        Check(st.GetRoster().size() == 1 &&
                  st.GetLedger().Size() == 1,
              "roster+ledger carry forward untouched");

        // To the end: complete sentinel = space (3).
        Check(ResolveAndAdvance(st, lib).ok() &&
                  ResolveAndAdvance(st, lib).ok(),
              "final chapters resolve");
        Check(st.GetChapter().current == 3,
              "completion sentinel reached");
        Check(!ResolveAndAdvance(st, lib).ok(),
              "advancing a complete campaign rejected");
        Check(st.GetChapter().resolved == std::vector<bool>(
                                              {true, true, true}),
              "all chapters marked resolved");

        // Complete campaign state round-trips (3.1 sentinel fix).
        Check(CampaignState::FromJson(st.ToJson().value).ok(),
              "completed campaign round-trips");

        // Sparse library {0,5}: vectors size 6, dead slots stay
        // locked.
        {
            const fs::path sdir = fs::temp_directory_path() /
                                  "potato_test_prog_sparse";
            fs::remove_all(sdir);
            fs::create_directories(sdir);
            wch(sdir, "a.json", chapter("s0", 0));
            wch(sdir, "b.json", chapter("s5", 5));
            ChapterLibrary sl;
            Check(ChapterLibrary::Load(sdir, sl).ok,
                  "sparse progression library loads");
            CampaignState ss;
            Check(InitializeProgress(ss, sl).ok() &&
                      ss.GetChapter().unlocked.size() == 6 &&
                      ss.GetChapter().current == 0,
                  "sparse init sizes to chapter space");
            Check(ResolveAndAdvance(ss, sl).ok() &&
                      ss.GetChapter().current == 5 &&
                      !ss.GetChapter().unlocked[3] &&
                      ss.GetChapter().unlocked[5],
                  "sparse advance skips dead slots");
            fs::remove_all(sdir);
        }

        // Uninitialized progress vectors can't advance.
        {
            CampaignState bad;
            Check(!ResolveAndAdvance(bad, lib).ok(),
                  "uninitialized progress rejected");
        }

        // Re-init refuses to wipe an in-progress campaign.
        {
            CampaignState mid;
            InitializeProgress(mid, lib);
            ResolveAndAdvance(mid, lib);
            Check(!InitializeProgress(mid, lib).ok(),
                  "re-init on live progress rejected");
        }

        // Dead-slot current: sparse lib {0,5}, vectors sized 6,
        // current=3 (dead) -> rejected. And a chapter that was
        // never unlocked can't resolve.
        {
            const fs::path sdir = fs::temp_directory_path() /
                                  "potato_test_prog_dead";
            fs::remove_all(sdir);
            fs::create_directories(sdir);
            wch(sdir, "a.json", chapter("d0", 0));
            wch(sdir, "b.json", chapter("d5", 5));
            ChapterLibrary dl;
            ChapterLibrary::Load(sdir, dl);
            CampaignState ds;
            InitializeProgress(ds, dl);
            ds.GetChapter().current = 3; // dead slot
            Check(!ResolveAndAdvance(ds, dl).ok(),
                  "dead-slot current rejected");
            ds.GetChapter().current = 5;
            ds.GetChapter().unlocked[5] = false;
            Check(!ResolveAndAdvance(ds, dl).ok(),
                  "locked chapter cannot resolve");
            fs::remove_all(sdir);
        }

        // Empty library can't init.
        {
            ChapterLibrary el;
            CampaignState es;
            Check(!InitializeProgress(es, el).ok(),
                  "empty library init rejected");
        }

        // 64-deep boundary: the complete sentinel at
        // current==MAX_CHAPTERS round-trips; 65 rejects.
        {
            CampaignState full;
            full.GetChapter().unlocked.assign(64, true);
            full.GetChapter().resolved.assign(64, true);
            full.GetChapter().current = 64;
            Check(CampaignState::FromJson(full.ToJson().value).ok(),
                  "64-deep complete sentinel round-trips");
            full.GetChapter().current = 65;
            Check(!CampaignState::FromJson(full.ToJson().value)
                       .ok(),
                  "current beyond MAX_CHAPTERS rejected");
        }

        fs::remove_all(pdir);
    }

    // --- Story 3.5: persistent roster ---
    {
        // Enlist is the roster's write path.
        {
            CampaignState st;
            const auto i0 = Enlist(st, "alpha");
            Check(i0.ok() && i0.value == 0, "enlist returns index");
            Check(st.GetRoster().size() == 1 &&
                      !st.GetRoster()[0].dead,
                  "enlist appends live entry");
            Check(!Enlist(st, "alpha").ok(),
                  "duplicate enlist rejected");
            Check(!Enlist(st, "").ok(),
                  "empty name rejected");
            Check(!Enlist(st, std::string(
                              CampaignState::MAX_NAME_LEN + 1,
                              'x'))
                       .ok(),
                  "oversize name rejected");
            Check(st.GetRoster().size() == 1,
                  "rejected enlists did not mutate");
        }

        // Aftermath: dead stays dead, survivors gain veterancy.
        {
            CampaignState st;
            Enlist(st, "vanguard");
            Enlist(st, "rearguard");
            const auto r = ApplyAftermath(
                st, {{"vanguard", 12, false},
                     {"rearguard", 30, true}});
            Check(r.ok() && r.value.rows == 2 &&
                      r.value.buried == 1 &&
                      r.value.veterans == 1,
                  "aftermath applies");
            const auto& e0 = st.GetRoster()[0];
            const auto& e1 = st.GetRoster()[1];
            Check(e0.veterancy == 1 && e0.casualties == 12 &&
                      !e0.dead,
                  "survivor gains veterancy");
            Check(e1.dead && e1.casualties == 30,
                  "wiped squad persists as dead");

            // Dead stays dead — a second aftermath on a dead
            // squad is a forged report, rejected atomically.
            const auto r2 = ApplyAftermath(
                st, {{"vanguard", 5, false},
                     {"rearguard", 1, false}});
            Check(!r2.ok() &&
                      st.GetRoster()[0].casualties == 12,
                  "dead-squad row rejects whole aftermath");

            // Second battle for the survivor stacks veterancy.
            ApplyAftermath(st, std::vector<AftermathRow>{{"vanguard", 3, false}});
            Check(st.GetRoster()[0].veterancy == 2 &&
                      st.GetRoster()[0].casualties == 15,
                  "survivor stacks veterancy");
        }

        // Atomicity: any bad row rejects the whole report.
        {
            CampaignState st;
            Enlist(st, "a");
            Enlist(st, "b");
            Check(!ApplyAftermath(st, std::vector<AftermathRow>{{"a", 1, false},
                                       {"ghost", 1, false}})
                       .ok(),
                  "unknown name rejected");
            Check(!ApplyAftermath(st, std::vector<AftermathRow>{{"a", 1, false},
                                       {"a", 2, false}})
                       .ok(),
                  "duplicate row rejected");
            Check(!ApplyAftermath(st, std::vector<AftermathRow>{{"a", -1, false}}).ok(),
                  "negative casualties rejected");
            Check(!ApplyAftermath(
                       st, {{"a",
                             CampaignState::MAX_ROSTER_COUNT + 1,
                             false}})
                       .ok(),
                  "unbounded casualties rejected");
            Check(st.GetRoster()[0].casualties == 0 &&
                      st.GetRoster()[0].veterancy == 0,
                  "rejected aftermath left no trace");
            Check(ApplyAftermath(st, {}).ok(),
                  "empty aftermath is a no-op");
        }

        // Round-trip: aftermath state survives the wire, and
        // aftermath applies on a FromJson-loaded roster.
        {
            CampaignState st;
            Enlist(st, "\xE5\x8F\xB3\xE7\xBF\xBC"); // 右翼
            ApplyAftermath(st, std::vector<AftermathRow>{{"\xE5\x8F\xB3\xE7\xBF\xBC", 7,
                                 true}});
            const auto doc = st.ToJson();
            const auto back =
                doc.ok() ? CampaignState::FromJson(doc.value)
                         : Gameplay::Result<CampaignState>{};
            Check(back.ok() && back.value.GetRoster()[0].dead &&
                      back.value.GetRoster()[0].casualties == 7,
                  "aftermath state round-trips");

            // Loaded state continues to take aftermath.
            CampaignState st2;
            Enlist(st2, "f");
            st2 = CampaignState::FromJson(st2.ToJson().value)
                      .value;
            const std::vector<AftermathRow> rows{
                {"f", 2, false}};
            Check(ApplyAftermath(st2, rows).ok() &&
                      st2.GetRoster()[0].veterancy == 1,
                  "aftermath on loaded roster");
        }

        // Boundaries + the mutable-accessor trust boundary.
        {
            CampaignState st;
            // Name exactly at MAX_NAME_LEN is legal.
            Check(Enlist(st, std::string(
                               CampaignState::MAX_NAME_LEN, 'n'))
                       .ok(),
                  "name at MAX_NAME_LEN accepted");
            // Roster fills at 256; 257th rejected.
            while (st.GetRoster().size() <
                   CampaignState::MAX_ROSTER) {
                Enlist(st, "s" +
                               std::to_string(
                                   st.GetRoster().size()));
            }
            Check(st.GetRoster().size() ==
                          CampaignState::MAX_ROSTER &&
                      !Enlist(st, "overflow").ok(),
                  "roster full rejected");
            // Oversized report rejected before per-row checks.
            Check(!ApplyAftermath(
                       st, std::vector<AftermathRow>(
                               st.GetRoster().size() + 1))
                       .ok(),
                  "oversized report rejected");
        }

        {
            CampaignState st;
            Enlist(st, "cap");
            // casualties at exactly MAX is legal.
            Check(ApplyAftermath(
                       st, {{"cap",
                             CampaignState::MAX_ROSTER_COUNT,
                             false}})
                       .ok(),
                  "casualties at bound accepted");
            // One more pushes over the bound -> whole report
            // rejected.
            const std::vector<AftermathRow> over{
                {"cap", 1, false}};
            Check(!ApplyAftermath(st, over).ok() &&
                      st.GetRoster()[0].casualties ==
                          CampaignState::MAX_ROSTER_COUNT,
                  "cumulative overflow rejected atomically");
        }

        {
            CampaignState st;
            Enlist(st, "vet");
            // Live-state bypass: write veterancy at the bound via
            // the mutable accessor (as a corrupt runtime could),
            // then aftermath must refuse the survivor path — but
            // a wipe still lands (wiped rows skip veterancy).
            st.GetRoster()[0].veterancy =
                CampaignState::MAX_ROSTER_COUNT;
            const std::vector<AftermathRow> surv{
                {"vet", 0, false}};
            const std::vector<AftermathRow> wipe{
                {"vet", 0, true}};
            Check(!ApplyAftermath(st, surv).ok(),
                  "veterancy-bound survivor rejected");
            Check(ApplyAftermath(st, wipe).ok() &&
                      st.GetRoster()[0].dead,
                  "wiped row bypasses veterancy check");
        }

        // Corrupted live roster (bypassed setters) can no longer
        // poison a save: ToJson re-validates at the boundary.
        {
            CampaignState st;
            st.GetRoster().push_back(
                RosterEntry{"x", 0, 0, false});
            st.GetRoster().push_back(
                RosterEntry{"x", 0, 0, false}); // dup name
            Check(!st.ToJson().ok(),
                  "ToJson rejects bypassed-invalid roster");
            CampaignState st2;
            st2.GetRoster().push_back(
                RosterEntry{"y", -1, 0, false}); // bad counter
            Check(!st2.ToJson().ok(),
                  "ToJson rejects bypassed-negative counter");
        }
    }

    // --- Story 3.6: RefitCamp ---
    {
        // Band table pins (GDD economy tempo).
        {
            const auto e = BandCosts(1);
            const auto m = BandCosts(5);
            const auto l = BandCosts(10);
            Check(e.heal == 20 && e.recruit == 40 &&
                      e.plunderGain == 30 && e.plunderCost == 10,
                  "early band costs");
            Check(m.heal == 30 && m.recruit == 60 &&
                      m.plunderGain == 45 && m.plunderCost == 15,
                  "mid band costs");
            Check(l.heal == 40 && l.recruit == 80 &&
                      l.plunderGain == 60 && l.plunderCost == 20,
                  "late band costs");
            Check(BandCosts(-1).heal == 40 &&
                      BandCosts(99).heal == 40,
                  "band index clamps");
            Check(BandCosts(2).heal == 20 &&
                      BandCosts(3).heal == 30 &&
                      BandCosts(7).heal == 30 &&
                      BandCosts(8).heal == 40,
                  "band edges partition correctly");
        }

        // Funded refit: deploy/heal/recruit/plunder all land.
        {
            CampaignState st;
            Enlist(st, "iron");
            ApplyAftermath(st, {{"iron", 25, false}});
            Posting grant;
            grant.credit = {Account::Materiel, 100};
            grant.debit = {Account::Mandate, 100};
            grant.memo = "war chest";
            st.GetLedger().Post(grant);
            const std::int64_t mat0 =
                st.GetLedger().Balance(Account::Materiel);
            const std::int64_t prest0 =
                st.GetLedger().Balance(Account::ArmyPrestige);
            const auto entries0 = st.GetLedger().Size();

            const auto dep = Deploy(st, "iron");
            Check(dep.ok() && dep.value == 0,
                  "deploy validates live squad");
            Check(Heal(st, "iron", 1).ok() &&
                      st.GetRoster()[0].casualties == 0,
                  "heal clears casualties");
            Check(st.GetLedger().Balance(Account::Materiel) ==
                          mat0 - 20 &&
                      st.GetLedger().Balance(
                          Account::ArmyPrestige) == prest0 + 20,
                  "heal posts debit/credit pair");

            Check(Recruit(st, "fresh", 1).ok() &&
                      st.GetRoster().size() == 2,
                  "recruit enlists");
            Check(st.GetLedger().Balance(Account::Materiel) ==
                          mat0 - 20 - 40,
                  "recruit posts cost");

            Check(Plunder(st, 1).ok() &&
                      st.GetLedger().Balance(Account::Materiel) ==
                          mat0 - 60 + 30 &&
                      st.GetLedger().Balance(
                          Account::PopularSupport) == -10,
                  "plunder posts asymmetric pair");
            Check(st.GetLedger().Entries().back().tags ==
                          std::vector<std::string>{"plunder"},
                  "plunder carries fold tag");
            Check(st.GetLedger().Size() == entries0 + 3,
                  "refit appends three postings");
        }

        // Overspend: rejected with skip reason, nothing mutates.
        {
            CampaignState st;
            Enlist(st, "poor");
            ApplyAftermath(st, {{"poor", 9, false}});
            const auto size0 = st.GetLedger().Size();
            const auto r = Heal(st, "poor", 1);
            Check(!r.ok() &&
                      r.reason.find("insufficient") !=
                          std::string::npos,
                  "overspend rejected with skip reason");
            Check(st.GetLedger().Size() == size0 &&
                      st.GetRoster()[0].casualties == 9,
                  "overspend leaves ledger+roster untouched");
        }

        // Skips that aren't overspend.
        {
            CampaignState st;
            Enlist(st, "dead");
            ApplyAftermath(st, {{"dead", 5, true}});
            Posting g;
            g.credit = {Account::Materiel, 500};
            g.debit = {Account::Mandate, 500};
            st.GetLedger().Post(g);
            Check(!Deploy(st, "dead").ok(),
                  "dead squad cannot deploy");
            Check(!Heal(st, "dead", 1).ok() &&
                      st.GetRoster()[0].dead,
                  "dead squad cannot heal");
            Check(!Heal(st, "ghost", 1).ok(),
                  "heal unknown squad rejected");
            Check(!Deploy(st, "ghost").ok(),
                  "deploy unknown squad rejected");
            Enlist(st, "fit");
            Check(!Heal(st, "fit", 1).ok(),
                  "full-strength heal skipped");
            Check(!Recruit(st, "dead", 1).ok(),
                  "recruit dup name rejected");
            Check(!Recruit(st, "", 1).ok(),
                  "recruit empty name rejected");
            Check(!Recruit(st,
                           std::string(
                               CampaignState::MAX_NAME_LEN + 1,
                               'x'),
                           1)
                       .ok(),
                  "recruit oversize name rejected");
            Check(st.GetLedger().Size() == 1,
                  "rejected refits post nothing");
        }

        // Afford boundary: exactly cost is enough, cost-1 is not.
        {
            CampaignState st;
            Enlist(st, "edge");
            ApplyAftermath(st, {{"edge", 4, false}});
            Posting g;
            g.credit = {Account::Materiel, 20}; // == early heal
            g.debit = {Account::Mandate, 20};
            st.GetLedger().Post(g);
            Check(Heal(st, "edge", 0).ok() &&
                      st.GetLedger().Balance(
                          Account::Materiel) == 0,
                  "afford at exact boundary");
            ApplyAftermath(st, {{"edge", 2, false}});
            Check(!Heal(st, "edge", 0).ok(),
                  "one short of cost rejected");

            // Recruit at roster-full posts nothing.
            CampaignState full;
            Posting gf;
            gf.credit = {Account::Materiel, 9999};
            gf.debit = {Account::Mandate, 9999};
            full.GetLedger().Post(gf);
            while (full.GetRoster().size() <
                   CampaignState::MAX_ROSTER) {
                Enlist(full, "r" +
                                 std::to_string(
                                     full.GetRoster().size()));
            }
            const auto sz = full.GetLedger().Size();
            Check(!Recruit(full, "one-more", 0).ok() &&
                      full.GetLedger().Size() == sz,
                  "roster-full recruit posts nothing");
        }

        // Plunder works at deficit (negative balances are legal).
        {
            CampaignState st;
            Check(Plunder(st, 9).ok() &&
                      st.GetLedger().Balance(
                          Account::Materiel) == 60 &&
                      st.GetLedger().Balance(
                          Account::PopularSupport) == -20,
                  "plunder at zero/deficit posts");
        }

        // Refit state round-trips through the save wire.
        {
            CampaignState st;
            Enlist(st, "v");
            ApplyAftermath(st, {{"v", 8, false}});
            Posting g;
            g.credit = {Account::Materiel, 100};
            g.debit = {Account::Mandate, 100};
            st.GetLedger().Post(g);
            Heal(st, "v", 0);
            const auto back =
                CampaignState::FromJson(st.ToJson().value);
            Check(back.ok() &&
                      back.value.GetRoster()[0].casualties == 0 &&
                      back.value.GetLedger().Balance(
                          Account::Materiel) == 80,
                  "refit state round-trips");
        }
    }

    std::printf(failures ? "CAMPAIGN TESTS FAILED: %d\n"
                         : "CAMPAIGN TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
