#pragma once

#include "../../Core/Engine/Types.h"
#include <string>
#include <vector>

namespace MingGoRTS {

// 渲染元素类型
enum class RenderElementType {
    Text,
    Line,
    Rectangle,
    Circle,
    Sprite,
    Map,
    UI_Panel,
    UI_Button,
    UI_Text
};

// 渲染元素
struct RenderElement {
    RenderElementType type;
    Vector2D position;
    Vector2D size;
    std::string text;
    int color;
    bool visible;
    int layer;  // 渲染层级
};

// 控制台渲染器 - 将游戏状态渲染为控制台输出
class ConsoleRenderer {
public:
    ConsoleRenderer();
    ~ConsoleRenderer();
    
    // 初始化
    bool Initialize(int width, int height);
    void Shutdown();
    
    // 渲染
    void Clear();
    void Present();
    
    // 绘制元素
    void DrawText(const std::string& text, int x, int y, int color = 7);
    void DrawLine(int x1, int y1, int x2, int y2, int color = 7);
    void DrawRectangle(int x, int y, int width, int height, int color = 7);
    void DrawFilledRectangle(int x, int y, int width, int height, int color = 7);
    void DrawCircle(int x, int y, int radius, int color = 7);
    
    // 地图渲染
    void DrawMapTile(int x, int y, char symbol, int color);
    void DrawUnit(int x, int y, char symbol, int factionColor);
    void DrawSettlement(int x, int y, const std::string& name, int factionColor);
    
    // UI 渲染
    void DrawPanel(int x, int y, int width, int height, const std::string& title);
    void DrawButton(int x, int y, int width, int height, const std::string& text, bool selected);
    void DrawProgressBar(int x, int y, int width, float progress, int color);
    
    // 辅助方法
    void SetCursorPosition(int x, int y);
    void SetColor(int color);
    void ResetColor();
    
    // 尺寸查询
    int GetWidth() const { return screenWidth_; }
    int GetHeight() const { return screenHeight_; }
    
    // 批量渲染
    void AddElement(const RenderElement& element);
    void RenderElements();
    void ClearElements();

private:
    int screenWidth_;
    int screenHeight_;
    std::vector<RenderElement> elements_;
    
    #ifdef _WIN32
    void* consoleHandle_;
    #endif
    
    void InitializeConsole();
    void RestoreConsole();
    void FlushBuffer();
};

} // namespace MingGoRTS
