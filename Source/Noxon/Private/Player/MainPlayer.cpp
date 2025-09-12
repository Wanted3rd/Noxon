// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempArm(TEXT("/Game/Assets/Characters/FirstPersonArms/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms"));
	if (tempArm.Succeeded())
	{
		armMesh->SetSkeletalMesh(tempArm.Object);
	}
	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(armMesh, TEXT("headCameraSocket"));
	camera->SetRelativeLocation(FVector(0, 0, 0));
	camera->SetRelativeRotation(FRotator(0, 0, 0));
	camera->FieldOfView = 110.f;
	
	handItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ViewHandItemMesh"));
	handItemMesh->bOnlyOwnerSee = true;
	handItemMesh->CastShadow = false;
	handItemMesh->SetupAttachment(armMesh, TEXT("hand_rSocket"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempAK(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Gun/Ak47/SKM_AK_LP_correct.SKM_AK_LP_correct'"));
	if (tempAK.Succeeded())
	{
		handItemMesh->SetSkeletalMesh(tempAK.Object);
	}
	
	
	
	imc_mainplayer = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Player/Input/IMC_MainPlayer.IMC_MainPlayer.IMC_MainPlayer"));
	
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




