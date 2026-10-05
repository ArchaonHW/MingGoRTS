#include "Campaign/Ledger/CrossCheck.h"
#include "Campaign/Ledger/DeedBook.h"
#include "Campaign/Ledger/HistorianReport.h"
#include "Campaign/Ledger/Ledger.h"
#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Record/BattleRecorder.h"

#include <cstdio>
#include <string>
#include <vector>

namespace {

int failures = 0;
void Check(bool cond, const char* name) {
    std::printf("[%s] %s\n", cond ? "PASS" : "FAIL", name);
    if (!cond) ++failures;
}

using namespace Potato::Campaign;
using Potato::Gameplay::JsonValue;

Posting BurnVillage() {
    Posting p;
    p.credit = {Account::Materiel, 40};
    p.debit = {Account::PopularSupport, 15};
    p.memo = "burned village r3";
    p.tags = {"atrocity", "r3"};
    return p;
}

} // namespace

int main() {
    // --- AC1: balanced pair posts atomically; balances fold ---
    {
        Ledger l;
        auto r = l.Post(BurnVillage());
        Check(r.ok() && r.value == 0, "post returns seq 0");
        Check(l.Size() == 1, "entry recorded once");
        Check(l.Balance(Account::Materiel) == 40, "credit folds +40");
        Check(l.Balance(Account::PopularSupport) == -15,
              "debit folds -15");
        Check(l.Balance(Account::Mandate) == 0, "untouched account 0");
        const LedgerEntry& e = l.Entries().front();
        Check(e.credit.account == Account::Materiel &&
                  e.debit.account == Account::PopularSupport &&
                  e.memo == "burned village r3" && e.tags.size() == 2,
              "entry carries both legs + memo + tags");
    }

    // --- AC1: unbalanced postings rejected; chain untouched ---
    {
        Ledger l;
        Posting p = BurnVillage();
        p.credit.amount = 0;
        Check(!l.Post(p).ok(), "zero credit rejected");
        p = BurnVillage();
        p.debit.amount = -5;
        Check(!l.Post(p).ok(), "negative debit rejected");
        p = BurnVillage();
        p.debit.account = Account::Materiel;
        Check(!l.Post(p).ok(), "same-account pair rejected");
        Check(l.Size() == 0 && l.Balance(Account::Materiel) == 0,
              "rejections leave the chain untouched");
    }

    // --- rejection matrix, second half ---
    {
        Ledger l;
        Posting p = BurnVillage();
        p.credit.amount = -1;
        Check(!l.Post(p).ok(), "negative credit rejected");
        p = BurnVillage();
        p.debit.amount = 0;
        Check(!l.Post(p).ok(), "zero debit rejected");
        p = BurnVillage();
        p.credit.account = static_cast<Account>(99);
        Check(!l.Post(p).ok(), "phantom credit account rejected");
        p = BurnVillage();
        p.credit.amount = Ledger::MAX_AMOUNT + 1;
        Check(!l.Post(p).ok(), "amount over MAX_AMOUNT rejected");
        p = BurnVillage();
        p.memo.assign(Ledger::MAX_MEMO_LEN + 1, 'x');
        Check(!l.Post(p).ok(), "oversized memo rejected");
        p = BurnVillage();
        p.tags = {"atrocity", "atrocity"};
        Check(!l.Post(p).ok(), "duplicate tag rejected");
        p = BurnVillage();
        p.tags = {""};
        Check(!l.Post(p).ok(), "empty tag rejected");
        p = BurnVillage();
        p.tags.assign(Ledger::MAX_TAGS + 1, "t");
        Check(!l.Post(p).ok(), "tag count over MAX_TAGS rejected");
        Check(l.Size() == 0, "rejection matrix leaves chain empty");
    }

    // --- MAX leg posts and folds without overflow ---
    {
        Ledger l;
        Posting p = BurnVillage();
        p.credit.amount = Ledger::MAX_AMOUNT;
        p.debit.amount = Ledger::MAX_AMOUNT;
        Check(l.Post(p).ok(), "MAX_AMOUNT leg accepted");
        Check(l.Balance(Account::Materiel) == Ledger::MAX_AMOUNT,
              "max credit folds exactly");
    }

    // --- asymmetric magnitudes are legal by design ---
    {
        Ledger l;
        Posting p = BurnVillage(); // +40 / -15 is the canonical case
        Check(l.Post(p).ok(), "asymmetric pair accepted");
    }

    // --- seq is strictly increasing across appends ---
    {
        Ledger l;
        auto s0 = l.Post(BurnVillage());
        Posting p2 = BurnVillage();
        p2.credit = {Account::MartialMerit, 7};
        p2.debit = {Account::Materiel, 12};
        auto s1 = l.Post(p2);
        Check(s0.ok() && s1.ok() && s1.value == s0.value + 1,
              "seq increments in append order");
        Check(l.Balance(Account::Materiel) == 40 - 12,
              "balances fold across entries");
    }

    // --- potato.ledger/3 round-trip ---
    {
        Ledger l;
        l.Post(BurnVillage());
        Posting p = BurnVillage();
        p.credit = {Account::Mandate, 3};
        p.debit = {Account::ArmyPrestige, 9};
        p.memo = "temple levy";
        p.tags = {};
        l.Post(p);
        auto doc = l.ToJson();
        Check(doc.ok() &&
                  doc.value.FindString("schema") != nullptr &&
                  *doc.value.FindString("schema") ==
                      std::string(Ledger::SCHEMA),
              "ToJson emits potato.ledger/3");
        auto back = Ledger::FromJson(doc.value);
        Check(back.ok() && back.value.Size() == 2 &&
                  back.value.Balance(Account::Materiel) == 40 &&
                  back.value.Balance(Account::PopularSupport) == -15 &&
                  back.value.Balance(Account::Mandate) == 3 &&
                  back.value.Balance(Account::ArmyPrestige) == -9,
              "round-trip preserves entries + balances");
        Check(back.ok() &&
                  back.value.Entries()[0].tags.size() == 2 &&
                  back.value.Entries()[1].memo == "temple levy",
              "round-trip preserves memo + tags");
    }

    // --- serialization via Emit->Parse (the real wire path) ---
    {
        Ledger l;
        l.Post(BurnVillage());
        auto doc = l.ToJson();
        auto parsed = JsonValue::Parse(doc.value.Emit());
        Check(parsed.ok(), "emitted json re-parses");
        auto back = parsed.ok() ? Ledger::FromJson(parsed.value)
                                : Potato::Gameplay::Fail<Ledger>("", "");
        Check(back.ok() && back.value.Balance(Account::Materiel) == 40,
              "emit->parse->load lossless");
    }

    // --- AC (2.2): chain formation ---
    {
        Ledger l;
        l.Post(BurnVillage());
        Posting p = BurnVillage();
        p.credit = {Account::Mandate, 3};
        p.debit = {Account::ArmyPrestige, 9};
        l.Post(p);
        const auto& es = l.Entries();
        Check(es[0].prevHash == Ledger::kGenesisHash,
              "entry 0 chains to genesis");
        Check(es[1].prevHash == es[0].hash && es[1].hash != es[0].hash,
              "entry 1 links to entry 0's hash");
        Check(l.Tip() == es[1].hash, "Tip() is the last entry hash");
        Check(Ledger{}.Tip() == Ledger::kGenesisHash,
              "empty chain tip is genesis");
        Check(l.Verify() == nullptr, "honest chain verifies clean");
    }

    // --- AC (2.2): mutated history fails verify; load rejected ---
    {
        Ledger l;
        l.Post(BurnVillage());
        Posting p = BurnVillage();
        p.credit = {Account::Mandate, 3};
        p.debit = {Account::ArmyPrestige, 9};
        l.Post(p);
        const JsonValue doc = l.ToJson().value;

        const auto mutateEntry = [&doc](std::size_t idx,
                                        const char* key,
                                        JsonValue v) {
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[idx].Members();
            e[key] = std::move(v);
            arr[idx] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            return JsonValue::MakeObject(std::move(o));
        };
        Check(!Ledger::FromJson(mutateEntry(
                  0, "memo", JsonValue::String("altered"))).ok(),
              "mutated memo -> hash mismatch -> reject");
        Check(!Ledger::FromJson(mutateEntry(
                  0, "hash", JsonValue::Int(42))).ok(),
              "forged stored hash -> reject");
        Check(!Ledger::FromJson(mutateEntry(
                  1, "prevHash", JsonValue::Int(42))).ok(),
              "relinked prevHash -> chain break -> reject");

        // Truncate the tail but keep the stored seal.
        {
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            arr.pop_back();
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            Check(!Ledger::FromJson(JsonValue::MakeObject(std::move(o)))
                       .ok(),
                  "truncated tail -> seal mismatch -> reject");
        }
        // Honest round-trip still verifies.
        auto back = Ledger::FromJson(doc);
        Check(back.ok() && back.value.Verify() == nullptr &&
                  back.value.Entries()[1].hash == l.Entries()[1].hash,
              "round-trip preserves + verifies chain");
    }

    // --- FromJson hard gates ---
    {
        auto bad = JsonValue::Parse(R"({"schema":"potato.ledger/1"})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "old version rejected");
        bad = JsonValue::Parse(R"({"schema":"potato.ledger/2"})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "/2 rejected (no provenance semantics)");
        bad = JsonValue::Parse(R"({"schema":"potato.ledger/4"})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "bad version rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/3","seal":0})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "missing entries rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/3","entries":[]})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "missing seal rejected");
        bad = JsonValue::Parse(R"([1,2,3])");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "non-object root rejected");

        // Structural gates on a real /2 doc — mutate fields that fail
        // before the chain check runs.
        Ledger l;
        l.Post(BurnVillage());
        const JsonValue doc = l.ToJson().value;
        const auto mutateEntry = [&doc](const char* key, JsonValue v) {
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[0].Members();
            e[key] = std::move(v);
            arr[0] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            return JsonValue::MakeObject(std::move(o));
        };
        Check(!Ledger::FromJson(mutateEntry("seq", JsonValue::Int(1)))
                   .ok(),
              "non-contiguous seq rejected");
        const auto mutateCredit = [&doc](const char* key, JsonValue v) {
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[0].Members();
            JsonValue::Object leg = e["credit"].Members();
            leg[key] = std::move(v);
            e["credit"] = JsonValue::MakeObject(std::move(leg));
            arr[0] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            return JsonValue::MakeObject(std::move(o));
        };
        Check(!Ledger::FromJson(mutateCredit("amount", JsonValue::Int(0)))
                   .ok(),
              "unbalanced persisted entry rejected");
        Check(!Ledger::FromJson(mutateCredit(
                  "account", JsonValue::String("gold")))
                   .ok(),
              "unknown account name rejected");
        Check(!Ledger::FromJson(mutateCredit(
                  "amount", JsonValue::Int(2000000000000LL)))
                   .ok(),
              "persisted over-cap amount rejected");
        Check(!Ledger::FromJson(mutateEntry(
                  "tags", JsonValue::MakeArray(
                              {JsonValue::String("a"),
                               JsonValue::String("a")})))
                   .ok(),
              "persisted duplicate tag rejected");
    }

    // --- account name map is total and round-trips ---
    {
        bool all = true;
        for (int i = 0; i < kAccountCount; ++i) {
            Account a;
            all = all && AccountFromName(
                             AccountName(static_cast<Account>(i)), a) &&
                  static_cast<int>(a) == i;
        }
        Check(all, "all five accounts name round-trip");
        Account a;
        Check(!AccountFromName("min_xin", a),
              "cjk/pinyin variants not silently accepted");
    }

    // --- threat model: recompute is the boundary (2.5 anchors) ---
    {
        Ledger l;
        l.Post(BurnVillage());
        Posting p = BurnVillage();
        p.credit = {Account::Mandate, 3};
        p.debit = {Account::ArmyPrestige, 9};
        l.Post(p);
        const JsonValue doc = l.ToJson().value;

        // Truncate + recompute seal: LOADS. The seal is unkeyed —
        // documented boundary; Story 2.5 anchors the tip externally.
        {
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            arr.pop_back();
            const std::uint64_t h0 = static_cast<std::uint64_t>(
                arr[0]["hash"].AsInt());
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            o["seal"] = JsonValue::Int(static_cast<std::int64_t>(
                Ledger::SealHash(1, h0)));
            Check(Ledger::FromJson(JsonValue::MakeObject(std::move(o)))
                      .ok(),
                  "truncate+reseal loads (recompute-class boundary)");
        }
        // Rehashed rewrite: mutate memo, then recompute the WHOLE
        // chain (every downstream prevHash+hash) and the seal ->
        // LOADS. Same boundary: the seal is unkeyed.
        {
            LedgerEntry e0 = l.Entries()[0];
            e0.memo = "rewritten";
            e0.hash = Ledger::EntryHash(e0);
            LedgerEntry e1 = l.Entries()[1];
            e1.prevHash = e0.hash;
            e1.hash = Ledger::EntryHash(e1);
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object f0 = arr[0].Members();
            f0["memo"] = JsonValue::String("rewritten");
            f0["hash"] =
                JsonValue::Int(static_cast<std::int64_t>(e0.hash));
            arr[0] = JsonValue::MakeObject(std::move(f0));
            JsonValue::Object f1 = arr[1].Members();
            f1["prevHash"] =
                JsonValue::Int(static_cast<std::int64_t>(e1.prevHash));
            f1["hash"] =
                JsonValue::Int(static_cast<std::int64_t>(e1.hash));
            arr[1] = JsonValue::MakeObject(std::move(f1));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            o["seal"] = JsonValue::Int(static_cast<std::int64_t>(
                Ledger::SealHash(2, e1.hash)));
            Check(Ledger::FromJson(JsonValue::MakeObject(std::move(o)))
                      .ok(),
                  "rehashed rewrite loads (anchoring is story 2.5)");
        }
        // Mistyped chain fields rejected at the IsInt gates.
        const auto mutE = [&doc](const char* key, JsonValue v) {
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[0].Members();
            e[key] = std::move(v);
            arr[0] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            return JsonValue::MakeObject(std::move(o));
        };
        Check(!Ledger::FromJson(mutE("hash",
                  JsonValue::String("x"))).ok(),
              "string hash rejected");
        Check(!Ledger::FromJson(mutE("prevHash",
                  JsonValue::Real(1.5))).ok(),
              "real prevHash rejected");
        {
            JsonValue::Object o = doc.Members();
            o["seal"] = JsonValue::Bool(true);
            Check(!Ledger::FromJson(JsonValue::MakeObject(std::move(o)))
                       .ok(),
                  "bool seal rejected");
        }
        // Entry 0's prevHash must be genesis — 0 is not a synonym.
        Check(!Ledger::FromJson(mutE("prevHash",
                  JsonValue::Int(0))).ok(),
              "prevHash 0 != genesis -> reject");

        // Empty ledger round-trips; seal commits to count+genesis.
        {
            const JsonValue empty = Ledger{}.ToJson().value;
            auto back = Ledger::FromJson(empty);
            Check(back.ok() &&
                      back.value.Tip() == Ledger::kGenesisHash,
                  "empty ledger round-trips with genesis seal");
        }
        // Post on a loaded ledger continues the chain.
        {
            auto back = Ledger::FromJson(doc);
            Check(back.ok() &&
                      back.value.Post(BurnVillage()).ok() &&
                      back.value.Entries()[2].prevHash ==
                          l.Entries()[1].hash &&
                      back.value.Verify() == nullptr,
                  "post-on-load continues chain");
        }
    }
    // --- AC (2.3): forgery channel + suspect flags ---
    {
        Ledger l;
        l.Post(BurnVillage());
        const auto fr = l.Forge(BurnVillage());
        Check(fr.ok() && fr.value == 1 &&
                  l.Entries()[1].provenance == Provenance::Forged &&
                  l.Entries()[0].provenance == Provenance::Honest,
              "forge marks provenance");
        Check(l.Verify() == nullptr,
              "forged entry chain-verifies (kept, not corrupt)");
        Check(l.Entries()[1].prevHash == l.Entries()[0].hash,
              "forged entry links through the chain");
        // Forge validates like Post — a forgery pretends to be
        // valid bookkeeping.
        {
            Posting bad = BurnVillage();
            bad.debit = bad.credit;
            Check(!l.Forge(bad).ok() && l.Size() == 2,
                  "forge enforces the same balance invariant");
        }
        // Forged entries still fold — kept data, not hidden.
        Check(l.Balance(Account::Materiel) == 80,
              "forged entries fold into balances");

        // Suspect flag is a mutable judgment overlay.
        Check(!l.Entries()[1].suspect, "suspect defaults false");
        Check(l.SetSuspect(1), "set suspect on forged entry");
        Check(!l.SetSuspect(999), "set suspect out of range");
        Check(l.SuspectEntries().size() == 1 &&
                  l.SuspectEntries()[0] == 1,
              "SuspectEntries lists flagged seqs");
        Check(l.Verify() == nullptr,
              "flag write never breaks the chain");

        // Provenance + suspect both persist.
        auto back = Ledger::FromJson(l.ToJson().value);
        Check(back.ok() &&
                  back.value.Entries()[1].provenance ==
                      Provenance::Forged &&
                  back.value.Entries()[1].suspect &&
                  back.value.SuspectEntries().size() == 1,
              "provenance + suspect round-trip");

        // Flipping provenance in the file is a content mutation —
        // the stored hash no longer matches.
        {
            const JsonValue doc = l.ToJson().value;
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[1].Members();
            e["provenance"] = JsonValue::String("honest");
            arr[1] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            Check(!Ledger::FromJson(JsonValue::MakeObject(
                                       std::move(o)))
                       .ok(),
                  "stripped forged mark -> hash mismatch -> reject");
        }
        // Flipping the suspect flag in the file is hash-transparent:
        // the doc LOADS with the tampered flag — judgment metadata
        // is not a claim (documented surface).
        {
            const JsonValue doc = l.ToJson().value;
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[1].Members();
            e["suspect"] = JsonValue::Bool(false);
            arr[1] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            auto t = Ledger::FromJson(JsonValue::MakeObject(
                std::move(o)));
            Check(t.ok() && !t.value.Entries()[1].suspect,
                  "suspect flag is hash-transparent (loads)");
        }
        // Strict typing on the new fields.
        const auto mutField = [&l](const char* key, JsonValue v) {
            const JsonValue doc = l.ToJson().value;
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[0].Members();
            e.erase(key);
            if (!v.IsNull()) e[key] = std::move(v);
            arr[0] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            return JsonValue::MakeObject(std::move(o));
        };
        Check(!Ledger::FromJson(mutField("provenance",
                  JsonValue{})).ok(),
              "missing provenance rejected");
        Check(!Ledger::FromJson(mutField("provenance",
                  JsonValue::Int(1))).ok(),
              "int provenance rejected");
        Check(!Ledger::FromJson(mutField("provenance",
                  JsonValue::String("planted"))).ok(),
              "unknown provenance rejected");
        Check(!Ledger::FromJson(mutField("suspect",
                  JsonValue::Int(1))).ok(),
              "int suspect rejected");
    }
    // --- 2.3 edge probes (review additions) ---
    {
        // Forge on an empty ledger: first link is genesis.
        Ledger l;
        const auto fr = l.Forge(BurnVillage());
        Check(fr.ok() && l.Entries()[0].prevHash ==
                  Ledger::kGenesisHash &&
                  l.Entries()[0].provenance == Provenance::Forged,
              "forge on empty ledger chains to genesis");
        // An all-forged chain verifies and round-trips.
        l.Forge(BurnVillage());
        Check(l.Verify() == nullptr, "all-forged chain verifies");
        auto back = Ledger::FromJson(l.ToJson().value);
        Check(back.ok() &&
                  back.value.Entries()[0].provenance ==
                      Provenance::Forged &&
                  back.value.Entries()[1].provenance ==
                      Provenance::Forged,
              "all-forged chain round-trips");

        // Suspect on an honest entry: set, clear, persist both ways.
        Check(l.SetSuspect(0) && l.Entries()[0].suspect,
              "honest entry can be flagged (suspicious auditor)");
        Check(l.SetSuspect(0, false) && !l.Entries()[0].suspect,
              "suspect flag clears");
        l.SetSuspect(0);
        back = Ledger::FromJson(l.ToJson().value);
        Check(back.ok() && back.value.Entries()[0].suspect &&
                  back.value.Entries()[0].provenance ==
                      Provenance::Forged,
              "flagged entry round-trips with provenance");

        // Case-variant / whitespace provenance rejected.
        const auto mutProv = [&l](const char* s) {
            const JsonValue doc = l.ToJson().value;
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[0].Members();
            e["provenance"] = JsonValue::String(s);
            arr[0] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            return JsonValue::MakeObject(std::move(o));
        };
        Check(!Ledger::FromJson(mutProv("Forged")).ok(),
              "wrong-case provenance rejected");
        Check(!Ledger::FromJson(mutProv("forged ")).ok(),
              "trailing-space provenance rejected");
        // Missing suspect field rejected (strict typing).
        {
            const JsonValue doc = l.ToJson().value;
            JsonValue::Object o = doc.Members();
            JsonValue::Array arr = o["entries"].Items();
            JsonValue::Object e = arr[0].Members();
            e.erase("suspect");
            arr[0] = JsonValue::MakeObject(std::move(e));
            o["entries"] = JsonValue::MakeArray(std::move(arr));
            Check(!Ledger::FromJson(JsonValue::MakeObject(
                                       std::move(o)))
                       .ok(),
                  "missing suspect rejected");
        }
    }
    // --- AC (2.4): audit-aware report ---
    {
        Ledger l;
        l.Post(BurnVillage());  // seq 0: honest
        l.Forge(BurnVillage()); // seq 1: forged
        l.Post(BurnVillage());  // seq 2: honest
        l.SetSuspect(2);        // seq 2: flagged

        const HistorianReport r = RenderHistorianReport(l);
        Check(r.audit.entries == 3 && r.audit.forged == 1 &&
                  r.audit.suspect == 1,
              "audit segment folds counts");
        Check(r.audit.chainOk && r.audit.tip == l.Tip() &&
                  r.audit.seal == l.Seal(),
              "audit carries chain state");
        Check(r.omissions == 2 && r.includedSeqs.size() == 1 &&
                  r.includedSeqs[0] == 0,
              "forged + suspect omitted, confessedly counted");

        OmissionPolicy p;
        p.omitSuspect = false;
        const HistorianReport rp = RenderHistorianReport(l, p);
        Check(rp.omissions == 1 && rp.includedSeqs.size() == 2,
              "policy: suspect narrated, forged still hidden");
        p.omitForged = false;
        const HistorianReport ra = RenderHistorianReport(l, p);
        Check(ra.omissions == 0 && ra.includedSeqs.size() == 3,
              "policy: nothing omitted confesses zero");

        Check(r.RenderText().find("\xE6\x9C\xAC\xE5\xA0\x81\xE5\x91\x8A\xE7\x9C\x81\xE7\x95\xA5 2 \xE9\xA0\x85") !=
                  std::string::npos,
              "confession line carries the count");
        const HistorianReport re = RenderHistorianReport(Ledger{});
        Check(re.omissions == 0 &&
                  re.RenderText().find("\xE6\x9C\xAC\xE5\xA0\x81\xE5\x91\x8A\xE7\x9C\x81\xE7\x95\xA5 0 \xE9\xA0\x85") !=
                      std::string::npos,
              "empty ledger still confesses");
        Check(re.audit.chainOk && re.audit.breakReason == nullptr,
              "empty chain state is honest");

        // Overlap: forged AND suspect — omitted once, counted in both.
        l.SetSuspect(1);
        const HistorianReport ro = RenderHistorianReport(l);
        Check(ro.omissions == 2 && ro.audit.forged == 1 &&
                  ro.audit.suspect == 2,
              "overlap entry omitted once, counted in both audits");
        // Narrate-all policy: confession zero, audit counts persist.
        const HistorianReport rn =
            RenderHistorianReport(l, {false, false});
        Check(rn.omissions == 0 && rn.audit.forged == 1 &&
                  rn.audit.suspect == 2 && rn.includedSeqs.size() == 3,
              "narrate-all keeps audit counts");
        // Partial policy: forged narrated, suspect still hidden.
        const HistorianReport rg =
            RenderHistorianReport(l, {false, true});
        Check(rg.omissions == 2 && rg.includedSeqs.size() == 1 &&
                  rg.includedSeqs[0] == 0,
              "forged-narrated policy hides only suspect");
        // Exact included seqs under default policy.
        Check(r.includedSeqs.size() == 1 && r.includedSeqs[0] == 0,
              "default policy narrates only clean entries");
        // tip/seal present in the text render.
        Check(r.RenderText().find("tip: ") != std::string::npos &&
                  r.RenderText().find("seal: ") != std::string::npos,
              "render carries tip + seal");
    }
    // --- AC (2.5): replay-ledger cross-check ---
    {
        using Potato::Gameplay::BattleRecorder;

        // A minimal but real record doc: payload + integrity.root.
        const auto makeRecord = [](std::int64_t seed) {
            JsonValue::Object p;
            p["schema"] =
                JsonValue::String("potato.battle_record/1");
            p["seed"] = JsonValue::Int(seed);
            const std::uint64_t root = BattleRecorder::ComputeRoot(
                JsonValue::MakeObject(p));
            JsonValue::Object o = std::move(p);
            JsonValue::Object in;
            in["root"] =
                JsonValue::Int(static_cast<std::int64_t>(root));
            o["integrity"] = JsonValue::MakeObject(std::move(in));
            return std::pair{JsonValue::MakeObject(std::move(o)),
                             root};
        };

        // Tag convention round-trips.
        std::uint64_t parsed = 0;
        Check(ParseRecordRootTag(RecordRootTag(0xdeadbeef01ULL),
                                 parsed) &&
                  parsed == 0xdeadbeef01ULL,
              "record_root tag round-trips");
        Check(!ParseRecordRootTag("record_root:xyz", parsed) &&
                  !ParseRecordRootTag("record_:00", parsed),
              "malformed anchor tags ignored");

        // Clean pair: anchor tag binds the record's real root.
        Ledger l;
        const auto [docA, rootA] = makeRecord(42);
        {
            Posting p = BurnVillage();
            p.tags.push_back(RecordRootTag(rootA));
            l.Post(p);
        }
        const CrossCheckResult rc = CrossCheckRecord(l, docA);
        Check(rc.verdict == CrossVerdict::Clean && rc.anchors == 1 &&
                  rc.flagged == 0,
              "consistent pair verifies clean");

        // Tampered record: payload edited post-seal -> recomputed
        // root diverges from the anchor's stored root.
        {
            const auto [docB, rootB] = makeRecord(43);
            JsonValue::Object o = docB.Members();
            o["seed"] = JsonValue::Int(999); // tamper after seal
            const JsonValue tampered =
                JsonValue::MakeObject(std::move(o));
            Posting p = BurnVillage();
            p.tags.push_back(RecordRootTag(rootB));
            l.Post(p); // seq 1 anchors docB's true root
            const CrossCheckResult rm = CrossCheckRecord(l, tampered);
            Check(rm.verdict == CrossVerdict::RecordInconsistent &&
                      rm.flagged == 1 &&
                      l.Entries()[1].suspect,
                  "mismatch flags the covering entry suspect");
            Check(l.SuspectEntries().size() == 1,
                  "suspect flag persists after cross-check");
        }

        // Record uncovered by any anchor: absence, not mismatch.
        {
            const auto [docC, rootC] = makeRecord(7);
            const CrossCheckResult rn = CrossCheckRecord(l, docC);
            Check(rn.verdict == CrossVerdict::NoAnchor &&
                      rn.anchors == 0,
                  "uncovered record -> NoAnchor");
        }
        // Malformed record doc: no integrity root.
        {
            JsonValue::Object o;
            o["schema"] =
                JsonValue::String("potato.battle_record/1");
            const CrossCheckResult rb =
                CrossCheckRecord(l, JsonValue::MakeObject(
                                        std::move(o)));
            Check(rb.verdict == CrossVerdict::BadRecord,
                  "missing integrity -> BadRecord");
        }
        // An anchor claiming a root this record neither declares
        // nor produces belongs to a different record — skipped.
        {
            Ledger l2;
            const auto [docD, rootD] = makeRecord(11);
            Posting p1 = BurnVillage();
            p1.tags.push_back(RecordRootTag(rootD));
            l2.Post(p1); // seq 0: this record's anchor
            Posting p2 = BurnVillage();
            p2.tags.push_back(RecordRootTag(rootD + 1));
            l2.Post(p2); // seq 1: some other record's anchor
            const CrossCheckResult rd = CrossCheckRecord(l2, docD);
            Check(rd.verdict == CrossVerdict::Clean &&
                      rd.anchors == 1 && rd.flagged == 0 &&
                      !l2.Entries()[1].suspect,
                  "foreign-record anchor is skipped");
        }
        // Tampered payload: BOTH anchors claiming the declared root
        // now disagree with what the bytes produce — both flagged.
        {
            Ledger l3;
            const auto [docE, rootE] = makeRecord(13);
            Posting p1 = BurnVillage();
            p1.tags.push_back(RecordRootTag(rootE));
            l3.Post(p1);
            Posting p2 = BurnVillage();
            p2.tags.push_back(RecordRootTag(rootE));
            l3.Post(p2); // second covering entry for same record
            JsonValue::Object o = docE.Members();
            o["seed"] = JsonValue::Int(77); // tamper post-seal
            const JsonValue tampered =
                JsonValue::MakeObject(std::move(o));
            const CrossCheckResult re =
                CrossCheckRecord(l3, tampered);
            Check(re.verdict == CrossVerdict::RecordInconsistent &&
                      re.anchors == 2 && re.flagged == 2 &&
                      l3.Entries()[0].suspect &&
                      l3.Entries()[1].suspect,
                  "every diverging anchor flagged");
        }
    }
    // --- 2.5 review additions ---
    {
        using Potato::Gameplay::BattleRecorder;
        const auto makeRecord = [](std::int64_t seed) {
            JsonValue::Object p;
            p["schema"] =
                JsonValue::String("potato.battle_record/1");
            p["seed"] = JsonValue::Int(seed);
            const std::uint64_t root = BattleRecorder::ComputeRoot(
                JsonValue::MakeObject(p));
            JsonValue::Object o = std::move(p);
            JsonValue::Object in;
            in["root"] =
                JsonValue::Int(static_cast<std::int64_t>(root));
            o["integrity"] = JsonValue::MakeObject(std::move(in));
            return std::pair{JsonValue::MakeObject(std::move(o)),
                             root};
        };

        // integrity.root tampered, payload intact: the anchor is
        // relevant via stored==recomputed; record fails its own
        // seal -> RecordInconsistent, anchor flagged.
        {
            Ledger l;
            const auto [doc, root] = makeRecord(21);
            Posting p = BurnVillage();
            p.tags.push_back(RecordRootTag(root));
            l.Post(p);
            JsonValue::Object o = doc.Members();
            JsonValue::Object in = o["integrity"].Members();
            in["root"] = JsonValue::Int(12345); // corrupt the seal
            o["integrity"] = JsonValue::MakeObject(std::move(in));
            const CrossCheckResult rr =
                CrossCheckRecord(l, JsonValue::MakeObject(
                                        std::move(o)));
            Check(rr.verdict == CrossVerdict::RecordInconsistent &&
                      rr.declaredRoot == 12345 &&
                      rr.recomputedRoot == root &&
                      rr.anchors == 1 && rr.flagged == 1 &&
                      l.Entries()[0].suspect,
                  "corrupt seal -> RecordInconsistent + flag");
        }
        // Self-inconsistent record with NO matching anchor: the
        // broken seal must still surface (not NoAnchor).
        {
            Ledger l;
            const auto [doc, root] = makeRecord(31);
            JsonValue::Object o = doc.Members();
            o["seed"] = JsonValue::Int(99);
            const CrossCheckResult rn =
                CrossCheckRecord(l, JsonValue::MakeObject(
                                        std::move(o)));
            Check(rn.verdict ==
                      CrossVerdict::RecordInconsistent &&
                      rn.anchors == 0 && rn.flagged == 0,
                  "broken-seal record is never NoAnchor");
        }
        // Boundary pin: payload+root BOTH honestly recomputed after
        // tamper -> indistinguishable from an uncovered record ->
        // NoAnchor. OrphanAnchors() finds the orphaned claim.
        {
            Ledger l;
            const auto [doc, root] = makeRecord(41);
            Posting p = BurnVillage();
            p.tags.push_back(RecordRootTag(root));
            l.Post(p);
            JsonValue::Object o = doc.Members();
            o["seed"] = JsonValue::Int(55); // tamper payload
            o.erase("integrity"); // reseal over payload alone
            // attacker honestly reseals:
            JsonValue::Object in;
            in["root"] = JsonValue::Int(static_cast<std::int64_t>(
                BattleRecorder::ComputeRoot(
                    JsonValue::MakeObject(o))));
            o["integrity"] = JsonValue::MakeObject(std::move(in));
            const JsonValue resigned =
                JsonValue::MakeObject(std::move(o));
            const CrossCheckResult ro =
                CrossCheckRecord(l, resigned);
            Check(ro.verdict == CrossVerdict::NoAnchor,
                  "resealed tamper reads as uncovered (boundary)");
            const auto orphans =
                OrphanAnchors(l, {ro.recomputedRoot});
            Check(orphans.size() == 1 && orphans[0] == 0,
                  "orphan anchor surfaces the orphaned claim");
            Check(OrphanAnchors(l, {root}).empty(),
                  "presented root clears its anchor");
        }
        // Forged anchors still cover (claim accuracy is what the
        // chain attests) — but the count is exposed.
        {
            Ledger l;
            const auto [doc, root] = makeRecord(51);
            Posting p = BurnVillage();
            p.tags.push_back(RecordRootTag(root));
            l.Forge(p);
            const CrossCheckResult rf = CrossCheckRecord(l, doc);
            Check(rf.verdict == CrossVerdict::Clean &&
                      rf.forgedAnchors == 1,
                  "forged anchor covers, counted");
        }
        // Idempotency: second check flags nothing new.
        {
            Ledger l;
            const auto [doc, root] = makeRecord(61);
            Posting p = BurnVillage();
            p.tags.push_back(RecordRootTag(root));
            l.Post(p);
            JsonValue::Object o = doc.Members();
            o["seed"] = JsonValue::Int(1);
            const JsonValue t = JsonValue::MakeObject(std::move(o));
            CrossCheckRecord(l, t);
            const CrossCheckResult r2 = CrossCheckRecord(l, t);
            Check(r2.flagged == 0 && l.Entries()[0].suspect,
                  "repeat check does not reflag");
        }
        // Uppercase tag is not canonical.
        std::uint64_t tmp = 0;
        Check(!ParseRecordRootTag("record_root:DEADBEEF00000000",
                                  tmp),
              "uppercase anchor tag rejected");
    }
    // --- AC (4.2): deeds translate; atrocities post tagged ---
    {
        using Potato::Gameplay::SimEvent;
        Ledger l;
        std::vector<SimEvent> evs;
        const auto ev = [&](SimEvent::Kind k, int side, int sq,
                            int param) {
            SimEvent e;
            e.kind = k;
            e.side = side;
            e.squadIndex = sq;
            e.param = param;
            evs.push_back(e);
        };
        ev(SimEvent::Kind::VillageBurned, 0, 0, 3);   // atrocity
        ev(SimEvent::Kind::SquadExecuted, 0, 4, 2);   // atrocity
        ev(SimEvent::Kind::VillageBurned, 1, 1, 0);   // enemy deed
        ev(SimEvent::Kind::CardFired, 0, 0, -1);      // not a deed
        ev(SimEvent::Kind::VillageOccupied, 0, 0, 1);
        ev(SimEvent::Kind::ConvoyRaided, 0, 2, 5);
        const auto n = BookDeeds(l, 0, evs);
        Check(n.ok() && n.value == 4,
              "deeds post; enemy + non-deeds skipped");
        Check(l.Size() == 4, "posting order preserved in chain");
        const LedgerEntry& burn = l.Entries()[0];
        Check(burn.credit.account == Account::Materiel &&
                  burn.credit.amount == 40 &&
                  burn.debit.account == Account::PopularSupport &&
                  burn.debit.amount == 15,
              "burn books +物資40 / -民心15");
        bool burnTagged = false;
        for (const std::string& t : burn.tags) {
            if (t == "atrocity") burnTagged = true;
        }
        Check(burnTagged, "burn carries the atrocity tag");
        const LedgerEntry& exe = l.Entries()[1];
        Check(exe.credit.account == Account::ArmyPrestige &&
                  exe.credit.amount == 5 &&
                  exe.debit.account == Account::PopularSupport &&
                  exe.debit.amount == 10,
              "execution books +軍威5 / -民心10");
        Check(exe.tags.size() == 3 && exe.tags[1] == "victim:4" &&
                  exe.tags[2] == "region:2",
              "execution tags carry victim + place");
        const LedgerEntry& occ = l.Entries()[2];
        Check(occ.credit.account == Account::PopularSupport &&
                  occ.debit.account == Account::Materiel,
              "occupation books +民心 / -物資");
        const LedgerEntry& raid = l.Entries()[3];
        Check(raid.tags.size() == 2 && raid.tags[0] == "raid",
              "raid tagged raid, not atrocity");
        Check(l.Balance(Account::Materiel) == 40 + 25 - 5 &&
                  l.Balance(Account::PopularSupport) ==
                      -15 - 10 + 10 - 5 &&
                  l.Balance(Account::ArmyPrestige) == 5,
              "deed folds land in balances");
        // Enemy-side deeds don't post even when the fold asks for
        // the player — but they DO post under their own side.
        Ledger le;
        const auto ne = BookDeeds(le, 1, evs);
        Check(ne.ok() && ne.value == 1 && le.Size() == 1,
              "enemy deeds book only under the enemy side");
    }
    // --- AC (4.2): the report acknowledges or visibly omits ---
    {
        using Potato::Gameplay::SimEvent;
        Ledger l;
        SimEvent b;
        b.kind = SimEvent::Kind::VillageBurned;
        b.side = 0; b.param = 3; b.squadIndex = 0;
        const SimEvent evs[1] = {b};
        Check(BookDeeds(l, 0, evs).ok(), "deed booked");
        l.Post(BurnVillage()); // second honest atrocity entry
        Posting plain;
        plain.credit = {Account::Materiel, 1};
        plain.debit = {Account::PopularSupport, 1};
        plain.memo = "routine levy";
        l.Post(plain); // non-atrocity
        const HistorianReport r = RenderHistorianReport(l);
        Check(r.audit.atrocities == 2,
              "audit counts atrocity-tagged entries");
        Check(r.RenderText().find("atrocities: 2") !=
                  std::string::npos,
              "report text acknowledges the count");
        // Omitted atrocity still can't hide: omission confession +
        // audit count both show it.
        l.SetSuspect(0);
        const HistorianReport ro = RenderHistorianReport(l);
        Check(ro.omissions == 1 && ro.audit.atrocities == 2 &&
                  ro.includedSeqs.size() == 2,
              "omitted atrocity still counted in audit");
        const HistorianReport re = RenderHistorianReport(Ledger{});
        Check(re.audit.atrocities == 0, "clean ledger counts zero");
    }
    std::printf(failures ? "LEDGER TESTS FAILED: %d\n"
                         : "LEDGER TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
