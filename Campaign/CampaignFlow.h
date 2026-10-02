#pragma once
#include "Campaign/CampaignState.h"
#include "Campaign/ChapterLibrary.h"
namespace Potato::Campaign {
/**
 * 連續戰役的協調層：串接章節資產、營地、名冊與帳本，不持有即時戰場。
 *
 * 合法順序：StartNew → Briefing／Choose → CompleteBattle 或 CompletePeace
 *          → Aftermath／Advance → 下一章 Briefing；末章 Advance 進入 Complete。
 *
 * 本類修改的是記憶體狀態，不自行寫檔。遊戲入口必須透過 Checkpoint.h 的
 * CommitCampaign 包住決策、結算、整補及推進，才能在寫檔失敗時保留原狀。
 * ChapterLibrary 以參考持有，呼叫端必須讓資產庫活得比本物件久。
 */
class CampaignFlow {
  public:
    explicit CampaignFlow(const ChapterLibrary &value) : library(value) {}
    // 僅明確建立新戰役時生成原軍；營地空了不表示應重新初始化。
    bool StartNew(CampaignState &, std::string &error) const;
    // 傳回資產庫內的唯讀指標；ID 不存在時回 nullptr，不能直接解參考。
    const ChapterDefinition *Current(const CampaignState &) const;
    const ChapterChoice *SelectedChoice(const CampaignState &) const;
    // 本章只能選一次：先支付 cost、取得 supply，再記下選項 ID。
    bool Choose(CampaignState &, const std::string &choiceId, std::string &error) const;
    // 必須傳入已終止的戰鬥及已 Update 的本場名冊；Aftermath 阻擋重複結算。
    bool CompleteBattle(CampaignState &, const Gameplay::BattleController &,
                        const Gameplay::Roster &, std::string &error) const;
    // 和平路線不建立戰場、不產生戰利品或傷亡，仍保留敵將處置與完成記錄。
    bool CompletePeace(CampaignState &, std::string &error) const;
    // 只接受 Aftermath；章節編號是 1 起算，末章不再讀取下一筆陣列元素。
    bool Advance(CampaignState &, std::string &error) const;
    // 同時要求第一章 mercy/victory 與第二章 rescue/victory；reason 供 UI 說明。
    bool CanResolvePeacefully(const CampaignState &, std::string &reason) const;
    // 檢查跨子系統的一致性；JSON 型別／數值檢查另由各 FromJson 負責。
    bool Validate(const CampaignState &, std::string &error) const;
    // 將新招募或接收傷員的軍官加入永久歷史，不保存臨時 Squad 指標。
    void RegisterCampRoster(CampaignState &) const;

  private:
    const ChapterLibrary &library;
};
} // namespace Potato::Campaign
