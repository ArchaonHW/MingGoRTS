#pragma once

// F-2：potato.settings/1 —— UI 設定持久化（theme/uiScale）。
// 缺檔 = 預設值不報錯；壞檔拒絕載入不動現況；
// 寫入走 tmp+rename 原子替換（與 CampaignState 同規）。
// theme 存 int 序數——本檔不引入 UITheme::Id 以外的狀態型別。

#include "Serialization/JsonParser.h"
#include "Campaign/JsonWriter.h"
#include "UITheme.h"
#include "HUDDensity.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace UISettings {

struct Data {
    int theme = 0;         // UITheme::Id 序數
    float uiScale = 1.0f;  // ClampScale 守衛 0.75–1.5
    int hudDensity = 1;    // HUDDensityUI::Density 序數（預設 Standard）
    int motionReduction = 0; // D-5：非零=滲透/主題過渡降為 crossfade
};

inline bool Save(const std::string& path, const Data& d) {
    Potato::JsonValue root;
    root.type = Potato::JsonValue::Type::Object;
    root.objectValue["schema"] =
        Potato::JsonValue::String("potato.settings/1");
    root.objectValue["theme"] = Potato::JsonValue::Number(d.theme);
    root.objectValue["uiScale"] =
        Potato::JsonValue::Number(UITheme::ClampScale(d.uiScale));
    root.objectValue["hudDensity"] = Potato::JsonValue::Number(
        (int)HUDDensityUI::DensityFromInt(d.hudDensity));
    root.objectValue["motionReduction"] =
        Potato::JsonValue::Number(d.motionReduction ? 1 : 0);

    // 首次存設定時 saves/ 可能尚未建立（原本由 campaign 存檔建立）
    std::error_code dirc;
    std::filesystem::create_directories(
        std::filesystem::path(path).parent_path(), dirc);
    const std::string tmp = path + ".tmp";
    {
        std::ofstream f(tmp, std::ios::trunc);
        if (!f) return false;
        f << Potato::Campaign::WriteJson(root);
        if (!f.good()) return false;
    }
    // tmp+rename 原子替換；Windows rename 不覆蓋，先移除目標
    std::error_code ec;
    std::filesystem::rename(tmp, path, ec);
    if (ec) {
        std::error_code ec2;
        std::filesystem::remove(path, ec2);
        ec.clear();
        std::filesystem::rename(tmp, path, ec);
        if (ec) {
            std::filesystem::remove(tmp, ec2);
            return false;
        }
    }
    return true;
}

inline bool Load(const std::string& path, Data& out) {
    std::ifstream f(path);
    if (!f) return false; // 缺檔 = 預設值不報錯
    std::ostringstream buf;
    buf << f.rdbuf();
    Potato::JsonValue root;
    if (!Potato::JsonValue::ParseOk(buf.str(), root) ||
        !root.IsObject() ||
        root["schema"].AsString() != "potato.settings/1") {
        return false; // 壞檔/異版拒絕載入，不動現況
    }
    Data d;
    d.theme = root["theme"].AsInt(0);
    d.uiScale = UITheme::ClampScale(root["uiScale"].AsFloat(1.0f));
    // F-3：可選欄位——舊檔無 hudDensity → Standard；越界鉗回
    d.hudDensity = (int)HUDDensityUI::DensityFromInt(
        root["hudDensity"].AsInt(1));
    // D-5：可選欄位——舊檔無 motionReduction → 0（不降速）；
    // AsNumber 比對避開 AsInt 對異常值的 static_cast<int> UB
    d.motionReduction =
        root["motionReduction"].AsNumber(0.0) != 0.0 ? 1 : 0;
    out = d;
    return true;
}

} // namespace UISettings
