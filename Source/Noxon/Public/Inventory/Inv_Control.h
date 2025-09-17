#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_Control.generated.h"

class UInventory;
class UInventoryWidgetBase;
class UInputMappingContext;
class UInputAction;
class APlayerController;

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
	
protected:
	virtual void BeginPlay() override;

private:
	void ToggleInventory();

	void OpenInventory();

	void CloseInventory();

	void TraceItem();

	void CacheOwnerController();

	void CacheInventoryComponent();

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
	
private:
	bool bInventoryOpen = false;

	// Own
	TWeakObjectPtr<APlayerController> OwnCtrl;

	TWeakObjectPtr<UInventory> Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UInventoryWidgetBase> InventoryMenu;
	
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	
};
