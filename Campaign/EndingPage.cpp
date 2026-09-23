#include "EndingPage.h"
#include "CampaignState.h"
#include "Gameplay/CampaignLedger.h"
#include "Gameplay/MythLog.h"

#include <set>

namespace Potato {
namespace Campaign {

EndingResult EndingPage::Generate(const CampaignState& campaign,
                                  const Gameplay::MythLog* myths) {
    using CC = Gameplay::ChapterConventions;
    using Gameplay::GeneralDisposition;

    CC::EndingInput in;
    in.popularSupport = campaign.Gov().PopularSupport();
    in.civilOrder = campaign.Gov().CivilOrder();
    in.corruption = campaign.Gov().Depravity();

    std::set<int> peaceChapters;
    for (const auto& r : campaign.Ledger().Records()) {
        switch (r.disposition) {
        case GeneralDisposition::Slain:
            ++in.annihilated;
            break;
        case GeneralDisposition::Negotiated:
        case GeneralDisposition::Intimidated:
        case GeneralDisposition::Defected:
            // 無戰處置：計收服側，且所在章節記為不戰而定
            ++in.subdued;
            peaceChapters.insert(r.chapter);
            break;
        case GeneralDisposition::Subdued:
        case GeneralDisposition::Retired:
            ++in.subdued;
            break;
        case GeneralDisposition::Unknown:
            break;
        }
    }
    in.peaceChapters = static_cast<int>(peaceChapters.size());

    static const Gameplay::MythLog emptyMyths;
    const Gameplay::MythLog& m = myths ? *myths : emptyMyths;

    EndingResult out;
    out.voice = CC::ResolveEnding(in);
    out.voiceName = CC::VoiceName(out.voice);
    out.text = CC::ComposeEnding(out.voice, campaign.Ledger(), m);
    out.annihilated = in.annihilated;
    out.subdued = in.subdued;
    out.peaceChapters = in.peaceChapters;
    return out;
}

} // namespace Campaign
} // namespace Potato
