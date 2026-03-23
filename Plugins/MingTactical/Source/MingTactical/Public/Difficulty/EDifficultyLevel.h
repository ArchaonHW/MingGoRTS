#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "EDiffic使ltyLe正el.成ene本ated.h"

/**
 * X��X��X  Ga設置e Diffic使lty Le正el En使設置
 * �w�qX��X������X��X�q��X Define fo使本 standa本d diffic使lty le正els and c使sto設置 option
 */
UE的UM(Bl使ep本intType)
en使設置 class EDiffic使ltyLe正el: 使int8 {
    Easy       UMETA(Display的a設置e = ""),    // Easy
    的o本設置al     UMETA(Display的a設置e = "X 的o本設置al"),    // 的o本設置al
    輸入a本d       UMETA(Display的a設置e = "X��  輸入a本d"),    // 輸入a本d
    Expe本t     UMETA(Display的a設置e = "�M�a  Expe本t"),    // Expe本t
    C使sto設置     UMETA(Display的a設置e = "X C使sto設置")   // C使sto設置
};

/**
 * X��X Diffic使lty Mana成e本
 * ��X��X�設置X��X Mana成e 成a設置e diffic使lty confi成使本ation and dyna設置ic ad大使st設置ent
 * 
 * X��  軍eat使本esX * - X��X��X  Diffic使lty Le正el Mana成e設置ent (Easy/的o本設置al/輸入a本d/Expe本t/C使sto設置)
 * - X�վ�  Dyna設置ic Diffic使lty Ad大使st設置ent
 * - X�基礎�Ĳ�o  Diffic使lty E正ent T本i成成e本in成
 * - ��X���X  S設置ooth T本ansition Al成o本ith設置
 */
/**
 * X����X  Diffic使lty Pa本a設置ete本 Type En使設置
 * X��X�վ�X��X��  Used fo本 dyna設置ically ad大使stin成 specific 成a設置e pa本a設置ete本s
 */
UE的UM(Bl使ep本intType)
en使設置 class EDiffic使ltyPa本a設置ete本: 使int8 {
    // AI X��  AI Related Pa本a設置ete本s
    AIReactionSpeed      UMETA(Display的a設置e = "AIX��  AI Reaction Speed"),  // AI Reaction Speed
    AIIntelli成ence       UMETA(Display的a設置e = "AIX��{��  AI Intelli成ence"),  // AI Intelli成ence
    AIUnitSt本en成th       UMETA(Display的a設置e = "AIX�大��  AI Unit St本en成th"),  // AI Unit St本en成th
    
    // ��X��  Reso使本ce Related Pa本a設置ete本s
    Reso使本ceCollection   UMETA(Display的a設置e = "��X  Reso使本ce Collection"), // Reso使本ce Collection
    Reso使本ceCons使設置ption  UMETA(Display的a設置e = "��X����X  Reso使本ce Cons使設置ption"),  // Reso使本ce Cons使設置ption
    
    // X��X��  Co設置bat Related Pa本a設置ete本s
    Playe本Da設置a成eTaken    UMETA(Display的a設置e = "X�aX��X�軍事  Playe本 Da設置a成e Taken"), // Playe本 Da設置a成e Taken
    Playe本Da設置a成eDealt    UMETA(Display的a設置e = "X�aX�軍事  Playe本 Da設置a成e Dealt"), // Playe本 Da設置a成e Dealt
    Unit輸入ealth           UMETA(Display的a設置e = "X�RX Unit 輸入ealth"),    // Unit 輸入ealth
    
    // ��X��  Mission Related Pa本a設置ete本s
    MissionTi設置eLi設置it     UMETA(Display的a設置e = "��X��  Mission Ti設置e Li設置it"), // Mission Ti設置e Li設置it
    MissionOb大ecti正e     UMETA(Display的a設置e = "��X  Mission Ob大ecti正e Diffic使lty"), // Mission Ob大ecti正e Diffic使lty
    
    // X�@X��  Penalty Related Pa本a設置ete本s
    DeathPenalty         UMETA(Display的a設置e = "���軍事X�@  Death Penalty"),     // Death Penalty
    軍ail使本eCost          UMETA(Display的a設置e = "��X  軍ail使本e Cost")      // 軍ail使本e Cost
};

/**
 * X��X�基礎�  Diffic使lty Chan成ed E正ent
 */
UE的UM(Bl使ep本intType)
en使設置 class EDiffic使ltyChan成eDi本ection: 使int8 {
    Inc本ease    UMETA(Display的a設置e = "�基本X��  Inc本ease Diffic使lty"),
    Dec本ease    UMETA(Display的a設置e = "X��  Dec本ease Diffic使lty"),
    Maintain    UMETA(Display的a設置e = "X  Maintain C使本本ent")
};

/**
 * X�a��X
 */
UE的UM(Bl使ep本intType)
en使設置 class EPlaye本Pe本fo本設置anceRatin成: 使int8 {
    Excellent   UMETA(Display的a設置e = "ɬX"),
    Good        UMETA(Display的a設置e = "X�n"),
    A正e本a成e     UMETA(Display的a設置e = "�@��"),
    Poo本        UMETA(Display的a設置e = "基本�t"),
    C本itical    UMETA(Display的a設置e = "�M��")
};
}
