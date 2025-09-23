#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

class AHandItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedHandItemChanged, AHandItem*, HandItem);

#include "Inv_Control.generated.h"

class UInventory;
class UInventoryWidgetBase;
class UInputMappingContext;
class UInputAction;
class APlayerController;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UInv_Control : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_Control();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ConstructInventory();
	
	void Interact();

	void RefreshOwnerReferences();
	
	UFUNCTION()
	void HandleHandItemAcquired(FName DefId, AHandItem* Item);

	void HandleHotbarInput(const struct FInputActionValue& Value);
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void ToggleInventory();

	void OpenInventory();

	void CloseInventory();

	void TraceItem();

	void CacheOwnerController();

	void CacheInventoryComponent();

	void BindInventoryDelegates();
	void UnbindInventoryDelegates();
	AHandItem* EquipHotbarSlot(int32 Hotkey);
	void UpdateEquippedHandItem(AHandItem* NewItem);


public:
	// UI 관련
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventoryWidgetBase> InventoryMenuClass;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InvIMC;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> ToggleIA;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InteractIA;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> HotbarIA;

	UPROPERTY(EditAnywhere, Category = "HandItem")
	FName HandItemSocketName = TEXT("ik_hand_gun");
	
private:
	bool bInventoryOpen = false;

	// Own
	TWeakObjectPtr<APlayerController> OwnCtrl;

	TWeakObjectPtr<UInventory> Inventory;

	TWeakObjectPtr<AHandItem> CurrentHandItem;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UInventoryWidgetBase> InventoryMenu;
	
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	
public:
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnEquippedHandItemChanged OnEquippedHandItemChanged;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	AHandItem* GetCurrentHandItem() const { return CurrentHandItem.Get(); }
	
};
