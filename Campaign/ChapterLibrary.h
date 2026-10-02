#pragma once
#include "MathUtils/Vector2.h"
#include <array>
#include <string>
#include <vector>
namespace Potato::Campaign {
struct ChapterChoice {
    std::string id, label, description;
    int cost = 0, supply = 0, reward = 0;
    float holdSeconds = 0;
};
struct EnemyDeployment {
    std::string name;
    int members = 0;
    Vector2 position;
};
struct ChapterDefinition {
    std::string id, title, opening, objectiveText, victoryText, defeatText, closingHook, mapPath,
        enemyGeneralName, enemyPersonality;
    int number = 0, arc = 0;
    float enemySpeed = 1.6f, enemyRange = 1.4f, enemyDamage = 0.025f, holdSeconds = 0;
    std::array<float, 3> tint{0.24f, 0.3f, 0.2f};
    std::vector<Vector2> friendlyDeployment;
    std::vector<EnemyDeployment> enemies;
    std::vector<ChapterChoice> choices;
};
class ChapterLibrary {
  public:
    bool LoadFromFile(const std::string &path, std::string &error);
    const std::vector<ChapterDefinition> &Chapters() const { return chapters; }
    const ChapterDefinition *Find(const std::string &id) const;
    bool IsKnownTemplate(const std::string &id) const;

  private:
    std::vector<ChapterDefinition> chapters;
    std::vector<std::string> templateIds;
};
} // namespace Potato::Campaign
