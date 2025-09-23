#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UCanvasPanel;

UCLASS()
class NOXON_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void SetOwningCanvas(UCanvasPanel* OwningCanvas);
	FVector2D GetTotalPixelSize() const;

private:
	void GridConstruct();
	void BuildGrid(UCanvasPanel* TargetPanel);

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<class UInventoryGridSlot> GridSlotClass;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows = 4;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Cols = 4;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float SlotSize = 32.f;

	UPROPERTY()
	TArray<TObjectPtr<class UInventoryGridSlot>> GridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
};
