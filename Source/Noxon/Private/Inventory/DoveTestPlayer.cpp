#include "Inventory/DoveTestPlayer.h"

#include "Inventory/Inventory.h"
#include "Inventory/Inv_Control.h"

ADoveTestPlayer::ADoveTestPlayer()
{
    PrimaryActorTick.bCanEverTick = true;

    Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
    InventoryControl = CreateDefaultSubobject<UInv_Control>(TEXT("InventoryControl"));
}

void ADoveTestPlayer::BeginPlay()
{
    Super::BeginPlay();

    if (Inventory)
    {
        Inventory->InitializeSlots(Inventory->NumRows, Inventory->NumCols);
    }
}

void ADoveTestPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADoveTestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
