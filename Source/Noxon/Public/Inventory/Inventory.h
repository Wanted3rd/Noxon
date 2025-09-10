#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Key = FName("None");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 quantity = 0;
};

UENUM(BlueprintType)
enum class EInvContainer : uint8
{
	Hotbar,
	Backpack
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	int32 HotbarSize = 9;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	int32 Rows = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	int32 Cols = 8;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemTabel")
	UDataTable* ItemTable = nullptr; // Table

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure)
	bool GetSlot (int32 index, FItemSlot& outslot) const;
	
	UFUNCTION(BlueprintCallable)
	bool AddItemInven (FName key, int32 quantity);
	
	UFUNCTION(BlueprintCallable)
	bool RemoveItem (int32 index, int32 quantity);

	UFUNCTION(BlueprintPure)
	bool GetDefinition (FName key) const;


protected:
	TArray<FItemSlot> Slots;
	
};