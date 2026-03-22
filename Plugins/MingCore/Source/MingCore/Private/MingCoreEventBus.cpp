#include "MingCoreEventBus.h"
#include "MingCoreModule.h"
#include "HAL/ThreadSafeBool.h"
#include "Containers/Queue.h"
#include "Async/Async.h"

// 事件總線實現類
class FMingCoreEventBusImpl
{
public:
    static FMingCoreEventBusImpl& Get()
    {
        static FMingCoreEventBusImpl Instance;
        return Instance;
    }
    
    // 發布事件
    template<typename EventType>
    void PublishEventImpl(const EventType& Event)
    {
        // 根據優先級處理事件
        if (Event.Priority == IMingCoreEventBus::EventPriority::Critical)
        {
            // 關鍵事件立即處理
            ProcessEventImmediate(Event);
        }
        else
        {
            // 其他事件加入批處理隊列
            AddToBatchQueue(Event);
        }
    }
    
    // 訂閱事件
    template<typename EventType>
    void SubscribeImpl(UObject* Listener, TFunction<void(const EventType&)> Callback)
    {
        if (!Listener || !Callback)
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid subscription parameters"));
            return;
        }
        
        FEventSubscription Subscription;
        Subscription.Listener = Listener;
        Subscription.Callback = [Callback](const FMingCoreEvent& Event)
        {
            if (const EventType* TypedEvent = static_cast<const EventType*>(&Event))
            {
                Callback(*TypedEvent);
            }
        };
        
        // 檢查監聽器是否有效
        if (!Listener->IsValidLowLevel())
        {
            UE_LOG(LogTemp, Warning, TEXT("Attempting to subscribe with invalid listener"));
            return;
        }
        
        Subscribers.Add(EventType::StaticStruct()->GetFName(), Subscription);
        
        UE_LOG(LogTemp, Log, TEXT("Subscribed %s to event type %s"), 
               *Listener->GetName(), *EventType::StaticStruct()->GetFName().ToString());
    }
    
    // 取消訂閱
    template<typename EventType>
    void UnsubscribeImpl(UObject* Listener)
    {
        if (!Listener)
        {
            return;
        }
        
        FName EventTypeName = EventType::StaticStruct()->GetFName();
        
        // 移除特定監聽器的訂閱
        for (auto It = Subscribers.CreateIterator(); It; ++It)
        {
            if (It->Key == EventTypeName && It->Value.Listener == Listener)
            {
                It.RemoveCurrent();
            }
        }
        
        UE_LOG(LogTemp, Log, TEXT("Unsubscribed %s from event type %s"), 
               *Listener->GetName(), *EventTypeName.ToString());
    }
    
    // 批處理事件發布
    void PublishBatchEventsImpl(const TArray<FMingCoreEvent>& Events)
    {
        // 按優先級排序事件
        TArray<FMingCoreEvent> SortedEvents = Events;
        SortedEvents.Sort([](const FMingCoreEvent& A, const FMingCoreEvent& B)
        {
            return A.Priority < B.Priority; // 數字越小優先級越高
        });
        
        // 處理排序後的事件
        for (const FMingCoreEvent& Event : SortedEvents)
        {
            ProcessEventImmediate(Event);
        }
    }
    
    // 處理單個事件
    void ProcessEventImmediate(const FMingCoreEvent& Event)
    {
        FName EventTypeName = Event.GetStruct()->GetFName();
        
        // 查找並通知所有訂閱者
        for (auto It = Subscribers.CreateIterator(); It; ++It)
        {
            if (It->Key == EventTypeName)
            {
                const FEventSubscription& Subscription = It->Value;
                
                // 檢查監聽器是否仍然有效
                if (Subscription.Listener && Subscription.Listener->IsValidLowLevel())
                {
                    // 在Game Thread中執行回調
                    AsyncTask(ENamedThreads::GameThread, [this, Subscription, Event]()
                    {
                        if (Subscription.Listener && Subscription.Listener->IsValidLowLevel())
                        {
                            Subscription.Callback(Event);
                        }
                    });
                }
                else
                {
                    // 移除無效的訂閱
                    It.RemoveCurrent();
                }
            }
        }
    }
    
    // 清理無效訂閱（定期調用）
    void CleanupInvalidSubscriptions()
    {
        for (auto It = Subscribers.CreateIterator(); It; ++It)
        {
            if (!It->Value.Listener || !It->Value.Listener->IsValidLowLevel())
            {
                It.RemoveCurrent();
            }
        }
    }
    
private:
    FMingCoreEventBusImpl()
    {
        // 註冊清理任務
        FCoreDelegates::OnEndFrame.AddRaw(this, &FMingCoreEventBusImpl::OnEndFrame);
    }
    
    ~FMingCoreEventBusImpl()
    {
        FCoreDelegates::OnEndFrame.RemoveRaw(this, &FMingCoreEventBusImpl::OnEndFrame);
    }
    
    // 添加事件到批處理隊列
    void AddToBatchQueue(const FMingCoreEvent& Event)
    {
        FScopeLock Lock(&BatchQueueMutex);
        BatchQueue.Enqueue(Event);
    }
    
    // 處理批處理隊列
    void ProcessBatchQueue()
    {
        FScopeLock Lock(&BatchQueueMutex);
        
        TArray<FMingCoreEvent> EventsToProcess;
        while (!BatchQueue.IsEmpty())
        {
            FMingCoreEvent Event;
            BatchQueue.Dequeue(Event);
            EventsToProcess.Add(Event);
        }
        
        Lock.Unlock();
        
        if (!EventsToProcess.IsEmpty())
        {
            PublishBatchEventsImpl(EventsToProcess);
        }
    }
    
    // 幀結束回調
    void OnEndFrame()
    {
        // 每幀處理批處理隊列
        ProcessBatchQueue();
        
        // 定期清理無效訂閱
        static int32 FrameCount = 0;
        if (++FrameCount >= 60) // 每60幀清理一次
        {
            CleanupInvalidSubscriptions();
            FrameCount = 0;
        }
    }
    
    // 事件訂閱結構
    struct FEventSubscription
    {
        TObjectPtr<UObject> Listener;
        TFunction<void(const FMingCoreEvent&)> Callback;
    };
    
    // 訂閱者映射
    TMap<FName, FEventSubscription> Subscribers;
    
    // 批處理隊列
    TQueue<FMingCoreEvent> BatchQueue;
    
    // 線程安全
    mutable FCriticalSection BatchQueueMutex;
};

// 模板實現
template<typename EventType>
void IMingCoreEventBus::PublishEvent(const EventType& Event)
{
    FMingCoreEventBusImpl::Get().PublishEventImpl(Event);
}

template<typename EventType>
void IMingCoreEventBus::Subscribe(UObject* Listener, TFunction<void(const EventType&)> Callback)
{
    FMingCoreEventBusImpl::Get().SubscribeImpl<EventType>(Listener, Callback);
}

template<typename EventType>
void IMingCoreEventBus::Unsubscribe(UObject* Listener)
{
    FMingCoreEventBusImpl::Get().UnsubscribeImpl<EventType>(Listener);
}

// 批處理事件發布實現
void IMingCoreEventBus::PublishBatchEvents(const TArray<FMingCoreEvent>& Events)
{
    FMingCoreEventBusImpl::Get().PublishBatchEventsImpl(Events);
}
