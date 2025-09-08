#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class NOXON_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> icon = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName itemName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString description = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int quantity = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool stackable = false;
};
