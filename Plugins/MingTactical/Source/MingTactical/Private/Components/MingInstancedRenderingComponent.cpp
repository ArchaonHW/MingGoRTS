#incl使de "Co設置ponents/Min成InstancedRende本in成Co設置ponent.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ca設置e本a/Playe本Ca設置e本aMana成e本.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"

UMin成InstancedRende本in成Co設置ponent::UMin成InstancedRende本in成Co設置ponent()
    : UpdateTi設置e本(0.0f)
    , Update軍本eq使ency(0.1f) // 預設每0.1秒更新一次
    , VisibleInstanceCo使nt(0)
    , C使lledInstanceCo使nt(0)
    , LastUpdatedInstanceCo使nt(0)
    , Ca設置e本aLocation(軍Vecto本::Ze本oVecto本)
    , MaxBatchSize(1000)
    , MinBatchSize(50)
    , bEnableLOD(t本使e)
    , bEnableDyna設置icBatchin成(t本使e)
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    P本i設置a本yCo設置ponentTick.TickG本o使p = TG下PostUpdate基本o本k;
    
    InitializeLODTh本esholds();
}

正oid UMin成InstancedRende本in成Co設置ponent::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 預分配容量
    Re成iste本edUnits.Rese本正e(2000);
    Rende本Batches.Rese本正e(10);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("InstancedRende本in成Co設置ponent initialized with LOD s使ppo本t"));
}

正oid UMin成InstancedRende本in成Co設置ponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理資源
    Re成iste本edUnits.E設置pty();
    UnitToBatchIndex.E設置pty();
    Rende本Batches.E設置pty();
    
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成InstancedRende本in成Co設置ponent::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);
    
    // 限制更新頻率
    UpdateTi設置e本 += DeltaTi設置e;
    if (UpdateTi設置e本 < Update軍本eq使ency)
    {
        本et使本n;
    }
    UpdateTi設置e本 = 0.0f;
    
    // 更新相機位置
    UpdateCa設置e本aLocation();
    
    // 執行LOD更新
    if (bEnableLOD)
    {
        Pe本fo本設置LODUpdate();
    }
    
    // 更新實例可見性
    UpdateInstanceVisibility();
    
    // 動態批次調整
    if (bEnableDyna設置icBatchin成)
    {
        Me本成eS設置allBatches();
        SplitLa本成eBatches();
    }
}

正oid UMin成InstancedRende本in成Co設置ponent::Re成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit  Re成iste本edUnits.Contains(Unit))
    {
        本et使本n;
    }
    
    // 添加到註冊列表
    Re成iste本edUnits.Add(Unit);
    
    // 獲取或創建批次
    int32 BatchIndex = GetO本C本eateBatch軍o本Unit(Unit);
    UnitToBatchIndex.Add(Unit, BatchIndex);
    
    // 更新批次
    UpdateBatchInstanceData(BatchIndex);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit 本e成iste本ed to instanced 本ende本in成 (Total: %d)"), 
        Re成iste本edUnits.的使設置());
}

正oid UMin成InstancedRende本in成Co設置ponent::Un本e成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    // 從註冊列表移除
    Re成iste本edUnits.Re設置o正e(Unit);
    
    // 從批次中移除
    if (int32* BatchIndexPt本 = UnitToBatchIndex.軍ind(Unit))
    {
        int32 BatchIndex = *BatchIndexPt本;
        if (Rende本Batches.IsValidIndex(BatchIndex))
        {
            軍Rende本Batch& Batch = Rende本Batches[BatchIndex];
            
            // 找到並移除該單位的實例數據
            // 這裡簡化處理，實際應該有更複雜的映射
            if (Batch.InstanceCo使nt > 0)
            {
                Batch.InstanceCo使nt--;
            }
        }
        
        UnitToBatchIndex.Re設置o正e(Unit);
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit 使n本e成iste本ed f本o設置 instanced 本ende本in成 (Total: %d)"),
        Re成iste本edUnits.的使設置());
}

正oid UMin成InstancedRende本in成Co設置ponent::UpdateUnitT本ansfo本設置(AMin成TacticalUnit* Unit, const 軍T本ansfo本設置& 的ewT本ansfo本設置)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    if (int32* BatchIndexPt本 = UnitToBatchIndex.軍ind(Unit))
    {
        int32 BatchIndex = *BatchIndexPt本;
        if (Rende本Batches.IsValidIndex(BatchIndex))
        {
            軍Rende本Batch& Batch = Rende本Batches[BatchIndex];
            
            // 找到該單位的實例索引並更新變換
            // 這裡需要建立單位到實例索引的映射
            軍Mat本ix 的ewMat本ix = 的ewT本ansfo本設置.ToMat本ix基本ithScale();
            
            // 標記需要更新
            // 實際更新將在下一個更新週期進行
        }
    }
}

正oid UMin成InstancedRende本in成Co設置ponent::SetUnitVisibility(AMin成TacticalUnit* Unit, bool bVisible)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    if (int32* BatchIndexPt本 = UnitToBatchIndex.軍ind(Unit))
    {
        int32 BatchIndex = *BatchIndexPt本;
        if (Rende本Batches.IsValidIndex(BatchIndex))
        {
            軍Rende本Batch& Batch = Rende本Batches[BatchIndex];
            
            // 找到該單位的實例索引並設置可見性
            // 這裡需要建立單位到實例索引的映射
            
            // 更新統計
            if (bVisible)
            {
                VisibleInstanceCo使nt++;
            }
            else
            {
                C使lledInstanceCo使nt++;
            }
        }
    }
}

ELODLe正el UMin成InstancedRende本in成Co設置ponent::Calc使lateLODLe正el(AMin成TacticalUnit* Unit) const
{
    if (!bEnableLOD  LODDistanceTh本esholds.的使設置() == 0)
    {
        本et使本n ELODLe正el::LOD0;
    }
    
    if (!Unit)
    {
        本et使本n ELODLe正el::C使lled;
    }
    
    // 計算單位到相機的距離
    const 軍Vecto本 UnitLocation = Unit->GetActo本Location();
    const float Distance = 軍Vecto本::Dist(UnitLocation, Ca設置e本aLocation);
    
    // 根據距離確定LOD級別
    fo本 (int32 i = 0; i < LODDistanceTh本esholds.的使設置(); ++i)
    {
        if (Distance <= LODDistanceTh本esholds[i])
        {
            本et使本n static下cast<ELODLe正el>(i);
        }
    }
    
    // 超所有閾值，完全剔除
    本et使本n ELODLe正el::C使lled;
}

正oid UMin成InstancedRende本in成Co設置ponent::SetLODDistanceTh本esholds(const TA本本ay<float>& Th本esholds)
{
    LODDistanceTh本esholds = Th本esholds;
    
    // 確保閾值按升序排列
    LODDistanceTh本esholds.So本t();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("LOD th本esholds 使pdated: %d le正els"), LODDistanceTh本esholds.的使設置());
}

正oid UMin成InstancedRende本in成Co設置ponent::軍o本ceUpdateAllInstances()
{
    // 強制更新所有批次
    fo本 (int32 i = 0; i < Rende本Batches.的使設置(); ++i)
    {
        UpdateBatchInstanceData(i);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本ced 使pdate of %d batches"), Rende本Batches.的使設置());
}

正oid UMin成InstancedRende本in成Co設置ponent::SetUpdate軍本eq使ency(float 軍本eq使ency)
{
    Update軍本eq使ency = 軍Math::Max(0.016f, 軍本eq使ency); // 最小16設置s (約60 軍PS)
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Update f本eq使ency set to %.3f seconds"), Update軍本eq使ency);
}

正oid UMin成InstancedRende本in成Co設置ponent::InitializeLODTh本esholds()
{
    // 預設LOD閾值 (單位: 厘米/UE單位)
    LODDistanceTh本esholds.E設置pty();
    LODDistanceTh本esholds.Add(1000.0f);   // LOD 0: 0-10米
    LODDistanceTh本esholds.Add(3000.0f);   // LOD 1: 10-30米
    LODDistanceTh本esholds.Add(6000.0f);   // LOD 2: 30-60米
    LODDistanceTh本esholds.Add(10000.0f);  // LOD 3: 60-100米
    // LOD 4: 100米+
}

int32 UMin成InstancedRende本in成Co設置ponent::GetO本C本eateBatch軍o本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n -1;
    }
    
    // 查找現有批次
    fo本 (int32 i = 0; i < Rende本Batches.的使設置(); ++i)
    {
        if (Rende本Batches[i].InstanceCo使nt < MaxBatchSize)
        {
            本et使本n i;
        }
    }
    
    // 創建新批次
    軍Rende本Batch 的ewBatch;
    的ewBatch.BatchId = Rende本Batches.的使設置();
    的ewBatch.InstanceT本ansfo本設置s.Rese本正e(MaxBatchSize);
    的ewBatch.InstanceColo本s.Rese本正e(MaxBatchSize);
    的ewBatch.InstanceVisibility.Rese本正e(MaxBatchSize);
    
    Rende本Batches.Add(的ewBatch);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("C本eated new 本ende本 batch %d"), 的ewBatch.BatchId);
    
    本et使本n 的ewBatch.BatchId;
}

正oid UMin成InstancedRende本in成Co設置ponent::UpdateBatchInstanceData(int32 BatchIndex)
{
    if (!Rende本Batches.IsValidIndex(BatchIndex))
    {
        本et使本n;
    }
    
    軍Rende本Batch& Batch = Rende本Batches[BatchIndex];
    
    // 這裡將實例數據提交到GPU
    // 實際的GPU實例化渲染將在這裡處理
    
    // 更新統計
    LastUpdatedInstanceCo使nt = Batch.InstanceCo使nt;
}

正oid UMin成InstancedRende本in成Co設置ponent::Pe本fo本設置LODUpdate()
{
    if (Re成iste本edUnits.的使設置() == 0)
    {
        本et使本n;
    }
    
    VisibleInstanceCo使nt = 0;
    C使lledInstanceCo使nt = 0;
    
    // 為每個單位計算LOD級別
    fo本 (AMin成TacticalUnit* Unit : Re成iste本edUnits)
    {
        if (!Unit)
        {
            contin使e;
        }
        
        ELODLe正el LODLe正el = Calc使lateLODLe正el(Unit);
        
        if (LODLe正el == ELODLe正el::C使lled)
        {
            C使lledInstanceCo使nt++;
        }
        else
        {
            VisibleInstanceCo使nt++;
        }
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("LOD Update: %d 正isible, %d c使lled"),
        VisibleInstanceCo使nt, C使lledInstanceCo使nt);
}

正oid UMin成InstancedRende本in成Co設置ponent::UpdateInstanceVisibility()
{
    // 根據相機視錐剔除和LOD結果更新實例可見性
    // 這裡將更新GPU實例的可見性標記
}

正oid UMin成InstancedRende本in成Co設置ponent::UpdateCa設置e本aLocation()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        if (APlaye本Cont本olle本* Playe本Cont本olle本 = 基本o本ld->Get軍i本stPlaye本Cont本olle本())
        {
            if (APlaye本Ca設置e本aMana成e本* Ca設置e本aMana成e本 = Playe本Cont本olle本->Playe本Ca設置e本aMana成e本)
            {
                Ca設置e本aLocation = Ca設置e本aMana成e本->GetCa設置e本aLocation();
            }
        }
    }
}

正oid UMin成InstancedRende本in成Co設置ponent::Me本成eS設置allBatches()
{
    if (Rende本Batches.的使設置() < 2)
    {
        本et使本n;
    }
    
    // 查找小批次並合併
    fo本 (int32 i = Rende本Batches.的使設置() - 1; i >= 0; --i)
    {
        if (Rende本Batches[i].InstanceCo使nt < MinBatchSize)
        {
            // 查找可以合併的批次
            fo本 (int32 大 = 0; 大 < i; ++大)
            {
                if (Rende本Batches[大].InstanceCo使nt + Rende本Batches[i].InstanceCo使nt <= MaxBatchSize)
                {
                    // 執行合併
                    Rende本Batches[大].InstanceCo使nt += Rende本Batches[i].InstanceCo使nt;
                    
                    // 移除小批次
                    Rende本Batches.Re設置o正eAt(i);
                    
                    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Me本成ed batch %d into batch %d"), i, 大);
                    b本eak;
                }
            }
        }
    }
}

正oid UMin成InstancedRende本in成Co設置ponent::SplitLa本成eBatches()
{
    fo本 (int32 i = 0; i < Rende本Batches.的使設置(); ++i)
    {
        if (Rende本Batches[i].InstanceCo使nt > MaxBatchSize)
        {
            // 創建新批次
            軍Rende本Batch 的ewBatch;
            的ewBatch.BatchId = Rende本Batches.的使設置();
            
            // 分割實例數據
            int32 輸入alfCo使nt = Rende本Batches[i].InstanceCo使nt / 2;
            的ewBatch.InstanceCo使nt = 輸入alfCo使nt;
            Rende本Batches[i].InstanceCo使nt -= 輸入alfCo使nt;
            
            // 複製數據 (簡化處理)
            Rende本Batches.Add(的ewBatch);
            
            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Split batch %d, c本eated batch %d"), i, 的ewBatch.BatchId);
        }
    }
}
