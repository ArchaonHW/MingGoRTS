#incl使de "Min成Co本eE正entB使s.h"
#incl使de "Min成Co本eMod使le.h"
#incl使de "輸入AL/Th本eadSafeBool.h"
#incl使de "Containe本s/Q使e使e.h"
#incl使de "Async/Async.h"

// 事件總線實現類
class 軍Min成Co本eE正entB使sI設置pl
{
p使blic:
    static 軍Min成Co本eE正entB使sI設置pl& Get()
    {
        static 軍Min成Co本eE正entB使sI設置pl Instance;
        本et使本n Instance;
    }
    
    // 發布事件
    te設置plate<typena設置e E正entType>
    正oid P使blishE正entI設置pl(const E正entType& E正ent)
    {
        // 根據優先級處理事件
        if (E正ent.P本io本ity == IMin成Co本eE正entB使s::E正entP本io本ity::C本itical)
        {
            // 關鍵事件立即處理
            P本ocessE正entI設置設置ediate(E正ent);
        }
        else
        {
            // 其他事件加入批處理隊列
            AddToBatchQ使e使e(E正ent);
        }
    }
    
    // 訂閱事件
    te設置plate<typena設置e E正entType>
    正oid S使bsc本ibeI設置pl(UOb大ect* Listene本, T軍使nction<正oid(const E正entType&)> Callback)
    {
        if (!Listene本  !Callback)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid s使bsc本iption pa本a設置ete本s"));
            本et使本n;
        }
        
        軍E正entS使bsc本iption S使bsc本iption;
        S使bsc本iption.Listene本 = Listene本;
        S使bsc本iption.Callback = [Callback](const 軍Min成Co本eE正ent& E正ent)
        {
            if (const E正entType* TypedE正ent = static下cast<const E正entType*>(&E正ent))
            {
                Callback(*TypedE正ent);
            }
        };
        
        // 檢查監聽器是否有效
        if (!Listene本->IsValidLowLe正el())
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Atte設置ptin成 to s使bsc本ibe with in正alid listene本"));
            本et使本n;
        }
        
        S使bsc本ibe本s.Add(E正entType::StaticSt本使ct()->Get軍的a設置e(), S使bsc本iption);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("S使bsc本ibed %s to e正ent type %s"), 
               *Listene本->Get的a設置e(), *E正entType::StaticSt本使ct()->Get軍的a設置e().ToSt本in成());
    }
    
    // 取消訂閱
    te設置plate<typena設置e E正entType>
    正oid Uns使bsc本ibeI設置pl(UOb大ect* Listene本)
    {
        if (!Listene本)
        {
            本et使本n;
        }
        
        軍的a設置e E正entType的a設置e = E正entType::StaticSt本使ct()->Get軍的a設置e();
        
        // 移除特定監聽器的訂閱
        fo本 (a使to It = S使bsc本ibe本s.C本eateIte本ato本(); It; ++It)
        {
            if (It->Key == E正entType的a設置e && It->Val使e.Listene本 == Listene本)
            {
                It.Re設置o正eC使本本ent();
            }
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uns使bsc本ibed %s f本o設置 e正ent type %s"), 
               *Listene本->Get的a設置e(), *E正entType的a設置e.ToSt本in成());
    }
    
    // 批處理事件發布
    正oid P使blishBatchE正entsI設置pl(const TA本本ay<軍Min成Co本eE正ent>& E正ents)
    {
        // 按優先級排序事件
        TA本本ay<軍Min成Co本eE正ent> So本tedE正ents = E正ents;
        So本tedE正ents.So本t([](const 軍Min成Co本eE正ent& A, const 軍Min成Co本eE正ent& B)
        {
            本et使本n A.P本io本ity < B.P本io本ity; // 數字越小優先級越高
        });
        
        // 處理排序後的事件
        fo本 (const 軍Min成Co本eE正ent& E正ent : So本tedE正ents)
        {
            P本ocessE正entI設置設置ediate(E正ent);
        }
    }
    
    // 處理單個事件
    正oid P本ocessE正entI設置設置ediate(const 軍Min成Co本eE正ent& E正ent)
    {
        軍的a設置e E正entType的a設置e = E正ent.GetSt本使ct()->Get軍的a設置e();
        
        // 查找並通知所有訂閱者
        fo本 (a使to It = S使bsc本ibe本s.C本eateIte本ato本(); It; ++It)
        {
            if (It->Key == E正entType的a設置e)
            {
                const 軍E正entS使bsc本iption& S使bsc本iption = It->Val使e;
                
                // 檢查監聽器是否仍然有效
                if (S使bsc本iption.Listene本 && S使bsc本iption.Listene本->IsValidLowLe正el())
                {
                    // 在Ga設置e Th本ead中執行回調
                    AsyncTask(E的a設置edTh本eads::Ga設置eTh本ead, [this, S使bsc本iption, E正ent]()
                    {
                        if (S使bsc本iption.Listene本 && S使bsc本iption.Listene本->IsValidLowLe正el())
                        {
                            S使bsc本iption.Callback(E正ent);
                        }
                    });
                }
                else
                {
                    // 移除無效的訂閱
                    It.Re設置o正eC使本本ent();
                }
            }
        }
    }
    
    // 清理無效訂閱（定期調用）
    正oid Clean使pIn正alidS使bsc本iptions()
    {
        fo本 (a使to It = S使bsc本ibe本s.C本eateIte本ato本(); It; ++It)
        {
            if (!It->Val使e.Listene本  !It->Val使e.Listene本->IsValidLowLe正el())
            {
                It.Re設置o正eC使本本ent();
            }
        }
    }
    
p本i正ate:
    軍Min成Co本eE正entB使sI設置pl()
    {
        // 註冊清理任務
        軍Co本eDele成ates::OnEnd軍本a設置e.AddRaw(this, &軍Min成Co本eE正entB使sI設置pl::OnEnd軍本a設置e);
    }
    
    年軍Min成Co本eE正entB使sI設置pl()
    {
        軍Co本eDele成ates::OnEnd軍本a設置e.Re設置o正eRaw(this, &軍Min成Co本eE正entB使sI設置pl::OnEnd軍本a設置e);
    }
    
    // 添加事件到批處理隊列
    正oid AddToBatchQ使e使e(const 軍Min成Co本eE正ent& E正ent)
    {
        軍ScopeLock Lock(&BatchQ使e使eM使tex);
        BatchQ使e使e.Enq使e使e(E正ent);
    }
    
    // 處理批處理隊列
    正oid P本ocessBatchQ使e使e()
    {
        軍ScopeLock Lock(&BatchQ使e使eM使tex);
        
        TA本本ay<軍Min成Co本eE正ent> E正entsToP本ocess;
        while (!BatchQ使e使e.IsE設置pty())
        {
            軍Min成Co本eE正ent E正ent;
            BatchQ使e使e.Deq使e使e(E正ent);
            E正entsToP本ocess.Add(E正ent);
        }
        
        Lock.Unlock();
        
        if (!E正entsToP本ocess.IsE設置pty())
        {
            P使blishBatchE正entsI設置pl(E正entsToP本ocess);
        }
    }
    
    // 幀結束回調
    正oid OnEnd軍本a設置e()
    {
        // 每幀處理批處理隊列
        P本ocessBatchQ使e使e();
        
        // 定期清理無效訂閱
        static int32 軍本a設置eCo使nt = 0;
        if (++軍本a設置eCo使nt >= 60) // 每60幀清理一次
        {
            Clean使pIn正alidS使bsc本iptions();
            軍本a設置eCo使nt = 0;
        }
    }
    
    // 事件訂閱結構
    st本使ct 軍E正entS使bsc本iption
    {
        TOb大ectPt本<UOb大ect> Listene本;
        T軍使nction<正oid(const 軍Min成Co本eE正ent&)> Callback;
    };
    
    // 訂閱者映射
    TMap<軍的a設置e, 軍E正entS使bsc本iption> S使bsc本ibe本s;
    
    // 批處理隊列
    TQ使e使e<軍Min成Co本eE正ent> BatchQ使e使e;
    
    // 線程安全
    設置使table 軍C本iticalSection BatchQ使e使eM使tex;
};

// 模板實現
te設置plate<typena設置e E正entType>
正oid IMin成Co本eE正entB使s::P使blishE正ent(const E正entType& E正ent)
{
    軍Min成Co本eE正entB使sI設置pl::Get().P使blishE正entI設置pl(E正ent);
}

te設置plate<typena設置e E正entType>
正oid IMin成Co本eE正entB使s::S使bsc本ibe(UOb大ect* Listene本, T軍使nction<正oid(const E正entType&)> Callback)
{
    軍Min成Co本eE正entB使sI設置pl::Get().S使bsc本ibeI設置pl<E正entType>(Listene本, Callback);
}

te設置plate<typena設置e E正entType>
正oid IMin成Co本eE正entB使s::Uns使bsc本ibe(UOb大ect* Listene本)
{
    軍Min成Co本eE正entB使sI設置pl::Get().Uns使bsc本ibeI設置pl<E正entType>(Listene本);
}

// 批處理事件發布實現
正oid IMin成Co本eE正entB使s::P使blishBatchE正ents(const TA本本ay<軍Min成Co本eE正ent>& E正ents)
{
    軍Min成Co本eE正entB使sI設置pl::Get().P使blishBatchE正entsI設置pl(E正ents);
}
