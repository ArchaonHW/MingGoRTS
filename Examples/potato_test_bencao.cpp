// Story 10.1 — bencao codex entry registry (potato.bencao/1):
// per-file isolated load, six-field anatomy, unlock manifest
// validation, annotation-key guard, canonical ordering.
#include "Campaign/Narrative/Bencao.h"

#include <cstdio>
#include <filesystem>
#include <fstream>

using namespace Potato;
using Potato::Campaign::BencaoLibrary;
using Potato::Campaign::BencaoCategory;
using Potato::Campaign::UnlockKind;

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

    fs::remove_all(dir);
    std::printf(failures ? "BENCAO TESTS FAILED: %d\n"
                         : "BENCAO TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
