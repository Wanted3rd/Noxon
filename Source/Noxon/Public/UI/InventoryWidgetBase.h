#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

class UInventory;

/**
 * Base class for inventory driven widgets. Exposes the inventory component so derived widgets can react to slot changes.
 */
UCLASS()
class NOXON_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Provides an inventory component reference so derived widgets can listen for slot updates.
	 * @param InInventory Inventory component that drives this widget.
	 */
	void InitializeInventory(class UInventory* InInventory);

protected:
	/**
	 * Hook that derived widgets override to bind themselves to the provided inventory component.
	 * @param InInventory Inventory component that has just been assigned (can be nullptr).
	 */
	virtual void OnInventoryAssigned(class UInventory* InInventory);

	/** Weak reference to the currently bound inventory; kept for quick access in derived classes. */
	TWeakObjectPtr<class UInventory> BoundInventory;
};
