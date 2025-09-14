// InventoryTypes.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/SoftObjectPtr.h"
#include "GameplayTagContainer.h"
#include "InventoryType.generated.h"

// 전방 선언: 헤더 경량화 및 컴파일 안정성
class UTexture2D;

UENUM(BlueprintType)
enum class EInventoryOpError : uint8
{
	None,
	OutOfBounds,       // 슬롯 인덱스 범위 초과
	EmptySource,       // 원본 슬롯이 비어있음
	InvalidQuantity,   // 0 이하 혹은 보유 수량 초과
	NotStackable,      // 서로 합칠 수 없는 조합
	SlotBlocked,       // 슬롯 타입/필터 등의 제약으로 불가
	NoEmptySlot,       // 빈 슬롯 없음
};

/**
 * 아이템 고유 Key
 * - DataTable RowName 또는 PrimaryAssetId.PrimaryAssetName 등으로 사용
 * - 본 샘플은 단순화를 위해 FName 키만 사용
 */
USTRUCT(BlueprintType)
struct FItemKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefId  = NAME_None; // 정적 정의를 조회하는 Key

	bool IsValid() const
	{
		return DefId  != NAME_None;
	}
};

/**
 * 정적 아이템 정의(아이템 DB의 캐시 단위)
 * - DataTable에서 읽어와 캐시에 담아두는 구조체
 * - "변하지 않는 속성"만 둔다(이름/아이콘/최대 스택/카테고리 등)
 */
USTRUCT(BlueprintType)
struct FItemStaticData : public FTableRowBase
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefId = NAME_None; // 키(중복 확인/로그용)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags; // 카테고리/필터(예: Item.Consumable, Item.Weapon 등)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize = 1;     // 1이면 비스택

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStackable = false;    // MaxStackSize>1 과 중복되지만 명시적 플래그

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon; // UI 아이콘(SoftPtr → 비동기 로딩)

};

/**
 * 인스턴스(개체) 상태(슬롯마다 다를 수 있는, "변하는 속성")
 * - 예: 토치 남은 시간, 내구도, 무기 탄창 수, 고유 시드 등
 * - 프로젝트 규칙에 맞게 확장 가능
 */
USTRUCT(BlueprintType)
struct FItemInstanceState
{
	GENERATED_BODY()

	// 고유 개체 식별(퀵슬롯 바인딩/장비 연동용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid InstanceId;

	// 대표 예시 필드들(필요한 것만 사용/확장)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RemainingTime = 0.f;     // 예: 토치 남은 지속시간(초)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Durability = -1;         // -1이면 미사용

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magazine = -1;           // -1이면 미사용
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Reserve = -1;            // -1이면 미사용
};

/**
 * 인벤토리 실제 저장 슬롯(모델)
 * - "키 + 수량 + 인스턴스 상태"만 저장 (정의는 DB에서 조회)
 */
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemKey Key; // == DefId

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemInstanceState InstanceState;

	// 유틸
	bool IsEmpty() const { return !Key.IsValid() || Quantity <= 0; }
	void Clear()
	{
		Key = FItemKey{};
		Quantity = 0;
		InstanceState = FItemInstanceState{};
	}
};

/**
 * UI가 받는 "뷰" 구조체
 * - 슬롯의 Key를 아이템 DB로 조회해 채워진 읽기 전용 덩어리
 * - UI는 이 구조만 알면 되며, DT/PDA가 어딘지 몰라도 됨
 */
USTRUCT(BlueprintType)
struct FInventorySlotView
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 0;

	// 정적 정의에서 채워지는 필드들
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStackSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	// 인스턴스 상태에서 노출하고 싶은 일부(예시)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RemainingTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Durability = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Magazine = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Reserve = -1;
};

/**
 * 인벤토리 연산 결과
 * - UI는 ChangedSlots만 받아 부분 리프레시
 */
USTRUCT(BlueprintType)
struct FInventoryOpResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSuccess = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> ChangedSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInventoryOpError Error = EInventoryOpError::None;

	static FInventoryOpResult Ok(TArray<int32> InChanged)
	{
		FInventoryOpResult R;
		R.ChangedSlots = MoveTemp(InChanged);
		return R;
	}
	static FInventoryOpResult Fail(EInventoryOpError InError)
	{
		FInventoryOpResult R;
		R.bSuccess = false;
		R.Error = InError;
		return R;
	}
};
