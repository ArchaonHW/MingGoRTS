#pragma once

// TutorialFlow —— G-5 教學章節引導狀態機（headless 可驗）。
//
// 章節定義帶 tutor 欄位時，章節開始先走引導序列：
//   選卡 pick_card → 部署 deploy → 看 doctrine 觸發
//   watch_trigger → CP 介入 cp_intervene
// 每一步由 UI/殼層在完成對應操作時回報 Advance(id)；
// 亂序回報被拒絕（回 false 不動狀態），Skip() 隨時可離開。
//
// 引導文字從 NarrativePack 取 tutorial.step.<id>——文案是
// 內容不是程式；查無 id 回退內建字串。
//
// 序列完整性用 Validate() 靜態檢查：步須為正典序的子序列、
// 無重複、全為已知步——寫章節檔時即可驗，不必進遊戲。

#include "Campaign/ChapterLibrary.h"
#include "Gameplay/NarrativePack.h"

#include <string>
#include <vector>

namespace Potato {
namespace Campaign {

class TutorialFlow {
public:
    // 掛載章節定義：tutor=false → 直接完成（一般章節不引導）；
    // tutor=true 且 steps 空 → 用正典序列
    void Begin(const ChapterDef& def);

    bool Active() const { return active; }
    bool Done() const { return !active; }
    void Skip() { active = false; }

    // 當前步 id；Done/未掛載時回空字串
    const std::string& Current() const { return current; }
    // 當前步序號（0 起）與總步數——進度條用
    int StepIndex() const { return stepIndex; }
    int StepCount() const { return static_cast<int>(steps.size()); }

    // 回報步驟完成：只有命中當前步才推進（亂序回 false）。
    // 最後一步推進後 Active→false，進入正常章節流程。
    bool Advance(const std::string& stepId);

    // 引導文字：pack 查 tutorial.step.<id>，查無回退內建句
    std::string Prompt(const Gameplay::NarrativePack* pack,
                       const std::string& locale = "zh-TW") const;

    // 正典步序（索引序即合法先後）；終止於 nullptr
    static const char* const* CanonicalSteps();
    static bool IsStepKnown(const std::string& id);

    // 序列完整性檢查：回傳警告串（空=合法）。
    // 規則：非空、全已知、無重複、須為正典序子序列
    static std::vector<std::string>
    Validate(const std::vector<std::string>& steps);

private:
    std::vector<std::string> steps;
    std::string current;
    int stepIndex = 0;
    bool active = false;
};

} // namespace Campaign
} // namespace Potato
