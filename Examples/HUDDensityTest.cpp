// HUDDensityTest - F-3 HUD 密度三檔無頭測試
//
// 驗證（純函數層，不需 ImGui context）：
//   [1] 門檻矩陣：Minimal 全裁 / Standard 開前五項 / Verbose 全開
//   [2] DensityFromInt 鉗制與名稱表
//   [3] UISettings hudDensity 持久化：roundtrip/舊檔缺欄/越界鉗制

#include "HUDDensity.h"
#include "UISettings.h"

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

using namespace HUDDensityUI;
namespace fs = std::filesystem;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s\n", name);
    }
}

int main() {
    printf("=== HUD Density Test ===\n\n");

    // ---- [1] 門檻矩陣 ----
    printf("[1] 門檻矩陣\n");
    {
        bool allMin = true, stdCut = true, allVerb = true;
        for (int i = 0; i < (int)Feature::Count; ++i) {
            const Feature f = (Feature)i;
            allMin &= !HUDShown(f, Density::Minimal);
            allVerb &= HUDShown(f, Density::Verbose);
            const bool verboseOnly =
                (f == Feature::FogIntel || f == Feature::ExecStats);
            stdCut &= (HUDShown(f, Density::Standard) == !verboseOnly);
        }
        Check(allMin, "Minimal 全裁（恆在件無枚舉值所以無閘）");
        Check(stdCut, "Standard 開前五項、verbose 專屬仍關");
        Check(allVerb, "Verbose 全開");
    }

    // ---- [2] FromInt 鉗制與名稱 ----
    printf("\n[2] 序數鉗制與名稱\n");
    Check(DensityFromInt(-5) == Density::Minimal, "-5 → Minimal");
    Check(DensityFromInt(0) == Density::Minimal, "0 → Minimal");
    Check(DensityFromInt(1) == Density::Standard, "1 → Standard");
    Check(DensityFromInt(2) == Density::Verbose, "2 → Verbose");
    Check(DensityFromInt(99) == Density::Verbose, "99 → Verbose");
    Check(std::string(DensityName(Density::Minimal)) == "精簡",
          "Minimal=精簡");
    Check(std::string(DensityName(Density::Verbose)) == "詳盡",
          "Verbose=詳盡");

    // ---- [3] UISettings 持久化 ----
    printf("\n[3] hudDensity 持久化\n");
    {
        const fs::path dir = fs::temp_directory_path() / "potato_hud_test";
        std::error_code ec;
        fs::remove_all(dir, ec);
        fs::create_directories(dir);
        const std::string path = (dir / "settings.json").generic_string();

        UISettings::Data d;
        d.theme = 2;
        d.uiScale = 1.25f;
        d.hudDensity = 0; // Minimal
        Check(UISettings::Save(path, d), "設定寫出");
        UISettings::Data back;
        Check(UISettings::Load(path, back), "設定讀回");
        Check(back.hudDensity == 0 && back.theme == 2 &&
                  std::abs(back.uiScale - 1.25f) < 0.001f,
              "hudDensity/theme/uiScale roundtrip");

        // 舊檔無 hudDensity 欄 → Standard（可選欄位相容）
        {
            std::ofstream f(path, std::ios::trunc);
            f << "{\"schema\":\"potato.settings/1\",\"theme\":1,"
                 "\"uiScale\":1.0}";
        }
        UISettings::Data legacy;
        Check(UISettings::Load(path, legacy), "舊檔載入");
        Check(legacy.hudDensity == 1, "舊檔缺欄 → Standard");

        // 越界檔案值鉗回
        {
            std::ofstream f(path, std::ios::trunc);
            f << "{\"schema\":\"potato.settings/1\",\"theme\":0,"
                 "\"uiScale\":1.0,\"hudDensity\":99}";
        }
        UISettings::Data clamped;
        Check(UISettings::Load(path, clamped), "越界檔案載入");
        Check(clamped.hudDensity == 2, "hudDensity=99 鉗回 Verbose");

        // 寫出端越界也鉗
        UISettings::Data wild;
        wild.hudDensity = 42;
        UISettings::Save(path, wild);
        UISettings::Data wr;
        UISettings::Load(path, wr);
        Check(wr.hudDensity == 2, "Save 端鉗制生效");

        fs::remove_all(dir, ec);
    }

    printf("\n=== %d PASS / %d FAIL ===\n", g_pass, g_fail);
    return g_fail > 0 ? 1 : 0;
}
