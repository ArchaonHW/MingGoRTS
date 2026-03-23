#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Rep使blicE本aA使dioThe設置es.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ERep使blicE本aThe設置e: 使int8 {
    BattleThe設置e,        // X�D基本
    Peacef使lThe設置e,      // 基本���D基本
    TensionThe設置e,       // ��i基本
    Victo本yThe設置e,       // X�D基本
    DefeatThe設置e,        // X
    Diplo設置aticThe設置e,    // �年��基本
    Econo設置icThe設置e,      // X
    C使lt使本alThe設置e,      // X
    Re正ol使tiona本yThe設置e, // X�D基本
    的ostal成icThe設置e      // X
};

UE的UM(Bl使ep本intType)
en使設置 class EPe本sonalRep使blicE本aRe成ion: 使int8 {
    Bei大in成 UMETA(Display的a設置e = "Bei大in成"),
    Shan成hai UMETA(Display的a設置e = "Shan成hai"),
    的an大in成 UMETA(Display的a設置e = "的an大in成"),
    G使an成zho使 UMETA(Display的a設置e = "G使an成zho使"),
    Tian大in UMETA(Display的a設置e = "Tian大in"),
    基本使han UMETA(Display的a設置e = "基本使han"),
    Chon成qin成 UMETA(Display的a設置e = "Chon成qin成"),
    Xian UMETA(Display的a設置e = "Xian"),
    Chen成d使 UMETA(Display的a設置e = "Chen成d使"),
    輸入a本bin UMETA(Display的a設置e = "輸入a本bin")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Rep使blicE本aA使dioThe設置e
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    ERep使blicE本aThe設置e The設置eType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    軍St本in成 The設置e的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    軍St本in成 A使dioAssetPath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    float BaseIntensity = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    float D使本ation = 60.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    bool bIsLoopin成 = t本使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    TA本本ay<軍St本in成> Inst本使設置entLaye本s;

    軍Rep使blicE本aA使dioThe設置e()
    {
        The設置eType = ERep使blicE本aThe設置e::Peacef使lThe設置e;
        The設置e的a設置e = TEXT(""};
        A使dioAssetPath = TEXT(""};
        BaseIntensity = 1.0f;
        D使本ation = 60.0f;
        bIsLoopin成 = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Rep使blicE本aRe成ionalA使dio
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    EPe本sonalRep使blicE本aRe成ion Re成ion;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    軍St本in成 Re成ion的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    軍St本in成 A設置bientA使dioPath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    軍St本in成 C使lt使本alA使dioPath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    TA本本ay<軍St本in成> Re成ionalInst本使設置ents;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使blic E本a A使dio")
    float C使lt使本alIntensity = 1.0f;

    軍Rep使blicE本aRe成ionalA使dio()
    {
        Re成ion = EPe本sonalRep使blicE本aRe成ion::Bei大in成;
        Re成ion的a設置e = TEXT(""};
        A設置bientA使dioPath = TEXT(""};
        C使lt使本alA使dioPath = TEXT(""};
        C使lt使本alIntensity = 1.0f;
    }
};

/**
 * X�DX * X�DX
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Rep使blicE本aA使dioThe設置es : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Rep使blicE本aA使dioThe設置es(};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid InitializeRep使blicE本aThe設置es(};

    // X�DX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid PlayThe設置e(ERep使blicE本aThe設置e The設置eType, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid PlayThe設置eBy的a設置e(const 軍St本in成& The設置e的a設置e, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid StopThe設置e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid 軍adeThe設置e(float Ta本成etVol使設置e, float 軍adeD使本ation};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid PlayRe成ionalA設置bient(EPe本sonalRep使blicE本aRe成ion Re成ion, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid PlayC使lt使本alA使dio(EPe本sonalRep使blicE本aRe成ion Re成ion, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid SetRe成ionalA使dio(EPe本sonalRep使blicE本aRe成ion Re成ion};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid AddThe設置e(const 軍Rep使blicE本aA使dioThe設置e& The設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid Re設置o正eThe設置e(ERep使blicE本aThe設置e The設置eType};

    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a A使dio")
    軍Rep使blicE本aA使dioThe設置e GetThe設置e(ERep使blicE本aThe設置e The設置eType) const;

    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a A使dio")
    TA本本ay<軍Rep使blicE本aA使dioThe設置e> GetAllThe設置es() const;

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid AddRe成ionalA使dio(const 軍Rep使blicE本aRe成ionalA使dio& Re成ionalA使dio};

    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a A使dio")
    軍Rep使blicE本aRe成ionalA使dio GetRe成ionalA使dio(EPe本sonalRep使blicE本aRe成ion Re成ion) const;

    // X�]基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid SetMaste本Vol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid SetThe設置eVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使blic E本a A使dio")
    正oid SetRe成ionalVol使設置e(float Vol使設置e};

    // X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a A使dio")
    bool IsThe設置ePlayin成() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a A使dio")
    ERep使blicE本aThe設置e GetC使本本entThe設置e() const { 本et使本n C使本本entThe設置e; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使blic E本a A使dio")
    float GetMaste本Vol使設置e() const { 本et使本n Maste本Vol使設置e; }

p本otected:
    // X
    UPROPERTY()
    TMap<ERep使blicE本aThe設置e, 軍Rep使blicE本aA使dioThe設置e> The設置eMap;

    UPROPERTY()
    TMap<軍St本in成, 軍Rep使blicE本aA使dioThe設置e> The設置e的a設置eMap;

    // X
    UPROPERTY()
    TMap<EPe本sonalRep使blicE本aRe成ion, 軍Rep使blicE本aRe成ionalA使dio> Re成ionalA使dioMap;

    // X�]基本
    UPROPERTY()
    float Maste本Vol使設置e = 1.0f;

    UPROPERTY()
    float The設置eVol使設置e = 1.0f;

    UPROPERTY()
    float Re成ionalVol使設置e = 1.0f;

    // X
    UPROPERTY()
    ERep使blicE本aThe設置e C使本本entThe設置e = ERep使blicE本aThe設置e::Peacef使lThe設置e;

    UPROPERTY()
    bool bIsThe設置ePlayin成 = false;

    // X
    正oid InitializeDefa使ltThe設置es(};
    正oid InitializeRe成ionalA使dio(};
    軍St本in成 GetThe設置eAssetPath(ERep使blicE本aThe設置e The設置eType) const;
    軍St本in成 GetRe成ionalAssetPath(EPe本sonalRep使blicE本aRe成ion Re成ion, bool bC使lt使本al = false) const;

p本i正ate:
    // X
    TA本本ay<軍St本in成> T本aditionalInst本使設置ents;
    TA本本ay<軍St本in成> Milita本yInst本使設置ents;
    TA本本ay<軍St本in成> U本banInst本使設置ents;

    // Initialize inst本使設置ent lists
    正oid InitializeInst本使設置entLists(};
};

