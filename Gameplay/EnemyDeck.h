#pragma once

// 敵軍牌組（EnemyDeck）——章節級敵方 doctrine 配置束
//
// ChapterDef.enemy_deck 指向 potato.enemy_deck/1 檔案的 id：
//   {"schema":"potato.enemy_deck/1","id":"duanqiao_garrison",
//    "name":"斷橋守軍","description":"...",
//    "cards":["hold_line","rally_guard"]}
//
// cards 是 assets/doctrine 卡池（potato.doctrine/1）的卡 id——
// 章節層只選卡不寫規則，規則內容仍以卡池為單一真相來源。
// Resolve() 把卡攤平成 DoctrineRule 交由 EnemyGeneral::AddCard
// 併入敵將手牌；未知卡 id 記警告略過（不讓整章因一張壞卡失效）。
//
// 消費端：部署階段讀 chapter.enemy_deck → Find(id) →
// ApplyTo(general, deck, doctrineLib)。無頭測試直接走同路徑。

#include <string>
#include <vector>

#include "Gameplay/Doctrine.h"
#include "Gameplay/DoctrineLibrary.h"

namespace Potato {
namespace Gameplay {

class EnemyGeneral;

struct EnemyDeck {
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> cardIds; // doctrine 卡池 id
};

class EnemyDeckLibrary {
public:
    int LoadDir(const std::string& dir);  // 掃 *.json，回傳入庫束數
    int LoadFile(const std::string& path);

    const EnemyDeck* Find(const std::string& id) const;
    const std::vector<EnemyDeck>& Decks() const { return decks; }
    const std::vector<std::string>& Warnings() const { return warnings; }

    // 攤平 + 套用：把 deck 的卡逐條併入敵將手牌（AddCard 具名去重，
    // 重複套牌不翻倍）。回傳實際併入的規則數；deck 為 nullptr 回 0。
    int ApplyTo(EnemyGeneral& general, const EnemyDeck* deck,
                const DoctrineLibrary& lib);

private:
    std::vector<EnemyDeck> decks;
    std::vector<std::string> warnings;
};

} // namespace Gameplay
} // namespace Potato
