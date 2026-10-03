#include "Campaign/Ledger/Ledger.h"
#include "Gameplay/Json/JsonValue.h"

#include <cstdio>
#include <string>

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

    // --- potato.ledger/2 round-trip ---
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
              "ToJson emits potato.ledger/2");
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
        bad = JsonValue::Parse(R"({"schema":"potato.ledger/3"})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "bad version rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/2","seal":0})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "missing entries rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/2","entries":[]})");
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
    std::printf(failures ? "LEDGER TESTS FAILED: %d\n"
                         : "LEDGER TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
