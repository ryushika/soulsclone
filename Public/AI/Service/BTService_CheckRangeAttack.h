// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckRangeAttack.generated.h"

UCLASS()
class MEMORYLAKE_API UBTService_CheckRangeAttack : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttachRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MinRangeAttack = 0.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxRangeAttack;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
