#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeOnInitialized() override;
	
	
	void SetOwningCanvas(class UCanvasPanel* OwningCanvas);


private:
	void GridConstruct();
	
	
public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<class UInventoryGridSlot> GridSlotClass;

private:	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows=4;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Cols=4 ;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotSize = 32.f;

	UPROPERTY()
	TArray<TObjectPtr<class UInventoryGridSlot>> GridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> CanvasPanel;

	TWeakObjectPtr<UCanvasPanel> OwningCanvasPanel;
	
};
