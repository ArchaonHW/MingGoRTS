#pragma once
#include "Campaign/CampaignFlow.h"

namespace Potato::Campaign {
/**
 * 所有可持續操作共用的提交邊界，順序不能交換：
 * 1. 複製現況，讓 action 只修改 candidate。
 * 2. action 成功後檢查章節、帳本、營地與結果是否一致。
 * 3. 寫出完整存檔，成功後才用 candidate 替換現況。
 *
 * 任一步失敗均不更動 state；因此同一份本場結果可重試，卻不會重複吸收
 * 傷亡或領取補給。action 只能改傳入的副本，不能捕捉並修改原 state，
 * 也不能自行產生不可回復的外部副作用。這是單執行緒遊戲殼層的協定，
 * 並未提供多執行緒同時寫入同一存檔的鎖。
 */
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
