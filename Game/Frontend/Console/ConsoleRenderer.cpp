#include "ConsoleRenderer.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

namespace MingGoRTS {

ConsoleRenderer::ConsoleRenderer()
    : screenWidth_(80)
    , screenHeight_(25)
    #ifdef _WIN32
    , consoleHandle_(nullptr)
    #endif
{
}

ConsoleRenderer::~ConsoleRenderer() {
    Shutdown();
}

bool ConsoleRenderer::Initialize(int width, int height) {
    screenWidth_ = width;
    screenHeight_ = height;
    
    InitializeConsole();
    
    return true;
}

void ConsoleRenderer::Shutdown() {
    RestoreConsole();
    elements_.clear();
}

void ConsoleRenderer::InitializeConsole() {
    #ifdef _WIN32
    consoleHandle_ = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 设置控制台窗口大小
    COORD bufferSize;
    bufferSize.X = screenWidth_;
    bufferSize.Y = screenHeight_;
    SetConsoleScreenBufferSize(consoleHandle_, bufferSize);
    
    // 设置窗口信息
    SMALL_RECT windowInfo;
    windowInfo.Left = 0;
    windowInfo.Top = 0;
    windowInfo.Right = screenWidth_ - 1;
    windowInfo.Bottom = screenHeight_ - 1;
    SetConsoleWindowInfo(consoleHandle_, TRUE, &windowInfo);
    
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle_, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle_, &cursorInfo);
    #endif
}

void ConsoleRenderer::RestoreConsole() {
    #ifdef _WIN32
    if (consoleHandle_) {
        // 恢复光标
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(consoleHandle_, &cursorInfo);
        cursorInfo.bVisible = TRUE;
        SetConsoleCursorInfo(consoleHandle_, &cursorInfo);
        
        ResetColor();
    }
    #endif
}

void ConsoleRenderer::Clear() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    
    elements_.clear();
}

void ConsoleRenderer::Present() {
    RenderElements();
    FlushBuffer();
}

void ConsoleRenderer::DrawText(const std::string& text, int x, int y, int color) {
    SetCursorPosition(x, y);
    SetColor(color);
    std::cout << text;
    ResetColor();
}

void ConsoleRenderer::DrawLine(int x1, int y1, int x2, int y2, int color) {
    // 简化的线条绘制（Bresenham算法简化版）
    SetColor(color);
    
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        SetCursorPosition(x1, y1);
        std::cout << "*";
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    
    ResetColor();
}

void ConsoleRenderer::DrawRectangle(int x, int y, int width, int height, int color) {
    SetColor(color);
    
    // 上边框
    SetCursorPosition(x, y);
    for (int i = 0; i < width; ++i) {
        std::cout << "-";
    }
    
    // 下边框
    SetCursorPosition(x, y + height - 1);
    for (int i = 0; i < width; ++i) {
        std::cout << "-";
    }
    
    // 左右边框
    for (int i = 1; i < height - 1; ++i) {
        SetCursorPosition(x, y + i);
        std::cout << "|";
        SetCursorPosition(x + width - 1, y + i);
        std::cout << "|";
    }
    
    ResetColor();
}

void ConsoleRenderer::DrawFilledRectangle(int x, int y, int width, int height, int color) {
    SetColor(color);
    
    for (int row = 0; row < height; ++row) {
        SetCursorPosition(x, y + row);
        for (int col = 0; col < width; ++col) {
            std::cout << " ";
        }
    }
    
    ResetColor();
}

void ConsoleRenderer::DrawCircle(int x, int y, int radius, int color) {
    SetColor(color);
    
    // 简化的圆形绘制
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy <= radius * radius) {
                SetCursorPosition(x + dx, y + dy);
                std::cout << "*";
            }
        }
    }
    
    ResetColor();
}

void ConsoleRenderer::DrawMapTile(int x, int y, char symbol, int color) {
    SetCursorPosition(x, y);
    SetColor(color);
    std::cout << symbol;
    ResetColor();
}

void ConsoleRenderer::DrawUnit(int x, int y, char symbol, int factionColor) {
    SetCursorPosition(x, y);
    SetColor(factionColor);
    std::cout << "[" << symbol << "]";
    ResetColor();
}

void ConsoleRenderer::DrawSettlement(int x, int y, const std::string& name, int factionColor) {
    SetCursorPosition(x, y);
    SetColor(factionColor);
    std::cout << "[" << name.substr(0, 3) << "]";
    ResetColor();
}

void ConsoleRenderer::DrawPanel(int x, int y, int width, int height, const std::string& title) {
    DrawRectangle(x, y, width, height, 7);
    
    // 标题
    if (!title.empty()) {
        int titleX = x + (width - static_cast<int>(title.length())) / 2;
        DrawText(title, titleX, y, 15);
    }
}

void ConsoleRenderer::DrawButton(int x, int y, int width, int height, const std::string& text, bool selected) {
    int color = selected ? 12 : 7;  // 选中红色，默认白色
    DrawRectangle(x, y, width, height, color);
    
    int textX = x + (width - static_cast<int>(text.length())) / 2;
    int textY = y + height / 2;
    DrawText(text, textX, textY, color);
}

void ConsoleRenderer::DrawProgressBar(int x, int y, int width, float progress, int color) {
    int filledWidth = static_cast<int>(width * progress);
    
    SetCursorPosition(x, y);
    SetColor(color);
    
    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < filledWidth) {
            std::cout << "=";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "]";
    
    ResetColor();
}

void ConsoleRenderer::SetCursorPosition(int x, int y) {
    #ifdef _WIN32
    COORD coord;
    coord.X = static_cast<SHORT>(x);
    coord.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(consoleHandle_, coord);
    #else
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    #endif
}

void ConsoleRenderer::SetColor(int color) {
    #ifdef _WIN32
    SetConsoleTextAttribute(consoleHandle_, color);
    #else
    // ANSI color codes for Linux/Mac
    std::cout << "\033[" << (30 + (color % 8)) << "m";
    if (color >= 8) {
        std::cout << "\033[1m";  // bright
    }
    #endif
}

void ConsoleRenderer::ResetColor() {
    #ifdef _WIN32
    SetConsoleTextAttribute(consoleHandle_, 7);  // 默认白色
    #else
    std::cout << "\033[0m";
    #endif
}

void ConsoleRenderer::AddElement(const RenderElement& element) {
    elements_.push_back(element);
}

void ConsoleRenderer::RenderElements() {
    // 按层级排序
    std::sort(elements_.begin(), elements_.end(), 
        [](const RenderElement& a, const RenderElement& b) {
            return a.layer < b.layer;
        });
    
    // 渲染每个元素
    for (const auto& elem : elements_) {
        if (!elem.visible) continue;
        
        switch (elem.type) {
            case RenderElementType::Text:
                DrawText(elem.text, static_cast<int>(elem.position.x), 
                        static_cast<int>(elem.position.y), elem.color);
                break;
            case RenderElementType::Rectangle:
                DrawRectangle(static_cast<int>(elem.position.x), 
                             static_cast<int>(elem.position.y),
                             static_cast<int>(elem.size.x), 
                             static_cast<int>(elem.size.y), elem.color);
                break;
            case RenderElementType::Line:
                // 简化处理
                break;
            default:
                break;
        }
    }
}

void ConsoleRenderer::ClearElements() {
    elements_.clear();
}

void ConsoleRenderer::FlushBuffer() {
    std::cout << std::flush;
}

} // namespace MingGoRTS
