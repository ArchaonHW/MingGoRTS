#pragma once

#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Result.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Campaign {

// The five spendable accounts. 秩序/墮落 are deliberately absent —
// they are derived accumulators folded from entry tags (Epic 4),
// never booked directly.
enum class Account : std::uint8_t {
    MartialMerit = 0,   // 武功
    PopularSupport = 1, // 民心
    Mandate = 2,        // 天命
    ArmyPrestige = 3,   // 軍威
    Materiel = 4,       // 物資
};
constexpr int kAccountCount = 5;

// Stable ASCII wire ids (CJK labels are presentation-layer, Epic 8).
const char* AccountName(Account a);
bool AccountFromName(std::string_view name, Account& out);

// One leg of a double-entry posting. `amount > 0` always: the credit
// leg adds it to `account`; the debit leg subtracts it.
struct Leg {
    Account account = Account::Materiel;
    std::int64_t amount = 0;
};

// The write input — one event, two accounts.
struct Posting {
    Leg credit;
    Leg debit;
    std::string memo;              // what happened ("burned village r3")
    std::vector<std::string> tags; // fold keys ("atrocity", region ids)
};

// The append-only unit: a validated Posting plus its position.
struct LedgerEntry {
    std::uint64_t seq = 0; // stable append-order key (0-based)
    Leg credit;
    Leg debit;
    std::string memo;
    std::vector<std::string> tags;
};

// The campaign's state substrate. Append-only; balances are folds,
// never stored — an account total can only change by posting.
// The hash chain (Story 2.2) and the forgery channel (Story 2.3) are
// deliberately separate mechanisms layered on this type.
class Ledger {
public:
    static constexpr std::string_view SCHEMA = "potato.ledger/1";
    static constexpr std::size_t MAX_ENTRIES = 1000000;
    static constexpr std::size_t MAX_TAGS = 16;
    static constexpr std::size_t MAX_TAG_LEN = 64;
    static constexpr std::size_t MAX_MEMO_LEN = 256;
    // Per-leg amount cap. With MAX_ENTRIES worst-case entries the
    // Balance fold stays ~1e18 — far inside int64.
    static constexpr std::int64_t MAX_AMOUNT = 1000000000000LL;

    // The only write path. A posting is balanced iff both amounts
    // are > 0 and the two legs name different accounts. Magnitudes
    // need NOT match — asymmetric costs are the design: burning a
    // village books +40 物資 / −15 民心. The pair is atomic: a
    // rejected posting leaves the chain untouched.
    Gameplay::Result<std::uint64_t> Post(Posting p);

    // Pure fold over entries — +credit where credit.account==a,
    // −debit where debit.account==a. Negative balances are legal
    // (a 民心 deficit is a meaningful ledger state).
    std::int64_t Balance(Account a) const;

    // Append-only view — refs/iterators invalidate on the next Post
    // (std::vector semantics); index by seq instead of caching refs.
    const std::vector<LedgerEntry>& Entries() const { return entries_; }
    std::size_t Size() const { return entries_.size(); }

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    static Gameplay::Result<Ledger> FromJson(const Gameplay::JsonValue& doc);

private:
    std::vector<LedgerEntry> entries_;
};

// Shared invariant used by both Post() and FromJson() — a persisted
// unbalanced entry cannot smuggle past a load.
const char* ValidateLegs(const Leg& credit, const Leg& debit);

} // namespace Potato::Campaign
