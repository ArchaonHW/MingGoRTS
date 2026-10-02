#pragma once

// 戰役狀態（E-7）：跨章節狀態的 facade 聚合。
//
// 架構（game-architecture.md §Campaign State）：各子系統自持資料
// 與序列化，CampaignState 只在章節邊界聚合成單一
// potato.campaign/1 檔——原子寫（tmp+rename），壞檔大聲拒絕。
// 子系統互不探入；跨章節協調全經本類。
//
// 已掛載：RefitCamp（整補營）、Roster（名冊）、
// CampaignLedger（N-4 敵將處置/稱號/章節序帳本）、ChapterState
// （弧/章節id/已收戰線）。預留段 governance/god_stance/
// intel_ledger 由 E-5/E-6/E-11 後續填充（缺段載入不報錯）。
//
// 章節序由帳本管（AdvanceChapter 同步 ChapterState），
// 檢查點涵蓋戰前決策、整補、戰後結果與章節推進；不保存逐幀戰場。
// 執行中關閉遊戲，重開從該場戰前檢查點接續，而非恢復關閉瞬間。

#include "Campaign/ChapterState.h"
#include "Gameplay/CampaignLedger.h"
#include "Gameplay/RefitCamp.h"
#include "Gameplay/Roster.h"

#include "Gameplay/PostBattle.h"
#include <map>
#include <string>

namespace Potato {
namespace Campaign {

enum class CampaignStage { Briefing, Aftermath, Complete };
// 持久進度與暫時的 ShellScreen 分開：主選單、軍議與 3D 畫面不另存為章節。
// completed 必須是章節庫的連續前綴；choices/outcomes 以穩定 ID 而非顯示名稱索引。
// lastReport 只保存最近一次結果，cumulativeDead 才是全卷永久陣亡合計。
struct CampaignProgress {
    bool initialized = false;
    CampaignStage stage = CampaignStage::Briefing;
    std::vector<std::string> completed;
    std::map<std::string, std::string> choices, outcomes;
    Gameplay::PostBattleReport lastReport;
    int cumulativeDead = 0;
    bool lastPeaceful = false;
    JsonValue ToJson() const;
    bool FromJson(const JsonValue &);
};
class CampaignState {
  public:
    // ---- 子系統掛載 ----
    Gameplay::RefitCamp &Camp() { return camp; }
    const Gameplay::RefitCamp &Camp() const { return camp; }
    Gameplay::Roster &NamedRoster() { return roster; }
    const Gameplay::Roster &NamedRoster() const { return roster; }
    // N-4 帳本：敵將處置記錄 + 稱號軌跡 + 章節序
    Gameplay::CampaignLedger &Ledger() { return ledger; }
    const Gameplay::CampaignLedger &Ledger() const { return ledger; }

    // 弧/章節id/已收服戰線（章節序由帳本管，AdvanceChapter 同步）
    ChapterState chapter;
    CampaignProgress progress;

    // 章節邊界推進：同步帳本章節序與 ChapterState
    void AdvanceChapter(int arc, int ch, const std::string &chapterId);

    // ---- 存檔：potato.campaign/1，tmp+rename 原子寫 ----
    bool SaveToFile(const std::string &path) const;
    // 壞檔/異版：回 false 且不更動現況（先驗證再置換）
    bool LoadFromFile(const std::string &path);

  private:
    Gameplay::RefitCamp camp;
    Gameplay::Roster roster;
    Gameplay::CampaignLedger ledger;
};

} // namespace Campaign
} // namespace Potato
