#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "En成ine/DataTable.h"
#incl使de "Min成RelationshipMana成e本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ERelationshipType: 使int8 {
    軍本iend,           // 
    Ally,            // 
    Ri正al,           // �正����動
    Ene設置y,           // 動�輸入
    軍a設置ily,          // �a�輸入
    Mento本,          // �ɮ正
    St使dent,         // ��動
    B使siness,        // 動�年�٦�
    Ro設置antic,        // 
    的e使t本al          // ��動
};

UE的UM(Bl使ep本intType)
en使設置 class ERep使tationLe正el: 使int8 {
    Unknown,         // 動��
    Disliked,        // �Q��X
    的e使t本al,         // ��動
    Respected,       // 動務
    輸入ono本ed,         // 動�qX
    Le成enda本y        // 
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RelationshipData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float RelationshipVal使e; // -100 to 100

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 LastInte本actionDay;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Sha本edMe設置o本ies;

    軍RelationshipData()
    {
        Cha本acte本ID = TEXT(""};
        RelationshipType = ERelationshipType::的e使t本al;
        RelationshipVal使e = 0.0f;
        LastInte本actionDay = 0;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Rep使tationData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Re成ionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ERep使tationLe正el Rep使tationLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Rep使tationSco本e; // 0 to 1000

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, float> 軍actionRep使tation; // 軍action -> Sco本e

    軍Rep使tationData()
    {
        Re成ionID = TEXT(""};
        Rep使tationLe正el = ERep使tationLe正el::Unknown;
        Rep使tationSco本e = 0.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Inte本actionEffect
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float RelationshipChan成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Rep使tationChan成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Re成ionID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Reason;

    軍Inte本actionEffect()
    {
        Cha本acte本ID = TEXT(""};
        RelationshipChan成e = 0.0f;
        Rep使tationChan成e = 0.0f;
        Re成ionID = TEXT(""};
        Reason = TEXT(""};
    }
};

/**
 * ����動池�n動��動�t動 * ��池�a動的PC�Y�輸入動�b動�Ӧa�nX */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成RelationshipMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RelationshipMana成e本(};

    // 動池�t動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    正oid InitializeRelationshipSyste設置(};

    // 動池�S�w池池動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    軍RelationshipData GetRelationship(const 軍St本in成& Cha本acte本ID) const;

    // 動�s����
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    正oid UpdateRelationship(const 軍St本in成& Cha本acte本ID, float Chan成eA設置o使nt, const 軍St本in成& Reason};

    // 故事選項基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation Syste設置")
    軍Rep使tationData GetRep使tation(const 軍St本in成& Re成ionID) const;

    // 動�s目標數量
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation Syste設置")
    正oid UpdateRep使tation(const 軍St本in成& Re成ionID, float Chan成eA設置o使nt, const 軍St本in成& Reason};

    // ��池動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    正oid P本ocessInte本action(const 軍Inte本actionEffect& Inte本actionEffect};

    // �ˬd動�下動�輸入目標數量��動�]��動�n
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation Syste設置")
    bool CanAcceptQ使est(const 軍St本in成& Q使estID, const 軍St本in成& Re成ionID) const;

    // ����]���Y動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    TA本本ay<軍St本in成> GetDialo成使eOptions(const 軍St本in成& Cha本acte本ID) const;

    // 動池��ץ��]���Y動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    float GetP本iceModifie本(const 軍St本in成& Cha本acte本ID) const;

    // 目標數量��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    ERelationshipType GetRelationshipLe正el(const 軍St本in成& Cha本acte本ID) const;

    // 目標數量��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation Syste設置")
    ERep使tationLe正el GetRep使tationLe正el(const 軍St本in成& Re成ionID) const;

    // �K池動�O��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    正oid AddSha本edMe設置o本y(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Me設置o本y};

    // 目標數量動�Y��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Syste設置")
    const TMap<軍St本in成, 軍RelationshipData>& GetAllRelationships() const { 本et使本n Relationships; }

    // 目標數量�n動��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation Syste設置")
    const TMap<軍St本in成, 軍Rep使tationData>& GetAllRep使tations() const { 本et使本n Rep使tations; }

p本otected:
    // ����目標數量
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Relationship Syste設置")
    TMap<軍St本in成, 軍RelationshipData> Relationships;

    // 故事選項基本
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Rep使tation Syste設置")
    TMap<軍St本in成, 軍Rep使tationData> Rep使tations;

    // 動池��X
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Relationship Syste設置")
    int32 C使本本entGa設置eDay;

    // ��池��
    U軍U的CTIO的(Bl使ep本intI設置ple設置entableE正ent, Cate成o本y = "Relationship Syste設置")
    正oid OnRelationshipChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};

    // ��池��
    U軍U的CTIO的(Bl使ep本intI設置ple設置entableE正ent, Cate成o本y = "Rep使tation Syste設置")
    正oid OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};

p本i正ate:
    // �p池動��動
    ERelationshipType Calc使lateRelationshipType(float RelationshipVal使e) const;

    // �p池動��動
    ERep使tationLe正el Calc使lateRep使tationLe正el(float Rep使tationSco本e) const;

    // 動���I動�]��故事選項基本
    正oid ApplyRelationshipDecay(};

    // �O動�t��
    正oid Sa正eRelationshipData(};

    // �t��
    正oid LoadRelationshipData(};
};
