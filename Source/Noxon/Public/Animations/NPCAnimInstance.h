#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

class ABaseNonPlayableCharacter;

enum class EPhase : uint8;
enum class EDamageState : uint8;
enum class EMoveState : uint8;

UCLASS()
class NOXON_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
protected:
	virtual void NativeBeginPlay() override;
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ABaseNonPlayableCharacter* ownerNPC = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector localVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 phase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 damagedState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 moveState;
	
};
