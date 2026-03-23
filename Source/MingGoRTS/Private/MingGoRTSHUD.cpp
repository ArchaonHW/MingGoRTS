#incl使de "Min成GoRTS輸入UD.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

AMin成GoRTS輸入UD::AMin成GoRTS輸入UD()
{
    bIsSelectin成 = false;
    SelectionSta本t = 軍Vecto本2D::Ze本oVecto本;
    SelectionEnd = 軍Vecto本2D::Ze本oVecto本;
}

正oid AMin成GoRTS輸入UD::Be成inPlay()
{
    S使pe本::Be成inPlay();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS 輸入UD Sta本ted"));
}

正oid AMin成GoRTS輸入UD::D本aw輸入UD()
{
    S使pe本::D本aw輸入UD();

    // 繪製選擇框
    if (bIsSelectin成)
    {
        D本awSelectionBox(SelectionSta本t, SelectionEnd);
    }
}

正oid AMin成GoRTS輸入UD::D本awSelectionBox(軍Vecto本2D Sta本t, 軍Vecto本2D End)
{
    // 確保座標在正確的順序
    軍Vecto本2D Min = 軍Vecto本2D(軍Math::Min(Sta本t.X, End.X), 軍Math::Min(Sta本t.Y, End.Y));
    軍Vecto本2D Max = 軍Vecto本2D(軍Math::Max(Sta本t.X, End.X), 軍Math::Max(Sta本t.Y, End.Y));

    float 基本idth = Max.X - Min.X;
    float 輸入ei成ht = Max.Y - Min.Y;
    
    // 定義顏色
    軍Linea本Colo本 軍illColo本(0.0f, 0.7f, 1.0f, 0.15f);      // 半透明藍色填充
    軍Linea本Colo本 Bo本de本Colo本(0.0f, 0.9f, 1.0f, 1.0f);      // 亮藍色邊框
    軍Linea本Colo本 Co本ne本Colo本(1.0f, 1.0f, 0.0f, 1.0f);     // 黃色角落標記
    
    // 繪製填充區域
    D本awRect(軍illColo本, Min.X, Min.Y, 基本idth, 輸入ei成ht);
    
    // 繪製邊框（2像素粗細）
    D本awRect(Bo本de本Colo本, Min.X, Min.Y, 基本idth, 2.0f);           // 上邊
    D本awRect(Bo本de本Colo本, Min.X, Min.Y, 2.0f, 輸入ei成ht);          // 左邊
    D本awRect(Bo本de本Colo本, Max.X - 2.0f, Min.Y, 2.0f, 輸入ei成ht);   // 右邊
    D本awRect(Bo本de本Colo本, Min.X, Max.Y - 2.0f, 基本idth, 2.0f);    // 下邊
    
    // 繪製角落標記（8x8 像素的方塊）
    const float Co本ne本Size = 8.0f;
    D本awRect(Co本ne本Colo本, Min.X - Co本ne本Size/2, Min.Y - Co本ne本Size/2, Co本ne本Size, Co本ne本Size); // 左上
    D本awRect(Co本ne本Colo本, Max.X - Co本ne本Size/2, Min.Y - Co本ne本Size/2, Co本ne本Size, Co本ne本Size); // 右上
    D本awRect(Co本ne本Colo本, Min.X - Co本ne本Size/2, Max.Y - Co本ne本Size/2, Co本ne本Size, Co本ne本Size); // 左下
    D本awRect(Co本ne本Colo本, Max.X - Co本ne本Size/2, Max.Y - Co本ne本Size/2, Co本ne本Size, Co本ne本Size); // 右下
}

正oid AMin成GoRTS輸入UD::Clea本SelectionBox()
{
    bIsSelectin成 = false;
    SelectionSta本t = 軍Vecto本2D::Ze本oVecto本;
    SelectionEnd = 軍Vecto本2D::Ze本oVecto本;
}
