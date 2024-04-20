// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_SetRandomTag.h"

#include "AIController.h"
#include "MemoryLake.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_SetRandomTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	const APawn* MyPawn = MyController->GetPawn();
	if (!ensure(MyPawn))
	{
		return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* MyBlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!MyBlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}
	
	uint32 SumWeight{};
	for (const auto& [Weight, Index] : WeightTags)
	{
		if (!ensureMsgf(Weight, TEXT("[%d] Weight must be greater than zero!"), Index))
		{
			return EBTNodeResult::Failed;
		}
		
		SumWeight += Weight;
	}

	// Weighted Random
	const uint32 RandomNumber = FMath::RandHelper(SumWeight);
	uint32 CurrentNumber{};
	for (auto& [Weight, Index] : WeightTags)
	{
		CurrentNumber += Weight;
		if (RandomNumber < CurrentNumber)
		{
			MyBlackboardComponent->SetValueAsInt(AttachTagKey.SelectedKeyName, Index);

			for (auto& [MyWeight, MyIndex] : WeightTags)
			{
				if (Index != MyIndex)
				{
					++MyWeight;
				}
			}
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
