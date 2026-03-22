#!/usr/bin/env python3
"""
MingGoRTS Epic Asset Generation System
?? Epic 隞餃????蔣???璅????
Features:
- Epic ??蝝???
- ?寞活???脣漲餈質馱
- 憭撘撓?箸??- ?釭?批??霅?"""

import os
import sys
import json
import argparse
import subprocess
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional
import shutil

# 撠?頝臬?閮剖?
PROJECT_ROOT = Path("C:/HW/MingGoRTS")
OUTPUT_BASE = PROJECT_ROOT / "Content" / "EpicAssets"
CONFIG_PATH = PROJECT_ROOT / "Tools" / "ai" / "epic_assets_config.json"
PROGRESS_FILE = PROJECT_ROOT / "Tools" / "ai" / "epic_generation_progress.json"

# Epic 蝝??蔭摰儔
EPIC_ASSETS_CONFIG = {
    "epic1": {
        "name": "甇瑕瘙箇?擃??箇?",
        "description": "甇瑕瘙箇?蝟餌絞????隞嗉孛?潦???蝞?,
        "assets": {
            "images": [
                {"name": "decision_bg", "prompt": "Chinese Republican era 1920s, war room interior, wooden desk with military maps, vintage oil lamp, dramatic lighting, cinematic atmosphere, historical documentary style, 4K", "category": "Background", "resolution": "1920x1080"},
                {"name": "historical_characters", "prompt": "Portrait of Chinese Republican era military officers, formal uniforms, serious expressions, vintage photography style, sepia tone, historical authenticity", "category": "Characters", "resolution": "512x512", "count": 5},
                {"name": "event_triggers", "prompt": "Historical event scenes from Republican China, crowd gatherings, revolutionary moments, vintage documentary style, dramatic composition", "category": "Events", "resolution": "1024x576", "count": 3},
            ],
            "music": [
                {"name": "decision_theme", "prompt": "Tense decision-making music, Chinese traditional instruments mixed with Western orchestra, dramatic and contemplative, suitable for historical strategy decisions, 100 BPM", "duration": 45, "category": "Decision"},
                {"name": "historical_event", "prompt": "Epic historical event music, Chinese Republican era documentary style, brass fanfare with traditional percussion, inspiring and solemn", "duration": 60, "category": "Events"},
            ],
            "sfx": [
                {"name": "decision_confirm", "description": "Decision confirmation sound, historical military stamp/seal effect", "type": "UI"},
                {"name": "event_trigger", "description": "Historical event notification, dramatic fanfare", "type": "Notification"},
                {"name": "paper_rustle", "description": "Document/paper handling sound", "type": "Ambient"},
            ],
            "video": [
                {"name": "intro_cinematic", "description": "Epic 1 intro cinematic, 1920s China historical context", "duration": 30, "style": "Documentary"},
            ]
        }
    },
    "epic2": {
        "name": "憭扯?璅⊥銵擛亦頂蝯?,
        "description": "1000+?桐??圈洛?TS?詨??擛丕I",
        "assets": {
            "images": [
                {"name": "battlefield_tactical", "prompt": "Epic battlefield scene, 1000+ soldiers, Chinese Republican era warfare, strategic overview, smoke and explosions, cinematic aerial view, dramatic lighting", "category": "Battlefield", "resolution": "1920x1080"},
                {"name": "unit_sprites", "prompt": "RTS unit icons, Chinese Republican era soldiers, infantry and cavalry, transparent background, game asset style, isometric view", "category": "Units", "resolution": "256x256", "count": 8},
                {"name": "battle_ui_elements", "prompt": "Military strategy game UI elements, minimap frames, command buttons, resource counters, Chinese Republican era aesthetic", "category": "UI", "resolution": "512x512", "count": 6},
            ],
            "music": [
                {"name": "battle_epic", "prompt": "Intense large-scale battle music, full orchestra with Chinese war drums and percussion, epic and driving, 140 BPM, suitable for 1000+ unit battles", "duration": 120, "category": "Battle"},
                {"name": "tactical_planning", "prompt": "Strategic planning music, military command atmosphere, tense but methodical, brass and strings, suitable for pre-battle preparation", "duration": 90, "category": "Strategy"},
                {"name": "victory_fanfare", "prompt": "Triumphant victory music, Chinese military celebration, brass fanfare with traditional instruments, short and glorious", "duration": 20, "category": "Victory"},
            ],
            "sfx": [
                {"name": "gunfire_rifle", "description": "Rifle gunfire, Republican era weapons", "type": "Combat", "variations": 3},
                {"name": "cannon_fire", "description": "Artillery cannon fire", "type": "Combat"},
                {"name": "cavalry_charge", "description": "Horse galloping and charging", "type": "Combat"},
                {"name": "battle_ambience", "description": "Large battle ambient sounds", "type": "Ambient", "loop": True},
                {"name": "unit_select", "description": "Unit selection confirmation", "type": "UI", "variations": 3},
                {"name": "command_issue", "description": "Command given sound", "type": "UI"},
            ],
            "video": [
                {"name": "battle_demo", "description": "Large scale battle demonstration, 1000+ units, tactical movements", "duration": 60, "style": "Gameplay"},
            ]
        }
    },
    "epic3": {
        "name": "閫???鈭頂蝯?,
        "description": "閫?萄遣???賜撅?鈭?閰晞風?脰??脫瞍?,
        "assets": {
            "images": [
                {"name": "character_creation_bg", "prompt": "Character creation screen background, Chinese Republican era study room, books and scrolls, warm lighting, elegant and scholarly atmosphere", "category": "Background", "resolution": "1920x1080"},
                {"name": "character_portraits", "prompt": "Character portrait frames, Chinese Republican era style, military and civilian attire, different ages and genders, painterly style", "category": "Characters", "resolution": "512x512", "count": 10},
                {"name": "skill_icons", "prompt": "Skill and ability icons, Chinese martial arts and military strategy, calligraphy brush style, game UI icons", "category": "Skills", "resolution": "128x128", "count": 15},
                {"name": "dialogue_bg", "prompt": "Dialogue scene backgrounds, Chinese Republican era interiors and exteriors, various locations, atmospheric lighting", "category": "Dialogue", "resolution": "1920x1080", "count": 5},
            ],
            "music": [
                {"name": "character_theme", "prompt": "Character development music, personal growth and introspection, Chinese flute and strings, emotional and inspiring", "duration": 60, "category": "Character"},
                {"name": "dialogue_ambient", "prompt": "Dialogue background music, subtle and atmospheric, Chinese traditional instruments, non-intrusive", "duration": 45, "category": "Dialogue"},
                {"name": "narrative_moment", "prompt": "Narrative storytelling music, historical drama style, emotional peaks and valleys, orchestral with Chinese elements", "duration": 90, "category": "Narrative"},
            ],
            "sfx": [
                {"name": "level_up", "description": "Character level up sound, progression and achievement", "type": "UI"},
                {"name": "skill_unlock", "description": "New skill unlocked sound", "type": "UI"},
                {"name": "dialogue_advance", "description": "Dialogue text advance sound", "type": "UI"},
                {"name": "page_turn", "description": "Book/page turning sound", "type": "Ambient"},
            ],
            "video": [
                {"name": "character_showcase", "description": "Character creation and skill system showcase", "duration": 45, "style": "UI_Demo"},
            ]
        }
    },
    "epic4": {
        "name": "?箏撱箄身??皞恣??,
        "description": "?箏撱粹?皞?遣蝭?蝝???",
        "assets": {
            "images": [
                {"name": "base_construction", "prompt": "Base building construction site, Chinese Republican era military base, barracks and factories, industrial atmosphere, strategic game style", "category": "Buildings", "resolution": "1920x1080"},
                {"name": "building_sprites", "prompt": "Isometric building sprites, Chinese Republican era military structures, barracks, armories, factories, transparent background, game asset style", "category": "Buildings", "resolution": "512x512", "count": 8},
                {"name": "resource_icons", "prompt": "Resource icons, food, money, materials, ammunition, Chinese Republican era style, game UI icons, clear and readable", "category": "Resources", "resolution": "128x128", "count": 6},
                {"name": "tech_tree_bg", "prompt": "Technology research background, Chinese study with scientific equipment, maps and blueprints, warm lighting", "category": "Background", "resolution": "1920x1080"},
            ],
            "music": [
                {"name": "construction_theme", "prompt": "Base building and construction music, productive and industrious, Chinese folk elements mixed with modern, upbeat rhythm 110 BPM", "duration": 60, "category": "Construction"},
                {"name": "resource_management", "prompt": "Resource management background music, calm and methodical, suitable for economic planning, ambient and peaceful", "duration": 75, "category": "Economy"},
                {"name": "research_complete", "prompt": "Technology research completion, achievement music, discovery and progress theme", "duration": 15, "category": "Achievement"},
            ],
            "sfx": [
                {"name": "building_place", "description": "Building placement sound, construction begin", "type": "Construction"},
                {"name": "building_complete", "description": "Building construction complete", "type": "Construction"},
                {"name": "resource_collect", "description": "Resource collection sound", "type": "Economy", "variations": 3},
                {"name": "upgrade_apply", "description": "Building/technology upgrade sound", "type": "UI"},
                {"name": "hammering", "description": "Construction hammering sounds", "type": "Ambient", "loop": True},
            ],
            "video": [
                {"name": "building_timelapse", "description": "Base building timelapse demonstration", "duration": 30, "style": "Timelapse"},
            ]
        }
    },
    "epic5": {
        "name": "?惜蝑?游?蝟餌絞",
        "description": "?啁撅扎銵惜?犖撅扎遣?惜?游?",
        "assets": {
            "images": [
                {"name": "strategic_map", "prompt": "Strategic map overview, China Republican era, territories and factions, beautiful and detailed, game map style, vintage cartography", "category": "Maps", "resolution": "1920x1080"},
                {"name": "layer_transitions", "prompt": "Layer transition effects, strategic to tactical zoom, blur and focus effects, cinematic transitions", "category": "Effects", "resolution": "1920x1080", "count": 4},
                {"name": "faction_symbols", "prompt": "Faction and alliance symbols, Chinese Republican era warlord factions, distinctive emblems, military heraldry style", "category": "UI", "resolution": "256x256", "count": 8},
                {"name": "diplomacy_bg", "prompt": "Diplomacy meeting background, Chinese Republican era government building, formal and official atmosphere", "category": "Background", "resolution": "1920x1080"},
            ],
            "music": [
                {"name": "strategic_theme", "prompt": "Grand strategic overview music, vast and contemplative, Chinese imperial court influence mixed with modern orchestra", "duration": 90, "category": "Strategy"},
                {"name": "layer_transition", "prompt": "Layer transition music, seamless movement between strategic and tactical, dynamic and flowing", "duration": 10, "category": "Transition"},
                {"name": "diplomacy_theme", "prompt": "Diplomacy and alliance music, sophisticated and political, strings and subtle percussion, negotiation atmosphere", "duration": 60, "category": "Diplomacy"},
            ],
            "sfx": [
                {"name": "map_zoom", "description": "Strategic map zoom sound", "type": "UI"},
                {"name": "layer_switch", "description": "Layer transition sound effect", "type": "UI"},
                {"name": "diplomacy_success", "description": "Successful negotiation/diplomacy", "type": "UI"},
                {"name": "territory_claim", "description": "Territory capture/claim sound", "type": "Strategy"},
            ],
            "video": [
                {"name": "layer_switching", "description": "Four-layer system demonstration video", "duration": 90, "style": "System_Demo"},
            ]
        }
    },
    "epic6": {
        "name": "頝典像?啁蝮恍?撽?,
        "description": "憭像?啣董?蝡臬?甇乓像?圈???賢??,
        "assets": {
            "images": [
                {"name": "cross_platform_ui", "prompt": "Cross-platform UI mockups, PC and mobile interfaces, responsive design, Chinese Republican era game theme, side-by-side comparison", "category": "UI", "resolution": "1920x1080"},
                {"name": "device_showcase", "prompt": "Game running on multiple devices, PC monitor, tablet, smartphone, seamless experience showcase", "category": "Marketing", "resolution": "1920x1080"},
                {"name": "sync_visualization", "prompt": "Cloud sync visualization, data flowing between devices, abstract and modern, secure connection", "category": "Effects", "resolution": "1024x1024"},
            ],
            "music": [
                {"name": "menu_theme", "prompt": "Main menu music, grand and inviting, suitable for all platforms, Chinese and Western fusion, memorable theme", "duration": 120, "category": "Main"},
                {"name": "loading_music", "prompt": "Loading screen music, optimistic and progressive, seamless loop", "duration": 30, "category": "Loading", "loop": True},
            ],
            "sfx": [
                {"name": "sync_complete", "description": "Cloud sync complete notification", "type": "UI"},
                {"name": "login_success", "description": "Successful login sound", "type": "UI"},
                {"name": "platform_switch", "description": "Platform switch notification", "type": "UI"},
            ],
            "video": [
                {"name": "cross_platform_demo", "description": "Cross-platform seamless experience demonstration", "duration": 60, "style": "Marketing"},
            ]
        }
    }
}

def save_config():
    """靽??蔭??JSON ?辣"""
    with open(CONFIG_PATH, 'w', encoding='utf-8') as f:
        json.dump(EPIC_ASSETS_CONFIG, f, ensure_ascii=False, indent=2)
    print(f"???蔭撌脖?摮? {CONFIG_PATH}")

def load_config() -> Dict:
    """頛?蔭"""
    if CONFIG_PATH.exists():
        with open(CONFIG_PATH, 'r', encoding='utf-8') as f:
            return json.load(f)
    return EPIC_ASSETS_CONFIG

def load_progress() -> Dict:
    """頛???脣漲"""
    if PROGRESS_FILE.exists():
        with open(PROGRESS_FILE, 'r', encoding='utf-8') as f:
            return json.load(f)
    return {}

def save_progress(progress: Dict):
    """靽????脣漲"""
    with open(PROGRESS_FILE, 'w', encoding='utf-8') as f:
        json.dump(progress, f, ensure_ascii=False, indent=2)

def create_output_structure():
    """?萄遣頛詨?桅?蝯?"""
    for epic_id in EPIC_ASSETS_CONFIG.keys():
        epic_path = OUTPUT_BASE / epic_id
        for asset_type in ["images", "music", "sfx", "video"]:
            (epic_path / asset_type).mkdir(parents=True, exist_ok=True)
    print(f"??頛詨?桅?蝯?撌脣撱? {OUTPUT_BASE}")

def generate_placeholder_image(name: str, category: str, resolution: str, output_dir: Path):
    """??雿???嚗蝙??PIL ?萄遣撣嗆?摮???嚗?""
    try:
        from PIL import Image, ImageDraw, ImageFont
        
        width, height = map(int, resolution.split('x'))
        img = Image.new('RGB', (width, height), color=(40, 44, 52))
        draw = ImageDraw.Draw(img)
        
        # 瘛餃???
        text = f"{name}\n{category}\n{resolution}"
        try:
            font = ImageFont.truetype("arial.ttf", 40)
        except:
            font = ImageFont.load_default()
        
        # ?脣?????獢蒂撅葉
        bbox = draw.textbbox((0, 0), text, font=font)
        text_width = bbox[2] - bbox[0]
        text_height = bbox[3] - bbox[1]
        x = (width - text_width) // 2
        y = (height - text_height) // 2
        
        draw.text((x, y), text, fill=(200, 200, 200), font=font, align='center')
        
        output_path = output_dir / f"{name}.png"
        img.save(output_path)
        return str(output_path)
    except ImportError:
        # 憒? PIL 銝?剁??萄遣銝?征?辣
        output_path = output_dir / f"{name}.png"
        output_path.touch()
        return str(output_path)

def generate_placeholder_audio(name: str, duration: int, category: str, output_dir: Path):
    """??雿??喲?辣"""
    output_path = output_dir / f"{name}.wav"
    # ?萄遣蝛箇? WAV ?辣??    with open(output_path, 'wb') as f:
        # ?撠???WAV ?辣??        import struct
        f.write(b'RIFF')
        f.write(struct.pack('<I', 36))  # ?辣憭批?
        f.write(b'WAVE')
        f.write(b'fmt ')
        f.write(struct.pack('<I', 16))  # fmt chunk 憭批?
        f.write(struct.pack('<H', 1))   # PCM ?澆?
        f.write(struct.pack('<H', 1))   # ?株??        f.write(struct.pack('<I', 44100))  # ?⊥見??        f.write(struct.pack('<I', 88200))  # 摮???        f.write(struct.pack('<H', 2))   # 憛?朣?        f.write(struct.pack('<H', 16))  # 雿楛摨?        f.write(b'data')
        f.write(struct.pack('<I', 0))   # ?豢?憭批?
    return str(output_path)

def generate_assets_for_epic(epic_id: str, asset_types: List[str] = None, force: bool = False):
    """?箇摰?Epic ??蝝?"""
    config = load_config()
    
    if epic_id not in config:
        print(f"???芰??Epic ID: {epic_id}")
        return False
    
    epic_config = config[epic_id]
    epic_path = OUTPUT_BASE / epic_id
    progress = load_progress()
    
    if epic_id not in progress:
        progress[epic_id] = {}
    
    print(f"\n{'='*60}")
    print(f"? ?? Epic: {epic_config['name']}")
    print(f"   {epic_config['description']}")
    print(f"{'='*60}")
    
    results = {"success": [], "failed": [], "skipped": []}
    
    types_to_generate = asset_types or ["images", "music", "sfx", "video"]
    
    for asset_type in types_to_generate:
        if asset_type not in epic_config["assets"]:
            continue
            
        print(f"\n?? 鞈憿?: {asset_type.upper()}")
        type_path = epic_path / asset_type
        type_path.mkdir(parents=True, exist_ok=True)
        
        assets = epic_config["assets"][asset_type]
        
        for asset in assets:
            if isinstance(asset, dict):
                name = asset.get("name", "unnamed")
                count = asset.get("count", 1)
            else:
                name = str(asset)
                count = 1
            
            # 瑼Ｘ?臬撌脩???            asset_key = f"{asset_type}_{name}"
            if not force and asset_key in progress[epic_id]:
                print(f"   ??頝喲? {name} (撌脣???")
                results["skipped"].append(name)
                continue
            
            try:
                if asset_type == "images":
                    resolution = asset.get("resolution", "512x512")
                    category = asset.get("category", "General")
                    
                    for i in range(count):
                        suffix = f"_{i+1}" if count > 1 else ""
                        output_path = generate_placeholder_image(
                            f"{name}{suffix}", category, resolution, type_path
                        )
                        print(f"   ????: {name}{suffix} ({resolution})")
                    
                elif asset_type == "music":
                    duration = asset.get("duration", 30)
                    category = asset.get("category", "General")
                    output_path = generate_placeholder_audio(name, duration, category, type_path)
                    print(f"   ???單?: {name} ({duration}s)")
                    
                elif asset_type == "sfx":
                    variations = asset.get("variations", 1)
                    for i in range(variations):
                        suffix = f"_{i+1}" if variations > 1 else ""
                        output_path = type_path / f"{name}{suffix}.wav"
                        output_path.touch()
                        print(f"   ???單?: {name}{suffix}")
                        
                elif asset_type == "video":
                    duration = asset.get("duration", 30)
                    output_path = type_path / f"{name}.mp4"
                    output_path.touch()
                    print(f"   ??敶梁?: {name} ({duration}s)")
                
                progress[epic_id][asset_key] = {
                    "status": "generated",
                    "timestamp": datetime.now().isoformat()
                }
                results["success"].append(name)
                
            except Exception as e:
                print(f"   ??憭望? {name}: {e}")
                progress[epic_id][asset_key] = {
                    "status": "failed",
                    "error": str(e),
                    "timestamp": datetime.now().isoformat()
                }
                results["failed"].append(name)
    
    save_progress(progress)
    
    # 頛詨??
    print(f"\n?? Epic {epic_id} ????:")
    print(f"   ????: {len(results['success'])}")
    print(f"   ??頝喲?: {len(results['skipped'])}")
    print(f"   ??憭望?: {len(results['failed'])}")
    
    return len(results['failed']) == 0

def generate_all_epics(asset_types: List[str] = None, force: bool = False):
    """?????Epic ????""
    config = load_config()
    
    print("\n" + "="*60)
    print("?? MingGoRTS Epic Asset Generation System")
    print("="*60)
    
    total_epics = len(config)
    success_count = 0
    
    for i, epic_id in enumerate(config.keys(), 1):
        print(f"\n[{i}/{total_epics}] ?? Epic: {epic_id}")
        if generate_assets_for_epic(epic_id, asset_types, force):
            success_count += 1
    
    print("\n" + "="*60)
    print("?? 蝮賡?????")
    print("="*60)
    print(f"   摰?: {success_count}/{total_epics} Epics")
    print(f"   頛詨?桅?: {OUTPUT_BASE}")
    
    return success_count == total_epics

def list_epics():
    """????Epic ?蝝??瘙?""
    config = load_config()
    progress = load_progress()
    
    print("\n?? Epic 蝝??瘙???)
    print("="*60)
    
    for epic_id, epic_data in config.items():
        print(f"\n? {epic_id}: {epic_data['name']}")
        print(f"   {epic_data['description']}")
        
        epic_progress = progress.get(epic_id, {})
        
        for asset_type, assets in epic_data["assets"].items():
            total = len(assets)
            generated = sum(1 for k in epic_progress.keys() if k.startswith(f"{asset_type}_"))
            
            # 閮?撖阡??賊?嚗 count ??variations嚗?            actual_total = 0
            for asset in assets:
                if isinstance(asset, dict):
                    count = asset.get("count", 1)
                    variations = asset.get("variations", 1)
                    actual_total += count * variations
                else:
                    actual_total += 1
            
            status_icon = "?? if generated >= actual_total else "??
            print(f"   {status_icon} {asset_type}: {generated}/{actual_total}")

def clean_generated():
    """皜??????蝝?"""
    if OUTPUT_BASE.exists():
        shutil.rmtree(OUTPUT_BASE)
        print(f"??撌脫??? {OUTPUT_BASE}")
    
    if PROGRESS_FILE.exists():
        PROGRESS_FILE.unlink()
        print(f"??撌脫??脣漲?辣")
    
    create_output_structure()

def main():
    parser = argparse.ArgumentParser(
        description="MingGoRTS Epic Asset Generation System",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
雿輻蝭?:
  python epic_asset_generator.py --list                    # ????Epic
  python epic_asset_generator.py --epic epic1              # ?? Epic 1 ??????  python epic_asset_generator.py --epic epic2 --type music # ?芰??璅?  python epic_asset_generator.py --all                     # ???????  python epic_asset_generator.py --clean                   # 皜????????        """
    )
    
    parser.add_argument("--list", action="store_true", help="????Epic 蝝??瘙?)
    parser.add_argument("--epic", type=str, help="?? Epic ID (e.g., epic1, epic2)")
    parser.add_argument("--type", type=str, choices=["images", "music", "sfx", "video"],
                       help="?芰??摰?????)
    parser.add_argument("--all", action="store_true", help="?????Epic ??????)
    parser.add_argument("--force", action="store_true", help="撘瑕???嚗????")
    parser.add_argument("--clean", action="store_true", help="皜??????蝝?")
    parser.add_argument("--init", action="store_true", help="????蝵桀??桅?蝯?")
    
    args = parser.parse_args()
    
    if args.init:
        save_config()
        create_output_structure()
        print("\n????????")
        return
    
    if args.clean:
        clean_generated()
        return
    
    if args.list:
        list_epics()
        return
    
    if args.all:
        asset_types = [args.type] if args.type else None
        success = generate_all_epics(asset_types, args.force)
        sys.exit(0 if success else 1)
    
    if args.epic:
        asset_types = [args.type] if args.type else None
        success = generate_assets_for_epic(args.epic, asset_types, args.force)
        sys.exit(0 if success else 1)
    
    # ?身銵嚗＊蝷箏鼠?拙?皜
    parser.print_help()
    print("\n")
    list_epics()

if __name__ == "__main__":
    main()

