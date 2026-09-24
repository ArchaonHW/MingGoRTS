#include "PlayerDeck.h"

#include <cmath>

namespace Potato {
namespace Campaign {

const char* LootOutcomeName(PlayerDeck::LootOutcome o) {
    switch (o) {
    case PlayerDeck::LootOutcome::NotEligible: return "NotEligible";
    case PlayerDeck::LootOutcome::NoSignature: return "NoSignature";
    case PlayerDeck::LootOutcome::UnknownCard: return "UnknownCard";
    case PlayerDeck::LootOutcome::Added:       return "Added";
    case PlayerDeck::LootOutcome::Refined:     return "Refined";
    case PlayerDeck::LootOutcome::Converted:   return "Converted";
    }
    return "Unknown";
}

PlayerDeck::LootOutcome
PlayerDeck::LootSignature(const std::string& generalId,
                          const std::string& cardId,
                          Gameplay::GeneralDisposition disp,
                          int chapter,
                          const Gameplay::DoctrineLibrary& lib) {
    if (disp == Gameplay::GeneralDisposition::Unknown) {
        return LootOutcome::NotEligible; // 未決不掠奪，也不上帳
    }
    if (cardId.empty()) {
        return LootOutcome::NoSignature;
    }
    if (!lib.Find(cardId)) {
        return LootOutcome::UnknownCard; // 內容錯誤：帳外警告由呼叫端處理
    }

    auto it = owned.end();
    for (auto i = owned.begin(); i != owned.end(); ++i)
        if (i->id == cardId) { it = i; break; }

    LootOutcome out;
    if (it == owned.end()) {
        owned.push_back({cardId, 0});
        out = LootOutcome::Added;
    } else if (it->refine < kRefineCap) {
        ++it->refine;
        out = LootOutcome::Refined;
    } else {
        const auto* card = lib.Find(cardId);
        spoils += (card ? (std::max)(1, card->cost) : 1);
        out = LootOutcome::Converted;
    }
    journal.push_back({generalId, cardId, chapter, disp, out});
    return out;
}

bool PlayerDeck::Owns(const std::string& id) const {
    for (const Owned& o : owned)
        if (o.id == id) return true;
    return false;
}

int PlayerDeck::RefineLevel(const std::string& id) const {
    for (const Owned& o : owned)
        if (o.id == id) return o.refine;
    return 0;
}

std::set<std::string> PlayerDeck::Ids() const {
    std::set<std::string> out;
    for (const Owned& o : owned) out.insert(o.id);
    return out;
}

float PlayerDeck::TotalModifier(const Gameplay::DoctrineLibrary& lib,
                                const std::string& key) const {
    float total = 0.0f;
    for (const Owned& o : owned) {
        const auto* card = lib.Find(o.id);
        if (!card) continue;
        const float scale = 1.0f + kRefineBonus * o.refine;
        for (const auto& m : card->modifiers)
            if (m.key == key) total += m.value * scale;
    }
    return total;
}

void PlayerDeck::AppendOwnedRules(
    const Gameplay::DoctrineLibrary& lib,
    Gameplay::DoctrineSet& set) const {
    for (const Owned& o : owned) {
        const auto* card = lib.Find(o.id);
        if (card) card->AppendRules(set);
    }
}

JsonValue PlayerDeck::ToJson() const {
    JsonValue j;
    j.type = JsonValue::Type::Object;
    j.objectValue["schema"] =
        JsonValue::String("potato.player_deck/1");
    j.objectValue["spoils"] = JsonValue::Number(spoils);
    JsonValue cards;
    cards.type = JsonValue::Type::Array;
    for (const Owned& o : owned) {
        JsonValue c;
        c.type = JsonValue::Type::Object;
        c.objectValue["id"] = JsonValue::String(o.id);
        c.objectValue["refine"] = JsonValue::Number(o.refine);
        cards.arrayValue.push_back(c);
    }
    j.objectValue["cards"] = cards;
    JsonValue jl;
    jl.type = JsonValue::Type::Array;
    for (const LootEvent& e : journal) {
        JsonValue v;
        v.type = JsonValue::Type::Object;
        v.objectValue["general"] = JsonValue::String(e.generalId);
        v.objectValue["card"] = JsonValue::String(e.cardId);
        v.objectValue["chapter"] = JsonValue::Number(e.chapter);
        v.objectValue["disp"] =
            JsonValue::String(Gameplay::DispositionName(
                e.disposition));
        v.objectValue["outcome"] =
            JsonValue::String(LootOutcomeName(e.outcome));
        jl.arrayValue.push_back(v);
    }
    j.objectValue["journal"] = jl;
    return j;
}

bool PlayerDeck::FromJson(const JsonValue& j) {
    if (j.IsNull()) return true; // 缺段容忍：空牌庫
    if (!j.IsObject() ||
        j["schema"].AsString() != "potato.player_deck/1") {
        return false;
    }
    std::vector<Owned> newOwned;
    std::vector<LootEvent> newJournal;
    for (const JsonValue& c : j["cards"].AsArray()) {
        const std::string id = c["id"].AsString();
        if (id.empty()) return false; // 壞檔大聲拒絕
        const int refine = c["refine"].AsInt(0);
        if (refine < 0 || refine > kRefineCap) return false;
        newOwned.push_back({id, refine});
    }
    for (const JsonValue& v : j["journal"].AsArray()) {
        LootEvent e;
        e.generalId = v["general"].AsString();
        e.cardId = v["card"].AsString();
        e.chapter = v["chapter"].AsInt(0);
        if (!Gameplay::DispositionFromName(
                v["disp"].AsString(), e.disposition)) {
            return false;
        }
        const std::string o = v["outcome"].AsString();
        bool ok = false;
        for (LootOutcome cand :
             {LootOutcome::Added, LootOutcome::Refined,
              LootOutcome::Converted}) {
            if (o == LootOutcomeName(cand)) {
                e.outcome = cand;
                ok = true;
            }
        }
        if (!ok) return false;
        newJournal.push_back(e);
    }
    const int sp = j["spoils"].AsInt(0);
    if (sp < 0) return false;
    owned = std::move(newOwned);
    journal = std::move(newJournal);
    spoils = sp;
    return true;
}

} // namespace Campaign
} // namespace Potato
