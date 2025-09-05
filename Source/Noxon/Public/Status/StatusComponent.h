// StatusComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/Interface.h"
#include "StatusComponent.generated.h"

/**
 * 오너에게 상태 변화/고갈만 통지하는 아주 단순한 인터페이스
 * - OnStatusChanged: 어떤 속성이 어떻게 바뀌었는지
 * - OnStatusDepleted: 어떤 속성이 최소값(보통 0)에 도달했는지
 */
/*
UINTERFACE(BlueprintType)
class NOXON_API UStatusNotifyTarget : public UInterface
{
    GENERATED_BODY()
};

class NOXON_API IStatusNotifyTarget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnStatusChanged(class UStatusComponent* Comp, FName Attribute, float OldValue, float NewValue);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnStatusDepleted(class UStatusComponent* Comp, FName Attribute);
};
*/

USTRUCT(BlueprintType)
struct NOXON_API FStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float Max = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    float Current = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
    bool bActive = false;
};

UCLASS(ClassGroup=(Status), meta=(BlueprintSpawnableComponent))
class NOXON_API UStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UStatusComponent();
    
    UFUNCTION(BlueprintCallable, Category="Status")
    bool RegStatus(FName Name, float Current, float Max, bool bActive = true);

    UFUNCTION(BlueprintPure, Category="Status")
    FStatus GetStatus(FName Name) const;
    

protected:
    // 내부 저장소: 값/범위/클램프 여부
    UPROPERTY() TMap<FName, FStatus> StatusMap;

    // 통지 헬퍼(인터페이스만 사용, 델리게이트 없음)
    /*UFUNCTION() void NotifyOwner_StatusChanged(FName Attribute, float OldValue, float NewValue);
    UFUNCTION() void NotifyOwner_Depleted(FName Attribute);*/
};
