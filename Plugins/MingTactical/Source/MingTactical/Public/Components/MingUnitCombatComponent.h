#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Co設置ponents/Acto本Co設置ponent.h"
#incl使de "Min成UnitCo設置batCo設置ponent.成ene本ated.h"

class AMin成TacticalUnit;

/**
 * 動池�O��動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍AttackCo設置設置and
{
    GE的ERATED下BODY()
    
    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    T基本eakOb大ectPt本<AMin成TacticalUnit> Ta本成etUnit;
    
    // ��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etLocation;
    
    // 動�下�設置��X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsLocationAttack;
    
    軍AttackCo設置設置and()
        : Ta本成etUnit(n使llpt本)
        , Ta本成etLocation(軍Vecto本::Ze本oVecto本)
        , bIsLocationAttack(false)
    {}
};

/**
 * 動池���ե�
 * 故事重要性基本��動�B���設置池�軍事動��
 */
UCLASS(ClassG本o使p=(C使sto設置), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GTACTICAL下API UMin成UnitCo設置batCo設置ponent : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成UnitCo設置batCo設置ponent(};

    正i本t使al 正oid Be成inPlay() o正e本本ide;
    正i本t使al 正oid TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction) o正e本本ide;

    // 動池�O
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid AttackTa本成et(AMin成TacticalUnit* Ta本成et};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid AttackLocation(const 軍Vecto本& Ta本成etLocation};

    // 動��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid StopAttack(};

    // 動�下動��動池��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat")
    bool IsTa本成etInRan成e(AMin成TacticalUnit* Ta本成et) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat")
    bool IsLocationInRan成e(const 軍Vecto本& Location) const;

    // 故事選項基本
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat")
    AMin成TacticalUnit* GetC使本本entTa本成et() const { 本et使本n C使本本entTa本成et.Get(); }

    // 動�下動�輸入
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Co設置bat")
    bool CanAttack() const;

    // �]�設置動池�o
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid SetAttackCooldown(float Cooldown};

p本otected:
    // 動池��動
    UPROPERTY()
    TOb大ectPt本<AMin成TacticalUnit> Owne本Unit;

    // 故事選項基本
    UPROPERTY()
    T基本eakOb大ectPt本<AMin成TacticalUnit> C使本本entTa本成et;

    // ��設置
    UPROPERTY()
    軍Vecto本 Ta本成etAttackLocation;

    // 動�下動��動
    UPROPERTY()
    bool bIsAttackin成Location;

    // 動�下動��X
    UPROPERTY(VisibleAnywhe本e, Bl使ep本intReadOnly, Cate成o本y = "Co設置bat")
    bool bIsAttackin成;

    // 動池�o
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat")
    float AttackCooldown;

    // 動池�o
    UPROPERTY()
    float C使本本entCooldown;

    // �d動
    UPROPERTY()
    float AttackRan成e;

    // 動池�軍事
    UPROPERTY()
    float AttackDa設置a成e;

    // 目標數量
    正oid Pe本fo本設置Attack(};

    // ��ؼгy池�軍事
    正oid DealDa設置a成eToTa本成et(AMin成TacticalUnit* Ta本成et};

    // �M��d池池�輸入
    AMin成TacticalUnit* 軍indEne設置yInRan成e(};

    // 動池��動
    正oid RotateToTa本成et(float DeltaTi設置e};

    // �ˬd動池�下
    bool IsValidTa本成et(AMin成TacticalUnit* Ta本成et) const;

    // 動池�eĲ�o
    正oid T本i成成e本AttackAni設置ation(};
};

