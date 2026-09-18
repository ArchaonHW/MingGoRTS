#pragma once

namespace Potato {
namespace Gameplay {

// ---- Q-9 量子偵查教學狀態機（純邏輯,headless 可測）----
// 引導玩家走完 observe → probe → 情報時效 循環。
// 事件由 UI 層依 fog 狀態轉成 TutorialEvent 餵進來;
// 非當前步驟的事件不推進（自由探索不打斷教學）。

enum class TutorialEvent {
    None,
    Observed, // 情報觀測塌縮（Observe 成功）
    Probed,   // 弱探測收縮（Probe 成功）
    Expired,  // 情報時效到期回雲
};

enum class TutorialStep {
    Observe = 0, // 教學 1：花情報點觀測 → 雲塌縮見真身
    Probe,       // 教學 2：弱探測 → 雲收縮但不塌縮
    Expire,      // 教學 3：等時效 → 真身回雲
    Done,        // 完成
};

class TutorialScript {
public:
    // targetA/targetB：兩個教學用敵情 entity 的 fog id
    TutorialScript(int targetA = 0, int targetB = 1)
        : targetA(targetA), targetB(targetB) {}

    TutorialStep Step() const { return step; }
    bool IsDone() const { return step == TutorialStep::Done; }

    // 當前步驟要玩家操作的 fog entity；-1 = 無特定目標
    int TargetEntity() const {
        switch (step) {
        case TutorialStep::Observe: return targetA;
        case TutorialStep::Probe:   return targetB;
        default:                    return -1;
        }
    }

    int StepIndex() const { return static_cast<int>(step); }
    static constexpr int StepCount() { return 3; } // Done 不算步驟

    // 當前步驟提示文字（繁中,UI 直接顯示）
    const char* Hint() const {
        switch (step) {
        case TutorialStep::Observe:
            return "步驟 1/3 — 觀測：左鍵點高亮的那朵機率雲,"
                   "花 2 點情報讓敵軍現形";
        case TutorialStep::Probe:
            return "步驟 2/3 — 探測：右鍵（或按住 P+左鍵）點高亮的"
                   "機率雲,花 1 點情報讓雲收縮（不會完全現形）";
        case TutorialStep::Expire:
            return "步驟 3/3 — 時效：觀測得來的情報會過期。"
                   "稍等片刻,看真身退回機率雲";
        case TutorialStep::Done:
            return "教學完成！你已掌握 觀測→探測→時效 循環。"
                   "按 R 重新演練";
        }
        return "";
    }

    // 事件不屬於當前步驟時的友善提示（不推進、不計錯）
    const char* Nudge() const {
        switch (step) {
        case TutorialStep::Observe:
            return "先看提示：左鍵點高亮的那朵雲做觀測";
        case TutorialStep::Probe:
            return "先看提示：右鍵（或 P+左鍵）點高亮的那朵雲做探測";
        case TutorialStep::Expire:
            return "情報時效快到就會回雲,稍等一下";
        default:
            return "";
        }
    }

    // 餵事件；回傳是否推進了步驟。entityId = 事件作用的 fog entity
    bool Advance(TutorialEvent ev, int entityId = -1) {
        switch (step) {
        case TutorialStep::Observe:
            if (ev == TutorialEvent::Observed && entityId == targetA) {
                step = TutorialStep::Probe;
                return true;
            }
            return false;
        case TutorialStep::Probe:
            if (ev == TutorialEvent::Probed && entityId == targetB) {
                step = TutorialStep::Expire;
                return true;
            }
            // 已塌縮也算抓到重點（玩家選了強觀測）——但本步要教弱探測,
            // 故只收 Probed;Observed 給 nudge
            return false;
        case TutorialStep::Expire:
            if (ev == TutorialEvent::Expired) {
                step = TutorialStep::Done;
                return true;
            }
            return false;
        case TutorialStep::Done:
            return false;
        }
        return false;
    }

    void Reset() { step = TutorialStep::Observe; }

private:
    TutorialStep step = TutorialStep::Observe;
    int targetA;
    int targetB;
};

} // namespace Gameplay
} // namespace Potato
