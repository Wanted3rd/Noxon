// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/EtcItems/RifleDefaultAmmo.h"
#include "Items/HandItems/HandItem.h"
#include "Items/HandItems/RifleDemoGun.h"
#include "Player/Components/Hud/HudComponent.h"
#include "Utility/FindHelper.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hudComp = CreateDefaultSubobject<UHudComponent>(TEXT("HUDComponent"));

	
	USkeletalMesh* skm = BASE_SKM;
	if (IsValid(skm))
	{
		GetMesh()->SetSkeletalMesh(skm);
		GetMesh()->SetRelativeLocation(FVector(0,0, -87));
		GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	}

	// GetMesh()->SetOwnerNoSee(true);


	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArm->TargetArmLength = 0.f;
	springArm->bUsePawnControlRotation = true;
	springArm->bInheritPitch = true;
	springArm->bInheritYaw = true;
	springArm->bInheritRoll = false;
	springArm->bDoCollisionTest = false;
	springArm->SocketOffset = FVector(0.0,0.0,20.f);
	springArm->TargetOffset = FVector(0.0,0.0,40.f);
	springArm->SetupAttachment(RootComponent);

	armMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	armMesh->bOnlyOwnerSee = true;
	armMesh->CastShadow = false;
	armMesh->SetRelativeLocation(FVector(0, 0, -133.f));
	armMesh->SetRelativeRotation(FRotator(0, -90.f, 0));
	armMesh->SetupAttachment(springArm);
	
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempArm(TEXT("/Game/Assets/CustomAssets/FP_Mannequin/SK_FP_Manny_Simple.SK_FP_Manny_Simple/Game/Assets/CustomAssets/FP_Mannequin/SK_FP_Manny_Simple.SK_FP_Manny_Simple"));
	if (tempArm.Succeeded())
	{
		armMesh->SetSkeletalMesh(tempArm.Object);
	}
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(armMesh, TEXT("head"));
	camera->SetRelativeLocation(FVector(0, 0, 0));
	camera->SetRelativeRotation(FRotator(0, 0, 0));
	camera->FieldOfView = 90.f;
	
	handItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandItemMesh"));
	handItemMesh->bOnlyOwnerSee = true;
	handItemMesh->CastShadow = false;
	handItemMesh->SetupAttachment(armMesh, TEXT("ik_hand_gun"));
	
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Meshes/SK_AssaultRifle.SK_AssaultRifle"));
	if (tempRifle.Succeeded())
	{
		handItemMesh->SetSkeletalMesh(tempRifle.Object);
	}

	
	imc_mainplayer = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Player/Input/IMC_MainPlayer.IMC_MainPlayer.IMC_MainPlayer"));

	//임시 발사 에니메이션
	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempFireMontage(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Animations/A_FP_AssaultRifle_Fire_Montage.A_FP_AssaultRifle_Fire_Montage"));
	if (tempFireMontage.Succeeded())
	{
		fireMontage = tempFireMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> tempFireAimMontage(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Animations/A_FP_AssaultRifle_Fire_Aimed_Montage.A_FP_AssaultRifle_Fire_Aimed_Montage"));
	if (tempFireAimMontage.Succeeded())
	{
		fireAimMontage = tempFireAimMontage.Object;
	}

	
	static ConstructorHelpers::FObjectFinder<UInputAction> TempMove(TEXT("/Game/Player/Input/IA_Move.IA_Move"));

	if (TempMove.Succeeded())
	{
		ia_move = TempMove.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempTurn(TEXT("/Game/Player/Input/IA_Turn.IA_Turn"));
	if (TempTurn.Succeeded())
	{
		ia_turn = TempTurn.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempLookUp(TEXT("/Game/Player/Input/IA_LookUp.IA_LookUp"));
	if (TempLookUp.Succeeded())
	{
		ia_lookUp = TempLookUp.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempJump(TEXT("/Game/Player/Input/IA_Jump.IA_Jump"));
	if (TempJump.Succeeded())
	{
		ia_jump = TempJump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempSprint(TEXT("/Game/Player/Input/IA_Sprint.IA_Sprint"));
	if (TempSprint.Succeeded())
	{
		ia_sprint = TempSprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempLeftAction(TEXT("/Game/Player/Input/IA_LeftAction.IA_LeftAction"));
	if (TempLeftAction.Succeeded())
	{
		ia_leftAction = TempLeftAction.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TempRightAction(TEXT("/Game/Player/Input/IA_RightAction.IA_RightAction"));
	if (TempRightAction.Succeeded())
	{
		ia_rightAction = TempRightAction.Object;
	}

	handItemClass = ARifleDemoGun::StaticClass();

	bulletFactory = ARifleDefaultAmmo::StaticClass();

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if  (subsys)
		{
			subsys->AddMappingContext(imc_mainplayer, 0);
		}
	}

	GetCharacterMovement()->MaxAcceleration = 100000.f;           // 가속 거의 즉시
	GetCharacterMovement()->BrakingDecelerationWalking = 100000.f; // 즉시 멈춤
	GetCharacterMovement()->GroundFriction = 100.f;   

	// handItemMesh = GetWorld()->SpawnActor<AHandItem>(handItemClass);
	//인벤토리가 아직 없기에 하드코딩
	// AGun* SpawnedGun = GetWorld()->SpawnActor<AGun>(
	// 	handItemClass,
	// 	FVector::ZeroVector,
	// 	FRotator::ZeroRotator
	// );
	// if (SpawnedGun)
	// {
	// 	SpawnedGun->AttachToComponent(
	// 		armMesh,
	// 		FAttachmentTransformRules::SnapToTargetIncludingScale,
	// 		TEXT("ik_hand_gun")
	// 	);
	// 	UE_LOG(LogTemp, Log, TEXT("에러 ㄴ"))
	//
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Log, TEXT("에러"))
	// }
	//

	// 오브젝트 풀 만들기
	for (int i=0;i<bulletPoolSize;i++)
	{
		// 총알 만들기
		auto bullet = GetWorld()->SpawnActor<ARifleDefaultAmmo>(bulletFactory);
		// 탄창에 넣기
		bulletPool.Add(bullet);
		// 총알 비활성화
		bullet->SetActive(false);
	}
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlayerControlCalculate();
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	UEnhancedInputComponent* playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (playerInput)
	{
		playerInput->BindAction(ia_move, ETriggerEvent::Triggered, this, &AMainPlayer::MoveInput);
		playerInput->BindAction(ia_turn, ETriggerEvent::Triggered, this, &AMainPlayer::TurnInput);
		playerInput->BindAction(ia_lookUp, ETriggerEvent::Triggered, this, &AMainPlayer::LookUpInput);
		playerInput->BindAction(ia_jump, ETriggerEvent::Triggered, this, &AMainPlayer::JumpInput);
		playerInput->BindAction(ia_sprint, ETriggerEvent::Started, this, &AMainPlayer::StartSprintInput);
		playerInput->BindAction(ia_sprint, ETriggerEvent::Completed, this, &AMainPlayer::StopSprintInput);
		playerInput->BindAction(ia_rightAction, ETriggerEvent::Triggered, this, &AMainPlayer::StartRightActionInput);
		playerInput->BindAction(ia_rightAction, ETriggerEvent::Completed, this, &AMainPlayer::StopRightActionInput);

		playerInput->BindAction(ia_leftAction, ETriggerEvent::Started, this, &AMainPlayer::TriggerLeftActionInput);
		playerInput->BindAction(ia_leftAction, ETriggerEvent::Completed, this, &AMainPlayer::CompleteLeftActionInput);

	}
	
}

void AMainPlayer::MoveInput(const struct FInputActionValue& value)
{
	FVector2d v = value.Get<FVector2d>();


	direction.X = v.X;
	direction.Y = v.Y;
}

void AMainPlayer::TurnInput(const struct FInputActionValue& value)
{
	rot_yaw = value.Get<float>();
}

void AMainPlayer::LookUpInput(const struct FInputActionValue& value)
{
	rot_pitch = value.Get<float>();
}

void AMainPlayer::JumpInput(const struct FInputActionValue& value)
{
	Jump();
}

void AMainPlayer::StartSprintInput(const struct FInputActionValue& value)
{
	bIsSprinting = true;

}

void AMainPlayer::StopSprintInput(const struct FInputActionValue& value)
{
	bIsSprinting = false;
}

void AMainPlayer::StartRightActionInput(const struct FInputActionValue& value)
{
	bIsRigthClicking = true;
}

void AMainPlayer::StopRightActionInput(const struct FInputActionValue& value)
{
	bIsRigthClicking = false;

}

void AMainPlayer::TriggerLeftActionInput(const struct FInputActionValue& value)
{
	//첫 총알 발사 후 HoldingShoot은 true가 되고 총 발사 에니메이션 재생. 그리고 0.2초간 딜레이
	//첫 총알 발사 후 두번째 총알이 발사되기 전에 마우스좌클릭을 때면 HoldingShoot는 false가 된다.
	// 마우스 좌클릭 눌림 → HoldingShoot true
	bHoldingLeftClick = true;
	armMesh->GetAnimInstance()->Montage_Play(fireMontage);

	FireWeapon();
}

void AMainPlayer::CompleteLeftActionInput(const struct FInputActionValue& value)
{
	bHoldingLeftClick = false;

}


void AMainPlayer::PlayerControlCalculate()
{
	if (direction.X > 0 && direction.Y == 0 && bIsSprinting == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	
	//월드 좌표인 direction 입력값을 현재 컨트롤 로테이션 값에 맞게 변환
	direction = FTransform(YawRotation).TransformVector(direction);
	if (!direction.IsNearlyZero())
	{
		direction.Normalize();
	}
	AddControllerYawInput(rot_yaw);
	AddControllerPitchInput(rot_pitch);

	
	AddMovementInput(direction);


	rot_yaw = 0.0f;
	rot_pitch = 0.0f;
	direction = FVector::ZeroVector;
}

void AMainPlayer::FireWeapon()
{
	// 스프린트 중이면 발사하지 않음
	if (bIsSprinting)
	{
		return;
	}
	
	if (bHoldingLeftClick) // 연사 중복 방지
	{
		// 0.2초 뒤에 다시 발사 가능
		GetWorldTimerManager().SetTimer(
			FireRateHandle,
			this,
			&AMainPlayer::FireWeapon,
			0.2f,
			false
		);
		armMesh->GetAnimInstance()->Montage_Play(fireMontage);


		// 총알 만들어서 발사시키기
		auto firePosition = armMesh->GetSocketTransform(TEXT("headSocket"));

		// 탄창에서 가져와서 총알 발사하기
		if (bulletPool.Num() > 0)
		{

			auto bullet = bulletPool[0];
			bullet->SetActorTransform(firePosition);
			bullet->SetActive(true);
			// 탄창에서 총알 제거하기
			bulletPool.RemoveAt(0);
		}
		
	}
}




