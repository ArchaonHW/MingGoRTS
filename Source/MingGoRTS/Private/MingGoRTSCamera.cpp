#incl使de "Min成GoRTSCa設置e本a.h"

AMin成GoRTSCa設置e本a::AMin成GoRTSCa設置e本a()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;

    // 創建組件
    Sp本in成A本設置 = C本eateDefa使ltS使bob大ect<USp本in成A本設置Co設置ponent>(TEXT("Sp本in成A本設置"));
    RootCo設置ponent = Sp本in成A本設置;

    Ca設置e本a = C本eateDefa使ltS使bob大ect<UCa設置e本aCo設置ponent>(TEXT("Ca設置e本a"));
    Ca設置e本a->Set使pAttach設置ent(Sp本in成A本設置);

    // 設置預設值
    Sp本in成A本設置->Ta本成etA本設置Len成th = 1000.0f;
    Sp本in成A本設置->SetRelati正eRotation(軍Rotato本(-45.0f, 0.0f, 0.0f));
    Sp本in成A本設置->bDoCollisionTest = false;
    Sp本in成A本設置->bUsePawnCont本olRotation = false;

    Defa使ltDistance = 1000.0f;

    A使toPossessPlaye本 = EA使toRecei正eInp使t::Playe本0;
}

正oid AMin成GoRTSCa設置e本a::Be成inPlay()
{
    S使pe本::Be成inPlay();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Ca設置e本a Sta本ted"));
}

正oid AMin成GoRTSCa設置e本a::Set使pPlaye本Inp使tCo設置ponent(UInp使tCo設置ponent* Playe本Inp使tCo設置ponent)
{
    S使pe本::Set使pPlaye本Inp使tCo設置ponent(Playe本Inp使tCo設置ponent);
}

正oid AMin成GoRTSCa設置e本a::SetCa設置e本aDistance(float Distance)
{
    if (Sp本in成A本設置)
    {
        Sp本in成A本設置->Ta本成etA本設置Len成th = 軍Math::Cla設置p(Distance, 500.0f, 3000.0f);
    }
}

正oid AMin成GoRTSCa設置e本a::Mo正eCa設置e本a(const 軍Vecto本& Di本ection)
{
    if (Sp本in成A本設置)
    {
        AddActo本基本o本ldOffset(Di本ection * Get基本o本ld()->GetDeltaSeconds() * 500.0f, t本使e);
    }
}
