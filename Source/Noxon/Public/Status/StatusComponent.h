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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float Max;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    float Current;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
    bool bActive;
};

UCLASS(ClassGroup=(Status), meta=(BlueprintSpawnableComponent))
class NOXON_API UStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UStatusComponent();
    
    UFUNCTION(BlueprintCallable, Category="Status")
    bool RegStatus(FName Name, float _Max = 100.f, float _Current = -1.f, bool _bActive = true);

    UFUNCTION(BlueprintPure, Category="Status")
    FStatus GetStatus(FName Name) const;

    UFUNCTION(BlueprintCallable, Category="Status")
    void SetStatus(FName Name, float _Max = -1.f, float _Current = -1.f, bool NewActive = true);

    UFUNCTION(BlueprintCallable, Category="Status")
    bool ApplyStatusDelta(FName Name, float Delta);

    
protected:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, FStatus> StatusMap;

    // 통지 헬퍼(인터페이스만 사용, 델리게이트 없음)
    /*UFUNCTION() void NotifyOwner_StatusChanged(FName Attribute, float OldValue, float NewValue);
    UFUNCTION() void NotifyOwner_Depleted(FName Attribute);*/
};
