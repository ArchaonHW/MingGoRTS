#include "Gameplay/Ledger.h"

#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cstdio>
#include <numeric>
#include <random>

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

const char* SourceName(EntrySource s) {
    switch (s) {
    case EntrySource::Unknown:     return "Unknown";
    case EntrySource::Battle:      return "Battle";
    case EntrySource::Refit:       return "Refit";
    case EntrySource::Negotiation: return "Negotiation";
    case EntrySource::Governance:  return "Governance";
    case EntrySource::Myth:        return "Myth";
    case EntrySource::System:      return "System";
    case EntrySource::Deterrence:  return "Deterrence";
    case EntrySource::Subversion:  return "Subversion";
    case EntrySource::Count:       break;
    }
    return "Unknown";
}

const char* SourceNameZh(EntrySource s) {
    switch (s) {
    case EntrySource::Unknown:     return "失考";
    case EntrySource::Battle:      return "戰陣";
    case EntrySource::Refit:       return "營務";
    case EntrySource::Negotiation: return "談判";
    case EntrySource::Governance:  return "治理";
    case EntrySource::Myth:        return "神異";
    case EntrySource::System:      return "紀要";
    case EntrySource::Deterrence:  return "嚇阻";
    case EntrySource::Subversion:  return "顛覆";
    case EntrySource::Count:       break;
    }
    return "失考";
}

bool SourceFromName(const std::string& name, EntrySource& out) {
    for (int i = 0; i < static_cast<int>(EntrySource::Count); ++i) {
        EntrySource s = static_cast<EntrySource>(i);
        if (name == SourceName(s)) {
            out = s;
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
    e.prov.source = EntrySource::Battle;
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
    e.prov.source = EntrySource::Refit;
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
    e.prov.source = EntrySource::Negotiation;
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
    // L-7：provenance 非預設才入 canon——舊檔 hash 不變（降級相容），
    // 新分錄的產生軌跡受鏈保護；對舊分錄補 prov 反而斷鏈（篡改揭露）
    if (!e.prov.IsDefault()) {
        s += '|';
        s += SourceName(e.prov.source);
        s += '|';
        s += std::to_string(e.prov.tick);
        s += '|';
        s += e.prov.eventId;
    }
    return s;
}

uint64_t LedgerHash(uint64_t prevHash, const std::string& text) {
    uint64_t h = 0xcbf29ce484222325ULL;
    h = Fnv1a(reinterpret_cast<const char*>(&prevHash),
              sizeof(prevHash), h);
    return Fnv1a(text.data(), text.size(), h);
}

uint64_t LedgerChain::HashEntry(uint64_t prevHash, const LedgerEntry& e) {
    return LedgerHash(prevHash, Canon(e));
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

int LedgerChain::InjectForgery(const LedgerEntry& e) {
    // 偽帳走雜湊鏈合法路徑但跳過 WellFormed 守衛——
    // 對手注入的分錄在鏈上成立、在帳上不成立
    Chained c;
    c.entry = e;
    c.prevHash = RootHash();
    c.hash = HashEntry(c.prevHash, e);
    entries.push_back(c);
    return static_cast<int>(entries.size() - 1);
}

bool LedgerChain::MarkSuspect(size_t idx) {
    if (idx >= entries.size() || IsSuspect(idx)) {
        return false;
    }
    auto it = std::lower_bound(suspects.begin(), suspects.end(), idx);
    suspects.insert(it, idx);
    return true;
}

bool LedgerChain::IsSuspect(size_t idx) const {
    return std::binary_search(suspects.begin(), suspects.end(), idx);
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

std::string LedgerChain::RegistryId() const {
    // L-6：RootHash 高低 32 位 XOR 折叠 → 8 位大寫 hex "XXXX-XXXX"。
    // 純函數於帳簿內容——篡改任一分錄（含 prov）末端 hash 變、編號即變
    const uint64_t root = RootHash();
    const uint32_t folded = static_cast<uint32_t>(root >> 32) ^
                            static_cast<uint32_t>(root);
    char buf[12];
    std::snprintf(buf, sizeof(buf), "%04X-%04X",
                  (folded >> 16) & 0xFFFFu, folded & 0xFFFFu);
    return buf;
}

std::string LedgerChain::ToJson() const {
    // L-6 登錄編號：可選附加欄位——讀者可忽略（編號恆可重算），
    // 寫出供玩家直接引用；FromJson 不強制驗（Verify() 才是防線）。
    // 置於 entries 前，保持「entries 為末尾陣列」的既有書寫慣例
    std::string out =
        "{\"schema\":\"potato.ledger_chain/1\",\"registryId\":\"" +
        RegistryId() + "\",\"entries\":[";
    char buf[640];
    for (size_t i = 0; i < entries.size(); ++i) {
        const Chained& c = entries[i];
        std::snprintf(buf, sizeof(buf),
                      "{\"d\":\"%s\",\"c\":\"%s\",\"amt\":%d,\"ch\":%d,"
                      "\"memo\":\"%s\",",
                      AccountName(c.entry.debit), AccountName(c.entry.credit),
                      c.entry.amount, c.entry.chapter,
                      c.entry.memo.c_str());
        out += buf;
        // L-7：prov 為可選附加欄位——預設值不寫出，舊讀者可忽略
        if (!c.entry.prov.IsDefault()) {
            std::snprintf(buf, sizeof(buf),
                          "\"prov\":{\"src\":\"%s\",\"t\":%d,\"ev\":\"%s\"},",
                          SourceName(c.entry.prov.source), c.entry.prov.tick,
                          c.entry.prov.eventId.c_str());
            out += buf;
        }
        std::snprintf(buf, sizeof(buf),
                      "\"prev\":\"%016llx\",\"hash\":\"%016llx\"}%s",
                      (unsigned long long)c.prevHash,
                      (unsigned long long)c.hash,
                      i + 1 < entries.size() ? "," : "");
        out += buf;
    }
    out += ']';
    if (!suspects.empty()) {
        out += ",\"suspect\":[";
        for (size_t i = 0; i < suspects.size(); ++i) {
            char sbuf[24];
            std::snprintf(sbuf, sizeof(sbuf), "%zu%s", suspects[i],
                          i + 1 < suspects.size() ? "," : "");
            out += sbuf;
        }
        out += ']';
    }
    out += '}';
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
        // L-7：prov 可選——舊檔無欄位 → 預設軌跡（失考），hash 照舊驗
        const JsonValue& pv = j["prov"];
        if (pv.IsObject()) {
            EntrySource src = EntrySource::Unknown;
            SourceFromName(pv["src"].AsString(), src);
            c.entry.prov.source = src;
            c.entry.prov.tick = pv["t"].AsInt(0);
            c.entry.prov.eventId = pv["ev"].AsString();
        }
        c.prevHash = ParseHex(j["prev"].AsString());
        c.hash = ParseHex(j["hash"].AsString());
        entries.push_back(c);
    }
    suspects.clear();
    for (const JsonValue& s : root["suspect"].AsArray()) {
        const int idx = s.AsInt(-1);
        if (idx >= 0) {
            MarkSuspect(static_cast<size_t>(idx));
        }
    }
    return true;
}

LedgerAssurance AssureLedger(const LedgerChain& ledger, uint64_t seed,
                             int sampleN) {
    LedgerAssurance r;
    r.brokenAt = ledger.Verify();
    r.unsoundAt = ledger.SoundnessViolation();
    r.suspectCount = static_cast<int>(ledger.SuspectCount());

    const size_t n = ledger.Size();
    if (seed == 0) {
        seed = ledger.RootHash(); // 自引種：同帳簿恆同樣本
    }
    if (sampleN > 0 && n > 0) {
        const auto& es = ledger.Entries();
        const int k = std::min<int>(sampleN, static_cast<int>(n));
        r.sampleSize = k;
        // Fisher-Yates 部分洗牌取前 k 個互異索引——
        // mt19937_64 為標準化引擎、modulo 取樣不依賴
        // uniform_int_distribution（其結果隨標準庫實作而異），
        // 同 seed 跨 MSVC/MinGW/g++ 產同一樣本
        std::vector<size_t> idx(n);
        std::iota(idx.begin(), idx.end(), size_t{0});
        std::mt19937_64 rng(seed);
        for (int i = 0; i < k; ++i) {
            const size_t j = i + static_cast<size_t>(rng() % (n - i));
            std::swap(idx[static_cast<size_t>(i)], idx[j]);
        }
        for (int i = 0; i < k; ++i) {
            const size_t at = idx[static_cast<size_t>(i)];
            const LedgerChain::Chained& c = es[at];
            const uint64_t expectPrev =
                at == 0 ? LedgerChain::kGenesisHash
                        : es[at - 1].hash;
            if (c.prevHash != expectPrev ||
                LedgerChain::HashEntry(c.prevHash, c.entry) != c.hash) {
                r.sampleBad = static_cast<int>(at);
                break;
            }
            ++r.sampled;
        }
    }

    if (r.brokenAt >= 0 || r.sampleBad >= 0) {
        r.verdict = AssuranceVerdict::Adverse;
    } else if (r.unsoundAt >= 0 || r.suspectCount > 0) {
        r.verdict = AssuranceVerdict::Qualified;
    }
    return r;
}

} // namespace Gameplay
} // namespace Potato
