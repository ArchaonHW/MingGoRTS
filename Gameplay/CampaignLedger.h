#pragma once

#include "Core/CoreTypes.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

/**
 * 敵將處置（N-4，narrative-design §System Requirements）
 *
 * 每章記下敵將的下場——收服/授首/變節/歸隱——
 * 結局時引用這些記錄（「斷橋軍師」的帳本自指）。
 */
enum class GeneralDisposition {
    Unknown,     // 未見/未決
    Subdued,     // 收服（無戰屈兵）
    Slain,       // 授首（陣斬）
    Defected,    // 變節投誠
    Retired,     // 歸隱（戰後退出）
    Negotiated,  // 議和（談判罷兵——E-1）
    Intimidated, // 懾服（嚇阻退兵——E-1）
};

const char* DispositionName(GeneralDisposition d);
bool DispositionFromName(const std::string& name,
                         GeneralDisposition& out);

/**
 * 戰役帳本：處置記錄 + 稱號軌跡 + 章節
 *
 * 跨場持久化（potato.campaign_ledger/1）；meta 層
 * （RefitCamp）每場結束後 RecordDisposition / AdvanceChapter。
 */
class CampaignLedger {
public:
    struct DispositionRecord {
        std::string generalId;
        std::string generalName;
        int chapter = 0;
        GeneralDisposition disposition = GeneralDisposition::Unknown;
    };

    // 記敵將處置；同 id 覆寫（處置可隨劇情修正）
    void RecordDisposition(const std::string& id,
                           const std::string& name, int chapter,
                           GeneralDisposition disp);
    const DispositionRecord* Find(const std::string& id) const;
    const std::vector<DispositionRecord>& Records() const {
        return records;
    }

    // 稱號軌跡：帳本自指（「斷橋軍師」式稱號隨章節演變）
    void EarnTitle(const std::string& title, int chapter);
    const std::string& CurrentTitle() const;
    const std::vector<std::pair<int, std::string>>& Titles() const {
        return titles;
    }

    // 章節推進
    void AdvanceChapter(int chapter);
    int Chapter() const { return chapter; }

    // 結局引用資料：每條處置/稱號產一行證詞
    std::vector<std::string> EndingCitations() const;

    // potato.campaign_ledger/1 序列化
    std::string ToJson() const;
    bool FromJson(const std::string& json);

private:
    std::vector<DispositionRecord> records;
    std::vector<std::pair<int, std::string>> titles; // (章節, 稱號)
    int chapter = 0;
};

} // namespace Gameplay
} // namespace Potato
