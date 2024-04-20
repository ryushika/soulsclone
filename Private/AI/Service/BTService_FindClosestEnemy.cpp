// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_FindClosestEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <GameFramework/GameStates/MLGameState.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/DynamicCameraComponent.h"

void UBTService_FindClosestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return;
	}

	const UAIPerceptionComponent* PerceptionComponent = Controller->GetPerceptionComponent();
	if (!PerceptionComponent)
	{
		return;
	}

	TArray<AActor*> Enemies;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), Enemies);
	if (Enemies.IsEmpty())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, nullptr);
		AMLGameState* MLGameState = GetWorld()->GetGameState<AMLGameState>();
		if (!MLGameState)
		{
			return;
		}
		Cast<APlayerCharacter>(MLGameState->GetPlayer()->GetPawn())->DynamicCameraComponent->RemoveTarget(OwnerComp.GetAIOwner()->GetPawn());
		return;
	}

	const APawn* OwnerPawn = Controller->GetPawn();
	if (!OwnerPawn)
	{
		return;
	}

	float ClosestDistance = MAX_FLT;
	AActor* ClosestEnemy = nullptr;

	for (const auto& Enemy : Enemies)
	{
		UAbilitySystemComponent* EnemyASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Enemy);
		if (EnemyASC && !EnemyASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
		{
			const float Distance = FVector::Distance(Enemy->GetActorLocation(), OwnerPawn->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestEnemy = Enemy;
			}
		}
	}

	if (ClosestEnemy)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, ClosestEnemy);
		AMLGameState* MLGameState = GetWorld()->GetGameState<AMLGameState>();
		if (!MLGameState)
		{
			return;
		}
		Cast<APlayerCharacter>(MLGameState->GetPlayer()->GetPawn())->DynamicCameraComponent->AddTarget(OwnerComp.GetAIOwner()->GetPawn());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, nullptr);
		Controller->SetFocus(nullptr);
		AMLGameState* MLGameState = GetWorld()->GetGameState<AMLGameState>();
		if (!MLGameState)
		{
			return;
		}
		Cast<APlayerCharacter>(MLGameState->GetPlayer()->GetPawn())->DynamicCameraComponent->RemoveTarget(OwnerComp.GetAIOwner()->GetPawn());
	}
}
