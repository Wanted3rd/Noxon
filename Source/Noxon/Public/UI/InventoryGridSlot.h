#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryType.h"
#include "InventoryGridSlot.generated.h"

class UInventoryGrid;
class UImage;
class UTextBlock;

/** UI tile that represents a single inventory slot and supports drag interactions. */
UCLASS()
class NOXON_API UInventoryGridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	/**
	 * Injects the owning grid reference so drag-and-drop callbacks can notify the container.
	 * @param InOwnerGrid Grid widget that manages all slots.
	 */
	void InitializeSlot(class UInventoryGrid* InOwnerGrid);

	void SetSlotIndex(int32 idx) { SlotIndex = idx; }
	int32 GetSlotIndex() const { return SlotIndex; }

	/** Applies the latest slot data so the widget visuals stay in sync with the inventory model. */
	void RefreshFromView(const FInventorySlotView& ViewData);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	/** Visual highlight helper used while drag targets hover over the slot. */
	void UpdateHighlight(bool bEnable);

private:
	UPROPERTY(meta = (BindWidget), EditAnywhere)
	TObjectPtr<class UImage> SlotImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UTextBlock> QuantityText;

	/** Cached index assigned by the owning grid. */
	int32 SlotIndex{INDEX_NONE};

	/** Base tint cached so highlights can restore the original color. */
	FLinearColor DefaultTint = FLinearColor::White;

	/** Tint representing the current visual state before hover effects are applied. */
	FLinearColor CurrentTint = FLinearColor::White;

	/** Tracks whether the slot currently displays an item and can be dragged. */
	bool bHasItem = false;

	/** Latest view data so repeated drags can reuse the same payload. */
	FInventorySlotView CachedView;

	/** Weak pointer back to the grid that owns this slot for drop notifications. */
	TWeakObjectPtr<class UInventoryGrid> OwningGrid;
};


