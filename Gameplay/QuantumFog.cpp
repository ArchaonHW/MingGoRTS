#include "QuantumFog.h"

#include "BattleResources.h"
#include "MathUtils/QuasiRandom.h"

#include <algorithm>
#include <cmath>

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
                               const Vector2* biasPoint) {
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
    const double sigma = static_cast<double>(radius) * 0.5;
    const double invTwoSigmaSq = 1.0 / (2.0 * sigma * sigma);
    std::vector<double> priors(candidates.size());
    double total = 0.0;
    for (size_t i = 0; i < candidates.size(); ++i) {
        const Vector2 d = candidates[i] - bias;
        const double w = std::exp(-(d.x * d.x + d.y * d.y) * invTwoSigmaSq);
        priors[i] = w;
        total += w;
    }
    if (total <= 0.0) return -1;
    for (double& p : priors) p /= total;

    return AddEntity(name, team, candidates, priors);
}

bool QuantumFog::Observe(int entityId, const Vector2& truePos) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return false;
    }
    UncertainEntity& e = entities[entityId];
    if (e.revealed) { // 已揭露不重複扣點,但刷新真值與時效
        e.revealedPos = truePos;
        e.revealTimer = intelDuration;
        return true;
    }

    // 情報點 = 觀測成本；資源不足則觀測失敗（態不塌縮）
    if (resources && !resources->SpendIntel(e.team, intelCost)) {
        return false;
    }

    CollapseNear(e, truePos);
    return true;
}

bool QuantumFog::Reveal(int entityId, const Vector2& truePos) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return false;
    }
    CollapseNear(entities[entityId], truePos);
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
    return true;
}

void QuantumFog::CollapseNear(UncertainEntity& e, const Vector2& truePos) {
    // 塌縮到距真值最近的候選態
    int best = 0;
    float bestDist = 1e30f;
    for (int i = 0; i < static_cast<int>(e.candidates.size()); ++i) {
        const Vector2 d = e.candidates[i] - truePos;
        const float dist = d.x * d.x + d.y * d.y;
        if (dist < bestDist) { bestDist = dist; best = i; }
    }
    e.state.CollapseTo(best);
    e.revealed = true;
    e.revealedPos = truePos;
    e.revealTimer = intelDuration;
}

int QuantumFog::ObserveRandom(int entityId) {
    if (entityId < 0 || entityId >= static_cast<int>(entities.size())) {
        return -1;
    }
    UncertainEntity& e = entities[entityId];
    if (resources && !resources->SpendIntel(e.team, intelCost)) {
        return -1;
    }
    const int outcome = e.state.Measure();
    e.revealed = true;
    e.revealedPos = e.candidates[outcome];
    e.revealTimer = intelDuration;
    return outcome;
}

void QuantumFog::Update(float dt) {
    for (auto& e : entities) {
        if (e.revealed) {
            if (intelDuration > 0.0f) {
                e.revealTimer -= dt;
                if (e.revealTimer <= 0.0f) {
                    // 情報過期：回到疊加態（以先驗分佈重建）
                    e.revealed = false;
                    e.state.SetProbabilities(e.priors);
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

} // namespace Gameplay
} // namespace Potato
