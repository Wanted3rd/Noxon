#include "Items/BaseItem.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}
