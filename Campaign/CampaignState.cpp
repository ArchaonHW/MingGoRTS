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
    // D-1：章節邊界——滲透層以治理快照重推導（跨章同樣只升不降）
    myth.DeriveFrom(governance.Depravity(), governance.CivilOrder(),
                    governance.UnrestLevel(), ch);
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
    // C-2 治理帳已填充；E-6/E-11 預留段仍空物件佔位
    root.objectValue["governance"] = governance.ToJson();
    // D-1 滲透層已填充；god_stance/intel_ledger 仍佔位
    root.objectValue["myth_layer"] = myth.ToJson();
    JsonValue empty;
    empty.type = JsonValue::Type::Object;
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
    // C-2 治理段：缺段容忍（舊檔無此段），有段則驗證後採用
    Governance newGov;
    if (!root["governance"].IsNull() &&
        !newGov.FromJson(root["governance"])) {
        POTATO_LOG_ERROR("CampaignState: 治理段損毀，拒絕載入 " + path);
        return false;
    }
    // D-1 滲透段：缺段容忍（舊檔無此段），有段損毀則拒絕
    MythLayer newMyth;
    if (!newMyth.FromJson(root["myth_layer"])) {
        POTATO_LOG_ERROR("CampaignState: 滲透段損毀，拒絕載入 " + path);
        return false;
    }
    newMyth.SetEventCallback(myth.EventCallback()); // 保留已註冊敘事出口
    // god_stance/intel_ledger：缺段容忍，內容暫不解析
    camp = newCamp;
    roster = newRoster;
    ledger = newLedger;
    chapter = newChapter;
    governance = newGov;
    myth = newMyth;
    return true;
}

} // namespace Campaign
} // namespace Potato
