// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCalculation.h"
#include <GameplayEffectExecutionCalculation.h>
#include "GES_KnockBackCaclulation.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYLAKE_API UGES_KnockBackCaclulation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGES_KnockBackCaclulation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const override;
	
};
