#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UInventory;

/**
 * UI grid that mirrors the player's inventory slots.
 */
UCLASS()
class NOXON_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	
	void SetOwningCanvas(class UCanvasPanel* OwningCanvas);

	/**
	 * Binds an inventory component so the grid can create slots and listen for change events.
	 * @param InInventory Inventory component that provides slot data.
	 */
	void InitializeInventory(class UInventory* InInventory);

	/** Forces every visual slot to refresh using the current inventory data. */
	void RefreshAllSlots();

private:
	void GridConstruct();

	/** Updates a single slot widget with the latest data coming from the inventory. */
	void RefreshSlotInternal(int32 SlotIndex);

	/** Released when the grid needs to remove and rebuild slot widgets. */
	void ClearGrid();

	/** Reacts to inventory dirty events and patches the affected slots only. */
	UFUNCTION()
	void HandleInventorySlotsChanged(const TArray<int32>& ChangedSlots);

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

	/** Cached inventory used to drive slot refreshes and delegate bindings. */
	TWeakObjectPtr<class UInventory> BoundInventory;

	/** Tracks whether the internal widgets already exist to prevent duplicate construction. */
	bool bGridConstructed = false;
	
};



