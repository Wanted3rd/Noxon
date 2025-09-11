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

	//크로스헤어 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TSubclassOf<class UHudComponent> hudComp; 
	class UHudComponent* hudComp; 

	

	
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
	
public:
	UFUNCTION()
	void PlayerControlCalculate();
	
};
