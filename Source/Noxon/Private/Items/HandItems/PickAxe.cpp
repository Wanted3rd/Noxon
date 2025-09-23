// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HandItems/PickAxe.h"

#include "Items/EtcItems/GatherableRock.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickAxe::APickAxe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APickAxe::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGatherableRock::StaticClass(), FoundRocks);
	for (AActor* Actor : FoundRocks)
	{
		gatherableRock = Cast<AGatherableRock>(Actor);
		if (gatherableRock)
		{
			gatherableRock->m_Dele_hit.AddDynamic(this , &APickAxe::DamageRock);
		}
	}

	

}

// Called every frame
void APickAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void APickAxe::DamageRock(AGatherableRock* hitRock, AActor* pickAxe)
{
	hitRock->hp = hitRock->hp - 20;
}

