// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EtcItems/Ammo.h"

#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AAmmo::AAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//아이템 기본값 설정
	property.bQuantity = true;
	property.maxQuantity = 120;
	property.quantity = 0;
	property.bDurability = true;
	property.durability = 0.f;
	property.bMagazine = false;
	property.magazine = 30;
	property.deal = 10.f;
	property.heal = 0.f;
	
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AAmmo::SetActive(bool bActive)
{
}

void AAmmo::PostEditChangeProperty(
	struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// speed 값이 수정이 됐는지 체크
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		moveComp->InitialSpeed = speed;
		moveComp->MaxSpeed = speed;
	}
}


// public: // components
// UPROPERTY(VisibleAnywhere)
// class USphereComponent* collisionComp;
// UPROPERTY(VisibleAnywhere)
// class UStaticMeshComponent* bodyMesh;
// UPROPERTY(VisibleAnywhere)
// class UProjectileMovementComponent* moveComp;
//
// // 일정시간이 흐르면 없애고 싶다.
// UPROPERTY(EditAnywhere, Category=Settings)
// float destroyTime = 2;
// float currentTime = 0;
//
// UPROPERTY()
// class AMainPlayer* player;
// 	
// // 활성화 세팅 함수
// void SetActive(bool bActive);
//
// UPROPERTY(EditAnywhere)
// float speed = 10000;
//
// // 에디터에서 속성을 수정했을 때, 다른 속성들도 수정할 수 있게 하고 싶다.
// virtual  void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;	