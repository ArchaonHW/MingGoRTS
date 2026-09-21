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
    // 重綁=新場域：先消散舊鬼隊，不讓上一場的幻影留存
    if (ghost && !ghost->IsEliminated()) {
        ghost->ApplyCasualties(ghost->GetMembers());
    }
    ghost = nullptr;
    ghostEid = -1;
    fog = nullptr;
    // 越界門檻視為關閉（滲透序數上限 Manifest=3）
    armed = seepageThreshold >= 1 && seepageThreshold <= 3;
    threshold = seepageThreshold;
    kind = k;
    spawnPos = pos;
    state = IncursionState{};
    state.kind = k; // 渲染層在觸發前就能讀到正確 kind
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
    ghost = battle.CreateSquad(std::string(kindTag) + ":" + display,
                               kGhostTeam, spawnPos, kGhostMembers);
    if (!ghost) return; // 生成失敗：不消耗 latch，下拍重試

    if (kind == IncursionKind::FoxRumor && battle.GetFog()) {
        fog = battle.GetFog();
        // 假訊：情報雲候選格指向虛構方位（真身其實在 spawnPos）。
        // entity team=觀測方 0——觀測扣的是玩家情報點（Duanqiao 慣例）
        Vector2 fake = spawnPos;
        fake.x += kRumorOffset;
        // 夾進場界，防止假雲全部落格線外
        const float maxX =
            battle.GetField().GetWidth() * battle.GetField().GetCellSize();
        const float maxY =
            battle.GetField().GetHeight() * battle.GetField().GetCellSize();
        fake.x = std::clamp(fake.x, 0.0f, maxX - 1.0f);
        fake.y = std::clamp(fake.y, 0.0f, maxY - 1.0f);
        ghostEid = fog->AddEntityCloud(ghost->GetName(),
                                       /*觀測方=*/0, fake, 4.0f, 4,
                                       2.0f);
        if (ghostEid >= 0) {
            battle.BindFogSquad(ghost, ghostEid);
        }
    }

    state.fired = true;
    state.pending = true;
    state.kind = kind;
    state.region = region;
    state.spirit = spirit; // 存原始名（可能為空）——OutcomeEvent 如實回傳
    state.windowLeft = kResolveWindow;

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
