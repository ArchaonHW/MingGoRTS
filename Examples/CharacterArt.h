#pragma once

#include "Rendering/OpenGLRenderer.h"
#include "Rendering/ImageCodec.h"
#include "imgui.h"
#include <algorithm>
#include <fstream>
#include <iterator>

namespace CharacterArt {

// Texture::LoadFromFile 尚是白色佔位實作；明確解碼本圖集再上傳像素。
inline bool LoadAtlas(Potato::Texture& texture, const std::string& path, std::string& error) {
    std::ifstream input(path, std::ios::binary);
    if (!input) { error = "portrait file not found: " + path; return false; }
    const std::vector<Potato::uint8> bytes((std::istreambuf_iterator<char>(input)),
                                          std::istreambuf_iterator<char>());
    std::vector<Potato::uint8> pixels;
    int width = 0, height = 0;
    if (!Potato::ImageCodec::DecodePNG(bytes.data(), bytes.size(), pixels, width, height, &error)) return false;
    if (width != height * 3) { error = "portrait atlas must contain three equal square panels"; return false; }
    if (!texture.LoadFromMemory(pixels.data(), width, height, 4)) {
        error = "portrait upload failed";
        return false;
    }
    return true;
}

// 三幅等寬方形軍官肖像共用圖集，依顯示框比例裁切而不拉伸。
inline void DrawPortrait(ImDrawList* draw, Potato::Texture* texture, int index,
                         ImVec2 minimum, ImVec2 maximum) {
    if (!texture || maximum.x <= minimum.x || maximum.y <= minimum.y) return;
    index = std::clamp(index, 0, 2);
    const float aspect = (maximum.x - minimum.x) / (maximum.y - minimum.y);
    const float cropX = aspect < 1.0f ? (1.0f - aspect) * 0.5f : 0.0f;
    const float cropY = aspect > 1.0f ? (1.0f - 1.0f / aspect) * 0.5f : 0.0f;
    draw->AddImage(static_cast<ImTextureID>(texture->GetTextureID()), minimum, maximum,
                   ImVec2((index + cropX) / 3.0f, cropY),
                   ImVec2((index + 1.0f - cropX) / 3.0f, 1.0f - cropY));
}

inline void Portrait(Potato::Texture* texture, int index, ImVec2 size) {
    const ImVec2 p = ImGui::GetCursorScreenPos();
    DrawPortrait(ImGui::GetWindowDrawList(), texture, index, p,
                 ImVec2(p.x + size.x, p.y + size.y));
    ImGui::Dummy(size);
}

} // namespace CharacterArt
