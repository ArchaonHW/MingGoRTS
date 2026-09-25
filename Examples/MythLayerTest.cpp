// MythLayerTest - D-1 神話滲透狀態機無頭測試
// 驗收：閾值升階錄事件、章內等級只升不降、多區域獨立、
//       怒神壓力加成、跳階各錄一筆、DeriveFrom 跨章重推導、
//       JSON roundtrip、缺段降級、渲染唯讀、CampaignState 整合
#include "Campaign/CampaignState.h"
#include "Serialization/JsonWriter.h"
#include "Campaign/MythLayer.h"
#include "Gameplay/GovernanceEvent.h"
#include "Gameplay/MythLog.h"
#include "Serialization/JsonParser.h"

#include <cstdio>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace Potato::Campaign;
using namespace Potato::Gameplay;
using Potato::JsonValue;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

static std::string ReadAll(const std::string& path) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main() {
    printf("=== MythLayer Seepage Tests (D-1) ===\n\n");

    // ---- [1] 閾值升階：暴行累壓力跨閾 → 升階 + 錄轉換 ----
    printf("[1] 閾值升階錄事件\n");
    {
        MythLayer m;
        Check(m.Level("渡口") == Seepage::Quiet, "未知區域為 Quiet");
        m.Feed("渡口", GovernanceEvent::Atrocity); // +10 → 跨 kAnomalies
        Check(m.Level("渡口") == Seepage::Anomalies, "跨閾升 Anomalies");
        Check(m.Transitions().size() == 1, "錄一筆轉換");
        Check(m.Transitions()[0].from == Seepage::Quiet &&
              m.Transitions()[0].to == Seepage::Anomalies,
              "轉換記錄 from/to 正確");
        m.Feed("渡口", GovernanceEvent::Atrocity);
        m.Feed("渡口", GovernanceEvent::Atrocity); // 30 → 跨 kSeep
        Check(m.Level("渡口") == Seepage::Seep, "再累積跨 Seep");
        Check(m.Transitions().size() == 2, "第二筆轉換入帳");
    }

    // ---- [2] 章內單調：壓力降、等級不降 ----
    printf("\n[2] 章內只升不降\n");
    {
        MythLayer m;
        m.Feed("渡口", GovernanceEvent::Atrocity);   // level 1, p=10
        Check(m.Level("渡口") == Seepage::Anomalies, "已升階");
        // 連續仁政把壓力壓回 0
        for (int i = 0; i < 6; ++i) {
            m.Feed("渡口", GovernanceEvent::SurrenderAccepted);
        }
        Check(m.Pressure("渡口") == 0.0f, "仁政壓回壓力");
        Check(m.Level("渡口") == Seepage::Anomalies,
              "等級不回落（ratchet）");
    }

    // ---- [3] 多區域獨立 ----
    printf("\n[3] 多區域獨立\n");
    {
        MythLayer m;
        m.Feed("渡口", GovernanceEvent::Atrocity);
        m.Feed("渡口", GovernanceEvent::Atrocity);
        Check(m.Level("渡口") == Seepage::Anomalies, "渡口升階");
        Check(m.Level("神社") == Seepage::Quiet, "他區域不受波及");
        Check(m.Levels().size() == 1, "Levels() 只含已觸區域");
    }

    // ---- [4] 怒神加成 ----
    printf("\n[4] 怒神壓力加成\n");
    {
        MythLayer calm, angry;
        calm.BindSpirit("渡口", "土地公");
        angry.BindSpirit("渡口", "狐仙");
        angry.AdjustFavor("狐仙", -100.0f); // favor → 0 < kAngryFavor
        calm.Feed("渡口", GovernanceEvent::Atrocity);
        angry.Feed("渡口", GovernanceEvent::Atrocity);
        Check(angry.Pressure("渡口") > calm.Pressure("渡口"),
              "怒神區域壓力加成");
        Check(angry.Pressure("渡口") ==
              calm.Pressure("渡口") * MythLayer::kAngerBoost,
              "加成倍率即 kAngerBoost");
        // 平神不加成
        Check(calm.Pressure("渡口") == 10.0f, "平神區域壓力正常");
        // 非暴行類（ConvoyLost）不吃怒神加成
        calm.Feed("渡口", GovernanceEvent::ConvoyLost);
        angry.Feed("渡口", GovernanceEvent::ConvoyLost);
        Check(angry.Pressure("渡口") - calm.Pressure("渡口") ==
              10.0f * (MythLayer::kAngerBoost - 1.0f),
              "非暴行類不吃怒神加成");
        // 未綁靈區域：favor 對空字串，不應加成
        MythLayer unbound;
        unbound.Feed("渡口", GovernanceEvent::Atrocity);
        Check(unbound.Pressure("渡口") == 10.0f, "未綁靈區域不加成");
    }

    // ---- [5] 跳階：單次注入連跨多階各錄一筆 ----
    printf("\n[5] 跳階各錄一筆\n");
    {
        MythLayer m;
        m.BindSpirit("渡口", "怒神");
        m.AdjustFavor("怒神", -100.0f);
        // DeriveFrom 大注入:depravity 拉滿
        m.Feed("渡口", GovernanceEvent::Atrocity); // 建檔
        m.DeriveFrom(100.0f, 0.0f, 3, 2);          // +15+15+12=42+15=57
        Check(m.Level("渡口") == Seepage::Seep, "注入後跨至 Seep");
        Check(m.Transitions().size() >= 2, "跳階每階各錄一筆");
    }

    // ---- [6] 跨章重推導不降階 ----
    printf("\n[6] 重推導只升不降\n");
    {
        MythLayer m;
        m.Feed("渡口", GovernanceEvent::Atrocity);   // Anomalies
        m.DeriveFrom(0.0f, 100.0f, 0, 2);            // 清廉章
        Check(m.Level("渡口") == Seepage::Anomalies,
              "清廉快照不回落");
        m.DeriveFrom(80.0f, 10.0f, 3, 3);            // 腐敗章再推
        Check(static_cast<int>(m.Level("渡口")) >=
              static_cast<int>(Seepage::Seep), "腐敗快照再升階");
    }

    // ---- [7] 轉換事件入 MythLog（回呼）----
    printf("\n[7] 轉換發 MythEvent\n");
    {
        MythLayer m;
        MythLog log;
        m.SetEventCallback([&log](const MythEvent& e) {
            log.Record(e.shrine, e.spirit, e.when, e.detail);
        });
        m.BindSpirit("渡口", "土地公");
        m.Feed("渡口", GovernanceEvent::Atrocity);
        Check(log.Count() == 1, "跨階發一筆 MythEvent");
        Check(log.Events()[0].shrine == "渡口", "事件祠名即區域");
        Check(log.Events()[0].spirit == "土地公", "事件靈名即守護靈");
    }

    // ---- [8] JSON roundtrip ----
    printf("\n[8] 持久化 roundtrip\n");
    {
        MythLayer m;
        m.BindSpirit("渡口", "土地公");
        m.Feed("渡口", GovernanceEvent::Atrocity);
        m.Feed("神社", GovernanceEvent::VillageBurned);
        m.AdjustFavor("土地公", 15.0f);
        m.DeriveFrom(50.0f, 20.0f, 2, 3);

        MythLayer back;
        Check(back.FromJson(m.ToJson()), "回讀成功");
        Check(back.Level("渡口") == m.Level("渡口"), "等級保留");
        Check(back.Level("神社") == m.Level("神社"), "多區域保留");
        Check(back.Pressure("渡口") == m.Pressure("渡口"), "壓力保留");
        Check(back.Favor("土地公") == m.Favor("土地公"), "favor 保留");
        Check(back.Transitions().size() == m.Transitions().size(),
              "轉換記錄保留");
        Check(back.Transitions()[0].region ==
              m.Transitions()[0].region, "轉換內容一致");
    }

    // ---- [9] 缺段/壞段降級 ----
    printf("\n[9] 缺段降級\n");
    {
        MythLayer m;
        JsonValue empty; // Null
        Check(m.FromJson(empty), "Null 段視為空層");
        Check(m.Level("渡口") == Seepage::Quiet, "缺段全 Quiet");
        JsonValue bad;
        bad.type = JsonValue::Type::Number;
        bad.numberValue = 42;
        Check(!m.FromJson(bad), "非物件段拒絕");
        // 段內欄位型別錯誤 → 壞段拒絕
        JsonValue badRegions;
        badRegions.type = JsonValue::Type::Object;
        badRegions.objectValue["regions"] = JsonValue::Number(5);
        Check(!m.FromJson(badRegions), "regions 非陣列拒絕");
        // 非有限壓力載入歸零（防 %g 寫出 nan/inf 磚掉存檔）
        JsonValue infObj;
        infObj.type = JsonValue::Type::Object;
        JsonValue regs;
        regs.type = JsonValue::Type::Array;
        JsonValue re;
        re.type = JsonValue::Type::Object;
        re.objectValue["id"] = JsonValue::String("渡口");
        re.objectValue["lv"] = JsonValue::Number(0);
        re.objectValue["p"] = JsonValue::Number(
            std::numeric_limits<double>::infinity());
        regs.arrayValue.push_back(re);
        infObj.objectValue["regions"] = regs;
        Check(m.FromJson(infObj), "inf 壓力容錯載入");
        Check(m.Pressure("渡口") == 0.0f, "inf 壓力歸零");
        // Null 降級清殘態（重用同一物件時語義一致）
        m.Feed("渡口", GovernanceEvent::Atrocity);
        JsonValue emptyAgain;
        Check(m.FromJson(emptyAgain), "再降級成功");
        Check(m.Level("渡口") == Seepage::Quiet,
              "Null 降級清掉殘態");
    }

    // ---- [9b] favor 讀寫預設對稱 ----
    printf("\n[9b] 未註冊靈 favor 中立起算\n");
    {
        MythLayer m;
        m.AdjustFavor("新神", 10.0f); // 未註冊：應自 kNeutralFavor 起算
        Check(m.Favor("新神") == MythLayer::kNeutralFavor + 10.0f,
              "正向調整不造怒神");
        MythLayer m2;
        m2.AdjustFavor("新神", -10.0f);
        Check(m2.Favor("新神") == MythLayer::kNeutralFavor - 10.0f,
              "負向調整自中立起算");
    }

    // ---- [9c] 回呼再入層不懸空 ----
    printf("\n[9c] 事件回呼再入安全\n");
    {
        MythLayer m;
        int fired = 0;
        m.SetEventCallback([&m, &fired](const MythEvent&) {
            ++fired;
            // 回呼中再入層：不應懸空/崩潰（事件已延後派出）
            m.Feed("新區域", GovernanceEvent::SurrenderAccepted);
        });
        m.Feed("渡口", GovernanceEvent::Atrocity);
        Check(fired == 1, "轉換事件照常派出");
        Check(m.Level("新區域") == Seepage::Quiet,
              "回呼內再入層建檔安全");
    }

    // ---- [10] CampaignState 整合：存檔段 + AdvanceChapter 推導 ----
    printf("\n[10] CampaignState 整合\n");
    {
        const std::string kPath = "myth_layer_test.json";
        CampaignState cs;
        cs.Myths().BindSpirit("渡口", "土地公");
        cs.Myths().Feed("渡口", GovernanceEvent::Atrocity);
        Check(cs.Myths().Level("渡口") == Seepage::Anomalies,
              "facade 直通 Myths()");

        // 墮落治理快照 → AdvanceChapter 觸發重推導
        std::unordered_map<GovernanceEvent, int> ev;
        ev[GovernanceEvent::Atrocity] = 20; // 暴行×20 → 墮落推高
        cs.Gov().Accumulate(ev);
        cs.AdvanceChapter(1, 2, "ch2");
        Check(static_cast<int>(cs.Myths().Level("渡口")) >=
              static_cast<int>(Seepage::Seep),
              "章節邊界 DeriveFrom 推進");

        Check(cs.SaveToFile(kPath), "存檔寫出");
        const std::string json = ReadAll(kPath);
        Check(json.find("myth_layer") != std::string::npos,
              "存檔含 myth_layer 段");
        Check(json.find("\"渡口\"") != std::string::npos,
              "段內含區域資料");

        CampaignState back;
        Check(back.LoadFromFile(kPath), "回讀成功");
        Check(back.Myths().Level("渡口") == cs.Myths().Level("渡口"),
              "回讀等級一致");
        Check(back.Myths().Favor("土地公") ==
              cs.Myths().Favor("土地公"), "回讀 favor 一致");

        // 舊檔降級：剝除 myth_layer 段仍載入（JsonValue 層面操作）
        const std::string kLegacy = "myth_layer_legacy.json";
        {
            JsonValue root;
            Check(JsonValue::ParseOk(ReadAll(kPath), root),
                  "解析存檔供剝段");
            Check(root.objectValue.erase("myth_layer") == 1,
                  "myth_layer 段已剝除");
            std::ofstream f(kLegacy, std::ios::trunc);
            f << Potato::WriteJson(root);
        }
        CampaignState legacy;
        Check(legacy.LoadFromFile(kLegacy), "缺段舊檔照載");
        Check(legacy.Myths().Level("渡口") == Seepage::Quiet,
              "缺段為空層");
        std::remove(kPath.c_str());
        std::remove(kLegacy.c_str());
    }

    // [11] 序列化確定性：同邏輯狀態、不同插入序 → 同位元組輸出
    //      （unordered_map 迭代序不確定，ToJson 排序鍵 + WriteJson
    //      排序物件鍵讓存檔可 golden-diff/replay 比對）
    {
        const char* j1 =
            R"({"chapter":2,"regions":[{"id":"渡口","lv":1,"p":5},)"
            R"({"id":"神社","lv":2,"p":35}]})";
        const char* j2 =
            R"({"chapter":2,"regions":[{"id":"神社","lv":2,"p":35},)"
            R"({"id":"渡口","lv":1,"p":5}]})";
        MythLayer a, b;
        Check(a.FromJson(JsonValue::Parse(j1)), "a 載入(渡先神後)");
        Check(b.FromJson(JsonValue::Parse(j2)), "b 載入(神先渡後)");
        Check(Potato::WriteJson(a.ToJson()) == Potato::WriteJson(b.ToJson()),
              "ToJson 位元組與插入序無關");

        // DeriveFrom 事件序確定：排序鍵迭代 → 同序轉換記錄
        std::vector<std::string> ea, eb;
        a.SetEventCallback(
            [&](const MythEvent& e) { ea.push_back(e.shrine); });
        b.SetEventCallback(
            [&](const MythEvent& e) { eb.push_back(e.shrine); });
        // inject = 200*0.15 = 30：渡口 p5→35 跨 Seep、神社 p35→65 跨 Manifest
        a.DeriveFrom(200.0f, 50.0f, 0, 3);
        b.DeriveFrom(200.0f, 50.0f, 0, 3);
        Check(ea == eb, "DeriveFrom 事件序與插入序無關");
        Check(ea.size() == 2 && ea[0] == "渡口" && ea[1] == "神社",
              "事件序為排序鍵序（渡<神 by UTF-8 bytes）");
    }

    // [12] 最短 roundtrip 全精度：大數值 WriteJson→Parse 無損
    {
        JsonValue big;
        big.type = JsonValue::Type::Object;
        big.objectValue["p"] = JsonValue::Number(1234567.8901234567);
        JsonValue back;
        Check(JsonValue::ParseOk(Potato::WriteJson(big), back),
              "解析最短 roundtrip 輸出");
        Check(back["p"].AsNumber() == 1234567.8901234567,
              "大數值 roundtrip 無損（%g 六位會截斷）");
    }

    // [13] myth_layer schema tag：寫出帶 potato.myth_layer/1;
    //      讀入缺 tag 容忍（舊檔）、錯 tag 拒絕（版本鉤）
    {
        MythLayer m;
        JsonValue out = m.ToJson();
        Check(out["schema"].AsString() == "potato.myth_layer/1",
              "ToJson 帶 schema tag");

        // 舊檔（無 schema）仍可讀
        JsonValue legacy;
        Check(JsonValue::ParseOk(
                  "{\"chapter\":2,\"regions\":[],\"favor\":{}}",
                  legacy),
              "解析舊檔段");
        Check(m.FromJson(legacy) &&
                  m.ToJson()["chapter"].AsInt() == 2,
              "無 schema 舊檔容忍");

        // 錯 schema → 拒絕且不污染現狀
        JsonValue wrong;
        Check(JsonValue::ParseOk(
                  "{\"schema\":\"potato.myth_layer/2\",\"chapter\":9}",
                  wrong),
              "解析錯 tag 段");
        Check(!m.FromJson(wrong) &&
                  m.ToJson()["chapter"].AsInt() == 2,
              "錯 schema 拒絕、狀態不變");

        // 正確 schema 讀入
        JsonValue good;
        Check(JsonValue::ParseOk(
                  "{\"schema\":\"potato.myth_layer/1\",\"chapter\":3}",
                  good),
              "解析帶 tag 段");
        Check(m.FromJson(good) &&
                  m.ToJson()["chapter"].AsInt() == 3,
              "正確 schema 讀入");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
