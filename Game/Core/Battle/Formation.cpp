#include "Formation.h"
#include <cmath>

namespace MingGoRTS {

Formation::Formation(int id, const std::string& name)
    : id_(id)
    , name_(name)
    , rows_(1)
    , columns_(1)
    , spacingX_(1.0f)
    , spacingY_(1.0f)
    , defenseBonus_(0.0f)
    , attackBonus_(0.0f)
    , movementPenalty_(0.0f) {
}

Formation::~Formation() = default;

void Formation::SetFormationShape(int rows, int columns, float spacingX, float spacingY) {
    rows_ = rows;
    columns_ = columns;
    spacingX_ = spacingX;
    spacingY_ = spacingY;
}

void Formation::SetBonus(float defense, float attack, float movement) {
    defenseBonus_ = defense;
    attackBonus_ = attack;
    movementPenalty_ = movement;
}

std::vector<Vector2D> Formation::CalculatePositions(Vector2D center, float rotation, int soldierCount) const {
    std::vector<Vector2D> positions;
    
    float rad = rotation * 3.14159f / 180.0f;
    float cosRot = std::cos(rad);
    float sinRot = std::sin(rad);
    
    // 计算编队总尺寸
    float totalWidth = (columns_ - 1) * spacingX_;
    float totalDepth = (rows_ - 1) * spacingY_;
    
    // 起始偏移（中心对齐）
    float startX = -totalWidth / 2.0f;
    float startY = -totalDepth / 2.0f;
    
    int count = 0;
    for (int row = 0; row < rows_ && count < soldierCount; ++row) {
        for (int col = 0; col < columns_ && count < soldierCount; ++col) {
            // 本地坐标
            float localX = startX + col * spacingX_;
            float localY = startY + row * spacingY_;
            
            // 旋转
            float rotX = localX * cosRot - localY * sinRot;
            float rotY = localX * sinRot + localY * cosRot;
            
            // 世界坐标
            positions.emplace_back(center.x + rotX, center.y + rotY);
            
            ++count;
        }
    }
    
    return positions;
}

Formation Formation::CreateLineFormation() {
    Formation f(1, "Line");
    f.SetFormationShape(3, 40, 1.0f, 1.0f);
    f.SetBonus(0.0f, 0.1f, 0.0f);
    return f;
}

Formation Formation::CreateColumnFormation() {
    Formation f(2, "Column");
    f.SetFormationShape(20, 6, 1.0f, 1.0f);
    f.SetBonus(0.0f, 0.0f, -0.1f);
    return f;
}

Formation Formation::CreateWedgeFormation() {
    Formation f(3, "Wedge");
    f.SetFormationShape(10, 10, 1.0f, 1.0f);
    f.SetBonus(-0.1f, 0.3f, -0.2f);
    return f;
}

Formation Formation::CreateSquareFormation() {
    Formation f(4, "Square");
    f.SetFormationShape(12, 12, 1.0f, 1.0f);
    f.SetBonus(0.3f, 0.0f, -0.3f);
    return f;
}

Formation Formation::CreateSkirmishFormation() {
    Formation f(5, "Skirmish");
    f.SetFormationShape(8, 15, 2.0f, 2.0f);
    f.SetBonus(0.1f, 0.1f, 0.2f);
    return f;
}

Formation Formation::CreateShieldWallFormation() {
    Formation f(6, "Shield Wall");
    f.SetFormationShape(2, 40, 1.0f, 0.5f);
    f.SetBonus(0.4f, -0.1f, -0.2f);
    return f;
}

Formation Formation::CreatePhalanxFormation() {
    Formation f(7, "Phalanx");
    f.SetFormationShape(8, 16, 0.8f, 1.2f);
    f.SetBonus(0.5f, 0.2f, -0.3f);
    return f;
}

Formation Formation::CreateTestudoFormation() {
    Formation f(8, "Testudo");
    f.SetFormationShape(10, 12, 0.7f, 0.7f);
    f.SetBonus(0.6f, -0.2f, -0.4f);
    return f;
}

} // namespace MingGoRTS
