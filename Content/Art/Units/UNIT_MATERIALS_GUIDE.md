# 單位材質配置指南

## 材質系統概述

為Sprint 1的單位系統創建基礎材質，支持不同狀態的視覺反饋。

---

## 🎨 材質類型

### 1. 基礎單位材質 (M_Unit_Base)
```
用途: 單位正常狀態顯示
參數:
- BaseColor: 可自定義單位顏色
- Metallic: 0.0 (非金屬)
- Roughness: 0.8 (較粗糙)
- Emissive: 0.0 (無發光)
```

### 2. 選中材質 (M_Unit_Selected)
```
用途: 單位被選中時的視覺反饋
參數:
- BaseColor: 基礎顏色 + 綠色調
- GlowIntensity: 1.0 (發光強度)
- EmissiveColor: 綠色 (0.0, 1.0, 0.0)
- PulseSpeed: 2.0 (脈動速度)
```

### 3. 懸停材質 (M_Unit_Hover)
```
用途: 鼠標懸停時的視覺反饋
參數:
- BaseColor: 基礎顏色 + 藍色調
- GlowIntensity: 0.5 (中等發光)
- EmissiveColor: 淺藍色 (0.5, 0.5, 1.0)
- PulseSpeed: 1.0 (慢脈動)
```

### 4. 受傷材質 (M_Unit_Damaged)
```
用途: 單位受傷時的視覺反饋
參數:
- BaseColor: 基礎顏色 + 紅色調
- DamageAmount: 0.0-1.0 (受傷程度)
- EmissiveColor: 紅色 (1.0, 0.0, 0.0)
- PulseSpeed: 3.0 (快脈動)
```

---

## 🔧 UE5材質實現

### 基礎材質節點結構
```
M_Unit_Base:
├── Vector Parameter (BaseColor)
├── Scalar Parameter (Metallic)
├── Scalar Parameter (Roughness)
├── Scalar Parameter (Emissive)
└── Texture Sample (BaseTexture)
```

### 選中材質節點結構
```
M_Unit_Selected:
├── Vector Parameter (BaseColor)
├── Scalar Parameter (GlowIntensity)
├── Vector Parameter (EmissiveColor)
├── Scalar Parameter (PulseSpeed)
├── Time Node
├── Sine Node (脈動效果)
├── Lerp Node (顏色混合)
└── Add Node (發光疊加)
```

---

## 📋 材質參數配置

### 通用參數
```json
{
  "base_color": {
    "default": [0.7, 0.7, 0.7, 1.0],
    "type": "Vector4",
    "description": "單位基礎顏色"
  },
  "metallic": {
    "default": 0.0,
    "type": "Scalar",
    "description": "金屬度"
  },
  "roughness": {
    "default": 0.8,
    "type": "Scalar",
    "description": "粗糙度"
  }
}
```

### 狀態特定參數
```json
{
  "glow_intensity": {
    "default": 0.0,
    "type": "Scalar",
    "description": "發光強度"
  },
  "damage_amount": {
    "default": 0.0,
    "type": "Scalar",
    "description": "受傷程度"
  },
  "pulse_speed": {
    "default": 1.0,
    "type": "Scalar",
    "description": "脈動速度"
  }
}
```

---

## 🎯 使用方法

### 在RTSUnit中使用
```cpp
// 在BeginPlay中初始化材質管理器
void ARTSUnit::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取材質管理器
    UUnitMaterialManager* MaterialManager = GetComponentByClass<UUnitMaterialManager>();
    
    if (MaterialManager)
    {
        // 設置基礎材質
        MaterialManager->BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Unit_Base"));
        MaterialManager->SelectedMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Unit_Selected"));
        MaterialManager->HoverMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Unit_Hover"));
        MaterialManager->DamagedMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_Unit_Damaged"));
        
        // 初始化材質
        MaterialManager->InitializeMaterials(MeshComponent);
    }
}
```

### 在藍圖中使用
```
1. 創建單位藍圖
2. 添加UnitMaterialManager組件
3. 在屬性面板設置材質引用
4. 在事件圖中調用InitializeMaterials
```

---

## 🎨 視覺效果示例

### 正常狀態
- 顏色: 灰色基調
- 光照: 標準PBR材質
- 效果: 無特殊效果

### 選中狀態
- 顏色: 綠色調
- 光照: 發光效果
- 效果: 脈動光暈

### 懸停狀態
- 顏色: 藍色調
- 光照: 輕微發光
- 效果: 柔和光暈

### 受傷狀態
- 顏色: 紅色調
- 光照: 閃爍效果
- 效果: 快速脈動

---

## 🚀 性能優化

### 材質實例化
- 使用動態材質實例
- 避免重複創建材質
- 合理使用材質參數

### 渲染優化
- 控制發光強度
- 使用LOD系統
- 批量渲染相同材質

### 內存管理
- 緩存材質引用
- 避免頻繁載入
- 使用材質集合

---

## 📋 測試清單

### 基礎功能
- [ ] 材質正常載入
- [ ] 參數正確設置
- [ ] 狀態切換正常
- [ ] 視覺效果正確

### 性能測試
- [ ] 100個單位渲染測試
- [ ] 材質切換性能測試
- [ ] 內存使用測試
- [ ] 幀率穩定性測試

### 兼容性測試
- [ ] 不同解析度測試
- [ ] 不同品質設置測試
- [ ] 跨平台兼容性測試
- [ ] 藍圖兼容性測試

---

**這些材質配置將為Sprint 1的單位系統提供完整的視覺反饋！**
