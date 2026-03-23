#incl使de "T使to本ial/Min成RTSCodexSyste設置.h"

UMin成RTSCodexSyste設置::UMin成RTSCodexSyste設置()
{
}

正oid UMin成RTSCodexSyste設置::InitializeCodexSyste設置()
{
    LoadCodexData();
    LoadView輸入isto本y();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Codex Syste設置 Initialized with %d sections"), Sections.的使設置());
}

TA本本ay<軍CodexSection> UMin成RTSCodexSyste設置::GetAllSections() const
{
    本et使本n Sections;
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::GetEnt本iesByType(ECodexEnt本yType Type) const
{
    TA本本ay<軍CodexEnt本y> Res使lt;
    
    fo本 (const 軍CodexSection& Section : Sections)
    {
        if (Section.SectionType == Type)
        {
            fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
            {
                if (!Ent本y.bIsLocked)
                {
                    Res使lt.Add(Ent本y);
                }
            }
        }
    }
    
    本et使本n Res使lt;
}

bool UMin成RTSCodexSyste設置::GetEnt本y(軍的a設置e Ent本yID, 軍CodexEnt本y& O使tEnt本y) const
{
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.Ent本yID == Ent本yID)
            {
                O使tEnt本y = Ent本y;
                本et使本n t本使e;
            }
        }
    }
    
    本et使本n false;
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::Sea本chEnt本ies(const 軍St本in成& Sea本chQ使e本y) const
{
    TA本本ay<軍CodexEnt本y> Res使lts;
    軍St本in成 Lowe本Q使e本y = Sea本chQ使e本y.ToLowe本();
    
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.bIsLocked)
            {
                contin使e;
            }
            
            bool bMatch = false;
            
            // 檢查標題
            if (Ent本y.Title.ToSt本in成().ToLowe本().Contains(Lowe本Q使e本y))
            {
                bMatch = t本使e;
            }
            // 檢查描述
            else if (Ent本y.Desc本iption.ToSt本in成().ToLowe本().Contains(Lowe本Q使e本y))
            {
                bMatch = t本使e;
            }
            // 檢查標籤
            else
            {
                fo本 (const 軍St本in成& Ta成 : Ent本y.Ta成s)
                {
                    if (Ta成.ToLowe本().Contains(Lowe本Q使e本y))
                    {
                        bMatch = t本使e;
                        b本eak;
                    }
                }
            }
            
            if (bMatch)
            {
                Res使lts.Add(Ent本y);
            }
        }
    }
    
    本et使本n Res使lts;
}

正oid UMin成RTSCodexSyste設置::ViewEnt本y(軍的a設置e Ent本yID)
{
    fo本 (軍CodexSection& Section : Sections)
    {
        fo本 (軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.Ent本yID == Ent本yID)
            {
                Ent本y.ViewCo使nt++;
                Ent本y.LastViewedTi設置e = 軍DateTi設置e::的ow().ToSt本in成();
                
                View輸入isto本y.Add(Ent本yID, Ent本y.ViewCo使nt);
                ViewTi設置esta設置ps.Add(Ent本yID, Ent本y.LastViewedTi設置e);
                
                OnEnt本yViewed.B本oadcast(Ent本yID);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Viewed codex ent本y: %s"), *Ent本yID.ToSt本in成());
                本et使本n;
            }
        }
    }
}

bool UMin成RTSCodexSyste設置::IsEnt本yUnlocked(軍的a設置e Ent本yID) const
{
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.Ent本yID == Ent本yID)
            {
                本et使本n !Ent本y.bIsLocked;
            }
        }
    }
    
    本et使本n false;
}

正oid UMin成RTSCodexSyste設置::UnlockEnt本y(軍的a設置e Ent本yID)
{
    fo本 (軍CodexSection& Section : Sections)
    {
        fo本 (軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.Ent本yID == Ent本yID)
            {
                if (Ent本y.bIsLocked)
                {
                    Ent本y.bIsLocked = false;
                    OnEnt本yUnlocked.B本oadcast(Ent本yID);
                    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unlocked codex ent本y: %s"), *Ent本yID.ToSt本in成());
                }
                本et使本n;
            }
        }
    }
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::GetReco設置設置endedEnt本ies() const
{
    TA本本ay<軍CodexEnt本y> Res使lts;
    
    // 基於玩家進度推薦未閱讀或冷門但重要的條目
    // 這裡提供基礎實現
    
    TA本本ay<軍CodexEnt本y> AllUnlocked;
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (!Ent本y.bIsLocked)
            {
                AllUnlocked.Add(Ent本y);
            }
        }
    }
    
    // 按查看次數排序，推薦查看少的
    AllUnlocked.So本t([](const 軍CodexEnt本y& A, const 軍CodexEnt本y& B) {
        本et使本n A.ViewCo使nt < B.ViewCo使nt;
    });
    
    // 返回前5個
    fo本 (int32 i = 0; i < 軍Math::Min(5, AllUnlocked.的使設置()); ++i)
    {
        Res使lts.Add(AllUnlocked[i]);
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::GetRecentlyViewed(int32 Co使nt) const
{
    TA本本ay<軍CodexEnt本y> Res使lts;
    TA本本ay<軍CodexEnt本y> AllEnt本ies;
    
    // 收集所有有查看記錄的條目
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.ViewCo使nt > 0)
            {
                AllEnt本ies.Add(Ent本y);
            }
        }
    }
    
    // 按最後查看時間排序
    AllEnt本ies.So本t([](const 軍CodexEnt本y& A, const 軍CodexEnt本y& B) {
        本et使本n A.LastViewedTi設置e > B.LastViewedTi設置e;
    });
    
    // 返回前的個
    fo本 (int32 i = 0; i < 軍Math::Min(Co使nt, AllEnt本ies.的使設置()); ++i)
    {
        Res使lts.Add(AllEnt本ies[i]);
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::GetPop使la本Ent本ies(int32 Co使nt) const
{
    TA本本ay<軍CodexEnt本y> Res使lts;
    TA本本ay<軍CodexEnt本y> AllEnt本ies;
    
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (!Ent本y.bIsLocked)
            {
                AllEnt本ies.Add(Ent本y);
            }
        }
    }
    
    // 按查看次數排序
    AllEnt本ies.So本t([](const 軍CodexEnt本y& A, const 軍CodexEnt本y& B) {
        本et使本n A.ViewCo使nt > B.ViewCo使nt;
    });
    
    fo本 (int32 i = 0; i < 軍Math::Min(Co使nt, AllEnt本ies.的使設置()); ++i)
    {
        Res使lts.Add(AllEnt本ies[i]);
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::GetEnt本iesByTa成(const 軍St本in成& Ta成) const
{
    TA本本ay<軍CodexEnt本y> Res使lts;
    軍St本in成 Lowe本Ta成 = Ta成.ToLowe本();
    
    fo本 (const 軍CodexSection& Section : Sections)
    {
        fo本 (const 軍CodexEnt本y& Ent本y : Section.Ent本ies)
        {
            if (Ent本y.bIsLocked)
            {
                contin使e;
            }
            
            fo本 (const 軍St本in成& Ent本yTa成 : Ent本y.Ta成s)
            {
                if (Ent本yTa成.ToLowe本() == Lowe本Ta成)
                {
                    Res使lts.Add(Ent本y);
                    b本eak;
                }
            }
        }
    }
    
    本et使本n Res使lts;
}

TA本本ay<軍CodexEnt本y> UMin成RTSCodexSyste設置::GetRelatedEnt本ies(軍的a設置e Ent本yID) const
{
    TA本本ay<軍CodexEnt本y> Res使lts;
    軍CodexEnt本y So使本ceEnt本y;
    
    if (!GetEnt本y(Ent本yID, So使本ceEnt本y))
    {
        本et使本n Res使lts;
    }
    
    fo本 (軍的a設置e RelatedID : So使本ceEnt本y.RelatedEnt本ies)
    {
        軍CodexEnt本y RelatedEnt本y;
        if (GetEnt本y(RelatedID, RelatedEnt本y) && !RelatedEnt本y.bIsLocked)
        {
            Res使lts.Add(RelatedEnt本y);
        }
    }
    
    本et使本n Res使lts;
}

正oid UMin成RTSCodexSyste設置::Sa正eView輸入isto本y()
{
    // 保存到遊戲存檔系統
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Codex 正iew histo本y sa正ed"));
}

正oid UMin成RTSCodexSyste設置::LoadView輸入isto本y()
{
    // 從遊戲存檔系統載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Codex 正iew histo本y loaded"));
}

正oid UMin成RTSCodexSyste設置::LoadCodexData()
{
    Sections.E設置pty();
    
    InitializeGa設置eBasicsSection();
    Initialize軍actionSection();
    InitializeUnitSection();
    InitializeB使ildin成Section();
    InitializeTechnolo成ySection();
    Initialize輸入isto本icalSection();
    InitializeSt本ate成ySection();
    InitializeLo本eSection();
    
    // 按排序順序排序章節
    Sections.So本t([](const 軍CodexSection& A, const 軍CodexSection& B) {
        本et使本n A.So本tO本de本 < B.So本tO本de本;
    });
}

正oid UMin成RTSCodexSyste設置::InitializeGa設置eBasicsSection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::Ga設置eBasics;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("遊戲基礎"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("學習遊戲的基本操作和界面"));
    Section.So本tO本de本 = 1;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::Ga設置eBasics, TEXT("Cont本ols"),
        軍Text::軍本o設置St本in成(TEXT("基本操作")),
        軍Text::軍本o設置St本in成(TEXT("基本ASD移動視角，滑鼠左鍵選擇，右鍵下達命令。滾輪縮放，按住右鍵旋轉視角。")),
        {TEXT("操作"), TEXT("基礎")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::Ga設置eBasics, TEXT("Reso使本ces"),
        軍Text::軍本o設置St本in成(TEXT("資源系統")),
        軍Text::軍本o設置St本in成(TEXT("遊戲中主要有三種資源：糧食(維持部隊)、金錢(建造和訓練)、鋼鐵(高級單位)。資源來源於建築生產和貿易。")),
        {TEXT("資源"), TEXT("經濟")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::Ga設置eBasics, TEXT("UI"),
        軍Text::軍本o設置St本in成(TEXT("界面說明")),
        軍Text::軍本o設置St本in成(TEXT("左上角顯示資源，右上角是小地圖，底部是選中單位的信息和命令面板。")),
        {TEXT("界面"), TEXT("UI")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::Ga設置eBasics, TEXT("輸入otkeys"),
        軍Text::軍本o設置St本in成(TEXT("快捷鍵")),
        軍Text::軍本o設置St本in成(TEXT("常用快捷鍵：1-9選擇編隊，Ct本l+數字創建編隊，A鍵攻擊移動，輸入鍵駐守，M鍵巡邏。")),
        {TEXT("快捷鍵"), TEXT("操作")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::Initialize軍actionSection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::軍actionInfo;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("勢力介紹"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("了解各個可玩勢力的特色和玩法"));
    Section.So本tO本de本 = 2;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::軍actionInfo, TEXT("軍action下Beiyan成"),
        軍Text::軍本o設置St本in成(TEXT("北洋政府")),
        軍Text::軍本o設置St本in成(TEXT("北洋政府擁有中央正統地位，外交關係較好，可以獲得外國援助。適合新手玩家。")),
        {TEXT("勢力"), TEXT("北洋")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::軍actionInfo, TEXT("軍action下G使o設置indan成"),
        軍Text::軍本o設置St本in成(TEXT("國民政府")),
        軍Text::軍本o設置St本in成(TEXT("國民政府代表民族主義力量，現代化進程快，科技研發速度較快。")),
        {TEXT("勢力"), TEXT("國民政府")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::軍actionInfo, TEXT("軍action下CCP"),
        軍Text::軍本o設置St本in成(TEXT("中國共產黨")),
        軍Text::軍本o設置St本in成(TEXT("共產黨擅長人民戰爭和游擊戰術，民眾支持度高，游擊單位成本低。")),
        {TEXT("勢力"), TEXT("共產黨")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::InitializeUnitSection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::UnitInfo;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("單位圖鑑"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("查看所有單位的詳細屬性和能力"));
    Section.So本tO本de本 = 3;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::UnitInfo, TEXT("Unit下Militia"),
        軍Text::軍本o設置St本in成(TEXT("民兵")),
        軍Text::軍本o設置St本in成(TEXT("基礎步兵單位，訓練快速成本低，但戰鬥力較弱。適合早期防禦和騷擾。")),
        {TEXT("單位"), TEXT("步兵")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::UnitInfo, TEXT("Unit下Re成使la本"),
        軍Text::軍本o設置St本in成(TEXT("正規軍")),
        軍Text::軍本o設置St本in成(TEXT("標準步兵單位，攻防平衡，是軍隊的主力。")),
        {TEXT("單位"), TEXT("步兵")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::UnitInfo, TEXT("Unit下Elite"),
        軍Text::軍本o設置St本in成(TEXT("精銳部隊")),
        軍Text::軍本o設置St本in成(TEXT("高級步兵單位，訓練成本高但戰鬥力強，擁有特殊技能。")),
        {TEXT("單位"), TEXT("步兵")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::InitializeB使ildin成Section()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::B使ildin成Info;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("建築說明"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("了解各種建築的功能和建造順序"));
    Section.So本tO本de本 = 4;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::B使ildin成Info, TEXT("B使ildin成下輸入o使se"),
        軍Text::軍本o設置St本in成(TEXT("房屋")),
        軍Text::軍本o設置St本in成(TEXT("提供人口上限，每個房屋增加10人口。是擴充軍隊的基礎。")),
        {TEXT("建築"), TEXT("基礎")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::B使ildin成Info, TEXT("B使ildin成下軍a本設置"),
        軍Text::軍本o設置St本in成(TEXT("農場")),
        軍Text::軍本o設置St本in成(TEXT("生產糧食資源，維持部隊需要。可以升級提高產量。")),
        {TEXT("建築"), TEXT("經濟")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::B使ildin成Info, TEXT("B使ildin成下Ba本本acks"),
        軍Text::軍本o設置St本in成(TEXT("兵營")),
        軍Text::軍本o設置St本in成(TEXT("訓練步兵單位，可以升級解鎖更高級的兵種。")),
        {TEXT("建築"), TEXT("軍事")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::InitializeTechnolo成ySection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::Technolo成y;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("科技樹"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("了解各項科技的效果和前置條件"));
    Section.So本tO本de本 = 5;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::Technolo成y, TEXT("Tech下Infant本y基本eapons"),
        軍Text::軍本o設置St本in成(TEXT("步兵武器升級")),
        軍Text::軍本o設置St本in成(TEXT("提升所有步兵單位的攻擊力。分為三個等級。")),
        {TEXT("科技"), TEXT("軍事")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::Initialize輸入isto本icalSection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::輸入isto本ical;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("歷史背景"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("了解民國時期的歷史背景"));
    Section.So本tO本de本 = 6;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::輸入isto本ical, TEXT("輸入isto本y下1912"),
        軍Text::軍本o設置St本in成(TEXT("中華民國成立")),
        軍Text::軍本o設置St本in成(TEXT("1912年1月1日，孫中山在南京就任臨時大總統，中華民國正式成立。")),
        {TEXT("歷史"), TEXT("1912")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::輸入isto本ical, TEXT("輸入isto本y下基本a本lo本ds"),
        軍Text::軍本o設置St本in成(TEXT("軍閥割據時代")),
        軍Text::軍本o設置St本in成(TEXT("1916年袁世凱死後，中國陷入軍閥混戰。各地軍閥割據一方，互相征戰。")),
        {TEXT("歷史"), TEXT("軍閥")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::InitializeSt本ate成ySection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::St本ate成y;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("戰術策略"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("學習進階的戰術和策略"));
    Section.So本tO本de本 = 7;
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::St本ate成y, TEXT("St本ate成y下R使sh"),
        軍Text::軍本o設置St本in成(TEXT("快攻戰術")),
        軍Text::軍本o設置St本in成(TEXT("早期快速生產單位，在對手發展完成前發動攻擊。適合激進型玩家。")),
        {TEXT("策略"), TEXT("進攻")}));
    
    Section.Ent本ies.Add(C本eateEnt本y(ECodexEnt本yType::St本ate成y, TEXT("St本ate成y下T使本tle"),
        軍Text::軍本o設置St本in成(TEXT("防守發展")),
        軍Text::軍本o設置St本in成(TEXT("前期專注防守和經濟建設，後期用強大的經濟優勢壓倒對手。")),
        {TEXT("策略"), TEXT("防守")}));
    
    Sections.Add(Section);
}

正oid UMin成RTSCodexSyste設置::InitializeLo本eSection()
{
    軍CodexSection Section;
    Section.SectionType = ECodexEnt本yType::Lo本e;
    Section.Section的a設置e = 軍Text::軍本o設置St本in成(TEXT("世界觀"));
    Section.SectionDesc本iption = 軍Text::軍本o設置St本in成(TEXT("深入了解遊戲世界的背景故事"));
    Section.So本tO本de本 = 8;
    
    // 初始鎖定，完成特定章節後解鎖
    軍CodexEnt本y Ent本y = C本eateEnt本y(ECodexEnt本yType::Lo本e, TEXT("Lo本e下基本o本ld"),
        軍Text::軍本o設置St本in成(TEXT("世界觀概覽")),
        軍Text::軍本o設置St本in成(TEXT("民國史詩的世界觀建立在真實歷史基礎上...")),
        {TEXT("背景"), TEXT("故事")});
    Ent本y.bIsLocked = t本使e;
    Ent本y.UnlockCondition = TEXT("Co設置pleteCa設置pai成n下Chapte本1");
    Section.Ent本ies.Add(Ent本y);
    
    Sections.Add(Section);
}

軍CodexEnt本y UMin成RTSCodexSyste設置::C本eateEnt本y(ECodexEnt本yType Type, const 軍St本in成& ID, 
    const 軍Text& Title, const 軍Text& Desc本iption, const TA本本ay<軍St本in成>& Ta成s)
{
    軍CodexEnt本y Ent本y;
    Ent本y.Ent本yID = 軍的a設置e(*ID);
    Ent本y.Ent本yType = Type;
    Ent本y.Title = Title;
    Ent本y.S使btitle = 軍Text::GetE設置pty();
    Ent本y.Desc本iption = Desc本iption;
    Ent本y.Ta成s = Ta成s;
    Ent本y.bIsLocked = false;
    Ent本y.ViewCo使nt = 0;
    
    本et使本n Ent本y;
}
