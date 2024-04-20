// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "MemoryLake.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AEnemyAIController::AEnemyAIController()
{
}

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnActorPerceptionUpdated);
	
	SetGenericTeamId(FGenericTeamId(1));
}

FVector AEnemyAIController::GetTargetEnemyVector() const
{
	const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return FVector::ZeroVector;

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
	if (!TargetActor) return FVector::ZeroVector;

	return TargetActor->GetActorLocation();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyAIController::OnActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	if (auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor))
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
		{
			return;
		}
	}

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
	if (!Target)
	{
		BlackboardComponent->SetValueAsObject("TargetActor", Actor);
		return;
	}
	
	const APawn* OwningPawn = GetPawn();
	if (!ensure(OwningPawn))
	{
		return;
	}

	const float DistanceBetweenArrivedActor = FVector::Distance(Actor->GetActorLocation(), OwningPawn->GetActorLocation());
	const float DistanceBetweenCurrentTarget = FVector::Distance(Target->GetActorLocation(), OwningPawn->GetActorLocation());
	const bool bCanRetarget = FMath::RandBool();

	if (bCanRetarget && DistanceBetweenArrivedActor < DistanceBetweenCurrentTarget)
	{
		BlackboardComponent->SetValueAsObject("TargetActor", Actor);
	}
}
