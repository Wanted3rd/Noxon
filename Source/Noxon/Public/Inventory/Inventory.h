#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Key;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 quantity = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintPure)
	bool GetSlot (int32 index, FItemSlot& outSlot) const;
	
	UFUNCTION(BlueprintCallable)
	bool RegItem (FName itemkey, int32 quantity, int32& outadded, int32& outremainder);
	
	UFUNCTION(BlueprintCallable)
	bool RemoveItem (int32 slotindex, int32 quantity, int32& outremoved);

	UFUNCTION(BlueprintPure)
	bool GetDefinition (FName itemkey, const FItemBaseRow*& outrow) const;


protected:
	UPROPERTY()
	TArray<FItemSlot> Slots;
	
	UPROPERTY(EditDefaultsOnly)
	UDataTable* ItemTable = nullptr; // Table
};