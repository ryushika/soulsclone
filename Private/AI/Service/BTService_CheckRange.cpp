// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_CheckRange.h"

#include "AIController.h"
#include "MemoryLake.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	bool bWithinRange = DistanceTo < RangeDetected;

	bool bHasLOS = false;
	if (bWithinRange)
	{
		bHasLOS = MyController->LineOfSightTo(TargetActor);
	}

	BlackboardComponent->SetValueAsBool(AttachRangeKeyWithin.SelectedKeyName, bWithinRange);
	BlackboardComponent->SetValueAsBool(AttachRangeKeyLOS.SelectedKeyName, bHasLOS);
}
