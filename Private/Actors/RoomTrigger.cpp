// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RoomTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModes/MLGameMode.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationInvokerComponent.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"

ARoomTrigger::ARoomTrigger()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
	SetRootComponent(Root);
	CreateWalls();
}

void ARoomTrigger::CreateWalls()
{
	Wall0 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall 0"));
	Wall0->SetupAttachment(Root);
	Wall0->SetGenerateOverlapEvents(true);

	Wall1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall 1"));
	Wall1->SetupAttachment(Root);
	Wall1->SetGenerateOverlapEvents(true);

	Wall2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall 2"));
	Wall2->SetupAttachment(Root);
	Wall2->SetGenerateOverlapEvents(true);

	Wall3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall 3"));
	Wall3->SetupAttachment(Root);
	Wall3->SetGenerateOverlapEvents(true);
}

FVector ARoomTrigger::GetRandomPointInRoom()
{
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	if (!NavigationSystem) return FVector();

	FNavLocation NavLocation;
	NavigationSystem->GetRandomReachablePointInRadius(GetActorLocation(), NavigationInvoker->GetGenerationRadius(), NavLocation);
	return NavLocation.Location + FVector(0.0f, 0.0f, 100.0f);
}

void ARoomTrigger::BeginPlay()
{
	Super::BeginPlay();

	SetupWalls();
}

void ARoomTrigger::SetupWalls()
{
	Walls.Push(Wall0);
	Walls.Push(Wall1);
	Walls.Push(Wall2);
	Walls.Push(Wall3);

	for (auto Wall : Walls)
	{
		if (!Wall) continue;

		Wall->OnComponentBeginOverlap.AddDynamic(this, &ARoomTrigger::OnWallBeginOverlap);
	}
}

void ARoomTrigger::OnWallBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* MLCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!GetWorld() || !MLCharacter) return;

	for (auto Wall : Walls)
	{
		if (Wall)
		{
			Wall->SetCollisionResponseToAllChannels(ECR_Ignore);
		}
	}
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	AMLGameMode* MLGameMode = Cast<AMLGameMode>(GameMode);
	if (!MLGameMode) return;

	SpawnedEnemies = MLGameMode->HandleRoomEntering(this , MLCharacter);
}
