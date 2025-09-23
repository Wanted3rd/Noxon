#include "UI/InventoryGridSlot.h"

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/InventoryGrid.h"

void UInventoryGridSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (SlotImage)
	{
		DefaultTint = SlotImage->GetColorAndOpacity();
		CurrentTint = DefaultTint;
	}

	UpdateHighlight(false);
}

void UInventoryGridSlot::InitializeSlot(UInventoryGrid* InOwnerGrid)
{
	// Cache the grid so drag/drop callbacks can route operations back to the container.
	OwningGrid = InOwnerGrid;
}

void UInventoryGridSlot::RefreshFromView(const FInventorySlotView& ViewData)
{
	// Keep a copy of the data so drag payloads can reuse it without another inventory query.
	CachedView = ViewData;
	bHasItem = ViewData.Key.IsValid() && ViewData.Quantity > 0;

	if (SlotImage)
	{
		// Resolve the soft icon pointer on demand so static C++ tests work without editor preloads.
		UTexture2D* IconTexture = nullptr;
		if (bHasItem)
		{
			if (ViewData.Icon.IsValid())
			{
				IconTexture = ViewData.Icon.Get();
			}
			else if (!ViewData.Icon.IsNull())
			{
				IconTexture = ViewData.Icon.LoadSynchronous();
			}

			SlotImage->SetBrushFromTexture(IconTexture);
			CurrentTint = DefaultTint;
			SlotImage->SetColorAndOpacity(CurrentTint);
		}
		else
		{
			SlotImage->SetBrushFromTexture(nullptr);
			CurrentTint = DefaultTint;
			CurrentTint.A = DefaultTint.A * 0.35f;
			SlotImage->SetColorAndOpacity(CurrentTint);
		}
	}

	if (QuantityText)
	{
		// Only show a quantity badge when stacks are larger than one for readability.
		if (bHasItem && ViewData.Quantity > 1)
		{
			QuantityText->SetText(FText::AsNumber(ViewData.Quantity));
			QuantityText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			QuantityText->SetText(FText::GetEmpty());
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Convert left clicks on filled slots into drag gestures.
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bHasItem)
	{
		return Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply;
}

void UInventoryGridSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!bHasItem)
	{
		return;
	}

	// Spawn a generic drag operation whose payload is the originating slot.
	UDragDropOperation* DragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (!DragOperation)
	{
		return;
	}

	DragOperation->Payload = this;
	DragOperation->DefaultDragVisual = this;
	DragOperation->Pivot = EDragPivot::CenterCenter;
	OutOperation = DragOperation;

	UpdateHighlight(true);
}

bool UInventoryGridSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const bool bSuperHandled = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UpdateHighlight(false);

	if (!InOperation || !OwningGrid.IsValid())
	{
		return bSuperHandled;
	}

	// Accept drops only when they originate from another slot within the same grid instance.
	UInventoryGridSlot* SourceSlot = Cast<UInventoryGridSlot>(InOperation->Payload);
	if (!SourceSlot || SourceSlot == this)
	{
		return bSuperHandled;
	}

	if (!SourceSlot->OwningGrid.IsValid() || SourceSlot->OwningGrid.Get() != OwningGrid.Get())
	{
		return bSuperHandled;
	}

	const bool bHandled = OwningGrid->HandleSlotDrop(SourceSlot->GetSlotIndex(), SlotIndex);
	return bHandled ? true : bSuperHandled;
}

void UInventoryGridSlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	UInventoryGridSlot* SourceSlot = InOperation ? Cast<UInventoryGridSlot>(InOperation->Payload) : nullptr;
	if (SourceSlot && SourceSlot->OwningGrid.IsValid() && SourceSlot->OwningGrid.Get() == OwningGrid.Get())
	{
		UpdateHighlight(true);
	}
}

void UInventoryGridSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UpdateHighlight(false);
}

void UInventoryGridSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	UpdateHighlight(false);
}
void UInventoryGridSlot::UpdateHighlight(bool bEnable)
{
	if (!SlotImage)
	{
		return;
	}

	// Apply a lightweight alpha tweak so users receive visual feedback during drag hover.
	FLinearColor HighlightTint = CurrentTint;
	HighlightTint.A = bEnable ? CurrentTint.A * 0.8f : CurrentTint.A;
	SlotImage->SetColorAndOpacity(HighlightTint);
}







