#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NPCAnimInstance.generated.h"

enum class EDefaultsStates : uint8;
class ABaseNonPlayableCharacter;

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
	EDefaultsStates state;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector velocity;
};
