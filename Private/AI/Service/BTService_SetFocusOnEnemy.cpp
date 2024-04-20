// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SetFocusOnEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTService_SetFocusOnEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!Blackboard || !AIController)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyKey.SelectedKeyName));

	AIController->SetFocus(TargetActor);
}
