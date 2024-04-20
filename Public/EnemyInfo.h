#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EnemyInfo.generated.h"

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* MontageToPlay;
};