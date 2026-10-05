// Story 11.1 — mint-claim schema + outbox (potato.mintclaim/1):
// deterministic ids (filename-safe '-' separators), id==recomputed
// binding, per-kind field rules, atomic Emit, per-file-isolated
// Scan, ledger seal bitcast round-trip.
#include "Campaign/Chain/MintClaim.h"
#include "Campaign/Chain/MintOutbox.h"
#include "Gameplay/Json/JsonValue.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace Potato;
using Potato::Campaign::ClaimKind;
using Potato::Campaign::ClaimId;
using Potato::Campaign::MintClaim;
using Potato::Campaign::MintOutbox;
using Potato::Campaign::ParseRootHex;
using Potato::Campaign::RootHex;
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

static std::string ReadAll(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// A settlement claim with a deliberately >=2^63 ledger tip — the
// wire value must round-trip as an int64 bit-cast (negative JSON).
static MintClaim Settlement() {
    MintClaim c;
    c.kind = ClaimKind::ChapterSettlement;
    c.chapter = 7;
    c.recordRoot = 0x0123abcdef456789ull;
    c.resolution = "governance_victory";
    c.ledgerCount = 412;
    c.ledgerTip = 0xdbdc385a55b95eedull;
    return c;
}

static MintClaim Achievement() {
    MintClaim c;
    c.kind = ClaimKind::Achievement;
    c.chapter = 7;
    c.achievement = "zero_combat";
    c.ledgerCount = 412;
    c.ledgerTip = 0xdbdc385a55b95eedull;
    return c;
}

static bool Rejects(const char* text, const char* errClass) {
    const auto doc = JsonValue::Parse(text);
    if (!doc.ok()) return errClass == std::string("parse");
    const auto r = MintClaim::FromJson(doc.value);
    return !r.ok() && r.error == errClass;
}

int main() {
    // --- ids & codec ---
    {
        const MintClaim s = Settlement();
        Check(ClaimId(s) == "settlement-0123abcdef456789",
              "11.1: settlement id binds record root");
        const MintClaim a = Achievement();
        Check(ClaimId(a) == "achievement-zero_combat-0007",
              "11.1: achievement id binds key+chapter");

        const auto doc = s.ToJson();
        Check(doc.ok(), "11.1: settlement ToJson ok");
        const auto parsed = JsonValue::Parse(doc.value.Emit());
        Check(parsed.ok(), "11.1: emit reparses");
        const auto back = MintClaim::FromJson(parsed.value);
        Check(back.ok() &&
                  back.value.recordRoot == s.recordRoot &&
                  back.value.chapter == s.chapter &&
                  back.value.resolution == s.resolution &&
                  back.value.ledgerCount == s.ledgerCount &&
                  back.value.ledgerTip == s.ledgerTip,
              "11.1: settlement round-trips (tip bitcast)");
        // The wire really did carry a negative int64.
        Check(doc.value.Emit().find("\"tip\":-2604144523890565395") !=
                  std::string::npos,
              "11.1: tip emits as int64 bit-cast");

        const auto adoc = Achievement().ToJson();
        const auto aparsed = JsonValue::Parse(adoc.value.Emit());
        const auto aback = MintClaim::FromJson(aparsed.value);
        Check(aback.ok() &&
                  aback.value.kind == ClaimKind::Achievement &&
                  aback.value.achievement == "zero_combat",
              "11.1: achievement round-trips");

        uint64_t v = 0;
        Check(ParseRootHex("0123abcdef456789", v) &&
                  v == 0x0123abcdef456789ull &&
                  RootHex(v) == "0123abcdef456789",
              "11.1: root hex codec");
    }

    // --- rejection battery ---
    Check(Rejects("{\"schema\":\"potato.mintclaim/2\"}", "schema"),
          "11.1: wrong schema rejected");
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\",\"id\":\"x\","
                  "\"kind\":\"bogus\",\"chapter\":0,"
                  "\"ledger\":{\"count\":0,\"tip\":0}}",
                  "kind"),
          "11.1: unknown kind rejected");
    // Settlement missing record_root.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"settlement-0123abcdef456789\","
                  "\"kind\":\"chapter_settlement\",\"chapter\":7,"
                  "\"resolution\":\"defeat\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: missing record_root rejected");
    // Settlement with non-hex record_root (right id, bad root).
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"settlement-0123abcdef456789\","
                  "\"kind\":\"chapter_settlement\",\"chapter\":7,"
                  "\"record_root\":\"XYZ\",\"resolution\":\"defeat\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: bad record_root hex rejected");
    // Settlement with unknown resolution.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"settlement-0123abcdef456789\","
                  "\"kind\":\"chapter_settlement\",\"chapter\":7,"
                  "\"record_root\":\"0123abcdef456789\","
                  "\"resolution\":\"total_war\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: unknown resolution rejected");
    // Achievement missing key.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"achievement--0007\","
                  "\"kind\":\"achievement\",\"chapter\":7,"
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: missing achievement rejected");
    // Achievement with ':' in key — filename-illegal charset.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"achievement-bad:key-0007\","
                  "\"kind\":\"achievement\",\"chapter\":7,"
                  "\"achievement\":\"bad:key\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: ':' in achievement key rejected");
    // Kind-crossed: settlement carrying achievement.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"settlement-0123abcdef456789\","
                  "\"kind\":\"chapter_settlement\",\"chapter\":7,"
                  "\"record_root\":\"0123abcdef456789\","
                  "\"resolution\":\"defeat\","
                  "\"achievement\":\"zero_combat\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: settlement+achievement field rejected");
    // Kind-crossed: achievement carrying record_root.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"achievement-zero_combat-0007\","
                  "\"kind\":\"achievement\",\"chapter\":7,"
                  "\"achievement\":\"zero_combat\","
                  "\"record_root\":\"0123abcdef456789\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: achievement+record_root field rejected");
    // Stored id != recomputed content id — the tamper check.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"settlement-0000000000000000\","
                  "\"kind\":\"chapter_settlement\",\"chapter\":7,"
                  "\"record_root\":\"0123abcdef456789\","
                  "\"resolution\":\"defeat\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "id"),
          "11.1: id mismatch rejected");
    // Chapter bounds.
    Check(Rejects("{\"schema\":\"potato.mintclaim/1\","
                  "\"id\":\"settlement-0123abcdef456789\","
                  "\"kind\":\"chapter_settlement\",\"chapter\":99,"
                  "\"record_root\":\"0123abcdef456789\","
                  "\"resolution\":\"defeat\","
                  "\"ledger\":{\"count\":1,\"tip\":0}}",
                  "field"),
          "11.1: chapter out of range rejected");

    // --- outbox emit / scan ---
    const fs::path dir =
        fs::temp_directory_path() / "potato_test_mintclaim";
    fs::remove_all(dir);
    {
        MintOutbox box(dir);
        const auto p = box.Emit(Settlement());
        Check(p.ok() && fs::exists(p.value),
              "11.1: emit commits <id>.json");
        Check(p.value.filename().string() ==
                  "settlement-0123abcdef456789.json",
              "11.1: filename is the claim id");
        const std::string first = ReadAll(p.value);
        const auto p2 = box.Emit(Settlement());
        Check(p2.ok() && ReadAll(p2.value) == first,
              "11.1: re-emit byte-identical (idempotent)");
        Check(!fs::exists(dir / "settlement-0123abcdef456789.tmp"),
              "11.1: no tmp left behind");

        Check(box.Emit(Achievement()).ok(),
              "11.1: achievement emits");
        // One poisoned file in the spool.
        Write(dir, "poison.json",
              "{\"schema\":\"potato.mintclaim/1\",\"id\":\"bogus\"}");
        const auto scan = box.Scan();
        Check(scan.ok(), "11.1: scan ok with poison present");
        Check(scan.value.claims.size() == 2 &&
                  scan.value.rejected.size() == 1,
              "11.1: per-file isolation — poison rejected");
        // Canonical order: id sort — achievement- sorts before
        // settlement-.
        Check(ClaimId(scan.value.claims[0]) ==
                  "achievement-zero_combat-0007" &&
                  ClaimId(scan.value.claims[1]) ==
                      "settlement-0123abcdef456789",
              "11.1: scan returns claims in id order");
        // Scanned claims must themselves re-emit identically.
        const auto reDoc = scan.value.claims[1].ToJson();
        Check(reDoc.ok() && reDoc.value.Emit() == first,
              "11.1: scanned claim re-emits identical doc");
    }
    {
        MintOutbox missing(dir / "no_such_dir");
        const auto scan = missing.Scan();
        Check(!scan.ok() && scan.error == "io",
              "11.1: unreadable dir fails io");
    }
    fs::remove_all(dir);

    if (failures == 0) {
        std::printf("ALL PASS\n");
        return 0;
    }
    std::printf("%d FAILURES\n", failures);
    return 1;
}
