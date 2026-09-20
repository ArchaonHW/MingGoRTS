// 智能建議規則強化測試：命中行號、anti-pattern 接線、
// 真優化偵測、信心 enum 保留、函式偵測擴充、
// 註解/字串遮罩、文件回看、指派條件、專案規範、
// 游標鄰近排序、學習權重持久化
#include "MingGoRTS_IDE/IntelligentSuggestion.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace MingGoRTSIDE;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const char* name) {
    if (cond) { ++g_pass; std::printf("  [PASS] %s\n", name); }
    else      { ++g_fail; std::printf("  [FAIL] %s\n", name); }
}

static bool HasTitle(const std::vector<Suggestion>& v, const std::string& prefix) {
    for (const auto& s : v)
        if (s.title.find(prefix) == 0) return true;
    return false;
}

static const Suggestion* FindTitle(const std::vector<Suggestion>& v,
                                   const std::string& prefix) {
    for (const auto& s : v)
        if (s.title.find(prefix) == 0) return &s;
    return nullptr;
}

int main() {
    IntelligentSuggestionSystem sys;
    sys.Initialize();

    // [1] 命中行號：strcpy 在第 4 行，請求游標在 (1,1)
    std::printf("[1] banned API 命中行號\n");
    {
        const std::string code =
            "#include <cstring>\n"
            "void f() {\n"
            "    char b[8];\n"
            "    strcpy(b, \"x\");\n"
            "}\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(sug, "Unsafe C API");
        Check(s != nullptr, "strcpy → Unsafe C API 建議");
        Check(s && s->lineNumber == 4, "建議行號 = 命中行 4（非游標行 1）");
    }

    // [2] 多次出現 → description 帶 occurrences
    std::printf("[2] 多次命中計數\n");
    {
        const std::string code =
            "void f() {\n"
            "    char a[4], b[4];\n"
            "    strcpy(a, \"x\"); strcpy(b, \"y\"); strcat(a, b);\n"
            "}\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(sug, "Unsafe C API");
        Check(s && s->description.find("occurrences") != std::string::npos,
              "2 次 strcpy → occurrences 計數");
    }

    // [3] VeryHigh 不被權重洗回 High
    std::printf("[3] 信心 enum 保留\n");
    {
        const std::string code = "void f(){ char b[4]; strcpy(b,\"x\"); }\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(sug, "Unsafe C API");
        Check(s && s->confidence == ConfidenceLevel::VeryHigh,
              "banned API 維持 VeryHigh");
        Check(s && s->confidenceScore > 0.90f, "score > 0.90");
    }

    // [4] anti-pattern 接線：goto/malloc/printf 出 BugFix 建議
    std::printf("[4] anti-pattern → 建議\n");
    {
        const std::string code =
            "void f() {\n"
            "    void* p = malloc(4);\n"
            "    printf(\"%d\", 1);\n"
            "    goto end;\n"
            "end: return;\n"
            "}\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        Check(HasTitle(sug, "Anti-pattern: malloc"), "malloc → 建議");
        Check(HasTitle(sug, "Anti-pattern: goto"), "goto → 建議");
        Check(HasTitle(sug, "Anti-pattern: printf"), "printf → 建議");
    }

    // [5] 真優化偵測：by-value std::string 參數
    std::printf("[5] by-value 容器參數 → const&\n");
    {
        const std::string code =
            "void f(std::string name) {\n"
            "    (void)name;\n"
            "}\n"
            "void g(const std::string& ok) { (void)ok; }\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(sug, "Pass by const reference");
        Check(s != nullptr, "by-value std::string → const& 建議");
        Check(s && s->lineNumber == 1, "建議行號 = 參數行 1");
        // const& 版本不得再額外計數——occurrences 應只有 1
        Check(s && s->description.find("occurrences") == std::string::npos,
              "const& 參數不計入命中");
    }

    // [6] std::endl + 迴圈內 push_back
    std::printf("[6] endl / push_back 優化提示\n");
    {
        const std::string code =
            "#include <iostream>\n"
            "void f(std::vector<int>& out, int n) {\n"
            "    for (int i = 0; i < n; ++i) {\n"
            "        out.push_back(i);\n"
            "        std::cout << i << std::endl;\n"
            "    }\n"
            "}\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        Check(HasTitle(sug, "Prefer '\\n' over std::endl"), "std::endl → 建議");
        Check(HasTitle(sug, "Consider reserve()"), "迴圈 push_back → reserve 建議");
    }

    // [7] 函式偵測：擴型別收錄、呼叫點排除
    std::printf("[7] 函式偵測\n");
    {
        const std::string code =
            "std::string Build() { return \"x\"; }\n"   // 非四內建型別
            "auto Calc() { return 1; }\n"               // auto 回傳
            "void Use() {\n"
            "    int r = Build().size();\n"             // 呼叫點——不是函式
            "    return Calc();\n"                      // return 呼叫——不是函式
            "}\n";
        auto analysis = sys.AnalyzeCode(code, "t.cpp");
        Check(analysis.functions.size() == 3,
              "Build/Calc/Use 三函式，呼叫點不收");
        Check(analysis.functionLines.size() == analysis.functions.size(),
              "functionLines 與 functions 平行");
        Check(!analysis.functionLines.empty() && analysis.functionLines[0] == 1,
              "Build 在第 1 行");
    }

    // [8] 長函式建議指向區塊起始行
    std::printf("[8] 長函式行號\n");
    {
        std::string code = "int header_line;\n\nint BigFunc() {\n";
        for (int i = 0; i < 60; ++i) code += "    int v = 0;\n";
        code += "    return 0;\n}\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(sug, "Extract Method: BigFunc");
        Check(s != nullptr, "60 行函式 → Extract Method");
        Check(s && s->lineNumber == 3, "建議行號 = 函式起始行 3");
    }

    // [9] 測試建議帶真實函式行號
    std::printf("[9] 測試建議行號\n");
    {
        const std::string code =
            "// header comment\n"
            "int Alpha() { return 1; }\n"
            "int Beta() { return 2; }\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(sug, "Generate Unit Test: Beta");
        Check(s && s->lineNumber == 3, "Beta 測試建議 → 第 3 行");
    }

    // [10] 註解/字串遮罩：註解裡的 strcpy( 與字串裡的 sprintf( 不誤報；
    // 字串裡的 { 不錯亂區塊深度
    std::printf("[10] 註解/字串遮罩\n");
    {
        const std::string code =
            "void f() {\n"
            "    // strcpy(b, \"x\"); 已改 strncpy\n"
            "    const char* s = \"sprintf(fmt)\";\n"
            "    std::string t = \"{ not a brace\";\n"
            "}\n";
        auto sug = sys.GenerateSuggestions(code, "t.cpp", 1, 1);
        Check(!HasTitle(sug, "Unsafe C API"),
              "註解/字串內 banned API 不誤報");
        Check(!HasTitle(sug, "Anti-pattern:"),
              "字串內 anti-pattern 不誤報");
    }

    // [11] 文件回看：上一行有註解的函式不再被標 undocumented
    std::printf("[11] 文件註解回看\n");
    {
        const std::string code =
            "// documented function\n"
            "int Documented() { return 1; }\n"
            "int Bare() { return 2; }\n";
        auto analysis = sys.AnalyzeCode(code, "t.cpp");
        auto sug = sys.GetDocumentationSuggestions(analysis);
        Check(!HasTitle(sug, "Add Function Documentation: Documented"),
              "有註解的函式不建議文件");
        Check(HasTitle(sug, "Add Function Documentation: Bare"),
              "無註解的函式仍建議文件");
    }

    // [12] 指派誤植條件
    std::printf("[12] 指派條件偵測\n");
    {
        auto bad = sys.GenerateSuggestions(
            "void f(int x) {\n    if (x = 0) {}\n}\n", "t.cpp", 1, 1);
        const Suggestion* s = FindTitle(bad, "Assignment in condition");
        Check(s != nullptr && s->lineNumber == 2,
              "if (x = 0) → 建議指到第 2 行");
        auto ok = sys.GenerateSuggestions(
            "void f(int x) {\n"
            "    if (x == 0) {}\n"
            "    while ((x = next()) != 0) {}\n"  // 雙層括號慣用法
            "    if (int y = 1; y > 0) {}\n"      // C++17 init-statement
            "}\n", "t.cpp", 1, 1);
        Check(!HasTitle(ok, "Assignment in condition"),
              "==/括號慣用法/init-statement 不誤報");
    }

    // [13] 專案規範：第三方 JSON、依賴方向
    std::printf("[13] 專案規範規則\n");
    {
        const std::string json = "#include <nlohmann/json.hpp>\nvoid f() {}\n";
        auto sug = sys.GenerateSuggestions(json, "Gameplay/Foo.cpp", 1, 1);
        Check(HasTitle(sug, "Use Serialization/JsonParser.h"),
              "nlohmann → JsonParser 建議");
        auto ext = sys.GenerateSuggestions(json, "external/x/y.cpp", 1, 1);
        Check(!HasTitle(ext, "Use Serialization/JsonParser.h"),
              "external/ 路徑不報");

        const std::string dep = "#include \"Gameplay/Squad.h\"\nvoid f() {}\n";
        auto eng = sys.GenerateSuggestions(dep, "Rendering/Mesh.cpp", 1, 1);
        Check(HasTitle(eng, "Engine must not depend on game layer"),
              "引擎→Gameplay 依賴違規");
        auto demo = sys.GenerateSuggestions(dep, "Examples/demo.cpp", 1, 1);
        Check(!HasTitle(demo, "Engine must not depend"),
              "Examples 路徑不報");

        const std::string dep2 = "#include \"Campaign/CampaignState.h\"\nvoid f() {}\n";
        auto gp = sys.GenerateSuggestions(dep2, "Gameplay/Battle.cpp", 1, 1);
        Check(HasTitle(gp, "Gameplay must not depend on Campaign"),
              "Gameplay→Campaign 依賴違規");
    }

    // [14] 游標鄰近排序：同信心建議，離游標近的排前面
    std::printf("[14] 游標鄰近排序\n");
    {
        std::string code = "void a() {\n    malloc(4);\n}\n";
        for (int i = 0; i < 40; ++i) code += "int filler = 0;\n";
        code += "void b() {\n    printf(\"x\");\n}\n";
        const int cursorLine = static_cast<int>(code.size() > 0 ? 46 : 1);
        auto sug = sys.GenerateSuggestions(code, "t.cpp", cursorLine, 1);
        Check(!sug.empty() &&
                  sug.front().title.find("printf") != std::string::npos,
              "游標旁的 printf 建議排到第一");
    }

    // [15] 學習權重持久化：外部權重檔載入後影響過濾
    std::printf("[15] 學習權重持久化\n");
    {
        const char* path = "suggestion_weights_test.json";
        {
            std::ofstream w(path);
            w << "{\"weights\":{\"bug_fix\":0.15}}";
        }
        IntelligentSuggestionSystem sys2;
        sys2.SetLearningPersistencePath(path);
        sys2.Initialize();
        auto sug = sys2.GenerateSuggestions(
            "void f(){ char b[4]; strcpy(b,\"x\"); }\n", "t.cpp", 1, 1);
        Check(!HasTitle(sug, "Unsafe C API"),
              "bug_fix 權重 0.15 → banned API 被 Medium 門檻過濾");
        sys2.Shutdown(); // 應寫回權重檔
        std::ifstream check(path);
        Check(check.good(), "Shutdown 寫回權重檔");
        check.close();
        std::remove(path);
    }

    sys.Shutdown();
    std::printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
