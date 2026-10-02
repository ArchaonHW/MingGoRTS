#pragma once
#include "Campaign/CampaignState.h"
#include "Campaign/ChapterLibrary.h"
namespace Potato::Campaign {
class CampaignFlow {
  public:
    explicit CampaignFlow(const ChapterLibrary &value) : library(value) {}
    bool StartNew(CampaignState &, std::string &error) const;
    const ChapterDefinition *Current(const CampaignState &) const;
    const ChapterChoice *SelectedChoice(const CampaignState &) const;
    bool Choose(CampaignState &, const std::string &choiceId, std::string &error) const;
    bool CompleteBattle(CampaignState &, const Gameplay::BattleController &,
                        const Gameplay::Roster &, std::string &error) const;
    bool CompletePeace(CampaignState &, std::string &error) const;
    bool Advance(CampaignState &, std::string &error) const;
    bool CanResolvePeacefully(const CampaignState &, std::string &reason) const;
    bool Validate(const CampaignState &, std::string &error) const;
    void RegisterCampRoster(CampaignState &) const;

  private:
    const ChapterLibrary &library;
};
} // namespace Potato::Campaign
