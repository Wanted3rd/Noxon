#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGridSlot.generated.h"

UCLASS()
class NOXON_API UInventoryGridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlotIndex(int32 idx) { SlotIndex = idx; }
	int32 GetSlotIndex() const { return SlotIndex; }

private:

public:
	

private:
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class UImage> SlotImage;

	int32 SlotIndex{INDEX_NONE};
	
};
