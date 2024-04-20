// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckRange.generated.h"

UCLASS()
class MEMORYLAKE_API UBTService_CheckRange : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttachRangeKeyWithin;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttachRangeKeyLOS;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RangeDetected;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
