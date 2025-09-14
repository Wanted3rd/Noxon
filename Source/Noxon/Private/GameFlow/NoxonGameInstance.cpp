// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameFlow/NoxonGameInstance.h"
#include "Inventory/ItemDBSubsystem.h"
#include "Engine/DataTable.h"

void UNoxonGameInstance::Init()
{
    Super::Init();

    // 에셋(DataTable) 로드 후 DB 서브시스템에 주입
    if (UItemDBSubsystem* DB = GetSubsystem<UItemDBSubsystem>())
    {
        if (DB->ItemDataTable == nullptr)
        {
            const TCHAR* Path = TEXT("/Game/Items/DT_Item.DT_Item");
            if (UDataTable* Table = LoadObject<UDataTable>(nullptr, Path))
            {
                DB->ItemDataTable = Table;
                UE_LOG(LogTemp, Log, TEXT("ItemDataTable loaded: %s"), Path);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to load ItemDataTable at %s"), Path);
            }
        }
    }
}
