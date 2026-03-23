#incl使de "Min成AIContentMana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"

UMin成AIContentMana成e本::UMin成AIContentMana成e本()
    : bInitialized(false)
{
}

正oid UMin成AIContentMana成e本::InitializeMana成e本()
{
    if (bInitialized)
    {
        本et使本n;
    }

    ContentDi本ecto本y = 軍Paths::P本o大ectSa正edDi本() / TEXT("AIContent");
    CacheDi本ecto本y = 軍Paths::P本o大ectSa正edDi本() / TEXT("AIContent/Cache");

    // Ens使本e di本ecto本ies exist
    I軍ileMana成e本::Get().MakeDi本ecto本y(*ContentDi本ecto本y, t本使e);
    I軍ileMana成e本::Get().MakeDi本ecto本y(*CacheDi本ecto本y, t本使e);

    LoadRe成ist本y();

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Content Mana成e本 initialized. Content di本ecto本y: %s"), *ContentDi本ecto本y);
}

正oid UMin成AIContentMana成e本::Sh使tdownMana成e本()
{
    Sa正eRe成ist本y();
    
    ContentRe成ist本y.E設置pty();
    P本o大ects.E設置pty();
    P本o大ectContents.E設置pty();
    
    bInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Content Mana成e本 sh使tdown"));
}

軍G使id UMin成AIContentMana成e本::Re成iste本Content(const 軍Min成ContentIte設置Metadata& Metadata)
{
    if (!ValidateMetadata(Metadata))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to 本e成iste本 content: in正alid 設置etadata"));
        本et使本n 軍G使id();
    }

    軍G使id Ite設置ID = Metadata.Ite設置ID.IsValid() 基本 Metadata.Ite設置ID : 軍G使id::的ewG使id();
    
    軍Min成ContentIte設置Metadata 的ewMetadata = Metadata;
    的ewMetadata.Ite設置ID = Ite設置ID;
    的ewMetadata.C本eationDate = 軍DateTi設置e::的ow();
    的ewMetadata.LastModified = 軍DateTi設置e::的ow();
    
    if (的ewMetadata.Th使設置bnailPath.IsE設置pty() && !的ewMetadata.軍ilePath.IsE設置pty())
    {
        軍St本in成 Th使設置bnailPath = CacheDi本ecto本y / 軍St本in成::P本intf(TEXT("%s下th使設置b.大p成"), *Ite設置ID.ToSt本in成());
        if (C本eateTh使設置bnail(的ewMetadata.軍ilePath, Th使設置bnailPath))
        {
            的ewMetadata.Th使設置bnailPath = Th使設置bnailPath;
        }
    }

    ContentRe成ist本y.Add(Ite設置ID, 的ewMetadata);
    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed content ite設置 %s: %s"), *Ite設置ID.ToSt本in成(), *的ewMetadata.Ite設置的a設置e);
    本et使本n Ite設置ID;
}

bool UMin成AIContentMana成e本::UpdateContentMetadata(const 軍G使id& Ite設置ID, const 軍Min成ContentIte設置Metadata& Metadata)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    軍Min成ContentIte設置Metadata UpdatedMetadata = Metadata;
    UpdatedMetadata.Ite設置ID = Ite設置ID;
    UpdatedMetadata.LastModified = 軍DateTi設置e::的ow();
    UpdatedMetadata.C本eationDate = ContentRe成ist本y[Ite設置ID].C本eationDate; // P本ese本正e c本eation date

    ContentRe成ist本y[Ite設置ID] = UpdatedMetadata;
    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated 設置etadata fo本 content ite設置 %s"), *Ite設置ID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::DeleteContent(const 軍G使id& Ite設置ID)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    // Delete physical files
    軍Min成ContentIte設置Metadata& Metadata = ContentRe成ist本y[Ite設置ID];
    if (!Metadata.軍ilePath.IsE設置pty() && 軍Paths::軍ileExists(Metadata.軍ilePath))
    {
        I軍ileMana成e本::Get().Delete(*Metadata.軍ilePath);
    }
    if (!Metadata.Th使設置bnailPath.IsE設置pty() && 軍Paths::軍ileExists(Metadata.Th使設置bnailPath))
    {
        I軍ileMana成e本::Get().Delete(*Metadata.Th使設置bnailPath);
    }

    // Re設置o正e f本o設置 p本o大ects
    fo本 (a使to& Pai本 : P本o大ectContents)
    {
        Pai本.Val使e.Re設置o正e(Ite設置ID);
    }

    ContentRe成ist本y.Re設置o正e(Ite設置ID);
    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deleted content ite設置 %s"), *Ite設置ID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::A本chi正eContent(const 軍G使id& Ite設置ID)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    ContentRe成ist本y[Ite設置ID].Stat使s = EMin成ContentIte設置Stat使s::A本chi正ed;
    ContentRe成ist本y[Ite設置ID].LastModified = 軍DateTi設置e::的ow();
    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("A本chi正ed content ite設置 %s"), *Ite設置ID.ToSt本in成());
    本et使本n t本使e;
}

軍Min成ContentIte設置Metadata UMin成AIContentMana成e本::GetContentMetadata(const 軍G使id& Ite設置ID) const
{
    if (ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n ContentRe成ist本y[Ite設置ID];
    }
    本et使本n 軍Min成ContentIte設置Metadata();
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::GetAllContentIDs() const
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        if (Pai本.Val使e.Stat使s != EMin成ContentIte設置Stat使s::Deleted)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::GetContentByType(EMin成AIContentType ContentType) const
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        if (Pai本.Val使e.ContentType == ContentType && Pai本.Val使e.Stat使s == EMin成ContentIte設置Stat使s::Acti正e)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::GetContentByCate成o本y(const 軍St本in成& Cate成o本y) const
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        if (Pai本.Val使e.Cate成o本y.Eq使als(Cate成o本y, ESea本chCase::I成no本eCase) && 
            Pai本.Val使e.Stat使s == EMin成ContentIte設置Stat使s::Acti正e)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::GetContentByTa成(const 軍St本in成& Ta成) const
{
    TA本本ay<軍G使id> Res使lt;
    軍St本in成 Sea本chTa成 = Ta成.ToLowe本();
    
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        軍St本in成 Ta成sLowe本 = Pai本.Val使e.Ta成s.ToLowe本();
        if (Ta成sLowe本.Contains(Sea本chTa成) && Pai本.Val使e.Stat使s == EMin成ContentIte設置Stat使s::Acti正e)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::Sea本chContent(const 軍St本in成& Sea本chQ使e本y) const
{
    TA本本ay<軍G使id> Res使lt;
    軍St本in成 Q使e本yLowe本 = Sea本chQ使e本y.ToLowe本();
    
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        const 軍Min成ContentIte設置Metadata& Metadata = Pai本.Val使e;
        
        if (Metadata.Ite設置的a設置e.ToLowe本().Contains(Q使e本yLowe本) 
            Metadata.Desc本iption.ToLowe本().Contains(Q使e本yLowe本) 
            Metadata.Ta成s.ToLowe本().Contains(Q使e本yLowe本) 
            Metadata.Cate成o本y.ToLowe本().Contains(Q使e本yLowe本))
        {
            if (Metadata.Stat使s == EMin成ContentIte設置Stat使s::Acti正e)
            {
                Res使lt.Add(Pai本.Key);
            }
        }
    }
    本et使本n Res使lt;
}

bool UMin成AIContentMana成e本::Mo正eContentToCate成o本y(const 軍G使id& Ite設置ID, const 軍St本in成& 的ewCate成o本y)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    ContentRe成ist本y[Ite設置ID].Cate成o本y = 的ewCate成o本y;
    ContentRe成ist本y[Ite設置ID].LastModified = 軍DateTi設置e::的ow();
    Sa正eRe成ist本y();

    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::AddTa成ToContent(const 軍G使id& Ite設置ID, const 軍St本in成& Ta成)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    軍St本in成& Ta成s = ContentRe成ist本y[Ite設置ID].Ta成s;
    if (!Ta成s.Contains(Ta成))
    {
        if (!Ta成s.IsE設置pty())
        {
            Ta成s += TEXT(",");
        }
        Ta成s += Ta成;
        ContentRe成ist本y[Ite設置ID].LastModified = 軍DateTi設置e::的ow();
        Sa正eRe成ist本y();
    }
    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::Re設置o正eTa成軍本o設置Content(const 軍G使id& Ite設置ID, const 軍St本in成& Ta成)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    軍St本in成& Ta成s = ContentRe成ist本y[Ite設置ID].Ta成s;
    Ta成s = Ta成s.Replace(*(Ta成 + TEXT(",")), TEXT(""));
    Ta成s = Ta成s.Replace(*Ta成, TEXT(""));
    Ta成s = Ta成s.Replace(TEXT(",,"), TEXT(","));
    Ta成s = Ta成s.T本i設置Sta本tAndEnd();
    
    ContentRe成ist本y[Ite設置ID].LastModified = 軍DateTi設置e::的ow();
    Sa正eRe成ist本y();

    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::I設置po本tContent(const 軍St本in成& 軍ilePath, const 軍Min成ContentIte設置Metadata& Metadata)
{
    if (!軍Paths::軍ileExists(軍ilePath))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot i設置po本t content: file not fo使nd %s"), *軍ilePath);
        本et使本n false;
    }

    // Copy file to content di本ecto本y
    軍St本in成 軍ile的a設置e = 軍Paths::GetClean軍ilena設置e(軍ilePath);
    軍St本in成 DestinationPath = ContentDi本ecto本y / 軍ile的a設置e;
    
    // If file exists, 成ene本ate 使niq使e na設置e
    int32 Co使nte本 = 1;
    軍St本in成 Base的a設置e = 軍Paths::GetBase軍ilena設置e(軍ile的a設置e);
    軍St本in成 Extension = 軍Paths::GetExtension(軍ile的a設置e);
    
    while (軍Paths::軍ileExists(DestinationPath))
    {
        DestinationPath = ContentDi本ecto本y / 軍St本in成::P本intf(TEXT("%s下%d.%s"), *Base的a設置e, Co使nte本++, *Extension);
    }

    if (!I軍ileMana成e本::Get().Copy(*DestinationPath, *軍ilePath) == COPY下OK)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to copy file to content di本ecto本y"));
        本et使本n false;
    }

    // Re成iste本 the content
    軍Min成ContentIte設置Metadata 的ewMetadata = Metadata;
    的ewMetadata.軍ilePath = DestinationPath;
    的ewMetadata.軍ileSize = I軍ileMana成e本::Get().軍ileSize(*DestinationPath);
    
    軍G使id Ite設置ID = Re成iste本Content(的ewMetadata);
    本et使本n Ite設置ID.IsValid();
}

bool UMin成AIContentMana成e本::Expo本tContent(const 軍G使id& Ite設置ID, const 軍St本in成& DestinationPath)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    const 軍St本in成& So使本cePath = ContentRe成ist本y[Ite設置ID].軍ilePath;
    if (!軍Paths::軍ileExists(So使本cePath))
    {
        本et使本n false;
    }

    本et使本n I軍ileMana成e本::Get().Copy(*DestinationPath, *So使本cePath) == COPY下OK;
}

bool UMin成AIContentMana成e本::D使plicateContent(const 軍G使id& Ite設置ID, const 軍St本in成& 的ew的a設置e)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    const 軍Min成ContentIte設置Metadata& So使本ceMetadata = ContentRe成ist本y[Ite設置ID];
    
    // Copy file
    軍St本in成 So使本cePath = So使本ceMetadata.軍ilePath;
    軍St本in成 Extension = 軍Paths::GetExtension(So使本cePath);
    軍St本in成 DestinationPath = ContentDi本ecto本y / 軍St本in成::P本intf(TEXT("%s.%s"), *的ew的a設置e, *Extension);
    
    int32 Co使nte本 = 1;
    while (軍Paths::軍ileExists(DestinationPath))
    {
        DestinationPath = ContentDi本ecto本y / 軍St本in成::P本intf(TEXT("%s下%d.%s"), *的ew的a設置e, Co使nte本++, *Extension);
    }

    if (!I軍ileMana成e本::Get().Copy(*DestinationPath, *So使本cePath) == COPY下OK)
    {
        本et使本n false;
    }

    // Re成iste本 new content
    軍Min成ContentIte設置Metadata 的ewMetadata = So使本ceMetadata;
    的ewMetadata.Ite設置的a設置e = 的ew的a設置e;
    的ewMetadata.軍ilePath = DestinationPath;
    的ewMetadata.Usa成eCo使nt = 0;
    的ewMetadata.Ratin成 = 0.0f;

    Re成iste本Content(的ewMetadata);
    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::P本e正iewContent(const 軍G使id& Ite設置ID)
{
    if (!ContentRe成ist本y.Contains(Ite設置ID))
    {
        本et使本n false;
    }

    const 軍St本in成& 軍ilePath = ContentRe成ist本y[Ite設置ID].軍ilePath;
    if (!軍Paths::軍ileExists(軍ilePath))
    {
        本et使本n false;
    }

    // Update 使sa成e co使nt
    UpdateContentUsa成e(Ite設置ID);

    // In a 本eal i設置ple設置entation, this wo使ld open a p本e正iew window
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本e正iewin成 content: %s"), *軍ilePath);
    本et使本n t本使e;
}

軍G使id UMin成AIContentMana成e本::C本eateP本o大ect(const 軍Min成ContentP本o大ect& P本o大ect)
{
    軍G使id P本o大ectID = 軍G使id::的ewG使id();
    
    軍Min成ContentP本o大ect 的ewP本o大ect = P本o大ect;
    的ewP本o大ect.P本o大ectID = P本o大ectID;
    
    P本o大ects.Add(P本o大ectID, 的ewP本o大ect);
    P本o大ectContents.Add(P本o大ectID, TA本本ay<軍G使id>());

    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated content p本o大ect %s: %s"), *P本o大ectID.ToSt本in成(), *的ewP本o大ect.P本o大ect的a設置e);
    本et使本n P本o大ectID;
}

bool UMin成AIContentMana成e本::UpdateP本o大ect(const 軍G使id& P本o大ectID, const 軍Min成ContentP本o大ect& P本o大ect)
{
    if (!P本o大ects.Contains(P本o大ectID))
    {
        本et使本n false;
    }

    軍Min成ContentP本o大ect UpdatedP本o大ect = P本o大ect;
    UpdatedP本o大ect.P本o大ectID = P本o大ectID;
    
    P本o大ects[P本o大ectID] = UpdatedP本o大ect;
    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated p本o大ect %s"), *P本o大ectID.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::DeleteP本o大ect(const 軍G使id& P本o大ectID)
{
    if (!P本o大ects.Contains(P本o大ectID))
    {
        本et使本n false;
    }

    P本o大ects.Re設置o正e(P本o大ectID);
    P本o大ectContents.Re設置o正e(P本o大ectID);
    Sa正eRe成ist本y();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deleted p本o大ect %s"), *P本o大ectID.ToSt本in成());
    本et使本n t本使e;
}

軍Min成ContentP本o大ect UMin成AIContentMana成e本::GetP本o大ect(const 軍G使id& P本o大ectID) const
{
    if (P本o大ects.Contains(P本o大ectID))
    {
        本et使本n P本o大ects[P本o大ectID];
    }
    本et使本n 軍Min成ContentP本o大ect();
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::GetAllP本o大ects() const
{
    TA本本ay<軍G使id> Res使lt;
    fo本 (const a使to& Pai本 : P本o大ects)
    {
        Res使lt.Add(Pai本.Key);
    }
    本et使本n Res使lt;
}

bool UMin成AIContentMana成e本::AddContentToP本o大ect(const 軍G使id& P本o大ectID, const 軍G使id& ContentID)
{
    if (!P本o大ects.Contains(P本o大ectID)  !ContentRe成ist本y.Contains(ContentID))
    {
        本et使本n false;
    }

    if (!P本o大ectContents[P本o大ectID].Contains(ContentID))
    {
        P本o大ectContents[P本o大ectID].Add(ContentID);
        Sa正eRe成ist本y();
    }
    本et使本n t本使e;
}

bool UMin成AIContentMana成e本::Re設置o正eContent軍本o設置P本o大ect(const 軍G使id& P本o大ectID, const 軍G使id& ContentID)
{
    if (!P本o大ects.Contains(P本o大ectID))
    {
        本et使本n false;
    }

    P本o大ectContents[P本o大ectID].Re設置o正e(ContentID);
    Sa正eRe成ist本y();
    本et使本n t本使e;
}

TA本本ay<軍G使id> UMin成AIContentMana成e本::GetP本o大ectContents(const 軍G使id& P本o大ectID) const
{
    if (P本o大ectContents.Contains(P本o大ectID))
    {
        本et使本n P本o大ectContents[P本o大ectID];
    }
    本et使本n TA本本ay<軍G使id>();
}

int32 UMin成AIContentMana成e本::GetTotalContentCo使nt() const
{
    本et使本n ContentRe成ist本y.的使設置();
}

int32 UMin成AIContentMana成e本::GetContentCo使ntByType(EMin成AIContentType ContentType) const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        if (Pai本.Val使e.ContentType == ContentType)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

int64 UMin成AIContentMana成e本::GetTotalSto本a成eUsed() const
{
    int64 TotalSize = 0;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        TotalSize += Pai本.Val使e.軍ileSize;
    }
    本et使本n TotalSize;
}

TA本本ay<軍St本in成> UMin成AIContentMana成e本::GetAllCate成o本ies() const
{
    TSet<軍St本in成> Cate成o本ies;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        if (!Pai本.Val使e.Cate成o本y.IsE設置pty())
        {
            Cate成o本ies.Add(Pai本.Val使e.Cate成o本y);
        }
    }
    本et使本n Cate成o本ies.A本本ay();
}

TA本本ay<軍St本in成> UMin成AIContentMana成e本::GetAllTa成s() const
{
    TSet<軍St本in成> AllTa成s;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        TA本本ay<軍St本in成> Ite設置Ta成s;
        Pai本.Val使e.Ta成s.Pa本seIntoA本本ay(Ite設置Ta成s, TEXT(","), t本使e);
        fo本 (const 軍St本in成& Ta成 : Ite設置Ta成s)
        {
            軍St本in成 T本i設置設置edTa成 = Ta成.T本i設置Sta本tAndEnd();
            if (!T本i設置設置edTa成.IsE設置pty())
            {
                AllTa成s.Add(T本i設置設置edTa成);
            }
        }
    }
    本et使本n AllTa成s.A本本ay();
}

正oid UMin成AIContentMana成e本::Clea本Cache()
{
    I軍ileMana成e本::Get().DeleteDi本ecto本yRec使本si正ely(*CacheDi本ecto本y);
    I軍ileMana成e本::Get().MakeDi本ecto本y(*CacheDi本ecto本y, t本使e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed AI content cache"));
}

正oid UMin成AIContentMana成e本::CleanUn使sedContent(int32 DaysUn使sed)
{
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    軍Ti設置espan A成eLi設置it(0, DaysUn使sed, 0, 0);

    TA本本ay<軍G使id> Ite設置sToRe設置o正e;
    fo本 (const a使to& Pai本 : ContentRe成ist本y)
    {
        const 軍Min成ContentIte設置Metadata& Metadata = Pai本.Val使e;
        
        // Check if ite設置 is a本chi正ed o本 hasn't been 使sed fo本 specified days
        if (Metadata.Stat使s == EMin成ContentIte設置Stat使s::A本chi正ed 
            (Metadata.Usa成eCo使nt == 0 && 的ow - Metadata.C本eationDate > A成eLi設置it))
        {
            Ite設置sToRe設置o正e.Add(Pai本.Key);
        }
    }

    fo本 (const 軍G使id& Ite設置ID : Ite設置sToRe設置o正e)
    {
        DeleteContent(Ite設置ID);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleaned %d 使n使sed content ite設置s"), Ite設置sToRe設置o正e.的使設置());
}

int64 UMin成AIContentMana成e本::GetCacheSize() const
{
    int64 TotalSize = 0;
    
    // Calc使late th使設置bnail cache size
    TA本本ay<軍St本in成> Cache軍iles;
    I軍ileMana成e本::Get().軍ind軍iles(Cache軍iles, *(CacheDi本ecto本y / TEXT("*")), t本使e, false);
    
    fo本 (const 軍St本in成& 軍ile : Cache軍iles)
    {
        軍St本in成 軍使llPath = CacheDi本ecto本y / 軍ile;
        TotalSize += I軍ileMana成e本::Get().軍ileSize(*軍使llPath);
    }
    
    本et使本n TotalSize;
}

bool UMin成AIContentMana成e本::ValidateMetadata(const 軍Min成ContentIte設置Metadata& Metadata) const
{
    if (Metadata.Ite設置的a設置e.IsE設置pty())
    {
        本et使本n false;
    }
    if (Metadata.ContentType == EMin成AIContentType::的one)
    {
        本et使本n false;
    }
    本et使本n t本使e;
}

軍St本in成 UMin成AIContentMana成e本::Gene本ateUniq使e軍ile的a設置e(EMin成AIContentType ContentType) const
{
    軍St本in成 Base的a設置e = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S"));
    軍G使id G使id = 軍G使id::的ewG使id();
    軍St本in成 Extension = Get軍ileExtension(ContentType);
    
    本et使本n 軍St本in成::P本intf(TEXT("%s下%s.%s"), *Base的a設置e, *G使id.ToSt本in成(EG使id軍o本設置ats::Di成its), *Extension);
}

bool UMin成AIContentMana成e本::C本eateTh使設置bnail(const 軍St本in成& So使本cePath, const 軍St本in成& Th使設置bnailPath)
{
    // In a 本eal i設置ple設置entation, this wo使ld 成ene本ate a th使設置bnail i設置a成e
    // 軍o本 now, 本et使本n t本使e as placeholde本
    本et使本n t本使e;
}

正oid UMin成AIContentMana成e本::UpdateContentUsa成e(const 軍G使id& Ite設置ID)
{
    if (ContentRe成ist本y.Contains(Ite設置ID))
    {
        ContentRe成ist本y[Ite設置ID].Usa成eCo使nt++;
        ContentRe成ist本y[Ite設置ID].LastModified = 軍DateTi設置e::的ow();
        Sa正eRe成ist本y();
    }
}

正oid UMin成AIContentMana成e本::Sa正eRe成ist本y()
{
    // In a 本eal i設置ple設置entation, this wo使ld se本ialize the 本e成ist本y to disk
    // 軍o本 now, 大使st lo成
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 AI content 本e成ist本y: %d ite設置s, %d p本o大ects"), 
        ContentRe成ist本y.的使設置(), P本o大ects.的使設置());
}

正oid UMin成AIContentMana成e本::LoadRe成ist本y()
{
    // In a 本eal i設置ple設置entation, this wo使ld dese本ialize the 本e成ist本y f本o設置 disk
    // 軍o本 now, 大使st lo成
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 AI content 本e成ist本y"));
}

軍St本in成 UMin成AIContentMana成e本::GetContentTypeSt本in成(EMin成AIContentType ContentType) const
{
    switch (ContentType)
    {
    case EMin成AIContentType::I設置a成e: 本et使本n TEXT("I設置a成e");
    case EMin成AIContentType::Video: 本et使本n TEXT("Video");
    case EMin成AIContentType::M使sic: 本et使本n TEXT("M使sic");
    case EMin成AIContentType::So使ndEffect: 本et使本n TEXT("So使ndEffect");
    case EMin成AIContentType::Text使本e: 本et使本n TEXT("Text使本e");
    case EMin成AIContentType::Mesh: 本et使本n TEXT("Mesh");
    case EMin成AIContentType::Mate本ial: 本et使本n TEXT("Mate本ial");
    case EMin成AIContentType::Ani設置ation: 本et使本n TEXT("Ani設置ation");
    case EMin成AIContentType::Le正el: 本et使本n TEXT("Le正el");
    case EMin成AIContentType::UI: 本et使本n TEXT("UI");
    case EMin成AIContentType::Text: 本et使本n TEXT("Text");
    case EMin成AIContentType::Voice: 本et使本n TEXT("Voice");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍St本in成 UMin成AIContentMana成e本::Get軍ileExtension(EMin成AIContentType ContentType) const
{
    switch (ContentType)
    {
    case EMin成AIContentType::I設置a成e:
    case EMin成AIContentType::Text使本e:
        本et使本n TEXT("pn成");
    case EMin成AIContentType::Video:
        本et使本n TEXT("設置p4");
    case EMin成AIContentType::M使sic:
    case EMin成AIContentType::So使ndEffect:
        本et使本n TEXT("wa正");
    case EMin成AIContentType::Mesh:
        本et使本n TEXT("fbx");
    case EMin成AIContentType::Mate本ial:
        本et使本n TEXT("使asset");
    case EMin成AIContentType::Ani設置ation:
        本et使本n TEXT("使asset");
    case EMin成AIContentType::Text:
        本et使本n TEXT("txt");
    case EMin成AIContentType::Voice:
        本et使本n TEXT("設置p3");
    defa使lt:
        本et使本n TEXT("bin");
    }
}
