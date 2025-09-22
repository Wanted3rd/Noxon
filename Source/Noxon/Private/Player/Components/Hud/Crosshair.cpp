// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/Hud/Crosshair.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/Character.h"


class UCanvasPanelSlot;
// Sets default values for this component's properties
UCrosshair::UCrosshair()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bCanEverTick = true;
	ownerCharacter = Cast<ACharacter>(GetOwner());

	if (ownerCharacter)
	{
		PC = Cast<APlayerController>(ownerCharacter->GetController());
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TempCrossClass(TEXT("/Game/Player/Components/Hud/WBP_Crosshair.WBP_Crosshair_C"));
	if (TempCrossClass.Succeeded())
	{
		CrosshairClass = TempCrossClass.Class;
	}}


// Called when the game starts
void UCrosshair::BeginPlay()
{
	Super::BeginPlay();

	if (CrosshairClass)
	{
		CrosshairInstance = CreateWidget<UUserWidget>(GetWorld(), CrosshairClass);
		if (CrosshairInstance)
		{
			CrosshairInstance->AddToViewport();
		}
	}
	
}


// Called every frame
void UCrosshair::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetCrosshairPosition();
	// ...
}

void UCrosshair::SetCrosshairPosition()
{
	if (PC)
	{
		// 플레이어가 제어중인 방향
		FVector ForwardVector = PC->GetControlRotation().Vector();
		centerDirection = ForwardVector * 1;

		//카메라 위치, 회전 받아옴
		FVector cameraLocation;
		FRotator cameraRotation;
		PC->GetPlayerViewPoint(cameraLocation,cameraRotation);

		FVector crosshairLocation = ForwardVector + cameraLocation;
		FVector2D ScreenPos;

		//X, Y를 버리면 → 좌우/앞뒤 위치 정보가 사라져서, 화면상의 픽셀 위치를 못 구함.
		//Z를 버리면 → "깊이 정보"만 사라지고, 화면상 픽셀 위치는 유지됨.
		//카메라 위치벡터에서 플레이어가 나아갈 방향 (크기 1)이 크로스헤어가 위치하는 좌표 
		PC->ProjectWorldLocationToScreen(crosshairLocation,ScreenPos, true);
		centerPositionX = ScreenPos.X;
		centerPositionY = ScreenPos.Y;

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CrosshairInstance);
		CanvasSlot->SetPosition(ScreenPos);
		
	}
}

