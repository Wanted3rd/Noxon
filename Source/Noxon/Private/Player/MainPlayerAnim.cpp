// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayerAnim.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/MainPlayer.h"

UMainPlayerAnim::UMainPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempFire(TEXT("/Script/Engine.AnimSequence'/Game/Assets/Characters/FirstPersonArms/Animations/FP_Rifle_Fire.FP_Rifle_Fire'"));
	if (tempFire.Succeeded())
	{
		attackMontage = tempFire.Object;
	}
}

void UMainPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//속도 구하기
	//Pawn -> Player
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<AMainPlayer>(ownerPawn);
	if (player)
	{
		//플레이어 진행방향
		FVector velocity = player->GetVelocity();
		//플레이어 앞 방향
		FVector forward = player->GetActorForwardVector();
		//플레이어 오른쪽 방향
		FVector right = player->GetActorRightVector();


		//진행방향과 forward vector가 양수라면 앞으로 가는 에니메이션, 음수면 뒤로 가는 에니메이션
		speed = FVector::DotProduct(velocity, forward);
		//진행방향과 right vector가 양수면 오른쪽, 음수면 왼쪽. 아마도?
		direction = FVector::DotProduct(velocity, right);


		//firstperson 에니메이션 용 코드
		auto cmp = player->GetCharacterMovement();
		isInAir = cmp->IsFalling();
		isMoving = velocity.Size() > 0.f;

		isAiming = player->bIsRigthClicking;
		isSprinting = player->bIsSprinting;
	}
}

void UMainPlayerAnim::PlayLeftActionAnimation()
{
	Montage_Play(attackMontage);
}
