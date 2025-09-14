#include "Inventory/ItemDBSubsystem.h"

void UItemDBSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Cache.Reset();
}

void UItemDBSubsystem::Deinitialize()
{
    Cache.Reset();
    Super::Deinitialize();
}

bool UItemDBSubsystem::TryGetStaticData(const FItemKey& Key, FItemStaticData& OutData)
{
    if (!Key.IsValid() || !ItemDataTable)
    {
        return false;
    }

    // 1) 캐시 조회
    if (const FItemStaticData* Found = Cache.Find(Key.DefId))
    {
        OutData = *Found;
        return true;
    }

    // 2) DataTable에서 로드 (RowStruct가 FItemStaticData여야 함)
    static const FString Context(TEXT("ItemDBSubsystem::TryGetStaticData"));
    if (FItemStaticData* Row = ItemDataTable->FindRow<FItemStaticData>(Key.DefId, Context, /*bWarnIfMissing*/true))
    {
        Cache.Add(Key.DefId, *Row);
        OutData = *Row;
        return true;
    }

    return false;
}

void UItemDBSubsystem::Prefetch(const TArray<FItemKey>& Keys)
{
    // 간단 구현: 조회하면 TryGetStaticData() 내부에서 캐시됨
    // 고급 구현: 비동기 로드 + 아이콘 소프트참조 프리로드 등
    for (const FItemKey& K : Keys)
    {
        FItemStaticData Dummy;
        TryGetStaticData(K, Dummy);
    }
}

bool UItemDBSubsystem::BuildSlotView(int32 SlotIndex, const FInventorySlot& Slot, FInventorySlotView& OutView)
{
    OutView = FInventorySlotView{};
    OutView.SlotIndex = SlotIndex;
    OutView.Key = Slot.Key;
    OutView.Quantity = Slot.Quantity;

    if (!Slot.Key.IsValid())
    {
        // 빈 슬롯도 유효 뷰로 반환
        return true;
    }

    FItemStaticData Def;
    if (!TryGetStaticData(Slot.Key, Def))
    {
        return false; // 정의 없음 → UI에서 오류 상태로 표시 가능
    }

    // 정적 정의 기반 필드 채움
    OutView.DisplayName = Def.DisplayName;
    OutView.MaxStackSize = Def.MaxStackSize;
    OutView.bStackable = Def.bStackable;
    OutView.Icon = Def.Icon;

    // 인스턴스 상태 전달(필요한 것만)
    OutView.RemainingTime = Slot.InstanceState.RemainingTime;
    OutView.Durability = Slot.InstanceState.Durability;
    OutView.Magazine = Slot.InstanceState.Magazine;
    OutView.Reserve = Slot.InstanceState.Reserve;
    return true;
}

