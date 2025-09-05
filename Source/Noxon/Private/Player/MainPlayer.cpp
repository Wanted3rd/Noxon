// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MainPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	imc_mainplayer = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Player/Input/IMC_MainPlayer.IMC_MainPlayer.IMC_MainPlayer"));
	
	static ConstructorHelpers::FObjectFinder<UInputAction> TempMove(TEXT("/Game/Player/Input/IA_Move.IA_Move"));

	if (TempMove.Succeeded())
	{
		ia_move = TempMove.Object;
	}

	speed = 500.0f;
	
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
	}
	
	
}

void AMainPlayer::MoveInput(const struct FInputActionValue& value)
{
	FVector2d v = value.Get<FVector2d>();

	
	direction.X = v.X;
	direction.Y = v.Y;
}

void AMainPlayer::PlayerControlCalculate()
{
	FVector CurLocation = GetActorLocation();
	SetActorLocation(CurLocation + direction * speed * GetWorld()->GetDeltaSeconds());
	direction = FVector::ZeroVector;

}


