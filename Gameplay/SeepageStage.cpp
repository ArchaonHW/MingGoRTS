#include "Gameplay/SeepageStage.h"

#include <algorithm>
#include <cmath>

namespace Potato {
namespace Gameplay {

namespace {

// scrim 過渡曲線：attack（淡入）→ hold（峰值停）→ release（淡出）。
// 常規檔偏戲劇化；motion-reduction 檔只留短 crossfade。
struct ScrimCurve { float attack, hold, release, peak; };
constexpr ScrimCurve kNormal{0.35f, 0.25f, 0.90f, 0.55f};
constexpr ScrimCurve kReduced{0.05f, 0.0f, 0.12f, 0.40f};

} // namespace

SeepageVisual SeepageStage::ForLevel(int lv) {
    SeepageVisual v{};
    switch (lv) {
    case 1: // 局部異常：霧色偏移 + 陰影錯位
        v.fogTint = Vector3(0.12f, -0.06f, 0.10f);
        v.shadowOffset = 0.3f;
        v.audioLevel = 1;
        break;
    case 2: // 音景先於視覺 + 物件微移
        v.fogTint = Vector3(0.16f, -0.08f, 0.14f);
        v.shadowOffset = 0.5f;
        v.jitterAmp = 0.12f;
        v.audioLevel = 2;
        break;
    case 3: // 全主題切換
        v.fogTint = Vector3(0.20f, -0.10f, 0.18f);
        v.shadowOffset = 0.8f;
        v.jitterAmp = 0.22f;
        v.audioLevel = 2; // 無專屬音態：bed+layer 延續
        v.themeOverride = true;
        break;
    default:
        break; // 0 或越界：全零指令包
    }
    return v;
}

void SeepageStage::Update(float dt, int seepageLevel) {
    const int lv = std::clamp(seepageLevel, 0, 3);
    if (lv != level) {
        if (lv > level) {
            scrimT = 0.0f; // 等級上升才觸發過渡（含 0→N 首升）
        }
        level = lv;
        visual = ForLevel(lv);
    }
    // dt<=0/NaN 不推進計時——暫停中餵 0 不會讓 scrim 卡住狀態外洩
    if (scrimT >= 0.0f && dt > 0.0f && std::isfinite(dt)) {
        scrimT += dt;
        const ScrimCurve& c = reduced ? kReduced : kNormal;
        if (scrimT > c.attack + c.hold + c.release) {
            scrimT = -1.0f; // 過渡結束
        }
    }
}

float SeepageStage::ScrimAlpha() const {
    if (scrimT < 0.0f) return 0.0f;
    const ScrimCurve& c = reduced ? kReduced : kNormal;
    if (scrimT < c.attack) {
        return c.peak * (c.attack > 0.0f ? scrimT / c.attack : 1.0f);
    }
    if (scrimT < c.attack + c.hold) return c.peak;
    const float r = (scrimT - c.attack - c.hold) / c.release;
    return c.peak * std::max(0.0f, 1.0f - r);
}

} // namespace Gameplay
} // namespace Potato
