#include "CardEditor.h"

#include "Gameplay/DoctrineLibrary.h"
#include "Serialization/JsonWriter.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace Potato {
namespace Gameplay {

namespace {

const char* kTextFields[] = {
    "name", "epithet", "courtesyName", "faction", "role",
    "lore", "historicalBattle", "art",
    nullptr,
};

const char* kAxes[] = {"aggression", "discipline", "cunning", nullptr};

bool InList(const char* const* table, const std::string& s) {
    for (int i = 0; table[i]; ++i)
        if (s == table[i]) return true;
    return false;
}

JsonValue MakeRule(const std::string& name, const std::string& trigger,
                   const std::string& action, float threshold,
                   int priority, float cooldown) {
    JsonValue j;
    j.type = JsonValue::Type::Object;
    j.objectValue["name"] = JsonValue::String(name);
    j.objectValue["trigger"] = JsonValue::String(trigger);
    j.objectValue["action"] = JsonValue::String(action);
    j.objectValue["threshold"] = JsonValue::Number(threshold);
    j.objectValue["priority"] = JsonValue::Number(priority);
    j.objectValue["cooldown"] = JsonValue::Number(cooldown);
    return j;
}

} // namespace

const char* const* CardEditor::TextFields() { return kTextFields; }
bool CardEditor::IsTextField(const std::string& f) {
    return InList(kTextFields, f);
}

bool CardEditor::Fail(std::string msg) {
    lastError_ = std::move(msg);
    return false;
}

bool CardEditor::Load(const std::string& path) {
    std::ifstream f(path);
    if (!f) return Fail("讀檔失敗：" + path);
    std::ostringstream ss;
    ss << f.rdbuf();
    if (!Parse(ss.str())) return false;
    path_ = path;
    return true;
}

bool CardEditor::Parse(const std::string& json) {
    JsonValue v;
    if (!JsonValue::ParseOk(json, v) || !v.IsObject())
        return Fail("JSON 解析失敗或非物件");
    root_ = std::move(v);
    loaded_ = true;
    lastError_.clear();
    return true;
}

bool CardEditor::SetPersonality(const std::string& axis, float v) {
    if (!loaded_) return Fail("未載入卡檔");
    if (!InList(kAxes, axis))
        return Fail("未知人格軸「" + axis +
                    "」（限 aggression/discipline/cunning）");
    const float c = std::max(0.0f, std::min(100.0f, v));
    JsonValue& p = root_.objectValue["personality"];
    if (!p.IsObject()) {
        JsonValue obj;
        obj.type = JsonValue::Type::Object;
        p = std::move(obj);
    }
    p.objectValue[axis] = JsonValue::Number(c);
    return true;
}

bool CardEditor::SetSignature(const std::string& name,
                              const std::string& trigger,
                              const std::string& action,
                              float threshold) {
    if (!loaded_) return Fail("未載入卡檔");
    if (!InList(CardValidator::TriggerNames(), trigger))
        return Fail("未知 trigger「" + trigger + "」");
    if (!InList(CardValidator::ActionNames(), action))
        return Fail("未知 action「" + action + "」");
    root_.objectValue["signatureDoctrine"] =
        MakeRule(name, trigger, action, threshold, 0, 0.0f);
    return true;
}

bool CardEditor::SetSignatureDoctrineId(const std::string& id) {
    if (!loaded_) return Fail("未載入卡檔");
    // 綁了池就先驗存在——壞引用 op 即拒，不必等 Save
    if (!id.empty() && !doctrineDir_.empty()) {
        DoctrineLibrary lib;
        if (lib.LoadDir(doctrineDir_) > 0 && lib.Find(id) == nullptr)
            return Fail("未知 doctrine id「" + id + "」（池內無此卡）");
    }
    root_.objectValue["signatureDoctrineId"] = JsonValue::String(id);
    return true;
}

bool CardEditor::SetText(const std::string& field,
                         const std::string& value) {
    if (!loaded_) return Fail("未載入卡檔");
    if (!IsTextField(field))
        return Fail("「" + field + "」不在可編輯文字欄白名單"
                    "（結構欄位請用專用 op）");
    root_.objectValue[field] = JsonValue::String(value);
    return true;
}

bool CardEditor::AddCardRule(const std::string& name,
                             const std::string& trigger,
                             const std::string& action, float threshold,
                             int priority, float cooldown) {
    if (!loaded_) return Fail("未載入卡檔");
    if (!InList(CardValidator::TriggerNames(), trigger))
        return Fail("未知 trigger「" + trigger + "」");
    if (!InList(CardValidator::ActionNames(), action))
        return Fail("未知 action「" + action + "」");
    JsonValue& arr = root_.objectValue["cards"];
    if (!arr.IsArray()) {
        JsonValue a;
        a.type = JsonValue::Type::Array;
        arr = std::move(a);
    }
    arr.arrayValue.push_back(
        MakeRule(name, trigger, action, threshold, priority, cooldown));
    return true;
}

bool CardEditor::RemoveCardRule(int index) {
    if (!loaded_) return Fail("未載入卡檔");
    JsonValue& arr = root_.objectValue["cards"];
    if (!arr.IsArray() || index < 0 ||
        static_cast<size_t>(index) >= arr.arrayValue.size())
        return Fail("cards[" + std::to_string(index) + "] 不存在");
    arr.arrayValue.erase(arr.arrayValue.begin() + index);
    return true;
}

CardReport CardEditor::Validate() const {
    if (!loaded_) {
        CardReport r;
        r.path = path_.empty() ? "(memory)" : path_;
        r.issues.push_back({true, "(editor)", "未載入卡檔"});
        return r;
    }
    std::string json;
    WriteJsonValue(json, root_);
    return CardValidator::ValidateString(
        json, path_.empty() ? "(memory)" : path_,
        assetsRoot_, doctrineDir_);
}

bool CardEditor::Save() {
    if (path_.empty()) return Fail("無路徑——Parse 來的 DOM 需 Save(path)");
    return Save(path_);
}

bool CardEditor::Save(const std::string& path) {
    if (!loaded_) return Fail("未載入卡檔");

    const CardReport report = Validate();
    if (!report.Ok()) {
        std::string msg = "驗證未過拒存：";
        for (const auto& i : report.issues)
            if (i.error) msg += "[" + i.field + "] " + i.message + "；";
        return Fail(msg);
    }

    std::string json;
    WriteJsonValue(json, root_);

    // tmp+rename 原子替換（Windows rename 不覆蓋，先移除目標）
    const std::string tmp = path + ".tmp";
    {
        std::ofstream f(tmp, std::ios::binary | std::ios::trunc);
        if (!f) return Fail("tmp 寫入失敗：" + tmp);
        f << json;
        if (!f) return Fail("tmp 寫入不完整：" + tmp);
    }
    std::error_code ec;
    fs::rename(tmp, path, ec);
    if (ec) {
        fs::remove(path, ec);
        ec.clear();
        fs::rename(tmp, path, ec);
    }
    if (ec) {
        fs::remove(tmp, ec);
        return Fail("rename 失敗：" + ec.message());
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
