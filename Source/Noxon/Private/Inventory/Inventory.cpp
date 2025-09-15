#include "Inventory/Inventory.h"
#include "Inventory/ItemDBSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "GameplayTagsManager.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UnrealType.h"
#include "Items/HandItems/HandItem.h"
#include "UI/InventoryWidgetBase.h"

UInventory::UInventory()
{
	// ActorComponent의 틱 설정 필드는 PrimaryComponentTick
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventory::ConstructInventory()
{
	OwnCtrl = CastChecked<APlayerController>(GetOwner());
	
	InventoryMenu = CreateWidget<UInventoryWidgetBase>(OwnCtrl.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
}

void UInventory::InitializeSlots(int32 InRows, int32 InCols)
{
	NumRows = FMath::Max(1, InRows);
	NumCols = FMath::Max(1, InCols);

	const int32 Count = NumRows * NumCols;
	Slots.SetNum(Count);
	for (int32 i=0; i<Count; ++i)
	{
		Slots[i].SlotIndex = i;
		Slots[i].Clear();
	}

	// 인벤토리 열릴 때, 보유 키들 프리패치(아이콘 스트리밍) 하면 UX 좋아짐
	if (UItemDBSubsystem* DB = GetWorld()->GetGameInstance()->GetSubsystem<UItemDBSubsystem>())
	{
		TArray<FItemKey> Keys;
		GetAllKeys(Keys);
		DB->Prefetch(Keys);
	}

	// 초기 전체 리프레시를 원하면 모든 인덱스 브로드캐스트
	TArray<int32> AllIndices;
	AllIndices.Reserve(Count);
	for (int32 i=0; i<Count; ++i) { AllIndices.Add(i); }
	BroadcastChanged(AllIndices);
}

int32 UInventory::IndexFromRC(int32 Row, int32 Col) const
{
	if (Row < 0 || Col < 0 || Row >= NumRows || Col >= NumCols) return INDEX_NONE;
	return Row * NumCols + Col;
}

const FInventorySlot& UInventory::GetSlot(int32 Index) const
{
	// 주의: 호출측에서 IsValidIndex 체크 권장
	return Slots[Index];
}

bool UInventory::GetSlotView(int32 Index, FInventorySlotView& Out) const
{
    if (!IsValidIndex(Index)) return false;

    // 기본 뷰(빈 슬롯 표시용) 미리 채워둠
    Out = FInventorySlotView{};
    Out.SlotIndex = Index;
    Out.Key = Slots[Index].Key;
    Out.Quantity = Slots[Index].Quantity;

    // DB 서브시스템이 있으면 정적 정의까지 포함해 빌드
    if (UGameInstance* GI = GetWorld()->GetGameInstance();
    	IsValid(GI))
    {
        if (UItemDBSubsystem* DB = GI->GetSubsystem<UItemDBSubsystem>())
        {
            return DB->BuildSlotView(Index, Slots[Index], Out);
        }
    }

    // DB가 없어도 빈 슬롯은 UI에서 사각형으로 표시 가능하도록 true 반환
    return Slots[Index].IsEmpty();
}

void UInventory::GetAllKeys(TArray<FItemKey>& OutKeys) const
{
	OutKeys.Reset();
	for (const FInventorySlot& S : Slots)
	{
		if (S.Key.IsValid() && S.Quantity > 0)
		{
			OutKeys.Add(S.Key);
		}
	}
}

int32 UInventory::FindFirstEmptySlot() const
{
	for (const FInventorySlot& S : Slots)
	{
		if (S.IsEmpty())
			return S.SlotIndex;
	}
	return INDEX_NONE;
}

int32 UInventory::FindStackableSlot(const FItemKey& Key, int32& OutRemainingCapacity) const
{
	OutRemainingCapacity = 0;

	// Key→정의 조회해서 MaxStackSize 확인
	const UItemDBSubsystem* DB = GetWorld()->GetGameInstance()->GetSubsystem<UItemDBSubsystem>();
	if (!DB) return INDEX_NONE;

	FItemStaticData Def;
	if (!const_cast<UItemDBSubsystem*>(DB)->TryGetStaticData(Key, Def))
	{
		return INDEX_NONE;
	}
	if (!Def.bStackable || Def.MaxStackSize <= 1) return INDEX_NONE;

	for (const FInventorySlot& S : Slots)
	{
		if (!S.IsEmpty() && S.Key.DefId == Key.DefId)
		{
			const int32 Remain = Def.MaxStackSize - S.Quantity;
			if (Remain > 0)
			{
				OutRemainingCapacity = Remain;
				return S.SlotIndex;
			}
		}
	}
	return INDEX_NONE;
}

void UInventory::BroadcastChanged(const TArray<int32>& DirtyIndices)
{
	TArray<int32> Unique = DirtyIndices;
	Unique.Sort();
	Unique.SetNum(Unique.Num()); // (정렬 후) 그대로 사용 / 중복 제거 원하면 Unique 구현

	OnInventorySlotsChanged.Broadcast(Unique);
}

FInventoryOpResult UInventory::AddItem(const FItemKey& Key, int32 Quantity, const FItemInstanceState& InStateOpt)
{
	if (!Key.IsValid() || Quantity <= 0)
	{
		return FInventoryOpResult::Fail(EInventoryOpError::InvalidQuantity);
	}

	// 1) 스택 가능한지 확인 후 기존 스택에 먼저 합치기
	int32 RemainingToAdd = Quantity;
	TArray<int32> Dirty;

	int32 RemainCap = 0;
	while (RemainingToAdd > 0)
	{
		const int32 StackIdx = FindStackableSlot(Key, RemainCap);
		if (StackIdx == INDEX_NONE) break;

		const int32 ToAdd = FMath::Min(RemainingToAdd, RemainCap);
		Slots[StackIdx].Quantity += ToAdd;
		// 스택에 합칠 때 InstanceState는 보통 무의미(동일 개체 집합).
		// 독립 개체(예: 고유 시드)가 필요하면 "비스택"으로 설계하거나, 별도 컨테이너가 필요.
		Dirty.AddUnique(StackIdx);
		RemainingToAdd -= ToAdd;
	}

	// 2) 남은 수량은 빈 슬롯에 채우기(비스택 또는 스택 꽉 찼을 때)
	while (RemainingToAdd > 0)
	{
		const int32 Empty = FindFirstEmptySlot();
		if (Empty == INDEX_NONE)
		{
			// 더 이상 넣을 곳이 없음 → 지금까지 변경된 슬롯만 반영하고 실패 처리할지, 부분 성공 처리할지 정책 결정
			// 여기선 "부분 성공"을 허용하고, 남은 수량은 버린다(또는 호출자에게 반환).
			return Dirty.Num() > 0 ? FInventoryOpResult::Ok(Dirty) : FInventoryOpResult::Fail(EInventoryOpError::NoEmptySlot);
		}

		// 새 스택을 하나 생성
		FInventorySlot& Slot = Slots[Empty];
		Slot.Key = Key;
		Slot.Quantity = 0;

		// 인스턴스 상태가 의미 있는 아이템(토치/무기 등)은 여기서 InstanceId가 없으면 새로 발급
		Slot.InstanceState = InStateOpt;
		if (!Slot.InstanceState.InstanceId.IsValid())
		{
			Slot.InstanceState.InstanceId = FGuid::NewGuid();
		}

		// 새로 채울 수 있는 최대량 = (스택형이면 MaxStackSize, 아니면 1)
		int32 MaxPush = 1;

		// 정의 참조(스택형이면 MaxStackSize 사용)
		if (UItemDBSubsystem* DB = GetWorld()->GetGameInstance()->GetSubsystem<UItemDBSubsystem>())
		{
			FItemStaticData Def;
			if (DB->TryGetStaticData(Key, Def))
			{
				MaxPush = Def.bStackable ? Def.MaxStackSize : 1;
			}
		}

		const int32 ToAdd = FMath::Min(RemainingToAdd, MaxPush);
		Slot.Quantity += ToAdd;
		Dirty.AddUnique(Empty);
		RemainingToAdd -= ToAdd;
	}

	BroadcastChanged(Dirty);
	return FInventoryOpResult::Ok(Dirty);
}

FInventoryOpResult UInventory::MoveItem(int32 FromIndex, int32 ToIndex, int32 Quantity)
{
	if (!IsValidIndex(FromIndex) || !IsValidIndex(ToIndex))
		return FInventoryOpResult::Fail(EInventoryOpError::OutOfBounds);

	if (FromIndex == ToIndex) // 자기 자신으로 이동은 무시
		return FInventoryOpResult::Ok({FromIndex});

	FInventorySlot& From = Slots[FromIndex];
	FInventorySlot& To   = Slots[ToIndex];

	if (From.IsEmpty())
		return FInventoryOpResult::Fail(EInventoryOpError::EmptySource);

	const int32 MoveQty = (Quantity <= 0) ? From.Quantity : FMath::Min(Quantity, From.Quantity);
	if (MoveQty <= 0)
		return FInventoryOpResult::Fail(EInventoryOpError::InvalidQuantity);

	TArray<int32> Dirty;

	// 대상이 빈 슬롯 → 순수 이동
	if (To.IsEmpty())
	{
		To.Key = From.Key;
		To.Quantity = MoveQty;
		To.InstanceState = From.InstanceState; // 스택 일부 이동 시 인스턴스 개념이 있다면 주의: 보통 스택형은 개체가 "같음"

		From.Quantity -= MoveQty;
		if (From.Quantity <= 0) From.Clear();

		Dirty.AddUnique(FromIndex);
		Dirty.AddUnique(ToIndex);
		BroadcastChanged(Dirty);
		return FInventoryOpResult::Ok(Dirty);
	}

	// 대상이 같은 키 → 합치기 시도
	if (To.Key.DefId == From.Key.DefId)
	{
		// 최대 스택 확인
		int32 MaxStackSize = 1;
		bool bStackable = false;

		if (UItemDBSubsystem* DB = GetWorld()->GetGameInstance()->GetSubsystem<UItemDBSubsystem>())
		{
			FItemStaticData Def;
			DB->TryGetStaticData(To.Key, Def);
			MaxStackSize = Def.MaxStackSize;
			bStackable = Def.bStackable;
		}

		if (!bStackable || MaxStackSize <= 1)
		{
			return FInventoryOpResult::Fail(EInventoryOpError::NotStackable);
		}

		const int32 CanPush = MaxStackSize - To.Quantity;
		if (CanPush > 0)
		{
			const int32 ToMove = FMath::Min(MoveQty, CanPush);
			To.Quantity += ToMove;
			From.Quantity -= ToMove;
			if (From.Quantity <= 0) From.Clear();

			Dirty.AddUnique(FromIndex);
			Dirty.AddUnique(ToIndex);
			BroadcastChanged(Dirty);
			return FInventoryOpResult::Ok(Dirty);
		}
		// 대상이 가득 차 있으면 아래 스왑 로직으로 빠짐
	}

	// 다른 키(혹은 같은 키지만 대상 가득 참) → 스왑
	{
		Swap(From.Key, To.Key);
		Swap(From.Quantity, To.Quantity);
		Swap(From.InstanceState, To.InstanceState);

		Dirty.AddUnique(FromIndex);
		Dirty.AddUnique(ToIndex);
		BroadcastChanged(Dirty);
		return FInventoryOpResult::Ok(Dirty);
	}
}

FInventoryOpResult UInventory::SplitStack(int32 FromIndex, int32 ToIndex, int32 SplitQuantity)
{
	if (!IsValidIndex(FromIndex) || !IsValidIndex(ToIndex))
		return FInventoryOpResult::Fail(EInventoryOpError::OutOfBounds);

	if (FromIndex == ToIndex)
		return FInventoryOpResult::Fail(EInventoryOpError::InvalidQuantity);

	FInventorySlot& From = Slots[FromIndex];
	FInventorySlot& To   = Slots[ToIndex];

	if (From.IsEmpty())
		return FInventoryOpResult::Fail(EInventoryOpError::EmptySource);

	if (!To.IsEmpty())
		return FInventoryOpResult::Fail(EInventoryOpError::SlotBlocked); // 분할 대상은 빈 슬롯이어야 단순함

	if (SplitQuantity <= 0 || SplitQuantity >= From.Quantity)
		return FInventoryOpResult::Fail(EInventoryOpError::InvalidQuantity);

	// 같은 아이템의 새로운 "스택"을 만든다.
	To.Key = From.Key;
	To.Quantity = SplitQuantity;

	// 스택형은 보통 개별 InstanceId 개념이 없지만,
	// 고유 개체로 취급하려면 새 GUID를 발급(퀵슬롯에서 개체 단위 바인딩이 필요할 때 유용)
	To.InstanceState = From.InstanceState;
	To.InstanceState.InstanceId = FGuid::NewGuid();

	From.Quantity -= SplitQuantity;

	TArray<int32> Dirty = { FromIndex, ToIndex };
	BroadcastChanged(Dirty);
	return FInventoryOpResult::Ok(Dirty);
}

FInventoryOpResult UInventory::RemoveAt(int32 Index, int32 Quantity)
{
	if (!IsValidIndex(Index))
		return FInventoryOpResult::Fail(EInventoryOpError::OutOfBounds);

	FInventorySlot& S = Slots[Index];
	if (S.IsEmpty())
		return FInventoryOpResult::Fail(EInventoryOpError::EmptySource);

	if (Quantity <= 0 || Quantity > S.Quantity)
		return FInventoryOpResult::Fail(EInventoryOpError::InvalidQuantity);

	S.Quantity -= Quantity;
	if (S.Quantity <= 0)
	{
		S.Clear();
	}

	TArray<int32> Dirty = { Index };
	BroadcastChanged(Dirty);
	return FInventoryOpResult::Ok(Dirty);
}

void UInventory::BeginPlay()
{
    Super::BeginPlay();
    BuildHandInstancesPool();
}

void UInventory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (auto& KVP : Instances)
    {
        if (IsValid(KVP.Value))
        {
            KVP.Value->Destroy();
        }
    }
    Instances.Empty();
    InUseHandItems.Empty();

    Super::EndPlay(EndPlayReason);
}

void UInventory::BuildHandInstancesPool()
{
    Instances.Empty();

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    UGameInstance* GI = World->GetGameInstance();
    if (!GI)
    {
        return;
    }

    UItemDBSubsystem* DB = GI->GetSubsystem<UItemDBSubsystem>();
    if (!DB || !DB->ItemDataTable)
    {
        return;
    }

    UDataTable* Table = DB->ItemDataTable;
    static const FString Ctx = TEXT("UInventory::BuildHandInstancesPool");

    const TArray<FName> RowNames = Table->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        const FItemStaticData* Row = Table->FindRow<FItemStaticData>(RowName, Ctx, /*bWarnIfMissing*/true);
        if (!Row)
        {
            continue;
        }

        bool bHandable = Row->bIsHandable;
        if (!bHandable)
        {
            // 호환: DataTable 컬럼명이 isHandable 인 경우 리플렉션으로 조회
            if (UScriptStruct* RowStruct = Table->RowStruct)
            {
                if (FProperty* FoundProp = RowStruct->FindPropertyByName(TEXT("isHandable")))
                {
                    if (FBoolProperty* BoolProp = CastField<FBoolProperty>(FoundProp))
                    {
                        bHandable = BoolProp->GetPropertyValue_InContainer(Row);
                    }
                }
            }
        }
        if (!bHandable)
        {
            continue;
        }

        // 스폰 클래스 로드
        TSubclassOf<AHandItem> SpawnClass = nullptr;
        if (Row->HandItemClass.IsValid())
        {
            SpawnClass = Row->HandItemClass.Get();
        }
        else if (Row->HandItemClass.ToSoftObjectPath().IsValid())
        {
            SpawnClass = Row->HandItemClass.LoadSynchronous();
        }

        if (!*SpawnClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("HandItemClass not set or failed to load for DefId '%s'"), *RowName.ToString());
            continue;
        }

        if (Instances.Contains(RowName))
        {
            // 이미 풀에 존재
            continue;
        }

        FActorSpawnParameters Params;
        Params.Owner = GetOwner();
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AHandItem* Spawned = World->SpawnActor<AHandItem>(SpawnClass, FTransform::Identity, Params);
        if (!Spawned)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to spawn hand item for DefId '%s'"), *RowName.ToString());
            continue;
        }

        // 풀용으로 비활성화
        Spawned->SetActorHiddenInGame(true);
        Spawned->SetActorEnableCollision(false);
        Spawned->SetActorTickEnabled(false);

        Instances.Add(RowName, Spawned);
    }
}

AHandItem* UInventory::AcquireHandItemByKey(const FItemKey& Key)
{
    if (!Key.IsValid()) return nullptr;
    return AcquireHandItemByDefId(Key.DefId);
}

AHandItem* UInventory::AcquireHandItemByDefId(FName DefId)
{
    if (DefId == NAME_None) return nullptr;

    // 이미 풀에 있나 확인
    AHandItem* Item = nullptr;
    if (AHandItem** Found = Instances.Find(DefId))
    {
        Item = *Found;
        if (!IsValid(Item))
        {
            Instances.Remove(DefId);
            Item = nullptr;
        }
    }

    // 없으면 스폰 시도(데이터테이블 기준)
    if (!Item)
    {
        UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
        UItemDBSubsystem* DB = GI ? GI->GetSubsystem<UItemDBSubsystem>() : nullptr;
        if (DB && DB->ItemDataTable)
        {
            static const FString Ctx = TEXT("UInventory::AcquireHandItemByDefId");
            if (const FItemStaticData* Row = DB->ItemDataTable->FindRow<FItemStaticData>(DefId, Ctx, /*bWarnIfMissing*/true))
            {
                bool bHandable = Row->bIsHandable;
                if (!bHandable)
                {
                    if (UScriptStruct* RowStruct = DB->ItemDataTable->RowStruct)
                    {
                        if (FProperty* FoundProp = RowStruct->FindPropertyByName(TEXT("isHandable")))
                        {
                            if (FBoolProperty* BoolProp = CastField<FBoolProperty>(FoundProp))
                            {
                                bHandable = BoolProp->GetPropertyValue_InContainer(Row);
                            }
                        }
                    }
                }

                if (bHandable)
                {
                    TSubclassOf<AHandItem> SpawnClass = nullptr;
                    if (Row->HandItemClass.IsValid())
                    {
                        SpawnClass = Row->HandItemClass.Get();
                    }
                    else if (Row->HandItemClass.ToSoftObjectPath().IsValid())
                    {
                        SpawnClass = Row->HandItemClass.LoadSynchronous();
                    }

                    if (*SpawnClass)
                    {
                        FActorSpawnParameters Params;
                        Params.Owner = GetOwner();
                        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                        if (UWorld* World = GetWorld())
                        {
                            Item = World->SpawnActor<AHandItem>(SpawnClass, FTransform::Identity, Params);
                            if (Item)
                            {
                                Item->SetActorHiddenInGame(true);
                                Item->SetActorEnableCollision(false);
                                Item->SetActorTickEnabled(false);
                                Instances.Add(DefId, Item);
                            }
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("HandItemClass missing for DefId '%s'"), *DefId.ToString());
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("DefId '%s' is not handable"), *DefId.ToString());
                }
            }
        }
    }

    if (!Item)
    {
        return nullptr;
    }

    // 이미 사용 중이면 그대로 포인터 반환(중복 획득 방지)
    if (!InUseHandItems.Contains(DefId))
    {
        InUseHandItems.Add(DefId);
        Item->SetActorHiddenInGame(false);
        // 충돌/틱은 장비 시스템에서 관리 예정
        OnHandItemAcquired.Broadcast(DefId, Item);
    }

    return Item;
}

bool UInventory::ReleaseHandItemByDefId(FName DefId)
{
    if (AHandItem** Found = Instances.Find(DefId))
    {
        AHandItem* Item = *Found;
        if (IsValid(Item))
        {
            Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            Item->SetActorHiddenInGame(true);
            Item->SetActorEnableCollision(false);
            Item->SetActorTickEnabled(false);
        }
        InUseHandItems.Remove(DefId);
        OnHandItemReleased.Broadcast(DefId);
        return true;
    }
    return false;
}

FInventoryOpResult UInventory::UseItemAt(int32 SlotIndex)
{
    if (!IsValidIndex(SlotIndex))
    {
        return FInventoryOpResult::Fail(EInventoryOpError::OutOfBounds);
    }

    if (Slots[SlotIndex].IsEmpty())
    {
        return FInventoryOpResult::Fail(EInventoryOpError::EmptySource);
    }

    const FItemKey Key = Slots[SlotIndex].Key;

    UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
    UItemDBSubsystem* DB = GI ? GI->GetSubsystem<UItemDBSubsystem>() : nullptr;
    if (!DB)
    {
        return FInventoryOpResult::Fail(EInventoryOpError::SlotBlocked);
    }

    FItemStaticData Def;
    if (!DB->TryGetStaticData(Key, Def))
    {
        return FInventoryOpResult::Fail(EInventoryOpError::SlotBlocked);
    }

    // 1) 핸드 장비라면 풀에서 획득(장비 시스템 연계는 외부에서 처리)
    bool bHandable = Def.bIsHandable;
    if (!bHandable)
    {
        if (UScriptStruct* RowStruct = DB->ItemDataTable ? DB->ItemDataTable->RowStruct : nullptr)
        {
            if (FProperty* FoundProp = RowStruct->FindPropertyByName(TEXT("isHandable")))
            {
                if (FBoolProperty* BoolProp = CastField<FBoolProperty>(FoundProp))
                {
                    bHandable = BoolProp->GetPropertyValue_InContainer(&Def);
                }
            }
        }
    }

    if (bHandable)
    {
        // 전환 정책: 기존 장착 해제 후 새로 획득
        ReleaseAllInUseHandItems();
        AHandItem* Hand = AcquireHandItemByDefId(Key.DefId);
        if (Hand)
        {
            // 슬롯은 그대로 두고, 상위 시스템(장비)이 실제 장착 후 필요 시 제거 처리
            return FInventoryOpResult::Ok({ SlotIndex });
        }
        return FInventoryOpResult::Fail(EInventoryOpError::SlotBlocked);
    }

    // 2) 소비 가능 항목이면 1개 소모
    bool bConsumable = false;
    const FGameplayTag ConsumableTag = FGameplayTag::RequestGameplayTag(FName("Item.Consumable"), false);
    if (ConsumableTag.IsValid() && Def.Tags.HasTag(ConsumableTag))
    {
        bConsumable = true;
    }
    else if (Def.bStackable)
    {
        bConsumable = true; // 태그가 없어도 스택형이면 기본적으로 소모 가능 처리
    }

    if (bConsumable)
    {
        const FItemKey ConsumedKey = Key;
        FInventoryOpResult R = RemoveAt(SlotIndex, 1);
        if (R.bSuccess)
        {
            OnItemConsumed.Broadcast(SlotIndex, ConsumedKey);
        }
        return R;
    }

    return FInventoryOpResult::Fail(EInventoryOpError::SlotBlocked);
}

void UInventory::ReleaseAllInUseHandItems()
{
    if (InUseHandItems.Num() == 0) return;
    TArray<FName> ToRelease;
    ToRelease.Reserve(InUseHandItems.Num());
    for (const FName& Id : InUseHandItems)
    {
        ToRelease.Add(Id);
    }
    for (const FName& Id : ToRelease)
    {
        ReleaseHandItemByDefId(Id);
    }
}

int32 UInventory::GetSlotIndexForHotkey(int32 Hotkey) const
{
    if (Hotkey <= 0) return INDEX_NONE;
    const int32 Col = Hotkey - 1; // 1번키 → 0열
    if (Col >= 0 && Col < NumCols)
    {
        return IndexFromRC(0, Col); // 1행(0 Row)을 핫바로 가정
    }
    return INDEX_NONE;
}

AHandItem* UInventory::SelectHotbarSlot(int32 Hotkey)
{
    const int32 SlotIndex = GetSlotIndexForHotkey(Hotkey);
    if (!IsValidIndex(SlotIndex) || Slots[SlotIndex].IsEmpty())
    {
        // 전환 정책: 기존 장착 해제 후 널 반환
        ReleaseAllInUseHandItems();
        return nullptr;
    }

    const FItemKey Key = Slots[SlotIndex].Key;

    // 핸드 가능하면 풀에서 꺼내 포인터 반환(없으면 null)
    UGameInstance* GI = IsValid(GetWorld()) ? GetWorld()->GetGameInstance() : nullptr;
    UItemDBSubsystem* DB = IsValid(GI) ? GI->GetSubsystem<UItemDBSubsystem>() : nullptr;
    // 전환 정책: 기존 장착 해제
    ReleaseAllInUseHandItems();
    if (!DB) { return nullptr; }

    FItemStaticData Def;
    if (!DB->TryGetStaticData(Key, Def))
    {
        return nullptr;
    }

    bool bHandable = Def.bIsHandable;
    if (!bHandable)
    {
        if (UScriptStruct* RowStruct = DB->ItemDataTable ? DB->ItemDataTable->RowStruct : nullptr)
        {
            if (FProperty* FoundProp = RowStruct->FindPropertyByName(TEXT("isHandable")))
            {
                if (FBoolProperty* BoolProp = CastField<FBoolProperty>(FoundProp))
                {
                    bHandable = BoolProp->GetPropertyValue_InContainer(&Def);
                }
            }
        }
    }

    if (!bHandable) { return nullptr; }

    return AcquireHandItemByDefId(Key.DefId);
}
