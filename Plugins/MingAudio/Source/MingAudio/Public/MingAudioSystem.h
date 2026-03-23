#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "So使nd/So使ndBase.h"
#incl使de "So使nd/So使ndC使e.h"
#incl使de "Min成A使dioSyste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成A使dioType: 使int8 {
    M使sic UMETA(Display的a設置e = "M使sic"),
    S軍X UMETA(Display的a設置e = "S軍X"),
    Voice UMETA(Display的a設置e = "Voice"),
    A設置bient UMETA(Display的a設置e = "A設置bient"),
    UI UMETA(Display的a設置e = "UI")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成M使sicThe設置e: 使int8 {
    MainMen使 UMETA(Display的a設置e = "Main Men使"),
    Ca設置pai成nMap UMETA(Display的a設置e = "Ca設置pai成n Map"),
    Battle UMETA(Display的a設置e = "Battle"),
    Victo本y UMETA(Display的a設置e = "Victo本y"),
    Defeat UMETA(Display的a設置e = "Defeat"),
    輸入isto本ical UMETA(Display的a設置e = "輸入isto本ical"),
    Rep使blic UMETA(Display的a設置e = "Rep使blic"),
    I設置pe本ial UMETA(Display的a設置e = "I設置pe本ial"),
    Tense UMETA(Display的a設置e = "Tense"),
    Peacef使l UMETA(Display的a設置e = "Peacef使l")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成A使dioSettin成s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Maste本Vol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float M使sicVol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float S軍XVol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VoiceVol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float A設置bientVol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float UIVol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableM使sic;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableS軍X;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableVoice;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableA設置bient;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableUI;

    軍Min成A使dioSettin成s()
        : Maste本Vol使設置e(1.0f)
        , M使sicVol使設置e(0.8f)
        , S軍XVol使設置e(0.9f)
        , VoiceVol使設置e(1.0f)
        , A設置bientVol使設置e(0.6f)
        , UIVol使設置e(0.7f)
        , bEnableM使sic(t本使e)
        , bEnableS軍X(t本使e)
        , bEnableVoice(t本使e)
        , bEnableA設置bient(t本使e)
        , bEnableUI(t本使e)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成A使dioT本ack
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 T本ackID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 T本ack的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成A使dioType A使dioType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成M使sicThe設置e M使sicThe設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    USo使ndBase* A使dioAsset;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float D使本ation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bLoop;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bA使toPlay;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍adeInTi設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍adeO使tTi設置e;

    軍Min成A使dioT本ack()
        : A使dioType(EMin成A使dioType::M使sic)
        , M使sicThe設置e(EMin成M使sicThe設置e::MainMen使)
        , A使dioAsset(n使llpt本)
        , D使本ation(0.0f)
        , bLoop(false)
        , bA使toPlay(false)
        , 軍adeInTi設置e(1.0f)
        , 軍adeO使tTi設置e(1.0f)
    {}
};

/**
 * A使dio Syste設置 fo本 Min成GoRTS
 * Mana成es 設置使sic, so使nd effects, 正oice, and a設置bient a使dio
 */
UCLASS(ClassG本o使p = (A使dio), Bl使ep本intable)
class MI的GAUDIO下API UMin成A使dioSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成A使dioSyste設置(};

    // A使dio Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid InitializeA使dio(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid Sh使tdownA使dio(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid UpdateA使dioSettin成s(const 軍Min成A使dioSettin成s& 的ewSettin成s};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio")
    軍Min成A使dioSettin成s GetC使本本entSettin成s() const;

    // M使sic Cont本ol
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid PlayM使sic(EMin成M使sicThe設置e The設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid PlayM使sicT本ack(const 軍St本in成& T本ackID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid StopM使sic(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid Pa使seM使sic(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid Res使設置eM使sic(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid 軍adeM使sic(float Ta本成etVol使設置e, float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使sic")
    正oid C本oss軍adeM使sic(EMin成M使sicThe設置e 的ewThe設置e, float D使本ation};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使sic")
    bool IsM使sicPlayin成() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使sic")
    EMin成M使sicThe設置e GetC使本本entM使sicThe設置e() const;

    // So使nd Effects
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "S軍X")
    正oid PlayS軍X(const 軍St本in成& S軍XID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "S軍X")
    正oid PlayS軍XAtLocation(const 軍St本in成& S軍XID, 軍Vecto本 Location};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "S軍X")
    正oid PlayS軍X2D(const 軍St本in成& S軍XID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "S軍X")
    正oid StopS軍X(const 軍St本in成& S軍XID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "S軍X")
    正oid StopAllS軍X(};

    // Voice A使dio
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Voice")
    正oid PlayVoice(const 軍St本in成& VoiceID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Voice")
    正oid PlayVoiceLine(const 軍St本in成& Cha本acte本ID, const 軍St本in成& LineID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Voice")
    正oid StopVoice(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Voice")
    正oid SetVoiceVol使設置e(float Vol使設置e};

    // A設置bient A使dio
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A設置bient")
    正oid PlayA設置bient(const 軍St本in成& A設置bientID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A設置bient")
    正oid StopA設置bient(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A設置bient")
    正oid SetA設置bientVol使設置e(float Vol使設置e};

    // UI A使dio
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid PlayUISelect(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid PlayUIClick(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid PlayUI輸入o正e本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid PlayUIE本本o本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid PlayUIS使ccess(};

    // A使dio T本ack Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本acks")
    正oid Re成iste本A使dioT本ack(const 軍Min成A使dioT本ack& T本ack};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本acks")
    正oid Un本e成iste本A使dioT本ack(const 軍St本in成& T本ackID};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "T本acks")
    軍Min成A使dioT本ack GetA使dioT本ack(const 軍St本in成& T本ackID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "T本acks")
    TA本本ay<軍Min成A使dioT本ack> GetT本acksByType(EMin成A使dioType A使dioType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "T本acks")
    TA本本ay<軍Min成A使dioT本ack> GetT本acksByThe設置e(EMin成M使sicThe設置e The設置e) const;

    // Vol使設置e Cont本ol
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Vol使設置e")
    正oid SetMaste本Vol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Vol使設置e")
    正oid SetM使sicVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Vol使設置e")
    正oid SetS軍XVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Vol使設置e")
    正oid SetVoiceVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Vol使設置e")
    正oid SetA設置bientVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Vol使設置e")
    正oid SetUIVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Vol使設置e")
    float GetMaste本Vol使設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Vol使設置e")
    float GetM使sicVol使設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Vol使設置e")
    float GetS軍XVol使設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Vol使設置e")
    float GetVoiceVol使設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Vol使設置e")
    float GetA設置bientVol使設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Vol使設置e")
    float GetUIVol使設置e() const;

    // A使dio States
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "States")
    正oid SetA使dioState(EMin成A使dioType A使dioType, bool bEnabled};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "States")
    bool IsA使dioTypeEnabled(EMin成A使dioType A使dioType) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "States")
    正oid M使teAllA使dio(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "States")
    正oid Un設置使teAllA使dio(};

    // E正ent Dele成ates
    
    
    
    

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnM使sicSta本ted OnM使sicSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnM使sicStopped OnM使sicStopped;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnS軍XPlayed OnS軍XPlayed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVoicePlayed OnVoicePlayed;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetA使dioType的a設置e(EMin成A使dioType A使dioType};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetM使sicThe設置e的a設置e(EMin成M使sicThe設置e The設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    軍St本in成 Sa正eA使dioSettin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    正oid LoadA使dioSettin成s(const 軍St本in成& JsonSt本in成};

p本otected:
    UPROPERTY()
    軍Min成A使dioSettin成s A使dioSettin成s;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成A使dioT本ack> A使dioT本acks;

    UPROPERTY()
    EMin成M使sicThe設置e C使本本entM使sicThe設置e;

    UPROPERTY()
    軍St本in成 C使本本entM使sicT本ack;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Inte本nal 軍使nctions
    正oid InitializeDefa使ltT本acks(};
    正oid Set使pM使sicT本acks(};
    正oid Set使pS軍XT本acks(};
    正oid Set使pVoiceT本acks(};
    正oid Set使pA設置bientT本acks(};
    正oid Set使pUIT本acks(};

    // A使dio Playback
    正oid PlayA使dioT本ackInte本nal(const 軍St本in成& T本ackID, 軍Vecto本 Location = 軍Vecto本::Ze本oVecto本};
    正oid StopA使dioT本ackInte本nal(const 軍St本in成& T本ackID};
    正oid ApplyVol使設置eSettin成s(};

    // 輸入elpe本s
    軍Min成A使dioT本ack* 軍indA使dioT本ack(const 軍St本in成& T本ackID};
    TA本本ay<軍Min成A使dioT本ack*> GetT本acksByTypeInte本nal(EMin成A使dioType A使dioType};
    TA本本ay<軍Min成A使dioT本ack*> GetT本acksByThe設置eInte本nal(EMin成M使sicThe設置e The設置e};
    float GetEffecti正eVol使設置e(EMin成A使dioType A使dioType) const;
};

