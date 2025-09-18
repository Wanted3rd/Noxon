#include "UI/InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/InventoryGridSlot.h"
#include "Utility/DebugHelper.h"

void UInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GridConstruct();
}

void UInventoryGrid::SetOwningCanvas(UCanvasPanel* OwningCanvas)
{
	if (CanvasPanel && CanvasPanel != OwningCanvas)
	{
		CanvasPanel->ClearChildren();
	}

	if (OwningCanvas)
	{
		BuildGrid(OwningCanvas);
	}
	else
	{
		GridConstruct();
	}
}

void UInventoryGrid::GridConstruct()
{
	BuildGrid(CanvasPanel);
}

void UInventoryGrid::BuildGrid(UCanvasPanel* TargetPanel)
{
	GridSlots.Reset();

	if (!TargetPanel || !GridSlotClass)
	{
		if (TargetPanel)
		{
			TargetPanel->ClearChildren();
		}
		return;
	}

	TargetPanel->ClearChildren();

	const int32 EffectiveRows = FMath::Max(1, Rows);
	const int32 EffectiveCols = FMath::Max(1, Cols);
	const float EffectiveSlotSize = FMath::Max(1.f, SlotSize);

	GridSlots.Reserve(EffectiveRows * EffectiveCols);

	for (int32 Row = 0; Row < EffectiveRows; ++Row)
	{
		for (int32 Col = 0; Col < EffectiveCols; ++Col)
		{
			if (UInventoryGridSlot* SlotWidget = CreateWidget<UInventoryGridSlot>(this, GridSlotClass))
			{
				if (UCanvasPanelSlot* CanvasSlot = TargetPanel->AddChildToCanvas(SlotWidget))
				{
					const int32 Index = Row * EffectiveCols + Col;
					SlotWidget->SetSlotIndex(Index);

					CanvasSlot->SetAutoSize(false);
					CanvasSlot->SetAlignment(FVector2D::ZeroVector);
					CanvasSlot->SetPosition(FVector2D(Col * EffectiveSlotSize, Row * EffectiveSlotSize));
					CanvasSlot->SetSize(FVector2D(EffectiveSlotSize, EffectiveSlotSize));

					GridSlots.Add(SlotWidget);
					LOG_TEXT(TEXT("slot : %d, %d, %.2f"), Col, Row, EffectiveSlotSize);
					LOG_TEXT(TEXT("%d"), GridSlots.Num());
				}
			}
		}
	}

	const FVector2D TotalSize(EffectiveCols * EffectiveSlotSize, EffectiveRows * EffectiveSlotSize);

	if (UCanvasPanelSlot* PanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TargetPanel))
	{
		PanelSlot->SetAutoSize(false);
		PanelSlot->SetAlignment(FVector2D::ZeroVector);
		PanelSlot->SetSize(TotalSize);
	}

	if (TargetPanel == CanvasPanel)
	{
		if (UCanvasPanelSlot* WidgetSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this))
		{
			WidgetSlot->SetAutoSize(false);
			WidgetSlot->SetAlignment(FVector2D::ZeroVector);
			WidgetSlot->SetSize(TotalSize);
		}
	}
}

FVector2D UInventoryGrid::GetTotalPixelSize() const
{
	const float EffectiveSlotSize = FMath::Max(1.f, SlotSize);
	const int32 EffectiveRows = FMath::Max(1, Rows);
	const int32 EffectiveCols = FMath::Max(1, Cols);
	return FVector2D(EffectiveCols * EffectiveSlotSize, EffectiveRows * EffectiveSlotSize);
}
