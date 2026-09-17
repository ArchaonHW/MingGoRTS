# 素材庫 (assets/)

MingGoRTS 遊戲素材庫。所有素材以 JSON 描述 + PNG 卡面圖組成，可直接被引擎的
`Serialization/JsonParser.h` 解析。

## 目錄結構

```
assets/
├── manifest.json          # 素材庫總索引(所有素材的 id/路徑/稀有度)
└── cards/
    ├── roc/               # 民國名將角色卡資料(*.json)
    └── art/               # 卡面圖(*.png, 512×768)
```

## 角色卡格式 (`schema: potato.character_card/1`)

| 欄位 | 說明 |
|---|---|
| `id` | 唯一識別碼(`roc_<拼音>`) |
| `name` / `courtesyName` / `epithet` | 姓名 / 字 / 稱號 |
| `rarity` | `legendary`(傳奇·金框)或 `epic`(史詩·紫框) |
| `faction` | 所屬派系(中央軍/桂系/東北軍/西北軍/晉綏軍/遠征軍) |
| `personality` | 人格三軸:`aggression`/`discipline`/`cunning`(0–100,對齊 brainstorming 敵將人格向量) |
| `stats` | `command`/`attack`/`defense`/`mobility`/`moraleAura`(0–100) |
| `signatureDoctrine` | 專屬 doctrine 卡——`trigger`/`action` 字串**直接對齊 `Gameplay/Doctrine.h` 的 enum 名稱**,`threshold` 為觸發參數 |
| `lore` / `historicalBattle` | 傳記文與代表戰役 |
| `art` | 卡面圖相對路徑 |

## 收錄名將

**傳奇 (5)**:蔣介石·中正、張學良·漢卿、李宗仁·德鄰、薛岳·伯陵、孫立人·仲倫
**史詩 (5)**:張自忠·藎臣、戴安瀾·衍功、白崇禧·健生、馮玉祥·煥章、閻錫山·百川

## 與引擎整合

- `signatureDoctrine` 可直接轉成 `DoctrineRule`:
  `DoctrineTrigger`/`DoctrineAction` 的 enum 值與 JSON 字串同名,
  載入時做字串→enum 映射即可掛進 `DoctrineSet::AddRule`
- 卡面圖由 `tools/generate_roc_cards.py` 程式化生成;
  修改該腳本的 `CARDS` 表後重跑即可同步更新 JSON + PNG + manifest
