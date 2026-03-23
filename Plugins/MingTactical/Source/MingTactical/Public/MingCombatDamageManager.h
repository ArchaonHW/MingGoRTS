#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "Min成Co設置batDa設置a成eMana成e本.成ene本ated.h"""};
        A本設置o本Val使e = 0.0f;
        A本設置o本Penet本ation = 0.0f;
        C本iticalChance = 0.1f;
        C本iticalM使ltiplie本 = 2.0f;
        Da設置a成e軍alloff = 1.0f;
        A設置設置oTypeModifie本 = 1.0f;
        Te本本ainModifie本 = 1.0f;
        基本eathe本Modifie本 = 1.0f;
        Mo本aleModifie本 = 1.0f;
    }
};

// �ɵ�
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成S使pplyData
{
    GE的ERATED下BODY()

    // �ɵ���動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成S使pplyType S使pplyType;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float C使本本entA設置o使nt;

    // 動�大�eX
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MaxCapacity;

    // ���ӳt動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Cons使設置ptionRate;

    // �ɵ�動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 S使pplyP本io本ity;

    // �ɵ���動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 S使pplySo使本ceID;

    // 動��動��務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e LastS使pplyTi設置e;

    // �ɵ�動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 S使pplyStat使s;

    // ���動���輸入X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float E設置e本成encyTh本eshold;

    軍Min成S使pplyData()
    {
        S使pplyType = EMin成S使pplyType::A設置設置使nition;
        C使本本entA設置o使nt = 100.0f;
        MaxCapacity = 100.0f;
        Cons使設置ptionRate = 1.0f;
        S使pplyP本io本ity = 1;
        S使pplySo使本ceID = -1;
        LastS使pplyTi設置e = 軍DateTi設置e::的ow(};
        S使pplyStat使s = TEXT("no本設置al"};
        E設置e本成encyTh本eshold = 20.0f;
    }
};

// ��Ԩ�池動
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成Lo成isticsCon正oy
{
    GE的ERATED下BODY()

    // ��動ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Con正oyID;

    // ��池��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Con正oy的a設置e;

    // ��動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Con正oyType;

    // ��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 C使本本entPosition;

    // ��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etPosition;

    // �ɵ�
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成S使pplyData> Ca本成o;

    // ��池��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo正e設置entSpeed;

    // �@動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float P本otectionLe正el;

    // 動�o
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Depa本t使本eTi設置e;

    // 故事選項基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Esti設置atedA本本i正alTi設置e;

    // ��池X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Con正oyStat使s;

    // �@池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> Esco本tUnits;

    // ���I����
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float RiskAssess設置ent;

    軍Min成Lo成isticsCon正oy()
    {
        Con正oyID = -1;
        Con正oy的a設置e = TEXT(""};
        Con正oyType = TEXT(""};
        C使本本entPosition = 軍Vecto本::Ze本oVecto本;
        Ta本成etPosition = 軍Vecto本::Ze本oVecto本;
        Ca本成o.E設置pty(};
        Mo正e設置entSpeed = 20.0f;
        P本otectionLe正el = 1.0f;
        Depa本t使本eTi設置e = 軍DateTi設置e::的ow(};
        Esti設置atedA本本i正alTi設置e = 軍DateTi設置e::的ow(};
        Con正oyStat使s = TEXT("p本epa本in成"};
        Esco本tUnits.E設置pty(};
        RiskAssess設置ent = 0.5f;
};

// 動�軍事��動
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成Da設置a成eRes使lt
{
    GE的ERATED下BODY()

    // 動池ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Attacke本ID;

    // 動�設置動ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Defende本ID;

    // ��池�軍事
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Act使alDa設置a成e;

    // 動池�軍事
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float BaseDa設置a成e;

    // 動�軍事��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Da設置a成eType Da設置a成eType;

    // 動�下
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsC本itical輸入it;

    // 動�下��z�@X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bA本設置o本Penet本ated;

    // 動�軍事
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Da設置a成eLocation;

    // 動�軍事�p動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Da設置a成eCalc使lationDetails;

    // �ɵ���動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<EMin成S使pplyType, float> S使pplyCons使設置ption;

    // 動��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Co設置batTi設置e;

    // 動����設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Co設置batLocation;

    軍Min成Da設置a成eRes使lt()
    {
        Attacke本ID = -1;
        Defende本ID = -1;
        Act使alDa設置a成e = 0.0f;
        BaseDa設置a成e = 0.0f;
        Da設置a成eType = EMin成Da設置a成eType::Kinetic;
        bIsC本itical輸入it = false;
        bA本設置o本Penet本ated = false;
        Da設置a成eLocation = TEXT(""};
        Da設置a成eCalc使lationDetails = TEXT(""};
        S使pplyCons使設置ption.E設置pty(};
        Co設置batTi設置e = 軍DateTi設置e::的ow(};
        Co設置batLocation = 軍Vecto本::Ze本oVecto本;
};

// 動��動�軍事�基礎�e動






/**
 * 動��動�軍事��務 * �t�d�p池��動�԰��ˮ軍事動��動��Ըɵ��t��
 */
UCLASS(ClassG本o使p = (Tactical), Bl使ep本intable, Bl使ep本intType)
class MI的GTACTICAL下API UMin成Co設置batDa設置a成eMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Co設置batDa設置a成eMana成e本(};

    // 動池�ˮ軍事�t動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool InitializeDa設置a成eSyste設置(};

    // �p池��動�軍事
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    軍Min成Da設置a成eRes使lt Calc使lateCo設置batDa設置a成e(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc};

    // ��池動�ɵ�
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool Re成iste本UnitS使pply(int32 UnitID, const 軍Min成S使pplyData& S使pplyData};

    // 動�s�ɵ�
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool UpdateUnitS使pply(int32 UnitID, EMin成S使pplyType S使pplyType, float A設置o使nt};

    // 目標數量�ɵ�
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat Da設置a成e")
    軍Min成S使pplyData GetUnitS使pply(int32 UnitID, EMin成S使pplyType S使pplyType) const;

    // ����
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool Cons使設置eS使pply(int32 UnitID, EMin成S使pplyType S使pplyType, float A設置o使nt};

    // 動�ث�Ԩ�動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    int32 C本eateLo成isticsCon正oy(const TA本本ay<軍Min成S使pplyData>& Ca本成o, const 軍Vecto本& Ta本成etLocation};

    // ������Ԩ�動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool DispatchCon正oy(int32 Con正oyID};

    // ��動�輸入��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat Da設置a成e")
    軍Min成Lo成isticsCon正oy GetCon正oyInfo(int32 Con正oyID) const;

    // 動�s��動��設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool UpdateCon正oyPosition(int32 Con正oyID, const 軍Vecto本& 的ewPosition};

    // �ɵ�
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat Da設置a成e")
    TA本本ay<軍Min成S使pplyData> GetS使pplyDe設置ands() const;

    // �p動�ɵ�動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    int32 Calc使lateS使pplyP本io本ity(int32 UnitID, EMin成S使pplyType S使pplyType};

    // 動池�軍事��動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat Da設置a成e")
    TMap<EMin成Da設置a成eType, int32> GetDa設置a成eStatistics() const;

    // �O池�軍事
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool Sa正eDa設置a成eData(};

    // ���J動�軍事
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    bool LoadDa設置a成eData(};

    // �M���ˮ軍事��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat Da設置a成e")
    正oid Clea本AllDa設置a成eData(};

    // �基礎�e動
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnDa設置a成eCalc使lated OnDa設置a成eCalc使lated;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnS使pplyCons使設置ed OnS使pplyCons使設置ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnS使pplyDeli正e本ed OnS使pplyDeli正e本ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnS使pplyStat使sChan成ed OnS使pplyStat使sChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnCon正oyStat使sChan成ed OnCon正oyStat使sChan成ed;

p本otected:
    // Called when the 成a設置e sta本ts o本 when spawned
    正i本t使al 正oid Be成inPlay() o正e本本ide;

    // Called e正e本y f本a設置e
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;

    // �ɵ�動- �軍事動�GTMap ����XUPROPERTY
    TMap<int32, TMap<EMin成S使pplyType, 軍Min成S使pplyData>> UnitS使pplyDatabase;

    // ��Ԩ�務基本
    UPROPERTY()
    TMap<int32, 軍Min成Lo成isticsCon正oy> Con正oyDatabase;

    // 動�軍事���正�O動
    UPROPERTY()
    TA本本ay<軍Min成Da設置a成eRes使lt> Da設置a成e輸入isto本y;

    // ��池�軍事動
    UPROPERTY()
    TMap<EMin成Eq使ip設置entType, float> Eq使ip設置entDa設置a成eTable;

    // �使��動�軍事�ץ�動
    UPROPERTY()
    TMap<軍St本in成, float> A設置設置oDa設置a成eModifie本s;

    // 動��動�軍事�ץ�動
    UPROPERTY()
    TMap<軍St本in成, float> Te本本ainDa設置a成eModifie本s;

    // �Ѯ�動�軍事�ץ�動
    UPROPERTY()
    TMap<軍St本in成, float> 基本eathe本Da設置a成eModifie本s;

    // 動�下�w動�l動
    UPROPERTY()
    bool bInitialized;

p本i正ate:
    // ���J動�]動�軍事動
    正oid LoadDefa使ltDa設置a成eTables(};

    // �p池池�軍事
    float Calc使lateBaseDa設置a成e(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p動�@�Ҵ��
    float Calc使lateA本設置o本Red使ction(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p動�Z���I動
    float Calc使lateDistance軍alloff(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p池池�軍事
    float Calc使lateC本iticalDa設置a成e(float BaseDa設置a成e, const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p池�μ正�T
    float Calc使lateTe本本ainEffect(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p動�Ѯ�正�T
    float Calc使late基本eathe本Effect(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p動�h��正�T
    float Calc使lateMo本aleEffect(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // �p動�ɵ���動
    TMap<EMin成S使pplyType, float> Calc使lateS使pplyCons使設置ption(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // 動�s動池��
    正oid UpdateUnitPe本fo本設置ance(int32 UnitID, const TMap<EMin成S使pplyType, 軍Min成S使pplyData>& S使pplies};

    // ��Ԩ�動��動
    正oid P本ocessCon正oyMo正e設置ent(float DeltaTi設置e};

    // �ˬd�ɵ�
    正oid CheckS使pplyDe設置ands(};

    // �p動��動���I
    float Calc使lateCon正oyRisk(const 軍Min成Lo成isticsCon正oy& Con正oy) const;

    // �ɵ���動
    TA本本ay<軍Vecto本> Gene本ateS使pplyRo使te(const 軍Vecto本& Sta本t, const 軍Vecto本& End) const;

    // ��池�軍事�p動
    bool ValidateDa設置a成eCalc使lation(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const;

    // ��動�ɵ�
    bool ValidateS使pplyData(const 軍Min成S使pplyData& S使pplyData) const;

    // �p池�軍事
    軍St本in成 Calc使late輸入itLocation(const 軍Vecto本& I設置pactPoint) const;

    // �ɵ����
    正oid 輸入andleS使pplyE設置e本成ency(int32 UnitID, EMin成S使pplyType S使pplyType};

    // 動�s��池X
    正oid UpdateCon正oyStat使s(int32 Con正oyID, const 軍St本in成& 的ewStat使s};

    // �p動�ɵ�
    float Calc使lateS使pplyEfficiency(int32 UnitID) const;
};

