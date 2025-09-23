#include "Items/BaseItem.h"

#include "Inventory/InventoryType.h"
#include "Items/HandItems/HandItem.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ABaseItem::RegisterStaticData()
{
	const TCHAR* Path = TEXT("/Game/Items/DT_Item.DT_Item");
	UDataTable* Table = LoadObject<UDataTable>(nullptr, Path);
	if (!Table)
	{
		return;
	}
	
	FItemStaticData* data = Table->FindRow<FItemStaticData>(property.itemName, "");
	if (data)
	{
		return;
	}
	FItemStaticData newData = FItemStaticData();
	newData.DefId = property.itemName;
	newData.bIsHandable = property.isHandable;
	newData.bStackable = property.bQuantity;
	newData.DisplayName = FText::FromName(property.itemName);
	newData.HandItemClass = this->IsA(AHandItem::StaticClass()) ? this->StaticClass() : nullptr;
	newData.MaxStackSize = property.maxQuantity;
	newData.Icon = property.icon;
	Table->AddRow(newData.DefId, newData);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}
