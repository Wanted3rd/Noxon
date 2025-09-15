#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "InventoryType.h"
#include "ItemDBSubsystem.generated.h"


UCLASS()
class NOXON_API UItemDBSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ItemDB")
	UDataTable* ItemDataTable = nullptr;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * 키(DefId)로 정적 정의를 캐시에서 얻거나, 없으면 DataTable에서 로드해 캐시에 넣는다.
	 * @param Key            DataTable RowName과 동일한 값
	 * @param OutData        찾은 정적 정의
	 * @return               성공 여부(false면 키가 존재하지 않음)
	 */
	bool TryGetStaticData(const FItemKey& Key, FItemStaticData& OutData);

	/**
	 * 현재 인벤토리에 존재하는 키들을 한 번에 미리 로드(아이콘 스트리밍 등 사전 준비)
	 * @param Keys           인벤토리 컴포넌트가 가진 모든 DefId 목록
	 */
	void Prefetch(const TArray<FItemKey>& Keys);

	/**
	 * UI/인벤토리가 슬롯 인덱스를 기반으로 "뷰 데이터"를 요청할 때 사용.
	 * 실제 슬롯의 Key/수량/인스턴스 상태는 인벤토리에서 가져오고,
	 * 정적 정의는 여기서 채워 넣는다.
	 * @param SlotIndex      어느 슬롯의 뷰인지(로그/디버깅용)
	 * @param Slot           인벤토리의 원본 슬롯(키/수량/인스턴스 상태)
	 * @param OutView        UI가 쓸 최종 뷰 데이터
	 */
	bool BuildSlotView(int32 SlotIndex, const FInventorySlot& Slot, FInventorySlotView& OutView);

private:
	TMap<FName, FItemStaticData> Cache;
};
