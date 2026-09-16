#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
民國名將角色卡素材生成器
產出:
  assets/cards/roc/*.json   — 卡片資料(對齊 Gameplay/Doctrine.h 的 trigger/action enum)
  assets/cards/art/*.png    — 卡面圖(PIL 程式化繪製:稀有度邊框 + 名號 + 數值)
  assets/manifest.json      — 素材庫索引

用法: python tools/generate_roc_cards.py
"""
import json
import math
import os
from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CARDS_DIR = os.path.join(ROOT, "assets", "cards", "roc")
ART_DIR = os.path.join(ROOT, "assets", "cards", "art")

# ---------------------------------------------------------------------------
# 卡片資料
# trigger/action 字串對齊 Gameplay/Doctrine.h:
#   trigger: Always HealthBelow MoraleBelow EnemyInRange UnderAttack
#            Outnumbered AllyEngaged ObjectiveReached
#   action:  AttackNearest AttackWeakest AdvanceToObjective HoldPosition
#            RetreatToRally DefendNearestAlly
# ---------------------------------------------------------------------------
CARDS = [
    # ===== 傳奇 =====
    dict(id="roc_chiang_kaishek", name="蔣介石", courtesy="中正",
         epithet="黃埔校長", rarity="legendary", faction="國民政府",
         role="commander",
         personality=dict(aggression=65, discipline=75, cunning=85),
         stats=dict(command=95, attack=70, defense=80, mobility=55, moraleAura=95),
         doctrine=dict(name="空間換時間", trigger="MoraleBelow", threshold=30.0,
                       action="RetreatToRally",
                       description="士氣低落時有序後撤,以縱深消耗敵軍——持久抗戰的總體戰略"),
         lore="黃埔軍校校長,北伐統一中國,領導八年抗戰。一生在軍事與政治的鋼索上行走。",
         battle="北伐戰爭 1926-1928"),
    dict(id="roc_zhang_xueliang", name="張學良", courtesy="漢卿",
         epithet="少帥", rarity="legendary", faction="東北軍",
         role="commander",
         personality=dict(aggression=50, discipline=60, cunning=70),
         stats=dict(command=80, attack=72, defense=68, mobility=85, moraleAura=78),
         doctrine=dict(name="西安兵諫", trigger="AllyEngaged", threshold=0.0,
                       action="DefendNearestAlly",
                       description="友軍接戰即馳援——兵諫的膽魄化為戰場上的救援直覺"),
         lore="東北易幟促成統一,西安事變改寫歷史。半生戎馬,半生軟禁,世紀滄桑。",
         battle="西安事變 1936"),
    dict(id="roc_li_zongren", name="李宗仁", courtesy="德鄰",
         epithet="台兒莊的磐石", rarity="legendary", faction="桂系",
         role="commander",
         personality=dict(aggression=55, discipline=82, cunning=68),
         stats=dict(command=92, attack=78, defense=90, mobility=58, moraleAura=88),
         doctrine=dict(name="台兒莊誘敵", trigger="Outnumbered", threshold=0.0,
                       action="HoldPosition",
                       description="寡不敵眾時死守陣地,誘敵深入後外圍合圍——台兒莊大捷的戰法"),
         lore="桂系首領,指揮台兒莊大捷,抗戰正面戰場首次大勝。後任中華民國代總統。",
         battle="台兒莊大捷 1938"),
    dict(id="roc_xue_yue", name="薛岳", courtesy="伯陵",
         epithet="長沙之虎", rarity="legendary", faction="中央軍",
         role="commander",
         personality=dict(aggression=85, discipline=80, cunning=75),
         stats=dict(command=90, attack=88, defense=85, mobility=70, moraleAura=82),
         doctrine=dict(name="天爐戰法", trigger="EnemyInRange", threshold=20.0,
                       action="AttackWeakest",
                       description="敵軍進入伏擊圈即猛攻其最弱環節——四次長沙會戰的爐心"),
         lore="第九戰區司令長官,四次長沙會戰重創日軍,殲敵數為抗戰諸將之冠。",
         battle="第三次長沙會戰 1941"),
    dict(id="roc_sun_liren", name="孫立人", courtesy="仲倫",
         epithet="東方隆美爾", rarity="legendary", faction="遠征軍",
         role="commander",
         personality=dict(aggression=80, discipline=90, cunning=65),
         stats=dict(command=88, attack=90, defense=75, mobility=82, moraleAura=85),
         doctrine=dict(name="仁安羌突擊", trigger="AllyEngaged", threshold=0.0,
                       action="AttackNearest",
                       description="友軍被圍立即强攻解围——仁安羌以寡擊眾救出英軍七千"),
         lore="維吉尼亞軍校出身,新一軍軍長。仁安羌大捷名揚國際,緬北反攻屢建奇功。",
         battle="仁安羌大捷 1942"),
    # ===== 史詩 =====
    dict(id="roc_zhang_zizhong", name="張自忠", courtesy="藎臣",
         epithet="梅花將軍", rarity="epic", faction="西北軍",
         role="commander",
         personality=dict(aggression=88, discipline=78, cunning=55),
         stats=dict(command=82, attack=90, defense=80, mobility=72, moraleAura=92),
         doctrine=dict(name="決死衝鋒", trigger="HealthBelow", threshold=0.3,
                       action="AttackNearest",
                       description="兵力瀕危時發起決死突擊——南瓜店最後的衝鋒,以死明志"),
         lore="第三十三集團軍總司令,棗宜會戰壯烈殉國,為二戰同盟國陣亡最高階將領。",
         battle="棗宜會戰 1940"),
    dict(id="roc_dai_anlan", name="戴安瀾", courtesy="衍功",
         epithet="海鷗將軍", rarity="epic", faction="遠征軍",
         role="commander",
         personality=dict(aggression=75, discipline=85, cunning=60),
         stats=dict(command=80, attack=82, defense=88, mobility=68, moraleAura=86),
         doctrine=dict(name="同古死守", trigger="ObjectiveReached", threshold=0.0,
                       action="HoldPosition",
                       description="抵達目標點後死守不退——同古保衛戰十二晝夜的鋼鐵防線"),
         lore="第二百師師長,遠征緬甸血戰同古,撤退途中壯烈殉國,年僅三十八歲。",
         battle="同古保衛戰 1942"),
    dict(id="roc_bai_chongxi", name="白崇禧", courtesy="健生",
         epithet="小諸葛", rarity="epic", faction="桂系",
         role="commander",
         personality=dict(aggression=60, discipline=80, cunning=92),
         stats=dict(command=90, attack=75, defense=85, mobility=75, moraleAura=80),
         doctrine=dict(name="諸葛調度", trigger="AllyEngaged", threshold=0.0,
                       action="DefendNearestAlly",
                       description="友軍接戰即機動策應——小諸葛的全局調度藝術"),
         lore="桂系軍事靈魂,抗戰參謀副總長,善於運動戰與戰略調度,軍事才華冠絕一時。",
         battle="崑崙關戰役 1939"),
    dict(id="roc_feng_yuxiang", name="馮玉祥", courtesy="煥章",
         epithet="基督將軍", rarity="epic", faction="西北軍",
         role="commander",
         personality=dict(aggression=62, discipline=70, cunning=78),
         stats=dict(command=78, attack=74, defense=76, mobility=60, moraleAura=84),
         doctrine=dict(name="察哈爾抗敵", trigger="UnderAttack", threshold=0.0,
                       action="HoldPosition",
                       description="遭受攻擊時原地抗擊——察哈爾民眾抗日同盟軍的鐵血意志"),
         lore="西北軍統帥,一生倒戈八次卻始終抗日。布衣將軍,練兵嚴苛,愛兵如子。",
         battle="察哈爾抗戰 1933"),
    dict(id="roc_yan_xishan", name="閻錫山", courtesy="百川",
         epithet="山西王", rarity="epic", faction="晉綏軍",
         role="commander",
         personality=dict(aggression=40, discipline=85, cunning=90),
         stats=dict(command=76, attack=65, defense=92, mobility=50, moraleAura=75),
         doctrine=dict(name="模範省壁壘", trigger="Always", threshold=0.0,
                       action="HoldPosition",
                       description="永遠優先固守——三十八年的山西經營,把防守做成一門哲學"),
         lore="統治山西三十八年的不倒翁。太原會戰雖敗猶榮,保存實力的小大智慧。",
         battle="太原會戰 1937"),
]

RARITY_COLOR = {"legendary": (212, 175, 55), "epic": (148, 88, 214)}
RARITY_LABEL = {"legendary": "傳  奇", "epic": "史  詩"}
RARITY_GLOW = {"legendary": (255, 223, 120), "epic": (190, 140, 255)}

FONT_BOLD = "C:/Windows/Fonts/msjhbd.ttc"
FONT_REG = "C:/Windows/Fonts/msjh.ttc"

CARD_W, CARD_H = 512, 768


def font(size, bold=True):
    return ImageFont.truetype(FONT_BOLD if bold else FONT_REG, size)


def draw_text_center(d, cx, y, text, f, fill, spacing=0):
    w = d.textlength(text, font=f) + spacing * (len(text) - 1)
    x = cx - w / 2
    for ch in text:
        d.text((x, y), ch, font=f, fill=fill)
        x += d.textlength(ch, font=f) + spacing


def radial_bg(w, h, inner, outer):
    """簡易徑向漸層背景"""
    img = Image.new("RGB", (w, h))
    px = img.load()
    cx, cy = w / 2, h * 0.38
    maxd = math.hypot(max(cx, w - cx), max(cy, h - cy))
    for y in range(h):
        for x in range(w):
            t = min(1.0, math.hypot(x - cx, y - cy) / maxd)
            px[x, y] = tuple(int(inner[i] + (outer[i] - inner[i]) * t) for i in range(3))
    return img


def render_card(card):
    """程式化卡面:深色底 + 稀有度邊框 + 勳章圖騰 + 名號 + 數值條"""
    rarity = card["rarity"]
    frame = RARITY_COLOR[rarity]
    glow = RARITY_GLOW[rarity]

    img = radial_bg(CARD_W, CARD_H, (48, 44, 58), (14, 12, 18))
    d = ImageDraw.Draw(img)

    # 外框(三層:外亮線 / 主框 / 內細線)
    d.rectangle([6, 6, CARD_W - 7, CARD_H - 7], outline=glow, width=2)
    d.rectangle([12, 12, CARD_W - 13, CARD_H - 13], outline=frame, width=5)
    d.rectangle([22, 22, CARD_W - 23, CARD_H - 23], outline=frame, width=1)

    # 四角飾
    for cx, cy in [(12, 12), (CARD_W - 13, 12), (12, CARD_H - 13), (CARD_W - 13, CARD_H - 13)]:
        d.ellipse([cx - 7, cy - 7, cx + 7, cy + 7], fill=frame)

    # 稀有度標籤
    draw_text_center(d, CARD_W / 2, 34, RARITY_LABEL[rarity], font(22), frame, spacing=8)

    # 中央勳章:多角星 + 內圈 + 名號首字
    cx, cy, r_out = CARD_W / 2, 250, 120
    pts = []
    for i in range(16):
        ang = math.pi / 8 * i - math.pi / 2
        r = r_out if i % 2 == 0 else r_out * 0.82
        pts.append((cx + r * math.cos(ang), cy + r * math.sin(ang)))
    d.polygon(pts, outline=frame, fill=None)
    d.ellipse([cx - 96, cy - 96, cx + 96, cy + 96], outline=glow, width=3)
    d.ellipse([cx - 88, cy - 88, cx + 88, cy + 88], outline=frame, width=2)
    draw_text_center(d, cx, cy - 52, card["name"][0], font(96), glow)

    # 姓名與稱號
    draw_text_center(d, CARD_W / 2, 400, card["name"], font(64), (240, 236, 228), spacing=6)
    draw_text_center(d, CARD_W / 2, 482,
                     f"字 {card['courtesy']} · {card['epithet']}", font(24, False), frame)
    draw_text_center(d, CARD_W / 2, 516, card["faction"], font(20, False), (150, 145, 140))

    # 數值條
    stats = card["stats"]
    labels = [("指揮", "command"), ("攻擊", "attack"), ("防禦", "defense"),
              ("機動", "mobility"), ("威儀", "moraleAura")]
    y = 566
    for label, key in labels:
        v = stats[key]
        d.text((48, y), label, font=font(20, False), fill=(200, 196, 190))
        d.rectangle([116, y + 4, 116 + 340, y + 22], outline=(90, 86, 96), width=1)
        fill_w = int(340 * v / 100)
        d.rectangle([116, y + 4, 116 + fill_w, y + 22], fill=frame)
        d.text((466, y), str(v), font=font(18), fill=(230, 226, 220))
        y += 30

    # 代表戰役
    draw_text_center(d, CARD_W / 2, CARD_H - 44, card["battle"], font(18, False), (170, 165, 158))
    return img


def card_json(card):
    return {
        "schema": "potato.character_card/1",
        "id": card["id"],
        "name": card["name"],
        "courtesyName": card["courtesy"],
        "epithet": card["epithet"],
        "rarity": card["rarity"],
        "faction": card["faction"],
        "role": card["role"],
        "personality": card["personality"],
        "stats": card["stats"],
        "signatureDoctrine": {
            "name": card["doctrine"]["name"],
            "trigger": card["doctrine"]["trigger"],
            "threshold": card["doctrine"]["threshold"],
            "action": card["doctrine"]["action"],
            "description": card["doctrine"]["description"],
        },
        "lore": card["lore"],
        "historicalBattle": card["battle"],
        "art": f"cards/art/{card['id']}.png",
    }


def main():
    os.makedirs(CARDS_DIR, exist_ok=True)
    os.makedirs(ART_DIR, exist_ok=True)

    manifest_entries = []
    for card in CARDS:
        data = card_json(card)
        json_path = os.path.join(CARDS_DIR, card["id"] + ".json")
        with open(json_path, "w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=2)

        img = render_card(card)
        img.save(os.path.join(ART_DIR, card["id"] + ".png"))

        manifest_entries.append({
            "id": card["id"], "name": card["name"], "rarity": card["rarity"],
            "faction": card["faction"],
            "data": f"cards/roc/{card['id']}.json",
            "art": f"cards/art/{card['id']}.png",
        })
        print(f"  [OK] {card['name']} ({card['rarity']})")

    manifest = {
        "schema": "potato.asset_manifest/1",
        "library": "roc-character-cards",
        "version": "1.0.0",
        "description": "民國名將角色卡 — 史詩/傳奇級素材庫(doctrine 對齊 Gameplay/Doctrine.h)",
        "cards": manifest_entries,
    }
    with open(os.path.join(ROOT, "assets", "manifest.json"), "w", encoding="utf-8") as f:
        json.dump(manifest, f, ensure_ascii=False, indent=2)

    print(f"\n完成: {len(manifest_entries)} 張卡 → assets/cards/")


if __name__ == "__main__":
    main()
