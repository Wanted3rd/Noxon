// InventoryComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/InventoryType.h"
#include "Inventory.generated.h"

// UI가 바인딩할 델리게이트: 변경된 슬롯 인덱스들만 내려준다(부분 리프레시)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotsChanged, const TArray<int32>&, ChangedSlots);

/**
 * 플레이어가 소유하는 인벤토리(싱글 로컬)
 * - "슬롯 배열"과 "연산(Add/Move/Split/Drop)"을 책임진다.
 * - 정적 정의는 UItemDBSubsystem이 가지고 있고,
 *   UI는 GetSlotView()/BuildSlotView()를 통해 데이터 조회.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory();

	/** 인벤토리 크기(행/열) - 디자이너 수정 가능 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 NumRows = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 NumCols = 8;

	/** 내부 슬롯 배열(모델) */
	UPROPERTY(VisibleAnywhere, Category="Inventory")
	TArray<FInventorySlot> Slots;

	/** 변경 이벤트: UI는 이걸 구독해서 해당 슬롯만 Refresh */
	UPROPERTY(BlueprintAssignable)
	FOnInventorySlotsChanged OnInventorySlotsChanged;

	// ====== 초기화 & 조회 ======

	/**
	 * 슬롯 배열 초기화(게임 시작/로드 직후 등)
	 * @param InRows,InCols  그리드 사이즈(변경 시 재생성)
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeSlots(int32 InRows, int32 InCols);

	/**
	 * 2D(Row,Col) → 1D(Index) 변환
	 */
	int32 IndexFromRC(int32 Row, int32 Col) const;

	/**
	 * 슬롯 원본 데이터(모델) 가져오기(읽기 전용)
	 */
	const FInventorySlot& GetSlot(int32 Index) const;

	/**
	 * UI가 사용할 뷰 데이터 구성(아이템 DB 조회 포함)
	 * @param Index   어느 슬롯을 그릴지
	 * @param Out     UI 뷰 구조체
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool GetSlotView(int32 Index, FInventorySlotView& Out) const;

	/**
	 * 현재 보유 키 목록을 반환(아이템 DB 프리패치용)
	 */
	void GetAllKeys(TArray<FItemKey>& OutKeys) const;

	// ====== 연산(모델 변경) ======

	/**
	 * 아이템 추가(드랍 줍기/보상 수령 등)
	 * - 스택 가능하면 기존 스택에 합치고, 아니면 빈 슬롯을 찾는다.
	 * @param Key         추가할 아이템 정의 키
	 * @param Quantity    추가 수량
	 * @param InStateOpt  (선택) 특정 인스턴스 상태(토치 남은시간 등)를 미리 지정
	 * @return            결과 + 바뀐 슬롯 인덱스들(부분 갱신)
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory", meta=(AdvancedDisplay="InStateOpt"))
	FInventoryOpResult AddItem(const FItemKey& Key, int32 Quantity, const FItemInstanceState& InStateOpt);

	// C++ 편의 오버로드(블루프린트 미노출): 기본 인스턴스 상태로 추가
	FInventoryOpResult AddItem(const FItemKey& Key, int32 Quantity)
	{
		return AddItem(Key, Quantity, FItemInstanceState{});
	}

	/**
	 * 슬롯 간 이동/스왑/합치기까지 포괄
	 * - 대상 슬롯이 비면 이동, 같은 키면 합치기, 다르면 스왑.
	 * @param FromIndex   원본 슬롯
	 * @param ToIndex     대상 슬롯
	 * @param Quantity    이동 수량(스택 일부만 이동할 때 사용). 0 또는 음수면 전량 이동으로 처리
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FInventoryOpResult MoveItem(int32 FromIndex, int32 ToIndex, int32 Quantity /*=0 → 전량*/);

	/**
	 * 스택 분할(새 슬롯으로 지정 수량만큼 떼어내기)
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FInventoryOpResult SplitStack(int32 FromIndex, int32 ToIndex, int32 SplitQuantity);

	/**
	 * 슬롯 비우기(월드 드롭 전에 내부 상태만 제거할 때 사용)
	 * - 실제 월드 스폰(픽업 액터)은 외부 시스템에서 처리하고,
	 *   성공 시점에 이 함수를 호출해 모델 상태를 갱신하는 패턴 권장.
	 */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	FInventoryOpResult RemoveAt(int32 Index, int32 Quantity);

	/**
	 * 빈 슬롯 탐색
	 */
	int32 FindFirstEmptySlot() const;

private:
	// 내부 유틸

	bool IsValidIndex(int32 Index) const
	{
		return Slots.IsValidIndex(Index);
	}

	/**
	 * 같은 키 스택을 찾아 수용 여유가 있는 슬롯 인덱스를 반환(없으면 INDEX_NONE)
	 * - AddItem에서 스택 합치기 우선 처리 시 사용
	 */
	int32 FindStackableSlot(const FItemKey& Key, int32& OutRemainingCapacity) const;

	/**
	 * 내부 브로드캐스트 헬퍼 (중복 제거 후 이벤트 발행)
	 */
	void BroadcastChanged(const TArray<int32>& DirtyIndices);
};
