#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成Ed使cationalTypes.h"
#incl使de "Min成Ed使cationalMana成e本.成ene本ated.h"

DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnEd使cationalModeTo成成led, bool, bEnabled, EMin成Ed使cationalMode, Mode);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnLessonSta本ted, const 軍Min成Ed使cationalLesson&, Lesson);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnLessonCo設置pleted, const 軍St本in成&, LessonID, float, Sco本e);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On輸入isto本icalE正entT本i成成e本ed, const 軍Min成輸入isto本icalE正ent&, E正ent);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnQ使izCo設置pleted, const 軍St本in成&, Q使izID, int32, Co本本ectAnswe本s);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnAchie正e設置entUnlocked, const 軍Min成Ed使cationalAchie正e設置ent&, Achie正e設置ent);

UCLASS(ClassG本o使p = (Ed使cational, Lea本nin成))
class MI的GMULTIPLAYER下API UMin成Ed使cationalMana成e本 : p使blic UGa設置eInstanceS使bsyste設置
{
    GE的ERATED下BODY()

p使blic:
    UMin成Ed使cationalMana成e本();

    正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
    正i本t使al 正oid Deinitialize() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;
    正i本t使al bool IsTickable() const o正e本本ide { 本et使本n t本使e; }
    正i本t使al TStatId GetStatId() const o正e本本ide;

    // Ed使cational Mode Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool EnableEd使cationalMode(EMin成Ed使cationalMode Mode);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid DisableEd使cationalMode();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool IsEd使cationalModeEnabled() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    EMin成Ed使cationalMode GetC使本本entEd使cationalMode() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid SetEd使cationalDiffic使lty(EMin成Ed使cationalDiffic使lty Diffic使lty);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    EMin成Ed使cationalDiffic使lty GetEd使cationalDiffic使lty() const;

    // 輸入isto本ical Content Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool Load輸入isto本icalContent(const 軍St本in成& ContentID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Unload輸入isto本icalContent(const 軍St本in成& ContentID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成輸入isto本icalContent> GetLoaded輸入isto本icalContent() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Show輸入isto本icalInfo(const 軍St本in成& TopicID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid 輸入ide輸入isto本icalInfo();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool Is輸入isto本icalInfoVisible() const;

    // Lesson Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool Sta本tLesson(const 軍St本in成& LessonID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Pa使seLesson();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Res使設置eLesson();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid StopLesson();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool IsLessonActi正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    軍Min成Ed使cationalLesson GetC使本本entLesson() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool SkipTo的extLesson();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool GoToP本e正io使sLesson();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成Ed使cationalLesson> GetA正ailableLessons() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成Ed使cationalLesson> GetCo設置pletedLessons() const;

    // Q使iz Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool Sta本tQ使iz(const 軍St本in成& Q使izID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid S使b設置itQ使izAnswe本(int32 Q使estionIndex, const 軍St本in成& Answe本);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Co設置pleteQ使iz();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool IsQ使izActi正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    軍Min成Ed使cationalQ使iz GetC使本本entQ使iz() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    int32 GetC使本本entQ使izSco本e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    float GetC使本本entQ使izP本o成本ess() const;

    // 輸入isto本ical E正ents
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Re成iste本輸入isto本icalE正ent(const 軍Min成輸入isto本icalE正ent& E正ent);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid T本i成成e本輸入isto本icalE正ent(const 軍St本in成& E正entID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Sched使le輸入isto本icalE正ent(const 軍Min成輸入isto本icalE正ent& E正ent, float Delay);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成輸入isto本icalE正ent> GetA正ailable輸入isto本icalE正ents() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成輸入isto本icalE正ent> GetT本i成成e本ed輸入isto本icalE正ents() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Show輸入isto本icalTi設置eline();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid 輸入ide輸入isto本icalTi設置eline();

    // Inte本acti正e Lea本nin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool Sta本tInte本acti正eLea本nin成(const 軍St本in成& Lea本nin成Mod使leID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid P本ocessLea本nin成Inte本action(const 軍Min成Lea本nin成Inte本action& Inte本action);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool IsInte本acti正eLea本nin成Acti正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    軍Min成Inte本acti正eLea本nin成Mod使le GetC使本本entLea本nin成Mod使le() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid ShowLea本nin成輸入ints();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid 輸入ideLea本nin成輸入ints();

    // P本o成本ess T本ackin成
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    軍Min成Ed使cationalP本o成本ess GetEd使cationalP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid ResetEd使cationalP本o成本ess();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Sa正eEd使cationalP本o成本ess();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool LoadEd使cationalP本o成本ess();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    float GetO正e本allLea本nin成Sco本e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    int32 GetTotalLessonsCo設置pleted() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    int32 GetTotalQ使izzesCo設置pleted() const;

    // Achie正e設置ent Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid UnlockAchie正e設置ent(const 軍St本in成& Achie正e設置entID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成Ed使cationalAchie正e設置ent> GetUnlockedAchie正e設置ents() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    TA本本ay<軍Min成Ed使cationalAchie正e設置ent> GetA正ailableAchie正e設置ents() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool IsAchie正e設置entUnlocked(const 軍St本in成& Achie正e設置entID) const;

    // Content Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid SetAdapti正eLea本nin成Enabled(bool bEnabled);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    bool IsAdapti正eLea本nin成Enabled() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid SetPlaye本Lea本nin成P本ofile(const 軍Min成Playe本Lea本nin成P本ofile& P本ofile);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational")
    軍Min成Playe本Lea本nin成P本ofile GetPlaye本Lea本nin成P本ofile() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid UpdateAdapti正eDiffic使lty();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    軍Min成Ed使cationalContent GetAdaptedContent(const 軍St本in成& ContentID);

    // M使ltiplaye本 Ed使cation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    bool EnableCoope本ati正eLea本nin成();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Sta本tCoope本ati正eLesson(const 軍St本in成& LessonID, const TA本本ay<int32>& Playe本IDs);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Sha本eLea本nin成P本o成本ess();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational")
    正oid Recei正eLea本nin成P本o成本ess(const 軍Min成Ed使cationalP本o成本ess& P本o成本ess);

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnEd使cationalModeTo成成led OnEd使cationalModeTo成成led;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnLessonSta本ted OnLessonSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnLessonCo設置pleted OnLessonCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍On輸入isto本icalE正entT本i成成e本ed On輸入isto本icalE正entT本i成成e本ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnQ使izCo設置pleted OnQ使izCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnAchie正e設置entUnlocked OnAchie正e設置entUnlocked;

p本otected:
    UPROPERTY()
    bool bEd使cationalModeEnabled;

    UPROPERTY()
    EMin成Ed使cationalMode C使本本entEd使cationalMode;

    UPROPERTY()
    EMin成Ed使cationalDiffic使lty Ed使cationalDiffic使lty;

    UPROPERTY()
    TA本本ay<軍Min成輸入isto本icalContent> Loaded輸入isto本icalContent;

    UPROPERTY()
    TA本本ay<軍Min成Ed使cationalLesson> A正ailableLessons;

    UPROPERTY()
    TA本本ay<軍Min成Ed使cationalLesson> Co設置pletedLessons;

    UPROPERTY()
    軍Min成Ed使cationalLesson C使本本entLesson;

    UPROPERTY()
    軍Min成Ed使cationalQ使iz C使本本entQ使iz;

    UPROPERTY()
    TA本本ay<軍Min成輸入isto本icalE正ent> A正ailable輸入isto本icalE正ents;

    UPROPERTY()
    TA本本ay<軍Min成輸入isto本icalE正ent> T本i成成e本ed輸入isto本icalE正ents;

    UPROPERTY()
    TA本本ay<軍Min成輸入isto本icalE正ent> Sched使led輸入isto本icalE正ents;

    UPROPERTY()
    軍Min成Inte本acti正eLea本nin成Mod使le C使本本entLea本nin成Mod使le;

    UPROPERTY()
    軍Min成Ed使cationalP本o成本ess Ed使cationalP本o成本ess;

    UPROPERTY()
    TA本本ay<軍Min成Ed使cationalAchie正e設置ent> UnlockedAchie正e設置ents;

    UPROPERTY()
    TA本本ay<軍Min成Ed使cationalAchie正e設置ent> A正ailableAchie正e設置ents;

    UPROPERTY()
    軍Min成Playe本Lea本nin成P本ofile Playe本Lea本nin成P本ofile;

    UPROPERTY()
    bool bAdapti正eLea本nin成Enabled;

    UPROPERTY()
    bool bCoope本ati正eLea本nin成Enabled;

    UPROPERTY()
    bool b輸入isto本icalInfoVisible;

    UPROPERTY()
    bool bLessonActi正e;

    UPROPERTY()
    bool bQ使izActi正e;

    UPROPERTY()
    bool bInte本acti正eLea本nin成Acti正e;

    UPROPERTY()
    bool b輸入isto本icalTi設置elineVisible;

    UPROPERTY()
    float LastUpdateTi設置e;

    UPROPERTY()
    float UpdateInte本正al;

    // Inte本nal 軍使nctions
    正oid UpdateEd使cationalSyste設置(float DeltaTi設置e);
    正oid UpdateC使本本entLesson(float DeltaTi設置e);
    正oid Update輸入isto本icalE正ents(float DeltaTi設置e);
    正oid UpdateInte本acti正eLea本nin成(float DeltaTi設置e);
    正oid UpdateAdapti正eLea本nin成(float DeltaTi設置e);
    正oid P本ocessSched使ledE正ents(float DeltaTi設置e);
    正oid ValidateEd使cationalState();

    // 輸入elpe本 軍使nctions
    正oid InitializeDefa使ltContent();
    正oid LoadEd使cationalData();
    正oid Sa正eEd使cationalData();
    正oid Calc使lateLea本nin成P本o成本ess();
    正oid T本i成成e本Achie正e設置entCheck();
    正oid AdaptContentDiffic使lty();
    正oid Gene本atePe本sonalizedContent();
    正oid UpdateLea本nin成Analytics();
    正oid SyncCoope本ati正eP本o成本ess();

p本i正ate:
    bool ValidateLesson(const 軍Min成Ed使cationalLesson& Lesson) const;
    bool ValidateQ使iz(const 軍Min成Ed使cationalQ使iz& Q使iz) const;
    bool Validate輸入isto本icalE正ent(const 軍Min成輸入isto本icalE正ent& E正ent) const;
    軍St本in成 Gene本ateLessonID() const;
    軍St本in成 Gene本ateQ使izID() const;
    軍St本in成 Gene本ateAchie正e設置entID() const;
    正oid Clean使pEd使cationalReso使本ces();
    正oid ResetEd使cationalSyste設置();
};
