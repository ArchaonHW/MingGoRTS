#include "Campaign/Chapters/ChapterLibrary.h"

#include "Campaign/State/CampaignState.h"
#include "Gameplay/Json/Json.h"

#include <algorithm>
#include <cctype>
#include <set>

namespace Potato::Campaign {

// The library bound and the save format's bound must agree —
// drift breaks the 3.4 chapter-space contract.
static_assert(ChapterLibrary::MAX_CHAPTERS ==
                  CampaignState::MAX_CHAPTERS,
              "chapter space bound drifted from CampaignState");

using Gameplay::JsonValue;
using Gameplay::Result;

namespace {

// One file -> ChapterDef or a reason string. Pure validation:
// unknown members are ignored (forward-compat).
const char* ValidateChapter(const JsonValue& doc, ChapterDef& out) {
    if (!doc.IsObject()) return "root is not an object";
    const std::string* id = doc.FindString("id");
    const std::string* title = doc.FindString("title");
    const std::string* map = doc.FindString("map");
    const std::string* briefing = doc.FindString("briefing");
    if (!id || !title || !map || !doc["index"].IsInt() ||
        !doc["combat"].IsBool()) {
        return "missing/mistyped field";
    }
    if (id->empty() || id->size() > ChapterLibrary::MAX_ID_LEN) {
        return "id out of range";
    }
    if (title->empty() ||
        title->size() > ChapterLibrary::MAX_TITLE_LEN) {
        return "title out of range";
    }
    if (map->empty() || map->size() > ChapterLibrary::MAX_REF_LEN) {
        return "map ref out of range";
    }
    if (doc.Has("briefing") && !doc["briefing"].IsString()) {
        return "missing/mistyped field";
    }
    if (briefing &&
        briefing->size() > ChapterLibrary::MAX_BRIEFING_LEN) {
        return "briefing too long";
    }
    const std::int64_t idx = doc["index"].AsInt();
    if (idx < 0 ||
        static_cast<std::uint64_t>(idx) >=
            ChapterLibrary::MAX_CHAPTERS) {
        return "index out of range";
    }
    out.id = *id;
    out.index = idx;
    out.title = *title;
    out.map = *map;
    out.combat = doc["combat"].AsBool();
    out.briefing = briefing ? *briefing : "";
    return nullptr;
}

} // namespace

ChapterLoadResult ChapterLibrary::Load(
    const std::filesystem::path& dir, ChapterLibrary& out) {
    ChapterLoadResult res;

    std::error_code ec;
    std::vector<std::filesystem::path> files;
    for (std::filesystem::directory_iterator it(
             dir, std::filesystem::directory_options::none, ec);
         !ec && it != std::filesystem::directory_iterator();
         it.increment(ec)) {
        // Per-entry stat uses its OWN error channel — sharing `ec`
        // would let a stat failure be cleared by the next
        // increment() and silently drop the file.
        std::error_code ec2;
        const bool isFile = it->is_regular_file(ec2);
        if (ec2) {
            res.rejected.push_back(
                {it->path(), "io", "stat failed: " + ec2.message()});
            continue;
        }
        // Case-folded extension: X.JSON/X.Json are still content.
        std::string ext = it->path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(),
                       [](unsigned char c) {
                           return static_cast<char>(std::tolower(c));
                       });
        if (isFile && ext == ".json") {
            files.push_back(it->path());
        }
    }
    if (ec) {
        res.error = "io";
        res.reason = "chapter dir unreadable: " + ec.message();
        return res;
    }
    // Deterministic registration order: filename sort.
    std::sort(files.begin(), files.end());

    std::vector<ChapterDef> chapters;
    std::set<std::string> ids;
    std::set<std::int64_t> indexes;
    for (const std::filesystem::path& f : files) {
        const std::u8string u8 = f.u8string();
        Result<JsonValue> doc = Gameplay::Json::Load(
            std::string_view(
                reinterpret_cast<const char*>(u8.data()),
                u8.size()),
            SCHEMA);
        if (!doc.ok()) {
            res.rejected.push_back(
                {f, doc.error, doc.reason});
            continue;
        }
        ChapterDef def;
        if (const char* why =
                ValidateChapter(doc.value, def)) {
            res.rejected.push_back({f, "field", why});
            continue;
        }
        // Both checks BEFORE either commit — a file rejected for a
        // dup index must not consume its id for later files.
        if (ids.count(def.id)) {
            res.rejected.push_back(
                {f, "duplicate", "chapter id already registered"});
            continue;
        }
        if (indexes.count(def.index)) {
            res.rejected.push_back({f, "duplicate",
                                    "chapter index already used"});
            continue;
        }
        ids.insert(def.id);
        indexes.insert(def.index);
        chapters.push_back(std::move(def));
    }

    // Library order = campaign order.
    std::sort(chapters.begin(), chapters.end(),
              [](const ChapterDef& a, const ChapterDef& b) {
                  return a.index < b.index;
              });
    out.chapters_ = std::move(chapters);
    res.ok = true;
    return res;
}

const ChapterDef* ChapterLibrary::Find(std::string_view id) const {
    for (const ChapterDef& c : chapters_) {
        if (c.id == id) return &c;
    }
    return nullptr;
}

const ChapterDef* ChapterLibrary::AtIndex(std::int64_t index) const {
    for (const ChapterDef& c : chapters_) {
        if (c.index == index) return &c;
    }
    return nullptr;
}

} // namespace Potato::Campaign
