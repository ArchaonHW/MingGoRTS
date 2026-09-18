#include "Gameplay/Ledger.h"

#include "Serialization/JsonParser.h"

#include <cstdio>

namespace Potato {
namespace Gameplay {

const char* AccountName(LedgerAccount a) {
    switch (a) {
    case LedgerAccount::Martial: return "Martial";
    case LedgerAccount::Civil:   return "Civil";
    case LedgerAccount::Fate:    return "Fate";
    case LedgerAccount::Army:    return "Army";
    case LedgerAccount::Supply:  return "Supply";
    case LedgerAccount::Count:   break;
    }
    return "Unknown";
}

const char* AccountNameZh(LedgerAccount a) {
    switch (a) {
    case LedgerAccount::Martial: return "武功";
    case LedgerAccount::Civil:   return "民心";
    case LedgerAccount::Fate:    return "天命";
    case LedgerAccount::Army:    return "軍威";
    case LedgerAccount::Supply:  return "物資";
    case LedgerAccount::Count:   break;
    }
    return "不明";
}

bool AccountFromName(const std::string& name, LedgerAccount& out) {
    for (int i = 0; i < static_cast<int>(LedgerAccount::Count); ++i) {
        LedgerAccount a = static_cast<LedgerAccount>(i);
        if (name == AccountName(a)) {
            out = a;
            return true;
        }
    }
    return false;
}

LedgerEntry LedgerEntry::BattleVictory(int cost, int chapter,
                                       const std::string& memo) {
    LedgerEntry e;
    e.debit = LedgerAccount::Martial;
    e.credit = LedgerAccount::Supply;
    e.amount = cost;
    e.chapter = chapter;
    e.memo = memo;
    return e;
}

LedgerEntry LedgerEntry::Recruitment(int cost, int chapter,
                                     const std::string& memo) {
    LedgerEntry e;
    e.debit = LedgerAccount::Army;
    e.credit = LedgerAccount::Supply;
    e.amount = cost;
    e.chapter = chapter;
    e.memo = memo;
    return e;
}

LedgerEntry LedgerEntry::Negotiation(int gain, int chapter,
                                     const std::string& memo) {
    LedgerEntry e;
    e.debit = LedgerAccount::Civil;
    e.credit = LedgerAccount::Martial;
    e.amount = gain;
    e.chapter = chapter;
    e.memo = memo;
    return e;
}

// FNV-1a 64 位元雜湊
static uint64_t Fnv1a(const char* data, size_t len, uint64_t h) {
    for (size_t i = 0; i < len; ++i) {
        h ^= static_cast<unsigned char>(data[i]);
        h *= 0x100000001b3ULL;
    }
    return h;
}

static std::string Canon(const LedgerEntry& e) {
    // canonical 字串：欄位順序固定，雜湊輸入確定
    std::string s;
    s += AccountName(e.debit);
    s += '|';
    s += AccountName(e.credit);
    s += '|';
    s += std::to_string(e.amount);
    s += '|';
    s += std::to_string(e.chapter);
    s += '|';
    s += e.memo;
    return s;
}

uint64_t LedgerChain::HashEntry(uint64_t prevHash, const LedgerEntry& e) {
    uint64_t h = 0xcbf29ce484222325ULL;
    h = Fnv1a(reinterpret_cast<const char*>(&prevHash),
              sizeof(prevHash), h);
    const std::string s = Canon(e);
    return Fnv1a(s.data(), s.size(), h);
}

bool LedgerChain::Append(const LedgerEntry& e) {
    if (!e.WellFormed()) {
        return false;
    }
    Chained c;
    c.entry = e;
    c.prevHash = RootHash();
    c.hash = HashEntry(c.prevHash, e);
    entries.push_back(c);
    return true;
}

int LedgerChain::Verify() const {
    uint64_t prev = kGenesisHash;
    for (size_t i = 0; i < entries.size(); ++i) {
        const Chained& c = entries[i];
        if (c.prevHash != prev) {
            return static_cast<int>(i);
        }
        if (c.hash != HashEntry(prev, c.entry)) {
            return static_cast<int>(i);
        }
        prev = c.hash;
    }
    return -1;
}

int LedgerChain::SoundnessViolation() const {
    for (size_t i = 0; i < entries.size(); ++i) {
        if (!entries[i].entry.WellFormed()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

std::array<int, static_cast<size_t>(LedgerAccount::Count)>
LedgerChain::TrialBalance() const {
    std::array<int, static_cast<size_t>(LedgerAccount::Count)> net{};
    for (const auto& c : entries) {
        const LedgerEntry& e = c.entry;
        if (e.debit < LedgerAccount::Count) {
            net[static_cast<size_t>(e.debit)] += e.amount;
        }
        if (e.credit < LedgerAccount::Count) {
            net[static_cast<size_t>(e.credit)] -= e.amount;
        }
    }
    return net;
}

uint64_t LedgerChain::RootHash() const {
    return entries.empty() ? kGenesisHash : entries.back().hash;
}

std::string LedgerChain::ToJson() const {
    std::string out =
        "{\"schema\":\"potato.ledger_chain/1\",\"entries\":[";
    char buf[640];
    for (size_t i = 0; i < entries.size(); ++i) {
        const Chained& c = entries[i];
        std::snprintf(buf, sizeof(buf),
                      "{\"d\":\"%s\",\"c\":\"%s\",\"amt\":%d,\"ch\":%d,"
                      "\"memo\":\"%s\",\"prev\":\"%016llx\",\"hash\":\"%016llx\"}%s",
                      AccountName(c.entry.debit), AccountName(c.entry.credit),
                      c.entry.amount, c.entry.chapter,
                      c.entry.memo.c_str(),
                      (unsigned long long)c.prevHash,
                      (unsigned long long)c.hash,
                      i + 1 < entries.size() ? "," : "");
        out += buf;
    }
    out += "]}";
    return out;
}

static uint64_t ParseHex(const std::string& s) {
    uint64_t v = 0;
    for (char ch : s) {
        v <<= 4;
        if (ch >= '0' && ch <= '9') {
            v |= static_cast<uint64_t>(ch - '0');
        } else if (ch >= 'a' && ch <= 'f') {
            v |= static_cast<uint64_t>(ch - 'a' + 10);
        } else if (ch >= 'A' && ch <= 'F') {
            v |= static_cast<uint64_t>(ch - 'A' + 10);
        }
    }
    return v;
}

bool LedgerChain::FromJson(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) ||
        root["schema"].AsString() != "potato.ledger_chain/1") {
        return false;
    }
    entries.clear();
    for (const JsonValue& j : root["entries"].AsArray()) {
        Chained c;
        if (!AccountFromName(j["d"].AsString(), c.entry.debit) ||
            !AccountFromName(j["c"].AsString(), c.entry.credit)) {
            return false;
        }
        c.entry.amount = j["amt"].AsInt(0);
        c.entry.chapter = j["ch"].AsInt(0);
        c.entry.memo = j["memo"].AsString();
        c.prevHash = ParseHex(j["prev"].AsString());
        c.hash = ParseHex(j["hash"].AsString());
        entries.push_back(c);
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
