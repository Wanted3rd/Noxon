#include "GameFlow/GameFinishManager.h"
#include "GameFlow/GameMode/IngameGameState.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Utility/DebugHelper.h"
#include "Utility/FindHelper.h"

AGameFinishManager::AGameFinishManager()
{
	PrimaryActorTick.bCanEverTick = false;
	gameFinishDelay = 2.0f;
	bAutoShowFinishUI = true;
	bGameFinished = false;

	gameFinishWidget = FinderHelper::FindAssetFromConstructor<UUserWidget>("/Game/UI/Map/WBP_Finish.WBP_Finish");	
}

void AGameFinishManager::BeginPlay()
{
	Super::BeginPlay();

	if (gameFinishWidget != nullptr) {LOG_TEXT(TEXT("%s"), *gameFinishWidget->GetDisplayLabel())}

	gameState = GetWorld()->GetGameState<AIngameGameState>();
	if (gameState)
	{
		gameState->OnAllLevelsCleared.AddDynamic(this, &AGameFinishManager::OnAllLevelsCleared);
	}
}

void AGameFinishManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (gameState)
	{
		gameState->OnAllLevelsCleared.RemoveDynamic(this, &AGameFinishManager::OnAllLevelsCleared);
	}

	if (gameFinishWidget)
	{
		gameFinishWidget->RemoveFromParent();
		gameFinishWidget = nullptr;
	}

	GetWorld()->GetTimerManager().ClearTimer(gameFinishTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void AGameFinishManager::TriggerGameFinish(EGameFinishType finishType)
{
	if (bGameFinished)
	{
		return;
	}

	bGameFinished = true;
	UE_LOG(LogTemp, Log, TEXT("Game finished with type: %s"), *UEnum::GetDisplayValueAsText(finishType).ToString());

	if (bAutoShowFinishUI)
	{
		if (gameFinishDelay > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(
				gameFinishTimerHandle,
				[this, finishType]() { ShowGameFinishUI(finishType); },
				gameFinishDelay,
				false
			);
		}
		else
		{
			ShowGameFinishUI(finishType);
		}
	}
}

void AGameFinishManager::ReturnToMainMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Returning to main menu"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void AGameFinishManager::RestartGame()
{
	UE_LOG(LogTemp, Log, TEXT("Restarting game"));
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (AGameModeBase* gameMode = GetWorld()->GetAuthGameMode())
		{
			gameMode->RestartPlayer(playerController);
		}
	}
}

void AGameFinishManager::QuitGame()
{
	UE_LOG(LogTemp, Log, TEXT("Quitting game"));
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void AGameFinishManager::OnAllLevelsCleared()
{
	UE_LOG(LogTemp, Log, TEXT("All levels cleared! Triggering victory"));
	TriggerGameFinish(EGameFinishType::Victory);
}

void AGameFinishManager::ShowGameFinishUI(EGameFinishType finishType)
{
	if (gameFinishWidgetClass)
	{
		gameFinishWidget = CreateWidget<UUserWidget>(GetWorld(), gameFinishWidgetClass);
		if (gameFinishWidget)
		{
			gameFinishWidget->AddToViewport(1000); // High Z-order to appear on top

			// Pause the game
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// Show cursor for UI interaction
			if (APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				playerController->bShowMouseCursor = true;
				playerController->SetInputMode(FInputModeUIOnly());
			}

			UE_LOG(LogTemp, Log, TEXT("Game finish UI shown"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game finish widget class not set"));
	}
}

void AGameFinishManager::DelayedGameFinish()
{
	ShowGameFinishUI(EGameFinishType::Victory);
}