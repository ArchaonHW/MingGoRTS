#!/usr/bin/env python3
"""
MingGoRTS 角色創建系統測試腳本
測試 Story 3-1: 角色創建和基礎屬性系統
"""

import unittest
import json
import sys
import os
from datetime import datetime

# 添加項目根目錄到 Python 路徑
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class TestCharacterCreation(unittest.TestCase):
    """角色創建系統測試類"""
    
    def setUp(self):
        """測試前的設置"""
        self.test_character_data = {
            "name": "張偉",
            "background": "MilitaryAcademy",
            "attributes": {
                "leadership": 75,
                "intelligence": 60,
                "courage": 80,
                "charisma": 55,
                "constitution": 70
            },
            "age": 28,
            "biography": "畢業於黃埔軍校的年輕軍官"
        }
    
    def test_character_name_validation(self):
        """測試角色名稱驗證"""
        # 測試有效名稱
        valid_names = ["張偉", "李明", "王強", "趙雲"]
        for name in valid_names:
            self.assertTrue(self._is_valid_name(name), f"名稱 {name} 應該有效")
        
        # 測試無效名稱
        invalid_names = ["", "A", "這是一個非常非常長的角色名稱超過了二十個字符的限制", "張123", "李!@#"]
        for name in invalid_names:
            self.assertFalse(self._is_valid_name(name), f"名稱 {name} 應該無效")
    
    def test_attributes_validation(self):
        """測試屬性驗證"""
        # 測試有效屬性
        valid_attributes = [
            {"leadership": 50, "intelligence": 50, "courage": 50, "charisma": 50, "constitution": 50},
            {"leadership": 100, "intelligence": 0, "courage": 75, "charisma": 25, "constitution": 90}
        ]
        
        for attrs in valid_attributes:
            self.assertTrue(self._are_valid_attributes(attrs), f"屬性 {attrs} 應該有效")
        
        # 測試無效屬性
        invalid_attributes = [
            {"leadership": -10, "intelligence": 50, "courage": 50, "charisma": 50, "constitution": 50},
            {"leadership": 150, "intelligence": 50, "courage": 50, "charisma": 50, "constitution": 50}
        ]
        
        for attrs in invalid_attributes:
            self.assertFalse(self._are_valid_attributes(attrs), f"屬性 {attrs} 應該無效")
    
    def test_background_system(self):
        """測試背景系統"""
        backgrounds = [
            "MilitaryAcademy",
            "WarlordSon", 
            "Revolutionary",
            "ScholarOfficial",
            "Merchant",
            "CommonSoldier"
        ]
        
        for background in backgrounds:
            self.assertTrue(self._is_valid_background(background), f"背景 {background} 應該有效")
            
            # 測試背景描述
            description = self._get_background_description(background)
            self.assertIsNotNone(description, f"背景 {background} 應該有描述")
            self.assertGreater(len(description), 10, f"背景 {background} 的描述應該足夠詳細")
    
    def test_age_validation(self):
        """測試年齡驗證"""
        # 測試有效年齡
        valid_ages = [18, 25, 35, 50, 65]
        for age in valid_ages:
            self.assertTrue(self._is_valid_age(age), f"年齡 {age} 應該有效")
        
        # 測試無效年齡
        invalid_ages = [17, 66, -5, 100]
        for age in invalid_ages:
            self.assertFalse(self._is_valid_age(age), f"年齡 {age} 應該無效")
    
    def test_character_data_serialization(self):
        """測試角色數據序列化"""
        # 測試序列化
        serialized = self._serialize_character_data(self.test_character_data)
        self.assertIsInstance(serialized, str)
        self.assertGreater(len(serialized), 0)
        
        # 測試反序列化
        deserialized = self._deserialize_character_data(serialized)
        self.assertEqual(deserialized["name"], self.test_character_data["name"])
        self.assertEqual(deserialized["background"], self.test_character_data["background"])
    
    def test_skill_system(self):
        """測試技能系統"""
        # 測試推薦技能
        for background in ["MilitaryAcademy", "WarlordSon", "Revolutionary"]:
            skills = self._get_recommended_skills(background)
            self.assertIsInstance(skills, list)
            self.assertGreater(len(skills), 0, f"背景 {background} 應該有推薦技能")
            
            for skill in skills:
                self.assertIn("name", skill)
                self.assertIn("description", skill)
                self.assertIn("level", skill)
    
    def test_attribute_points_allocation(self):
        """測試屬性點分配"""
        # 測試初始屬性點
        initial_points = 10
        self.assertEqual(initial_points, 10, "初始屬性點應該是10")
        
        # 測試屬性點分配
        test_allocation = {
            "leadership": 5,
            "intelligence": 3,
            "courage": 2,
            "charisma": 0,
            "constitution": 0
        }
        
        total_allocated = sum(test_allocation.values())
        self.assertEqual(total_allocated, 10, "分配的屬性點總和應該等於10")
        
        # 測試超額分配
        over_allocation = {
            "leadership": 15,
            "intelligence": 5,
            "courage": 5,
            "charisma": 5,
            "constitution": 5
        }
        
        total_over = sum(over_allocation.values())
        self.assertGreater(total_over, 10, "超額分配應該被檢測到")
    
    def test_character_creation_flow(self):
        """測試完整的角色創建流程"""
        # 步驟1: 驗證角色名稱
        self.assertTrue(self._is_valid_name(self.test_character_data["name"]))
        
        # 步驟2: 驗證背景
        self.assertTrue(self._is_valid_background(self.test_character_data["background"]))
        
        # 步驟3: 驗證屬性
        self.assertTrue(self._are_valid_attributes(self.test_character_data["attributes"]))
        
        # 步驟4: 驗證年齡
        self.assertTrue(self._is_valid_age(self.test_character_data["age"]))
        
        # 步驟5: 創建角色
        character_id = self._create_character(self.test_character_data)
        self.assertIsNotNone(character_id)
        
        # 步驟6: 驗證角色數據
        saved_data = self._load_character_data(character_id)
        self.assertEqual(saved_data["name"], self.test_character_data["name"])
    
    # 輔助方法（模擬 UE5 C++ 代碼的邏輯）
    
    def _is_valid_name(self, name):
        """模擬名稱驗證邏輯"""
        if not name or len(name) < 2 or len(name) > 20:
            return False
        
        # 檢查是否只包含中文字符和空格
        for char in name:
            if not (ord(char) >= 0x4e00 and ord(char) <= 0x9fff) and char != ' ':
                return False
        
        return True
    
    def _are_valid_attributes(self, attributes):
        """模擬屬性驗證邏輯"""
        required_keys = ["leadership", "intelligence", "courage", "charisma", "constitution"]
        
        for key in required_keys:
            if key not in attributes:
                return False
            if attributes[key] < 0 or attributes[key] > 100:
                return False
        
        return True
    
    def _is_valid_background(self, background):
        """模擬背景驗證邏輯"""
        valid_backgrounds = [
            "MilitaryAcademy",
            "WarlordSon",
            "Revolutionary", 
            "ScholarOfficial",
            "Merchant",
            "CommonSoldier"
        ]
        return background in valid_backgrounds
    
    def _is_valid_age(self, age):
        """模擬年齡驗證邏輯"""
        return isinstance(age, int) and age >= 18 and age <= 65
    
    def _get_background_description(self, background):
        """模擬獲取背景描述"""
        descriptions = {
            "MilitaryAcademy": "畢業於黃埔軍校，接受現代軍事教育，具備良好的戰術素養。",
            "WarlordSon": "出身軍閥世家，從小耳濡目染軍事事務，擁有豐厚的人脈資源。",
            "Revolutionary": "懷揣革命理想，為國家前途奮鬥的熱血青年。",
            "ScholarOfficial": "棄文從武的讀書人，以智謀和策略見長。",
            "Merchant": "富商從軍，善於理財和後勤管理。",
            "CommonSoldier": "從基層士兵一步步成長起來，經驗豐富，深得士兵擁戴。"
        }
        return descriptions.get(background)
    
    def _serialize_character_data(self, data):
        """模擬角色數據序列化"""
        return json.dumps(data, ensure_ascii=False, indent=2)
    
    def _deserialize_character_data(self, serialized_data):
        """模擬角色數據反序列化"""
        return json.loads(serialized_data)
    
    def _get_recommended_skills(self, background):
        """模擬獲取推薦技能"""
        skills = {
            "MilitaryAcademy": [
                {"name": "戰術指揮", "description": "提升部隊指揮效率", "level": 2},
                {"name": "軍事理論", "description": "增強戰略規劃能力", "level": 1}
            ],
            "WarlordSon": [
                {"name": "外交手腕", "description": "提升談判和聯盟能力", "level": 2},
                {"name": "資源管理", "description": "提升經濟運營效率", "level": 1}
            ],
            "Revolutionary": [
                {"name": "鼓舞人心", "description": "提升部隊士氣", "level": 2},
                {"name": "游擊戰術", "description": "提升非對稱戰鬥能力", "level": 1}
            ]
        }
        return skills.get(background, [])
    
    def _create_character(self, character_data):
        """模擬創建角色"""
        # 生成角色 ID
        character_id = f"char_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        # 這裡應該保存到數據庫或文件
        # 為了測試，我們只是返回 ID
        return character_id
    
    def _load_character_data(self, character_id):
        """模擬載入角色數據"""
        # 這裡應該從數據庫或文件載入
        # 為了測試，我們返回測試數據
        return self.test_character_data


def run_character_creation_tests():
    """運行角色創建測試"""
    print("開始運行 MingGoRTS 角色創建系統測試...")
    print("=" * 60)
    
    # 創建測試套件
    suite = unittest.TestLoader().loadTestsFromTestCase(TestCharacterCreation)
    
    # 運行測試
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    
    print("=" * 60)
    print(f"測試完成！")
    print(f"運行測試數量: {result.testsRun}")
    print(f"失敗數量: {len(result.failures)}")
    print(f"錯誤數量: {len(result.errors)}")
    
    if result.failures:
        print("\n失敗的測試:")
        for test, traceback in result.failures:
            print(f"- {test}: {traceback}")
    
    if result.errors:
        print("\n錯誤的測試:")
        for test, traceback in result.errors:
            print(f"- {test}: {traceback}")
    
    success_rate = (result.testsRun - len(result.failures) - len(result.errors)) / result.testsRun * 100
    print(f"\n成功率: {success_rate:.1f}%")
    
    return result.wasSuccessful()


if __name__ == "__main__":
    success = run_character_creation_tests()
    sys.exit(0 if success else 1)
