// ChapterLibraryTest — C-1 章節定義庫驗證（headless）
// 覆蓋：schema 驗證、欄位解析、越界消毒+警告、LoadDir 除重、
// Find/Sorted、assets/campaign 真實資產載入。

#include "Campaign/ChapterLibrary.h"

#include <cstdio>
#include <string>

using namespace Potato::Campaign;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        printf("  [PASS] %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
        ++failures;
    }
}

int main() {
    printf("=== Chapter Library Tests (C-1) ===\n");

    // ---- [1] 完整定義解析 ----
    printf("\n[1] 完整章節定義\n");
    {
        ChapterDef def;
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"duanqiao","name":"斷橋","arc":0,"chapter":1,
            "map":"maps/duanqiao.json","enemy_deck":"",
            "next":""})"), "完整定義解析");
        Check(def.id == "duanqiao" && def.name == "斷橋", "id/name");
        Check(def.arc == 0 && def.chapter == 1, "arc/chapter");
        Check(def.map == "maps/duanqiao.json", "map 路徑");
        Check(def.warnings.empty(), "合法定義無警告");
    }

    // ---- [2] 缺省值與拒絕路徑 ----
    printf("\n[2] 缺省值與拒絕\n");
    {
        ChapterDef def;
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"x"})"), "極簡定義解析");
        Check(def.name == "x", "缺 name 退回 id");
        Check(def.arc == 0 && def.chapter == 1, "arc/chapter 預設");
        Check(def.map.empty(), "缺 map 空字串");

        Check(!def.LoadFromString("not json"), "壞 JSON 拒絕");
        Check(!def.LoadFromString(R"({"id":"x"})"), "缺 schema 拒絕");
        Check(!def.LoadFromString(
                  R"({"schema":"potato.other/1","id":"x"})"),
              "錯 schema 拒絕");
        // 失敗後欄位重置為預設（不殘留上次載入）
        Check(def.id.empty() && def.arc == 0, "失敗後重置乾淨");

        ChapterDef anon;
        Check(anon.LoadFromString(
                  R"({"schema":"potato.campaign_chapter/1"})"),
              "空 id 仍解析");
        Check(!anon.warnings.empty(), "空 id 記警告");
    }

    // ---- [3] 越界消毒 ----
    printf("\n[3] 越界消毒\n");
    {
        ChapterDef def;
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"y","arc":9,"chapter":0})"), "越界值仍解析");
        Check(def.arc == 3, "arc 9 夾到 3");
        Check(def.chapter == 1, "chapter 0 夾到 1");
        Check(def.warnings.size() >= 2, "越界各記一條警告");
    }

    // ---- [4] Library ----
    printf("\n[4] ChapterLibrary\n");
    {
        ChapterLibrary lib;
        // 真實資產：assets/campaign/ 至少一個定義
        Check(lib.LoadDir("assets/campaign") >= 1,
              "assets/campaign 載入 ≥1 定義");
        const ChapterDef* dq = lib.Find("duanqiao");
        Check(dq != nullptr, "Find duanqiao");
        if (dq) {
            Check(dq->map == "maps/duanqiao.json",
                  "duanqiao 指向真實地圖");
        }
        Check(lib.Find("no_such") == nullptr, "Find 查無回 nullptr");

        // 除重：重載同目錄 keep-latest 不增長
        const size_t n1 = lib.Size();
        lib.LoadDir("assets/campaign");
        Check(lib.Size() == n1, "重載同目錄 keep-latest");

        // Add / Clear / Sorted
        ChapterDef b;
        b.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"z_last","arc":3,"chapter":9})");
        lib.Add(b);
        auto sorted = lib.Sorted();
        Check(sorted.size() == lib.Size(), "Sorted 數量一致");
        Check(sorted.back()->id == "z_last",
              "Sorted 依 (arc,chapter) 升冪");
        lib.Clear();
        Check(lib.Size() == 0 && lib.Find("duanqiao") == nullptr,
              "Clear 清空");

        Check(lib.LoadDir("") == 0, "空目錄路徑回 0");
        Check(lib.LoadDir("no_such_dir_xyz") == 0, "壞目錄回 0");
    }

    // ---- [5] LoadFromFile 路徑容錯 ----
    printf("\n[5] LoadFromFile\n");
    {
        ChapterDef def;
        Check(def.LoadFromFile("assets/campaign/duanqiao.json"),
              "LoadFromFile ../ 逐層容錯");
        Check(!def.LoadFromFile("no_such_chapter.json"),
              "LoadFromFile 壞路徑拒絕");
    }

    printf("\n=== 結果: %s ===\n", failures == 0 ? "全部 PASS" : "有 FAIL");
    return failures == 0 ? 0 : 1;
}
