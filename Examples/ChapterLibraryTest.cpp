// ChapterLibraryTest — C-1 章節定義庫驗證（headless）
// 覆蓋：schema 驗證、欄位解析、越界消毒+警告、LoadDir 除重、
// Find/Sorted、assets/campaign 真實資產載入。

#include "Campaign/ChapterLibrary.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
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
        // 找不到檔也重置乾淨（與 LoadFromString 同契約）
        Check(def.id.empty(), "LoadFromFile 失敗後重置乾淨");
    }

    // ---- [6] 型別警告與消毒邊界 ----
    printf("\n[6] 型別警告與消毒邊界\n");
    {
        ChapterDef def;
        // UB 邊界：巨大有限值夾到 INT_MAX 而不是溢位轉負
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"huge","chapter":1e300})"), "巨大 chapter 仍解析");
        Check(def.chapter == 2147483647, "chapter 1e300 夾到 INT_MAX");
        Check(!def.warnings.empty(), "巨大 chapter 記警告");

        // 型別不符：強轉但記警告
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":123,"arc":"abc","chapter":"x"})"), "錯型別仍解析");
        Check(def.arc == 0 && def.chapter == 1, "錯型別退回預設不強轉");
        Check(def.warnings.size() >= 3, "錯型別逐欄記警告");

        // 缺 name（id 存在）也要記警告
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"noname"})"), "缺 name 解析");
        Check(!def.warnings.empty(), "缺 name 記警告");

        // UTF-8 BOM
        Check(def.LoadFromString(
                  "\xEF\xBB\xBF{\"schema\":\"potato.campaign_chapter/1\","
                  "\"id\":\"bom\"}"),
              "BOM 前綴仍解析");
        Check(def.id == "bom", "BOM 檔 id 正確");

        // map 越界路徑記警告
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"evil","map":"../secret.json"})"), "越界 map 仍解析");
        Check(!def.warnings.empty(), "越界 map 記警告");

        // enemy_deck / next 欄位
        Check(def.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"linked","enemy_deck":"deck_a","next":"ch2"})"),
              "enemy_deck/next 解析");
        Check(def.enemyDeck == "deck_a" && def.next == "ch2",
              "enemy_deck/next 欄位值");
    }

    // ---- [7] LoadDir 注入與除重語義 ----
    printf("\n[7] LoadDir 注入\n");
    {
        namespace fs = std::filesystem;
        const fs::path tmp = "chapter_lib_test_tmp";
        std::error_code ec;
        fs::remove_all(tmp, ec);
        fs::create_directories(tmp, ec);

        auto write = [&](const char* name, const std::string& body) {
            std::ofstream f(tmp / name);
            f << body;
        };
        // 同 id 兩檔：後載（檔名序）覆蓋先載
        write("a_first.json", R"({"schema":"potato.campaign_chapter/1",
            "id":"dup","name":"先載","chapter":1})");
        write("b_second.json", R"({"schema":"potato.campaign_chapter/1",
            "id":"dup","name":"後載","chapter":2})");
        // 兩個空 id 檔：不參與除重，雙雙存活
        write("c_anon1.json",
              R"({"schema":"potato.campaign_chapter/1","name":"匿名一"})");
        write("d_anon2.json",
              R"({"schema":"potato.campaign_chapter/1","name":"匿名二"})");
        // 壞檔 + 非 json：跳過並計數
        write("e_bad.json", "{broken");
        write("f_note.txt", "not json at all");

        ChapterLibrary lib;
        const size_t n = lib.LoadDir(tmp.string());
        Check(n == 4, "LoadDir 回傳成功檔案數（壞檔/txt 不計）");
        Check(lib.LastSkipped() == 1, "LastSkipped 計壞檔");
        Check(lib.Size() == 3, "dup 除重 + 雙匿名存活");
        const ChapterDef* dup = lib.Find("dup");
        Check(dup && dup->name == "後載" && dup->chapter == 2,
              "keep-latest 內容被覆寫");

        // Add 同 id 不除重，Find 取先載入者
        ChapterDef x;
        x.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"same","name":"first"})");
        ChapterDef y;
        y.LoadFromString(R"({"schema":"potato.campaign_chapter/1",
            "id":"same","name":"second"})");
        const size_t before = lib.Size();
        lib.Add(x);
        lib.Add(y);
        Check(lib.Size() == before + 2, "Add 同 id 不除重");
        Check(lib.Find("same")->name == "first", "Find 取先載入者");

        fs::remove_all(tmp, ec);
    }

    // ---- [8] Sorted 完整排序（同弧/同章 tie-break）----
    printf("\n[8] Sorted tie-break\n");
    {
        ChapterLibrary lib;
        auto mk = [](const char* id, int arc, int ch) {
            ChapterDef d;
            d.id = id;
            d.arc = arc;
            d.chapter = ch;
            return d;
        };
        lib.Add(mk("c", 1, 2));
        lib.Add(mk("a", 1, 1));
        lib.Add(mk("b", 1, 1)); // 同 arc 同 chapter → id 決勝
        lib.Add(mk("z", 0, 9)); // 弧 0 排最前
        auto s = lib.Sorted();
        Check(s.size() == 4, "Sorted 全數回傳");
        Check(s[0]->id == "z" && s[1]->id == "a" && s[2]->id == "b" &&
                  s[3]->id == "c",
              "arc→chapter→id 三級排序");
    }

    printf("\n=== 結果: %s ===\n", failures == 0 ? "全部 PASS" : "有 FAIL");
    return failures == 0 ? 0 : 1;
}
