#pragma once

#include "CoreMinimal.h"
#include "InventoryWidgetBase.h"
#include "SpatiallInventory.generated.h"

class UCanvasPanel;
class UInventoryGrid;

UCLASS()
class NOXON_API USpatiallInventory : public UInventoryWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryGrid> Grid_BackPack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
};
