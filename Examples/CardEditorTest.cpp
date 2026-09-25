// CardEditorTest — G-3 卡編輯器無頭測試。
// 覆蓋：編輯→存回→EnemyGeneral 重載整鏈、壞 doctrineId 拒存、
// fail-fast 未知 trigger、文字欄 round-trip 保真、原子寫回、
// 池不可用 warn 不擋。
#include "Gameplay/CardEditor.h"
#include "Gameplay/EnemyGeneral.h"
#include "Serialization/JsonParser.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;
using Potato::Gameplay::CardEditor;
using Potato::Gameplay::EnemyGeneral;
using Potato::JsonValue;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) { std::printf("[PASS] %s\n", name); return; }
    std::printf("[FAIL] %s\n", name);
    ++failures;
}

static const char* kCard = R"({
  "schema": "potato.character_card/1",
  "id": "test_gen", "name": "試將", "epithet": "試",
  "personality": {"aggression": 60, "discipline": 80, "cunning": 92},
  "stats": {"command": 90, "attack": 75},
  "signatureDoctrine": {"name": "奇策", "trigger": "AllyEngaged",
                        "threshold": 0, "action": "DefendNearestAlly"},
  "cards": [{"name": "預備", "trigger": "HealthBelow",
             "threshold": 0.3, "action": "RetreatToRally",
             "priority": 20, "cooldown": 5}],
  "lore": "試將行狀。", "rarity": "epic"
})";

static std::string ReadAll(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main() {
    const fs::path tmp = fs::temp_directory_path() / "g3_card_editor";
    std::error_code ec;
    fs::remove_all(tmp, ec);
    fs::create_directories(tmp / "cards", ec);
    fs::create_directories(tmp / "doctrine", ec);
    fs::create_directories(tmp / "empty_doctrine", ec);

    const fs::path cardPath = tmp / "cards" / "test_gen.json";
    { std::ofstream f(cardPath); f << kCard; }

    // 自建 doctrine 池：一張合法卡
    {
        std::ofstream f(tmp / "doctrine" / "pool_card.json");
        f << R"({"schema":"potato.doctrine/1","id":"pool_card",
                 "name":"池卡","axis":"combat","cost":1,
                 "rules":[{"trigger":"Always","action":"Scout"}]})";
    }

    // [1] AC1:人格軸編輯→Save→EnemyGeneral 重載整鏈
    {
        CardEditor ed;
        Check(ed.Load(cardPath.string()), "load card");
        Check(ed.SetPersonality("aggression", 75.0f),
              "set personality op");
        Check(ed.Save(), "save card");
        EnemyGeneral g;
        Check(g.LoadFromFile(cardPath.string()), "reload via EnemyGeneral");
        Check(std::abs(g.GetAggression() - 75.0f) < 0.01f,
              "aggression=75 persists");
        // 還原原檔
        std::ofstream f(cardPath); f << kCard;
    }

    // [2] AC2:未知 doctrineId（綁池）→ Validate error + Save 拒存
    {
        CardEditor ed;
        Check(ed.Load(cardPath.string()), "reload card");
        ed.BindDoctrineDir((tmp / "doctrine").string());
        Check(ed.SetSignatureDoctrineId("nonexistent") == false,
              "bad sig id rejected at op time");
        Check(ed.SetSignatureDoctrineId("pool_card"),
              "good sig id accepted");
        // 卡檔內建壞 id（繞過 op 檢查的存量壞資料）→Validate 攔下
        const fs::path bad = tmp / "cards" / "bad_sig.json";
        {
            std::string s = kCard;
            const std::string tag = "\"lore\":";
            s.insert(s.find(tag),
                     "\"signatureDoctrineId\": \"bogus_id\",\n  ");
            std::ofstream f(bad); f << s;
        }
        {
            CardEditor ed2;
            Check(ed2.Load(bad.string()), "load bad-sig card");
            ed2.BindDoctrineDir((tmp / "doctrine").string());
            const auto r = ed2.Validate();
            bool hasErr = false;
            for (const auto& i : r.issues)
                if (i.error &&
                    i.field == "signatureDoctrineId") hasErr = true;
            Check(hasErr, "validate catches bad doctrineId");
            const std::string before = ReadAll(bad);
            Check(!ed2.Save(bad.string()), "save refused on error");
            Check(ReadAll(bad) == before, "file untouched on refuse");
        }
    }

    // [3] AC3:未知 trigger → op 失敗且 DOM 不變
    {
        CardEditor ed;
        ed.Load(cardPath.string());
        Check(!ed.SetSignature("x", "OnTuesday", "Scout", 0.f),
              "unknown trigger op fails");
        Check(ed.Root()["signatureDoctrine"]["trigger"].AsString() ==
                  "AllyEngaged",
              "dom unchanged after failed op");
    }

    // [4] AC5:文字欄 round-trip——改 lore，其餘語義逐值相等
    {
        CardEditor ed;
        ed.Load(cardPath.string());
        Check(ed.SetText("lore", "改寫行狀。"), "set lore");
        Check(!ed.SetText("schema", "x"), "non-whitelist field refused");
        const fs::path out = tmp / "cards" / "out.json";
        Check(ed.Save(out.string()), "save-as");
        JsonValue a, b;
        JsonValue::ParseOk(kCard, a);
        JsonValue::ParseOk(ReadAll(out), b);
        Check(b["lore"].AsString() == "改寫行狀。", "lore updated");
        Check(b["stats"]["command"].AsInt() == 90 &&
                  b["cards"][0]["priority"].AsInt() == 20 &&
                  b["personality"]["cunning"].AsInt() == 92,
              "untouched fields preserved");
        Check(b["schema"].AsString() == "potato.character_card/1",
              "schema preserved");
    }

    // [5] cards[] 槽位增刪
    {
        CardEditor ed;
        ed.Load(cardPath.string());
        Check(ed.AddCardRule("增援", "Outnumbered", "AttackWeakest",
                             0.0f, 50, 8.0f),
              "add rule op");
        Check(ed.Root()["cards"].Size() == 2, "cards grew");
        Check(ed.RemoveCardRule(1), "remove rule op");
        Check(!ed.RemoveCardRule(9), "oob remove fails");
        Check(ed.Root()["cards"].Size() == 1, "cards shrunk");
    }

    // [6] AC6/AC7:池不可用 warn 不擋 + dry-run 語義由 CLI 覆
    {
        CardEditor ed;
        ed.Load(cardPath.string());
        ed.BindDoctrineDir((tmp / "empty_doctrine").string());
        Check(ed.SetSignatureDoctrineId("anything"), "empty pool: op ok");
        const auto r = ed.Validate();
        bool warnOnly = true, hasPoolWarn = false;
        for (const auto& i : r.issues) {
            if (i.error) warnOnly = false;
            if (i.field == "(doctrine)") hasPoolWarn = true;
        }
        Check(hasPoolWarn, "unusable pool warns");
        Check(warnOnly, "unusable pool not an error");
    }

    // [7] Parse-only DOM：Save() 無路徑失敗；Save(path) 可另存
    {
        CardEditor ed;
        ed.Parse(kCard);
        Check(!ed.Save(), "pathless save fails");
        const fs::path alt = tmp / "cards" / "alt.json";
        Check(ed.Save(alt.string()), "save-as path works");
        JsonValue v;
        Check(JsonValue::ParseOk(ReadAll(alt), v) &&
                  v["id"].AsString() == "test_gen",
              "saved file parses");
    }

    fs::remove_all(tmp, ec);
    std::printf("%s\n", failures == 0 ? "ALL PASS" : "FAILURES");
    return failures == 0 ? 0 : 1;
}
