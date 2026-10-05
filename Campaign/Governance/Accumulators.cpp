#include "Campaign/Governance/Accumulators.h"

#include <string>

namespace Potato::Campaign {

namespace {

// int64 add with saturation — governance tags are fold inputs, not
// posting legs, so a hostile/malformed magnitude clamps at the bound
// instead of overflowing into UB.
std::int64_t SaturatingAdd(std::int64_t a, std::int64_t b) {
    if (b > 0 && a > INT64_MAX - b) return INT64_MAX;
    if (b < 0 && a < INT64_MIN - b) return INT64_MIN;
    return a + b;
}

} // namespace

bool ParseGovernanceTag(std::string_view tag, std::string_view axis,
                        std::int64_t& out) {
    // "<axis>:<signed int64>" — strict: non-empty axis match, ':'
    // separator, optional sign, digits only, magnitude capped.
    if (axis.empty() || tag.size() <= axis.size() + 1) return false;
    if (tag.compare(0, axis.size(), axis) != 0) return false;
    if (tag[axis.size()] != ':') return false;
    std::string_view num = tag.substr(axis.size() + 1);
    bool neg = false;
    if (num.front() == '+' || num.front() == '-') {
        neg = num.front() == '-';
        num.remove_prefix(1);
    }
    if (num.empty()) return false;
    std::int64_t v = 0;
    for (const char c : num) {
        if (c < '0' || c > '9') return false;
        const int d = c - '0';
        if (v > (MAX_GOVERNANCE_DELTA - d) / 10) return false;
        v = v * 10 + d;
    }
    out = neg ? -v : v;
    return true;
}

GovernanceAccumulators FoldGovernance(const Ledger& l) {
    GovernanceAccumulators a;
    // 民心 is a real account: net postings, one fold.
    a.popularSupport = l.Balance(Account::PopularSupport);
    // 墮落 is a ratchet: the corruption fold's running MAXIMUM.
    // Entries may carry corruption:-N (penance) and drag the running
    // sum — the accumulator keeps the peak. Monotonic over any
    // append-only history by construction.
    std::int64_t corruptionSum = 0;
    for (const LedgerEntry& e : l.Entries()) {
        // Ledger::ValidateMeta dedupes tags byte-exactly — but
        // "order:+5"/"order:05"/"order:+05" are distinct strings
        // folding to one value. Dedupe per (axis, value) within the
        // entry so spelling variants can't amplify a contribution;
        // two distinct values on one axis still stack.
        std::int64_t seen[2][Ledger::MAX_TAGS];
        std::size_t nSeen[2] = {0, 0};
        for (const std::string& t : e.tags) {
            for (int axis = 0; axis < 2; ++axis) {
                std::int64_t v = 0;
                if (!ParseGovernanceTag(
                        t, axis == 0 ? "order" : "corruption", v)) {
                    continue;
                }
                bool dup = false;
                for (std::size_t i = 0; i < nSeen[axis]; ++i) {
                    if (seen[axis][i] == v) { dup = true; break; }
                }
                if (dup) break;
                seen[axis][nSeen[axis]++] = v;
                if (axis == 0) {
                    a.order = SaturatingAdd(a.order, v);
                } else {
                    corruptionSum = SaturatingAdd(corruptionSum, v);
                    if (corruptionSum > a.corruption) {
                        a.corruption = corruptionSum;
                    }
                }
                break; // a tag matches at most one axis
            }
        }
    }
    return a;
}

} // namespace Potato::Campaign
