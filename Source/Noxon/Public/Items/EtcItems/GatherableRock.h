// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GatherableRock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_Hit, AGatherableRock*, hitRock, AActor*, pickAxe);

UCLASS()
class NOXON_API AGatherableRock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGatherableRock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RockMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* WidgetComp;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Blueprintable);
	FDele_Hit m_Dele_hit;


	// 피격시 호출될 이벤트 함수 -> 콜백
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM, meta=(AllowPrivateAccess=true))
	int32 MAX_HP = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=FSM)
	int32 hp = MAX_HP;
	
public:
	// 헤더
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			   const FHitResult& Hit);
	
};
