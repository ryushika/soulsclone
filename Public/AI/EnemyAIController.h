// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class UAIPerceptionComponent;

UCLASS()
class MEMORYLAKE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	AEnemyAIController(const FObjectInitializer& ObjectInitializer);

	FVector GetTargetEnemyVector() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
	TObjectPtr<UAISenseConfig_Sight> SightConfig = nullptr;

	UFUNCTION()
	void OnActorPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
