#include "Inventory/Inv_Control.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Inventory/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InventoryWidgetBase.h"
#include "UObject/ConstructorHelpers.h"

UInv_Control::UInv_Control()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tmpimc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inventory/IMC_Inventory.IMC_Inventory'"));
	if (tmpimc.Succeeded())
	{
		InvIMC = tmpimc.Object.Get();
	}

	ConstructorHelpers::FObjectFinder<UInputAction> tmptoggleia(TEXT("/Script/EnhancedInput.InputAction'/Game/Inventory/IA_ToggleInventory.IA_ToggleInventory'"));
	if (tmptoggleia.Succeeded())
	{
		ToggleIA = tmptoggleia.Object.Get();
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> tmpinteractia(TEXT("/Script/EnhancedInput.InputAction'/Game/Inventory/IA_Interact.IA_Interact'"));
	if (tmpinteractia.Succeeded())
	{
		InteractIA = tmpinteractia.Object.Get();
	}
}

void UInv_Control::BeginPlay()
{
	Super::BeginPlay();

	RefreshOwnerReferences();

	if (!OwnCtrl.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UInv_Control::BeginPlay: controller not ready; will retry when refreshed."));
	}

	ConstructInventory();

	if (!OwnCtrl.IsValid())
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = OwnCtrl->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InvIMC != nullptr)
			{
				Subsystem->AddMappingContext(InvIMC.Get(), 0);
			}
		}
	}

	if (UInputComponent* InputComp = OwnCtrl->InputComponent)
	{
		if (UEnhancedInputComponent* EIComp = Cast<UEnhancedInputComponent>(InputComp))
		{
			if (ToggleIA != nullptr)
			{
				EIComp->BindAction(ToggleIA.Get(), ETriggerEvent::Started, this, &UInv_Control::ToggleInventory);
			}
			if (InteractIA != nullptr)
			{
				EIComp->BindAction(InteractIA.Get(), ETriggerEvent::Started, this, &UInv_Control::Interact);
			}
		}
	}
}

void UInv_Control::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceItem();
}

void UInv_Control::ConstructInventory()
{
	RefreshOwnerReferences();

	if (!OwnCtrl.IsValid() || !InventoryMenuClass)
	{
		return;
	}

	if (!IsValid(InventoryMenu))
	{
		InventoryMenu = CreateWidget<UInventoryWidgetBase>(OwnCtrl.Get(), InventoryMenuClass);
	}

	if (IsValid(InventoryMenu))
	{
		if (Inventory.IsValid())
		{
			// Share the inventory component with the UI so it can build and refresh slot widgets.
			InventoryMenu->InitializeInventory(Inventory.Get());
		}

		InventoryMenu->AddToViewport();
		CloseInventory();
	}
}

void UInv_Control::ToggleInventory()
{
	if (bInventoryOpen)
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

void UInv_Control::OpenInventory()
{
	if (!IsValid(InventoryMenu))
	{
		return;
	}

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryOpen = true;

	if (OwnCtrl.IsValid())
	{
		FInputModeGameAndUI InputMode;
		OwnCtrl->SetInputMode(InputMode);
		OwnCtrl->SetShowMouseCursor(true);
	}
}

void UInv_Control::CloseInventory()
{
	if (!IsValid(InventoryMenu))
	{
		return;
	}

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryOpen = false;

	if (OwnCtrl.IsValid())
	{
		FInputModeGameOnly InputMode;
		OwnCtrl->SetInputMode(InputMode);
		OwnCtrl->SetShowMouseCursor(false);
	}
}

void UInv_Control::TraceItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport))
	{
		UE_LOG(LogTemp, Warning, TEXT("traceItem not begin"));
		return;
	}

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2;
	FVector Start, Forward;
	if (OwnCtrl.IsValid())
	{
		if (!UGameplayStatics::DeprojectScreenToWorld(OwnCtrl.Get(), ViewportCenter, Start, Forward))
		{
			UE_LOG(LogTemp, Warning, TEXT("traceItem 178"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("traceItem 183"));
		return;
	}

	FVector End = Start + Forward * 1000.f;
	FHitResult Hit;
	FCollisionQueryParams Params;
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ItemTraceChannel, Params);
	LastActor = ThisActor;
	ThisActor = Hit.GetActor();

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Started tracing a new actor."));
	}

	if (LastActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped tracing last actor."));
	}
}

void UInv_Control::Interact()
{
	if (!Inventory.IsValid())
	{
		RefreshOwnerReferences();
	}

	UE_LOG(LogTemp, Warning, TEXT("Interact On"));
}

void UInv_Control::RefreshOwnerReferences()
{
	CacheOwnerController();
	CacheInventoryComponent();

	if (IsValid(InventoryMenu) && Inventory.IsValid())
	{
		// Re-inject the freshly cached inventory so the UI stays in sync after controller swaps.
		InventoryMenu->InitializeInventory(Inventory.Get());
	}
}

void UInv_Control::CacheOwnerController()
{
	OwnCtrl.Reset();

	if (AActor* OwnerActor = GetOwner())
	{
		if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
		{
			OwnCtrl = OwnerPawn->GetController<APlayerController>();
		}
		else if (APlayerController* AsController = Cast<APlayerController>(OwnerActor))
		{
			OwnCtrl = AsController;
		}
	}

	if (!OwnCtrl.IsValid())
	{
		if (AActor* OwnerActor = GetOwner())
		{
			if (APlayerController* OwnerPC = Cast<APlayerController>(OwnerActor))
			{
				OwnCtrl = OwnerPC;
			}
		}
	}
}

void UInv_Control::CacheInventoryComponent()
{
	Inventory.Reset();

	if (AActor* OwnerActor = GetOwner())
	{
		if (UInventory* Found = OwnerActor->FindComponentByClass<UInventory>())
		{
			Inventory = Found;
			return;
		}
	}

	if (OwnCtrl.IsValid())
	{
		if (APawn* Pawn = OwnCtrl->GetPawn())
		{
			if (UInventory* Found = Pawn->FindComponentByClass<UInventory>())
			{
				Inventory = Found;
			}
		}
	}
}
