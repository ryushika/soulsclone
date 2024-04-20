#include "Actors/Shopkeeper.h"
#include "Blueprint/UserWidget.h"
#include "../MemoryLakePlayerController.h"

AShopkeeper::AShopkeeper()
{
 	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	ReactTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("React Trigger Box"));
	ReactTriggerBox->SetupAttachment(SceneRoot);

	ReactTriggerBox->SetGenerateOverlapEvents(true);
	ReactTriggerBox->SetNotifyRigidBodyCollision(true);
	ReactTriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ReactTriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ReactTriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void AShopkeeper::BeginPlay()
{
	Super::BeginPlay();

	ReactTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AShopkeeper::OnOverlapBegin);
	ReactTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AShopkeeper::OnOverlapEnd);

	SetActorTickEnabled(false);
}

void AShopkeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (bIsInteracting && bTransitionForwardActive && PlayerCamera)
	//{
	//	if (PlayerCamera->GetComponentLocation().Equals(CameraComponent->GetComponentLocation(), 1.f))
	//	{
	//		bTransitionForwardActive = false;
	//		SetActorTickEnabled(false);
	//	}
	//	else
	//	{
	//		PlayerCamera->SetWorldLocation(FMath::VInterpTo(PlayerCamera->GetComponentLocation(), CameraComponent->GetComponentLocation(), DeltaTime, 1.f));
	//		PlayerCamera->SetWorldRotation(FMath::RInterpTo(PlayerCamera->GetComponentRotation(), CameraComponent->GetComponentRotation(), DeltaTime, 1.f));
	//	}
	//}
	//else if (bTransitionBackActive && PlayerCamera)
	//{
	//	if (PlayerCamera->GetComponentLocation().Equals(OriginalCameraLocation, 1.f))
	//	{
	//		bTransitionBackActive = false;
	//		SetActorTickEnabled(false);
	//	}
	//	else
	//	{
	//		PlayerCamera->SetRelativeLocation(FMath::VInterpTo(PlayerCamera->GetRelativeLocation(), FVector::ZeroVector, DeltaTime, 1.f));
	//		PlayerCamera->SetRelativeRotation(FMath::RInterpTo(PlayerCamera->GetRelativeRotation(), FRotator::ZeroRotator, DeltaTime, 1.f));
	//	}
	//}
}

void AShopkeeper::StopFocus(FShopperData& Shopper)
{
	if (Shopper.ShopWidget)
	{
		Shopper.ShopWidget->RemoveFromParent();
		Shopper.ShopWidget = nullptr;
	}

	AMemoryLakePlayerController* PlayerController = Shopper.PlayerController;

	if (Shopper.PlayerController)
	{
		if (Shopper.Character)
		{
			if (Shopper.Character->ActorHasTag("Shopping") && !Shopper.Character->ActorHasTag("BrowsingUpgrades"))
			{
				ShowWidget(Shopper);
			}

			if (Shopper.Character->UpgradesLeftToSpawn <= 0)
			{
				Shopper.PlayerController->SetShowMouseCursor(false);
				Shopper.PlayerController->SetInputMode(FInputModeGameOnly());
				Shopper.PlayerController->ShowHUDWidgets();
				Shopper.Character->ResumeCursorWidget();
				Shopper.Character = nullptr;
			}
		}
	}
}

void AShopkeeper::StopFocus(APlayerController* PlayerController)
{
	int32 Index = -1;

	for (int32 i = 0; i < Shoppers.Num(); i++)
	{
		if (Shoppers.IsValidIndex(i) && Shoppers[i].PlayerController == PlayerController)
		{
			StopFocus(Shoppers[i]);
			break;
		}
	}
}

void AShopkeeper::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (Interactor)
	{
		if (Interactor->ActorHasTag("BrowsingUpgrades"))
		{
			return;
		}

		int32 Index = -1;
		if (Interactor->ActorHasTag("Shopping"))
		{
			AController* InteractorController = Interactor->GetController();
			Index = GetShopperIndex(InteractorController);
		}

		FShopperData* Shopper{};

		if (Index >= 0)
		{
			Shopper = &Shoppers[Index];
			Shopper->Character = Interactor;
		}
		else
		{
			int32 AddedIndex = Shoppers.AddDefaulted();
			Shopper = &Shoppers[AddedIndex];
			if (Shopper)
			{
				Shopper->Character = Interactor;
				AMemoryLakePlayerController* PlayerController = Cast<AMemoryLakePlayerController>(Shopper->Character->GetController());
				Shopper->PlayerController = PlayerController;
			}
		}

		if (!Shopper)
		{
			return;
		}

		Shopper->Character->StopCursorWidget();

		AMemoryLakePlayerController* PlayerController = Shopper->PlayerController;
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameAndUI());
			PlayerController->SetShowMouseCursor(true);
			PlayerController->HideHUDWidgets();
		}

		HideWidget(*Shopper);

		if (!Shopper->ShopWidget)
		{
			CreateShopWidget(*Shopper);
		}
	}
}

void AShopkeeper::CreateShopWidget(FShopperData& Shopper)
{
	APlayerController* OwningPlayer = Shopper.PlayerController;
	
	if (OwningPlayer && OwningPlayer->IsLocalController())
	{
		if (Shopper.ShopWidget)
		{
		}
		else
		{
			if (ShopWidgetClass)
			{
				Shopper.ShopWidget = CreateShopWidget_Worker(OwningPlayer);
			}
		}
	}
}

void AShopkeeper::ShowWidget(FShopperData& Shopper)
{
	APlayerController* OwningPlayer = Shopper.PlayerController;

	if (OwningPlayer && OwningPlayer->IsLocalController())
	{
		if (Shopper.ControlsWidget)
		{
		}
		else
		{
			if (ControlsWidgetClass)
			{
				Shopper.ControlsWidget = CreateWidget<UUserWidget>(OwningPlayer, ControlsWidgetClass);
				if (Shopper.ControlsWidget)
				{
					Shopper.ControlsWidget->AddToViewport();
				}
			}
		}
	}
}

void AShopkeeper::HideWidget(FShopperData& Shopper)
{
	if (Shopper.ControlsWidget)
	{
		Shopper.ControlsWidget->RemoveFromParent();
		Shopper.ControlsWidget = nullptr;
	}
}

int32 AShopkeeper::GetShopperIndex(AActor* ByActor)
{
	for (int32 i = 0; i < Shoppers.Num(); i++)
	{
		if (Shoppers.IsValidIndex(i))
		{
			if (Shoppers[i].Character == ByActor)
			{
				return i;
			}
		}
	}
	
	return -1;
}

int32 AShopkeeper::GetShopperIndex(AController* ByController)
{
	for (int32 i = 0; i < Shoppers.Num(); i++)
	{
		if (Shoppers.IsValidIndex(i))
		{
			if (Shoppers[i].PlayerController == ByController)
			{
				return i;
			}
		}
	}

	return -1;
}

void AShopkeeper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (OtherActor->ActorHasTag("BrowsingUpgrades"))
		{
			return;
		}

		OtherActor->Tags.Add("Shopping");
		AMemoryLakePlayerController* PlayerController = Cast<AMemoryLakePlayerController>(Cast<APawn>(OtherActor)->GetController());

		FShopperData Shopper;
		Shopper.PlayerController = PlayerController;
		int32 Index = Shoppers.Add(Shopper);

		ShowWidget(Shoppers[Index]);
	}
}

void AShopkeeper::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		OtherActor->Tags.Remove("Shopping");
		
		AController* OtherController = Cast<APawn>(OtherActor)->GetController();
		int32 Index = GetShopperIndex(OtherController);
		
		if (Index >= 0)
		{
			StopFocus(Shoppers[Index]);
			HideWidget(Shoppers[Index]);
			Shoppers.RemoveAt(Index);
		}
	}
}
