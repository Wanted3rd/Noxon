#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFlow/LevelStatus.h"
#include "QuestHUDComponent.generated.h"

class UQuestWidget;
class AIngameGameState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UQuestHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestHUDComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable)
	void ShowQuestWidget();

	UFUNCTION(BlueprintCallable)
	void HideQuestWidget();

	UFUNCTION(BlueprintCallable)
	void UpdateQuests();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest HUD")
	TSubclassOf<UQuestWidget> questWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest HUD")
	bool bAutoUpdateQuests = true;

private:
	UPROPERTY()
	TObjectPtr<UQuestWidget> questWidget;

	UPROPERTY()
	TObjectPtr<AIngameGameState> gameState;

	UFUNCTION()
	void OnActiveLevelChanged(FName newLevelName);

	UFUNCTION()
	void OnAllLevelsCleared();

	void RefreshQuestDisplay();
};