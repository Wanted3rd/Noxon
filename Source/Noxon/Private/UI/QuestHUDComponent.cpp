#include "UI/QuestHUDComponent.h"
#include "UI/QuestWidget.h"
#include "GameFlow/GameMode/IngameGameState.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

UQuestHUDComponent::UQuestHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoUpdateQuests = true;
}

void UQuestHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	gameState = GetWorld()->GetGameState<AIngameGameState>();
	if (gameState)
	{
		gameState->OnActiveLevelChanged.AddDynamic(this, &UQuestHUDComponent::OnActiveLevelChanged);
		gameState->OnAllLevelsCleared.AddDynamic(this, &UQuestHUDComponent::OnAllLevelsCleared);
	}

	if (questWidgetClass)
	{
		questWidget = CreateWidget<UQuestWidget>(GetWorld(), questWidgetClass);
		if (questWidget)
		{
			questWidget->AddToViewport();
			questWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Initial quest display update
	if (bAutoUpdateQuests)
	{
		UpdateQuests();
	}
}

void UQuestHUDComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (gameState)
	{
		gameState->OnActiveLevelChanged.RemoveDynamic(this, &UQuestHUDComponent::OnActiveLevelChanged);
		gameState->OnAllLevelsCleared.RemoveDynamic(this, &UQuestHUDComponent::OnAllLevelsCleared);
	}

	if (questWidget)
	{
		questWidget->RemoveFromParent();
		questWidget = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void UQuestHUDComponent::ShowQuestWidget()
{
	if (questWidget)
	{
		questWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UQuestHUDComponent::HideQuestWidget()
{
	if (questWidget)
	{
		questWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UQuestHUDComponent::UpdateQuests()
{
	RefreshQuestDisplay();
}

void UQuestHUDComponent::OnActiveLevelChanged(FName newLevelName)
{
	UE_LOG(LogTemp, Log, TEXT("Active level changed to: %s"), *newLevelName.ToString());

	if (bAutoUpdateQuests)
	{
		RefreshQuestDisplay();
	}
}

void UQuestHUDComponent::OnAllLevelsCleared()
{
	UE_LOG(LogTemp, Log, TEXT("All levels cleared! Game finished!"));

	if (questWidget)
	{
		questWidget->ClearQuests();
		questWidget->SetLevelName(TEXT("게임 클리어!"));
		questWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UQuestHUDComponent::RefreshQuestDisplay()
{
	if (!questWidget || !gameState)
	{
		return;
	}

	TArray<FLevelCompleteCondition> currentQuests;
	bool hasQuests = gameState->GetCurrentLevelQuests(currentQuests);

	if (hasQuests && !currentQuests.IsEmpty())
	{
		// Update quest widget with current level's quests
		questWidget->UpdateQuestConditions(currentQuests);
		questWidget->SetLevelName(gameState->GetActiveLevelName().ToString());
		ShowQuestWidget();
	}
	else
	{
		// No quests in current level, hide quest widget
		HideQuestWidget();
	}
}