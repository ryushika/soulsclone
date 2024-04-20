// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_CheckRangeAttack.h"

#include "AIController.h"
#include "MemoryLake.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckRangeAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent))
	{
		return;
	}
	
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
	if (!TargetActor)
	{
		return;
	}

	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController))
	{
		return;
	}

	APawn* AIPawn = MyController->GetPawn();
	if (!ensure(AIPawn))
	{
		return;
	}

	float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

	bool bWithinRangeAttack = DistanceTo < MaxRangeAttack && DistanceTo > MinRangeAttack;

	BlackboardComponent->SetValueAsBool(AttachRangeKey.SelectedKeyName, bWithinRangeAttack);
}
