// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DoveTestCtrl.h"
#include "Inventory/Inventory.h"
#include "GameFramework/Pawn.h"

void ADoveTestCtrl::BeginPlay()
{
    Super::BeginPlay();

    // 소유 폰에 인벤토리가 있다면 초기화 보장(플레이어 쪽에서 이미 했으면 중복 없이 통과)
    if (APawn* P = GetPawn())
    {
        if (UInventory* Inv = P->FindComponentByClass<UInventory>())
        {
            if (Inv->Slots.Num() == 0)
            {
                Inv->InitializeSlots(Inv->NumRows, Inv->NumCols);
            }
        }
    }
}
