// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DynamicCameraComponent.h"
#include <Characters/PlayerCharacter.h>
#include <Characters/BaseCharacter.h>
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UDynamicCameraComponent::UDynamicCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDynamicCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<APlayerCharacter>(GetOwner());
	AddTarget(Owner);
	CameraPosition = Owner->GetActorLocation();
	BoomLength = Owner->CameraBoom->TargetArmLength;
}

void UDynamicCameraComponent::SetCameraPosition()
{
	CameraPosition = FMath::Lerp(CameraPosition, FindCameraPosition(), 0.1);
	Owner->CameraBoom->SetWorldLocation(CameraPosition);
	BoomLength = FMath::Lerp(Owner->CameraBoom->TargetArmLength, FindCameraBoomDistacne(), 0.05);
	Owner->CameraBoom->TargetArmLength = BoomLength;
}

FVector UDynamicCameraComponent::FindCameraPosition()
{
	float x = 0;
	float y = 0;
	float z = 0;
	for (int i = 0; i < CameraTargets.Num(); i++)
	{
		x += CameraTargets[i]->GetActorLocation().X;
		y += CameraTargets[i]->GetActorLocation().Y;
		z += CameraTargets[i]->GetActorLocation().Z;
	}
	x /= CameraTargets.Num();
	y /= CameraTargets.Num();
	z /= CameraTargets.Num();

	return FVector(x, y, z);
}

float UDynamicCameraComponent::FindCameraBoomDistacne()
{
	float FarrestTargetDistance = 0.f;
	for (AActor* Target : CameraTargets)
	{
		float Distance = FVector::Distance(Target->GetActorLocation(), CameraPosition);
		if (Distance > FarrestTargetDistance)
		{
			FarrestTargetDistance = Distance;
		}
	}
	float NewBoomLength = FarrestTargetDistance * BoomLenghtMult;
	if (NewBoomLength <= MinBoomLength)
	{
		return MinBoomLength;
	}
	else if (NewBoomLength >= MaxBoomLength)
	{
		return MaxBoomLength;
	}
	return NewBoomLength;
}

void UDynamicCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bActiveDynamicCamera)
	{
		SetCameraPosition();
	}
}

void UDynamicCameraComponent::AddTarget(AActor* NewTarget)
{
	CameraTargets.AddUnique(NewTarget);
}

void UDynamicCameraComponent::RemoveTarget(AActor* NewTarget)
{
	CameraTargets.Remove(NewTarget);
}

void UDynamicCameraComponent::ClearTargets()
{
	CameraTargets.Empty();
}

void UDynamicCameraComponent::ScanForEnemies()
{
	TArray<AActor*> PiecesArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), PiecesArray);

	for (AActor* Actor : PiecesArray)
	{
		if (CameraTargets.Find(Actor) == INDEX_NONE)
		{
			AddTarget(Actor);
		}
	}
}