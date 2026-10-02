#pragma once
#include "Campaign/CampaignFlow.h"

namespace Potato::Campaign {
// 存檔成功後才更新現況，重試不會重複吸收傷亡或領取補給。
template <class Action>
bool CommitCampaign(CampaignState &state, const CampaignFlow &flow, const std::string &savePath,
                    std::string &error, Action action) {
    auto candidate = state;
    if (!action(candidate, error) || !flow.Validate(candidate, error))
        return false;
    if (!candidate.SaveToFile(savePath)) {
        error = "無法寫入存檔。進度仍保留在上一個檢查點；請確認資料夾可寫入後重試。";
        return false;
    }
    state = std::move(candidate);
    error.clear();
    return true;
}
} // namespace Potato::Campaign
