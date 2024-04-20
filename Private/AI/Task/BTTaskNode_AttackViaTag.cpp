// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_AttackViaTag.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "MemoryLake.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_AttackViaTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = MyController->GetPawn();
	if (!ensure(MyPawn))
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(MyPawn)->GetAbilitySystemComponent();
	if (!ensure(ASC))
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* TargetASC = Cast<IAbilitySystemInterface>(TargetActor)->GetAbilitySystemComponent();
	if (!TargetASC || TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
	{
		return EBTNodeResult::Failed;
	}

	if (Spells.IsEmpty())
	{
		return EBTNodeResult::Failed;
	}

	const int32 Index = Blackboard->GetValueAsInt(AttachTagKey.SelectedKeyName);
	const FGameplayTagContainer AttackTags = Spells[Index];
	
	return ASC->TryActivateAbilitiesByTag(AttackTags) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
