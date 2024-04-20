// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_AttackViaTag.generated.h"

UCLASS()
class MEMORYLAKE_API UBTTaskNode_AttackViaTag : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (ToolTip = "Tag container must be contains one tag."))
	TArray<FGameplayTagContainer> Spells;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttachTagKey;
};
