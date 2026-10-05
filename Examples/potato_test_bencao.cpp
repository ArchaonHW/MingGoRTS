// Story 10.1 — bencao codex entry registry (potato.bencao/1):
// per-file isolated load, six-field anatomy, unlock manifest
// validation, annotation-key guard, canonical ordering.
// Story 10.2 — unlock engine (potato.bencao_state/1): six-kind
// trigger evaluation over read-only signals, idempotent unlocks,
// pending 補鈔 FIFO, codex state round-trip.
#include "Campaign/Narrative/Bencao.h"
#include "Campaign/Narrative/BencaoCodex.h"
#include "Campaign/Ledger/Ledger.h"
#include "Campaign/Myth/MythLog.h"
#include "Campaign/Myth/MythState.h"
#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Map/BattleMap.h"

#include <cstdio>
#include <filesystem>
#include <fstream>

using namespace Potato;
using Potato::Campaign::Account;
using Potato::Campaign::BencaoCategory;
using Potato::Campaign::BencaoCodex;
using Potato::Campaign::BencaoLibrary;
using Potato::Campaign::CodexSignals;
using Potato::Campaign::Ledger;
using Potato::Campaign::MythLog;
using Potato::Campaign::MythState;
using Potato::Campaign::Posting;
using Potato::Campaign::ResolveBencaoUnlocks;
using Potato::Campaign::TerrainFlagsOf;
using Potato::Campaign::UnlockKind;
using Potato::Gameplay::BattleMap;
using Potato::Gameplay::JsonValue;

namespace fs = std::filesystem;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        std::printf("[PASS] %s\n", name);
    } else {
        std::printf("[FAIL] %s\n", name);
        ++failures;
    }
}

static void Write(const fs::path& dir, const char* name,
                  const char* text) {
    std::ofstream f(dir / name,
                    std::ios::binary | std::ios::trunc);
    f << text;
}

// A minimal valid entry; `kind`/`extra` let callers vary the
// unlock block without duplicating the whole fixture.
static std::string Entry(const char* id, const char* cat,
                         const char* unlock) {
    std::string s =
        "{\"schema\":\"potato.bencao/1\",\"id\":\"";
    s += id;
    s += "\",\"category\":\"";
    s += cat;
    s += "\",\"name\":\"三七\",\"nature\":\"甘、微苦，溫。\","
         "\"indications\":\"止血散血，定痛。\","
         "\"source\":\"本草綱目·卷十二\",\"unlock\":";
    s += unlock;
    s += "}";
    return s;
}

int main() {
    const fs::path dir =
        fs::temp_directory_path() / "potato_test_bencao";
    fs::remove_all(dir);
    fs::create_directories(dir);

    // --- valid load: canonical order, aliases/origin optional ---
    {
        Write(dir, "a.json",
              Entry("sanqi", "shancao",
                    "{\"kind\":\"ledger_tag\",\"tag\":\"first_casualty\"}")
                  .c_str());
        Write(dir, "b.json",
              Entry("fuzi", "ducao",
                    "{\"kind\":\"corruption\",\"at_least\":1}")
                  .c_str());
        Write(dir, "c.json",
              Entry("lingzhi", "jinshi",
                    "{\"kind\":\"myth_state\",\"state\":\"shrine_pacified\"}")
                  .c_str());
        Write(dir, "note.txt", "not json"); // skipped ext

        BencaoLibrary lib;
        const auto res = BencaoLibrary::Load(dir, lib);
        Check(res.ok, "10.1: library load succeeds");
        Check(res.rejected.empty(), "10.1: no rejections");
        Check(lib.Size() == 3, "10.1: valid entries registered");
        // Canonical order: category ordinal first (shancao < ducao
        // < jinshi) regardless of filename.
        Check(lib.Entries()[0].id == "sanqi" &&
                  lib.Entries()[1].id == "fuzi" &&
                  lib.Entries()[2].id == "lingzhi",
              "10.1: canonical category-then-id order");
        Check(lib.Find("sanqi") &&
                  lib.Find("sanqi")->category ==
                      BencaoCategory::Shancao &&
                  lib.Find("sanqi")->unlockKind ==
                      UnlockKind::LedgerTag &&
                  lib.Find("sanqi")->unlockParam == "first_casualty",
              "10.1: Find + manifest fields parse");
        Check(lib.Find("nobody") == nullptr, "10.1: Find misses");
    }

    // --- rejections: schema, fields, kinds, dup id, guard ---
    {
        Write(dir, "bad_schema.json",
              "{\"schema\":\"potato.bencao/2\",\"id\":\"x\"}");
        Write(dir, "no_nature.json",
              "{\"schema\":\"potato.bencao/1\",\"id\":\"nonat\","
              "\"category\":\"shancao\",\"name\":\"x\","
              "\"indications\":\"y\",\"source\":\"z\","
              "\"unlock\":{\"kind\":\"chapter_close\","
              "\"chapter\":-1}}");
        Write(dir, "bad_cat.json",
              Entry("badcat", "yaowu",
                    "{\"kind\":\"chapter_close\",\"chapter\":0}")
                  .c_str());
        Write(dir, "bad_kind.json",
              Entry("badkind", "shancao",
                    "{\"kind\":\"auto\"}")
                  .c_str());
        // corruption without at_least.
        Write(dir, "no_param.json",
              Entry("noparam", "ducao",
                    "{\"kind\":\"corruption\"}")
                  .c_str());
        // 批註 is runtime-bound — files cannot author it.
        Write(dir, "marg.json",
              "{\"schema\":\"potato.bencao/1\",\"id\":\"marg\","
              "\"category\":\"shancao\",\"name\":\"x\","
              "\"nature\":\"y\",\"indications\":\"z\","
              "\"source\":\"s\","
              "\"unlock\":{\"kind\":\"chapter_close\",\"chapter\":-1},"
              "\"marginalia\":{\"x\":\"頁邊字\"}}");
        // Duplicate id — same id as a.json's "sanqi".
        Write(dir, "zz.json",
              Entry("sanqi", "xicao",
                    "{\"kind\":\"terrain\",\"terrain\":\"ford\"}")
                  .c_str());
        // name over MAX_NAME_LEN.
        {
            std::string over =
                "{\"schema\":\"potato.bencao/1\",\"id\":\"over\","
                "\"category\":\"shancao\",\"name\":\"";
            over.append(BencaoLibrary::MAX_NAME_LEN + 1, 'x');
            over += "\",\"nature\":\"y\",\"indications\":\"z\","
                    "\"source\":\"s\","
                    "\"unlock\":{\"kind\":\"chapter_close\","
                    "\"chapter\":-1}}";
            Write(dir, "over.json", over.c_str());
        }

        BencaoLibrary lib;
        const auto res = BencaoLibrary::Load(dir, lib);
        Check(res.ok, "10.1: load succeeds despite rejects");
        Check(lib.Size() == 3,
              "10.1: bad files leave library untouched");
        Check(res.rejected.size() == 8,
              "10.1: schema/field/kind/param/marginalia/dup/bounds "
              "each rejected");
        // dup rejection must not consume the id of the good file.
        Check(lib.Find("sanqi") &&
                  lib.Find("sanqi")->category ==
                      BencaoCategory::Shancao,
              "10.1: dup file doesn't overwrite registered id");
    }

    // --- per-kind param pins ---
    {
        const fs::path kdir =
            fs::temp_directory_path() / "potato_test_bencao_kinds";
        fs::remove_all(kdir);
        fs::create_directories(kdir);
        Write(kdir, "t.json",
              Entry("t1", "manshui",
                    "{\"kind\":\"terrain\",\"terrain\":\"ford\"}")
                  .c_str());
        Write(kdir, "g.json",
              Entry("g1", "gucai",
                    "{\"kind\":\"governance\",\"event\":\"epidemic\"}")
                  .c_str());
        Write(kdir, "c.json",
              Entry("c1", "renbu",
                    "{\"kind\":\"chapter_close\",\"chapter\":-1}")
                  .c_str());
        BencaoLibrary lib;
        const auto res = BencaoLibrary::Load(kdir, lib);
        Check(res.ok && lib.Size() == 3 &&
                  res.rejected.empty(),
              "10.1: remaining unlock kinds parse");
        Check(lib.Find("g1")->unlockKind == UnlockKind::Governance &&
                  lib.Find("g1")->unlockParam == "epidemic" &&
                  lib.Find("c1")->unlockInt == -1,
              "10.1: kind params land");
        fs::remove_all(kdir);
    }

    // --- empty dir / unreadable dir ---
    {
        const fs::path empty =
            fs::temp_directory_path() / "potato_test_bencao_empty";
        fs::remove_all(empty);
        fs::create_directories(empty);
        BencaoLibrary lib;
        const auto res = BencaoLibrary::Load(empty, lib);
        Check(res.ok && lib.Size() == 0,
              "10.1: empty dir yields empty library");
        fs::remove_all(empty);

        BencaoLibrary x;
        const auto r = BencaoLibrary::Load(
            dir / "does_not_exist", x);
        Check(!r.ok && r.error == "io",
              "10.1: unreadable dir fails the load");
    }

    // ================= Story 10.2 — unlock engine =================

    const fs::path dir2 =
        fs::temp_directory_path() / "potato_test_bencao_102";
    fs::remove_all(dir2);
    fs::create_directories(dir2);

    // One entry per trigger kind plus a pinned chapter_close.
    Write(dir2, "a.json",
          Entry("t_herb", "shancao",
                "{\"kind\":\"terrain\",\"terrain\":\"forest\"}")
              .c_str());
    Write(dir2, "b.json",
          Entry("l_herb", "xicao",
                "{\"kind\":\"ledger_tag\",\"tag\":\"atrocity\"}")
              .c_str());
    Write(dir2, "c.json",
          Entry("g_herb", "gucai",
                "{\"kind\":\"governance\",\"event\":\"governance_victory\"}")
              .c_str());
    Write(dir2, "d.json",
          Entry("m_herb", "chongshou",
                "{\"kind\":\"myth_state\",\"state\":\"pacify_shrine\"}")
              .c_str());
    Write(dir2, "e.json",
          Entry("m_inf", "chongshou",
                "{\"kind\":\"myth_state\",\"state\":\"infiltrated\"}")
              .c_str());
    Write(dir2, "f.json",
          Entry("c_herb", "ducao",
                "{\"kind\":\"corruption\",\"at_least\":10}")
              .c_str());
    Write(dir2, "g.json",
          Entry("cl_any", "renbu",
                "{\"kind\":\"chapter_close\",\"chapter\":-1}")
              .c_str());
    Write(dir2, "h.json",
          Entry("cl_2", "renbu",
                "{\"kind\":\"chapter_close\",\"chapter\":2}")
              .c_str());

    BencaoLibrary lib2;
    Check(BencaoLibrary::Load(dir2, lib2).ok && lib2.Size() == 8,
          "10.2: trigger library loads");

    // --- null signals: only the "every settle" kind fires ---
    {
        BencaoCodex codex;
        const auto fresh =
            Potato::Campaign::ResolveBencaoUnlocks(lib2, CodexSignals{},
                                                   codex);
        Check(fresh.ok() && fresh.value.size() == 1 &&
                  fresh.value[0] == "cl_any",
              "10.2: null signals fire only chapter_close(any)");
        Check(!codex.IsUnlocked("cl_any") &&
                  codex.Pending().size() == 1,
              "10.2: trigger enqueues pending, book still blank");
    }

    // --- assembled signals: each kind resolves off its own store ---
    {
        Ledger ledger;
        Posting atrocity;
        atrocity.credit = {Account::Materiel, 40};
        atrocity.debit = {Account::PopularSupport, 15};
        atrocity.memo = "burned village r3";
        atrocity.tags = {"atrocity", "corruption:+15"};
        Check(ledger.Post(atrocity).ok(), "10.2: atrocity posts");
        Posting seal;
        seal.credit = {Account::Mandate, 1};
        seal.debit = {Account::ArmyPrestige, 1};
        seal.memo = "chapter sealed";
        seal.tags = {"resolution:governance_victory", "chapter:0"};
        Check(ledger.Post(seal).ok(), "10.2: resolution seal posts");

        MythLog mythLog;
        Check(mythLog.Record("pacify_shrine", "安撫", 0, 1, -1).ok(),
              "10.2: myth action logs");
        MythState myth;
        Check(myth.Set("ch1", 3, 2), "10.2: infiltration state sets");

        CodexSignals sig;
        sig.ledger = &ledger;
        sig.mythLog = &mythLog;
        sig.myth = &myth;
        sig.chapterId = "ch1";
        sig.chapterIndex = 0; // cl_2 wants 2 — must NOT fire here
        sig.terrains = {"forest"};

        BencaoCodex codex;
        const auto fresh =
            Potato::Campaign::ResolveBencaoUnlocks(lib2, sig, codex);
        Check(fresh.ok(), "10.2: resolve ok");
        // Canonical order = library order (category, then id):
        // t_herb(shancao) < l_herb(xicao) < c_herb(ducao)
        // < g_herb(gucai) < m_herb, m_inf (chongshou, id order)
        // < cl_any(renbu). cl_2 is silent at chapter 0.
        Check(fresh.value.size() == 7,
              "10.2: seven kinds fire, pinned chapter stays shut");
        Check(fresh.value[0] == "t_herb" && fresh.value[1] == "l_herb" &&
                  fresh.value[2] == "c_herb" &&
                  fresh.value[3] == "g_herb" &&
                  fresh.value[4] == "m_herb" &&
                  fresh.value[5] == "m_inf" &&
                  fresh.value[6] == "cl_any",
              "10.2: fresh ids in canonical eval order");

        // Idempotent re-run — same signals unlock nothing twice.
        const auto again =
            Potato::Campaign::ResolveBencaoUnlocks(lib2, sig, codex);
        Check(again.ok() && again.value.empty() &&
                  codex.Pending().size() == 7,
              "10.2: idempotent — no double unlock/enqueue");

        // Pending drains FIFO; chapter 2 settle fires cl_2.
        auto drained = codex.TakePending(3);
        Check(drained.size() == 3 && drained[0] == "t_herb" &&
                  drained[2] == "c_herb" && codex.Pending().size() == 4,
              "10.2: TakePending drains FIFO");
        Check(codex.IsUnlocked("t_herb") &&
                  codex.Unlocked().size() == 3,
              "10.2: 補鈔 delivery writes pages into the book");
        sig.chapterIndex = 2;
        const auto late =
            Potato::Campaign::ResolveBencaoUnlocks(lib2, sig, codex);
        Check(late.ok() && late.value.size() == 1 &&
                  late.value[0] == "cl_2",
              "10.2: pinned chapter_close fires on its chapter");

        // State round-trip: unlocked + remaining pending survive.
        const auto doc = codex.ToJson();
        Check(doc.ok(), "10.2: ToJson ok");
        const auto parsed = JsonValue::Parse(doc.value.Emit());
        Check(parsed.ok(), "10.2: emit reparses");
        const auto back = BencaoCodex::FromJson(parsed.value);
        Check(back.ok() &&
                  back.value.Unlocked() == codex.Unlocked() &&
                  back.value.Pending() == codex.Pending(),
              "10.2: state round-trips losslessly");
    }

    // --- FromJson guards: bad schema, dedupe, pending∩unlocked ---
    {
        const auto bad = JsonValue::Parse(
            "{\"schema\":\"potato.bencao_state/2\",\"unlocked\":[],"
            "\"pending\":[]}");
        Check(bad.ok() && !BencaoCodex::FromJson(bad.value).ok(),
              "10.2: wrong schema rejected");
        const auto dup = JsonValue::Parse(
            "{\"schema\":\"potato.bencao_state/1\","
            "\"unlocked\":[\"a\",\"a\",\"b\"],"
            "\"pending\":[\"b\",\"c\",\"c\"]}");
        const auto st = BencaoCodex::FromJson(dup.value);
        Check(st.ok() && st.value.Unlocked().size() == 2 &&
                  st.value.Pending().size() == 1 &&
                  st.value.Pending()[0] == "c",
              "10.2: dupes dedupe, pending∩unlocked drops");
    }

    // --- TerrainFlagsOf: distinct flag names, flag order ---
    {
        const auto mdoc = JsonValue::Parse(
            "{\"schema\":\"potato.map/1\",\"id\":\"m\",\"name\":\"m\","
            "\"regions\":["
            "{\"id\":\"a\",\"terrain\":[\"forest\",\"river\"]},"
            "{\"id\":\"b\",\"terrain\":[\"highland\"]}]}");
        Check(mdoc.ok(), "10.2: map doc parses");
        const auto map = BattleMap::FromJson(mdoc.value);
        Check(map.ok(), "10.2: map builds");
        const auto flags = Potato::Campaign::TerrainFlagsOf(map.value);
        Check(flags.size() == 3 && flags[0] == "river" &&
                  flags[1] == "forest" && flags[2] == "highland",
              "10.2: TerrainFlagsOf = distinct ids, flag order");
    }

    fs::remove_all(dir);
    fs::remove_all(dir2);
    std::printf(failures ? "BENCAO TESTS FAILED: %d\n"
                         : "BENCAO TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
