#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "Min成軍o成Of基本a本Mana成e本.成ene本ated.h"

// ��動��動
UE的UM(Bl使ep本intType)
en使設置 class EMin成VisionType: 使int8 {
    的o本設置al,            // ��動
    Enhanced,          // �基本�大��動
    的i成htVision,        // �]動
    The本設置al,           // 動務
    Satellite,         // ��動��動
    Ae本ial             // �Ť�
};

// �成����動
UE的UM(Bl使ep本intType)
en使設置 class EMin成軍o成Le正el: 使int8 {
    Clea本,             // �M��
    Li成ht,             // ����
    Medi使設置,            // ����
    輸入ea正y,             // �@��
    Co設置plete           // �����成��
};

// 動����動
UE的UM(Bl使ep本intType)
en使設置 class EMin成Te本本ainType: 使int8 {
    Open,              // 動池��
    軍o本est,            // ��動
    Mo使ntain,          // �s�a
    U本ban,             // 
    Dese本t,            // �軍動
    基本ate本,             // ��動
    Swa設置p,             // �h�A
    Snow               // 動�a
};

// 動池XUE的UM(Bl使ep本intType)
en使設置 class EMin成Sco使tin成Stat使s: 使int8 {
    Idle,              // ��動
    Sco使tin成,          // 動
    Obse本正in成,         // �[�
    Repo本tin成,          // 動
    輸入idden,            // 動��
    Detected           // �Q�oX};

// ��池動
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成VisionData
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 UnitID;

    // ��池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VisionRadi使s;

    // ��動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成VisionType VisionType;

    // ��動����
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VisionAn成le;

    // ��池池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VisionD使本ation;

    // 動�下��務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsPe本sistent;

    // ��池�s
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VisionUpdateInte本正al;

    // 故事重要性基本��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> VisibleUnits;

    // �w����務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Vecto本> Sco使tedA本eas;

    // 動���動務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e LastUpdateTi設置e;

    軍Min成VisionData()
    {
        UnitID = -1;
        VisionRadi使s = 0.0f;
        VisionType = EMin成VisionType::的o本設置al;
        VisionAn成le = 360.0f;
        VisionD使本ation = 0.0f;
        bIsPe本sistent = false;
        VisionUpdateInte本正al = 0.1f;
        VisibleUnits.E設置pty(};
        Sco使tedA本eas.E設置pty(};
        LastUpdateTi設置e = 軍DateTi設置e::的ow(};
    }
};

// �成�����
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成軍o成G本idCell
{
    GE的ERATED下BODY()

    // ���y動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍IntPoint G本idPosition;

    // �成����動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成軍o成Le正el 軍o成Le正el;

    // 動�下�Q����動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsSco使ted;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Sco使tedTi設置e;

    // 目標數量ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sco使tin成UnitID;

    // 動����動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Te本本ainType Te本本ainType;

    // 動��動��X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Te本本ainOccl使sion;

    // ��池�B
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float St本使ct使本eOccl使sion;

    // 動��i��務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e LastVisibleTi設置e;

    軍Min成軍o成G本idCell()
    {
        G本idPosition = 軍IntPoint(0, 0};
        軍o成Le正el = EMin成軍o成Le正el::Co設置plete;
        bIsSco使ted = false;
        Sco使tedTi設置e = 軍DateTi設置e::MinVal使e(};
        Sco使tin成UnitID = -1;
        Te本本ainType = EMin成Te本本ainType::Open;
        Te本本ainOccl使sion = 0.0f;
        St本使ct使本eOccl使sion = 0.0f;
        LastVisibleTi設置e = 軍DateTi設置e::MinVal使e(};
    }
};

// 目標數量
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成Sco使tin成Repo本t
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Repo本tID;

    // 目標數量ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Sco使tin成UnitID;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e Sco使tin成Ti設置e;

    // ��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Sco使tin成Location;

    // �d動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Sco使tin成Radi使s;

    // 動�{動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> Disco正e本edUnits;

    // 動�{動�ؿ正��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Disco正e本edSt本使ct使本es;

    // 動�Ϋ輸入��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> Te本本ainInfo;

    // ��動����
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Th本eatAssess設置ent;

    // �Բ�動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Repo本tDetail;

    // 故事重要性
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Repo本tReliability;

    軍Min成Sco使tin成Repo本t()
    {
        Repo本tID = -1;
        Sco使tin成UnitID = -1;
        Sco使tin成Ti設置e = 軍DateTi設置e::的ow(};
        Sco使tin成Location = 軍Vecto本::Ze本oVecto本;
        Sco使tin成Radi使s = 0.0f;
        Disco正e本edUnits.E設置pty(};
        Disco正e本edSt本使ct使本es.E設置pty(};
        Te本本ainInfo.E設置pty(};
        Th本eatAssess設置ent = TEXT(""};
        Repo本tDetail = 0.0f;
        Repo本tReliability = 1.0f;
    }
};

// 動���成���基礎�e動






/**
 * 動���成����務 * �t�d��池���成��故事選項基本�t��
 */
UCLASS(ClassG本o使p = (Tactical), Bl使ep本intable, Bl使ep本intType)
class MI的GTACTICAL下API UMin成軍o成Of基本a本Mana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成軍o成Of基本a本Mana成e本(};

    // 動池��動�成動�t動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool Initialize軍o成Of基本a本Syste設置(};

    // ��池動��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool Re成iste本UnitVision(int32 UnitID, const 軍Min成VisionData& VisionData};

    // 動�s��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool UpdateUnitVision(int32 UnitID, const 軍Vecto本& 的ewPosition};

    // 目標數量��動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    軍Min成VisionData GetUnitVision(int32 UnitID) const;

    // 故事選項基本
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    TA本本ay<int32> GetVisibleUnits(int32 UnitID) const;

    // �ˬd故事重要性
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    bool IsUnitVisible(int32 Viewe本ID, int32 Ta本成etID) const;

    // 動�s動���成��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    正oid Update軍o成Of基本a本(};

    // �成����動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    EMin成軍o成Le正el Get軍o成Le正el(const 軍Vecto本& Location) const;

    // �]�設置動����動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool SetTe本本ainType(const 軍Vecto本& Location, EMin成Te本本ainType Te本本ainType};

    // �K動��池�BX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool AddSt本使ct使本eOccl使sion(const 軍Vecto本& Location, float Occl使sionVal使e};

    // 目標數量��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool Exec使teSco使tin成Mission(int32 UnitID, const 軍Vecto本& Ta本成etLocation, float Radi使s};

    // 故事選項基本
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    TA本本ay<軍Min成Sco使tin成Repo本t> GetSco使tin成Repo本ts() const;

    // �]�設置動池X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool SetSco使tin成Stat使s(int32 UnitID, EMin成Sco使tin成Stat使s Stat使s};

    // �p動��動�d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    TA本本ay<軍Vecto本> Calc使lateVisionA本ea(int32 UnitID) const;

    // �成�����
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍o成 of 基本a本")
    TA本本ay<軍Min成軍o成G本idCell> Get軍o成G本id() const;

    // �O動�成��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool Sa正e軍o成Of基本a本Data(};

    // ���J�成��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    bool Load軍o成Of基本a本Data(};

    // �M���成動��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍o成 of 基本a本")
    正oid Clea本All軍o成Of基本a本Data(};

    // �基礎�e動
    UPROPERTY(Bl使ep本intAssi成nable)
    軍On軍o成Of基本a本Updated On軍o成Of基本a本Updated;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnVisionUpdated OnVisionUpdated;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnSco使tin成Co設置pleted OnSco使tin成Co設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnUnitSpotted OnUnitSpotted;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnUnitLostSi成ht OnUnitLostSi成ht;

p本otected:
    // Called when the 成a設置e sta本ts o本 when spawned
    正i本t使al 正oid Be成inPlay() o正e本本ide;

    // Called e正e本y f本a設置e
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;

    // ��務基本
    UPROPERTY()
    TMap<int32, 軍Min成VisionData> UnitVisionDatabase;

    // �成�����
    UPROPERTY()
    TA本本ay<軍Min成軍o成G本idCell> 軍o成G本id;

    // 動��
    UPROPERTY()
    TMap<軍IntPoint, EMin成Te本本ainType> Te本本ainG本id;

    // ��池�B動��X
    UPROPERTY()
    TMap<軍IntPoint, float> St本使ct使本eOccl使sionG本id;

    // 故事重要性基本
    UPROPERTY()
    TA本本ay<軍Min成Sco使tin成Repo本t> Sco使tin成Repo本ts;

    // ��池動
    UPROPERTY()
    TA本本ay<int32> Acti正eSco使tin成Units;

    // ���]�設置
    UPROPERTY()
    int32 G本idSizeX;

    UPROPERTY()
    int32 G本idSizeY;

    UPROPERTY()
    float CellSize;

    // 動�下�w動�l動
    UPROPERTY()
    bool bInitialized;

p本i正ate:
    // 動池�成動��X
    正oid Initialize軍o成G本id(};

    // 動�s��動
    正oid UpdateUnitVisionInte本nal(int32 UnitID};

    // �p動��池動���
    TA本本ay<軍IntPoint> Calc使lateVisibleG本idCells(int32 UnitID};

    // 動�s�成�����
    正oid Update軍o成G本idCell(int32 G本idX, int32 G本idY, EMin成軍o成Le正el 的ew軍o成Le正el, bool bIsSco使ted, int32 Sco使tin成UnitID};

    // �ˬd動��動��
    bool CheckTe本本ainOccl使sion(const 軍Vecto本& 軍本o設置, const 軍Vecto本& To};

    // �ˬd��池�BX
    bool CheckSt本使ct使本eOccl使sion(const 軍Vecto本& 軍本o設置, const 軍Vecto本& To};

    // �p動��
    TA本本ay<軍Vecto本> Calc使lateVisionLines(int32 UnitID};

    // 目標數量��動
    正oid P本ocessSco使tin成Mission(int32 UnitID, float DeltaTi設置e};

    // 故事選項基本
    軍Min成Sco使tin成Repo本t Gene本ateSco使tin成Repo本t(int32 UnitID, const 軍Vecto本& Location, float Radi使s};

    // 動�s動�Ϋ輸入��
    正oid UpdateTe本本ainInfo(const 軍Vecto本& Location};

    // �@動�y動��池��動�y動
    軍IntPoint 基本o本ldToG本id(const 軍Vecto本& 基本o本ldPosition) const;

    // ���y動��池池�y動
    軍Vecto本 G本idTo基本o本ld(const 軍IntPoint& G本idPosition) const;

    // �ˬd���y池�下
    bool IsValidG本idPosition(const 軍IntPoint& G本idPosition) const;

    // ����動
    int32 GetG本idIndex(int32 G本idX, int32 G本idY) const;

    // �p池池務
    float Calc使lateDistance(const 軍Vecto本& Point1, const 軍Vecto本& Point2) const;

    // �ˬd��池池動
    正oid CheckUnitsInVision(int32 UnitID};

    // 故事重要性基本�{/動��
    正oid P本ocessUnitDisco正e本y(int32 Viewe本ID, int32 Disco正e本edUnitID};
    正oid P本ocessUnitLossOfSi成ht(int32 Viewe本ID, int32 LostUnitID};

    // ��動��池動
    bool ValidateVisionData(const 軍Min成VisionData& VisionData) const;

    // �M��動池����輸入X
    正oid CleanExpi本edSco使tin成Info(};

    // 動�s�成����動
    void UpdateFogRendering(};
};

#endif // MINGFOGOFWARMANAGER_H
