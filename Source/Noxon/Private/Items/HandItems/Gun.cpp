// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HandItems/Gun.h"

#include "Items/EtcItems/Ammo.h"
#include "Utility/DebugHelper.h"


AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	//아이템 기본값 설정
	property.bQuantity = false;
	property.maxQuantity = 1;
	property.quantity = 0;
	property.bDurability = true;
	property.durability = 0.f;
	property.bMagazine = false;
	property.magazine = 30;
	property.deal = 10.f;
	property.heal = 0.f;
	
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(false);
	for (int i = 0; i < property.maxMagazine; ++i)
	{
		auto ammo = GetWorld()->SpawnActor<AAmmo>(ammoFactory);
		ammoPool.Add(ammo);
		ammo->SetActive(false);
	}
	fireDelay = 1 / fireRate;
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (delayDeltaTime > 0.f)
	{
		delayDeltaTime -= DeltaTime;
	}
}

void AGun::LeftAction()
{
	if (delayDeltaTime > 0.f)
	{
		return;
	}
	delayDeltaTime += fireDelay;
	FTransform muzzleTransform;
	if (IsValid(gunSKM->GetSkeletalMeshAsset()))
	{
		muzzleTransform = gunSKM->GetSocketTransform("muzzleSocket");
	}
	else if (IsValid(gunSM->GetStaticMesh()))
	{
		muzzleTransform = gunSKM->GetSocketTransform("muzzleSocket");
	}
	else
	{
		LOG_TEXT(TEXT("not Exist Socket"));
		return;
	}
	if (property.magazine > 0)
	{
		--property.magazine;
		
		if (AAmmo* firedAmmo = ammoPool.Pop())
		{
			firedAmmo->SetActive(true);
			firedAmmo->SetActorTransform(muzzleTransform);
			reloadingPool.Add(firedAmmo);
		}
		
	}
}

void AGun::RightAction()
{
	
}

int AGun::RKeyAction(int inValue)
{
	for (int i = 0; inValue > 0 && property.magazine < property.maxMagazine; ++i)
	{
		if (reloadingPool.IsEmpty())
		{
			LOG_TEXT(TEXT("pool size is not valid"));
			return -1;
		}
		++property.magazine;
		--inValue;
		ammoPool.Add(reloadingPool.Pop());
	}
	return inValue;
}

