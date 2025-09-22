// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EtcItems/GatherableRock.h"

#include "Components/WidgetComponent.h"
#include "Items/HandItems/PickAxe.h"


// Sets default values
AGatherableRock::AGatherableRock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// 메시 컴포넌트 생성
	RockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockMesh"));
	RootComponent = RockMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempRock(TEXT("/Game/Fab/Megascans/3D/Rock_shopk/Mid/Meshes/Rock_shopk_Mid.Rock_shopk_Mid"));
	if (TempRock.Succeeded())
	{
		RockMesh->SetStaticMesh(TempRock.Object);
	}
	
	// 3. 메시 자체 콜라이더를 이용하고 싶으면
	RockMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RockMesh->SetCollisionObjectType(ECC_WorldStatic);
	RockMesh->SetCollisionProfileName(TEXT("BlockAll"));

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UUserWidget> TempWidget(TEXT("/Game/UI/WBP_GatherableObjectHP.WBP_GatherableObjectHP"));
	if (TempWidget.Succeeded())
	{
		WidgetComp->SetWidgetClass(TempWidget.Class);
	}
	WidgetComp->SetRelativeScale3D(FVector(0.6, 0.25, 1));
	
}

// Called when the game starts or when spawned
void AGatherableRock::BeginPlay()
{
	Super::BeginPlay();

	RockMesh->OnComponentHit.AddDynamic(this, &AGatherableRock::OnHit);

}

// Called every frame
void AGatherableRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGatherableRock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, FVector NormalImpulse,
							const FHitResult& Hit)
{
	// if (OtherActor && OtherActor->IsA(APickAxe::StaticClass()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("돌이 곡괭이에 맞았다!"));
	// 	// 채집 로직 또는 데미지 처리
	// }
	
		m_Dele_hit.Broadcast(this, OtherActor);
} 