# ADR-001: 選擇 Unreal Engine 5 作為遊戲引擎

## 決策
採用 **Unreal Engine 5.4+** 作為開發引擎，支援 C++ 與 Blueprint 混合開發。

## 背景
- 需要支援大規模單位（100+）
- 目標平台：Windows + Android + iOS
- 團隊對 UE5 有相關經驗
- 需要高質量 rendering 和 Physics

## 考量因素
| 選項 | 優點 | 缺點 |
|------|------|------|
| **Unreal Engine 5** | Nanite、Lumen、Blueprint 可視化編程 | 包體大、學習曲線陡 |
| Unity | 上手套快、資源豐富 | C# 性能不如 C++，大規模單位 rendering 較吃力 |
| Godot | 輕量、開源 | 3D 遊戲生態尚不成熟 |
| 自研引擎 | 完全控制 | 開發成本過高，時間不够 |

## 決策理由
1. **性能需求**：UE5 的 Nanite 可處理數千個實例
2. **跨平台**：UE5 官方支援 Android/iOS 打包
3. **AI 集成**：Python 與 C++ 混合編程方便（AI 工具用 Python）
4. **生態**：Marketplace 有大量現成資源（材質、模型）

## 後果
- 學習成本：團隊需要學習 UE5 C++ 和 Blueprint
- 包體大小：基底安裝約 100GB，需通知玩家
- 授權：商業使用免費（5% 收入分成，超過 100 萬美元）

---
*狀態：已接受*
*日期：2026-03-15*