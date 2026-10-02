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

    // --- potato.ledger/1 round-trip ---
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
              "ToJson emits potato.ledger/1");
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

    // --- FromJson hard gates ---
    {
        auto bad = JsonValue::Parse(R"({"schema":"potato.ledger/2"})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(), "bad version rejected");
        bad = JsonValue::Parse(R"({"schema":"potato.ledger/1"})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(), "missing entries rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/1","entries":[{"seq":0,
              "credit":{"account":"materiel","amount":10},
              "debit":{"account":"materiel","amount":4},
              "memo":"x","tags":[]}]})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "unbalanced persisted entry rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/1","entries":[{"seq":0,
              "credit":{"account":"gold","amount":10},
              "debit":{"account":"materiel","amount":4},
              "memo":"x","tags":[]}]})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "unknown account name rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/1","entries":[{"seq":1,
              "credit":{"account":"materiel","amount":10},
              "debit":{"account":"mandate","amount":4},
              "memo":"x","tags":[]}]})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "non-contiguous seq rejected");
        bad = JsonValue::Parse(R"([1,2,3])");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "non-object root rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/1","entries":[{"seq":0,
              "credit":{"account":"materiel","amount":2000000000000},
              "debit":{"account":"mandate","amount":4},
              "memo":"x","tags":[]}]})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
              "persisted over-cap amount rejected");
        bad = JsonValue::Parse(
            R"({"schema":"potato.ledger/1","entries":[{"seq":0,
              "credit":{"account":"materiel","amount":10},
              "debit":{"account":"mandate","amount":4},
              "memo":"x","tags":["a","a"]}]})");
        Check(bad.ok() && !Ledger::FromJson(bad.value).ok(),
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

    std::printf(failures ? "LEDGER TESTS FAILED: %d\n"
                         : "LEDGER TESTS PASS\n",
                failures);
    return failures ? 1 : 0;
}
