#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFinishManager.generated.h"

class AIngameGameState;

UENUM(BlueprintType)
enum class EGameFinishType : uint8
{
	Victory UMETA(DisplayName = "Victory"),
	Defeat UMETA(DisplayName = "Defeat"),
	Quit UMETA(DisplayName = "Quit")
};

UCLASS()
class NOXON_API AGameFinishManager : public AActor
{
	GENERATED_BODY()

public:
	AGameFinishManager();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable)
	void TriggerGameFinish(EGameFinishType finishType);

	UFUNCTION(BlueprintCallable)
	void ReturnToMainMenu();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void QuitGame();

private:
	UFUNCTION()
	void OnAllLevelsCleared();
	
	void ShowGameFinishUI(EGameFinishType finishType);

	UFUNCTION()
	void DelayedGameFinish();
	


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Finish")
	float gameFinishDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Finish")
	bool bAutoShowFinishUI = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Finish")
	TSubclassOf<UUserWidget> gameFinishWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<AIngameGameState> gameState;

	UPROPERTY()
	TObjectPtr<UUserWidget> gameFinishWidget;

	bool bGameFinished = false;

		FTimerHandle gameFinishTimerHandle;
};