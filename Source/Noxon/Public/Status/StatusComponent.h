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

/**
 * 아주 가벼운 싱글(로컬) 전용 Status 컴포넌트
 * - 속성은 전부 FName 키로 등록/조회 (예: "Health", "Attack", "Hunger")
 * - 컴포넌트 스스로 속성 집합을 정의하지 않음. 오너가 필요한 것만 Register
 * - 공격 파이프라인: (Attacker=this, Tool=옵션) → Target에 공격력 전달
 *   - Attacker: Hunger 소모
 *   - Tool: Health(=내구도) 소모
 *   - Target: Health 감소
 */
UCLASS(ClassGroup=(Status), meta=(BlueprintSpawnableComponent))
class NOXON_API UStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UStatusComponent();

    // 기본 속성 이름(에디터에서 변경 가능)
    UPROPERTY(EditDefaultsOnly, Category="Status|Names")
    FName AttrHealth = TEXT("Health");

    UPROPERTY(EditDefaultsOnly, Category="Status|Names")
    FName AttrAttack = TEXT("Attack");

    UPROPERTY(EditDefaultsOnly, Category="Status|Names")
    FName AttrHunger = TEXT("Hunger");

    // -------- 속성 등록/해제 (오너가 필요한 것만 선택적으로 활성화) --------
    /*UFUNCTION(BlueprintCallable, Category="Status|Attributes")
    bool RegisterAttribute(FName Name, float InitialValue, float MinValue = 0.f, float MaxValue = 100.f, bool bClampToRange = true);

    UFUNCTION(BlueprintCallable, Category="Status|Attributes")
    bool UnregisterAttribute(FName Name);

    UFUNCTION(BlueprintPure, Category="Status|Attributes")
    bool HasAttribute(FName Name) const;

    UFUNCTION(BlueprintPure, Category="Status|Attributes")
    bool GetAttribute(FName Name, float& OutValue) const;

    UFUNCTION(BlueprintPure, Category="Status|Attributes")
    bool GetRange(FName Name, float& OutMin, float& OutMax) const;
    */

    // -------- 변경 API: 모든 상태 변화는 이 경로로만(이벤트/임계치 일원화) --------
    /*UFUNCTION(BlueprintCallable, Category="Status|Modify")
    bool SetAttribute(FName Name, float NewValue);

    UFUNCTION(BlueprintCallable, Category="Status|Modify")
    bool AddAttribute(FName Name, float Delta);

    UFUNCTION(BlueprintPure, Category="Status|Query")
    float GetPercent(FName Name) const;

    UFUNCTION(BlueprintPure, Category="Status|Query")
    bool IsDepleted(FName Name) const;
    */

    // -------- 공격 처리(플레이어+도끼→나무) --------
    // this = Attacker의 StatusComponent
    // - Target: 피격 대상
    // - Tool: 사용 도구(없으면 nullptr)
    // - HungerCost: 공격 시 Attacker가 소모하는 허기량
    // - ToolSelfDamage: 공격 시 Tool이 소모하는 체력(내구)
    // - DamageScale: (Attacker.Attack + Tool.Attack) * DamageScale가 타겟의 체력에서 빠짐
    /*UFUNCTION(BlueprintCallable, Category="Status|Combat")
    bool Attack(UStatusComponent* Target, UStatusComponent* Tool, float HungerCost = 5.f, float ToolSelfDamage = 1.f, float DamageScale = 1.f);
    */

protected:
    // 내부 저장소: 값/범위/클램프 여부
    UPROPERTY() TMap<FName, float> Values;            // 현재 값
    UPROPERTY() TMap<FName, FVector2f> Ranges;        // X=Min, Y=Max
    UPROPERTY() TSet<FName> ClampedAttributes;        // 범위 클램프 적용 대상

    // 통지 헬퍼(인터페이스만 사용, 델리게이트 없음)
    /*UFUNCTION() void NotifyOwner_StatusChanged(FName Attribute, float OldValue, float NewValue);
    UFUNCTION() void NotifyOwner_Depleted(FName Attribute);*/
};
