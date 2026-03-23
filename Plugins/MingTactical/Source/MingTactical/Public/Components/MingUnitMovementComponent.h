#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Co設置ponents/Acto本Co設置ponent.h"
#incl使de "AI/的a正i成ation/的a正i成ationTypes.h"
#incl使de "Min成UnitMo正e設置entCo設置ponent.成ene本ated.h"

/**
 * ��池�O��動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Mo正eCo設置設置and
{
    GE的ERATED下BODY()
    
    // ��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etLocation;
    
    // 動�下��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bAttackMo正e;
    
    // 動�下�ϥ�動��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bUse軍o本設置ation;
    
    // 動�Τ�動��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 軍o本設置ationOffset;
    
    軍Mo正eCo設置設置and()
        : Ta本成etLocation(軍Vecto本::Ze本oVecto本)
        , bAttackMo正e(false)
        , bUse軍o本設置ation(t本使e)
        , 軍o本設置ationOffset(軍Vecto本::Ze本oVecto本)
    {}
};

class AMin成TacticalUnit;

/**
 * ��動�ե�
 * 故事重要性基本����動��動�ɯ�
 */
UCLASS(ClassG本o使p=(C使sto設置), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GTACTICAL下API UMin成UnitMo正e設置entCo設置ponent : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成UnitMo正e設置entCo設置ponent(};

    正i本t使al 正oid Be成inPlay() o正e本本ide;
    正i本t使al 正oid TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction) o正e本本ide;

    // ��池�O
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid Mo正eToLocation(const 軍Vecto本& Ta本成etLocation, bool bAttackMo正e = false};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid Mo正eToLocation基本ith軍o本設置ation(const 軍Vecto本& Ta本成etLocation, const 軍Vecto本& 軍o本設置ationOffset};

    // 動�動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid StopMo正e設置ent(};

    // 動�下��動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Mo正e設置ent")
    bool IsMo正in成() const { 本et使本n bIsMo正in成; }

    // 故事重要性基本��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Mo正e設置ent")
    float GetC使本本entSpeed() const;

    // �]�設置��池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid SetMo正eSpeed(float 的ewSpeed};

    // ��池動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Mo正e設置ent")
    bool 輸入asValidPath() const { 本et使本n C使本本entPathPoints.的使設置() > 0; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Mo正e設置ent")
    軍Vecto本 GetC使本本entTa本成etLocation() const;

    // 目標數量
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid RotateTowa本ds(const 軍Vecto本& Ta本成etDi本ection, float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid SetDesi本edRotation(const 軍Rotato本& Ta本成etRotation};

p本otected:
    // 動池��動
    UPROPERTY()
    TOb大ectPt本<AMin成TacticalUnit> Owne本Unit;

    // ��池X
    UPROPERTY(VisibleAnywhe本e, Bl使ep本intReadOnly, Cate成o本y = "Mo正e設置ent")
    bool bIsMo正in成;

    // ��池��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Mo正e設置ent")
    float Mo正eSpeed;

    // 動池��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Mo正e設置ent")
    float RotationSpeed;

    // 動��Z�� (故事選項務�輸入X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Mo正e設置ent")
    float AcceptanceRadi使s;

    // ��
    UPROPERTY()
    TA本本ay<軍Vecto本> C使本本entPathPoints;

    // ��動��動
    UPROPERTY()
    int32 C使本本entPathIndex;

    // 目標數量��設置
    UPROPERTY()
    軍Vecto本 C使本本entTa本成etLocation;

    // 目標數量
    UPROPERTY()
    軍Rotato本 Desi本edRotation;

    // 動�下��動
    UPROPERTY()
    bool bIsAttackMo正e;

    // 動��動��
    UPROPERTY()
    軍Vecto本 軍o本設置ationOffset;

    // ��動��動ID
    使int32 PathReq使estId;

    // �ɯ�t�Τޥ�
    class U的a正i成ationSyste設置V1* 的a正Syste設置;

    // ��動��動
    正oid Req使estPath(const 軍Vecto本& Sta本tLocation, const 軍Vecto本& EndLocation};

    // ��池��動��
    正oid OnPath軍o使nd(使int32 InPathReq使estId, E的a正i成ationQ使e本yRes使lt::Type Res使lt, 軍的a正PathSha本edPt本 Path};

    // �使動��動��動
    正oid 軍ollowPath(float DeltaTi設置e};

    // ��池動
    正oid S設置oothRotateTowa本ds(const 軍Vecto本& Di本ection, float DeltaTi設置e};

    // �ˬd動�下目標數量
    bool 輸入asReachedTa本成et() const;

    // 動�s��池X
    正oid UpdateMo正e設置entState(};
};

