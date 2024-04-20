// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SetRandomTag.generated.h"

USTRUCT(BlueprintType)
struct FWeightTags
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Weight", meta = (ToolTip = "Chance to cast a Spell.", ClampMin = 1))
	int32 Counts{1};

	UPROPERTY(EditAnywhere, Category = "Weight", meta = (ToolTip = "Index which Spell must be cast."))
	int32 Index;
};

UCLASS()
class MEMORYLAKE_API UBTTaskNode_SetRandomTag : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Tags")
	TArray<FWeightTags> WeightTags;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttachTagKey;
};
