// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandItem.h"
#include "Gun.generated.h"

class AAmmo;

UCLASS()
class NOXON_API AGun : public AHandItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float reloadTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float fireRate;

	UPROPERTY()
	float fireDelay;
	float delayDeltaTime;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAutomatic; // 자동/단발여부
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float equipTime; //무기 꺼내는 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float unEquipTime; //무기 집어넣는 속도
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float bodyDmg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float headshotDmg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float legDmg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float armDmg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float handDmg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float footDmg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> gunSKM;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> gunSM;
	
	//총알 관련 값
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float power; //총알 나가는 거리
	float range; //유효사거리
	float bulletSpread; //탄 퍼짐 정도
	float recoil;
	
	UPROPERTY()
	TArray<AAmmo*> ammoPool;
	UPROPERTY()
	TArray<AAmmo*> reloadingPool;
	TSubclassOf<AAmmo> ammoFactory;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LeftAction() override;
	virtual void RightAction() override;
	virtual int RKeyAction(int inValue) override;
	
		
};

//bullet