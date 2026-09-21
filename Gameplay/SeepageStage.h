#pragma once

#include "MathUtils/Vector3.h"

namespace Potato {
namespace Gameplay {

// D-5 滲透視效消費端（UX-DR5）——等級→呈現指令包。
// 渲染/音訊/UI 層唯讀消費此結構，不回寫滲透狀態。
struct SeepageVisual {
    Vector3 fogTint;    // 敵情雲底色偏移（RGB 加算；0=原色）
    float shadowOffset; // 陰影錯位幅度（世界單位，+x 向）
    float jitterAmp;    // 物件微移幅度（世界單位，定域正弦擺）
    int audioLevel;     // AudioCues::SetSeepageLevel 值（0-2）
    bool themeOverride; // L3：全主題切換旗標（token swap 由呼叫端執行）
};

// 等級映射 + scrim 過渡狀態機。
// 契約：
// - 只收 int 滲透序數（0-3，越界 clamp）——Gameplay 不讀 Campaign；
//   呼叫端從 MythLayer::Level(region) 取值注入（MythIncursion 同款
//   型別隔離）。
// - Update 的 dt 必須是已乘 timeScale 的縮放後秒數——暫停中餵 0
//   不推進 scrim 計時。
// - 等級上升才觸發 scrim 過渡；同等級重餵不重播；跳級直取最高檔。
// - themeOverride 在 level>=3 期間恆真（滲透不回落的章節內語義）；
//   等級回落（跨章重置）時清除。
class SeepageStage {
public:
    // motion-reduction：scrim 縮為短 crossfade（時長驟減、無長漸變）。
    void SetMotionReduction(bool on) { reduced = on; }
    bool IsMotionReduced() const { return reduced; }

    void Update(float dt, int seepageLevel);

    const SeepageVisual& GetVisual() const { return visual; }
    // scrim 過渡遮罩不透明度（0-1；呼叫端每幀畫全屏 rect）。
    float ScrimAlpha() const;
    bool TransitionActive() const { return scrimT >= 0.0f; }
    int Level() const { return level; }

private:
    static SeepageVisual ForLevel(int lv);

    SeepageVisual visual;
    int level = 0;
    bool reduced = false;
    float scrimT = -1.0f; // <0 = 無過渡；否則為經過秒數
};

} // namespace Gameplay
} // namespace Potato
