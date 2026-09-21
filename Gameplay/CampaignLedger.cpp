#include "CampaignLedger.h"

#include "Serialization/JsonParser.h"

#include <cstdio>

namespace Potato {
namespace Gameplay {

const char* DispositionName(GeneralDisposition d) {
    switch (d) {
    case GeneralDisposition::Subdued:  return "Subdued";
    case GeneralDisposition::Slain:    return "Slain";
    case GeneralDisposition::Defected: return "Defected";
    case GeneralDisposition::Retired:  return "Retired";
    case GeneralDisposition::Negotiated:  return "Negotiated";
    case GeneralDisposition::Intimidated: return "Intimidated";
    case GeneralDisposition::Unknown:  return "Unknown";
    }
    return "Unknown";
}

bool DispositionFromName(const std::string& name,
                         GeneralDisposition& out) {
    static const GeneralDisposition all[] = {
        GeneralDisposition::Subdued, GeneralDisposition::Slain,
        GeneralDisposition::Defected, GeneralDisposition::Retired,
        GeneralDisposition::Negotiated, GeneralDisposition::Intimidated,
        GeneralDisposition::Unknown,
    };
    for (GeneralDisposition d : all) {
        if (name == DispositionName(d)) {
            out = d;
            return true;
        }
    }
    return false;
}

void CampaignLedger::RecordDisposition(const std::string& id,
                                       const std::string& name,
                                       int ch, GeneralDisposition disp) {
    for (auto& r : records) {
        if (r.generalId == id) {
            r.generalName = name;
            r.chapter = ch;
            r.disposition = disp;
            return;
        }
    }
    records.push_back({id, name, ch, disp});
}

const CampaignLedger::DispositionRecord*
CampaignLedger::Find(const std::string& id) const {
    for (const auto& r : records) {
        if (r.generalId == id) {
            return &r;
        }
    }
    return nullptr;
}

void CampaignLedger::EarnTitle(const std::string& title, int ch) {
    titles.emplace_back(ch, title);
}

const std::string& CampaignLedger::CurrentTitle() const {
    static const std::string none;
    return titles.empty() ? none : titles.back().second;
}

void CampaignLedger::AdvanceChapter(int ch) {
    if (ch > chapter) {
        chapter = ch;
    }
}

std::vector<std::string> CampaignLedger::EndingCitations() const {
    std::vector<std::string> lines;
    for (const auto& r : records) {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "第%d章·%s（%s）— %s",
                      r.chapter, r.generalName.c_str(),
                      r.generalId.c_str(),
                      DispositionName(r.disposition));
        lines.emplace_back(buf);
    }
    for (const auto& [ch, t] : titles) {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "第%d章·軍師稱號「%s」",
                      ch, t.c_str());
        lines.emplace_back(buf);
    }
    return lines;
}

std::string CampaignLedger::ToJson() const {
    std::string out =
        "{\"schema\":\"potato.campaign_ledger/1\",\"chapter\":";
    out += std::to_string(chapter);
    out += ",\"records\":[";
    for (size_t i = 0; i < records.size(); ++i) {
        const auto& r = records[i];
        char buf[384];
        std::snprintf(buf, sizeof(buf),
                      "{\"id\":\"%s\",\"name\":\"%s\",\"chapter\":%d,"
                      "\"disposition\":\"%s\"}%s",
                      r.generalId.c_str(), r.generalName.c_str(),
                      r.chapter, DispositionName(r.disposition),
                      i + 1 < records.size() ? "," : "");
        out += buf;
    }
    out += "],\"titles\":[";
    for (size_t i = 0; i < titles.size(); ++i) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "{\"chapter\":%d,\"title\":\"%s\"}%s",
                      titles[i].first, titles[i].second.c_str(),
                      i + 1 < titles.size() ? "," : "");
        out += buf;
    }
    out += "]}";
    return out;
}

bool CampaignLedger::FromJson(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) ||
        root["schema"].AsString() != "potato.campaign_ledger/1") {
        return false;
    }
    chapter = root["chapter"].AsInt(0);
    records.clear();
    for (const JsonValue& r : root["records"].AsArray()) {
        DispositionRecord rec;
        rec.generalId = r["id"].AsString();
        rec.generalName = r["name"].AsString();
        rec.chapter = r["chapter"].AsInt(0);
        if (!DispositionFromName(r["disposition"].AsString(),
                                 rec.disposition)) {
            return false;
        }
        records.push_back(rec);
    }
    titles.clear();
    for (const JsonValue& t : root["titles"].AsArray()) {
        titles.emplace_back(t["chapter"].AsInt(0),
                            t["title"].AsString());
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
