// InventoryComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryType.h"
#include "Inventory.generated.h"

class UInputAction;
class UInputMappingContext;
class UInventoryWidgetBase;
class AHandItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotsChanged, const TArray<int32>&, ChangedSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHandItemAcquired, FName, DefId, AHandItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandItemReleased, FName, DefId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemConsumed, int32, SlotIndex, FItemKey, Key);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UInventory : public UActorComponent
{
    GENERATED_BODY()
    
/* Field */
public:
    UInventory();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
    int32 NumRows = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
    int32 NumCols = 8;

    UPROPERTY(VisibleAnywhere, Category="Inventory")
    TArray<FInventorySlot> Slots;

    UPROPERTY(BlueprintAssignable)
    FOnInventorySlotsChanged OnInventorySlotsChanged;

    UPROPERTY(BlueprintAssignable)
    FOnHandItemAcquired OnHandItemAcquired;

    UPROPERTY(BlueprintAssignable)
    FOnHandItemReleased OnHandItemReleased;

    UPROPERTY(BlueprintAssignable)
    FOnItemConsumed OnItemConsumed;
    
    UPROPERTY()
    class UGameInstance* p_GameInstance;

    UPROPERTY()
    class UItemDBSubsystem* p_DataBase;

private:
    // 상태(런타임 전용)
    UPROPERTY(Transient)
    TMap<FName /*DefId*/, AHandItem*> Instances;

    UPROPERTY(Transient)
    TSet<FName> InUseHandItems;


/* Method */
public:
    
    
    // ====== 조회 ======
    UFUNCTION(BlueprintCallable, Category="Inventory")
    void InitializeSlots(int32 InRows, int32 InCols);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool GetSlotView(int32 Index, FInventorySlotView& Out) const;

    // ====== 조작 ======
    UFUNCTION(BlueprintCallable, Category="Inventory")
    FInventoryOpResult AddItem(const FItemKey& Key, int32 Quantity, const FItemInstanceState& InStateOpt);
    
    FInventoryOpResult AddItem(const FItemKey& Key, int32 Quantity)
    {
        return AddItem(Key, Quantity, FItemInstanceState{});
    }

    UFUNCTION(BlueprintCallable, Category="Inventory")
    FInventoryOpResult MoveItem(int32 FromIndex, int32 ToIndex, int32 Quantity /*=0*/);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    FInventoryOpResult SplitStack(int32 FromIndex, int32 ToIndex, int32 SplitQuantity);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    FInventoryOpResult RemoveAt(int32 Index, int32 Quantity);

    // 사용/핫바 선택(외부 공개 엔트리)
    UFUNCTION(BlueprintCallable, Category="Inventory|Use")
    FInventoryOpResult UseItemAt(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category="Inventory|Hotbar")
    AHandItem* SelectHotbarSlot(int32 Hotkey);
    
protected:
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // 풀/라이프사이클
    void BuildHandInstancesPool();

    void ReleaseAllInUseHandItems();
    
    // 내부 유틸
    int32 IndexFromRC(int32 Row, int32 Col) const;

    const FInventorySlot& GetSlot(int32 Index) const;

    void GetAllKeys(TArray<FItemKey>& OutKeys) const;

    int32 FindFirstEmptySlot() const;

    int32 GetSlotIndexForHotkey(int32 Hotkey) const;

    AHandItem* AcquireHandItemByKey(const FItemKey& Key);

    AHandItem* AcquireHandItemByDefId(FName DefId);

    bool ReleaseHandItemByDefId(FName DefId);
    
    bool IsValidIndex(int32 Index) const { return Slots.IsValidIndex(Index); }

    int32 FindStackableSlot(const FItemKey& Key, int32& OutRemainingCapacity) const;

    void BroadcastChanged(const TArray<int32>& DirtyIndices);
};

