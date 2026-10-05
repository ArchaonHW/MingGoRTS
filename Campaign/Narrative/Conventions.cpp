#include "Campaign/Narrative/Conventions.h"

#include "Campaign/Governance/Accumulators.h" // FoldGovernance
#include "Campaign/Ledger/Ledger.h"          // Account::Mandate

namespace Potato::Campaign {

using Gameplay::JsonValue;

namespace {

// Variant lookup — requested key, then "default", then the map's
// first entry (deterministic: std::map ordering), then empty.
std::string_view Variant(const std::map<std::string, std::string, std::less<>>& m,
                         std::string_view key) {
    auto it = m.find(key);
    if (it == m.end()) it = m.find("default");
    if (it == m.end() && !m.empty()) it = m.begin();
    return it == m.end() ? std::string_view()
                         : std::string_view(it->second);
}

// Variant map field — object of short string variants, or a bare
// string treated as a single "default" variant (content shorthand).
Gameplay::Result<std::map<std::string, std::string, std::less<>>>
ParseVariants(const JsonValue& v, const char* field) {
    std::map<std::string, std::string, std::less<>> m;
    if (v.IsString()) {
        if (v.AsString().size() > ChapterConventions::MAX_TEXT_LEN) {
            return Gameplay::Fail<std::map<std::string, std::string, std::less<>>>(
                "narrative", "variant text too long");
        }
        m.emplace("default", v.AsString());
        return Gameplay::Ok(std::move(m));
    }
    if (!v.IsObject()) {
        return Gameplay::Fail<std::map<std::string, std::string, std::less<>>>(
            "narrative", field);
    }
    if (v.Size() > ChapterConventions::MAX_VARIANTS) {
        return Gameplay::Fail<std::map<std::string, std::string, std::less<>>>(
            "narrative", "too many variants");
    }
    for (const auto& [key, val] : v.Members()) {
        if (key.empty() || key.size() > 64 || !val.IsString() ||
            val.AsString().size() >
                ChapterConventions::MAX_TEXT_LEN) {
            return Gameplay::Fail<std::map<std::string, std::string, std::less<>>>(
                "narrative", "bad variant");
        }
        m.emplace(key, val.AsString());
    }
    return Gameplay::Ok(std::move(m));
}

JsonValue VariantsToJson(const std::map<std::string, std::string, std::less<>>& m) {
    JsonValue::Object o;
    for (const auto& [k, v] : m) {
        o.emplace(k, JsonValue::String(v));
    }
    return JsonValue::MakeObject(std::move(o));
}

} // namespace

FrameMood LedgerMood(const Ledger& l) {
    const std::int64_t corruption = FoldGovernance(l).corruption;
    if (corruption >= MOOD_CORRUPT_AT) return FrameMood::Corrupt;
    if (corruption >= MOOD_TAINTED_AT) return FrameMood::Tainted;
    return FrameMood::Clean;
}

FrameOmen LedgerOmen(const Ledger& l) {
    const std::int64_t mandate = l.Balance(Account::Mandate);
    if (mandate >= OMEN_BLESSED_AT) return FrameOmen::Blessed;
    if (mandate >= OMEN_FADING_AT) return FrameOmen::Fading;
    return FrameOmen::Barren;
}

std::string_view MoodKey(FrameMood m) {
    switch (m) {
        case FrameMood::Clean: return "clean";
        case FrameMood::Tainted: return "tainted";
        case FrameMood::Corrupt: return "corrupt";
    }
    return "clean";
}

std::string_view OmenKey(FrameOmen o) {
    switch (o) {
        case FrameOmen::Blessed: return "blessed";
        case FrameOmen::Fading: return "fading";
        case FrameOmen::Barren: return "barren";
    }
    return "fading";
}

Gameplay::Result<ChapterConventions>
ChapterConventions::FromJson(const JsonValue& doc) {
    const std::string* s = doc.FindString("schema");
    if (s == nullptr || *s != SCHEMA) {
        return Gameplay::Fail<ChapterConventions>("narrative",
                                                  "bad schema");
    }
    const JsonValue& chapters = doc["chapters"];
    if (!chapters.IsObject() || chapters.Size() > MAX_CHAPTERS) {
        return Gameplay::Fail<ChapterConventions>("narrative",
                                                  "bad chapters");
    }
    ChapterConventions conv;
    for (const auto& [id, cv] : chapters.Members()) {
        if (id.empty() || id.size() > MAX_ID_LEN || !cv.IsObject()) {
            return Gameplay::Fail<ChapterConventions>("narrative",
                                                      "bad chapter id");
        }
        ChapterFrame f;
        const std::string* fp = cv.FindString("frontispiece");
        if (fp != nullptr) {
            if (fp->size() > MAX_TEXT_LEN) {
                return Gameplay::Fail<ChapterConventions>(
                    "narrative", "frontispiece too long");
            }
            f.frontispiece = *fp;
        }
        if (cv.Has("judgment")) {
            auto j = ParseVariants(cv["judgment"], "judgment");
            if (!j.ok()) {
                return Gameplay::Fail<ChapterConventions>(
                    "narrative", j.reason);
            }
            f.judgment = std::move(j.value);
        }
        if (cv.Has("cliffhanger")) {
            auto ch = ParseVariants(cv["cliffhanger"], "cliffhanger");
            if (!ch.ok()) {
                return Gameplay::Fail<ChapterConventions>(
                    "narrative", ch.reason);
            }
            f.cliffhanger = std::move(ch.value);
        }
        conv.frames_[id] = std::move(f);
    }
    return Gameplay::Ok(std::move(conv));
}

JsonValue ChapterConventions::ToJson() const {
    JsonValue::Object root;
    root["schema"] = JsonValue::String(std::string(SCHEMA));
    JsonValue::Object chapters;
    for (const auto& [id, f] : frames_) {
        JsonValue::Object c;
        if (!f.frontispiece.empty()) {
            c["frontispiece"] = JsonValue::String(f.frontispiece);
        }
        if (!f.judgment.empty()) {
            c["judgment"] = VariantsToJson(f.judgment);
        }
        if (!f.cliffhanger.empty()) {
            c["cliffhanger"] = VariantsToJson(f.cliffhanger);
        }
        chapters[id] = JsonValue::MakeObject(std::move(c));
    }
    root["chapters"] = JsonValue::MakeObject(std::move(chapters));
    return JsonValue::MakeObject(std::move(root));
}

const ChapterFrame*
ChapterConventions::Find(std::string_view chapterId) const {
    auto it = frames_.find(chapterId);
    return it == frames_.end() ? nullptr : &it->second;
}

std::string RenderChapterOpen(const ChapterConventions& conv,
                              std::string_view chapterId) {
    const ChapterFrame* f = conv.Find(chapterId);
    // Court commission register — imperative, formulaic; the header
    // names the mandate's target, the body is the sealed text.
    std::string s = "敕命·";
    s += chapterId;
    s += "\n奉敕：";
    if (f != nullptr && !f->frontispiece.empty()) {
        s += f->frontispiece;
    } else {
        s += "爾其整飭師旅，克復疆土，毋怠。"; // generic mandate
    }
    s += "\n";
    return s;
}

std::string RenderChapterClose(const ChapterConventions& conv,
                               std::string_view chapterId,
                               const Ledger& ledger) {
    const ChapterFrame* f = conv.Find(chapterId);
    std::string s = "評曰：";
    std::string_view j =
        f ? Variant(f->judgment, MoodKey(LedgerMood(ledger)))
          : std::string_view();
    s += j.empty() ? "是役也，勝負已判，功過在冊。" : j;
    s += "\n";
    std::string_view c =
        f ? Variant(f->cliffhanger, OmenKey(LedgerOmen(ledger)))
          : std::string_view();
    if (!c.empty()) {
        s += c;
    } else {
        s += "後事如何";
    }
    s += "——且聽下回分解。\n";
    return s;
}

} // namespace Potato::Campaign
