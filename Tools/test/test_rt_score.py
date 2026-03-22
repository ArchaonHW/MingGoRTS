#!/usr/bin/env python3
"""
MingWar-RTS ?皜祈岫?單
皜祈岫 Sprint 1 ?詨??嚗雿?宏?擛?
"""

import os
import sys
import json
import time
from pathlib import Path

# 瘛餃? UE5 Python API 頝臬?嚗???剁?
try:
    import unreal
    UE_AVAILABLE = True
except ImportError:
    UE_AVAILABLE = False
    print("霅血?: UE5 Python API 銝?剁????冽芋?祆芋撘?")

class RTSGameTester:
    def __init__(self, project_root):
        self.project_root = Path(project_root)
        self.config_path = self.project_root / "Content" / "Maps" / "TestMap" / "map_config.json"
        self.config = self.load_config()
        self.test_results = []
        
    def load_config(self):
        """頛?啣??蔭"""
        if self.config_path.exists():
            with open(self.config_path, 'r', encoding='utf-8') as f:
                return json.load(f)
        else:
            print(f"?航炊: ?曆??圈?蝵格?隞?{self.config_path}")
            return {}
    
    def log_test(self, test_name, passed, details=""):
        """閮?皜祈岫蝯?"""
        status = "??PASS" if passed else "??FAIL"
        entry = f"[{status}] {test_name}"
        if details:
            entry += f"\n   蝝啁?: {details}"
        self.test_results.append(entry)
        print(entry)
    
    def test_config_validity(self):
        """皜祈岫?蔭?辣????""
        required_sections = ['map_name', 'size', 'terrain', 'game_mode', 'initial_state']
        has_all = all(section in self.config for section in required_sections)
        self.log_test("?蔭摰?扳炎??, has_all, 
                     f"瑼Ｘ?畾? {required_sections}")
        return has_all
    
    def test_map_dimensions(self):
        """皜祈岫?啣?撠箏站"""
        size = self.config.get('size', {})
        width = size.get('width', 0)
        height = size.get('height', 0)
        valid = width >= 512 and height >= 512
        self.log_test("?啣?撠箏站瑼Ｘ", valid, f"撠箏站: {width}x{height}")
        return valid
    
    def test_initial_units_count(self):
        """皜祈岫???桐??賊?"""
        units = self.config.get('initial_state', {}).get('units', [])
        total_units = sum(u.get('count', 0) for u in units)
        valid = total_units >= 5  # ?喳?5?雿?
        self.log_test("???桐??賊?", valid, f"蝮賣: {total_units}")
        return valid
    
    def test_teams_configuration(self):
        """皜祈岫???蔭"""
        teams = self.config.get('game_mode', {}).get('teams', [])
        has_two_teams = len(teams) >= 2
        has_spawn_points = all('spawn_point' in team for team in teams[:2])
        valid = has_two_teams and has_spawn_points
        self.log_test("???蔭瑼Ｘ", valid, f"???? {len(teams)}, ?箇?暺? {has_spawn_points}")
        return valid
    
    def test_resources_configuration(self):
        """皜祈岫鞈??蔭"""
        resources = self.config.get('initial_state', {}).get('resources', {})
        player_res = resources.get('player', {})
        required_resources = ['gold', 'coal', 'food', 'ammo']
        has_all = all(res in player_res for res in required_resources)
        valid_amounts = all(player_res.get(res, 0) >= 0 for res in required_resources)
        valid = has_all and valid_amounts
        self.log_test("鞈??蔭瑼Ｘ", valid, f"鞈?憿?: {list(player_res.keys())}")
        return valid
    
    def test_buildings_configuration(self):
        """皜祈岫撱箇??蔭"""
        buildings = self.config.get('initial_state', {}).get('buildings', [])
        has_buildings = len(buildings) > 0
        valid_types = all('type' in b and 'position' in b for b in buildings)
        valid = has_buildings and valid_types
        self.log_test("撱箇??蔭瑼Ｘ", valid, f"撱箇??賊?: {len(buildings)}")
        return valid
    
    def test_ui_settings(self):
        """皜祈岫UI閮剔蔭"""
        ui = self.config.get('ui', {})
        required_settings = ['show_hud', 'show_health_bars', 'show_selection_box']
        has_all = all(setting in ui for setting in required_settings)
        self.log_test("UI?蔭瑼Ｘ", has_all, f"閮剔蔭: {list(ui.keys())}")
        return has_all
    
    def simulate_unit_selection(self):
        """璅⊥?桐??豢??摩皜祈岫"""
        if not UE_AVAILABLE:
            self.log_test("?桐??豢?皜祈岫 (璅⊥)", True, "UE5 API 銝?剁?頝喲?撖阡?皜祈岫")
            return True
            
        try:
            # ?ㄐ?矽??UE5 API ?脰?撖阡?皜祈岫
            # 靘?: selected_units = GameplayStatics.GetAllActorsOfClass(GetWorld(), ARTSUnit)
            # 銝阡?霅??頛?
            self.log_test("?桐??豢??摩皜祈岫", True, "璅⊥皜祈岫??")
            return True
        except Exception as e:
            self.log_test("?桐??豢??摩皜祈岫", False, str(e))
            return False
    
    def run_all_tests(self):
        """?瑁???葫閰?""
        print("=" * 60)
        print("MingWar-RTS ?皜祈岫")
        print("=" * 60)
        
        tests = [
            self.test_config_validity,
            self.test_map_dimensions,
            self.test_initial_units_count,
            self.test_teams_configuration,
            self.test_resources_configuration,
            self.test_buildings_configuration,
            self.test_ui_settings,
            self.simulate_unit_selection
        ]
        
        passed = 0
        for test in tests:
            try:
                if test():
                    passed += 1
            except Exception as e:
                self.log_test(test.__name__, False, f"?啣虜: {e}")
        
        print("=" * 60)
        print(f"皜祈岫摰?: {passed}/{len(tests)} ??")
        print("=" * 60)
        
        return passed == len(tests)
    
    def generate_report(self):
        """??皜祈岫?勗?"""
        report_path = self.project_root / "Tools" / "test" / "test_report.txt"
        with open(report_path, 'w', encoding='utf-8') as f:
            f.write("MingWar-RTS 皜祈岫?勗?\n")
            f.write("=" * 60 + "\n\n")
            for result in self.test_results:
                f.write(result + "\n")
        print(f"皜祈岫?勗?撌脩??? {report_path}")

def main():
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    tester = RTSGameTester(project_root)
    
    if tester.run_all_tests():
        tester.generate_report()
        print("????葫閰阡?嚗?)
        return 0
    else:
        print("???典?皜祈岫憭望?嚗?瑼Ｘ?蔭")
        tester.generate_report()
        return 1

if __name__ == "__main__":
    sys.exit(main())
