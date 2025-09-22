// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UMainPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerAnim)
	float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	float direction = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	bool isInAir = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	bool isMoving = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	bool isAiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	bool isSprinting = false;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=PlayerAnim)
	class UAnimMontage* attackMontage;

	//공격 에니메이션 재생 함수
	void PlayLeftActionAnimation();
	
};
