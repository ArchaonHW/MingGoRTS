#include "Campaign/CampaignState.h"

#include "Campaign/JsonWriter.h"
#include "Logging/Logger.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Potato {
namespace Campaign {

void CampaignState::AdvanceChapter(int arc, int ch,
                                   const std::string& chapterId) {
    chapter.arc = arc;
    chapter.chapter = ch;
    chapter.chapterId = chapterId;
    ledger.AdvanceChapter(ch);
}

bool CampaignState::SaveToFile(const std::string& path) const {
    JsonValue root;
    root.type = JsonValue::Type::Object;
    root.objectValue["schema"] = JsonValue::String("potato.campaign/1");
    root.objectValue["chapter"] = chapter.ToJson();
    root.objectValue["refit_camp"] = camp.ToJson();
    root.objectValue["roster"] = roster.ToJson();
    // 帳本自持字串式序列化：parse 回 JsonValue 嵌入聚合文件
    JsonValue ledgerDoc;
    if (JsonValue::ParseOk(ledger.ToJson(), ledgerDoc)) {
        root.objectValue["campaign_ledger"] = ledgerDoc;
    }
    // E-5/E-6/E-11 預留段：空物件佔位，後續填充
    JsonValue empty;
    empty.type = JsonValue::Type::Object;
    root.objectValue["governance"] = empty;
    root.objectValue["god_stance"] = empty;
    root.objectValue["intel_ledger"] = empty;

    const std::string tmp = path + ".tmp";
    {
        std::ofstream f(tmp, std::ios::trunc);
        if (!f) {
            POTATO_LOG_ERROR("CampaignState: 無法開啟暫存檔 " + tmp);
            return false;
        }
        f << WriteJson(root);
        if (!f.good()) {
            POTATO_LOG_ERROR("CampaignState: 寫入暫存檔失敗 " + tmp);
            return false;
        }
    }
    // tmp+rename 原子替換；Windows rename 不覆蓋，先移除目標
    std::error_code ec;
    std::filesystem::rename(tmp, path, ec);
    if (ec) {
        std::error_code ec2;
        std::filesystem::remove(path, ec2);
        ec.clear();
        std::filesystem::rename(tmp, path, ec);
        if (ec) {
            std::filesystem::remove(tmp, ec2);
            POTATO_LOG_ERROR("CampaignState: 原子替換失敗 " + path);
            return false;
        }
    }
    return true;
}

bool CampaignState::LoadFromFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) {
        return false;
    }
    std::ostringstream buf;
    buf << f.rdbuf();
    JsonValue root;
    if (!JsonValue::ParseOk(buf.str(), root) || !root.IsObject() ||
        root["schema"].AsString() != "potato.campaign/1") {
        POTATO_LOG_ERROR("CampaignState: 壞檔或異版，拒絕載入 " + path);
        return false;
    }
    // 先載入暫存副本，全過才置換——壞檔不動現況
    Gameplay::RefitCamp newCamp;
    Gameplay::Roster newRoster;
    Gameplay::CampaignLedger newLedger;
    ChapterState newChapter;
    if (!newCamp.FromJson(root["refit_camp"]) ||
        !newRoster.FromJson(root["roster"])) {
        POTATO_LOG_ERROR("CampaignState: 子文件損毀，拒絕載入 " + path);
        return false;
    }
    const JsonValue& lj = root["campaign_ledger"];
    if (!lj.IsNull() && !newLedger.FromJson(WriteJson(lj))) {
        POTATO_LOG_ERROR("CampaignState: 帳本段損毀，拒絕載入 " + path);
        return false;
    }
    newChapter.FromJson(root["chapter"]);
    // governance/god_stance/intel_ledger：缺段容忍，內容暫不解析
    camp = newCamp;
    roster = newRoster;
    ledger = newLedger;
    chapter = newChapter;
    return true;
}

} // namespace Campaign
} // namespace Potato
