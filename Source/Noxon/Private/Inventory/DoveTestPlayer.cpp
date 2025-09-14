// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/DoveTestPlayer.h"
#include "Inventory/Inventory.h"


// Sets default values
ADoveTestPlayer::ADoveTestPlayer()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 인벤토리 컴포넌트 생성 및 소유자에 부착
    Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
}

// Called when the game starts or when spawned
void ADoveTestPlayer::BeginPlay()
{
    Super::BeginPlay();
    // 게임 시작 시 인벤토리 슬롯 초기화
    if (Inventory)
    {
        Inventory->InitializeSlots(Inventory->NumRows, Inventory->NumCols);
    }
}

// Called every frame
void ADoveTestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADoveTestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

