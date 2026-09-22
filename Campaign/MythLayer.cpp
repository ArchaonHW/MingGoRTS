#include "Campaign/MythLayer.h"

#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace Potato {
namespace Campaign {

const char* SeepageName(Seepage s) {
    switch (s) {
    case Seepage::Quiet:     return "平靜";
    case Seepage::Anomalies: return "異象";
    case Seepage::Seep:      return "滲透";
    case Seepage::Manifest:  return "降臨";
    }
    return "未知";
}

// 治理事件 → 區域壓力 delta：暴行/焚村推進滲透，仁政類降壓
// （壓力可降、等級不降——ratchet 在 Ratchet 保證）
static float PressureOf(Gameplay::GovernanceEvent ev) {
    switch (ev) {
    case Gameplay::GovernanceEvent::Atrocity:          return 10.0f;
    case Gameplay::GovernanceEvent::VillageBurned:     return 6.0f;
    case Gameplay::GovernanceEvent::ConvoyLost:        return 3.0f;
    case Gameplay::GovernanceEvent::ConvoyRaided:      return 2.0f;
    case Gameplay::GovernanceEvent::VillageOccupied:   return -2.0f;
    case Gameplay::GovernanceEvent::SurrenderAccepted: return -3.0f;
    case Gameplay::GovernanceEvent::ConvoyProtected:   return -2.0f;
    }
    return 0.0f;
}

void MythLayer::Feed(const std::string& region,
                     Gameplay::GovernanceEvent ev) {
    Region& r = regions[region]; // 未知區域自動建檔 Quiet
    float delta = PressureOf(ev);
    // 怒神加成：綁定守護靈且 favor 過低時，暴行類壓力放大
    const bool atrocity =
        (ev == Gameplay::GovernanceEvent::Atrocity ||
         ev == Gameplay::GovernanceEvent::VillageBurned);
    if (atrocity && !r.spirit.empty() && Favor(r.spirit) < kAngryFavor) {
        delta *= kAngerBoost;
    }
    r.pressure = (std::max)(0.0f, r.pressure + delta);
    std::vector<Gameplay::MythEvent> pending;
    Ratchet(region, r, pending);
    Dispatch(pending);
}

void MythLayer::Ratchet(const std::string& region, Region& r,
                        std::vector<Gameplay::MythEvent>& out) {
    static constexpr float kGate[] = {kAnomalies, kSeep, kManifest};
    // 單次大壓力可連跨多階——每階各錄一筆轉換；
    // 事件只收集不即發：回呼若在變異途中再入層會 rehash 懸空引用
    while (r.level < Seepage::Manifest) {
        const int next = static_cast<int>(r.level) + 1;
        if (r.pressure < kGate[next - 1]) {
            break;
        }
        const Seepage from = r.level;
        r.level = static_cast<Seepage>(next);
        transitions.push_back({region, from, r.level, chapter});
        Gameplay::MythEvent ev;
        ev.shrine = region;
        ev.spirit = r.spirit.empty() ? "境靈" : r.spirit;
        char when[32];
        std::snprintf(when, sizeof(when), "第%d章", chapter);
        ev.when = when;
        ev.detail = std::string("滲透升階：") + SeepageName(from) +
                    "→" + SeepageName(r.level);
        out.push_back(std::move(ev));
    }
}

// 事件在全部變異完成後統一派出——此時無活的 Region&/迭代器，
// 回呼再入層（Feed/BindSpirit）也不會懸空
void MythLayer::Dispatch(
    const std::vector<Gameplay::MythEvent>& events) {
    if (!onEvent) {
        return;
    }
    for (const auto& ev : events) {
        onEvent(ev);
    }
}

void MythLayer::BindSpirit(const std::string& region,
                           const std::string& spirit) {
    regions[region].spirit = spirit;
    favor.try_emplace(spirit, kNeutralFavor);
}

void MythLayer::AdjustFavor(const std::string& spirit, float delta) {
    // 未登錄 spirit 以中立值起算——與 Favor() 的讀端預設對稱
    float& f = favor.try_emplace(spirit, kNeutralFavor).first->second;
    if (std::isfinite(delta)) {
        f = std::clamp(f + delta, 0.0f, 100.0f);
    }
}

float MythLayer::Favor(const std::string& spirit) const {
    const auto it = favor.find(spirit);
    return it != favor.end() ? it->second : kNeutralFavor;
}

bool MythLayer::HasSpirit(const std::string& spirit) const {
    return favor.find(spirit) != favor.end();
}

std::string MythLayer::SpiritOf(const std::string& region) const {
    const auto it = regions.find(region);
    return it != regions.end() ? it->second.spirit : std::string{};
}

Seepage MythLayer::Level(const std::string& region) const {
    const auto it = regions.find(region);
    return it != regions.end() ? it->second.level : Seepage::Quiet;
}

std::unordered_map<std::string, Seepage> MythLayer::Levels() const {
    std::unordered_map<std::string, Seepage> out;
    out.reserve(regions.size());
    for (const auto& [id, r] : regions) {
        out[id] = r.level;
    }
    return out;
}

float MythLayer::Pressure(const std::string& region) const {
    const auto it = regions.find(region);
    return it != regions.end() ? it->second.pressure : 0.0f;
}

void MythLayer::DeriveFrom(float depravity, float civilOrder,
                           int unrestLevel, int ch) {
    chapter = ch;
    // 治理快照 → 注入壓力：墮落與動亂餵養滲透；秩序崩壞另計
    float inject = depravity * kDepravityRate +
                   static_cast<float>(unrestLevel) * kUnrestRate;
    if (civilOrder < kOrderWarn) {
        inject += (kOrderWarn - civilOrder) * kOrderRate;
    }
    if (!std::isfinite(inject) || inject <= 0.0f) {
        return; // 清廉治理（或異常輸入）：不注入也不回落
    }
    std::vector<Gameplay::MythEvent> pending;
    // 排序迭代：unordered_map 順序不確定，pending 事件序（→ MythLog
    // 記事序）會逐次漂移——排序鍵讓同狀態同輸出（record-is-truth）
    std::vector<std::string> keys;
    keys.reserve(regions.size());
    for (const auto& [id, r] : regions) keys.push_back(id);
    std::sort(keys.begin(), keys.end());
    for (const std::string& id : keys) {
        Region& r = regions[id];
        r.pressure += inject;
        Ratchet(id, r, pending);
    }
    Dispatch(pending);
}

JsonValue MythLayer::ToJson() const {
    JsonValue o;
    o.type = JsonValue::Type::Object;
    o.objectValue["chapter"] = JsonValue::Number(chapter);

    JsonValue regs;
    regs.type = JsonValue::Type::Array;
    // 排序輸出：unordered_map 迭代序不確定，存檔位元組會逐次漂移
    {
        std::vector<std::string> keys;
        keys.reserve(regions.size());
        for (const auto& [id, r] : regions) keys.push_back(id);
        std::sort(keys.begin(), keys.end());
        for (const std::string& id : keys) {
            const Region& r = regions.at(id);
            JsonValue e;
            e.type = JsonValue::Type::Object;
            e.objectValue["id"] = JsonValue::String(id);
            e.objectValue["lv"] =
                JsonValue::Number(static_cast<int>(r.level));
            e.objectValue["p"] = JsonValue::Number(r.pressure);
            if (!r.spirit.empty()) {
                e.objectValue["sp"] = JsonValue::String(r.spirit);
            }
            regs.arrayValue.push_back(e);
        }
    }
    o.objectValue["regions"] = regs;

    // favor 是 object——WriteJson 的物件鍵已排序輸出,此處不需排序
    JsonValue fav;
    fav.type = JsonValue::Type::Object;
    for (const auto& [id, v] : favor) {
        fav.objectValue[id] = JsonValue::Number(v);
    }
    o.objectValue["favor"] = fav;

    JsonValue tr;
    tr.type = JsonValue::Type::Array;
    for (const auto& t : transitions) {
        JsonValue e;
        e.type = JsonValue::Type::Object;
        e.objectValue["r"] = JsonValue::String(t.region);
        e.objectValue["from"] = JsonValue::Number(static_cast<int>(t.from));
        e.objectValue["to"] = JsonValue::Number(static_cast<int>(t.to));
        e.objectValue["ch"] = JsonValue::Number(t.chapter);
        tr.arrayValue.push_back(e);
    }
    o.objectValue["trans"] = tr;
    return o;
}

bool MythLayer::FromJson(const JsonValue& j) {
    if (j.IsNull()) {
        regions.clear(); // 缺段降級：空層（清掉殘態，語義一致）
        favor.clear();
        transitions.clear();
        chapter = 1;
        return true;
    }
    if (!j.IsObject()) {
        return false;
    }
    // 段內欄位：缺則容忍（舊檔），在則必須型別正確——壞段拒絕
    const JsonValue& rj = j["regions"];
    const JsonValue& fj = j["favor"];
    const JsonValue& tj = j["trans"];
    if ((!rj.IsNull() && !rj.IsArray()) ||
        (!fj.IsNull() && !fj.IsObject()) ||
        (!tj.IsNull() && !tj.IsArray())) {
        return false;
    }
    std::unordered_map<std::string, Region> newRegions;
    for (const JsonValue& e : rj.AsArray()) {
        Region r;
        const std::string id = e["id"].AsString();
        r.level = static_cast<Seepage>(
            std::clamp(e["lv"].AsInt(0), 0, 3));
        // 非有限/負壓力一律歸零——否則 %g 寫出 nan/inf 後 parser 讀不回，
        // 整個戰役存檔永久磚掉
        const float p = e["p"].AsFloat(0.0f);
        r.pressure = std::isfinite(p) ? (std::max)(0.0f, p) : 0.0f;
        r.spirit = e["sp"].AsString();
        newRegions[id] = r;
    }
    std::unordered_map<std::string, float> newFavor;
    if (fj.IsObject()) {
        for (const auto& [id, v] : fj.objectValue) {
            const float f = v.AsFloat(kNeutralFavor);
            newFavor[id] = std::isfinite(f)
                ? std::clamp(f, 0.0f, 100.0f) : kNeutralFavor;
        }
    }
    std::vector<SeepageTransition> newTrans;
    for (const JsonValue& e : tj.AsArray()) {
        SeepageTransition t;
        t.region = e["r"].AsString();
        t.from = static_cast<Seepage>(
            std::clamp(e["from"].AsInt(0), 0, 3));
        t.to = static_cast<Seepage>(
            std::clamp(e["to"].AsInt(0), 0, 3));
        t.chapter = e["ch"].AsInt(1);
        newTrans.push_back(t);
    }
    regions = std::move(newRegions);
    favor = std::move(newFavor);
    transitions = std::move(newTrans);
    chapter = j["chapter"].AsInt(1);
    return true;
}

} // namespace Campaign
} // namespace Potato
