#include "GeneralDossier.h"
#include "EnemyGeneral.h"

namespace Potato {
namespace Gameplay {

// 顯性軸判詞：三軸取最高且 ≥ 顯性門檻者；皆中平寫中庸
static std::string VerdictFor(float aggression, float discipline,
                              float cunning) {
    if (aggression >= discipline && aggression >= cunning &&
        aggression >= GeneralDossier::kDominantThreshold) {
        return "其人好戰，鋒不可當";
    }
    if (discipline >= cunning &&
        discipline >= GeneralDossier::kDominantThreshold) {
        return "持重守紀，難以誘之";
    }
    if (cunning >= GeneralDossier::kDominantThreshold) {
        return "多詐善變，不可盡信其形";
    }
    return "資質中庸，無足深慮";
}

// 放假判詞：最弱軸的反向描述——讓對手讀到錯的人格
static std::string PlantedVerdictFor(float aggression, float discipline,
                                     float cunning) {
    if (aggression <= discipline && aggression <= cunning) {
        return "其人怯戰，聞風先遁";
    }
    if (discipline <= cunning) {
        return "其軍無紀，擊之自亂";
    }
    return "其人魯直，不設機變";
}

const HearsayEntry& GeneralDossier::Hear(const EnemyGeneral& g) {
    HearsayEntry e;
    e.generalName = g.GetName();
    const float a = g.GetAggression();
    const float d = g.GetDiscipline();
    const float c = g.GetCunning();
    // 聽聞一律低估——玩家可學習的固定偏差
    e.estAggression = a * kHearsayBias;
    e.estDiscipline = d * kHearsayBias;
    e.estCunning = c * kHearsayBias;
    if (c >= kCunningPlantThreshold) {
        // 狡詐者放假判詞：謊在文字，數仍照偏差
        e.verdict = PlantedVerdictFor(a, d, c);
        e.planted = true;
    } else {
        e.verdict = VerdictFor(a, d, c);
    }
    for (auto& x : entries) {
        if (x.generalName == e.generalName) {
            x = e;
            return x;
        }
    }
    entries.push_back(e);
    return entries.back();
}

bool GeneralDossier::Verify(const EnemyGeneral& g) {
    for (auto& e : entries) {
        if (e.generalName != g.GetName()) {
            continue;
        }
        e.estAggression = g.GetAggression();
        e.estDiscipline = g.GetDiscipline();
        e.estCunning = g.GetCunning();
        if (e.planted && !e.verified) {
            e.verdict += "（前判有詐）";
        }
        e.verified = true;
        return true;
    }
    return false;
}

int GeneralDossier::ConsumeVerified(const std::string& generalName) {
    int n = 0;
    for (auto& e : entries) {
        if (e.generalName == generalName && e.verified && !e.planted &&
            !e.consumed) {
            e.consumed = true;
            ++n;
        }
    }
    return n;
}

const HearsayEntry* GeneralDossier::Find(
    const std::string& generalName) const {
    for (const auto& e : entries) {
        if (e.generalName == generalName) {
            return &e;
        }
    }
    return nullptr;
}

} // namespace Gameplay
} // namespace Potato
