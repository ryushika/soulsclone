// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_Attack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "GAS/CharacterAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	return ASC->TryActivateAbilitiesByTag(AttackTags) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
