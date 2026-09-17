#include "SageCommand.h"

#include "BattleController.h"
#include "Squad.h"

#include <algorithm>
#include <cmath>

namespace Potato {
namespace Gameplay {

SageCommand::SageCommand(int player, int enemy)
    : playerTeam(player)
    , enemyTeam(enemy)
    , corruption(0.0f)
    , popularSupport(50.0f)
    , civilOrder(50.0f)
    , signLevel(0)
    , hereticSealed(false)
    , wuxing(WuXingPhase::WoodFire)
    , outcome(SageOutcome::Ongoing)
    , subdued(0)
    , annihilated(0) {
}

// ---------------------------------------------------------------------------
// 名稱表（中英並列）
// ---------------------------------------------------------------------------

const char* SageCommand::PolicyName(Policy p) {
    switch (p) {
    case Policy::EstablishPeople: return "立人 EstablishPeople";
    case Policy::EstablishOrder:  return "立制 EstablishOrder";
    case Policy::EstablishState:  return "立國 EstablishState";
    case Policy::RectifyNames:    return "正名 RectifyNames";
    case Policy::NourishPeople:   return "養民 NourishPeople";
    case Policy::RallyTroops:     return "整軍 RallyTroops";
    case Policy::SlanderEnemy:    return "讒敵 SlanderEnemy";
    case Policy::ConfuseEnemy:    return "惑敵 ConfuseEnemy";
    case Policy::BribeEnemy:      return "餌敵 BribeEnemy";
    case Policy::Terrorize:       return "威嚇 Terrorize";
    case Policy::Sabotage:        return "破壞 Sabotage";
    case Policy::DeceiveHeaven:   return "欺天 DeceiveHeaven";
    }
    return "?";
}

const char* SageCommand::PhaseName(WuXingPhase p) {
    switch (p) {
    case WuXingPhase::WoodFire:   return "木火 WoodFire";
    case WuXingPhase::Earth:      return "土 Earth";
    case WuXingPhase::MetalWater: return "金水 MetalWater";
    }
    return "?";
}

const char* SageCommand::OutcomeName(SageOutcome o) {
    switch (o) {
    case SageOutcome::Ongoing:          return "進行中 Ongoing";
    case SageOutcome::Victory:          return "勝利 Victory";
    case SageOutcome::Defeat:           return "敗北 Defeat";
    case SageOutcome::SubdueWithoutWar: return "至聖者無戰 SubdueWithoutWar";
    case SageOutcome::GovernedPeace:    return "治平者無勝 GovernedPeace";
    case SageOutcome::Fallen:           return "失格 Fallen";
    }
    return "?";
}

const char* SageCommand::SignName(int level) {
    switch (level) {
    case 1:  return "徵象一：以非常為常";
    case 2:  return "徵象二：以手段為正義";
    case 3:  return "徵象三：以勝負代是非";
    default: return "無徵象";
    }
}

const char* SageCommand::AuthorityName(Authority a) {
    switch (a) {
    case Authority::Tao:  return "道權 Tao";
    case Authority::Ce:   return "策權 Ce";
    case Authority::Bing: return "兵權 Bing";
    }
    return "?";
}

bool SageCommand::IsHereticPolicy(Policy p) {
    return static_cast<int>(p) >= static_cast<int>(Policy::SlanderEnemy);
}

bool SageCommand::IsUprightPolicy(Policy p) {
    return !IsHereticPolicy(p);
}

// ---------------------------------------------------------------------------
// 五行係數
// ---------------------------------------------------------------------------

float SageCommand::EffectFactor() const {
    // 金水期：裁斷與不作為，所有策效果減半
    return wuxing == WuXingPhase::MetalWater ? 0.5f : 1.0f;
}

float SageCommand::CorruptionFactor() const {
    // 木火期：生髮之勢，逆策的墮落成本折扣
    return wuxing == WuXingPhase::WoodFire ? 0.5f : 1.0f;
}

// ---------------------------------------------------------------------------
// 道權：墮落與徵象
// ---------------------------------------------------------------------------

void SageCommand::Emit(const std::string& msg) {
    if (onEvent) {
        onEvent(msg);
    }
}

void SageCommand::CheckSigns() {
    int level = corruption >= 100.0f ? 3
              : corruption >= 80.0f  ? 2
              : corruption >= 60.0f  ? 1 : 0;
    while (signLevel < level) {
        ++signLevel;
        Emit(std::string("[道權] 墮落徵象顯現：") + SignName(signLevel));
    }
    if (corruption >= CORRUPTION_MAX && outcome == SageOutcome::Ongoing) {
        outcome = SageOutcome::Fallen;
        Emit("[道權] 墮落達頂——以勝負代是非，至聖者失格，勝負無效");
    }
}

void SageCommand::AddCorruption(float amount) {
    if (amount <= 0.0f || outcome == SageOutcome::Fallen) {
        return;
    }
    corruption = std::min(CORRUPTION_MAX, corruption + amount);
    CheckSigns();
}

// ---------------------------------------------------------------------------
// 策權：政策效果
// ---------------------------------------------------------------------------

Squad* SageCommand::FindNearestEnemy(BattleController& battle) const {
    Squad* best = nullptr;
    float bestDist = 1e30f;
    // 以我方最近小隊為參照找最近敵隊；我方全滅時退為第一個活敵
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() != enemyTeam || s->IsEliminated() || s->IsRouting()) {
            continue;
        }
        float d = 1e29f;
        for (const auto& mine : battle.GetSquads()) {
            if (mine->GetTeam() == playerTeam && !mine->IsEliminated()) {
                d = std::min(d,
                    (s->GetPosition() - mine->GetPosition()).Length());
            }
        }
        if (d < bestDist) {
            bestDist = d;
            best = s.get();
        }
    }
    return best;
}

Squad* SageCommand::FindStrongestEnemy(BattleController& battle) const {
    Squad* best = nullptr;
    int strongest = -1;
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() != enemyTeam || s->IsEliminated() || s->IsRouting()) {
            continue;
        }
        if (s->GetMembers() > strongest) {
            strongest = s->GetMembers();
            best = s.get();
        }
    }
    return best;
}

int SageCommand::ApplyEnemyMorale(BattleController& battle, float delta,
                                bool all) {
    int affected = 0;
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() != enemyTeam || s->IsEliminated() || s->IsRouting()) {
            continue;
        }
        s->AdjustMorale(delta);
        ++affected;
        if (!all) {
            break;
        }
    }
    return affected;
}

void SageCommand::TryDisruptEnemy(BattleController& battle, int maxSquads,
                                  float holdSeconds) {
    // 與玩家 CP 介入同型：覆寫敵隊命令為撤退，消耗的是敵方 CP
    Vector2 rally = battle.HasRallyPoint(enemyTeam)
        ? battle.GetRallyPoint(enemyTeam)
        : Vector2(0.0f, 0.0f);
    int done = 0;
    for (const auto& s : battle.GetSquads()) {
        if (done >= maxSquads) {
            break;
        }
        if (s->GetTeam() != enemyTeam || s->IsEliminated() || s->IsRouting()) {
            continue;
        }
        if (battle.Intervene(s.get(), SquadOrder::Retreat, rally,
                             holdSeconds)) {
            ++done;
        }
    }
}

bool SageCommand::ApplyPolicy(Policy policy, BattleController& battle) {
    // 失格者無策可用；戰後（Resolution）不再受理政策
    if (outcome == SageOutcome::Fallen) {
        return false;
    }
    if (battle.GetPhase() == BattlePhase::Resolution) {
        Emit("[策權] 戰事已畢，政策不受理");
        return false;
    }

    const bool heretic = IsHereticPolicy(policy);
    if (heretic) {
        if (hereticSealed) {
            Emit("[策權] 逆策已被封邪鎖定");
            return false;
        }
        if (corruption >= CORRUPTION_MAX) {
            Emit("[策權] 墮落已滿，逆策拒絕");
            return false;
        }
    }

    const float f = EffectFactor();
    float corruptionCost = 0.0f;

    switch (policy) {
    // ---- 正六策：修己安人，不涉敵、不累墮 ----
    case Policy::EstablishPeople:
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == playerTeam && !s->IsEliminated() &&
                !s->IsRouting()) {
                s->AdjustMorale(0.15f * f);
            }
        }
        popularSupport = std::min(100.0f, popularSupport + 2.0f * f);
        break;
    case Policy::EstablishOrder:
        civilOrder = std::min(100.0f, civilOrder + 10.0f * f);
        break;
    case Policy::EstablishState:
        popularSupport = std::min(100.0f, popularSupport + 10.0f * f);
        break;
    case Policy::RectifyNames:
        civilOrder = std::min(100.0f, civilOrder + 6.0f * f);
        popularSupport = std::min(100.0f, popularSupport + 4.0f * f);
        break;
    case Policy::NourishPeople:
        popularSupport = std::min(100.0f, popularSupport + 8.0f * f);
        break;
    case Policy::RallyTroops:
        for (const auto& s : battle.GetSquads()) {
            if (s->GetTeam() == playerTeam && !s->IsEliminated() &&
                !s->IsRouting()) {
                s->AdjustMorale(0.10f * f);
            }
        }
        civilOrder = std::min(100.0f, civilOrder + 3.0f * f);
        break;

    // ---- 逆六策：傷敵取利，累積墮落 ----
    case Policy::SlanderEnemy:
        ApplyEnemyMorale(battle, -0.15f * f, false);
        corruptionCost = 8.0f;
        break;
    case Policy::ConfuseEnemy:
        ApplyEnemyMorale(battle, -0.05f * f, false);
        TryDisruptEnemy(battle, 2, 4.0f);
        corruptionCost = 10.0f;
        break;
    case Policy::BribeEnemy: {
        Squad* target = FindStrongestEnemy(battle);
        if (target) {
            target->AdjustMorale(-0.25f * f);
        }
        corruptionCost = 14.0f;
        break;
    }
    case Policy::Terrorize:
        ApplyEnemyMorale(battle, -0.08f * f, true);
        corruptionCost = 16.0f;
        break;
    case Policy::Sabotage:
        ApplyEnemyMorale(battle, -0.10f * f, true);
        TryDisruptEnemy(battle, 2, 3.0f);
        corruptionCost = 18.0f;
        break;
    case Policy::DeceiveHeaven:
        ApplyEnemyMorale(battle, -0.20f * f, true);
        corruptionCost = 25.0f;
        break;
    }

    if (heretic) {
        AddCorruption(corruptionCost * CorruptionFactor());
    }

    Emit(std::string("[策權] 施行 ") + PolicyName(policy) +
         (heretic ? "（逆策）" : "（正策）"));
    return true;
}

// ---------------------------------------------------------------------------
// 封邪與五行輪轉
// ---------------------------------------------------------------------------

bool SageCommand::SealHeresy() {
    if (wuxing != WuXingPhase::Earth) {
        Emit("[策權] 封邪失敗：非土階段");
        return false;
    }
    if (hereticSealed) {
        return false;
    }
    hereticSealed = true;
    Emit("[道權] 土德封邪：逆策鎖定，墮落漸衰，秩序回升");
    return true;
}

void SageCommand::AdvancePhase() {
    WuXingPhase prev = wuxing;
    switch (wuxing) {
    case WuXingPhase::WoodFire:   wuxing = WuXingPhase::Earth;      break;
    case WuXingPhase::Earth:      wuxing = WuXingPhase::MetalWater; break;
    case WuXingPhase::MetalWater: wuxing = WuXingPhase::WoodFire;   break;
    }
    // 離開土階段 → 封印解除
    if (prev == WuXingPhase::Earth && hereticSealed) {
        hereticSealed = false;
        Emit("[道權] 土階段已過，封邪解除");
    }
    Emit(std::string("[策權] 五行輪轉：") + PhaseName(prev) + " → " +
         PhaseName(wuxing));
}

// ---------------------------------------------------------------------------
// Tick / 結算
// ---------------------------------------------------------------------------

void SageCommand::UpdateBattleStats(const BattleController& battle) {
    annihilated = 0;
    subdued = 0;
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() != enemyTeam) {
            continue;
        }
        annihilated += s->GetMaxMembers() - s->GetMembers();
        if (s->IsRouting()) {
            subdued += s->GetMembers();
        }
    }
}

void SageCommand::Tick(float dt, const BattleController& battle) {
    // 封邪中：墮落衰減、秩序回升
    if (hereticSealed) {
        corruption = std::max(0.0f, corruption - SEAL_CORRUPTION_DECAY * dt);
        civilOrder = std::min(100.0f, civilOrder + SEAL_ORDER_RECOVER * dt);
    }

    // 歸附監察：新潰逃的敵隊計入並報告（潰逃為終態，只計一次）
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() == enemyTeam && s->IsRouting() &&
            routedCounted.find(s.get()) == routedCounted.end()) {
            routedCounted.insert(s.get());
            Emit("[道權] 敵隊潰逃歸附：" + s->GetName() + "（" +
                 std::to_string(s->GetMembers()) + " 人）");
        }
    }

    UpdateBattleStats(battle);

    // 結局更新：墮落滿即失格；戰後按勝利型態結算
    if (corruption >= CORRUPTION_MAX) {
        CheckSigns();
    } else if (battle.GetPhase() == BattlePhase::Resolution) {
        ResolveOutcome(battle);
    }
}

SageOutcome SageCommand::ResolveOutcome(const BattleController& battle) {
    // 失格優先：墮落達頂者勝負無效
    if (corruption >= CORRUPTION_MAX) {
        outcome = SageOutcome::Fallen;
        return outcome;
    }

    UpdateBattleStats(battle);

    switch (battle.GetOutcome()) {
    case BattleOutcome::Ongoing:
        outcome = SageOutcome::Ongoing;
        break;
    case BattleOutcome::Victory:
        // 至聖者無戰：敵軍潰逃/歸附數 > 殲滅數，且民心 ≥ 60
        if (subdued > annihilated && popularSupport >= SUBDUE_SUPPORT_MIN) {
            outcome = SageOutcome::SubdueWithoutWar;
            Emit("[治平] 不戰而屈人之兵——至聖者無戰");
        } else {
            outcome = SageOutcome::Victory;
        }
        break;
    case BattleOutcome::Defeat:
    case BattleOutcome::Draw:
        // 治平者無勝：戰場未勝但治理已成（民心、秩序皆 ≥70）——
        // 兵敗而政成，仁者無敵的反面證明
        if (popularSupport >= GOVERN_SUPPORT_MIN &&
            civilOrder >= GOVERN_ORDER_MIN) {
            outcome = SageOutcome::GovernedPeace;
            Emit("[治平] 戰敗而治成——治平者無勝");
        } else {
            outcome = SageOutcome::Defeat;
        }
        break;
    }
    return outcome;
}

} // namespace Gameplay
} // namespace Potato
