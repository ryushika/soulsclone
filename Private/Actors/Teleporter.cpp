// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Teleporter.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "../MemoryLakePlayerController.h"
#include <NiagaraFunctionLibrary.h>
#include "Niagara/Public/NiagaraComponent.h"

ATeleporter::ATeleporter()
{
 	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(GetRootComponent());

	SetActorTickEnabled(false);
}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PairTeleporter)
	{
		PairTeleporter->InitializePair(this);
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATeleporter::OnBoxEndOverlap);
	if (MyCoolFX->IsValid())
	{
		VFXComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MyCoolFX, GetActorLocation() + FVector(0.,0.,150.), GetActorRotation() + FRotator(-90., 0., 0.));
		VFXComp->SetHiddenInGame(true);
	}
}

void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCameraTo_Worker(DeltaTime);
}

void ATeleporter::InitializePair(ATeleporter* OriginalTeleporter)
{
	if (!PairTeleporter || PairTeleporter == OriginalTeleporter)
	{
		PairTeleporter = OriginalTeleporter;
	}
}

FVector ATeleporter::GetSpawnPointLocation() const
{
	return SpawnPoint->GetComponentLocation();
}

FRotator ATeleporter::GetSpawnPointRotation() const
{
	return SpawnPoint->GetComponentRotation();
}

void ATeleporter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IgnoreTargets.Contains(OtherActor))
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			APawn* Pawn = Cast<APawn>(OtherActor);
			if (Pawn)
			{
				APlayerCharacter* PC = Cast<APlayerCharacter>(Pawn);
				if (PC)
				{
					//ShowWidget(PC);
					BeginTeleporting(PC);
				}
			}
		}
	}
}

void ATeleporter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HideWidget();
}

void ATeleporter::MoveCameraTo_Timed(FTeleportTarget& TeleportTarget, const FMoveTargetData& MoveTargetData, float Time)
{
	TeleportTarget.StartLocation = TeleportTarget.Camera->GetActorLocation();
	TeleportTarget.TargetLocation = MoveTargetData.Location;
	TeleportTarget.TargetRotation = MoveTargetData.Rotation;
	TeleportTarget.Alpha = 0.f;

	float Distance = FVector::Distance(TeleportTarget.StartLocation, TeleportTarget.TargetLocation);
	float Speed = (Distance / Time);

	TeleportTarget.InterpSpeed = Speed;

	SetActorTickEnabled(true);
}

void ATeleporter::MoveCameraTo_Speed(FTeleportTarget& TeleportTarget, const FMoveTargetData& MoveTargetData, float Speed)
{
	TeleportTarget.StartLocation = TeleportTarget.Camera->GetActorLocation();
	TeleportTarget.TargetLocation = MoveTargetData.Location;
	TeleportTarget.TargetRotation = MoveTargetData.Rotation;
	TeleportTarget.Alpha = 0.f;
	TeleportTarget.InterpSpeed = Speed;

	SetActorTickEnabled(true);
}

void ATeleporter::MoveCameraTo_Worker(float DeltaTime)
{
	if (CurrentTeleportTargets.Num() == 0)
	{
		SetActorTickEnabled(false);
		return;
	}

	TArray<FTeleportTarget> TargetsToRemove;
	
	for (int32 i = 0; i < CurrentTeleportTargets.Num(); i++)
	{
		if (CurrentTeleportTargets.IsValidIndex(i))
		{
			if (CurrentTeleportTargets[i].Target)
			{
				FTeleportTarget& TeleportTarget = CurrentTeleportTargets[i];

				if (TeleportTarget.bMarkedForDeletion)
				{
					TargetsToRemove.Add(CurrentTeleportTargets[i]);
					continue;
				}

				if (TeleportTarget.Camera)
				{
					if (!(TeleportTarget.Camera->GetActorLocation() - TeleportTarget.TargetLocation).IsNearlyZero(1.f))
					{
						TeleportTarget.Alpha = FMath::FInterpConstantTo(TeleportTarget.Alpha, 1.f, DeltaTime, .06f);

						FVector NewLocation = FMath::Lerp(TeleportTarget.Camera->GetActorLocation(), TeleportTarget.TargetLocation, TeleportTarget.Alpha);

						if (TeleportTarget.TargetRotation != FRotator::ZeroRotator)
						{
							FRotator NewRotation = InterpRotation(TeleportTarget);
							TeleportTarget.Camera->SetActorRotation(NewRotation);
						}

						TeleportTarget.Camera->SetActorLocation(NewLocation);
					}
					else
					{
						SetActorTickEnabled(false);
						TeleportTarget.OnTargetReached.ExecuteIfBound(TeleportTarget);
					}
				}
			}
			else
			{
				TargetsToRemove.Add(CurrentTeleportTargets[i]);
			}
		}
	}

	for (int32 i = 0; i < TargetsToRemove.Num(); i++)
	{
		CurrentTeleportTargets.Remove(TargetsToRemove[i]);
	}
}

FRotator ATeleporter::InterpRotation(FTeleportTarget& TeleportTarget)
{
	float OldPitch = TeleportTarget.Camera->GetActorRotation().Pitch;
	float NewPitch = FMath::Lerp(OldPitch, TeleportTarget.TargetRotation.Pitch, TeleportTarget.Alpha);
	return FRotator(NewPitch, 0.f, 0.f);
}

void ATeleporter::FirstStageComplete(FTeleportTarget& TeleportTarget)
{
	FVector LocalTargetLocation = PairTeleporter->GetSpawnPointLocation();
	FRotator LocalTargetRotation = PairTeleporter->GetSpawnPointRotation();

	FMoveTargetData TeleportData(LocalTargetLocation, LocalTargetRotation);

	TeleportPlayer(TeleportTarget.Target, TeleportData);

	LocalTargetLocation.Z = TeleportTarget.Camera->GetActorLocation().Z;

	FMoveTargetData LocalTargetData(LocalTargetLocation);

	TeleportTarget.OnTargetReached.Unbind();
	TeleportTarget.OnTargetReached.BindUObject(this, &ATeleporter::SecondStageComplete);

	MoveCameraTo_Speed(TeleportTarget, LocalTargetData, SpeedToReachPair);
}

void ATeleporter::SecondStageComplete(FTeleportTarget& TeleportTarget)
{
	FVector LocalTargetLocation = PairTeleporter->GetSpawnPointLocation();
	FRotator LocalTargetRotation = FRotator::ZeroRotator;

	UCameraComponent* LocalCamera = TeleportTarget.Target->GetComponentByClass<UCameraComponent>();
	if (LocalCamera)
	{
		LocalTargetLocation = LocalCamera->GetComponentLocation();
		LocalTargetRotation = LocalCamera->GetComponentRotation();
	}

	FMoveTargetData LocalTargetData(LocalTargetLocation, LocalTargetRotation);

	TeleportTarget.OnTargetReached.Unbind();
	TeleportTarget.OnTargetReached.BindUObject(this, &ATeleporter::ThirdStageComplete);

	MoveCameraTo_Timed(TeleportTarget, LocalTargetData, TimeToGoDown);
}

void ATeleporter::ThirdStageComplete(FTeleportTarget& TeleportTarget)
{
	if (TeleportTarget.Camera)
	{
		TeleportTarget.Camera->Destroy();
		TeleportTarget.Camera = nullptr;
	}

	for (int32 i = 0; i < CurrentTeleportTargets.Num(); i++)
	{
		if (CurrentTeleportTargets[i].Target == TeleportTarget.Target)
		{
			CurrentTeleportTargets[i].bMarkedForDeletion = true;
		}
	}

	if (CurrentTeleportTargets.Num() == 0)
	{
		SetActorTickEnabled(false);
	}
	else
	{
		SetActorTickEnabled(true);
	}

	AMemoryLakePlayerController* PC = Cast<AMemoryLakePlayerController>(TeleportTarget.Target->GetController());
	if (PC)
	{
		PC->EnableInput(PC);
		PC->ShowHUDWidgets();
	}

	PairTeleporter->IgnoreTargets.Remove(TeleportTarget.Target);
	IgnoreTargets.Remove(TeleportTarget.Target);

	TeleportTarget.OnTargetReached.Unbind();
}

void ATeleporter::TeleportPlayer(ACharacter* Player, const FMoveTargetData& TargetData)
{
	if (!Player)
	{
		return;
	}

	FVector LocalTargetLocation = TargetData.Location;
	FRotator LocalTargetRotation = TargetData.Rotation;

	float RandX = FMath::RandRange(-100.f, 100.f);
	float RandY = FMath::RandRange(-100.f, 100.f);
	LocalTargetLocation += FVector(RandX, RandY, 0.f);

	bool bSuccess = Player->TeleportTo(LocalTargetLocation, LocalTargetRotation, false, true);

	if (!bSuccess)
	{
		FMoveTargetData NewTeleportData(LocalTargetLocation, LocalTargetRotation);
		TeleportPlayer(Player, NewTeleportData);
	}
}

TObjectPtr<class UBoxComponent> ATeleporter::GetBoxComponent()
{
	return BoxComponent;
}

void ATeleporter::BeginTeleporting(ACharacter* TeleportTarget)
{
	if (TeleportTarget && PairTeleporter)
	{
		if (ACameraActor* TargetCamera = CreateCameraAndSetView(TeleportTarget))
		{
			FTeleportTarget NewTeleportTarget(TeleportTarget, TargetCamera);
			NewTeleportTarget.StartLocation = TargetCamera->GetActorLocation();

			FVector StartLocation = SpawnPoint->GetComponentLocation() + FVector(0.f, 0.f, ZOffset);
			FRotator TargetRotation = FRotator(-90.f, 0.f, 0.f);

			FMoveTargetData MoveTargetData(StartLocation, TargetRotation);

			MoveCameraTo_Timed(NewTeleportTarget, MoveTargetData, TimeToGoUp);

			NewTeleportTarget.OnTargetReached.Unbind();
			NewTeleportTarget.OnTargetReached.BindUObject(this, &ATeleporter::FirstStageComplete);

			CurrentTeleportTargets.Add(NewTeleportTarget);

			PairTeleporter->IgnoreTargets.Add(TeleportTarget);
			IgnoreTargets.Add(TeleportTarget);

			AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(TeleportTarget->GetController());
			if (MLController)
			{
				TeleportTarget->GetController()->DisableInput(nullptr);
				MLController->HideHUDWidgets();
			}

			HideWidget();
		}
		else
		{
			return;
		}
	}
}

ACameraActor* ATeleporter::CreateCameraAndSetView(ACharacter* TeleportTarget)
{
	ACameraActor* Camera = nullptr;

	if (APlayerController* PC = Cast<APlayerController>(TeleportTarget->GetController()))
	{
		FVector InitialCameraLocation = PC->PlayerCameraManager->GetCameraLocation();
		FRotator InitialCameraRotation = PC->PlayerCameraManager->GetCameraRotation();

		Camera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), InitialCameraLocation, InitialCameraRotation);
		Camera->GetCameraComponent()->bConstrainAspectRatio = false;

		PC->SetViewTargetWithBlend(Camera, 0.f);
	}

	return Camera;
}


void ATeleporter::ShowWidget(APlayerController* OwningPlayer)
{
	if (OwningPlayer && OwningPlayer->IsLocalController())
	{
		if (ControlsWidget)
		{
		}
		else
		{
			if (ControlsWidgetClass)
			{
				ControlsWidget = CreateWidget<UUserWidget>(OwningPlayer, ControlsWidgetClass);
				if (ControlsWidget)
				{
					ControlsWidget->AddToViewport();
				}
			}
		}
	}
}

void ATeleporter::HideWidget()
{
	if (ControlsWidget)
	{
		ControlsWidget->RemoveFromParent();
		ControlsWidget = nullptr;
	}
}
