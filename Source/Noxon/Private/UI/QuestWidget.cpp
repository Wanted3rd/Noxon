#include "UI/QuestWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/UserWidget.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UQuestWidget::UpdateQuestConditions(const TArray<FLevelCompleteCondition>& conditions)
{
	ClearQuests();

	if (conditions.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);

	bool allCompleted = true;
	for (const FLevelCompleteCondition& condition : conditions)
	{
		CreateQuestItemWidget(condition);
		if (!condition.IsCompleted())
		{
			allCompleted = false;
		}
	}

	OnQuestUpdated();

	if (allCompleted)
	{
		OnAllQuestsCompleted();
	}
}

void UQuestWidget::ClearQuests()
{
	if (QuestContainer)
	{
		QuestContainer->ClearChildren();
	}

	for (UUserWidget* widget : questItemWidgets)
	{
		if (widget)
		{
			widget->RemoveFromParent();
		}
	}
	questItemWidgets.Empty();
}

void UQuestWidget::SetLevelName(const FString& levelName)
{
	if (LevelNameText)
	{
		LevelNameText->SetText(FText::FromString(levelName));
	}
}

void UQuestWidget::CreateQuestItemWidget(const FLevelCompleteCondition& condition)
{
	if (!QuestContainer || !questItemWidgetClass)
	{
		return;
	}

	UUserWidget* questItemWidget = CreateWidget<UUserWidget>(this, questItemWidgetClass);
	if (!questItemWidget)
	{
		return;
	}

	// Find text block in the quest item widget to set the condition text
	if (UTextBlock* questText = Cast<UTextBlock>(questItemWidget->GetWidgetFromName(TEXT("QuestText"))))
	{
		FString conditionText = GetConditionDescription(condition);
		questText->SetText(FText::FromString(conditionText));

		// Set color based on completion status
		FLinearColor textColor = condition.IsCompleted() ? completedQuestColor : incompleteQuestColor;
		questText->SetColorAndOpacity(FSlateColor(textColor));
	}

	QuestContainer->AddChild(questItemWidget);
	questItemWidgets.Add(questItemWidget);
}

FString UQuestWidget::GetConditionDescription(const FLevelCompleteCondition& condition) const
{
	FString description = condition.conditionDescription;

	if (description.IsEmpty())
	{
		// Generate default description based on condition type
		switch (condition.conditionType)
		{
		case ELevelCompleteCondition::ReachToGoal:
			description = TEXT("목표 지점에 도달하세요");
			break;
		case ELevelCompleteCondition::FarmingMaterials:
			description = FString::Printf(TEXT("재료를 %d개 수집하세요"), condition.requiredCount);
			break;
		case ELevelCompleteCondition::CraftWeapons:
			description = FString::Printf(TEXT("무기를 %d개 제작하세요"), condition.requiredCount);
			break;
		case ELevelCompleteCondition::EliminateAllEnemy:
			description = FString::Printf(TEXT("적을 %d명 처치하세요"), condition.requiredCount);
			break;
		default:
			description = TEXT("알 수 없는 조건");
			break;
		}
	}

	// Add progress information
	if (condition.requiredCount > 1)
	{
		description += FString::Printf(TEXT(" (%d/%d)"), condition.currentCount, condition.requiredCount);
	}
	else if (condition.IsCompleted())
	{
		description += TEXT(" ✓");
	}

	return description;
}