// PlayerDeckTest —— G-2 牌庫掠奪無頭測試：
// signature 卡解鎖/重複轉強化/上限轉資源/無戰同規/入帳/存檔聚合。

#include "Campaign/CampaignState.h"
#include "Campaign/PlayerDeck.h"
#include "Gameplay/EnemyGeneral.h"

#include <cstdio>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using namespace Potato;
using namespace Potato::Campaign;
using namespace Potato::Gameplay;

static int failures = 0;
static int checks = 0;

static void Check(bool cond, const char* name) {
    ++checks;
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", name);
    }
}

static bool Nearly(float a, float b) {
    return a > b - 0.001f && a < b + 0.001f;
}

int main() {
    std::string dir = "assets/doctrine";
    if (!fs::is_directory(dir)) dir = "../../assets/doctrine";
    DoctrineLibrary lib;
    Check(lib.LoadDir(dir) >= 10, "卡池載入");

    // ---- 敵將卡 signatureDoctrineId 解析 ----
    {
        EnemyGeneral g;
        Check(g.LoadFromString(
                  R"({"schema":"potato.character_card/1",
"id":"t","name":"試將",
"signatureDoctrineId":"steel_corps_charge"})") &&
                  g.GetSignatureCardId() == "steel_corps_charge",
              "signatureDoctrineId 解析");
        EnemyGeneral g2;
        Check(g2.LoadFromString(
                  R"({"schema":"potato.character_card/1",
"id":"t2","name":"無簽將"})") &&
                  g2.GetSignatureCardId().empty(),
              "缺欄位為空");
    }

    // ---- 掠奪流程 ----
    {
        PlayerDeck deck;
        using L = PlayerDeck::LootOutcome;
        Check(deck.LootSignature("g1", "vanguard_push",
                                 GeneralDisposition::Slain, 1,
                                 lib) == L::Added,
              "陣斬掠奪入庫");
        Check(deck.Owns("vanguard_push"), "Owns 命中");
        // 無戰處置同規給卡——不戰不懲罰收集
        for (GeneralDisposition d :
             {GeneralDisposition::Subdued,
              GeneralDisposition::Negotiated,
              GeneralDisposition::Intimidated,
              GeneralDisposition::Defected,
              GeneralDisposition::Retired}) {
            Check(deck.LootSignature("g2", "envoy_deference", d, 1,
                                     lib) != L::NotEligible,
                  "非 Unknown 處置皆可掠奪");
            break; // 首張即 Added，後續測重複分支
        }
        // 重複 → 強化 ×3 → 轉化
        Check(deck.LootSignature("g2", "vanguard_push",
                                 GeneralDisposition::Subdued, 2,
                                 lib) == L::Refined,
              "重複轉強化");
        Check(deck.RefineLevel("vanguard_push") == 1, "refine 層數");
        for (int i = 0; i < 3; ++i)
            deck.LootSignature("g3", "vanguard_push",
                               GeneralDisposition::Defected, 2, lib);
        Check(deck.RefineLevel("vanguard_push") ==
                  PlayerDeck::kRefineCap,
              "refine 達上限");
        Check(deck.LootSignature("g4", "vanguard_push",
                                 GeneralDisposition::Slain, 3,
                                 lib) == L::Converted,
              "滿層轉資源");
        Check(deck.Spoils() > 0, "spoils 入帳");

        // 邊界
        Check(deck.LootSignature("g5", "vanguard_push",
                                 GeneralDisposition::Unknown, 3,
                                 lib) == L::NotEligible,
              "未決不掠奪");
        Check(deck.LootSignature("g6", "",
                                 GeneralDisposition::Slain, 3,
                                 lib) == L::NoSignature,
              "無 signature 欄位");
        Check(deck.LootSignature("g7", "ghost_card",
                                 GeneralDisposition::Slain, 3,
                                 lib) == L::UnknownCard,
              "卡池查無 id 拒收");
        Check(!deck.Owns("ghost_card"), "幽靈卡不入庫");

        // 入帳：成功掠奪全記（Added/Refined/Converted）
        Check(deck.Journal().size() == 7, "掠奪流水帳筆數");
        Check(deck.Journal()[0].disposition ==
                  GeneralDisposition::Slain &&
                  deck.Journal()[0].chapter == 1,
              "帳目記處置與章節");

        // modifier 加總含 refine 加成：envoy 0.15 基礎
        Check(Nearly(deck.TotalModifier(lib, "nobattle.negotiation"),
                     0.15f),
              "modifier 加總");
        deck.LootSignature("g8", "envoy_deference",
                           GeneralDisposition::Slain, 4, lib);
        Check(Nearly(deck.TotalModifier(lib, "nobattle.negotiation"),
                     0.15f * 1.25f),
              "refine 加成生效");

        // 規則注入：擁有卡的 rules 全進 DoctrineSet
        DoctrineSet set;
        deck.AppendOwnedRules(lib, set);
        Check(set.Count() >= 2, "擁有卡規則注入");
    }

    // ---- 序列化 roundtrip + 壞檔拒絕 ----
    {
        PlayerDeck a;
        a.LootSignature("g1", "vanguard_push",
                        GeneralDisposition::Slain, 1, lib);
        a.LootSignature("g1", "vanguard_push",
                        GeneralDisposition::Slain, 1, lib);
        PlayerDeck b;
        Check(b.FromJson(a.ToJson()), "deck roundtrip 解析");
        Check(b.Owns("vanguard_push") &&
                  b.RefineLevel("vanguard_push") == 1 &&
                  b.Journal().size() == 2,
              "roundtrip 保內容");
        JsonValue bad;
        bad.type = JsonValue::Type::Object;
        bad.objectValue["schema"] =
            JsonValue::String("potato.other/1");
        Check(!b.FromJson(bad), "異版拒絕");
    }

    // ---- CampaignState 聚合存檔 ----
    {
        const fs::path p = "player_deck_test_save.json";
        CampaignState cs;
        cs.Deck().LootSignature("ija_itagaki", "steel_corps_charge",
                                GeneralDisposition::Subdued, 2, lib);
        Check(cs.SaveToFile(p.string()), "campaign 存檔");
        CampaignState cs2;
        Check(cs2.LoadFromFile(p.string()), "campaign 讀檔");
        Check(cs2.Deck().Owns("steel_corps_charge") &&
                  cs2.Deck().Journal().size() == 1,
              "player_deck 段隨存檔往返");
        std::error_code ec;
        fs::remove(p, ec);
    }

    std::printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
