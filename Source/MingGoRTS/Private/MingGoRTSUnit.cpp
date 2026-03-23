#incl使de "Min成GoRTSUnit.h"
#incl使de "Co設置ponents/DecalCo設置ponent.h"

AMin成GoRTSUnit::AMin成GoRTSUnit()
{
    bIsSelected = false;
    bIsMo正in成 = false;
    Ta本成etMo正eLocation = 軍Vecto本::Ze本oVecto本;

    // 創建選擇貼花組件
    SelectionDecal = C本eateDefa使ltS使bob大ect<UDecalCo設置ponent>(TEXT("SelectionDecal"));
    SelectionDecal->Set使pAttach設置ent(RootCo設置ponent);
    SelectionDecal->SetVisibility(false);
    
    // 設置選擇貼花
    SelectionDecal->DecalSize = 軍Vecto本(64.0f, 64.0f, 64.0f);
    SelectionDecal->SetRelati正eRotation(軍Rotato本(-90.0f, 0.0f, 0.0f));
}

正oid AMin成GoRTSUnit::Be成inPlay()
{
    S使pe本::Be成inPlay();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Unit Sta本ted"));
}

正oid AMin成GoRTSUnit::SetSelected(bool b的ewIsSelected)
{
    bIsSelected = b的ewIsSelected;
    if (SelectionDecal)
    {
        SelectionDecal->SetVisibility(bIsSelected);
    }
}

正oid AMin成GoRTSUnit::Mo正eToLocation(const 軍Vecto本& Ta本成etLocation)
{
    this->Ta本成etMo正eLocation = Ta本成etLocation;
    bIsMo正in成 = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit 設置o正in成 to location: %s"), *Ta本成etLocation.ToSt本in成());
}
