#include "QuantumFog.h"

#include "BattleResources.h"
#include "MathUtils/QuasiRandom.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

QuantumFog::QuantumFog(float duration, int cost, int probe)
    : intelDuration(duration), intelCost(std::max(0, cost)),
      probeCost(std::max(0, probe)) { // 負費用會反過來產出情報,clamp 掉
}

int QuantumFog::AddEntity(const std::string& name, int team,
                          const std::vector<Vector2>& candidates,
                          const std::vector<double>& priors) {
    if (candidates.size() < 2) return -1;

    UncertainEntity e(static_cast<int>(candidates.size()), seedCounter++);
    e.name = name;
    e.team = team;
    e.candidates = candidates;

    if (priors.size() == candidates.size()) {
        e.priors = priors;
        e.state.SetProbabilities(priors);
    } else {
        e.priors.assign(candidates.size(), 1.0 / candidates.size());
        e.state.SetUniform();
    }

    entities.push_back(std::move(e));
    return static_cast<int>(entities.size()) - 1;
}

int QuantumFog::AddEntityCloud(const std::string& name, int team,
                               Vector2 suspectedCenter, float radius,
                               int count, float minSpacing,
                               const Vector2* biasPoint, float priorScale) {
    if (radius <= 0.0f || count < 2) return -1;

    const uint64_t seed = seedCounter * 2654435761ULL;
    std::vector<Vector2> candidates = Quasi::PoissonDisk(
        suspectedCenter, radius, count, minSpacing, seed);

    // 飽和退縮：Poisson 產生不足時以 R2 圓盤序列補滿
    if (static_cast<int>(candidates.size()) < 2 ||
        static_cast<int>(candidates.size()) < count / 2) {
        candidates.clear();
        for (int i = 0; i < count; ++i) {
            candidates.push_back(
                suspectedCenter +
                Quasi::R2InDisk(static_cast<uint64_t>(i), radius, seed));
        }
    }

    // 先驗：依與 bias（預設 = suspectedCenter）的距離高斯遞減
    const Vector2 bias = biasPoint ? *biasPoint : suspectedCenter;
    const double sigma =
        static_cast<double>(radius) * 0.5 *
        static_cast<double>(priorScale > 0.0f ? priorScale : 1.0f);
    const double invTwoSigmaSq = 1.0 / (2.0 * sigma * sigma);
    std::vector<double> priors(candidates.size());
    double total = 0.0;
    for (size_t i = 0; i < candidates.size(); ++i) {
        const Vector2 d = candidates[i] - bias;
        const double w = std::exp(-(d.x * d.x + d.y * d.y) * invTwoSigmaSq);
        priors[i] = w;
        total += w;
    }
    if (!(total > 0.0)) return -1; // !(>0) 同時擋掉 NaN/0/負值
    for (double& p : priors) p /= total;

    return AddEntity(name, team, candidates, priors);
}

bool QuantumFog::Observe(int entityId, const Vector2& truePos) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return false;
    }
    UncertainEntity& e = entities[entityId];
    if (e.revealed) {
        // Q-7 相位干涉:同 tick(fogTime 未推進)二次觀測 → 相位差決定
        // 建設性(時效加成)或破壞性(觀測被拒)
        if (interference && !e.phases.empty() &&
            e.lastObserveAt == fogTime) {
            const int tgt = NearestCandidate(e, truePos);
            const int modal = ModalCandidate(e);
            const double c =
                std::cos(e.phases[tgt] - e.phases[modal]);
            e.lastObserveAt = fogTime;
            if (c < 0.0) {
                Emit("fog:interference #" + std::to_string(entityId) +
                     " " + e.name + " 破壞性干涉,觀測被拒");
                return false;
            }
            // 建設性:情報穿透——刷新時效再 +50%
            e.revealedPos = truePos;
            e.revealTimer = intelDuration * 1.5f;
            Emit("fog:interference #" + std::to_string(entityId) + " " +
                 e.name + " 建設性干涉,時效加成");
            return true;
        }
        // 已揭露不重複扣點,但刷新真值與時效
        e.revealedPos = truePos;
        e.revealTimer = intelDuration;
        e.lastObserveAt = fogTime;
        return true;
    }

    // 情報點 = 觀測成本；資源不足則觀測失敗（態不塌縮）
    if (resources && !resources->SpendIntel(e.team, intelCost)) {
        return false;
    }

    const int hit = CollapseNear(e, truePos);
    e.lastObserveAt = fogTime;
    Emit("fog:observe #" + std::to_string(entityId) + " " + e.name +
         " → 候選" + std::to_string(hit));
    PropagateEntanglement(entityId, hit);
    return true;
}

bool QuantumFog::Reveal(int entityId, const Vector2& truePos) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return false;
    }
    UncertainEntity& e = entities[entityId];
    if (e.revealed) { // 已揭露僅刷新真值/時效——糾纏只在塌縮轉換時傳遞
        e.revealedPos = truePos;
        e.revealTimer = intelDuration;
        return true;
    }
    const int hit = CollapseNear(e, truePos);
    Emit("fog:reveal #" + std::to_string(entityId) + " " + e.name +
         " → 候選" + std::to_string(hit));
    PropagateEntanglement(entityId, hit);
    return true;
}

bool QuantumFog::EliminateCandidate(int entityId, int candidateIndex) {
    UncertainEntity* e =
        (entityId >= 0 && entityId < static_cast<int>(entities.size()))
            ? &entities[entityId]
            : nullptr;
    if (!e || e->revealed || candidateIndex < 0 ||
        candidateIndex >= static_cast<int>(e->candidates.size())) {
        return false;
    }
    std::vector<double> probs = e->state.Probabilities();
    if (probs[candidateIndex] <= 1e-9) return false;

    // 真值必在雲中——不得消到一個候選都不剩
    int nonzero = 0;
    for (double p : probs) {
        if (p > 1e-9) ++nonzero;
    }
    if (nonzero <= 1) return false;

    probs[candidateIndex] = 0.0;
    e->priors[candidateIndex] = 0.0; // 過期重建時同樣排除
    e->state.SetProbabilities(probs);
    Emit("fog:eliminate #" + std::to_string(entityId) + " " + e->name +
         " 消去候選" + std::to_string(candidateIndex));
    return true;
}

bool QuantumFog::Probe(int entityId, const Vector2& truePos, float strength) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size()) ||
        !(strength > 0.0f)) { // !(>0) 同時擋掉 NaN/0/負值
        return false;
    }
    UncertainEntity& e = entities[entityId];
    if (e.revealed) return true; // 已揭露不需探測,也不扣點

    // 軟重加權:候選愈近真值權重愈高,strength 控制收縮幅度;
    // 只乘係數不歸零——probe 是軟情報,不做硬排除
    const float s = std::min(strength, 1.0f);
    std::vector<double> probs = e.state.Probabilities();
    std::vector<double> w(probs.size());
    double wMax = 0.0;
    for (size_t i = 0; i < probs.size(); ++i) {
        const Vector2 d = e.candidates[i] - truePos;
        w[i] = 1.0 / (1.0 + d.x * d.x + d.y * d.y);
        wMax = std::max(wMax, w[i]);
    }
    if (wMax <= 0.0) return false; // 防護:全零權重,不扣點不動雲
    double total = 0.0;
    for (size_t i = 0; i < probs.size(); ++i) {
        probs[i] *= (1.0 - s) + s * (w[i] / wMax);
        total += probs[i];
    }
    if (total <= 0.0) return false; // 不歸一化防護:不重置為均勻

    if (resources && !resources->SpendIntel(e.team, probeCost)) {
        return false;
    }
    e.state.SetProbabilities(probs);
    Emit("fog:probe #" + std::to_string(entityId) + " " + e.name +
         " 雲收縮");
    return true;
}

int QuantumFog::NearestCandidate(const UncertainEntity& e,
                                 const Vector2& pos) const {
    int best = 0;
    float bestDist = 1e30f;
    for (int i = 0; i < static_cast<int>(e.candidates.size()); ++i) {
        const Vector2 d = e.candidates[i] - pos;
        const float dist = d.x * d.x + d.y * d.y;
        if (dist < bestDist) { bestDist = dist; best = i; }
    }
    return best;
}

int QuantumFog::ModalCandidate(const UncertainEntity& e) const {
    const std::vector<double> probs = e.state.Probabilities();
    int best = 0;
    for (int i = 1; i < static_cast<int>(probs.size()); ++i) {
        if (probs[i] > probs[best]) best = i;
    }
    return best;
}

bool QuantumFog::SetEntityPhases(int entityId,
                                 const std::vector<double>& phases) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return false;
    }
    UncertainEntity& e = entities[entityId];
    e.phases.assign(e.candidates.size(), 0.0);
    for (size_t i = 0; i < phases.size() && i < e.phases.size(); ++i) {
        e.phases[i] = phases[i];
    }
    e.state.SetPhases(e.phases); // 振幅帶相位——decoherence 會沖掉
    return true;
}

int QuantumFog::CollapseNear(UncertainEntity& e, const Vector2& truePos) {
    // 塌縮到距真值最近的候選態
    const int best = NearestCandidate(e, truePos);
    e.state.CollapseTo(best);
    e.revealed = true;
    e.revealedPos = truePos;
    e.revealTimer = intelDuration;
    return best;
}

bool QuantumFog::Entangle(int a, int b) {
    if (a == b || !GetEntity(a) || !GetEntity(b)) return false;
    if (EntangledPartner(a) >= 0 || EntangledPartner(b) >= 0) return false;

    // 同向機動假設:以雲質心為原點的相對偏移最近者即相關候選
    // (佯攻偏其雲心北側 → 主力也偏其雲心北側)
    auto centroid = [](const std::vector<Vector2>& c) {
        Vector2 m(0.0f, 0.0f);
        for (const Vector2& p : c) m = m + p;
        return m * (1.0f / static_cast<float>(c.size()));
    };
    auto corr = [](const std::vector<Vector2>& from, const Vector2& cf,
                   const std::vector<Vector2>& to, const Vector2& ct) {
        std::vector<int> map(from.size());
        for (size_t i = 0; i < from.size(); ++i) {
            const Vector2 rel = from[i] - cf;
            int best = 0;
            float bestDist = 1e30f;
            for (size_t j = 0; j < to.size(); ++j) {
                const Vector2 d = (to[j] - ct) - rel;
                const float dist = d.x * d.x + d.y * d.y;
                if (dist < bestDist) { bestDist = dist; best = (int)j; }
            }
            map[i] = best;
        }
        return map;
    };

    const auto& ea = entities[a];
    const auto& eb = entities[b];
    const Vector2 ca = centroid(ea.candidates);
    const Vector2 cb = centroid(eb.candidates);
    EntangleLink link;
    link.a = a;
    link.b = b;
    link.a2b = corr(ea.candidates, ca, eb.candidates, cb);
    link.b2a = corr(eb.candidates, cb, ea.candidates, ca);
    links.push_back(std::move(link));
    return true;
}

int QuantumFog::EntangledPartner(int entityId) const {
    for (const auto& l : links) {
        if (l.a == entityId) return l.b;
        if (l.b == entityId) return l.a;
    }
    return -1;
}

void QuantumFog::PropagateEntanglement(int entityId, int candIdx) {
    if (candIdx < 0) return;
    for (const auto& l : links) {
        int partnerId = -1;
        int corrIdx = -1;
        if (l.a == entityId && candIdx < (int)l.a2b.size()) {
            partnerId = l.b;
            corrIdx = l.a2b[candIdx];
        } else if (l.b == entityId && candIdx < (int)l.b2a.size()) {
            partnerId = l.a;
            corrIdx = l.b2a[candIdx];
        } else {
            continue;
        }
        UncertainEntity& p = entities[partnerId];
        if (p.revealed || corrIdx < 0 ||
            corrIdx >= (int)p.candidates.size()) {
            continue;
        }
        std::vector<double> probs = p.state.Probabilities();
        if (probs[corrIdx] <= 1e-9) continue; // 已消去的候選不復活
        // blend 0.7:強烈集中但保留殘餘不確定——糾纏是推測非親見
        for (size_t j = 0; j < probs.size(); ++j) {
            probs[j] *= 0.3;
        }
        probs[corrIdx] += 0.7;
        p.state.SetProbabilities(probs);
        Emit("fog:entangle #" + std::to_string(entityId) + " → #" +
             std::to_string(partnerId) + " " + p.name + " 雲集中");
    }
}

int QuantumFog::ObserveRandom(int entityId) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return -1;
    }
    UncertainEntity& e = entities[entityId];
    if (resources && !resources->SpendIntel(e.team, intelCost)) {
        return -1;
    }
    const bool was = e.revealed;
    const int outcome = e.state.Measure();
    e.revealed = true;
    e.revealedPos = e.candidates[outcome];
    e.revealTimer = intelDuration;
    if (!was) {
        Emit("fog:observe #" + std::to_string(entityId) + " " + e.name +
             " → 候選" + std::to_string(outcome));
        PropagateEntanglement(entityId, outcome);
    }
    return outcome;
}

void QuantumFog::Update(float dt) {
    fogTime += dt; // 時鐘推進 → 跨 tick 的觀測不再構成「同刻雙測」
    for (auto& e : entities) {
        if (e.revealed) {
            if (intelDuration > 0.0f) {
                e.revealTimer -= dt;
                if (e.revealTimer <= 0.0f) {
                    // 情報過期：回到疊加態（以先驗分佈重建）
                    e.revealed = false;
                    e.state.SetProbabilities(e.priors);
                    Emit("fog:expire " + e.name + " 情報過期回雲");
                }
            }
        } else {
            // 未觀測態緩慢退相干：機率雲隨時間向均勻擴散
            e.state.Diffuse(static_cast<double>(dt) * 0.02);
        }
    }
}

bool QuantumFog::IsRevealed(int entityId) const {
    const UncertainEntity* e = GetEntity(entityId);
    return e && e->revealed;
}

Vector2 QuantumFog::GetRevealedPos(int entityId) const {
    const UncertainEntity* e = GetEntity(entityId);
    return (e && e->revealed) ? e->revealedPos : Vector2(0.0f, 0.0f);
}

std::vector<std::pair<Vector2, double>> QuantumFog::GetCloud(int entityId) const {
    std::vector<std::pair<Vector2, double>> cloud;
    const UncertainEntity* e = GetEntity(entityId);
    if (!e) return cloud;

    if (e->revealed) {
        cloud.emplace_back(e->revealedPos, 1.0);
        return cloud;
    }
    const std::vector<double> probs = e->state.Probabilities();
    for (size_t i = 0; i < e->candidates.size(); ++i) {
        if (probs[i] > 1e-4) {
            cloud.emplace_back(e->candidates[i], probs[i]);
        }
    }
    return cloud;
}

const UncertainEntity* QuantumFog::GetEntity(int entityId) const {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return nullptr;
    }
    return &entities[entityId];
}

// ---- Q-6 疊加態存檔 ----

static std::string FogEscapeJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\t': out += "\\t"; break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                char buf[8];
                std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                out += buf;
            } else {
                out += c;
            }
        }
    }
    return out;
}

bool QuantumFog::SaveToFile(const std::string& path) const {
    std::ofstream f(path);
    if (!f) return false;
    f << "{\n"
      << "  \"schema\": \"potato.quantum_fog/1\",\n"
      << "  \"intel_duration\": " << intelDuration << ",\n"
      << "  \"observe_cost\": " << intelCost << ",\n"
      << "  \"probe_cost\": " << probeCost << ",\n"
      << "  \"seed_counter\": " << seedCounter << ",\n"
      << "  \"entities\": [\n";
    for (size_t i = 0; i < entities.size(); ++i) {
        const auto& e = entities[i];
        f << "    {\"name\": \"" << FogEscapeJson(e.name) << "\", "
          << "\"team\": " << e.team << ", "
          << "\"revealed\": " << (e.revealed ? "true" : "false") << ", "
          << "\"revealed_pos\": [" << e.revealedPos.x << ", "
          << e.revealedPos.y << "], "
          << "\"reveal_timer\": " << e.revealTimer << ",\n"
          << "     \"candidates\": [";
        for (size_t j = 0; j < e.candidates.size(); ++j) {
            f << "[" << e.candidates[j].x << ", " << e.candidates[j].y
              << "]" << (j + 1 < e.candidates.size() ? ", " : "");
        }
        f << "],\n     \"priors\": [";
        for (size_t j = 0; j < e.priors.size(); ++j) {
            f << e.priors[j] << (j + 1 < e.priors.size() ? ", " : "");
        }
        f << "],\n     \"amplitudes\": [";
        const auto& amps = e.state.Amplitudes();
        for (size_t j = 0; j < amps.size(); ++j) {
            f << "[" << amps[j].real() << ", " << amps[j].imag() << "]"
              << (j + 1 < amps.size() ? ", " : "");
        }
        f << "]}" << (i + 1 < entities.size() ? ",\n" : "\n");
    }
    f << "  ],\n  \"entanglements\": [";
    for (size_t i = 0; i < links.size(); ++i) {
        f << "{\"a\": " << links[i].a << ", \"b\": " << links[i].b << "}"
          << (i + 1 < links.size() ? ", " : "");
    }
    f << "]\n}\n";
    return f.good();
}

bool QuantumFog::LoadFromFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) return false;
    std::ostringstream ss;
    ss << f.rdbuf();
    JsonValue root;
    if (!JsonValue::ParseOk(ss.str(), root)) return false;
    if (root["schema"].AsString() != "potato.quantum_fog/1") return false;

    entities.clear();
    links.clear();
    for (const auto& je : root["entities"].AsArray()) {
        const auto& jc = je["candidates"].AsArray();
        if (jc.size() < 2) return false;

        std::vector<Vector2> candidates;
        candidates.reserve(jc.size());
        for (const auto& c : jc) {
            candidates.emplace_back(c[0].AsFloat(), c[1].AsFloat());
        }
        std::vector<double> priors;
        for (const auto& p : je["priors"].AsArray()) {
            priors.push_back(p.AsNumber());
        }
        std::vector<Quantum::Qudit::Amplitude> amps;
        for (const auto& a : je["amplitudes"].AsArray()) {
            amps.emplace_back(a[0].AsNumber(), a[1].AsNumber());
        }

        const int id = AddEntity(je["name"].AsString(), je["team"].AsInt(1),
                                 candidates, priors);
        if (id < 0) return false;
        UncertainEntity& e = entities[id];
        if (amps.size() == candidates.size() &&
            e.state.SetAmplitudes(amps)) {
            // 振幅含相位，直接還原
        } else {
            e.state.SetProbabilities(e.priors);
        }
        e.revealed = je["revealed"].AsBool();
        e.revealedPos = Vector2(je["revealed_pos"][0].AsFloat(),
                                je["revealed_pos"][1].AsFloat());
        e.revealTimer = je["reveal_timer"].AsFloat();
    }
    for (const auto& jl : root["entanglements"].AsArray()) {
        Entangle(jl["a"].AsInt(-1), jl["b"].AsInt(-1));
    }
    const uint64_t sc =
        static_cast<uint64_t>(root["seed_counter"].AsNumber(0.0));
    if (sc > seedCounter) seedCounter = sc; // 新實體 seed 不回跳
    return true;
}

} // namespace Gameplay
} // namespace Potato
