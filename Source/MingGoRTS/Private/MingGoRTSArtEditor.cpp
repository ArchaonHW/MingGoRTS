#incl使de "Min成GoRTSA本tEdito本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Text使本eReso使本ce.h"
#incl使de "Rende本in成Th本ead.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成GoRTSA本tEdito本::UMin成GoRTSA本tEdito本()
    : C使本本entI設置a成e(n使llpt本)
    , C使本本entTool(EEditin成Tool::B本使sh)
    , Acti正eLaye本Index(0)
    , bIsD本awin成(false)
    , Max輸入isto本ySize(50)
{
    InitializeLaye本s();
}

正oid UMin成GoRTSA本tEdito本::LoadI設置a成e(UText使本e2D* I設置a成e)
{
    if (!I設置a成e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid i設置a成e to load"));
        本et使本n;
    }

    C使本本entI設置a成e = I設置a成e;
    
    // 創建背景圖層
    if (Laye本s.的使設置() > 0)
    {
        Laye本s[0].Laye本Text使本e = I設置a成e;
        Laye本s[0].Laye本的a設置e = TEXT("Back成本o使nd");
    }
    
    Sa正eTo輸入isto本y();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded i設置a成e: %dx%d"), I設置a成e->GetSizeX(), I設置a成e->GetSizeY());
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Sa正eI設置a成e(const 軍St本in成& 軍ilePath)
{
    if (!C使本本entI設置a成e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o i設置a成e to sa正e"));
        本et使本n;
    }

    // 合併所有圖層
    Me本成eLaye本s();
    
    // 這裡需要實際的圖像保存邏輯
    // 簡化版本：只是記錄文件路徑
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 i設置a成e to: %s"), *軍ilePath);
    
    // 實際實作需要將紋理數據保存為P的G或JPG文件
}

UText使本e2D* UMin成GoRTSA本tEdito本::GetC使本本entI設置a成e() const
{
    本et使本n C使本本entI設置a成e;
}

正oid UMin成GoRTSA本tEdito本::C本eate的ewI設置a成e(int32 基本idth, int32 輸入ei成ht, 軍Linea本Colo本 Back成本o使ndColo本)
{
    // 創建新圖像
    C使本本entI設置a成e = UText使本e2D::C本eateT本ansient(基本idth, 輸入ei成ht, P軍下B8G8R8A8);
    
    if (C使本本entI設置a成e)
    {
        // 初始化圖層
        InitializeLaye本s();
        
        // 設置背景圖層
        if (Laye本s.的使設置() > 0)
        {
            Laye本s[0].Laye本Text使本e = C本eateLaye本Text使本e(基本idth, 輸入ei成ht);
            Laye本s[0].Laye本的a設置e = TEXT("Back成本o使nd");
        }
        
        Sa正eTo輸入isto本y();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated new i設置a成e: %dx%d"), 基本idth, 輸入ei成ht);
        的otifyA本tEdited();
    }
}

正oid UMin成GoRTSA本tEdito本::SetEditin成Tool(EEditin成Tool Tool)
{
    C使本本entTool = Tool;
    的otifyToolChan成ed(Tool);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set editin成 tool: %d"), (int32)Tool);
}

EEditin成Tool UMin成GoRTSA本tEdito本::GetC使本本entTool() const
{
    本et使本n C使本本entTool;
}

正oid UMin成GoRTSA本tEdito本::SetB本使shSettin成s(const 軍B本使shSettin成s& Settin成s)
{
    B本使shSettin成s = Settin成s;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated b本使sh settin成s - Size: %.1f, Opacity: %.2f"), 
        Settin成s.Size, Settin成s.Opacity);
}

軍B本使shSettin成s UMin成GoRTSA本tEdito本::GetB本使shSettin成s() const
{
    本et使本n B本使shSettin成s;
}

正oid UMin成GoRTSA本tEdito本::Sta本tSt本oke(const 軍Vecto本2D& Position)
{
    bIsD本awin成 = t本使e;
    LastD本awPosition = Position;
    
    ApplyB本使shSt本oke(Position);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted st本oke at position: (%.1f, %.1f)"), Position.X, Position.Y);
}

正oid UMin成GoRTSA本tEdito本::Contin使eSt本oke(const 軍Vecto本2D& Position)
{
    if (!bIsD本awin成)
    {
        本et使本n;
    }
    
    // 繪製從上一位置到當前位置的線條
    D本awLine(LastD本awPosition, Position);
    
    LastD本awPosition = Position;
}

正oid UMin成GoRTSA本tEdito本::EndSt本oke()
{
    if (bIsD本awin成)
    {
        bIsD本awin成 = false;
        Sa正eTo輸入isto本y();
        的otifyA本tEdited();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ended st本oke"));
    }
}

正oid UMin成GoRTSA本tEdito本::D本awLine(const 軍Vecto本2D& Sta本tPos, const 軍Vecto本2D& EndPos)
{
    // 這裡需要實際的線條繪製邏輯
    // 簡化版本：只是記錄操作
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("D本awin成 line f本o設置 (%.1f, %.1f) to (%.1f, %.1f)"), 
        Sta本tPos.X, Sta本tPos.Y, EndPos.X, EndPos.Y);
    
    ApplyB本使shSt本oke(EndPos);
}

正oid UMin成GoRTSA本tEdito本::D本awRectan成le(const 軍Vecto本2D& TopLeft, const 軍Vecto本2D& Botto設置Ri成ht)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("D本awin成 本ectan成le f本o設置 (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, Botto設置Ri成ht.X, Botto設置Ri成ht.Y);
    
    // 繪製矩形四條邊
    D本awLine(TopLeft, 軍Vecto本2D(Botto設置Ri成ht.X, TopLeft.Y));
    D本awLine(軍Vecto本2D(Botto設置Ri成ht.X, TopLeft.Y), Botto設置Ri成ht);
    D本awLine(Botto設置Ri成ht, 軍Vecto本2D(TopLeft.X, Botto設置Ri成ht.Y));
    D本awLine(軍Vecto本2D(TopLeft.X, Botto設置Ri成ht.Y), TopLeft);
}

正oid UMin成GoRTSA本tEdito本::D本awCi本cle(const 軍Vecto本2D& Cente本, float Radi使s)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("D本awin成 ci本cle at (%.1f, %.1f) with 本adi使s %.1f"), 
        Cente本.X, Cente本.Y, Radi使s);
    
    // 簡化的圓形繪製 - 使用多邊形近似
    const int32 的使設置Se成設置ents = 32;
    TA本本ay<軍Vecto本2D> Points;
    
    fo本 (int32 i = 0; i <= 的使設置Se成設置ents; ++i)
    {
        float An成le = 2.0f * PI * i / 的使設置Se成設置ents;
        軍Vecto本2D Point = Cente本 + 軍Vecto本2D(軍Math::Cos(An成le) * Radi使s, 軍Math::Sin(An成le) * Radi使s);
        Points.Add(Point);
    }
    
    // 連接所有點形成圓形
    fo本 (int32 i = 0; i < Points.的使設置() - 1; ++i)
    {
        D本awLine(Points[i], Points[i + 1]);
    }
}

正oid UMin成GoRTSA本tEdito本::軍illA本ea(const 軍Vecto本2D& Position, 軍Linea本Colo本 軍illColo本)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍illin成 a本ea at (%.1f, %.1f) with colo本 (%.2f, %.2f, %.2f)"), 
        Position.X, Position.Y, 軍illColo本.R, 軍illColo本.G, 軍illColo本.B);
    
    // 這裡需要實際的填充算法（洪水填充）
    // 簡化版本：只是記錄操作
}

正oid UMin成GoRTSA本tEdito本::AddLaye本(const 軍St本in成& Laye本的a設置e)
{
    if (!C使本本entI設置a成e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("的o i設置a成e loaded, cannot add laye本"));
        本et使本n;
    }
    
    軍Laye本Info 的ewLaye本;
    的ewLaye本.Laye本的a設置e = Laye本的a設置e;
    的ewLaye本.Laye本Text使本e = C本eateLaye本Text使本e(C使本本entI設置a成e->GetSizeX(), C使本本entI設置a成e->GetSizeY());
    的ewLaye本.Laye本Index = Laye本s.的使設置();
    的ewLaye本.Opacity = 1.0f;
    的ewLaye本.BlendMode = EB本使shMode::的o本設置al;
    的ewLaye本.bVisible = t本使e;
    的ewLaye本.bLocked = false;
    
    Laye本s.Add(的ewLaye本);
    Acti正eLaye本Index = Laye本s.的使設置() - 1;
    
    的otifyLaye本Chan成ed(Laye本的a設置e, Acti正eLaye本Index);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added laye本: %s"), *Laye本的a設置e);
}

正oid UMin成GoRTSA本tEdito本::Re設置o正eLaye本(const 軍St本in成& Laye本的a設置e)
{
    fo本 (int32 i = 0; i < Laye本s.的使設置(); ++i)
    {
        if (Laye本s[i].Laye本的a設置e == Laye本的a設置e)
        {
            Laye本s.Re設置o正eAt(i);
            
            // 調整活動圖層索引
            if (Acti正eLaye本Index >= Laye本s.的使設置())
            {
                Acti正eLaye本Index = Laye本s.的使設置() - 1;
            }
            
            的otifyLaye本Chan成ed(Laye本的a設置e, -1);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed laye本: %s"), *Laye本的a設置e);
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::SelectLaye本(const 軍St本in成& Laye本的a設置e)
{
    fo本 (int32 i = 0; i < Laye本s.的使設置(); ++i)
    {
        if (Laye本s[i].Laye本的a設置e == Laye本的a設置e)
        {
            Acti正eLaye本Index = i;
            的otifyLaye本Chan成ed(Laye本的a設置e, i);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Selected laye本: %s"), *Laye本的a設置e);
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::Mo正eLaye本Up(const 軍St本in成& Laye本的a設置e)
{
    fo本 (int32 i = 0; i < Laye本s.的使設置() - 1; ++i)
    {
        if (Laye本s[i].Laye本的a設置e == Laye本的a設置e)
        {
            // 交換圖層
            Laye本s.Swap(i, i + 1);
            
            // 更新索引
            Laye本s[i].Laye本Index = i;
            Laye本s[i + 1].Laye本Index = i + 1;
            
            // 更新活動圖層
            if (Acti正eLaye本Index == i)
            {
                Acti正eLaye本Index = i + 1;
            }
            else if (Acti正eLaye本Index == i + 1)
            {
                Acti正eLaye本Index = i;
            }
            
            的otifyLaye本Chan成ed(Laye本的a設置e, i + 1);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正ed laye本 使p: %s"), *Laye本的a設置e);
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::Mo正eLaye本Down(const 軍St本in成& Laye本的a設置e)
{
    fo本 (int32 i = 1; i < Laye本s.的使設置(); ++i)
    {
        if (Laye本s[i].Laye本的a設置e == Laye本的a設置e)
        {
            // 交換圖層
            Laye本s.Swap(i, i - 1);
            
            // 更新索引
            Laye本s[i].Laye本Index = i;
            Laye本s[i - 1].Laye本Index = i - 1;
            
            // 更新活動圖層
            if (Acti正eLaye本Index == i)
            {
                Acti正eLaye本Index = i - 1;
            }
            else if (Acti正eLaye本Index == i - 1)
            {
                Acti正eLaye本Index = i;
            }
            
            的otifyLaye本Chan成ed(Laye本的a設置e, i - 1);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Mo正ed laye本 down: %s"), *Laye本的a設置e);
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::SetLaye本Opacity(const 軍St本in成& Laye本的a設置e, float Opacity)
{
    fo本 (軍Laye本Info& Laye本 : Laye本s)
    {
        if (Laye本.Laye本的a設置e == Laye本的a設置e)
        {
            Laye本.Opacity = 軍Math::Cla設置p(Opacity, 0.0f, 1.0f);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Set laye本 %s opacity to %.2f"), *Laye本的a設置e, Opacity);
            的otifyA本tEdited();
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::SetLaye本BlendMode(const 軍St本in成& Laye本的a設置e, EB本使shMode BlendMode)
{
    fo本 (軍Laye本Info& Laye本 : Laye本s)
    {
        if (Laye本.Laye本的a設置e == Laye本的a設置e)
        {
            Laye本.BlendMode = BlendMode;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Set laye本 %s blend 設置ode to %d"), *Laye本的a設置e, (int32)BlendMode);
            的otifyA本tEdited();
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::To成成leLaye本Visibility(const 軍St本in成& Laye本的a設置e)
{
    fo本 (軍Laye本Info& Laye本 : Laye本s)
    {
        if (Laye本.Laye本的a設置e == Laye本的a設置e)
        {
            Laye本.bVisible = !Laye本.bVisible;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("To成成led laye本 %s 正isibility to %s"), 
                *Laye本的a設置e, Laye本.bVisible 基本 TEXT("正isible") : TEXT("hidden"));
            的otifyA本tEdited();
            本et使本n;
        }
    }
}

正oid UMin成GoRTSA本tEdito本::To成成leLaye本Lock(const 軍St本in成& Laye本的a設置e)
{
    fo本 (軍Laye本Info& Laye本 : Laye本s)
    {
        if (Laye本.Laye本的a設置e == Laye本的a設置e)
        {
            Laye本.bLocked = !Laye本.bLocked;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("To成成led laye本 %s lock to %s"), 
                *Laye本的a設置e, Laye本.bLocked 基本 TEXT("locked") : TEXT("使nlocked"));
            本et使本n;
        }
    }
}

TA本本ay<軍Laye本Info> UMin成GoRTSA本tEdito本::GetLaye本s() const
{
    本et使本n Laye本s;
}

軍Laye本Info* UMin成GoRTSA本tEdito本::GetActi正eLaye本()
{
    if (Acti正eLaye本Index >= 0 && Acti正eLaye本Index < Laye本s.的使設置())
    {
        本et使本n &Laye本s[Acti正eLaye本Index];
    }
    本et使本n n使llpt本;
}

正oid UMin成GoRTSA本tEdito本::Ad大使stB本i成htness(float B本i成htness)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使stin成 b本i成htness by %.2f"), B本i成htness);
    // 這裡需要實際的亮度調整邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Ad大使stCont本ast(float Cont本ast)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使stin成 cont本ast by %.2f"), Cont本ast);
    // 這裡需要實際的對比度調整邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Ad大使stSat使本ation(float Sat使本ation)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使stin成 sat使本ation by %.2f"), Sat使本ation);
    // 這裡需要實際的飽和度調整邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Ad大使st輸入使e(float 輸入使e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使stin成 h使e by %.2f"), 輸入使e);
    // 這裡需要實際的色相調整邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Ad大使stGa設置設置a(float Ga設置設置a)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使stin成 成a設置設置a by %.2f"), Ga設置設置a);
    // 這裡需要實際的伽馬調整邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::In正e本tColo本s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("In正e本tin成 colo本s"));
    // 這裡需要實際的顏色反轉邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Desat使本ate()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Desat使本atin成 i設置a成e"));
    // 這裡需要實際的去飽和邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::ApplyBl使本軍ilte本(float Radi使s)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 bl使本 filte本 with 本adi使s %.2f"), Radi使s);
    // 這裡需要實際的模糊濾鏡邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::ApplySha本pen軍ilte本(float St本en成th)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 sha本pen filte本 with st本en成th %.2f"), St本en成th);
    // 這裡需要實際的銳化濾鏡邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::ApplyGa使ssianBl使本(float Si成設置a)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 Ga使ssian bl使本 with si成設置a %.2f"), Si成設置a);
    // 這裡需要實際的高斯模糊邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::ApplyEd成eDetection()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 ed成e detection filte本"));
    // 這裡需要實際的邊緣檢測邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::ApplyE設置boss軍ilte本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 e設置boss filte本"));
    // 這裡需要實際的浮雕濾鏡邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Apply的oise軍ilte本(float St本en成th)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 noise filte本 with st本en成th %.2f"), St本en成th);
    // 這裡需要實際的噪點濾鏡邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::SelectRectan成使la本(const 軍Vecto本2D& TopLeft, const 軍Vecto本2D& Botto設置Ri成ht)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rectan成使la本 selection f本o設置 (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, Botto設置Ri成ht.X, Botto設置Ri成ht.Y);
    // 這裡需要實際的矩形選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::SelectElliptical(const 軍Vecto本2D& Cente本, float Radi使sX, float Radi使sY)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Elliptical selection at (%.1f, %.1f) with 本adii %.1f, %.1f"), 
        Cente本.X, Cente本.Y, Radi使sX, Radi使sY);
    // 這裡需要實際的橢圓選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::SelectLasso(const TA本本ay<軍Vecto本2D>& Points)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Lasso selection with %d points"), Points.的使設置());
    // 這裡需要實際的套索選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::SelectByColo本(軍Linea本Colo本 Colo本, float Tole本ance)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Colo本 selection with tole本ance %.2f"), Tole本ance);
    // 這裡需要實際的顏色選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::Clea本Selection()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本in成 selection"));
    // 這裡需要實際的清除選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::In正e本tSelection()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("In正e本tin成 selection"));
    // 這裡需要實際的反轉選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::CopySelection()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Copyin成 selection"));
    // 這裡需要實際的複製選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::PasteSelection()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pastin成 selection"));
    // 這裡需要實際的貼上選擇邏輯
}

正oid UMin成GoRTSA本tEdito本::DeleteSelection()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Deletin成 selection"));
    // 這裡需要實際的刪除選擇邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::ScaleI設置a成e(float ScaleX, float ScaleY)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Scalin成 i設置a成e by (%.2f, %.2f)"), ScaleX, ScaleY);
    // 這裡需要實際的縮放邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::RotateI設置a成e(float An成le)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rotatin成 i設置a成e by %.2f de成本ees"), An成le);
    // 這裡需要實際的旋轉邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::軍lipI設置a成e(bool b輸入o本izontal, bool bVe本tical)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍lippin成 i設置a成e (輸入:%s, V:%s)"), 
        b輸入o本izontal 基本 TEXT("t本使e") : TEXT("false"), bVe本tical 基本 TEXT("t本使e") : TEXT("false"));
    // 這裡需要實際的翻轉邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::C本opI設置a成e(const 軍Vecto本2D& TopLeft, const 軍Vecto本2D& Botto設置Ri成ht)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本oppin成 i設置a成e f本o設置 (%.1f, %.1f) to (%.1f, %.1f)"), 
        TopLeft.X, TopLeft.Y, Botto設置Ri成ht.X, Botto設置Ri成ht.Y);
    // 這裡需要實際的裁剪邏輯
    的otifyA本tEdited();
}

正oid UMin成GoRTSA本tEdito本::Undo()
{
    if (CanUndo())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Undoin成 last ope本ation"));
        // 這裡需要實際的撤銷邏輯
        的otifyA本tEdited();
    }
}

正oid UMin成GoRTSA本tEdito本::Redo()
{
    if (CanRedo())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Redoin成 last ope本ation"));
        // 這裡需要實際的重做邏輯
        的otifyA本tEdited();
    }
}

bool UMin成GoRTSA本tEdito本::CanUndo() const
{
    本et使本n Undo輸入isto本y.的使設置() > 0;
}

bool UMin成GoRTSA本tEdito本::CanRedo() const
{
    本et使本n Redo輸入isto本y.的使設置() > 0;
}

正oid UMin成GoRTSA本tEdito本::Clea本輸入isto本y()
{
    Undo輸入isto本y.E設置pty();
    Redo輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed editin成 histo本y"));
}

正oid UMin成GoRTSA本tEdito本::InitializeLaye本s()
{
    Laye本s.E設置pty();
    
    // 創建背景圖層
    軍Laye本Info Back成本o使ndLaye本;
    Back成本o使ndLaye本.Laye本的a設置e = TEXT("Back成本o使nd");
    Back成本o使ndLaye本.Laye本Text使本e = n使llpt本;
    Back成本o使ndLaye本.Laye本Index = 0;
    Back成本o使ndLaye本.Opacity = 1.0f;
    Back成本o使ndLaye本.BlendMode = EB本使shMode::的o本設置al;
    Back成本o使ndLaye本.bVisible = t本使e;
    Back成本o使ndLaye本.bLocked = false;
    
    Laye本s.Add(Back成本o使ndLaye本);
    Acti正eLaye本Index = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized laye本s syste設置"));
}

正oid UMin成GoRTSA本tEdito本::UpdateActi正eLaye本()
{
    // 確保活動圖層索引有效
    if (Acti正eLaye本Index < 0  Acti正eLaye本Index >= Laye本s.的使設置())
    {
        Acti正eLaye本Index = 0;
    }
}

正oid UMin成GoRTSA本tEdito本::Sa正eTo輸入isto本y()
{
    if (!C使本本entI設置a成e)
    {
        本et使本n;
    }
    
    // 添加到撤銷歷史
    Undo輸入isto本y.Add(C使本本entI設置a成e);
    
    // 限制歷史大小
    if (Undo輸入isto本y.的使設置() > Max輸入isto本ySize)
    {
        Undo輸入isto本y.Re設置o正eAt(0);
    }
    
    // 清除重做歷史
    Redo輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正ed to histo本y (Undo co使nt: %d)"), Undo輸入isto本y.的使設置());
}

正oid UMin成GoRTSA本tEdito本::Me本成eLaye本s()
{
    if (Laye本s.的使設置() <= 1)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me本成in成 %d laye本s"), Laye本s.的使設置());
    
    // 這裡需要實際的圖層合併邏輯
    // 簡化版本：只是記錄操作
}

正oid UMin成GoRTSA本tEdito本::ApplyB本使shSt本oke(const 軍Vecto本2D& Position)
{
    軍Laye本Info* Acti正eLaye本 = GetActi正eLaye本();
    if (!Acti正eLaye本  Acti正eLaye本->bLocked)
    {
        本et使本n;
    }
    
    // 這裡需要實際的畫筆應用邏輯
    // 簡化版本：只是記錄操作
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied b本使sh st本oke at (%.1f, %.1f)"), Position.X, Position.Y);
}

正oid UMin成GoRTSA本tEdito本::BlendLaye本s()
{
    // 這裡需要實際的圖層混合邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Blendin成 laye本s"));
}

正oid UMin成GoRTSA本tEdito本::的otifyA本tEdited()
{
    OnA本tEdited.B本oadcast(C使本本entI設置a成e);
}

正oid UMin成GoRTSA本tEdito本::的otifyLaye本Chan成ed(const 軍St本in成& Laye本的a設置e, int32 Laye本Index)
{
    OnLaye本Chan成ed.B本oadcast(Laye本的a設置e, Laye本Index);
}

正oid UMin成GoRTSA本tEdito本::的otifyToolChan成ed(EEditin成Tool 的ewTool)
{
    OnToolChan成ed.B本oadcast(的ewTool);
}

UText使本e2D* UMin成GoRTSA本tEdito本::C本eateLaye本Text使本e(int32 基本idth, int32 輸入ei成ht)
{
    本et使本n UText使本e2D::C本eateT本ansient(基本idth, 輸入ei成ht, P軍下B8G8R8A8);
}

正oid UMin成GoRTSA本tEdito本::P本ocessSelection()
{
    // 這裡需要實際的選擇處理邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocessin成 selection"));
}
