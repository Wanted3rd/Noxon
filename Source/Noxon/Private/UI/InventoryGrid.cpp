#include "UI/InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryType.h"
#include "UI/InventoryGridSlot.h"
#include "Utility/DebugHelper.h"

void UInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GridConstruct();
}

void UInventoryGrid::NativeDestruct()
{
	if (BoundInventory.IsValid())
	{
		BoundInventory->OnInventorySlotsChanged.RemoveDynamic(this, &UInventoryGrid::HandleInventorySlotsChanged);
		BoundInventory.Reset();
	}

	Super::NativeDestruct();
}

void UInventoryGrid::SetOwningCanvas(class UCanvasPanel* OwningCanvas)
{
	OwningCanvasPanel = OwningCanvas;
}

void UInventoryGrid::InitializeInventory(UInventory* InInventory)
{
	if (BoundInventory.Get() == InInventory)
	{
		// The grid already consumes this inventory; force a refresh in case slot visuals fell out of sync.
		RefreshAllSlots();
		return;
	}

	if (UInventory* PreviousInventory = BoundInventory.Get())
	{
		PreviousInventory->OnInventorySlotsChanged.RemoveDynamic(this, &UInventoryGrid::HandleInventorySlotsChanged);
	}

	BoundInventory = InInventory;
	if (!BoundInventory.IsValid())
	{
		RefreshAllSlots();
		return;
	}

	BoundInventory->OnInventorySlotsChanged.AddDynamic(this, &UInventoryGrid::HandleInventorySlotsChanged);

	Rows = BoundInventory->NumRows;
	Cols = BoundInventory->NumCols;

	GridConstruct();
}

void UInventoryGrid::RefreshAllSlots()
{
	if (!bGridConstructed)
	{
		return;
	}

	for (int32 SlotIdx = 0; SlotIdx < GridSlots.Num(); ++SlotIdx)
	{
		RefreshSlotInternal(SlotIdx);
	}
}

void UInventoryGrid::GridConstruct()
{
	if (!CanvasPanel || !GridSlotClass)
	{
		return;
	}

	ClearGrid();

	const int32 ClampedRows = FMath::Max(1, Rows);
	const int32 ClampedCols = FMath::Max(1, Cols);

	GridSlots.Reserve(ClampedRows * ClampedCols);

	for (int32 j = 0; j < ClampedRows; ++j)
	{
		for (int32 i = 0; i < ClampedCols; ++i)
		{
			UInventoryGridSlot* SlotWidget = CreateWidget<UInventoryGridSlot>(this, GridSlotClass);
			if (!SlotWidget)
			{
				continue;
			}

			CanvasPanel->AddChild(SlotWidget);

			const int32 Index = j * ClampedCols + i;
			SlotWidget->SetSlotIndex(Index);
			SlotWidget->InitializeSlot(this);

			if (UCanvasPanelSlot* GridSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlotWidget))
			{
				GridSlot->SetSize(FVector2D(SlotSize));
				const FVector2D SlotPosition(static_cast<float>(i) * SlotSize, static_cast<float>(j) * SlotSize);
				GridSlot->SetPosition(SlotPosition);
			}

			GridSlots.Add(SlotWidget);
		}
	}

	bGridConstructed = true;

	RefreshAllSlots();
}
void UInventoryGrid::RefreshSlotInternal(int32 SlotIndex)
{
	if (!GridSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	UInventoryGridSlot* SlotWidget = GridSlots[SlotIndex];
	if (!SlotWidget)
	{
		return;
	}

	FInventorySlotView SlotView;
	bool bHasData = false;

	if (BoundInventory.IsValid())
	{
		bHasData = BoundInventory->GetSlotView(SlotIndex, SlotView);
	}

	if (!bHasData)
	{
		SlotView = FInventorySlotView{};
		SlotView.SlotIndex = SlotIndex;
	}

	// Defer to the slot widget for visual updates; implementation lives in the slot class.
	SlotWidget->RefreshFromView(SlotView);
}

void UInventoryGrid::ClearGrid()
{
	if (CanvasPanel)
	{
		CanvasPanel->ClearChildren();
	}

	GridSlots.Empty();
	bGridConstructed = false;
}

bool UInventoryGrid::HandleSlotDrop(int32 FromIndex, int32 ToIndex)
{
	if (!BoundInventory.IsValid() || FromIndex == ToIndex)
	{
		return false;
	}

	// Delegate the move operation to the inventory component so it can handle stacking rules.
	FInventoryOpResult MoveResult = BoundInventory->MoveItem(FromIndex, ToIndex, 0);
	if (!MoveResult.bSuccess)
	{
		return false;
	}

	RefreshSlotInternal(FromIndex);
	RefreshSlotInternal(ToIndex);
	return true;
}
void UInventoryGrid::HandleInventorySlotsChanged(const TArray<int32>& ChangedSlots)
{
	for (const int32 SlotIndex : ChangedSlots)
	{
		RefreshSlotInternal(SlotIndex);
	}
}
















