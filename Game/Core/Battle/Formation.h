#pragma once

#include "../Engine/Types.h"
#include <vector>
#include <string>

namespace MingGoRTS {

// 编队配置 - 定义单位的排列方式
struct FormationConfig {
    std::string name;
    int rows;           // 行数
    int columns;        // 列数
    float spacingX;     // 横向间距
    float spacingY;     // 纵向间距
    float width;        // 编队宽度
    float depth;        // 编队深度
    float defenseBonus;
    float attackBonus;
    float movementPenalty;
    bool canRotate;
};

// 编队 - 管理单位在战场上的排列
class Formation {
public:
    Formation(int id, const std::string& name);
    ~Formation();
    
    // 编队配置
    int GetId() const { return id_; }
    const std::string& GetName() const { return name_; }
    
    // 阵型参数
    void SetFormationShape(int rows, int columns, float spacingX, float spacingY);
    void SetBonus(float defense, float attack, float movement);
    
    int GetRows() const { return rows_; }
    int GetColumns() const { return columns_; }
    float GetSpacingX() const { return spacingX_; }
    float GetSpacingY() const { return spacingY_; }
    
    // 加成
    float GetDefenseBonus() const { return defenseBonus_; }
    float GetAttackBonus() const { return attackBonus_; }
    float GetMovementPenalty() const { return movementPenalty_; }
    
    // 计算士兵位置
    std::vector<Vector2D> CalculatePositions(Vector2D center, float rotation, int soldierCount) const;
    
    // 预定义编队类型
    static Formation CreateLineFormation();
    static Formation CreateColumnFormation();
    static Formation CreateWedgeFormation();
    static Formation CreateSquareFormation();
    static Formation CreateSkirmishFormation();
    static Formation CreateShieldWallFormation();
    static Formation CreatePhalanxFormation();
    static Formation CreateTestudoFormation();

private:
    int id_;
    std::string name_;
    
    int rows_;
    int columns_;
    float spacingX_;
    float spacingY_;
    
    float defenseBonus_;
    float attackBonus_;
    float movementPenalty_;
};

} // namespace MingGoRTS
