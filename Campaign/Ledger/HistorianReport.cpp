#include "Campaign/Ledger/HistorianReport.h"

namespace Potato::Campaign {

namespace {

bool Omitted(const LedgerEntry& e, const OmissionPolicy& p) {
    return (p.omitForged && e.provenance == Provenance::Forged) ||
           (p.omitSuspect && e.suspect);
}

void AppendUInt(std::string& s, std::uint64_t v) {
    char buf[24];
    int n = 0;
    do {
        buf[n++] = static_cast<char>('0' + (v % 10));
        v /= 10;
    } while (v > 0);
    while (n > 0) s += buf[--n];
}

} // namespace

HistorianReport RenderHistorianReport(const Ledger& l,
                                      OmissionPolicy policy) {
    HistorianReport r;
    r.audit.entries = l.Size();
    r.audit.breakReason = l.Verify();
    r.audit.chainOk = (r.audit.breakReason == nullptr);
    r.audit.tip = l.Tip();
    r.audit.seal = l.Seal();
    for (const LedgerEntry& e : l.Entries()) {
        if (e.provenance == Provenance::Forged) ++r.audit.forged;
        if (e.suspect) ++r.audit.suspect;
        for (const std::string& t : e.tags) {
            if (t == Ledger::TAG_ATROCITY) {
                ++r.audit.atrocities;
                break;
            }
        }
        if (Omitted(e, policy)) {
            ++r.omissions;
        } else {
            r.includedSeqs.push_back(e.seq);
        }
    }
    return r;
}

std::string HistorianReport::RenderText() const {
    std::string s;
    s += "entries: ";
    AppendUInt(s, audit.entries);
    s += "\nforged: ";
    AppendUInt(s, audit.forged);
    s += "\nsuspect: ";
    AppendUInt(s, audit.suspect);
    s += "\natrocities: ";
    AppendUInt(s, audit.atrocities);
    s += "\nchain: ";
    s += audit.chainOk ? "ok" : "BROKEN";
    if (audit.breakReason != nullptr) {
        s += " (";
        s += audit.breakReason;
        s += ")";
    }
    s += "\ntip: ";
    AppendUInt(s, audit.tip);
    s += "\nseal: ";
    AppendUInt(s, audit.seal);
    // 本報告省略 N 項 — epics.md's literal confession marker.
    s += "\n\xE6\x9C\xAC\xE5\xA0\x81\xE5\x91\x8A\xE7\x9C\x81\xE7\x95\xA5 ";
    AppendUInt(s, omissions);
    s += " \xE9\xA0\x85\n";
    return s;
}

} // namespace Potato::Campaign
