// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/Hud/HudComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/Character.h"
#include "Player/Components/Hud/Crosshair.h"


// Sets default values for this component's properties
UHudComponent::UHudComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	CrosshairComp = CreateDefaultSubobject<UCrosshair>(TEXT("Crosshair"));
}


// Called when the game starts
void UHudComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UHudComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}



