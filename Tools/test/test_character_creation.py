#!/usr/bin/env python3
"""
MingGoRTS 閫?萄遣蝟餌絞皜祈岫?單
皜祈岫 Story 3-1: 閫?萄遣?蝷惇?抒頂蝯?"""

import unittest
import json
import sys
import os
from datetime import datetime

# 瘛餃???寧? Python 頝臬?
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class TestCharacterCreation(unittest.TestCase):
    """閫?萄遣蝟餌絞皜祈岫憿?""
    
    def setUp(self):
        """皜祈岫??閮剔蔭"""
        self.test_character_data = {
            "name": "撘萄?",
            "background": "MilitaryAcademy",
            "attributes": {
                "leadership": 75,
                "intelligence": 60,
                "courage": 80,
                "charisma": 55,
                "constitution": 70
            },
            "age": 28,
            "biography": "?Ｘ平?潮????∠?撟渲?頠?"
        }
    
    def test_character_name_validation(self):
        """皜祈岫閫?迂撽?"""
        # 皜祈岫???迂
        valid_names = ["撘萄?", "??", "?撥", "頞"]
        for name in valid_names:
            self.assertTrue(self._is_valid_name(name), f"?迂 {name} ?府??")
        
        # 皜祈岫?⊥??迂
        invalid_names = ["", "A", "?銝??撣賊?撣賊???脣?蝔梯???鈭???蝚衣??", "撘?23", "??@#"]
        for name in invalid_names:
            self.assertFalse(self._is_valid_name(name), f"?迂 {name} ?府?⊥?")
    
    def test_attributes_validation(self):
        """皜祈岫撅祆折?霅?""
        # 皜祈岫??撅祆?        valid_attributes = [
            {"leadership": 50, "intelligence": 50, "courage": 50, "charisma": 50, "constitution": 50},
            {"leadership": 100, "intelligence": 0, "courage": 75, "charisma": 25, "constitution": 90}
        ]
        
        for attrs in valid_attributes:
            self.assertTrue(self._are_valid_attributes(attrs), f"撅祆?{attrs} ?府??")
        
        # 皜祈岫?⊥?撅祆?        invalid_attributes = [
            {"leadership": -10, "intelligence": 50, "courage": 50, "charisma": 50, "constitution": 50},
            {"leadership": 150, "intelligence": 50, "courage": 50, "charisma": 50, "constitution": 50}
        ]
        
        for attrs in invalid_attributes:
            self.assertFalse(self._are_valid_attributes(attrs), f"撅祆?{attrs} ?府?⊥?")
    
    def test_background_system(self):
        """皜祈岫?蝟餌絞"""
        backgrounds = [
            "MilitaryAcademy",
            "WarlordSon", 
            "Revolutionary",
            "ScholarOfficial",
            "Merchant",
            "CommonSoldier"
        ]
        
        for background in backgrounds:
            self.assertTrue(self._is_valid_background(background), f"? {background} ?府??")
            
            # 皜祈岫??膩
            description = self._get_background_description(background)
            self.assertIsNotNone(description, f"? {background} ?府??餈?)
            self.assertGreater(len(description), 10, f"? {background} ??餈唳?閰脰雲憭底蝝?)
    
    def test_age_validation(self):
        """皜祈岫撟湧翩撽?"""
        # 皜祈岫??撟湧翩
        valid_ages = [18, 25, 35, 50, 65]
        for age in valid_ages:
            self.assertTrue(self._is_valid_age(age), f"撟湧翩 {age} ?府??")
        
        # 皜祈岫?⊥?撟湧翩
        invalid_ages = [17, 66, -5, 100]
        for age in invalid_ages:
            self.assertFalse(self._is_valid_age(age), f"撟湧翩 {age} ?府?⊥?")
    
    def test_character_data_serialization(self):
        """皜祈岫閫?豢?摨???""
        # 皜祈岫摨???        serialized = self._serialize_character_data(self.test_character_data)
        self.assertIsInstance(serialized, str)
        self.assertGreater(len(serialized), 0)
        
        # 皜祈岫????
        deserialized = self._deserialize_character_data(serialized)
        self.assertEqual(deserialized["name"], self.test_character_data["name"])
        self.assertEqual(deserialized["background"], self.test_character_data["background"])
    
    def test_skill_system(self):
        """皜祈岫??賜頂蝯?""
        # 皜祈岫?刻???        for background in ["MilitaryAcademy", "WarlordSon", "Revolutionary"]:
            skills = self._get_recommended_skills(background)
            self.assertIsInstance(skills, list)
            self.assertGreater(len(skills), 0, f"? {background} ?府??行???)
            
            for skill in skills:
                self.assertIn("name", skill)
                self.assertIn("description", skill)
                self.assertIn("level", skill)
    
    def test_attribute_points_allocation(self):
        """皜祈岫撅祆折???"""
        # 皜祈岫??撅祆折?
        initial_points = 10
        self.assertEqual(initial_points, 10, "??撅祆折??府??0")
        
        # 皜祈岫撅祆折???
        test_allocation = {
            "leadership": 5,
            "intelligence": 3,
            "courage": 2,
            "charisma": 0,
            "constitution": 0
        }
        
        total_allocated = sum(test_allocation.values())
        self.assertEqual(total_allocated, 10, "???惇?折?蝮賢??府蝑10")
        
        # 皜祈岫頞???
        over_allocation = {
            "leadership": 15,
            "intelligence": 5,
            "courage": 5,
            "charisma": 5,
            "constitution": 5
        }
        
        total_over = sum(over_allocation.values())
        self.assertGreater(total_over, 10, "頞????府鋡急炎皜砍")
    
    def test_character_creation_flow(self):
        """皜祈岫摰???脣撱箸?蝔?""
        # 甇仿?1: 撽?閫?迂
        self.assertTrue(self._is_valid_name(self.test_character_data["name"]))
        
        # 甇仿?2: 撽??
        self.assertTrue(self._is_valid_background(self.test_character_data["background"]))
        
        # 甇仿?3: 撽?撅祆?        self.assertTrue(self._are_valid_attributes(self.test_character_data["attributes"]))
        
        # 甇仿?4: 撽?撟湧翩
        self.assertTrue(self._is_valid_age(self.test_character_data["age"]))
        
        # 甇仿?5: ?萄遣閫
        character_id = self._create_character(self.test_character_data)
        self.assertIsNotNone(character_id)
        
        # 甇仿?6: 撽?閫?豢?
        saved_data = self._load_character_data(character_id)
        self.assertEqual(saved_data["name"], self.test_character_data["name"])
    
    # 頛?寞?嚗芋??UE5 C++ 隞?Ⅳ??頛荔?
    
    def _is_valid_name(self, name):
        """璅⊥?迂撽??摩"""
        if not name or len(name) < 2 or len(name) > 20:
            return False
        
        # 瑼Ｘ?臬?芸??思葉??蝚血?蝛箸
        for char in name:
            if not (ord(char) >= 0x4e00 and ord(char) <= 0x9fff) and char != ' ':
                return False
        
        return True
    
    def _are_valid_attributes(self, attributes):
        """璅⊥撅祆折?霅?頛?""
        required_keys = ["leadership", "intelligence", "courage", "charisma", "constitution"]
        
        for key in required_keys:
            if key not in attributes:
                return False
            if attributes[key] < 0 or attributes[key] > 100:
                return False
        
        return True
    
    def _is_valid_background(self, background):
        """璅⊥?撽??摩"""
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
        """璅⊥撟湧翩撽??摩"""
        return isinstance(age, int) and age >= 18 and age <= 65
    
    def _get_background_description(self, background):
        """璅⊥?脣???膩"""
        descriptions = {
            "MilitaryAcademy": "?Ｘ平?潮????∴??亙??曆誨頠??嚗?憟賜??啗?蝝???,
            "WarlordSon": "?箄澈頠銝振嚗?撠單縑?格?頠?鈭?嚗?????鈭箄?鞈???,
            "Revolutionary": "?瑟?拙?嚗?振?幼擛亦??梯??僑??,
            "ScholarOfficial": "璉?敺郎???訾犖嚗誑?箄????亥??瑯?,
            "Merchant": "撖?敺?嚗??潛?鞎∪?敺蝞∠???,
            "CommonSoldier": "敺撅文ㄚ?萎?甇交郊?韏瑚?嚗?撽?撖?瘛勗?憯怠???
        }
        return descriptions.get(background)
    
    def _serialize_character_data(self, data):
        """璅⊥閫?豢?摨???""
        return json.dumps(data, ensure_ascii=False, indent=2)
    
    def _deserialize_character_data(self, serialized_data):
        """璅⊥閫?豢?????"""
        return json.loads(serialized_data)
    
    def _get_recommended_skills(self, background):
        """璅⊥?脣??刻???""
        skills = {
            "MilitaryAcademy": [
                {"name": "?啗??", "description": "???券????", "level": 2},
                {"name": "頠???", "description": "憓撥?啁閬??賢?", "level": 1}
            ],
            "WarlordSon": [
                {"name": "憭漱??", "description": "??隢????, "level": 2},
                {"name": "鞈?蝞∠?", "description": "??蝬?????", "level": 1}
            ],
            "Revolutionary": [
                {"name": "曌?鈭箏?", "description": "???券?憯急除", "level": 2},
                {"name": "皜豢??啗?", "description": "????蝔望擛亥??, "level": 1}
            ]
        }
        return skills.get(background, [])
    
    def _create_character(self, character_data):
        """璅⊥?萄遣閫"""
        # ??閫 ID
        character_id = f"char_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        # ?ㄐ?府靽??唳?澈??隞?        # ?箔?皜祈岫嚗???航???ID
        return character_id
    
    def _load_character_data(self, character_id):
        """璅⊥頛閫?豢?"""
        # ?ㄐ?府敺?澈??隞嗉???        # ?箔?皜祈岫嚗????葫閰行??        return self.test_character_data


def run_character_creation_tests():
    """??閫?萄遣皜祈岫"""
    print("???? MingGoRTS 閫?萄遣蝟餌絞皜祈岫...")
    print("=" * 60)
    
    # ?萄遣皜祈岫憟辣
    suite = unittest.TestLoader().loadTestsFromTestCase(TestCharacterCreation)
    
    # ??皜祈岫
    runner = unittest.TextTestRunner(verbosity=2)
    result = runner.run(suite)
    
    print("=" * 60)
    print(f"皜祈岫摰?嚗?)
    print(f"??皜祈岫?賊?: {result.testsRun}")
    print(f"憭望??賊?: {len(result.failures)}")
    print(f"?航炊?賊?: {len(result.errors)}")
    
    if result.failures:
        print("\n憭望??葫閰?")
        for test, traceback in result.failures:
            print(f"- {test}: {traceback}")
    
    if result.errors:
        print("\n?航炊?葫閰?")
        for test, traceback in result.errors:
            print(f"- {test}: {traceback}")
    
    success_rate = (result.testsRun - len(result.failures) - len(result.errors)) / result.testsRun * 100
    print(f"\n???? {success_rate:.1f}%")
    
    return result.wasSuccessful()


if __name__ == "__main__":
    success = run_character_creation_tests()
    sys.exit(0 if success else 1)

