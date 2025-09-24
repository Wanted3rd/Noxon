#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFlow/LevelStatus.h"
#include "QuestWidget.generated.h"

class UTextBlock;
class UVerticalBox;

UCLASS(BlueprintType, Blueprintable)
class NOXON_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateQuestConditions(const TArray<FLevelCompleteCondition>& conditions);

	UFUNCTION(BlueprintCallable)
	void ClearQuests();

	UFUNCTION(BlueprintCallable)
	void SetLevelName(const FString& levelName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuestUpdated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAllQuestsCompleted();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> QuestContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Display")
	TSubclassOf<UUserWidget> questItemWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Display")
	FLinearColor completedQuestColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Display")
	FLinearColor incompleteQuestColor = FLinearColor::White;

private:
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> questItemWidgets;

	void CreateQuestItemWidget(const FLevelCompleteCondition& condition);
	FString GetConditionDescription(const FLevelCompleteCondition& condition) const;
};