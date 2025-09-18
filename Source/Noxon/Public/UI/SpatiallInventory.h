#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "SpatiallInventory.generated.h"


UCLASS()
class NOXON_API USpatiallInventory : public UInventoryWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UInventoryGrid> Grid_BackPack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

protected:
	/** Forwards the assigned inventory to the grid widget so it can mirror slot data. */
	virtual void OnInventoryAssigned(class UInventory* InInventory) override;
};
