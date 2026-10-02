#include "Campaign/CampaignState.h"

#include "Campaign/JsonWriter.h"
#include "Logging/Logger.h"

#include "Serialization/JsonValidation.h"
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
#include <filesystem>
#include <fstream>
#include <sstream>

namespace Potato {
namespace Campaign {

void CampaignState::AdvanceChapter(int arc, int ch, const std::string &chapterId) {
    chapter.arc = arc;
    chapter.chapter = ch;
    chapter.chapterId = chapterId;
    ledger.AdvanceChapter(ch);
}

bool CampaignState::SaveToFile(const std::string &path) const {
    JsonValue root;
    root.type = JsonValue::Type::Object;
    root.objectValue["schema"] = JsonValue::String("potato.campaign/1");
    root.objectValue["chapter"] = chapter.ToJson();
    if (progress.initialized)
        root.objectValue["campaign_progress"] = progress.ToJson();
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

    // 暫存檔放在主檔旁，讓替換在同一檔案系統內進行；未寫完前保留舊主檔。
    // flush 檢查輸出失敗，區塊結束先關閉檔案，再執行替換，避免 Windows 檔案鎖。
    const std::string tmp = path + ".tmp";
    {
        std::ofstream f(tmp, std::ios::trunc);
        if (!f) {
            POTATO_LOG_ERROR("CampaignState: 無法開啟暫存檔 " + tmp);
            return false;
        }
        f << WriteJson(root);
        f.flush();
        if (!f.good()) {
            POTATO_LOG_ERROR("CampaignState: 寫入暫存檔失敗 " + tmp);
            return false;
        }
    }
    // 失敗時保留上一份存檔；Windows 原生替換不先刪舊檔。
#ifdef _WIN32
    const auto src = std::filesystem::path(tmp).wstring();
    const auto dst = std::filesystem::path(path).wstring();
    if (!MoveFileExW(src.c_str(), dst.c_str(),
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        std::error_code ignored;
        std::filesystem::remove(tmp, ignored);
        return false;
    }
#else
    std::error_code ec;
    std::filesystem::rename(tmp, path, ec);
    if (ec) {
        std::error_code ignored;
        std::filesystem::remove(tmp, ignored);
        return false;
    }
#endif
    return true;
}

bool CampaignState::LoadFromFile(const std::string &path) {
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
    if (!newCamp.FromJson(root["refit_camp"]) || !newRoster.FromJson(root["roster"])) {
        POTATO_LOG_ERROR("CampaignState: 子文件損毀，拒絕載入 " + path);
        return false;
    }
    const JsonValue &lj = root["campaign_ledger"];
    if (!lj.IsNull() && !newLedger.FromJson(WriteJson(lj))) {
        POTATO_LOG_ERROR("CampaignState: 帳本段損毀，拒絕載入 " + path);
        return false;
    }
    const auto &cj = root["chapter"];
    if (!cj.IsObject() || !JsonValidation::Integer(cj["arc"], 0, 3) ||
        !JsonValidation::Integer(cj["chapter"], 0, 10000) || !cj["chapter_id"].IsString() ||
        !JsonValidation::Strings(cj["fronts_taken"]))
        return false;
    newChapter.FromJson(cj);
    // 沒有 campaign_progress 的舊聚合檔仍可供底層工具載入；遊戲入口另透過
    // CampaignFlow::Validate 要求 initialized 與有效章節，不把舊檔誤當可繼續戰役。
    CampaignProgress newProgress;
    if (!root["campaign_progress"].IsNull() && !newProgress.FromJson(root["campaign_progress"]))
        return false;
    if (!lj.IsNull() && newLedger.Chapter() != newChapter.chapter)
        return false;
    // governance/god_stance/intel_ledger：缺段容忍，內容暫不解析
    camp = newCamp;
    roster = newRoster;
    ledger = newLedger;
    chapter = newChapter;
    progress = std::move(newProgress);
    return true;
}

} // namespace Campaign
} // namespace Potato
