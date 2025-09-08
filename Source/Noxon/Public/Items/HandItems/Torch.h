
#pragma once

#include "CoreMinimal.h"
#include "HandItem.h"
#include "Torch.generated.h"

UCLASS()
class NOXON_API ATorch : public AHandItem
{
	GENERATED_BODY()

public:
	ATorch();

	virtual void Tick(float DeltaTime) override;
	virtual void LeftAction() override;
	virtual void RightAction() override;

protected:
	virtual void BeginPlay() override;
	
};
