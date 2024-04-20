

#include "Components/FadeControllerComponent.h"
#include "Components/FadeMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UFadeControllerComponent::UFadeControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFadeControllerComponent::BeginPlay()
{
	Super::BeginPlay();
		
	SetupFadeController();
}

void UFadeControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFadeController();
}

void UFadeControllerComponent::SetupFadeController()
{
	OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
		if (OwnerPlayerController)
		{
			OwnerCameraManager = OwnerPlayerController->PlayerCameraManager;
		}
	}
}

void UFadeControllerComponent::UpdateFadeController()
{
	if (OwnerPawn && OwnerCameraManager)
	{
		TArray<UFadeMeshComponent*> DesiredList = GetFadeMeshes();

		if (DesiredList == CurrentFadeMeshesList)
		{
			return;
		}

		EndFadeForMissing(DesiredList);
		BeginFadeForNew(DesiredList);
	}
}

TArray<UFadeMeshComponent*> UFadeControllerComponent::GetFadeMeshes()
{
	TArray<UFadeMeshComponent*> FadeMeshes;

	if (OwnerPawn && OwnerCameraManager)
	{
		TArray<FHitResult> HitResults = GetHitsBetweenCameraAndOwner();
		FadeMeshes = GetFadeMeshesFromHits(HitResults);
	}

	return FadeMeshes;
}

void UFadeControllerComponent::EndFadeForMissing(const TArray<UFadeMeshComponent*>& DesiredList)
{
	TArray<UFadeMeshComponent*> CurrentList = CurrentFadeMeshesList;

	for (UFadeMeshComponent* FadeMesh : CurrentList)
	{
		if (!DesiredList.Contains(FadeMesh))
		{
			FadeMesh->EndFade();
			CurrentFadeMeshesList.Remove(FadeMesh);
		}
	}
}

void UFadeControllerComponent::BeginFadeForNew(const TArray<UFadeMeshComponent*>& DesiredList)
{
	for (UFadeMeshComponent* FadeMesh : DesiredList)
	{
		if (!CurrentFadeMeshesList.Contains(FadeMesh))
		{
			FadeMesh->BeginFade();
			CurrentFadeMeshesList.Add(FadeMesh);
		}
	}
}

TArray<UFadeMeshComponent*> UFadeControllerComponent::GetFadeMeshesFromHits(const TArray<FHitResult>& HitResults)
{
	TArray<UFadeMeshComponent*> FadeMeshes;

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* Actor = HitResult.GetActor();

		if (Actor->ActorHasTag("CanFade"))
		{
			UActorComponent* ActorComponent = Actor->GetComponentByClass(UFadeMeshComponent::StaticClass());
			if (ActorComponent)
			{
				UFadeMeshComponent* FadeMesh = Cast<UFadeMeshComponent>(ActorComponent);
				if (FadeMesh)
				{
					FadeMeshes.Add(FadeMesh);
				}
			}
		}
	}

	return FadeMeshes;
}

TArray<FHitResult> UFadeControllerComponent::GetHitsBetweenCameraAndOwner()
{
	TArray<FHitResult> HitResults;

	FVector Start = OwnerCameraManager->GetCameraLocation();
	FVector End = OwnerPawn->GetActorLocation();

	// So that the sphere trace doesn't start behind the player
	End = End + (Start - End).GetSafeNormal() * FadeRadius;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerPawn);

	UKismetSystemLibrary::SphereTraceMulti
	(
		OwnerPawn,
		Start,
		End,
		FadeRadius,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	return HitResults;
}
