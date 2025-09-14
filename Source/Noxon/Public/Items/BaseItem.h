#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"


USTRUCT(BlueprintType)
struct FItemProperty
{
    GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> icon = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName itemName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString description = "";

	// 스택 가능한 양
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bQuantity = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int quantity = 0;
	// 내구도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bDurability = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float durability = 0.f;
	// 총알
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMagazine = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int magazine = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float deal = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float heal = 0.f;
};

UCLASS()
class NOXON_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();

	UFUNCTION(BlueprintPure)
	const FItemProperty& GetItemProperty() const {return property;}

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FItemProperty property = FItemProperty();
	
};
