// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "HandItem.generated.h"

UINTERFACE()
class UItemAction : public UInterface
{
	GENERATED_BODY()
};

class IItemAction
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void LeftAction() = 0;
	UFUNCTION()
	virtual void RightAction() = 0;
	UFUNCTION()
	virtual int RKeyAction(int inValue) = 0;
};


UCLASS()
class NOXON_API AHandItem : public ABaseItem, public IItemAction
{
	GENERATED_BODY()

public:
	AHandItem();

	virtual void LeftAction() override {}
	virtual void RightAction() override {}
	virtual int RKeyAction(int inValue) override { return 0; }


	//skeletalMesh와 leftMontage등은 자식클래스의 값을 들고있기에 부모함수로도 값을 자식의 값을 줄 수 있다.
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetAttachPointSocketName() {return attachPointSocketName;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMesh* GetSkeletalMesh() {return skeletalMesh;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAnimMontage* GetLeftMontage() {return leftMontage;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAnimMontage* GetRightMontage() {return rightMontage;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UAnimMontage* GetRKeyMontage() {return rKeyMontage;}


protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName attachPointSocketName = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> skeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* leftMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* rightMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* rKeyMontage = nullptr;


};
