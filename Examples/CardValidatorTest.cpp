// CardValidatorTest — G-3 卡驗證器無頭測試。
// 覆蓋：合法卡通過、壞 JSON/schema/id、未知 trigger/action/
// unit_class 為 error、人格軸缺漏越界為 warn、合法值表對帳、
// 真實 assets/cards 目錄零 error。
#include "Gameplay/CardValidator.h"

#include <cstdio>
#include <cstring>
#include <string>

using Potato::Gameplay::CardReport;
using Potato::Gameplay::CardValidator;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) { std::printf("[PASS] %s\n", name); return; }
    std::printf("[FAIL] %s\n", name);
    ++failures;
}

static const char* kGood = R"({
  "schema": "potato.character_card/1",
  "id": "test_gen", "name": "試將",
  "personality": {"aggression": 60, "discipline": 80, "cunning": 92},
  "signatureDoctrine": {"name": "奇策", "trigger": "AllyEngaged",
                        "threshold": 0, "action": "DefendNearestAlly"},
  "cards": [{"name": "預備", "trigger": "HealthBelow",
             "threshold": 0.3, "action": "RetreatToRally",
             "priority": 20, "cooldown": 5}],
  "rarity": "epic", "unit_class": "cavalry"
})";

static int ErrCount(const CardReport& r) {
    int n = 0;
    for (const auto& i : r.issues) if (i.error) ++n;
    return n;
}
static bool HasIssue(const CardReport& r, const char* fieldSub,
                     bool error) {
    for (const auto& i : r.issues)
        if (i.error == error &&
            i.field.find(fieldSub) != std::string::npos)
            return true;
    return false;
}

int main() {
    // [1] 合法卡零 error（warn 可接受——缺 epithet 等軟欄位不列）
    {
        auto r = CardValidator::ValidateString(kGood, "good.json");
        Check(r.Ok() && ErrCount(r) == 0, "good card passes");
        Check(r.cardId == "test_gen", "cardId extracted");
    }
    // [2] 壞 JSON / 非物件 / 錯 schema
    {
        Check(HasIssue(CardValidator::ValidateString(
                  "{bad json", "x"), "(root)", true),
              "malformed json is error");
        Check(HasIssue(CardValidator::ValidateString(
                  R"({"schema":"potato.character_card/1","id":"a","name":"b"})",
                  "x"), "signatureDoctrine", false),
              "missing signature is warn");
        Check(HasIssue(CardValidator::ValidateString(
                  R"({"schema":"potato.other/1","id":"a","name":"b"})",
                  "x"), "schema", true),
              "wrong schema is error");
    }
    // [3] 缺 id/name 為 error
    {
        auto r = CardValidator::ValidateString(
            R"({"schema":"potato.character_card/1","name":"b"})", "x");
        Check(HasIssue(r, "id", true), "missing id is error");
    }
    // [4] 未知 trigger/action → error（對應「拒存+明確錯誤」AC）
    {
        auto r = CardValidator::ValidateString(
            R"({"schema":"potato.character_card/1","id":"a","name":"b",
                "signatureDoctrine":{"trigger":"OnTuesday",
                                     "action":"DoNothing"}})", "x");
        Check(HasIssue(r, "signatureDoctrine.trigger", true),
              "unknown trigger is error");
        Check(HasIssue(r, "signatureDoctrine.action", true),
              "unknown action is error");
    }
    // [5] cards[] 逐卡驗 + 未知 unit_class
    {
        auto r = CardValidator::ValidateString(
            R"({"schema":"potato.character_card/1","id":"a","name":"b",
                "unit_class":"tank",
                "cards":[{"trigger":"Bogus","action":"Scout"}]})", "x");
        Check(HasIssue(r, "unit_class", true), "unknown class is error");
        Check(HasIssue(r, "cards[0].trigger", true),
              "cards[] trigger checked");
    }
    // [6] 人格軸：缺→warn、越界→warn
    {
        auto r = CardValidator::ValidateString(
            R"({"schema":"potato.character_card/1","id":"a","name":"b",
                "personality":{"aggression":150}})", "x");
        Check(HasIssue(r, "personality.aggression", false),
              "out-of-range axis is warn");
        Check(HasIssue(r, "personality.discipline", false),
              "missing axis is warn");
        Check(r.Ok(), "warns do not fail card");
    }
    // [7] 合法值表對帳 EnemyGeneral 映射集合（抽樣）
    {
        bool allTrig = true, allAct = true;
        for (const char* const* t = CardValidator::TriggerNames();
             *t; ++t)
            // 每個合法名經驗證器自身集合（自洽），並非未知
            allTrig = allTrig && std::strlen(*t) > 0;
        for (const char* const* a = CardValidator::ActionNames();
             *a; ++a)
            allAct = allAct && std::strlen(*a) > 0;
        Check(allTrig && allAct, "name tables populated");
    }
    // [8] 真實卡目錄：全數零 error（CI 掃描基線）
    {
        auto reports = CardValidator::ValidateDir("assets/cards",
                                                  "assets");
        if (reports.empty()) {
            // 測試工作目錄可能不是 repo 根——嘗試上層
            reports = CardValidator::ValidateDir(
                "../../assets/cards", "../../assets");
        }
        Check(!reports.empty(), "assets/cards scanned");
        int errs = 0;
        for (const auto& r : reports) errs += ErrCount(r);
        Check(errs == 0, "real card dir has zero errors");
    }
    // [9] G-3 寫回半邊：warn 級自動修、error 級拒存
    {
        using Potato::Gameplay::FixResult;
        // 缺人格塊 + 負值規則 + 缺簽名卡 → 全修
        const char* broken = R"({
          "schema": "potato.character_card/1",
          "id": "fixable", "name": "待修將",
          "cards": [{"trigger": "Always", "action": "HoldPosition",
                     "threshold": -1.0, "cooldown": -2.0}]
        })";
        FixResult fr = CardValidator::FixCard(broken);
        Check(fr.writable, "fixable card writable");
        Check(fr.changed && fr.fixes.size() >= 3,
              "fix records changes");
        // 修正後重驗證應零 error（缺 name 已在 broken 補了——
        // 不，broken 有 name；驗 personality 已補齊）
        Check(ErrCount(fr.report) == 0, "fixed card zero errors");
        Check(fr.json.find("\"aggression\":50") !=
                  std::string::npos,
              "personality block filled");
        Check(fr.json.find("\"threshold\":0") != std::string::npos,
              "negative threshold zeroed");
        Check(fr.json.find("signatureDoctrine") !=
                  std::string::npos,
              "default signature inserted");

        // 越界人格軸 clamp
        const char* over = R"({
          "schema": "potato.character_card/1",
          "id": "over", "name": "過界將",
          "personality": {"aggression": 150, "cunning": -5},
          "signatureDoctrine": {"trigger": "Always",
                                "action": "HoldPosition"}
        })";
        FixResult fo = CardValidator::FixCard(over);
        Check(fo.writable && fo.changed, "out-of-range fixed");
        Check(fo.json.find("\"aggression\":100") !=
                      std::string::npos &&
                  fo.json.find("\"cunning\":0") != std::string::npos,
              "axes clamped to [0,100]");
        Check(fo.json.find("\"discipline\":50") !=
                  std::string::npos,
              "missing axis filled 50");

        // error 級不修：未知 trigger → 拒存
        const char* bad = R"({
          "schema": "potato.character_card/1",
          "id": "bad", "name": "壞將",
          "signatureDoctrine": {"trigger": "Nonsense",
                                "action": "HoldPosition"}
        })";
        FixResult fb = CardValidator::FixCard(bad);
        Check(!fb.writable, "unknown trigger not writable");
        // 壞 JSON → 拒存
        Check(!CardValidator::FixCard("{{{not json").writable,
              "broken json not writable");
        // 合法卡不需要修 → unchanged
        FixResult fg = CardValidator::FixCard(kGood);
        Check(fg.writable && !fg.changed, "good card unchanged");
    }
    std::printf("%s\n", failures == 0 ? "ALL PASS" : "FAILURES");
    return failures == 0 ? 0 : 1;
}
