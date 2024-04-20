// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_ChangeMovementMode.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTT_ChangeMovementMode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyCharacter = Cast<ACharacter>(MyController->GetPawn());
		if (!ensure(MyCharacter))
		{
			return EBTNodeResult::Failed;
		}
		
		UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(MyCharacter->GetMovementComponent());
		if (ensure(MovementComponent))
		{
			if (MovementComponent->MovementMode != MovementMode)
			{
				MovementComponent->SetMovementMode(MovementMode, 0);
			}

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
