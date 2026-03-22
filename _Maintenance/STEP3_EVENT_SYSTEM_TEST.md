# MingCore事件系統測試

## 目標
- 在乾淨環境中測試MingCore事件系統
- 驗證事件發布和訂閱功能
- 確認插件架構正常工作

## 測試內容
1. 創建事件測試類
2. 實現事件訂閱者
3. 測試事件發布
4. 驗證事件處理

## 測試代碼示例
```cpp
// 創建單位移動事件
auto UnitMovedEvent = MakeShared<MingUnitMovedEvent>();
UnitMovedEvent->UnitID = "TestUnit";
UnitMovedEvent->NewPosition = FVector(100, 100, 0);

// 發布事件
MingEventBus::Get().Publish(UnitMovedEvent);

// 訂閱事件
MingEventBus::Get().Subscribe<MingUnitMovedEvent>([](TSharedPtr<MingUnitMovedEvent> Event)
{
    UE_LOG(LogTemp, Warning, TEXT("Unit moved: %s"), *Event->UnitID);
});
```

## 預期結果
- 事件發布成功
- 訂閱者收到事件
- 無編譯錯誤
