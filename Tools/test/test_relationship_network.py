#!/usr/bin/env python3
"""
MingGoRTS ??蝬脩窗皜祈岫?單
皜祈岫 Story 3-5: 閫??蝬脩窗
"""

import unittest
import json
import sys
import os
from datetime import datetime
from enum import Enum

# 瘛餃???寧? Python 頝臬?
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

class ERelationshipType(Enum):
    """??憿???"""
    Family = "Family"
    Friend = "Friend"
    Mentor = "Mentor"
    Rival = "Rival"
    Ally = "Ally"
    Enemy = "Enemy"
    Colleague = "Colleague"
    Subordinate = "Subordinate"
    Superior = "Superior"
    Business = "Business"
    Political = "Political"
    Military = "Military"
    Romantic = "Romantic"
    Acquaintance = "Acquaintance"
    Stranger = "Stranger"

class ERelationshipStrength(Enum):
    """??撘瑕漲??"""
    VeryWeak = "VeryWeak"
    Weak = "Weak"
    Moderate = "Moderate"
    Strong = "Strong"
    VeryStrong = "VeryStrong"

class ERelationshipStatus(Enum):
    """???????""
    Active = "Active"
    Dormant = "Dormant"
    Strained = "Strained"
    Broken = "Broken"
    Developing = "Developing"
    Improving = "Improving"

class ERelationshipChangeType(Enum):
    """??霈?憿???"""
    Improvement = "Improvement"
    Deterioration = "Deterioration"
    Stabilization = "Stabilization"
    Conflict = "Conflict"
    Resolution = "Resolution"
    Transformation = "Transformation"

class EInfluencePropagationType(Enum):
    """敶梢?單憿???"""
    Direct = "Direct"
    Indirect = "Indirect"
    Cascading = "Cascading"
    Network = "Network"
    Global = "Global"

class ERelationshipStability(Enum):
    """??蝛拙??扳???""
    VeryStable = "VeryStable"
    Stable = "Stable"
    Moderate = "Moderate"
    Unstable = "Unstable"
    VeryUnstable = "VeryUnstable"

class TestRelationshipNetwork(unittest.TestCase):
    """??蝬脩窗皜祈岫憿?""
    
    def setUp(self):
        """皜祈岫??閮剔蔭"""
        self.relationship_network = MockRelationshipNetwork()
        self.relationship_dynamics = MockRelationshipDynamics()
        
        # 皜祈岫閫?豢?
        self.test_characters = [
            "ChiangKaiShek",
            "ZhouEnlai", 
            "MaoZedong",
            "ZhangXueliang",
            "SoongMeiLing"
        ]
        
        # 皜祈岫???豢?
        self.test_relationships = [
            {
                "source_id": "ChiangKaiShek",
                "target_id": "SoongMeiLing",
                "relationship_type": ERelationshipType.Romantic,
                "strength": ERelationshipStrength.VeryStrong,
                "status": ERelationshipStatus.Active,
                "relationship_value": 95.0,
                "trust_level": 90.0,
                "influence_level": 85.0,
                "is_reciprocal": True
            },
            {
                "source_id": "ChiangKaiShek",
                "target_id": "ZhouEnlai",
                "relationship_type": ERelationshipType.Political,
                "strength": ERelationshipStrength.Moderate,
                "status": ERelationshipStatus.Active,
                "relationship_value": 60.0,
                "trust_level": 40.0,
                "influence_level": 70.0,
                "is_reciprocal": True
            },
            {
                "source_id": "ZhouEnlai",
                "target_id": "MaoZedong",
                "relationship_type": ERelationshipType.Colleague,
                "strength": ERelationshipStrength.VeryStrong,
                "status": ERelationshipStatus.Active,
                "relationship_value": 90.0,
                "trust_level": 95.0,
                "influence_level": 80.0,
                "is_reciprocal": True
            }
        ]
        
        # 皜祈岫蝬脩窗?豢?
        self.test_network = {
            "network_id": "TestNetwork",
            "network_name": "皜祈岫蝬脩窗",
            "description": "?冽皜祈岫??靽雯蝯?,
            "character_ids": self.test_characters,
            "connections": self.test_relationships,
            "network_density": 0.6,
            "network_cohesion": 0.7,
            "central_character_id": "ChiangKaiShek",
            "key_influencers": ["ChiangKaiShek", "ZhouEnlai"],
            "bridging_characters": ["ZhangXueliang"],
            "is_stable": True,
            "is_growing": True
        }
    
    def test_relationship_network_initialization(self):
        """皜祈岫??蝬脩窗蝟餌絞????""
        self.relationship_network.initialize()
        
        self.assertTrue(self.relationship_network.is_initialized)
        self.assertGreater(len(self.relationship_network.all_networks), 0)
        self.assertGreater(len(self.relationship_network.network_map), 0)
        
        print("????蝬脩窗蝟餌絞???葫閰阡?")
    
    def test_create_network(self):
        """皜祈岫?萄遣??蝬脩窗"""
        self.relationship_network.initialize()
        
        # ?萄遣?啁雯蝯?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        
        self.assertIsInstance(network_id, str)
        self.assertGreater(len(network_id), 0)
        self.assertIn("TestNetwork", network_id)
        
        # 瑼Ｘ蝬脩窗?臬?萄遣??
        network = self.relationship_network.get_network(network_id)
        self.assertEqual(network["network_name"], "皜祈岫蝬脩窗")
        self.assertEqual(len(network["character_ids"]), len(self.test_characters))
        
        print("???萄遣??蝬脩窗皜祈岫??")
    
    def test_create_relationship(self):
        """皜祈岫?萄遣????"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗
        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        
        # ?萄遣??
        success = self.relationship_network.create_relationship(
            "ChiangKaiShek", 
            "SoongMeiLing", 
            ERelationshipType.Romantic, 
            ERelationshipStrength.VeryStrong
        )
        
        self.assertTrue(success)
        
        # 瑼Ｘ???臬?萄遣??
        relationship = self.relationship_network.get_relationship("ChiangKaiShek", "SoongMeiLing")
        self.assertEqual(relationship["relationship_type"], ERelationshipType.Romantic)
        self.assertEqual(relationship["strength"], ERelationshipStrength.VeryStrong)
        self.assertGreater(relationship["relationship_value"], 0.0)
        
        print("???萄遣????皜祈岫??")
    
    def test_update_relationship(self):
        """皜祈岫?湔??"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # ?湔??
        success = self.relationship_network.update_relationship("ChiangKaiShek", "SoongMeiLing", 85.0, "????")
        
        self.assertTrue(success)
        
        # 瑼Ｘ???臬?湔
        relationship = self.relationship_network.get_relationship("ChiangKaiShek", "SoongMeiLing")
        self.assertEqual(relationship["relationship_value"], 85.0)
        
        print("???湔??皜祈岫??")
    
    def test_get_character_relationships(self):
        """皜祈岫?脣?閫??"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗????靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        
        # ?脣?閫??
        relationships = self.relationship_network.get_character_relationships("ChiangKaiShek")
        
        self.assertEqual(len(relationships), 2)
        
        # 瑼Ｘ??憿?
        relationship_types = [rel["relationship_type"] for rel in relationships]
        self.assertIn(ERelationshipType.Romantic, relationship_types)
        self.assertIn(ERelationshipType.Political, relationship_types)
        
        print("???脣?閫??皜祈岫??")
    
    def test_direct_connections(self):
        """皜祈岫?脣??湔??"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # ?脣??湔??
        direct_connections = self.relationship_network.get_direct_connections("ChiangKaiShek")
        
        self.assertEqual(len(direct_connections), 2)
        self.assertIn("SoongMeiLing", direct_connections)
        self.assertIn("ZhouEnlai", direct_connections)
        
        print("???脣??湔??皜祈岫??")
    
    def test_indirect_connections(self):
        """皜祈岫?脣????"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # ?脣????
        indirect_connections = self.relationship_network.get_indirect_connections("ChiangKaiShek")
        
        self.assertGreater(len(indirect_connections), 0)
        self.assertIn("MaoZedong", indirect_connections)  # ?? ZhouEnlai ???仿?
        
        print("???脣????皜祈岫??")
    
    def test_shortest_path(self):
        """皜祈岫??剛楝敺?蝞?""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 閮???剛楝敺?        shortest_path = self.relationship_network.find_shortest_path("ChiangKaiShek", "MaoZedong")
        
        self.assertEqual(len(shortest_path), 3)
        self.assertEqual(shortest_path[0], "ChiangKaiShek")
        self.assertEqual(shortest_path[1], "ZhouEnlai")
        self.assertEqual(shortest_path[2], "MaoZedong")
        
        print("????剛楝敺?蝞葫閰阡?")
    
    def test_network_analysis(self):
        """皜祈岫蝬脩窗??"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # ??蝬脩窗
        self.relationship_network.analyze_network(network_id)
        
        # 瑼Ｘ??蝯?
        network = self.relationship_network.get_network(network_id)
        self.assertGreater(network["network_density"], 0.0)
        self.assertGreater(network["network_cohesion"], 0.0)
        self.assertIsNot(network["central_character_id"], "")
        
        print("??蝬脩窗??皜祈岫??")
    
    def test_influence_calculation(self):
        """皜祈岫敶梢??蝞?""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        
        # 閮?敶梢??        influence = self.relationship_network.calculate_influence("ChiangKaiShek")
        
        self.assertGreater(influence, 0.0)
        
        print("??敶梢??蝞葫閰阡?")
    
    def test_relationship_suggestions(self):
        """皜祈岫??撱箄降"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗???靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # ?脣???撱箄降
        suggestions = self.relationship_network.get_relationship_suggestions("ChiangKaiShek")
        
        self.assertIsInstance(suggestions, list)
        
        print("????撱箄降皜祈岫??")
    
    def test_relationship_dynamics_initialization(self):
        """皜祈岫????蝟餌絞????""
        self.relationship_dynamics.initialize()
        
        self.assertTrue(self.relationship_dynamics.is_initialized)
        
        print("??????蝟餌絞???葫閰阡?")
    
    def test_apply_relationship_change(self):
        """皜祈岫???霈?"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # ???霈?
        success = self.relationship_dynamics.apply_relationship_change(
            "ChiangKaiShek", 
            "SoongMeiLing", 
            ERelationshipChangeType.Improvement, 
            5.0, 
            "蝛扔鈭?"
        )
        
        self.assertTrue(success)
        
        # 瑼Ｘ霈?甇瑕
        history = self.relationship_dynamics.get_relationship_change_history("ChiangKaiShek", "SoongMeiLing")
        self.assertGreater(len(history), 0)
        
        print("?????霈?皜祈岫??")
    
    def test_influence_propagation(self):
        """皜祈岫敶梢?單"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # ?單敶梢
        propagation_id = self.relationship_dynamics.propagate_influence(
            "ChiangKaiShek", 
            "?踵祥敶梢", 
            10.0, 
            EInfluencePropagationType.Network, 
            3
        )
        
        self.assertIsInstance(propagation_id, str)
        self.assertGreater(len(propagation_id), 0)
        
        # 瑼Ｘ瘣餃??單
        active_propagations = self.relationship_dynamics.get_active_propagations()
        self.assertGreater(len(active_propagations), 0)
        
        print("??敶梢?單皜祈岫??")
    
    def test_network_stability_analysis(self):
        """皜祈岫蝬脩窗蝛拙??批???""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # ??蝬脩窗蝛拙???        self.relationship_dynamics.analyze_network_stability(network_id)
        
        # 瑼Ｘ??蝯?
        analysis = self.relationship_dynamics.get_stability_analysis(network_id)
        self.assertEqual(analysis["network_id"], network_id)
        self.assertIsInstance(analysis["overall_stability"], ERelationshipStability)
        self.assertGreater(analysis["stability_score"], 0.0)
        
        print("??蝬脩窗蝛拙??批??葫閰阡?")
    
    def test_relationship_trend_prediction(self):
        """皜祈岫??頞典?葫"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # ?銝鈭??誑?萄遣甇瑕
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Improvement, 3.0, "蝛扔鈭?")
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Improvement, 2.0, "?勗??格?")
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Deterioration, 1.0, "撠?甇?)
        
        # ?葫頞典
        trend = self.relationship_dynamics.predict_relationship_trend("ChiangKaiShek", "SoongMeiLing", 5)
        
        self.assertEqual(len(trend), 5)
        for value in trend:
            self.assertGreaterEqual(value, 0.0)
            self.assertLessEqual(value, 100.0)
        
        print("????頞典?葫皜祈岫??")
    
    def test_time_decay(self):
        """皜祈岫??銵唳?"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        
        # 閮?銵唳?
        decay = self.relationship_dynamics.calculate_relationship_decay("ChiangKaiShek", "SoongMeiLing", 10.0)
        
        self.assertGreaterEqual(decay, 0.0)
        
        # ???銵唳?
        self.relationship_dynamics.apply_time_decay(1.0)
        
        print("????銵唳?皜祈岫??")
    
    def test_relationship_conflicts(self):
        """皜祈岫??銵?瑼Ｘ葫"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??蝒?靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Enemy, ERelationshipStrength.Strong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Friend, ERelationshipStrength.Strong)
        
        # 瑼Ｘ銵?
        conflicts = self.relationship_dynamics.check_relationship_conflicts(network_id)
        
        # ?府瑼Ｘ葫?啗?蝒?        self.assertGreater(len(conflicts), 0)
        
        # 閫?捱銵?
        success = self.relationship_dynamics.resolve_relationship_conflict("ChiangKaiShek", "ZhouEnlai")
        self.assertTrue(success)
        
        print("????銵?瑼Ｘ葫皜祈岫??")
    
    def test_network_dynamics_calculation(self):
        """皜祈岫蝬脩窗??閮?"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        
        # 閮?蝬脩窗??
        dynamics = self.relationship_dynamics.calculate_network_dynamics(network_id)
        
        self.assertIsInstance(dynamics, dict)
        self.assertIn("ChangeFrequency", dynamics)
        self.assertIn("InfluenceStrength", dynamics)
        self.assertIn("StabilityIndex", dynamics)
        self.assertIn("ResilienceIndex", dynamics)
        
        print("??蝬脩窗??閮?皜祈岫??")
    
    def test_relationship_development_simulation(self):
        """皜祈岫???澆?璅⊥"""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # ?萄遣蝬脩窗??靽?        network_id = self.relationship_network.create_network("皜祈岫蝬脩窗", self.test_characters)
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 璅⊥???澆?
        self.relationship_dynamics.simulate_relationship_development(network_id, 5)
        
        # 瑼Ｘ璅⊥蝯?
        analysis = self.relationship_dynamics.get_stability_analysis(network_id)
        self.assertIsNot(analysis["network_id"], "")
        
        print("?????澆?璅⊥皜祈岫??")
    
    def test_complete_relationship_flow(self):
        """皜祈岫摰??靽雯蝯⊥?蝔?""
        self.relationship_network.initialize()
        self.relationship_dynamics.initialize()
        
        # 1. ?萄遣蝬脩窗
        network_id = self.relationship_network.create_network("摰皜祈岫蝬脩窗", self.test_characters)
        self.assertIsInstance(network_id, str)
        
        # 2. 瘛餃???
        self.relationship_network.create_relationship("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong)
        self.relationship_network.create_relationship("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate)
        self.relationship_network.create_relationship("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong)
        
        # 3. ??蝬脩窗
        self.relationship_network.analyze_network(network_id)
        network = self.relationship_network.get_network(network_id)
        self.assertGreater(network["network_density"], 0.0)
        
        # 4. ???霈?
        self.relationship_dynamics.apply_relationship_change("ChiangKaiShek", "SoongMeiLing", ERelationshipChangeType.Improvement, 5.0, "蝛扔鈭?")
        
        # 5. ?單敶梢
        propagation_id = self.relationship_dynamics.propagate_influence("ChiangKaiShek", "?踵祥敶梢", 10.0, EInfluencePropagationType.Network, 2)
        self.assertIsInstance(propagation_id, str)
        
        # 6. ??蝛拙???        self.relationship_dynamics.analyze_network_stability(network_id)
        analysis = self.relationship_dynamics.get_stability_analysis(network_id)
        self.assertIsInstance(analysis["overall_stability"], ERelationshipStability)
        
        # 7. ?葫頞典
        trend = self.relationship_dynamics.predict_relationship_trend("ChiangKaiShek", "SoongMeiLing", 3)
        self.assertEqual(len(trend), 3)
        
        # 8. ?脣?撱箄降
        suggestions = self.relationship_network.get_relationship_suggestions("ChiangKaiShek")
        self.assertIsInstance(suggestions, list)
        
        # 9. 璅⊥?澆?
        self.relationship_dynamics.simulate_relationship_development(network_id, 3)
        
        # 10. 瑼Ｘ?蝯???        final_network = self.relationship_network.get_network(network_id)
        self.assertGreater(len(final_network["connections"]), 0)
        
        print("??摰??蝬脩窗瘚?皜祈岫??")
    
    def test_historical_network_authenticity(self):
        """皜祈岫甇瑕蝬脩窗?祕??""
        self.relationship_network.initialize()
        
        # 瑼Ｘ??暺函雯蝯?        nationalist_network = self.relationship_network.get_network("NET_Nationalist")
        if nationalist_network["network_id"]:
            self.assertEqual(nationalist_network["network_name"], "??暺函雯蝯?)
            self.assertIn("ChiangKaiShek", nationalist_network["character_ids"])
            self.assertIn("SoongMeiLing", nationalist_network["character_ids"])
        
        # 瑼Ｘ?梁暺函雯蝯?        communist_network = self.relationship_network.get_network("NET_Communist")
        if communist_network["network_id"]:
            self.assertEqual(communist_network["network_name"], "?梁暺函雯蝯?)
            self.assertIn("MaoZedong", communist_network["character_ids"])
            self.assertIn("ZhouEnlai", communist_network["character_ids"])
        
        # 瑼Ｘ頠蝬脩窗
        warlord_network = self.relationship_network.get_network("NET_Warlord")
        if warlord_network["network_id"]:
            self.assertEqual(warlord_network["network_name"], "頠蝬脩窗")
            self.assertIn("ZhangZuolin", warlord_network["character_ids"])
            self.assertIn("ZhangXueliang", warlord_network["character_ids"])
        
        print("??甇瑕蝬脩窗?祕?扳葫閰阡?")
    
    def test_relationship_type_system(self):
        """皜祈岫??憿?蝟餌絞"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗
        network_id = self.relationship_network.create_network("??憿?皜祈岫", self.test_characters)
        
        # 皜祈岫銝???憿?
        relationship_types = [
            (ERelationshipType.Family, ERelationshipStrength.VeryStrong),
            (ERelationshipType.Friend, ERelationshipStrength.Strong),
            (ERelationshipType.Rival, ERelationshipStrength.Moderate),
            (ERelationshipType.Enemy, ERelationshipStrength.Weak),
            (ERelationshipType.Colleague, ERelationshipStrength.Strong),
            (ERelationshipType.Business, ERelationshipStrength.Moderate)
        ]
        
        for i, (rel_type, strength) in enumerate(relationship_types):
            if i < len(self.test_characters) - 1:
                success = self.relationship_network.create_relationship(
                    self.test_characters[i], 
                    self.test_characters[i + 1], 
                    rel_type, 
                    strength
                )
                self.assertTrue(success)
                
                # 瑼Ｘ??憿?
                relationship = self.relationship_network.get_relationship(self.test_characters[i], self.test_characters[i + 1])
                self.assertEqual(relationship["relationship_type"], rel_type)
                self.assertEqual(relationship["strength"], strength)
        
        print("????憿?蝟餌絞皜祈岫??")
    
    def test_network_statistics(self):
        """皜祈岫蝬脩窗蝯梯?"""
        self.relationship_network.initialize()
        
        # ?萄遣蝬脩窗????靽?        network_id = self.relationship_network.create_network("蝯梯?皜祈岫蝬脩窗", self.test_characters)
        
        # ?萄遣憭?靽?        relationships_to_create = [
            ("ChiangKaiShek", "SoongMeiLing", ERelationshipType.Romantic, ERelationshipStrength.VeryStrong),
            ("ChiangKaiShek", "ZhouEnlai", ERelationshipType.Political, ERelationshipStrength.Moderate),
            ("ZhouEnlai", "MaoZedong", ERelationshipType.Colleague, ERelationshipStrength.VeryStrong),
            ("ZhangXueliang", "ChiangKaiShek", ERelationshipType.Political, ERelationshipStrength.Moderate),
            ("SoongMeiLing", "ZhouEnlai", ERelationshipType.Diplomatic, ERelationshipStrength.Moderate)
        ]
        
        for source, target, rel_type, strength in relationships_to_create:
            self.relationship_network.create_relationship(source, target, rel_type, strength)
        
        # ??蝬脩窗
        self.relationship_network.analyze_network(network_id)
        
        # 瑼Ｘ蝯梯??豢?
        network = self.relationship_network.get_network(network_id)
        self.assertGreater(network["network_density"], 0.0)
        self.assertGreater(network["network_cohesion"], 0.0)
        self.assertIsNot(network["central_character_id"], "")
        self.assertGreater(len(network["key_influencers"]), 0)
        
        print("??蝬脩窗蝯梯?皜祈岫??")


class MockRelationshipNetwork:
    """璅⊥??蝬脩窗蝟餌絞"""
    
    def __init__(self):
        self.is_initialized = False
        self.all_networks = []
        self.network_map = {}
        self.character_network_map = {}
        self.relationship_map = {}
        self._initialize_historical_networks()
    
    def initialize(self):
        """????靽雯蝯∠頂蝯?""
        self.is_initialized = True
        self._build_network_maps()
    
    def _initialize_historical_networks(self):
        """???風?脩雯蝯?""
        networks = [
            {
                "network_id": "NET_Nationalist",
                "network_name": "??暺函雯蝯?,
                "description": "??暺典?券?靽雯蝯?,
                "character_ids": ["ChiangKaiShek", "SoongMeiLing", "ChenCheng", "HeYingqin"],
                "connections": [
                    {
                        "source_character_id": "ChiangKaiShek",
                        "target_character_id": "SoongMeiLing",
                        "relationship_type": ERelationshipType.Romantic,
                        "strength": ERelationshipStrength.VeryStrong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 95.0,
                        "trust_level": 90.0,
                        "influence_level": 85.0,
                        "is_reciprocal": True
                    },
                    {
                        "source_character_id": "ChiangKaiShek",
                        "target_character_id": "ChenCheng",
                        "relationship_type": ERelationshipType.Subordinate,
                        "strength": ERelationshipStrength.Strong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 80.0,
                        "trust_level": 75.0,
                        "influence_level": 90.0,
                        "is_reciprocal": False
                    }
                ],
                "network_density": 0.0,
                "network_cohesion": 0.0,
                "central_character_id": "",
                "key_influencers": [],
                "bridging_characters": [],
                "is_stable": False,
                "is_growing": False
            },
            {
                "network_id": "NET_Communist",
                "network_name": "?梁暺函雯蝯?,
                "description": "?梁暺典?券?靽雯蝯?,
                "character_ids": ["MaoZedong", "ZhouEnlai", "ZhuDe", "LiuShaoqi"],
                "connections": [
                    {
                        "source_character_id": "MaoZedong",
                        "target_character_id": "ZhouEnlai",
                        "relationship_type": ERelationshipType.Colleague,
                        "strength": ERelationshipStrength.VeryStrong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 90.0,
                        "trust_level": 95.0,
                        "influence_level": 80.0,
                        "is_reciprocal": True
                    },
                    {
                        "source_character_id": "ZhouEnlai",
                        "target_character_id": "ZhuDe",
                        "relationship_type": ERelationshipType.Colleague,
                        "strength": ERelationshipStrength.Strong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 75.0,
                        "trust_level": 80.0,
                        "influence_level": 70.0,
                        "is_reciprocal": True
                    }
                ],
                "network_density": 0.0,
                "network_cohesion": 0.0,
                "central_character_id": "",
                "key_influencers": [],
                "bridging_characters": [],
                "is_stable": False,
                "is_growing": False
            },
            {
                "network_id": "NET_Warlord",
                "network_name": "頠蝬脩窗",
                "description": "頠銋???靽雯蝯?,
                "character_ids": ["ZhangZuolin", "ZhangXueliang", "FengYuxiang", "YanXishan"],
                "connections": [
                    {
                        "source_character_id": "ZhangZuolin",
                        "target_character_id": "ZhangXueliang",
                        "relationship_type": ERelationshipType.Family,
                        "strength": ERelationshipStrength.VeryStrong,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 95.0,
                        "trust_level": 90.0,
                        "influence_level": 85.0,
                        "is_reciprocal": True
                    },
                    {
                        "source_character_id": "ZhangZuolin",
                        "target_character_id": "FengYuxiang",
                        "relationship_type": ERelationshipType.Rival,
                        "strength": ERelationshipStrength.Moderate,
                        "status": ERelationshipStatus.Active,
                        "relationship_value": 40.0,
                        "trust_level": 30.0,
                        "influence_level": 60.0,
                        "is_reciprocal": True
                    }
                ],
                "network_density": 0.0,
                "network_cohesion": 0.0,
                "central_character_id": "",
                "key_influencers": [],
                "bridging_characters": [],
                "is_stable": False,
                "is_growing": False
            }
        ]
        
        self.all_networks = networks
    
    def _build_network_maps(self):
        """瑽遣蝬脩窗??"""
        for network in self.all_networks:
            self.network_map[network["network_id"]] = network
            
            # 瑽遣閫?啁雯蝯∠???
            for character_id in network["character_ids"]:
                self.character_network_map[character_id] = network["network_id"]
            
            # 瑽遣????
            for connection in network["connections"]:
                relationship_id = f"{connection['source_character_id']}_{connection['target_character_id']}"
                self.relationship_map[relationship_id] = connection
    
    def create_network(self, network_name, character_ids):
        """?萄遣蝬脩窗"""
        network_id = f"NET_{network_name}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_network = {
            "network_id": network_id,
            "network_name": network_name,
            "description": f"?冽?萄遣?雯蝯∴?{network_name}",
            "character_ids": character_ids,
            "connections": [],
            "network_density": 0.0,
            "network_cohesion": 0.0,
            "central_character_id": "",
            "key_influencers": [],
            "bridging_characters": [],
            "is_stable": False,
            "is_growing": False
        }
        
        self.all_networks.append(new_network)
        self.network_map[network_id] = new_network
        
        # 瑽遣閫?啁雯蝯∠???
        for character_id in character_ids:
            self.character_network_map[character_id] = network_id
        
        return network_id
    
    def get_network(self, network_id):
        """?脣?蝬脩窗"""
        return self.network_map.get(network_id, {
            "network_id": "",
            "network_name": "",
            "description": "",
            "character_ids": [],
            "connections": [],
            "network_density": 0.0,
            "network_cohesion": 0.0,
            "central_character_id": "",
            "key_influencers": [],
            "bridging_characters": [],
            "is_stable": False,
            "is_growing": False
        })
    
    def create_relationship(self, source_id, target_id, relationship_type, strength):
        """?萄遣??"""
        if source_id == target_id:
            return False
        
        relationship_id = f"{source_id}_{target_id}"
        
        if relationship_id in self.relationship_map:
            return False
        
        new_connection = {
            "source_character_id": source_id,
            "target_character_id": target_id,
            "relationship_type": relationship_type,
            "strength": strength,
            "status": ERelationshipStatus.Active,
            "relationship_value": self._get_strength_value(strength) * 100.0,
            "trust_level": 50.0,
            "influence_level": 50.0,
            "is_reciprocal": self._is_symmetric(relationship_type),
            "last_interaction_date": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "interaction_frequency": 1.0
        }
        
        self.relationship_map[relationship_id] = new_connection
        
        # 瘛餃??啁雯蝯?        if source_id in self.character_network_map:
            network_id = self.character_network_map[source_id]
            if network_id in self.network_map:
                self.network_map[network_id]["connections"].append(new_connection)
        
        return True
    
    def update_relationship(self, source_id, target_id, new_value, reason):
        """?湔??"""
        relationship_id = f"{source_id}_{target_id}"
        
        if relationship_id not in self.relationship_map:
            return False
        
        connection = self.relationship_map[relationship_id]
        connection["relationship_value"] = max(0.0, min(100.0, new_value))
        connection["last_interaction_date"] = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        connection["interaction_frequency"] += 1.0
        
        return True
    
    def get_relationship(self, source_id, target_id):
        """?脣???"""
        relationship_id = f"{source_id}_{target_id}"
        return self.relationship_map.get(relationship_id, {
            "source_character_id": "",
            "target_character_id": "",
            "relationship_type": ERelationshipType.Stranger,
            "strength": ERelationshipStrength.Weak,
            "status": ERelationshipStatus.Active,
            "relationship_value": 0.0,
            "trust_level": 0.0,
            "influence_level": 0.0,
            "is_reciprocal": False
        })
    
    def get_character_relationships(self, character_id):
        """?脣?閫??"""
        relationships = []
        
        for relationship_id, connection in self.relationship_map.items():
            if connection["source_character_id"] == character_id or connection["target_character_id"] == character_id:
                relationships.append(connection)
        
        return relationships
    
    def get_direct_connections(self, character_id):
        """?脣??湔??"""
        direct_connections = []
        
        for relationship_id, connection in self.relationship_map.items():
            if connection["source_character_id"] == character_id:
                direct_connections.append(connection["target_character_id"])
            elif connection["target_character_id"] == character_id and connection["is_reciprocal"]:
                direct_connections.append(connection["source_character_id"])
        
        return direct_connections
    
    def get_indirect_connections(self, character_id):
        """?脣????"""
        indirect_connections = []
        direct_connections = self.get_direct_connections(character_id)
        processed_characters = {character_id}
        
        # 瘛餃??湔??
        for direct_id in direct_connections:
            processed_characters.add(direct_id)
        
        # ?交鈭漲??
        for direct_id in direct_connections:
            second_level_connections = self.get_direct_connections(direct_id)
            for second_level_id in second_level_connections:
                if second_level_id not in processed_characters:
                    indirect_connections.append(second_level_id)
                    processed_characters.add(second_level_id)
        
        return indirect_connections
    
    def find_shortest_path(self, source_id, target_id):
        """撠??剛楝敺?""
        if source_id == target_id:
            return [source_id]
        
        # 蝪∪?BFS撖衣
        from collections import deque
        
        queue = deque([source_id])
        visited = {source_id}
        parent = {}
        
        while queue:
            current = queue.popleft()
            
            if current == target_id:
                # ?遣頝臬?
                path = []
                node = target_id
                while node != source_id:
                    path.append(node)
                    node = parent[node]
                path.append(source_id)
                return list(reversed(path))
            
            connections = self.get_direct_connections(current)
            for connection in connections:
                if connection not in visited:
                    visited.add(connection)
                    parent[connection] = current
                    queue.append(connection)
        
        return []
    
    def analyze_network(self, network_id):
        """??蝬脩窗"""
        if network_id not in self.network_map:
            return
        
        network = self.network_map[network_id]
        
        # 閮?蝬脩窗撖漲
        node_count = len(network["character_ids"])
        if node_count >= 2:
            possible_connections = node_count * (node_count - 1) // 2
            actual_connections = len(network["connections"])
            network["network_density"] = actual_connections / possible_connections
        
        # 閮?蝬脩窗????        if network["connections"]:
            total_strength = sum(conn["relationship_value"] for conn in network["connections"])
            network["network_cohesion"] = total_strength / (len(network["connections"]) * 100.0)
        
        # ?曉銝剖?閫
        max_influence = 0.0
        central_character = ""
        for character_id in network["character_ids"]:
            influence = self.calculate_influence(character_id)
            if influence > max_influence:
                max_influence = influence
                central_character = character_id
        
        network["central_character_id"] = central_character
        
        # ?曉?敶梢??        influences = []
        for character_id in network["character_ids"]:
            influence = self.calculate_influence(character_id)
            influences.append((character_id, influence))
        
        influences.sort(key=lambda x: x[1], reverse=True)
        network["key_influencers"] = [char_id for char_id, _ in influences[:len(influences)//3 + 1]]
        
        # ?曉璈閫
        bridging_characters = []
        for character_id in network["character_ids"]:
            connections = self.get_direct_connections(character_id)
            if len(connections) >= 3:
                bridging_characters.append(character_id)
        
        network["bridging_characters"] = bridging_characters
        
        # ?斗蝬脩窗???        network["is_stable"] = network["network_cohesion"] > 0.7
        network["is_growing"] = network["network_density"] > 0.5
    
    def calculate_influence(self, character_id):
        """閮?敶梢??""
        total_influence = 0.0
        relationships = self.get_character_relationships(character_id)
        
        for connection in relationships:
            weight = self._calculate_relationship_weight(connection)
            total_influence += weight
        
        return total_influence
    
    def get_relationship_suggestions(self, character_id):
        """?脣???撱箄降"""
        suggestions = []
        
        # ?脣????
        indirect_connections = self.get_indirect_connections(character_id)
        
        # ?脣??暹???
        existing_relationships = self.get_character_relationships(character_id)
        existing_targets = {conn["target_character_id"] for conn in existing_relationships}
        
        # 撱箄降???仿?撱箇???
        for indirect_id in indirect_connections:
            if indirect_id not in existing_targets:
                suggestions.append(f"撱箄降??{indirect_id} 撱箇???")
        
        return suggestions
    
    def _get_strength_value(self, strength):
        """?脣?撘瑕漲?詨?""
        strength_values = {
            ERelationshipStrength.VeryWeak: 0.1,
            ERelationshipStrength.Weak: 0.3,
            ERelationshipStrength.Moderate: 0.5,
            ERelationshipStrength.Strong: 0.8,
            ERelationshipStrength.VeryStrong: 1.0
        }
        return strength_values.get(strength, 0.5)
    
    def _is_symmetric(self, relationship_type):
        """瑼Ｘ???臬撠迂"""
        symmetric_types = {
            ERelationshipType.Family,
            ERelationshipType.Friend,
            ERelationshipType.Ally,
            ERelationshipType.Rival,
            ERelationshipType.Enemy,
            ERelationshipType.Colleague,
            ERelationshipType.Acquaintance
        }
        return relationship_type in symmetric_types
    
    def _calculate_relationship_weight(self, connection):
        """閮???甈?"""
        type_weights = {
            ERelationshipType.Family: 1.0,
            ERelationshipType.Romantic: 0.95,
            ERelationshipType.Friend: 0.9,
            ERelationshipType.Ally: 0.85,
            ERelationshipType.Mentor: 0.8,
            ERelationshipType.Colleague: 0.7,
            ERelationshipType.Business: 0.6,
            ERelationshipType.Political: 0.65,
            ERelationshipType.Military: 0.7,
            ERelationshipType.Subordinate: 0.5,
            ERelationshipType.Superior: 0.55,
            ERelationshipType.Rival: 0.3,
            ERelationshipType.Enemy: 0.1,
            ERelationshipType.Acquaintance: 0.4,
            ERelationshipType.Stranger: 0.2
        }
        
        type_weight = type_weights.get(connection["relationship_type"], 0.5)
        strength_weight = self._get_strength_value(connection["strength"])
        value_weight = connection["relationship_value"] / 100.0
        
        return type_weight * strength_weight * value_weight


class MockRelationshipDynamics:
    """璅⊥????蝟餌絞"""
    
    def __init__(self):
        self.is_initialized = False
        self.relationship_changes = []
        self.active_propagations = []
        self.stability_analyses = {}
        self.relationship_network = None
        self.base_decay_rate = 0.01
        self.influence_decay_rate = 0.05
    
    def initialize(self):
        """????靽??頂蝯?""
        self.is_initialized = True
    
    def apply_relationship_change(self, source_id, target_id, change_type, change_value, reason):
        """???霈?"""
        change_id = f"Change_{source_id}_{target_id}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_change = {
            "change_id": change_id,
            "source_character_id": source_id,
            "target_character_id": target_id,
            "change_type": change_type,
            "old_value": 50.0,  # 暺???            "new_value": max(0.0, min(100.0, 50.0 + change_value)),
            "change_magnitude": abs(change_value),
            "change_reason": reason,
            "context": "",
            "trigger_event_id": "",
            "change_time": datetime.now(),
            "decay_rate": 0.0,
            "is_permanent": False,
            "propagates": change_type != ERelationshipChangeType.Stabilization
        }
        
        self.relationship_changes.append(new_change)
        return True
    
    def get_relationship_change_history(self, source_id, target_id):
        """?脣???霈?甇瑕"""
        history = []
        
        for change in self.relationship_changes:
            if change["source_character_id"] == source_id and change["target_character_id"] == target_id:
                history.append(change)
        
        return history
    
    def propagate_influence(self, source_id, influence_type, influence_value, propagation_type, max_depth):
        """?單敶梢"""
        propagation_id = f"Propagation_{source_id}_{influence_type}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        new_propagation = {
            "propagation_id": propagation_id,
            "source_character_id": source_id,
            "influence_type": influence_type,
            "propagation_type": propagation_type,
            "initial_influence": influence_value,
            "propagation_strength": 1.0,
            "max_depth": max_depth,
            "attenuation_factor": 0.5,
            "affected_characters": [],
            "influence_values": {},
            "start_time": datetime.now(),
            "end_time": datetime.now(),
            "is_active": True
        }
        
        self.active_propagations.append(new_propagation)
        return propagation_id
    
    def get_active_propagations(self):
        """?脣?瘣餃??單"""
        return [prop for prop in self.active_propagations if prop["is_active"]]
    
    def analyze_network_stability(self, network_id):
        """??蝬脩窗蝛拙???""
        analysis = {
            "network_id": network_id,
            "overall_stability": ERelationshipStability.Moderate,
            "stability_score": 0.6,
            "character_stability": {},
            "unstable_relationships": [],
            "critical_relationships": [],
            "risk_factors": [],
            "stabilizing_factors": [],
            "predicted_stability": 0.65,
            "analysis_time": datetime.now()
        }
        
        self.stability_analyses[network_id] = analysis
        return analysis
    
    def get_stability_analysis(self, network_id):
        """?脣?蝛拙??批???""
        return self.stability_analyses.get(network_id, {
            "network_id": "",
            "overall_stability": ERelationshipStability.Moderate,
            "stability_score": 0.0,
            "character_stability": {},
            "unstable_relationships": [],
            "critical_relationships": [],
            "risk_factors": [],
            "stabilizing_factors": [],
            "predicted_stability": 0.0,
            "analysis_time": datetime.now()
        })
    
    def predict_relationship_trend(self, source_id, target_id, time_steps):
        """?葫??頞典"""
        history = self.get_relationship_change_history(source_id, target_id)
        
        if len(history) < 2:
            return [50.0] * time_steps
        
        # 蝪∪?頞典?葫
        current_value = history[-1]["new_value"] if history else 50.0
        trend = 0.0
        
        if len(history) >= 3:
            recent_values = [change["new_value"] for change in history[-3:]]
            if len(recent_values) >= 2:
                trend = (recent_values[-1] - recent_values[0]) / len(recent_values)
        
        trend_values = []
        for i in range(time_steps):
            predicted_value = current_value + (trend * (i + 1))
            trend_values.append(max(0.0, min(100.0, predicted_value)))
        
        return trend_values
    
    def calculate_relationship_decay(self, source_id, target_id, time_elapsed):
        """閮???銵唳?"""
        change_frequency = self._get_change_frequency(source_id, target_id)
        
        decay_rate = self.base_decay_rate
        if change_frequency > 0.1:
            decay_rate *= 0.5
        
        decay_amount = decay_rate * time_elapsed
        return max(0.0, decay_amount)
    
    def apply_time_decay(self, delta_time):
        """???銵唳?"""
        # 蝪∪?撖衣
        pass
    
    def check_relationship_conflicts(self, network_id):
        """瑼Ｘ??銵?"""
        conflicts = []
        
        # 蝪∪?撖衣嚗??征?”
        return conflicts
    
    def resolve_relationship_conflict(self, source_id, target_id):
        """閫?捱??銵?"""
        # 蝪∪?撖衣
        return True
    
    def calculate_network_dynamics(self, network_id):
        """閮?蝬脩窗??"""
        dynamics = {
            "ChangeFrequency": 0.1,
            "InfluenceStrength": 50.0,
            "StabilityIndex": 0.6,
            "ResilienceIndex": 0.7
        }
        
        return dynamics
    
    def simulate_relationship_development(self, network_id, simulation_steps):
        """璅⊥???澆?"""
        # 蝪∪?撖衣
        pass
    
    def _get_change_frequency(self, source_id, target_id):
        """?脣?霈??餌?"""
        history = self.get_relationship_change_history(source_id, target_id)
        
        if len(history) < 2:
            return 0.0
        
        first_time = history[0]["change_time"]
        last_time = history[-1]["change_time"]
        time_span = last_time - first_time
        hours = time_span.total_seconds() / 3600.0
        
        if hours > 0:
            return len(history) / hours
        
        return 0.0


def run_relationship_network_tests():
    """????蝬脩窗皜祈岫"""
    print("???? MingGoRTS ??蝬脩窗皜祈岫...")
    print("=" * 60)
    
    # ?萄遣皜祈岫憟辣
    suite = unittest.TestLoader().loadTestsFromTestCase(TestRelationshipNetwork)
    
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
    success = run_relationship_network_tests()
    sys.exit(0 if success else 1)

