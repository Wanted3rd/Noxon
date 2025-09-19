#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "InventoryType.h"
#include "ItemDBSubsystem.generated.h"

/**
 * GameInstance 레벨에서 살아있는 "아이템 정의 리포지토리 + 캐시"
 * - DataTable(또는 나중에 PrimaryDataAsset) 뒤에 숨어서
 *   정적 정의를 찾아주고, 아이콘 등 SoftPtr을 전달해준다.
 * - UI는 BuildSlotView() 결과만 보면 되고, DT를 몰라도 된다.
 */
UCLASS()
class NOXON_API UItemDBSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ItemDB")
	UDataTable* ItemDataTable = nullptr;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	

	
	bool TryGetStaticData(const FItemKey& Key, FItemStaticData& OutData);
	
	void Prefetch(const TArray<FItemKey>& Keys);

	bool BuildSlotView(int32 SlotIndex, const FInventorySlot& Slot, FInventorySlotView& OutView);

private:
	// DefId → 캐시
	TMap<FName /*DefId*/, FItemStaticData> Cache;
};
