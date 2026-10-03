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

// The append-only unit: a validated Posting plus its position, hash-
// chained to its predecessor (Story 2.2). prevHash of entry 0 is
// kGenesisHash; hash covers all content fields + prevHash, so a
// mutated entry breaks its own stored hash and the next link.
struct LedgerEntry {
    std::uint64_t seq = 0; // stable append-order key (0-based)
    Leg credit;
    Leg debit;
    std::string memo;
    std::vector<std::string> tags;
    std::uint64_t prevHash = 0;
    std::uint64_t hash = 0;
};

// The campaign's state substrate. Append-only; balances are folds,
// never stored — an account total can only change by posting.
// The hash chain (Story 2.2) and the forgery channel (Story 2.3) are
// deliberately separate mechanisms layered on this type.
class Ledger {
public:
    // /2 adds hash-chain fields (prevHash/hash/seal). /1 docs are
    // rejected — a /1 loader would silently skip chain verification.
    // Wire note: hash-family values are uint64 bit-cast to int64
    // (bit-exact round-trip; ~half emit negative). External
    // producers writing unsigned literals >=2^63 parse as Real and
    // get rejected — producers must bit-cast too.
    static constexpr std::string_view SCHEMA = "potato.ledger/2";
    static constexpr std::uint64_t kGenesisHash =
        14695981039346656037ull; // FNV-1a offset basis
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

    // The chain tip commits to the entire history — an empty chain's
    // tip is the genesis hash.
    std::uint64_t Tip() const {
        return entries_.empty() ? kGenesisHash : entries_.back().hash;
    }

    // A seal commits to BOTH the entry count and the chain tip, so
    // truncating the tail requires recomputing it — the same effort
    // class as a wholesale rehash (everything is unkeyed: a determined
    // attacker CAN recompute; external anchoring is Story 2.5's job).
    static std::uint64_t SealHash(std::uint64_t count,
                                  std::uint64_t tip);
    std::uint64_t Seal() const {
        return SealHash(entries_.size(), Tip());
    }

    // Replays the chain: seq contiguity, prevHash linkage, stored-hash
    // recomputation, and the same leg/meta invariants the write path
    // enforces (a writer that bypasses Post still can't produce a
    // chain Verify accepts). Returns nullptr when clean, else a
    // static reason string for the first break.
    const char* Verify() const;

    // FNV-1a over prevHash (LE bytes) || canonical emit of the
    // content object {seq, credit, debit, memo, tags} — same hashing
    // convention as the battle record's integrity root.
    static std::uint64_t EntryHash(const LedgerEntry& e);

    Gameplay::Result<Gameplay::JsonValue> ToJson() const;
    static Gameplay::Result<Ledger> FromJson(const Gameplay::JsonValue& doc);

private:
    std::vector<LedgerEntry> entries_;
};

// Shared invariant used by both Post() and FromJson() — a persisted
// unbalanced entry cannot smuggle past a load.
const char* ValidateLegs(const Leg& credit, const Leg& debit);

} // namespace Potato::Campaign
