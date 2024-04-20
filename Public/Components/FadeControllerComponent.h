
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FadeControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORYLAKE_API UFadeControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFadeControllerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//  Functions

public:

protected:

private:
	void SetupFadeController();
	void UpdateFadeController();

	TArray<class UFadeMeshComponent*> GetFadeMeshes();

	void EndFadeForMissing(const TArray<UFadeMeshComponent*>& DesiredList);
	void BeginFadeForNew(const TArray<UFadeMeshComponent*>& DesiredList);

	TArray<UFadeMeshComponent*> GetFadeMeshesFromHits(const TArray<FHitResult>& HitResults);
	TArray<FHitResult> GetHitsBetweenCameraAndOwner();

//  Variables

public:

protected:

private:
	UPROPERTY(EditAnywhere)
	float FadeRadius = 200.0f;

	UPROPERTY()
	class APawn* OwnerPawn;

	UPROPERTY()
	TObjectPtr<class APlayerController> OwnerPlayerController;

	UPROPERTY()
	TObjectPtr<class APlayerCameraManager> OwnerCameraManager;

	UPROPERTY()
	TArray<class UFadeMeshComponent*> FadeMeshesList;

	UPROPERTY()
	TArray<class UFadeMeshComponent*> CurrentFadeMeshesList;
};
