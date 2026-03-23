#incl使de "Min成RTSMe設置o本yMana成e本.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "輸入AL/Me設置o本yMisc.h"

UMin成RTSMe設置o本yMana成e本::UMin成RTSMe設置o本yMana成e本()
{
    bInitialized = false;
    Me設置o本yBase = n使llpt本;
    TotalMe設置o本ySize = 0;
    UsedMe設置o本y = 0;
    AllocatedBlockCo使nt = 0;
    AllocationSt本ate成y = EAllocationSt本ate成y::軍i本st軍it;
    Me設置o本y基本a本nin成Th本eshold = 100; // 100MB
    bA使toDef本a成設置entation = t本使e;
    Def本a成設置entationTh本eshold = 0.3f; // 30%
    bMe設置o本yLo成成in成 = false;
}

bool UMin成RTSMe設置o本yMana成e本::InitializeMe設置o本yMana成e本(int32 TotalSizeMB)
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Me設置o本y Mana成e本 al本eady initialized"));
        本et使本n t本使e;
    }

    TotalMe設置o本ySize = TotalSizeMB * 1024 * 1024; // 轉換為字節

    // 分配系統內存
    Me設置o本yBase = 軍Me設置o本y::Malloc(TotalMe設置o本ySize);
    if (!Me設置o本yBase)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to allocate syste設置 設置e設置o本y: %d MB"), TotalSizeMB);
        OnMe設置o本yE本本o本.B本oadcast(軍St本in成::P本intf(TEXT("軍ailed to allocate %d MB 設置e設置o本y"), TotalSizeMB));
        本et使本n false;
    }

    // 初始化內存池
    if (!InitializeMe設置o本yPools())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize 設置e設置o本y pools"));
        軍Me設置o本y::軍本ee(Me設置o本yBase);
        Me設置o本yBase = n使llpt本;
        本et使本n false;
    }

    // 初始化內存塊
    if (!InitializeMe設置o本yBlocks())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize 設置e設置o本y blocks"));
        Clean使pMe設置o本yPools();
        軍Me設置o本y::軍本ee(Me設置o本yBase);
        Me設置o本yBase = n使llpt本;
        本et使本n false;
    }

    bInitialized = t本使e;
    ResetStatistics();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y Mana成e本 initialized with %d MB"), TotalSizeMB);
    
    // 啟動內存監控定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            UpdateStatistics();
        });
    }

    本et使本n t本使e;
}

正oid UMin成RTSMe設置o本yMana成e本::Sh使tdownMe設置o本yMana成e本()
{
    if (!bInitialized)
    {
        本et使本n;
    }

    // 清理所有內存池
    Clean使pMe設置o本yPools();

    // 清理所有內存塊
    Clean使pMe設置o本yBlocks();

    // 釋放系統內存
    if (Me設置o本yBase)
    {
        軍Me設置o本y::軍本ee(Me設置o本yBase);
        Me設置o本yBase = n使llpt本;
    }

    bInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y Mana成e本 sh使tdown co設置pleted"));
}

正oid* UMin成RTSMe設置o本yMana成e本::AllocateMe設置o本y(const 軍Me設置o本yAllocationReq使est& Req使est)
{
    if (!bInitialized  Req使est.Size <= 0)
    {
        本et使本n n使llpt本;
    }

    正oid* Add本ess = n使llpt本;

    // 根據分配策略進行分配
    switch (AllocationSt本ate成y)
    {
        case EAllocationSt本ate成y::軍i本st軍it:
            Add本ess = Allocate軍i本st軍it(Req使est.Size, Req使est.Owne本ID);
            b本eak;
        
        case EAllocationSt本ate成y::Best軍it:
            Add本ess = AllocateBest軍it(Req使est.Size, Req使est.Owne本ID);
            b本eak;
        
        case EAllocationSt本ate成y::基本o本st軍it:
            Add本ess = Allocate基本o本st軍it(Req使est.Size, Req使est.Owne本ID);
            b本eak;
        
        case EAllocationSt本ate成y::的ext軍it:
            Add本ess = Allocate的ext軍it(Req使est.Size, Req使est.Owne本ID);
            b本eak;
        
        case EAllocationSt本ate成y::B使ddy:
            Add本ess = AllocateB使ddy(Req使est.Size, Req使est.Owne本ID);
            b本eak;
        
        case EAllocationSt本ate成y::Slab:
            Add本ess = Allocate軍本o設置Slab(Req使est.Size, Req使est.Owne本ID);
            b本eak;
        
        defa使lt:
            Add本ess = Allocate軍i本st軍it(Req使est.Size, Req使est.Owne本ID);
            b本eak;
    }

    if (Add本ess)
    {
        // 對齊內存
        Add本ess = Ali成nAdd本ess(Add本ess, Req使est.Ali成n設置ent);

        // 如果需要，清零內存
        if (Req使est.bZe本oMe設置o本y)
        {
            軍Me設置o本y::Me設置ze本o(Add本ess, Req使est.Size);
        }

        // 更新統計
        {
            軍ScopeLock Lock(&StatisticsC本iticalSection);
            Statistics.TotalAllocations++;
            Statistics.AllocatedBlocks++;
        }

        // 記錄日誌
        Lo成Me設置o本yOpe本ation(TEXT("Allocate"), Add本ess, Req使est.Size, Req使est.Owne本ID);

        // 廣播事件
        OnMe設置o本yAllocated.B本oadcast(Add本ess, Req使est.Size);
    }
    else
    {
        // 分配失敗
        軍St本in成 E本本o本Messa成e = 軍St本in成::P本intf(TEXT("軍ailed to allocate %d bytes fo本 %s"), 
                                         Req使est.Size, *Req使est.Owne本ID);
        OnMe設置o本yE本本o本.B本oadcast(E本本o本Messa成e);
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("%s"), *E本本o本Messa成e);
    }

    本et使本n Add本ess;
}

正oid* UMin成RTSMe設置o本yMana成e本::AllocateMe設置o本ySi設置ple(int32 Size, const 軍St本in成& Owne本ID)
{
    軍Me設置o本yAllocationReq使est Req使est;
    Req使est.Size = Size;
    Req使est.Owne本ID = Owne本ID;
    Req使est.Ali成n設置ent = 4;
    Req使est.bZe本oMe設置o本y = false;
    Req使est.P本io本ity = 0;

    本et使本n AllocateMe設置o本y(Req使est);
}

bool UMin成RTSMe設置o本yMana成e本::DeallocateMe設置o本y(正oid* Add本ess)
{
    if (!bInitialized  !Add本ess)
    {
        本et使本n false;
    }

    bool S使ccess = DeallocateMe設置o本yBlock(Add本ess);

    if (S使ccess)
    {
        // 更新統計
        {
            軍ScopeLock Lock(&StatisticsC本iticalSection);
            Statistics.TotalDeallocations++;
            Statistics.AllocatedBlocks--;
        }

        // 記錄日誌
        Lo成Me設置o本yOpe本ation(TEXT("Deallocate"), Add本ess, 0, TEXT(""));

        // 廣播事件
        OnMe設置o本yDeallocated.B本oadcast(Add本ess, TEXT(""));
    }

    本et使本n S使ccess;
}

bool UMin成RTSMe設置o本yMana成e本::ReallocateMe設置o本y(正oid* Add本ess, int32 的ewSize)
{
    if (!bInitialized  !Add本ess  的ewSize <= 0)
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&Me設置o本yC本iticalSection);

    if (軍Me設置o本yBlock* Block = Me設置o本yBlocks.軍ind(Add本ess))
    {
        // 簡單實現：分配新內存，複製數據，釋放舊內存
        正oid* 的ewAdd本ess = AllocateMe設置o本ySi設置ple(的ewSize, Block->Owne本ID);
        if (的ewAdd本ess)
        {
            // 複製數據
            int32 CopySize = 軍Math::Min(Block->Size, 的ewSize);
            軍Me設置o本y::Me設置cpy(的ewAdd本ess, Add本ess, CopySize);

            // 釋放舊內存
            DeallocateMe設置o本y(Add本ess);

            UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y 本eallocated f本o設置 %d to %d bytes"), Block->Size, 的ewSize);
            本et使本n t本使e;
        }
    }

    本et使本n false;
}

bool UMin成RTSMe設置o本yMana成e本::C本eateMe設置o本yPool(int32 BlockSize, int32 BlockCo使nt)
{
    軍ScopeLock Lock(&PoolC本iticalSection);
    本et使本n C本eatePool(BlockSize, BlockCo使nt);
}

正oid* UMin成RTSMe設置o本yMana成e本::Allocate軍本o設置Pool(int32 BlockSize)
{
    軍ScopeLock Lock(&PoolC本iticalSection);
    
    if (軍Me設置o本yPool* Pool = 軍indS使itablePool(BlockSize))
    {
        if (Pool->軍本eeList.的使設置() > 0)
        {
            正oid* Add本ess = Pool->軍本eeList.Pop();
            Pool->軍本eeBlocks--;
            本et使本n Add本ess;
        }
    }

    本et使本n n使llpt本;
}

bool UMin成RTSMe設置o本yMana成e本::DeallocateToPool(正oid* Add本ess, int32 BlockSize)
{
    軍ScopeLock Lock(&PoolC本iticalSection);
    
    if (軍Me設置o本yPool* Pool = 軍indS使itablePool(BlockSize))
    {
        Pool->軍本eeList.Add(Add本ess);
        Pool->軍本eeBlocks++;
        本et使本n t本使e;
    }

    本et使本n false;
}

bool UMin成RTSMe設置o本yMana成e本::CopyMe設置o本y(正oid* Dest, const 正oid* S本c, int32 Size)
{
    if (!Dest  !S本c  Size <= 0)
    {
        本et使本n false;
    }

    軍Me設置o本y::Me設置cpy(Dest, S本c, Size);
    本et使本n t本使e;
}

bool UMin成RTSMe設置o本yMana成e本::SetMe設置o本y(正oid* Dest, int32 Val使e, int32 Size)
{
    if (!Dest  Size <= 0)
    {
        本et使本n false;
    }

    軍Me設置o本y::Me設置set(Dest, Val使e, Size);
    本et使本n t本使e;
}

bool UMin成RTSMe設置o本yMana成e本::Ze本oMe設置o本y(正oid* Dest, int32 Size)
{
    if (!Dest  Size <= 0)
    {
        本et使本n false;
    }

    軍Me設置o本y::Me設置ze本o(Dest, Size);
    本et使本n t本使e;
}

bool UMin成RTSMe設置o本yMana成e本::P本otectMe設置o本y(正oid* Add本ess, int32 Size, bool bRead, bool b基本本ite, bool bExec使te)
{
    // 簡化實現，實際應該調用平台特定的內存保護API
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y p本otection 本eq使ested fo本 add本ess %p, size %d"), Add本ess, Size);
    本et使本n t本使e;
}

bool UMin成RTSMe設置o本yMana成e本::Unp本otectMe設置o本y(正oid* Add本ess)
{
    // 簡化實現
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y p本otection 本e設置o正ed fo本 add本ess %p"), Add本ess);
    本et使本n t本使e;
}

bool UMin成RTSMe設置o本yMana成e本::Def本a成設置entMe設置o本y()
{
    if (!bInitialized)
    {
        本et使本n false;
    }

    Pe本fo本設置Def本a成設置entation();
    本et使本n t本使e;
}

正oid UMin成RTSMe設置o本yMana成e本::SetA使toDef本a成設置entation(bool bEnabled, float Th本esholdRatio)
{
    bA使toDef本a成設置entation = bEnabled;
    Def本a成設置entationTh本eshold = Th本esholdRatio;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to def本a成設置entation %s, th本eshold: %.2f"), 
           bEnabled 基本 TEXT("enabled") : TEXT("disabled"), Th本esholdRatio);
}

軍Me設置o本yStatistics UMin成RTSMe設置o本yMana成e本::GetMe設置o本yStatistics() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics;
}

int32 UMin成RTSMe設置o本yMana成e本::Get軍本eeMe設置o本y() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics.軍本eeMe設置o本y;
}

int32 UMin成RTSMe設置o本yMana成e本::GetUsedMe設置o本y() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics.UsedMe設置o本y;
}

float UMin成RTSMe設置o本yMana成e本::Get軍本a成設置entationRatio() const
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    本et使本n Statistics.軍本a成設置entationRatio;
}

TA本本ay<軍Me設置o本yBlock> UMin成RTSMe設置o本yMana成e本::GetMe設置o本yBlocks() const
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    TA本本ay<軍Me設置o本yBlock> Blocks;
    fo本 (const a使to& BlockPai本 : Me設置o本yBlocks)
    {
        Blocks.Add(BlockPai本.Val使e);
    }
    
    本et使本n Blocks;
}

正oid UMin成RTSMe設置o本yMana成e本::SetAllocationSt本ate成y(EAllocationSt本ate成y St本ate成y)
{
    AllocationSt本ate成y = St本ate成y;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Allocation st本ate成y chan成ed to %d"), (int32)St本ate成y);
}

正oid UMin成RTSMe設置o本yMana成e本::SetMe設置o本y基本a本nin成Th本eshold(int32 Th本esholdMB)
{
    Me設置o本y基本a本nin成Th本eshold = Th本esholdMB;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y wa本nin成 th本eshold set to %d MB"), Th本esholdMB);
}

正oid UMin成RTSMe設置o本yMana成e本::EnableMe設置o本yLo成成in成(bool bEnabled)
{
    bMe設置o本yLo成成in成 = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y lo成成in成 %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid* UMin成RTSMe設置o本yMana成e本::Allocate軍i本st軍it(int32 Size, const 軍St本in成& Owne本ID)
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    fo本 (a使to& BlockPai本 : Me設置o本yBlocks)
    {
        軍Me設置o本yBlock& Block = BlockPai本.Val使e;
        if (Block.State == EMe設置o本yBlockState::軍本ee && Block.Size >= Size)
        {
            Block.State = EMe設置o本yBlockState::Allocated;
            Block.Owne本ID = Owne本ID;
            Block.AllocationTi設置e = 軍DateTi設置e::的ow();
            Block.RefCo使nt = 1;
            
            UsedMe設置o本y += Size;
            本et使本n Block.BaseAdd本ess;
        }
    }
    
    本et使本n n使llpt本;
}

正oid* UMin成RTSMe設置o本yMana成e本::AllocateBest軍it(int32 Size, const 軍St本in成& Owne本ID)
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    正oid* Best軍itAdd本ess = n使llpt本;
    int32 Best軍itSize = I的T下MAX;
    
    fo本 (a使to& BlockPai本 : Me設置o本yBlocks)
    {
        軍Me設置o本yBlock& Block = BlockPai本.Val使e;
        if (Block.State == EMe設置o本yBlockState::軍本ee && 
            Block.Size >= Size && 
            Block.Size < Best軍itSize)
        {
            Best軍itSize = Block.Size;
            Best軍itAdd本ess = Block.BaseAdd本ess;
        }
    }
    
    if (Best軍itAdd本ess)
    {
        if (軍Me設置o本yBlock* Block = Me設置o本yBlocks.軍ind(Best軍itAdd本ess))
        {
            Block->State = EMe設置o本yBlockState::Allocated;
            Block->Owne本ID = Owne本ID;
            Block->AllocationTi設置e = 軍DateTi設置e::的ow();
            Block->RefCo使nt = 1;
            UsedMe設置o本y += Size;
        }
    }
    
    本et使本n Best軍itAdd本ess;
}

正oid* UMin成RTSMe設置o本yMana成e本::Allocate基本o本st軍it(int32 Size, const 軍St本in成& Owne本ID)
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    正oid* 基本o本st軍itAdd本ess = n使llpt本;
    int32 基本o本st軍itSize = 0;
    
    fo本 (a使to& BlockPai本 : Me設置o本yBlocks)
    {
        軍Me設置o本yBlock& Block = BlockPai本.Val使e;
        if (Block.State == EMe設置o本yBlockState::軍本ee && 
            Block.Size >= Size && 
            Block.Size > 基本o本st軍itSize)
        {
            基本o本st軍itSize = Block.Size;
            基本o本st軍itAdd本ess = Block.BaseAdd本ess;
        }
    }
    
    if (基本o本st軍itAdd本ess)
    {
        if (軍Me設置o本yBlock* Block = Me設置o本yBlocks.軍ind(基本o本st軍itAdd本ess))
        {
            Block->State = EMe設置o本yBlockState::Allocated;
            Block->Owne本ID = Owne本ID;
            Block->AllocationTi設置e = 軍DateTi設置e::的ow();
            Block->RefCo使nt = 1;
            UsedMe設置o本y += Size;
        }
    }
    
    本et使本n 基本o本st軍itAdd本ess;
}

正oid* UMin成RTSMe設置o本yMana成e本::Allocate的ext軍it(int32 Size, const 軍St本in成& Owne本ID)
{
    // 簡化實現，與軍i本st軍it相同
    本et使本n Allocate軍i本st軍it(Size, Owne本ID);
}

正oid* UMin成RTSMe設置o本yMana成e本::AllocateB使ddy(int32 Size, const 軍St本in成& Owne本ID)
{
    // 簡化的伴侶系統實現
    本et使本n AllocateBest軍it(Size, Owne本ID);
}

正oid* UMin成RTSMe設置o本yMana成e本::Allocate軍本o設置Slab(int32 Size, const 軍St本in成& Owne本ID)
{
    // 嘗試從內存池分配
    本et使本n Allocate軍本o設置Pool(Size);
}

bool UMin成RTSMe設置o本yMana成e本::DeallocateMe設置o本yBlock(正oid* Add本ess)
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    if (軍Me設置o本yBlock* Block = Me設置o本yBlocks.軍ind(Add本ess))
    {
        if (Block->State == EMe設置o本yBlockState::Allocated)
        {
            Block->RefCo使nt--;
            if (Block->RefCo使nt <= 0)
            {
                Block->State = EMe設置o本yBlockState::軍本ee;
                Block->Owne本ID = TEXT("");
                UsedMe設置o本y -= Block->Size;
                
                // 嘗試合併相鄰的空閒塊
                CoalesceMe設置o本y();
            }
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成RTSMe設置o本yMana成e本::CoalesceMe設置o本y()
{
    // 簡化的內存合併實現
    // 實際實現中應該檢查相鄰的空閒塊並合併它們
}

正oid UMin成RTSMe設置o本yMana成e本::UpdateStatistics()
{
    if (!bInitialized)
    {
        本et使本n;
    }

    軍ScopeLock Lock(&StatisticsC本iticalSection);
    
    // 計算內存統計
    Statistics.TotalMe設置o本y = TotalMe設置o本ySize;
    Statistics.UsedMe設置o本y = UsedMe設置o本y;
    Statistics.軍本eeMe設置o本y = TotalMe設置o本ySize - UsedMe設置o本y;
    Statistics.AllocatedBlocks = AllocatedBlockCo使nt;
    Statistics.軍本eeBlocks = Me設置o本yBlocks.的使設置() - AllocatedBlockCo使nt;
    
    // 計算碎片化程度
    if (Statistics.TotalMe設置o本y > 0)
    {
        Statistics.軍本a成設置entationRatio = (float)Statistics.軍本a成設置entedMe設置o本y / Statistics.TotalMe設置o本y;
    }
    
    // 計算平均分配大小
    if (Statistics.TotalAllocations > 0)
    {
        Statistics.A正e本a成eAllocationSize = (float)Statistics.UsedMe設置o本y / Statistics.TotalAllocations;
    }
    
    // 檢查內存警告
    int32 軍本eeMe設置o本yMB = Statistics.軍本eeMe設置o本y / (1024 * 1024);
    if (軍本eeMe設置o本yMB < Me設置o本y基本a本nin成Th本eshold)
    {
        OnMe設置o本y基本a本nin成.B本oadcast(軍本eeMe設置o本yMB);
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Low 設置e設置o本y wa本nin成: %d MB f本ee"), 軍本eeMe設置o本yMB);
    }
    
    // 自動碎片整理
    if (bA使toDef本a成設置entation && Statistics.軍本a成設置entationRatio > Def本a成設置entationTh本eshold)
    {
        Pe本fo本設置Def本a成設置entation();
    }
    
    // 繼續更新
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            UpdateStatistics();
        });
    }
}

bool UMin成RTSMe設置o本yMana成e本::IsMe設置o本yValid(正oid* Add本ess) const
{
    if (!Add本ess  !bInitialized)
    {
        本et使本n false;
    }

    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    if (const 軍Me設置o本yBlock* Block = Me設置o本yBlocks.軍ind(Add本ess))
    {
        本et使本n CheckMe設置o本yCo本本使ption(*Block);
    }
    
    本et使本n false;
}

正oid UMin成RTSMe設置o本yMana成e本::Lo成Me設置o本yOpe本ation(const 軍St本in成& Ope本ation, 正oid* Add本ess, int32 Size, const 軍St本in成& Owne本ID)
{
    if (bMe設置o本yLo成成in成)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y %s: Add本ess=%p, Size=%d, Owne本=%s"), 
               *Ope本ation, Add本ess, Size, *Owne本ID);
    }
}

正oid UMin成RTSMe設置o本yMana成e本::Pe本fo本設置Def本a成設置entation()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 設置e設置o本y def本a成設置entation"));
    
    TA本本ay<軍Me設置o本yBlock> 軍本a成設置entedBlocks = 軍ind軍本a成設置entedBlocks();
    
    // 簡化的碎片整理實現
    // 實際實現中應該移動內存塊以減少碎片
    fo本 (軍Me設置o本yBlock& Block : 軍本a成設置entedBlocks)
    {
        // 模擬移動內存塊
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Def本a成設置entin成 block at %p, size %d"), Block.BaseAdd本ess, Block.Size);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y def本a成設置entation co設置pleted"));
}

正oid UMin成RTSMe設置o本yMana成e本::Mo正eMe設置o本yBlock(軍Me設置o本yBlock& Block, 正oid* 的ewAdd本ess)
{
    // 移動內存塊數據到新地址
    if (Block.BaseAdd本ess && 的ewAdd本ess && Block.State == EMe設置o本yBlockState::Allocated)
    {
        軍Me設置o本y::Me設置cpy(的ewAdd本ess, Block.BaseAdd本ess, Block.Size);
        Block.BaseAdd本ess = 的ewAdd本ess;
    }
}

TA本本ay<軍Me設置o本yBlock> UMin成RTSMe設置o本yMana成e本::軍ind軍本a成設置entedBlocks() const
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    
    TA本本ay<軍Me設置o本yBlock> 軍本a成設置entedBlocks;
    
    // 簡化的碎片檢測邏輯
    fo本 (const a使to& BlockPai本 : Me設置o本yBlocks)
    {
        const 軍Me設置o本yBlock& Block = BlockPai本.Val使e;
        if (Block.State == EMe設置o本yBlockState::軍本a成設置ented)
        {
            軍本a成設置entedBlocks.Add(Block);
        }
    }
    
    本et使本n 軍本a成設置entedBlocks;
}

軍Me設置o本yPool* UMin成RTSMe設置o本yMana成e本::軍indS使itablePool(int32 Size)
{
    fo本 (軍Me設置o本yPool& Pool : Me設置o本yPools)
    {
        if (Pool.BlockSize >= Size && Pool.軍本eeBlocks > 0)
        {
            本et使本n &Pool;
        }
    }
    
    本et使本n n使llpt本;
}

bool UMin成RTSMe設置o本yMana成e本::C本eatePool(int32 BlockSize, int32 BlockCo使nt)
{
    軍Me設置o本yPool Pool;
    Pool.BlockSize = BlockSize;
    Pool.BlockCo使nt = BlockCo使nt;
    Pool.軍本eeBlocks = BlockCo使nt;
    
    // 分配池內存
    int32 PoolSize = BlockSize * BlockCo使nt;
    正oid* PoolMe設置o本y = 軍Me設置o本y::Malloc(PoolSize);
    
    if (!PoolMe設置o本y)
    {
        本et使本n false;
    }
    
    // 初始化空閒列表
    使int8* Me設置o本yPt本 = (使int8*)PoolMe設置o本y;
    fo本 (int32 i = 0; i < BlockCo使nt; ++i)
    {
        Pool.軍本eeList.Add(Me設置o本yPt本 + i * BlockSize);
    }
    
    Me設置o本yPools.Add(Pool);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated 設置e設置o本y pool: block size=%d, co使nt=%d"), BlockSize, BlockCo使nt);
    
    本et使本n t本使e;
}

正oid UMin成RTSMe設置o本yMana成e本::Dest本oyPool(int32 BlockSize)
{
    fo本 (int32 i = Me設置o本yPools.的使設置() - 1; i >= 0; --i)
    {
        if (Me設置o本yPools[i].BlockSize == BlockSize)
        {
            // 釋放池內存（這裡需要跟蹤原始分配的地址）
            Me設置o本yPools.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Dest本oyed 設置e設置o本y pool with block size %d"), BlockSize);
            b本eak;
        }
    }
}

bool UMin成RTSMe設置o本yMana成e本::InitializeMe設置o本yPools()
{
    // 創建預定義的內存池
    C本eatePool(64, 100);    // 64字節塊
    C本eatePool(256, 50);    // 256字節塊
    C本eatePool(1024, 25);   // 1KB塊
    C本eatePool(4096, 10);   // 4KB塊
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y pools initialized"));
    本et使本n t本使e;
}

bool UMin成RTSMe設置o本yMana成e本::InitializeMe設置o本yBlocks()
{
    // 創建一個大的空閒內存塊
    軍Me設置o本yBlock MainBlock;
    MainBlock.BaseAdd本ess = Me設置o本yBase;
    MainBlock.Size = TotalMe設置o本ySize;
    MainBlock.State = EMe設置o本yBlockState::軍本ee;
    MainBlock.Owne本ID = TEXT("Syste設置");
    MainBlock.AllocationTi設置e = 軍DateTi設置e::的ow();
    MainBlock.RefCo使nt = 0;
    MainBlock.Ma成ic的使設置be本 = 0xDEADBEE軍;
    
    Me設置o本yBlocks.Add(Me設置o本yBase, MainBlock);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y blocks initialized"));
    本et使本n t本使e;
}

正oid UMin成RTSMe設置o本yMana成e本::Clean使pMe設置o本yPools()
{
    軍ScopeLock Lock(&PoolC本iticalSection);
    Me設置o本yPools.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y pools cleaned 使p"));
}

正oid UMin成RTSMe設置o本yMana成e本::Clean使pMe設置o本yBlocks()
{
    軍ScopeLock Lock(&Me設置o本yC本iticalSection);
    Me設置o本yBlocks.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y blocks cleaned 使p"));
}

正oid UMin成RTSMe設置o本yMana成e本::ResetStatistics()
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    Statistics = 軍Me設置o本yStatistics();
    Statistics.TotalMe設置o本y = TotalMe設置o本ySize;
    Statistics.軍本eeMe設置o本y = TotalMe設置o本ySize;
}

int32 UMin成RTSMe設置o本yMana成e本::Ali成nSize(int32 Size, int32 Ali成n設置ent)
{
    本et使本n (Size + Ali成n設置ent - 1) & 年(Ali成n設置ent - 1);
}

bool UMin成RTSMe設置o本yMana成e本::IsPowe本OfTwo(int32 Val使e) const
{
    本et使本n Val使e > 0 && (Val使e & (Val使e - 1)) == 0;
}

int32 UMin成RTSMe設置o本yMana成e本::的extPowe本OfTwo(int32 Val使e) const
{
    if (IsPowe本OfTwo(Val使e))
    {
        本et使本n Val使e;
    }
    
    int32 Powe本 = 1;
    while (Powe本 < Val使e)
    {
        Powe本 <<= 1;
    }
    
    本et使本n Powe本;
}

正oid* UMin成RTSMe設置o本yMana成e本::Ali成nAdd本ess(正oid* Add本ess, int32 Ali成n設置ent)
{
    使intpt本下t Add本 = (使intpt本下t)Add本ess;
    本et使本n (正oid*)((Add本 + Ali成n設置ent - 1) & 年(Ali成n設置ent - 1));
}

bool UMin成RTSMe設置o本yMana成e本::CheckMe設置o本yCo本本使ption(const 軍Me設置o本yBlock& Block) const
{
    本et使本n Block.Ma成ic的使設置be本 == 0xDEADBEE軍;
}
