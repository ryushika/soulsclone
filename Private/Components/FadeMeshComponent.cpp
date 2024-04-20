
#include "Components/FadeMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

UFadeMeshComponent::UFadeMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFadeMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupFade();
}

void UFadeMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateFade(DeltaTime);
}

void UFadeMeshComponent::BeginFade()
{
	if (OwnerMesh && FadeMaterials.Num() > 0)
	{
		if (DMIFadeMaterials.Num() == 0)
		{
			CreateDMI();
		}

		SwapDefaultMatsForDMI();

		bFadeActive = true;
		SetComponentTickEnabled(true);
	}
}

void UFadeMeshComponent::SwapDefaultMatsForDMI()
{
	for (int8 i = 0; i < DMIFadeMaterials.Num(); i++)
	{
		OwnerMesh->SetMaterial(i, DMIFadeMaterials[i]);
	}
}

void UFadeMeshComponent::CreateDMI()
{
	TArray <UMaterialInterface*> Materials =
		bUseDifferentFadeMaterials ? FadeMaterials : DefaultMaterials;

	for (UMaterialInterface* Material : Materials)
	{
		UMaterialInstanceDynamic* CreatedDMI = UMaterialInstanceDynamic::Create(Material, this);
		DMIFadeMaterials.Add(CreatedDMI);
	}
}

void UFadeMeshComponent::EndFade()
{
	if (OwnerMesh && DefaultMaterials.Num() > 0)
	{
		bFadeActive = false;
		SetComponentTickEnabled(true);
	}
}

void UFadeMeshComponent::SetupFade()
{
	GetOwner()->Tags.Add("CanFade");

	OwnerMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (OwnerMesh)
	{
		DefaultMaterials = OwnerMesh->GetMaterials();
	}
}

void UFadeMeshComponent::UpdateFade(float DeltaTime)
{
	float TargetFade = bFadeActive ? MaxFade : 0.f;
	CurrentFade = FMath::FInterpConstantTo(CurrentFade, TargetFade, DeltaTime, FadeInterpSpeed);

	for (UMaterialInstanceDynamic* DMI : DMIFadeMaterials)
	{
		DMI->SetScalarParameterValue("Fade", CurrentFade);
	}

	StopFadingIfNeeded();
}

void UFadeMeshComponent::StopFadingIfNeeded()
{
	if (bFadeActive)
	{
		if (CurrentFade == MaxFade)
		{
			SetComponentTickEnabled(false);
		}
	}
	else
	{
		if (CurrentFade == 0.f)
		{
			SetComponentTickEnabled(false);
			if (bUseDifferentFadeMaterials)
			{
				ResetMaterials();
			}
		}
	}
}

void UFadeMeshComponent::ResetMaterials()
{
	if (OwnerMesh && DefaultMaterials.Num() > 0)
	{
		for (int8 i = 0; i < DefaultMaterials.Num(); i++)
		{
			OwnerMesh->SetMaterial(i, DefaultMaterials[i]);
		}
	}
}
