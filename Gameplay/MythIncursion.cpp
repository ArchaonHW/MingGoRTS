#include "Gameplay/MythIncursion.h"

#include "Gameplay/BattleController.h"
#include "Gameplay/QuantumFog.h"
#include "Gameplay/Squad.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <utility>

namespace Potato {
namespace Gameplay {

const char* IncursionKindName(IncursionKind k) {
    switch (k) {
    case IncursionKind::GhostLegion: return "鬼軍夜行";
    case IncursionKind::FoxRumor:    return "狐仙假訊";
    }
    return "未知";
}

bool ParseIncursionKind(const std::string& s, IncursionKind& out) {
    if (s == "ghost_legion") { out = IncursionKind::GhostLegion; return true; }
    if (s == "fox_rumor")    { out = IncursionKind::FoxRumor;    return true; }
    return false;
}

void MythIncursion::Arm(int seepageThreshold, IncursionKind k,
                        Vector2 pos) {
    // 重綁=新場域：先消散舊鬼隊，不讓上一場的幻影留存。
    // 假訊隊收隊前先 Reveal——否則假雲永遠綁在死隊上不可觀測
    if (ghost && !ghost->IsEliminated()) {
        if (fog && ghostEid >= 0) {
            fog->Reveal(ghostEid, ghost->GetPosition());
        }
        ghost->ApplyCasualties(ghost->GetMembers());
    }
    ghost = nullptr;
    ghostEid = -1;
    fog = nullptr;
    // 越界門檻視為關閉（滲透序數上限 Manifest=3）；
    // 非法 enum/非有限座標同樣拒絕——靜默退化會讓內容錯誤無痕
    const bool kindOk = k == IncursionKind::GhostLegion ||
                        k == IncursionKind::FoxRumor;
    const bool posOk = std::isfinite(pos.x) && std::isfinite(pos.y);
    armed = kindOk && posOk &&
            seepageThreshold >= 1 && seepageThreshold <= 3;
    threshold = seepageThreshold;
    kind = kindOk ? k : IncursionKind::GhostLegion;
    spawnPos = pos;
    state = IncursionState{};
    state.kind = kind; // 渲染層在觸發前就能讀到正確 kind
}

void MythIncursion::Disarm() {
    armed = false;
    ghost = nullptr;
    ghostEid = -1;
    fog = nullptr;
    state = IncursionState{};
}

void MythIncursion::Update(float dt, BattleController& battle,
                           int seepageLevel, const std::string& region,
                           const std::string& spirit) {
    if (!armed || state.resolved) return;

    if (!state.fired) {
        if (seepageLevel >= threshold) {
            Trigger(battle, region, spirit);
        }
        return;
    }

    // dt<=0/NaN 不累積窗口——暫停中餵 0 不會意外逾時
    if (state.pending && dt > 0.0f && std::isfinite(dt)) {
        state.windowLeft -= dt;
        if (state.windowLeft <= 0.0f) {
            Settle(false); // 逾時=忽視
        }
    }
}

void MythIncursion::Trigger(BattleController& battle,
                            const std::string& region,
                            const std::string& spirit) {
    const char* kindTag =
        kind == IncursionKind::FoxRumor ? "幻影" : "鬼軍";
    const std::string display =
        spirit.empty() ? "境靈" : spirit;

    // 先 latch 再生成——CreateSquad 會 Emit，回呼可再入
    // Arm/Resolve；未 latch 時再入會在半成品狀態上繼續變異
    state.fired = true;
    state.pending = true;
    state.kind = kind;
    state.region = region;
    state.spirit = display; // 存回填名——下游 AdjustFavor 不吃空鍵
    state.windowLeft = kResolveWindow;

    // 現身點夾進場界——內容可給場外座標（護欄同假雲 clamp）
    const float cell = battle.GetField().GetCellSize();
    const float maxX =
        (std::max)(0.0f,
                   battle.GetField().GetWidth() * cell - 1.0f);
    const float maxY =
        (std::max)(0.0f,
                   battle.GetField().GetHeight() * cell - 1.0f);
    spawnPos.x = std::clamp(spawnPos.x, 0.0f, maxX);
    spawnPos.y = std::clamp(spawnPos.y, 0.0f, maxY);

    ghost = battle.CreateSquad(std::string(kindTag) + ":" + display,
                               kGhostTeam, spawnPos, kGhostMembers);
    if (!ghost) {
        // 生成失敗：回滾 latch，下拍重試
        state.fired = false;
        state.pending = false;
        return;
    }

    if (kind == IncursionKind::FoxRumor && battle.GetFog()) {
        fog = battle.GetFog();
        // 假訊：情報雲候選格指向虛構方位（真身其實在 spawnPos）。
        // entity team=觀測方 0——觀測扣的是玩家情報點（Duanqiao 慣例）
        Vector2 fake = spawnPos;
        fake.x += kRumorOffset * cell;
        fake.x = std::clamp(fake.x, 0.0f, maxX);
        fake.y = std::clamp(fake.y, 0.0f, maxY);
        ghostEid = fog->AddEntityCloud(ghost->GetName(),
                                       /*觀測方=*/0, fake, 4.0f, 4,
                                       2.0f);
        if (ghostEid >= 0) {
            battle.BindFogSquad(ghost, ghostEid);
        }
    }

    char when[32];
    snprintf(when, sizeof(when), "T+%.1fs", battle.GetElapsed());

    // 先變異後派出；入帳字串先於 IncursionEvent——再入 Resolve
    // 時 ledger 順序仍是 觸發→結算
    if (onEvent) {
        onEvent(std::string("入侵:") + region + " " +
                IncursionKindName(kind) + " " + display);
    }
    if (onIncursion) {
        onIncursion({kind, region, display, when,
                     std::string(IncursionKindName(kind)) +
                         "：神話層越界"});
    }
}

bool MythIncursion::Resolve(bool pacified) {
    if (!state.pending) return false;
    Settle(pacified);
    return true;
}

void MythIncursion::Settle(bool pacified) {
    state.pending = false;
    state.resolved = true;
    state.pacified = pacified;
    state.windowLeft = 0.0f;

    // 消散=殲滅語義（無 RemoveSquad API）。
    // 鬼軍：安撫才散（忽視留存——它是真的）。
    // 假訊：任何結局都散——幻影本是情報層造假，
    //   留存會卡死勝利判定（隱形隊不可被攻擊）。
    if (ghost && (pacified || kind == IncursionKind::FoxRumor)) {
        if (fog && ghostEid >= 0) {
            fog->Reveal(ghostEid, ghost->GetPosition());
        }
        ghost->ApplyCasualties(ghost->GetMembers());
    }
    ghost = nullptr; // 結算後不再持指——防 UAF

    if (onOutcome) {
        onOutcome({pacified, state.region, state.spirit});
    }
    if (onEvent) {
        const std::string display =
            state.spirit.empty() ? "境靈" : state.spirit;
        onEvent(std::string("入侵結算:") + state.region + " " +
                (pacified ? "安撫" : "忽視") + " " + display);
    }
}

} // namespace Gameplay
} // namespace Potato
