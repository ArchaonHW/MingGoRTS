#pragma once
#include "MathUtils/Vector2.h"
#include <array>
#include <string>
#include <vector>
namespace Potato::Campaign {
// cost 在選擇時扣除，supply 同時入帳；reward 只在該章勝利時額外入帳。
// holdSeconds > 0 代表保護任務，0 則沿用一般擊潰敵軍的勝負條件。
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
/**
 * 開機載入的唯讀章節快取（assets/campaign/chapters.json）。
 * 編號連續且 ID 唯一；地圖座標、可通行格與選項參數會在載入時驗證。
 * mapPath 以 assets/maps/ 起頭，實際位置相對於章節檔的 assets 根目錄解析，
 * 不依賴 PowerShell 當下工作目錄。所有章節驗證通過才替換目前資產庫。
 * 新增章節後須重新啟動載入；Find 傳回的指標不能跨 LoadFromFile 保留。
 */
class ChapterLibrary {
  public:
    bool LoadFromFile(const std::string &path, std::string &error);
    const std::vector<ChapterDefinition> &Chapters() const { return chapters; }
    const ChapterDefinition *Find(const std::string &id) const;
    // 初始軍與地方預備隊允許空模板 ID；非空 ID 必須存在於快取的模板庫。
    bool IsKnownTemplate(const std::string &id) const;

  private:
    std::vector<ChapterDefinition> chapters;
    std::vector<std::string> templateIds;
};
} // namespace Potato::Campaign
