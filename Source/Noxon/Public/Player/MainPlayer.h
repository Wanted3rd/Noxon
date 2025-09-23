// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class NOXON_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//캐릭터 컴포넌트 계층 구조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> springArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> armMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> handItemMesh;
	
	// 이동 관련@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	float speed;
	FVector direction;
	float rot_yaw;
	float rot_pitch;
	bool bIsSprinting = false;

	

	// 마우스 관련
	bool bIsRigthClicking = false;
	bool bHoldingLeftClick = false;  // 마우스 좌클릭 여부
	bool bIsShooting = false;        // 현재 발사 중인지
	FTimerHandle FireRateHandle;     // 발사 딜레이용 타이머

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	class UAnimMontage* fireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	class UAnimMontage* fireAimMontage;
	
	//크로스헤어 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TSubclassOf<class UHudComponent> hudComp; 
	class UHudComponent* hudComp; 

	

	//HandItem
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AHandItem> handItemClass;

	UPROPERTY(VisibleAnywhere)
	class AHandItem* HandItem = nullptr;
	
	
	//Input Action - IA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* imc_mainplayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_turn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_lookUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_jump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_sprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_leftAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ia_rightAction;

	
public:
	//bindingFunction
	UFUNCTION()
	void MoveInput(const struct FInputActionValue& value);

	UFUNCTION()
	void TurnInput(const struct FInputActionValue& value);

	UFUNCTION()
	void LookUpInput(const struct FInputActionValue& value);

	UFUNCTION()
	void JumpInput(const struct FInputActionValue& value);

	UFUNCTION()
	void StartSprintInput(const struct FInputActionValue& value);
	
	UFUNCTION()
	void StopSprintInput(const struct FInputActionValue& value);

	UFUNCTION()
	void StartRightActionInput(const struct FInputActionValue& value);

	UFUNCTION()
	void StopRightActionInput(const struct FInputActionValue& value);

	UFUNCTION()
	void TriggerLeftActionInput(const struct FInputActionValue& value);

	UFUNCTION()
	void CompleteLeftActionInput(const struct FInputActionValue& value);
	
	// UFUNCTION()
	// void StartLeftActionInput(const struct FInputActionValue& value);

	// UFUNCTION()
	// void StopLeftActionI
public:
	UFUNCTION()
	void PlayerControlCalculate();

	UFUNCTION()
	void FireWeapon();

	// UFUNCTION()
	// void ResetFire();

public:
	//temp

	// 총알공장
	UPROPERTY(EditDefaultsOnly, Category=Bullet)
	TSubclassOf<class ARifleDefaultAmmo> bulletFactory;
	
	
	
	// Object Pool
	// 몇발
	UPROPERTY(EditAnywhere, Category=Bullet)
	int32 bulletPoolSize = 30;

	// 탄창
	UPROPERTY()
	TArray<class ARifleDefaultAmmo*> bulletPool;
};
