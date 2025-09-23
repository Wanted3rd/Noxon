#include "Inventory/DoveTestCtrl.h"

#include "Inventory/Inv_Control.h"
#include "Inventory/Inventory.h"
#include "GameFramework/Pawn.h"

void ADoveTestCtrl::BeginPlay()
{
    Super::BeginPlay();
    if (GetPawn())
    {
        if (UInventory* Inventory = GetPawn()->FindComponentByClass<UInventory>())
        {
            if (Inventory->Slots.Num() == 0)
            {
                Inventory->InitializeSlots(Inventory->NumRows, Inventory->NumCols);
            }
        }

        if (UInv_Control* Control = GetPawn()->FindComponentByClass<UInv_Control>())
        {
            Control->RefreshOwnerReferences();
        }
    }
}
