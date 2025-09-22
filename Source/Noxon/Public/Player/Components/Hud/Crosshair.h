// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crosshair.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UCrosshair : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCrosshair();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector centerDirection; //화면 가운데가 향하는 방향
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector viewLocation;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<ACharacter> ownerCharacter;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<APlayerController> PC;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float centerPositionX;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float centerPositionY;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Hud")
	TSubclassOf<class UUserWidget> CrosshairClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Hud")
	UUserWidget* CrosshairInstance;
	
	// UPROPERTY(meta = (BindWidget))
	// class UImage* Crosshair;


public:
	UFUNCTION(BlueprintCallable)
	void SetCrosshairPosition();
};
