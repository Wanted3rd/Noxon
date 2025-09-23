#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DoveTestPlayer.generated.h"

class UInventory;
class UInv_Control;

UCLASS()
class NOXON_API ADoveTestPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    ADoveTestPlayer();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    UInventory* Inventory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    UInv_Control* InventoryControl;
};
