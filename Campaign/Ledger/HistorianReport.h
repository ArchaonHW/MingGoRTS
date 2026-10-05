#pragma once

#include "Campaign/Ledger/Ledger.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace Potato::Campaign {

// The integrity surface of the ledger, folded for the report body.
struct AuditSegment {
    std::size_t entries = 0;
    std::size_t forged = 0;   // provenance == Forged
    std::size_t suspect = 0;  // suspect flag set
    // Entries carrying the "atrocity" tag (4.2). Counted ALWAYS —
    // the audit is the truth layer and doesn't take OmissionPolicy:
    // an atrocity the policy omits still shows here (and in the
    // omission counter). Cruelty can't hide in the numbers either way.
    std::size_t atrocities = 0;
    bool chainOk = false;     // Verify() == nullptr
    const char* breakReason = nullptr; // Verify()'s reason when broken
    std::uint64_t tip = 0;    // Ledger::Tip()
    std::uint64_t seal = 0;   // Ledger::Seal()
};

// Which entries stay out of the chronicle body. Omission is policy,
// not deletion — the entries remain in the append-only ledger; the
// report chooses what to narrate and confesses the count.
struct OmissionPolicy {
    bool omitForged = true;  // enemy-injected entries are not truth
    bool omitSuspect = true; // flagged entries are under question
};

// The rendered chapter chronicle's ledger section. `includedSeqs`
// are the entries the historian narrates; `omissions` is the
// confession counter — 本報告省略 N 項 (epics.md literal).
struct HistorianReport {
    AuditSegment audit;
    std::vector<std::uint64_t> includedSeqs; // ascending seq order
    std::size_t omissions = 0;

    // Deterministic integer-only text render (tooling/tests). The
    // confession line is the AC's literal marker; layout/styling is
    // Epic 8 presentation.
    std::string RenderText() const;
};

HistorianReport RenderHistorianReport(
    const Ledger& l, OmissionPolicy policy = {});

} // namespace Potato::Campaign
