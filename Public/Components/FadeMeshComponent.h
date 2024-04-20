
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FadeMeshComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORYLAKE_API UFadeMeshComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFadeMeshComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//  Functions

public:
	void BeginFade();
	void EndFade();

protected:

private:
	void SwapDefaultMatsForDMI();
	void CreateDMI();

	void SetupFade();
	void UpdateFade(float DeltaTime);
	void StopFadingIfNeeded();
	void ResetMaterials();

//  Variables

public:

protected:

private:
	UPROPERTY(EditAnywhere)
	bool bUseDifferentFadeMaterials = false;

	UPROPERTY(VisibleAnywhere)
	bool bFadeActive = false;

	UPROPERTY(VisibleAnywhere)
	float CurrentFade = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxFade = 0.95f;

	UPROPERTY(EditAnywhere)
	float FadeInterpSpeed = 3.f;

	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> OwnerMesh;

	UPROPERTY(VisibleAnywhere)
	TArray<class UMaterialInterface*> DefaultMaterials;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseDifferentFadeMaterials"))
	TArray<class UMaterialInterface*> FadeMaterials;

	// DMI stands for Dymamic Material Instance
	UPROPERTY(VisibleAnywhere)
	TArray<class UMaterialInstanceDynamic*> DMIFadeMaterials;
};
