#include "TutorialFlow.h"

#include <algorithm>

namespace Potato {
namespace Campaign {

namespace {

// 正典步序：核心迴路四步——索引序即合法先後
const char* const kSteps[] = {
    "pick_card",      // 選卡：寫出第一張 doctrine
    "deploy",         // 部署：放小隊上場
    "watch_trigger",  // 看 doctrine 觸發（成長綁動詞的見證）
    "cp_intervene",   // CP 介入一次（手動凌駕 doctrine）
    nullptr,
};

// pack 查無時的內建回退（zh-TW）
const char* FallbackOf(const std::string& id) {
    if (id == "pick_card")
        return "先寫軍令：選一張 doctrine 卡配給麾下小隊。";
    if (id == "deploy")
        return "部署：把小隊放上橋頭陣地。";
    if (id == "watch_trigger")
        return "觀敵：看 doctrine 依條件自行觸發——令出即行。";
    if (id == "cp_intervene")
        return "干預：按一次 CP 強令，凌駕 doctrine 親自指揮。";
    return "……";
}

int CanonicalIndex(const std::string& id) {
    for (int i = 0; kSteps[i]; ++i)
        if (id == kSteps[i]) return i;
    return -1;
}

} // namespace

const char* const* TutorialFlow::CanonicalSteps() { return kSteps; }

bool TutorialFlow::IsStepKnown(const std::string& id) {
    return CanonicalIndex(id) >= 0;
}

std::vector<std::string>
TutorialFlow::Validate(const std::vector<std::string>& steps) {
    std::vector<std::string> warns;
    if (steps.empty()) {
        warns.push_back("教學步序列為空");
        return warns;
    }
    int last = -1;
    for (const std::string& s : steps) {
        const int idx = CanonicalIndex(s);
        if (idx < 0) {
            warns.push_back("未知步 id「" + s + "」");
            continue;
        }
        if (idx <= last) {
            // 含重複（idx==last）與亂序（idx<last）
            warns.push_back("步「" + s + "」違反正典序或重複");
        }
        last = (std::max)(last, idx);
    }
    return warns;
}

void TutorialFlow::Begin(const ChapterDef& def) {
    steps.clear();
    stepIndex = 0;
    current.clear();
    if (!def.tutor) {
        active = false; // 一般章節：直接進正常流程
        return;
    }
    steps = def.tutorSteps;
    if (steps.empty()) {
        for (const char* const* s = kSteps; *s; ++s)
            steps.push_back(*s);
    }
    // 壞序列不引導——與其把玩家卡死在壞狀態機，
    // 不如直接進正常流程（內容錯誤由 Validate 在載入期抓）
    if (!Validate(steps).empty()) {
        steps.clear();
        active = false;
        return;
    }
    current = steps.front();
    active = true;
}

bool TutorialFlow::Advance(const std::string& stepId) {
    if (!active || stepId != current) {
        return false; // 亂序/已完成回報皆拒絕
    }
    if (++stepIndex >= static_cast<int>(steps.size())) {
        active = false;
        current.clear();
    } else {
        current = steps[stepIndex];
    }
    return true;
}

std::string TutorialFlow::Prompt(const Gameplay::NarrativePack* pack,
                                 const std::string& locale) const {
    if (!active) return "";
    const std::string key = "tutorial.step." + current;
    if (pack) {
        return pack->Frag(key, FallbackOf(current), {}, locale);
    }
    return FallbackOf(current);
}

} // namespace Campaign
} // namespace Potato
