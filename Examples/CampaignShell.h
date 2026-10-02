#pragma once

#include "Campaign/CampaignFlow.h"
#include "CharacterArt.h"
#include "Gameplay/ChapterConventions.h"
#include "Serialization/JsonWriter.h"
#include <filesystem>

#include "Campaign/Checkpoint.h"
using Potato::Campaign::CommitCampaign;

static const char *CampaignOutcomeName(const std::string &value) {
    if (value == "victory")
        return "勝利";
    if (value == "defeat")
        return "失利";
    if (value == "peace")
        return "無戰通行";
    return "平手";
}

// 重開只根據持久階段導頁；不把上次停留的 UI 頁面當作已完成的戰果。
static ShellScreen ResumeCampaign(const Potato::Campaign::CampaignState &state) {
    using Potato::Campaign::CampaignStage;
    if (state.progress.stage == CampaignStage::Complete)
        return ShellScreen::Complete;
    if (state.progress.stage == CampaignStage::Aftermath)
        return ShellScreen::Aftermath;
    return ShellScreen::Story;
}

// 每次只繪製一幀並傳回欲切換的 ShellScreen。章節、字體、肖像已在外層載入，
// 本頁不逐幀解碼資產；只有玩家執行決策／整補等操作時才透過 commit 寫檔。
static ShellScreen
CampaignFrame(GLFWwindow *window, OpenGLRenderer &renderer, ShellScreen screen,
              Campaign::CampaignState &state, const Campaign::ChapterLibrary &library,
              const Campaign::CampaignFlow &flow, const SquadTemplateLibrary &templates,
              const std::string &savePath, std::string &error, Texture *portraits, ImFont *sans,
              ImFont *serif, UITheme::Id &theme, float &uiScale, const char *capture = nullptr) {
    renderer.PollEvents();
    static float appliedScale = 1.0f;
    static UITheme::Id appliedTheme = UITheme::Id::TacticalSim;
    if (appliedTheme != theme) {
        UITheme::Apply(ImGui::GetStyle(), theme);
        appliedTheme = theme;
        appliedScale = 1.0f;
    }
    if (uiScale != appliedScale) {
        ImGui::GetStyle().ScaleAllSizes(uiScale / appliedScale);
        appliedScale = uiScale;
    }
    ImGui::GetIO().FontGlobalScale = uiScale;
    int w = 0, h = 0, dw = 0, dh = 0;
    glfwGetWindowSize(window, &w, &h);
    glfwGetFramebufferSize(window, &dw, &dh);
    if (dw <= 0 || dh <= 0)
        return screen;
    renderer.SetViewport(0, 0, dw, dh);
    renderer.SetClearColor(Vector3(.07f, .09f, .08f));
    renderer.Clear();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushFont(sans, 18.0f);
    const auto *chapter = flow.Current(state);
    ShellScreen next = screen;
    auto commit = [&](auto action) { return CommitCampaign(state, flow, savePath, error, action); };
    const float margin = w >= 1000 ? 40.0f : 20.0f;
    auto *bg = ImGui::GetBackgroundDrawList();
    bg->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(float(w), float(h)), IM_COL32(40, 47, 37, 255),
                                IM_COL32(16, 23, 21, 255), IM_COL32(9, 15, 14, 255),
                                IM_COL32(29, 33, 26, 255));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(float(w), float(h)));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(margin, 24));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 10));
    ImGui::Begin("##campaign", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBackground);
    ImGui::TextColored(UITheme::C(0xc2ad71), "MINGGORTS  /  戰役史卷  /  第一卷 · 七章");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 10));
    if (w >= 1000 && portraits) {
        const float pw = 235.0f, ph = 335.0f;
        auto p = ImGui::GetCursorScreenPos();
        auto end = ImVec2(p.x + pw, p.y + ph);
        CharacterArt::DrawPortrait(ImGui::GetWindowDrawList(), portraits,
                                   screen == ShellScreen::Aftermath ? 1 : 0, p, end);
        ImGui::GetWindowDrawList()->AddRect(p, end, IM_COL32(194, 173, 113, 180));
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(pw, ph));
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + pw);
        ImGui::TextColored(UITheme::C(0xc2ad71), "執筆者 · 行軍帳");
        ImGui::TextWrapped("每次勝利都留下代價。傷亡、決策與具名軍官會跟著你走入下一章。");
        ImGui::Separator();
        for (const auto &c : library.Chapters()) {
            const bool done =
                std::find(state.progress.completed.begin(), state.progress.completed.end(), c.id) !=
                state.progress.completed.end();
            ImGui::TextColored(c.id == state.chapter.chapterId ? UITheme::C(0xc2ad71)
                                                               : UITheme::C(0xa7ae9c),
                               "%s %d · %s",
                               done                              ? "✓"
                               : c.id == state.chapter.chapterId ? "›"
                                                                 : "·",
                               c.number, c.title.c_str());
        }
        ImGui::PopTextWrapPos();
        ImGui::EndGroup();
        ImGui::SameLine(0, 30);
    }
    ImGui::BeginGroup();
    ImGui::BeginChild("##campaign-content", ImVec2(0, std::max(200.0f, float(h) - 125)), false);
    ImGui::PushTextWrapPos(0);
    const char *heading = screen == ShellScreen::Title      ? "一筆，改寫戰局"
                          : screen == ShellScreen::Complete ? "第一卷 · 史卷收束"
                          : chapter                         ? chapter->title.c_str()
                                                            : "戰役";
    ImGui::PushFont(serif ? serif : sans, w >= 1000 ? 42.0f : 30.0f);
    ImGui::TextUnformatted(heading);
    ImGui::PopFont();
    if (screen == ShellScreen::Title) {
        ImGui::TextWrapped("從斷橋出發，穿過焦土、關城與同盟，走到神話的渡口。部隊不會在下一章自動"
                           "重置。你的決策會決定第三章是否需要開戰。");
        if (state.progress.initialized) {
            ImGui::TextColored(UITheme::C(0xc2ad71), "已存：第 %d 章 · %s", state.chapter.chapter,
                               chapter->title.c_str());
            if (ImGui::Button("繼續戰役", ImVec2(260, 52)))
                next = ResumeCampaign(state);
        }
        if (ImGui::Button("開始新戰役", ImVec2(260, 44)))
            ImGui::OpenPopup("建立新戰役");
        if (ImGui::BeginPopupModal("建立新戰役", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted("將建立新的行軍帳。已有存檔會先備份為 campaign.previous.json。");
            if (ImGui::Button("確認建立")) {
                // 建立新戰役是明確覆寫操作；備份失敗就停止，不犧牲舊存檔。
                std::error_code ec;
                if (std::filesystem::exists(savePath)) {
                    auto backup =
                        std::filesystem::path(savePath).parent_path() / "campaign.previous.json";
                    std::filesystem::copy_file(
                        savePath, backup, std::filesystem::copy_options::overwrite_existing, ec);
                }
                if (ec)
                    error = "無法備份原存檔，未建立新戰役：" + ec.message();
                else if (commit([&](auto &s, auto &e) { return flow.StartNew(s, e); }))
                    next = ShellScreen::Story;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("取消"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
        ImGui::TextWrapped("自動存檔：戰前決策、整補、戰後結算與章節推進。戰鬥中離開後，重開會從該"
                           "場戰前檢查點接續。");
        ImGui::TextDisabled("左鍵選隊 · 右鍵下令 · Space 暫停 · WASD 移動 · 滾輪縮放 · C 檢視角色");
        if (ImGui::CollapsingHeader("畫面設定")) {
            int selectedTheme = int(theme);
            if (ImGui::Combo("主題", &selectedTheme, "現代軍事\0泥濘沙盤\0軍電作戰室\0水墨史卷\0"))
                theme = UITheme::Id(selectedTheme);
            ImGui::SliderFloat("介面縮放", &uiScale, 1.0f, 1.5f, "%.2fx");
        }
        if (ImGui::Button("離開遊戲"))
            next = ShellScreen::Quit;
    } else if (chapter && screen == ShellScreen::Story) {
        ImGui::TextColored(UITheme::C(0xc2ad71), "第 %d 章 / %zu  ·  %s", chapter->number,
                           library.Chapters().size(), chapter->arc == 0 ? "諸侯篇" : "遠征篇");
        ImGui::TextWrapped("%s", chapter->opening.c_str());
        ImGui::TextColored(
            UITheme::C(0xc2ad71), "%s",
            ChapterConventions::OpeningVerse(chapter->number, chapter->title).c_str());
        ImGui::TextWrapped("守將：%s · %s。情報仍待戰場查證。", chapter->enemyGeneralName.c_str(),
                           chapter->enemyPersonality.c_str());
        if (chapter->number == 6) {
            const auto previous = state.progress.choices.find("coalition");
            ImGui::TextWrapped("%s", previous != state.progress.choices.end() &&
                                             previous->second == "alliance"
                                         ? "曾經共擔軍糧的人如今截斷糧道。盟約的紙上還留著你的印，"
                                           "你得決定是否替尚未背誓的士卒留下退路。"
                                         : "你沒有加入盟約，仍被捲進同盟的內鬥。城外的求援信沒有你"
                                           "的印，卻有你曾在戰場見過的名字。");
        }
        ImGui::Separator();
        ImGui::TextWrapped("作戰目標：%s", chapter->objectiveText.c_str());
        const auto *selected = flow.SelectedChoice(state);
        if (!selected) {
            ImGui::TextDisabled("選擇後立即存檔；本章方向確定後不能重複領取補給或更換決策。");
            for (const auto &choice : chapter->choices) {
                ImGui::PushID(choice.id.c_str());
                std::string reason;
                bool allowed = state.Camp().GetLoot() >= choice.cost;
                if (choice.id == "negotiate" && !flow.CanResolvePeacefully(state, reason))
                    allowed = false;
                ImGui::TextColored(UITheme::C(0xc2ad71), "%s", choice.label.c_str());
                ImGui::TextWrapped("%s", choice.description.c_str());
                ImGui::TextDisabled("消耗 %d · 即時補給 %d · 勝利額外補給 %d", choice.cost,
                                    choice.supply, choice.reward);
                if (!reason.empty())
                    ImGui::TextWrapped("談判條件：%s", reason.c_str());
                ImGui::BeginDisabled(!allowed);
                if (ImGui::Button("確定此方向")) {
                    commit([&](auto &s, auto &e) { return flow.Choose(s, choice.id, e); });
                }
                ImGui::EndDisabled();
                ImGui::Separator();
                ImGui::PopID();
            }
        } else {
            ImGui::TextColored(UITheme::C(0xc2ad71), "已記錄決策：%s", selected->label.c_str());
            ImGui::TextWrapped("%s", selected->description.c_str());
            if (selected->holdSeconds > 0)
                ImGui::TextWrapped(
                    "保護後衛 %.0f "
                    "秒；若後衛已不在編，改保護最後一支生還部隊。保護對象全滅或潰逃即失敗。",
                    selected->holdSeconds);
            // 可戰兵力與傷兵池不同；只有傷兵時先醫治或招募，不能進空戰場。
            const bool canDeploy =
                std::any_of(state.Camp().GetUnits().begin(), state.Camp().GetUnits().end(),
                            [](const auto &u) { return u.members > 0; });
            ImGui::BeginDisabled(selected->id != "negotiate" && !canDeploy);
            if (ImGui::Button(selected->id == "negotiate" ? "交涉 · 無戰通行" : "進入戰前軍議",
                              ImVec2(280, 50))) {
                if (selected->id == "negotiate") {
                    if (commit([&](auto &s, auto &e) { return flow.CompletePeace(s, e); }))
                        next = ShellScreen::Aftermath;
                } else
                    next = ShellScreen::Battle;
            }
            ImGui::EndDisabled();
            if (!canDeploy && selected->id != "negotiate")
                ImGui::TextWrapped("目前沒有可出戰的兵力，請先醫治或招募。");
        }
    } else if (chapter && screen == ShellScreen::Aftermath) {
        const auto &report = state.progress.lastReport;
        const auto outcome = state.progress.outcomes.at(chapter->id);
        ImGui::TextColored(UITheme::C(0xc2ad71), "%s · 結算已存檔", CampaignOutcomeName(outcome));
        ImGui::TextWrapped("%s",
                           outcome == "peace"
                               ? "守將讀過你的行軍帳，放下關門。這一頁沒有新增傷亡，也沒有戰利品。"
                           : outcome == "victory" ? chapter->victoryText.c_str()
                                                  : chapter->defeatText.c_str());
        ImGui::Text("本章我軍：陣亡 %d · 傷兵 %d · 拾獲補給 %d", report.TotalDead(0),
                    report.TotalWounded(0), report.lootPoints);
        for (const auto &casualty : report.casualties)
            if (casualty.team == 0 && casualty.lost > 0)
                ImGui::Text("%s：陣亡 %d / 傷兵 %d%s", casualty.squadName.c_str(), casualty.dead,
                            casualty.wounded, casualty.eliminated ? " · 編制全滅" : "");
        ImGui::Separator();
        ImGui::TextWrapped("下一頁：%s", chapter->closingHook.c_str());
        if (ImGui::Button(chapter->number == int(library.Chapters().size())
                              ? "完成第一卷"
                              : "整補完成 · 前往下一章",
                          ImVec2(300, 50)))
            if (commit([&](auto &s, auto &e) { return flow.Advance(s, e); }))
                next = ResumeCampaign(state);
        ImGui::TextDisabled("下一章獲得地方補給 8；兵力不足 24 時會登記全新的 24 "
                            "人預備隊。舊部隊與陣亡軍官不會復活。");
    } else if (screen == ShellScreen::Complete) {
        ImGui::TextWrapped("七章行軍告一段落。這是諸侯篇至遠征篇的第一卷；後續時代與最終結局可以沿"
                           "同一份行軍帳擴充。");
        ImGui::Text("累計永久陣亡：%d", state.progress.cumulativeDead);
        for (const auto &c : library.Chapters()) {
            const auto selected = state.progress.choices.at(c.id);
            std::string label = selected;
            for (const auto &choice : c.choices)
                if (choice.id == selected)
                    label = choice.label;
            ImGui::Text("第 %d 章 · %s / %s / %s", c.number, c.title.c_str(), label.c_str(),
                        CampaignOutcomeName(state.progress.outcomes.at(c.id)));
        }
    }
    if (state.progress.initialized && screen != ShellScreen::Title) {
        ImGui::Separator();
        int members = 0, wounded = 0;
        for (const auto &u : state.Camp().GetUnits()) {
            members += u.members;
            wounded += u.wounded;
        }
        ImGui::TextColored(UITheme::C(0xc2ad71), "整補營 · 生還 %d · 傷兵 %d · 補給 %d", members,
                           wounded, state.Camp().GetLoot());
        for (const auto &u : state.Camp().GetUnits())
            ImGui::Text("%s / %s：%d 人 · 傷 %d", u.squadName.c_str(), u.captainName.c_str(),
                        u.members, u.wounded);
        if (screen != ShellScreen::Complete) {
            ImGui::BeginDisabled(wounded == 0 || state.Camp().GetLoot() == 0);
            if (ImGui::Button("醫治傷兵 · 每人 1 補給"))
                commit(
                    [](auto &s, auto &) { return s.Camp().HealWounded(s.Camp().GetLoot()) > 0; });
            ImGui::EndDisabled();
            if (ImGui::CollapsingHeader("補充編制 · 最多 12 隊"))
                for (const auto *tpl : templates.SortedByCost()) {
                    ImGui::PushID(tpl->id.c_str());
                    ImGui::BeginDisabled(state.Camp().GetLoot() < tpl->cost ||
                                         state.Camp().GetUnits().size() >= 12);
                    std::string label =
                        "招募 " + tpl->name + " · " + std::to_string(tpl->cost) + " 補給";
                    if (ImGui::Button(label.c_str()))
                        commit([&](auto &s, auto &) {
                            if (!s.Camp().Recruit(templates, tpl->id))
                                return false;
                            flow.RegisterCampRoster(s);
                            return true;
                        });
                    ImGui::EndDisabled();
                    ImGui::PopID();
                }
        }
        if (ImGui::CollapsingHeader("具名軍官 · 永久名冊")) {
            for (const auto &entry : state.NamedRoster().GetEntries())
                if (entry.team == 0)
                    ImGui::Text("%s · %s%s", entry.name.c_str(), entry.squadName.c_str(),
                                entry.alive ? "" : " · 陣亡");
        }
        if (ImGui::Button("返回主選單"))
            next = ShellScreen::Title;
    }
    if (!error.empty())
        ImGui::TextColored(ImVec4(1, .55f, .42f, 1), "%s", error.c_str());
    ImGui::PopTextWrapPos();
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::End();
    ImGui::PopStyleVar(3);
    ImGui::PopFont();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (capture && !CaptureFrame(capture, dw, dh))
        error = "畫面驗證擷取失敗";
    renderer.SwapBuffers();
    return next;
}
