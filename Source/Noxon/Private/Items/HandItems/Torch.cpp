#include "Items/HandItems/Torch.h"


ATorch::ATorch()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATorch::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATorch::LeftAction()
{
}

void ATorch::RightAction()
{
}

